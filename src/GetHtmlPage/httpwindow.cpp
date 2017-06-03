/*******************************************************************
 *
 * Filename: httpwindow.cpp
 * Date:     2012-10-09
 *
 * Description: This module downloads html files
 *
 *
 *******************************************************************/

#include <QtGui>
#include <QtNetwork>

#include "httpwindow.h"

bool HttpWindow::m_supressRedirectionMessageBox = true; // ajn 170423 = false;
// abb

/****************************************************************
 *
 * Function:    HttpWindow()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
HttpWindow::HttpWindow(QWidget *parent)
    : QDialog(parent)
{
    m_httpStatus = HTTP_STATUS_IDLE;
    m_fileNumber = 0;

}


enum HttpFinishStatus_ET
{
    NOT_FINISH,
    FINISH_OK,
    FINISH_ERR
};

HttpFinishStatus_ET m_httpStatus;

/****************************************************************
 *
 * Function:    startRequest()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
void HttpWindow::startRequest(QUrl url, QString fileName, int fileNumber)
{

    // This signals need to download the file
    connect(&qnam, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),this, SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));

    m_fileNumber = fileNumber;
    m_fileName = fileName;

    // Remove old file if exists
    if (QFile::exists(fileName))
        QFile::remove(fileName);

    // Open file that will store html page
    file = new QFile(fileName);
    if (!file->open(QIODevice::WriteOnly))
    {
        QMessageBox::information(this, tr("HTTP"), tr("Unable to open file %1: %2.").arg(fileName).arg(file->errorString()));
        delete file;
        file = 0;
        return;
    }

    // schedule the request
    httpRequestAborted = false;

    // Send request to download file
    reply = qnam.get(QNetworkRequest(url));

    // Direct signals that is received when html file is downloaded to interrupt handlers
    connect(reply, SIGNAL(finished()), this, SLOT(httpFinished()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));

    m_httpStatus = HTTP_STATUS_NOT_FINISH;
}





/****************************************************************
 *
 * Function:    cancelDownload()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
void HttpWindow::cancelDownload()
{
    httpRequestAborted = true;
    reply->abort();
}


/****************************************************************
 *
 * Function:    setSupressRedirectionMessageBox()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
void HttpWindow::setSupressRedirectionMessageBox(bool value)
{
    m_supressRedirectionMessageBox = value;

}

/****************************************************************
 *
 * Function:    getSupressRedirectionMessageBox()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
bool HttpWindow::getSupressRedirectionMessageBox(void)
{
    return m_supressRedirectionMessageBox;

}


/****************************************************************
 *
 * Function:    httpFinished()
 *
 * Description: This slot is invoked when file is completely
 *              downloaded.
 *
 *
 *
 ****************************************************************/
void HttpWindow::httpFinished()
{
    bool supressMsgBox = false;

    // Have user canceld the downloading
    if (httpRequestAborted)
    {

        if (file)
        {
            file->close();
            file->remove();
            delete file;
            file = 0;
        }
        reply->deleteLater();
        m_httpStatus = HTTP_STATUS_FINISH_OK;
        return;
    }

    // All data is received flush and close file
    file->flush();
    file->close();


    // Check server response
    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (reply->error())
    {
        file->remove();

        // Do not show this error when download omx data
        bool supressMsgBox = getSupressRedirectionMessageBox();

        if(supressMsgBox == false)
        {
            QMessageBox::information(this, tr("HTTP"), tr("Download failed: %1.").arg(reply->errorString()));
        }
    }
    else if (!redirectionTarget.isNull())
    {

        QUrl newUrl = url.resolved(redirectionTarget.toUrl());


        supressMsgBox = getSupressRedirectionMessageBox();
        // Do not show this message box when download omx data
        if(supressMsgBox == true)
        {
            url = newUrl;
            reply->deleteLater();
            file->open(QIODevice::WriteOnly);
            file->resize(0);
            startRequest(url,  m_fileName, m_fileNumber);
            return;
        }
        else if (QMessageBox::question(this, tr("HTTP"), tr("Redirect to %1 ?").arg(newUrl.toString()), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            url = newUrl;
            reply->deleteLater();
            file->open(QIODevice::WriteOnly);
            file->resize(0);
            startRequest(url,  m_fileName, m_fileNumber);
            return;
        }
    }
    else
    {
        //QMessageBox::information(this, tr("HTTP"), tr("Download finished"));
        emit sendSignalTextToDlg2(m_fileNumber);
        m_httpStatus = HTTP_STATUS_FINISH_OK;

    }

    reply->deleteLater();
    reply = 0;
    delete file;
    file = 0;
}


/****************************************************************
 *
 * Function:    httpReadyRead()
 *
 * Description:  This slot gets called every time the QNetworkReply
 *               has new data. We read all of its new data and write
 *               it into the file.
 *
 *               That way we use less RAM than when reading it at the
 *               finished() signal of the QNetworkReply
 *
 ****************************************************************/
void HttpWindow::httpReadyRead()
{

    if (file)
        file->write(reply->readAll());
}




/****************************************************************
 *
 * Function:    sslErrors()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
void HttpWindow::sslErrors(QNetworkReply*,const QList<QSslError> &errors)
{
    QString errorString;
    foreach (const QSslError &error, errors)
    {
        if (!errorString.isEmpty())
            errorString += ", ";
        errorString += error.errorString();
    }
    

    if (QMessageBox::warning(this, tr("HTTP"), tr("One or more SSL errors has occurred: %1").arg(errorString),
                             QMessageBox::Ignore | QMessageBox::Abort) == QMessageBox::Ignore)
    {
        reply->ignoreSslErrors();
    }

}
