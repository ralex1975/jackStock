#ifndef IMPORTYAHOOTADATA_H
#define IMPORTYAHOOTADATA_H

#include <QtNetwork/QSslSocket>
#include <QThread>
#include <QtCore>
#include <QFile>
#include "httpwindow.h"

class importYahooTaData : public QThread
{
    Q_OBJECT
    bool m_importInfoIsSet;
    bool m_processSingleStock;
    QString m_filename;

    bool openAssetFile(QString filename, QFile **file, QTextStream **inStream);
    void closeAssetFile(QFile **file, QTextStream **inStream);
    bool retreiveAssetName(QString &stockSymbol, QString &stockName, QTextStream *inStream);
    void parseTaDataOnFile(QString stockName, QString stockSymbol);


public:

    enum StateImportTaData_ET
    {
        STATE_IDLE,
        STATE_OPEN_ASSET_FILE,
        STATE_RETRIEVE_ASSET_NAME,
        STATE_WAIT_ON_SERVER_DATA,
        STATE_PARSE_DATA
    };


    QString m_stockSymbol;
    QString m_stockName;
    StateImportTaData_ET m_stateImportTaData;

    enum StatusRespToUserImportTaData_ET
    {
        ERR_FAIL_USER_INPUT_DATA_MISSING,
        ERR_FAIL_OPEN_SYM_FILE,
        ERR_FAIL_GET_DATA_FROM_SERVER,
        ERR_OK
    };


    explicit importYahooTaData(QObject *parent = 0);
    void setImportInfo(QString filename);
    void setImportInfoSingleStock(QString stockName, QString stockSymbol);
    void run();

    
signals:
   void emitImportYahooTaDataIsFinish(int);
   void emitRequestYahooTaDataFromServer();
   void emitImportSingelStockDataIsFinish(int);
    
public slots:

    
};

#endif // IMPORTYAHOOTADATA_H
