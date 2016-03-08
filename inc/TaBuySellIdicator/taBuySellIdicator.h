#ifndef TABUYSELLIDICATOR_H
#define TABUYSELLIDICATOR_H

#include "dbHndl.h"

class TaBuySellIdicator
{
    // Store one stock with data
    CYahooStockPlotUtil::StockData_ST m_stockPrice;
    CYahooStockPlotUtil::StockData_ST m_avgShortData;
    CYahooStockPlotUtil::StockData_ST m_avgMidData;
    CYahooStockPlotUtil::StockData_ST m_avgLongData;


    bool buyAvg20Above50(QString stockSymbol);




public:
    struct SellSignalMovingAvgST
    {
        bool priceDipBelowShortAvg;
        bool priceDipBelowMidAvg;
        bool priceDipBelowLongAvg;

        bool ShortAvgDipBelowMidAvg;
        bool ShortAvgDipBelowLongAvg;

        bool MidAvgDipBelowLongAvg;
    };


struct BuySignalMovingAvgST
{
    bool priceRiseAboveShortAvg;
    bool priceRiseAboveMidAvg;
    bool priceRiseAboveLongAvg;

    bool ShortAvgRiseAboveMidAvg;
    bool ShortAvgRiseAboveLongAvg;

    bool MidAvgRiseAboveLongAvg;
};



    TaBuySellIdicator();
    bool buySignalAvgShortAboveMid(QString stockSymbol);
    bool sellSignalAvgShortAvgMidAvgLongAbovePrice(QString stockSymbol);
    void sellSignalFastAvgCrossSlowAvg(double priseVal1,
                                       double priseVal2,
                                       double avgFastVal1,
                                      double avgFastVal2,
                                      double avgSlowVal1,
                                      double avgSlowVal2,
                                      bool &selSignalFastCrossSlow);

    void sellSignalFastAvgCrossSlowAvg(double avgFastVal1,
                                       double avgFastVal2,
                                       double avgSlowVal1,
                                       double avgSlowVal2,
                                       bool &sellSignalFastCrossSlow);


    QString convAvgSellSignalToNumber(SellSignalMovingAvgST &sellSignals);
    bool getBuySignals(QString stockSymbol);
    void clearStockAndIndicatorMem(CYahooStockPlotUtil::StockData_ST &stockData);

    bool getAvgBuySellSignals(QString stockSymbol,
                              SellSignalMovingAvgST &sellSignals,
                              BuySignalMovingAvgST  &buySignals);

    QString convAvgBuySignalToNumber(BuySignalMovingAvgST &buySignals);
    void resetBuySignals(BuySignalMovingAvgST &buySignals);
    void resetSellSignals(SellSignalMovingAvgST &sellSignals);
    void buySignalFastAvgCrossSlowAvg(double priseVal1,
                                      double priseVal2,
                                      double avgFastVal1,
                                      double avgFastVal2,
                                      double avgSlowVal1,
                                      double avgSlowVal2,
                                      bool &buySignalFastCrossSlow);

    void buySignalFastAvgCrossSlowAvg(double avgFastVal1,
                                      double avgFastVal2,
                                      double avgSlowVal1,
                                      double avgSlowVal2,
                                      bool &buySignalFastCrossSlow);




};

#endif // TABUYSELLIDICATOR_H
