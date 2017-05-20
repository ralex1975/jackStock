#ifndef HTTPDOWNLOADER_H
#define HTTPDOWNLOADER_H

#include <QObject>
#include <QHttp>
//#include <QSslSocket>
#include <QFile>
#include <QDebug>
#include <QtCore>
#include <QtGui>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QNetworkReply>


class HttpDownloader : public QObject
{
    Q_OBJECT
public:
    void downloadFile(QString urlStr, QString filenameStr);
    explicit HttpDownloader(QObject *parent = 0);
    void doDownload(QString host, QString page);
    QHttp *m_qHttp;
    //QSslSocket *m_qHttp;
    void doPost(QString host, QString page);

    
signals:
    
public slots:
    void requestFinished ( int id, bool error );
    void responseHeaderReceived ( const QHttpResponseHeader & resp );
    void stateChanged ( int state );
    void requestFinished(QNetworkReply *reply);
    void readResponseHeader(const QHttpResponseHeader &responseHeader);

    
};

#endif // HTTPDOWNLOADER_H
