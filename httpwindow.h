/*******************************************************************
 *
 * Filename: httpwindow.h
 * Date:     2012-10-09
 *
 * Description: This module downloads html files
 *
 *
 *******************************************************************/

#ifndef HTTPWINDOW_H
#define HTTPWINDOW_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QUrl>

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QFile;
class QLabel;
class QLineEdit;
class QProgressDialog;
class QPushButton;
class QSslError;
class QAuthenticator;
class QNetworkReply;
QT_END_NAMESPACE



class HttpWindow : public QDialog
{
    Q_OBJECT


public:
    enum HttpFinishStatus_ET
    {
        HTTP_STATUS_IDLE,
        HTTP_STATUS_NOT_FINISH,
        HTTP_STATUS_FINISH_OK,
        HTTP_STATUS_FINISH_ERR

    };

    HttpFinishStatus_ET m_httpStatus;
     int m_fileNumber;
    HttpWindow(QWidget *parent = 0);
    void startRequest(QUrl url, QString fileName, int fileNumber);
    void cancelDownload();

private slots:
    void httpFinished();
    void httpReadyRead();
    void sslErrors(QNetworkReply*,const QList<QSslError> &errors);

signals:
    void sendSignalTextToDlg2(int);


private:
    QUrl url;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    QFile *file;
    int httpGetId;
    bool httpRequestAborted;
    QString m_fileName;
};

#endif
