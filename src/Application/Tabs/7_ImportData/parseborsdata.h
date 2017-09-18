#ifndef PARSEBORSDATA_H
#define PARSEBORSDATA_H

#include <QString>


class ParseBorsData
{
    enum MainStateET
    {
        MSTATE_YEAR,
        MSTATE_SOLIDITY,
        MSTATE_REVENUE_PER_SHARE,
        MSTATE_EQUITY_PER_SHARE,
        MSTATE_EARNINGS_PER_SHARE,
        MSTATE_DIVIDEND
    };


    struct StockDataST
    {
        QString stockSymbol;
        QString stockName;
        QString htmlFilename;
    };

    struct StockParseDataST
    {
        QString year;
        QString solidity;
        QString revenuePerShare;
        QString equityPerShare;
        QString earningPerShare;
        QString dividend;
    };

    #define PARSE_DATA_ARR_SIZE 100
    StockParseDataST m_parseDataArr[PARSE_DATA_ARR_SIZE];
    int m_nofParseArrData;
    MainStateET m_mainState;

public:
    ParseBorsData();
    bool helpParser(QString &result, QRegExp rx, MainStateET mainState, QString dbgStr, StockParseDataST *parseDataArr, int &nofParseArrData);
    bool cleanupParsedProcentValue(QString &value, QString str);
    bool cleanupParsedRealValue(QString &value, QString str);

    bool addRevenuePerShare(StockParseDataST *parseDataArr,
                                            int nofParseArrData,
                                            QString stockSymbol,
                                            QString stockName);


    bool addDividendPerShare(StockParseDataST *parseDataArr,
                                            int nofParseArrData,
                                            QString stockSymbol,
                                            QString stockName);

    bool addEarningsPerShare(StockParseDataST *parseDataArr,
                             int nofParseArrData,
                             QString stockSymbol,
                             QString stockName);


    bool addSolidityPerShare(StockParseDataST *parseDataArr,
                             int nofParseArrData,
                             QString stockSymbol,
                             QString stockName);

    bool addEquityPerShare(StockParseDataST *parseDataArr,
                           int nofParseArrData,
                           QString stockSymbol,
                           QString stockName);



    bool readStockSymbolsFile(QString filename);
    bool readParseHtmlFile(QString filename,
                           StockParseDataST *parseDataArr,
                           int &nofParseArrData);
};

#endif // PARSEBORSDATA_H
