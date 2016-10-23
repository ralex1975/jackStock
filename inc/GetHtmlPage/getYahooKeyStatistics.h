#ifndef GET_YAHOO_KEY_STATISTICS_H
#define GET_YAHOO_KEY_STATISTICS_H

#include "httpwindow.h"
#include <QObject>
#include <QUrl>
#include <QTimer>
#include "stockPlotUtil.h"
#include "dbHndl.h"

#define YAHOO_KEY_STATISTICS_ARR_SIZE 58



class GetYahooKeyStatistics : public QObject
{
    Q_OBJECT

    enum YahooKeyStatEt
    {
     YKS_STATE_READ_FILE,
     YKS_STATE_FIND_TOKEN,
     YKS_STATE_FIND_DATA_TOKEN,
    };


    YahooKeyStatEt m_yksState;



    struct inputKeyStatisticsST
    {
        QString stockSymbol;
    };

    HttpWindow m_hw1;
    bool m_sendNextReq;
    QTimer *m_timeoutTimer;
    bool m_waitOnServerResp;
    YahooKeyStatisticsST m_interimStorage;




public:

    static const QString yahooKeyStatisticsArr[YAHOO_KEY_STATISTICS_ARR_SIZE];

    explicit GetYahooKeyStatistics(QObject *parent = 0);
    ~GetYahooKeyStatistics();

    bool parseYahooKeyStatistics(QString filename, QString stockSymbol);
    void getYahooKeyStatisticsInDb(QString stockListName, int stockListId);


private:

    QVector <CStockPlotUtil::StockData_ST> m_stockArr;
    int m_stockArrIndex;
    void reqNextYahooKeyStatisticsFile(QString stockSymbol);
    //void slotReqNextCompanyData();
    void interimStorageYahooKeyStatistics(QString data, YahooKeyStatisticsET i, YahooKeyStatisticsST &interimStorage);
    void resetYahooKeyStatistics(YahooKeyStatisticsST &interimStorage);






signals:

public slots:
    void slotHtmlPageIsRecv(int number);


private slots:
    void slotReqNextCompanyData();


};

#endif // GETHTMLPAGE_H
