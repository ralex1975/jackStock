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

        bool priceContBelowShortAvg;
        bool priceContBelowMidAvg;
        bool priceContBelowLongAvg;

        bool priceContBelowMidLongAvg;

        // Very strong sell signal
        bool priceContBelowShortMidLongAvg;
    };


struct BuySignalMovingAvgST
{
    bool priceRiseAboveShortAvg;
    bool priceRiseAboveMidAvg;
    bool priceRiseAboveLongAvg;

    bool ShortAvgRiseAboveMidAvg;
    bool ShortAvgRiseAboveLongAvg;

    bool MidAvgRiseAboveLongAvg;

    bool priceContAboveShortAvg;
    bool priceContAboveMidAvg;
    bool priceContAboveLongAvg;

    bool priceContAboveMidLongAvg;

    // Very strong sell signal
    bool priceContAboveShortMidLongAvg;
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

    QString convAvgSellSignalToNumber(SellSignalMovingAvgST &sellSignals);
    bool getBuySignals(QString stockSymbol);
    void clearStockAndIndicatorMem(CYahooStockPlotUtil::StockData_ST &stockData);

    bool getAvgBuySellSignals(QString stockSymbol,
                                           SellSignalMovingAvgST &sellSignals,
                                           BuySignalMovingAvgST  &buySignals);

    QString convAvgBuySignalToNumber(BuySignalMovingAvgST &buySignals);
    bool resetBuySignals(BuySignalMovingAvgST &buySignals);
    bool resetSellSignals(SellSignalMovingAvgST &sellSignals);
    void buySignalFastAvgCrossSlowAvg(double priseVal1,
                                      double priseVal2,
                                      double avgFastVal1,
                                      double avgFastVal2,
                                      double avgSlowVal1,
                                      double avgSlowVal2,
                                      bool &buySignalFastCrossSlow);



};

#endif // TABUYSELLIDICATOR_H
