#ifndef TABUYSELLIDICATOR_H
#define TABUYSELLIDICATOR_H

#include "dbHndl.h"
#include <stdlib.h>
#include <stdio.h>


class TaBuySellIdicator
{
    FILE *m_file;

    // Store one stock with data
    CYahooStockPlotUtil::StockData_ST m_stockPrice;
    CYahooStockPlotUtil::StockData_ST m_avgShortData;
    CYahooStockPlotUtil::StockData_ST m_avgMidData;
    CYahooStockPlotUtil::StockData_ST m_avgLongData;

    CYahooStockPlotUtil::StockData_ST m_macdData;
    CYahooStockPlotUtil::StockData_ST m_rsiData;
    CYahooStockPlotUtil::StockData_ST m_stochasticsData;

    bool buyAvg20Above50(QString stockSymbol);


public:
    struct SellSignalMomentumST
    {
        bool stochasticsBelow80;
        bool macdBelowZero;
        bool rsiBelow70;
    };

    struct SellSignalMovingAvgST
    {
        bool priceDipBelowShortAvg;
        bool priceDipBelowMidAvg;
        bool priceDipBelowLongAvg;

        bool ShortAvgDipBelowMidAvg;
        bool ShortAvgDipBelowLongAvg;

        bool MidAvgDipBelowLongAvg;
    };

    struct BuySignalMomentumST
    {
        bool stochasticsAbove20;
        bool macdAboveZero;
        bool rsiAbove30;
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

    QString convMomentumSellSignalToNumber(BuySignalMomentumST &buySignals);
    QString convMomentumSellSignalToNumber(SellSignalMomentumST &sellSignals);


    void sellMomentumSignal(double dataVal1,
                            double dataVal2,
                            double level,
                            bool &sellSignal);

    void buyMomentumSignal(double dataVal1,
                            double dataVal2,
                            double level,
                            bool &buySignal);



    bool getAvgBuySellSignals(QString stockSymbol,
                              SellSignalMovingAvgST &sellSignals,
                              BuySignalMovingAvgST  &buySignals,
                              SellSignalMomentumST &sellMomentumSignals,
                              BuySignalMomentumST &buyMomentumSignals);

    QString convAvgBuySignalToNumber(BuySignalMovingAvgST &buySignals);
    void resetBuySignals(BuySignalMovingAvgST &buySignals);
    void resetSellSignals(SellSignalMovingAvgST &sellSignals);
    void resetBuySignals(BuySignalMomentumST &buySignals);
    void resetSellSignals(SellSignalMomentumST &sellSignals);

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
