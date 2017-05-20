/*******************************************************************
 *
 * Filename: httpdownloader.cpp
 * Date:     2012-07-08
 *
 * Description: Download file from website
 *
 *
 *******************************************************************/


#include "httpdownloader.h"
#include "common.h"
 #include <QNetworkAccessManager>

/****************************************************************
 *
 * Function:    ()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
HttpDownloader::HttpDownloader(QObject *parent) : QObject(parent)
{


}


/****************************************************************
 *
 * Function:    ()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
void HttpDownloader::doDownload(QString host, QString page)
{

    m_qHttp = new QHttp;

    connect(m_qHttp, SIGNAL(stateChanged(int)), this, SLOT(stateChanged(int)));
    connect(m_qHttp, SIGNAL(responseHeaderReceived(QHttpResponseHeader)), this, SLOT(responseHeaderReceived(QHttpResponseHeader)));
    connect(m_qHttp, SIGNAL(requestFinished(int,bool)), this, SLOT(requestFinished(int,bool)));
    connect(m_qHttp, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)), this, SLOT(readResponseHeader(const QHttpResponseHeader &)));

    if(!m_qHttp)
    {
        QMessageBox::critical(NULL, tr("Error"), tr("Fail to create QHttp object"));
        return;
    }


    m_qHttp->setHost(host, QHttp::ConnectionModeHttps, 443);
    m_qHttp->get(page);
    QMessageBox::information(NULL, tr("Application Name"), page );

}


/****************************************************************
 *
 * Function:    ()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
void  HttpDownloader::readResponseHeader(const QHttpResponseHeader &responseHeader)
 {
     QString location;
     //bool httpRequestAborted;
     int statusCode = responseHeader.statusCode();

     //QMessageBox::information(NULL, tr("Application Name"), tr("Hej") );
     QMessageBox::information(NULL, tr("Application Name"), responseHeader.toString() );



     QRegExp rx("\?$");

     switch(statusCode)
     {
    case 200:
    case 301:
    case 303:
    case 307:
        break;

    case 302:
     location = responseHeader.value("Location");
     QMessageBox::information(NULL, tr("Application Name"), responseHeader.toString() );
     // Remove trailing "?"
     location.remove(rx.indexIn(location), 1);
     //httpRequestAborted = true;
     m_qHttp->abort();
     break;

    default:
     qDebug() << QString(trUtf8("Download failed: %1.").arg(responseHeader.reasonPhrase()));
     //httpRequestAborted = true;
     m_qHttp->abort();
     }
 }


/****************************************************************
 *
 * Function:    ()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
void HttpDownloader::requestFinished (int id, bool error)
{

    id = id;

    QString err_Str = "No error";
    if( error )
    {
        err_Str = m_qHttp->errorString();
        QMessageBox::information(NULL, tr("Unconnected"), err_Str);
    }
    else
    {
        #if 0
        qDebug() << "OK";
         QFile *file = new QFile("C:/AkeHttpTest.html");
         if(file->open(QFile::Append))
         {
             qDebug("file->write(m_qHttp->readAll())\n");
             file->write(m_qHttp->readAll());
             file->flush();
             file->close();
         }
         delete file;
        #endif
    }
}


/****************************************************************
 *
 * Function:    ()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
void HttpDownloader::responseHeaderReceived ( const QHttpResponseHeader & resp)
{
    qDebug() << "Size" << resp.contentLength();
    qDebug() << "Type" << resp.contentLength();
    qDebug() << "Status" << resp.statusCode();
}


/****************************************************************
 *
 * Function:    ()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
void HttpDownloader::stateChanged(int state)
{
    switch(state)
    {
    case QHttp::Unconnected:
        qDebug("Unconnected\n");
        //QMessageBox::information(NULL, tr("Unconnected"), tr("Unconnected"));
        break;
    case QHttp::HostLookup:
         qDebug("HostLookup\n");
          //QMessageBox::information(NULL, tr("HostLookup"), tr("HostLookup"));
        break;
    case QHttp::Connecting:
         qDebug("Connecting\n");
         //QMessageBox::information(NULL, tr("Connecting"), tr("Connecting"));
        break;
    case QHttp::Sending:
         qDebug("Sending\n");
         //QMessageBox::information(NULL, tr("Sending"), tr("Sending"));
        break;
    case QHttp::Reading:
         qDebug("Reading\n");
         //QMessageBox::information(NULL, tr("Reading"), tr("Reading"));
        break;
    case QHttp::Connected:
         qDebug("Connected\n");
         //QMessageBox::information(NULL, tr("Connected"), tr("Connected"));
        break;
    case QHttp::Closing:
         qDebug("Closing\n");
         //QMessageBox::information(NULL, tr("Closing"), tr("Closing"));
        break;
    }

}


/****************************************************************
 *
 * Function:    ()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
void HttpDownloader::doPost(QString host, QString page)
{
    /*
        Creates the network access manager, request and reply
    */

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    QNetworkReply *reply = NULL;

     /*
       Gets the URL from the text field and sets it to the request
       Sets the request headers for authentication and content type
       use the QSslConfiguration for enabling HTTPS
     */

    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    config.setProtocol(QSsl::TlsV1);
    request.setSslConfiguration(config);

    request.setUrl(QUrl(host));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/html, application/xhtml+xml, */*");


 /*
   Connects the transmission finished signal from the manager, to the
   finished request slot from this class
  */

    connect(manager, SIGNAL(finished(QNetworkReply*)), SLOT(requestFinished(QNetworkReply*)));


    /*
        Trigger the POST request
    */
    reply = manager->post(request, page.toAscii());
    QMessageBox::information(NULL, tr("Closing"), page);
    reply->ignoreSslErrors();
}


 /*
  Assigns the response downloaded from the HTTP request
  to the text area
 */

/****************************************************************
 *
 * Function:    ()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
void HttpDownloader::requestFinished(QNetworkReply *reply)
{


 /*
   If there was an error in the reply, display the error string
   in the text area
  */
    if (reply->error() > 0)
        QMessageBox::information(NULL, tr("Request finish: Error 1"), reply->errorString());
    else
    {
#if 0
        QFile *file = new QFile("C:/Ake2HttpTest.html");
        if(file->open(QFile::Append))
        {
             qDebug("file->write(m_qHttp->readAll())\n");
             file->write(reply->readAll());
             file->flush();
             file->close();
        }
        delete file;
#endif
    }
}

