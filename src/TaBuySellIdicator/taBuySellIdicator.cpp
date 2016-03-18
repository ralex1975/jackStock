#include "inc/TaBuySellIdicator/taBuySellIdicator.h"
#include "inc/TaBuySellIdicator/taBuySellTask.h"
#include "yahooStockPlotUtil.h"
#include "util.h"
#include "QDebug"


/****************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
TaBuySellIdicator::TaBuySellIdicator()
{
   // m_taBuySellTask = new taBuySellTask;
   // m_taBuySellTask->start(QThread::NormalPriority);
}


/****************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
bool TaBuySellIdicator::getBuySignals(QString stockSymbol)
{
    CUtil cu;
    bool res = false;
    CDbHndl db;
    int intMonth = -12;
    int arrLen;
    QString endDate;
    QString startDate;
    CYahooStockPlotUtil::PlotData_ST dummyQwtStockPlotData1;
    CYahooStockPlotUtil::PlotData_ST dummyQwtStockPlotData2;
    double avg20Val;
    double avg50Val;


    clearStockAndIndicatorMem(m_avgShortData);
    clearStockAndIndicatorMem(m_avgMidData);


    cu.getCurrentDate(endDate);

    if(false == cu.addMonth(endDate, startDate, intMonth))
    {
        return false;
    }


    db.getYahooTaMovingAverage(stockSymbol, startDate, endDate, 20, dummyQwtStockPlotData1, m_avgShortData);
    db.getYahooTaMovingAverage(stockSymbol, startDate, endDate, 50, dummyQwtStockPlotData2, m_avgMidData);

    arrLen = m_avgMidData.data.x.size();

    if(arrLen < 50 )
    {
        return false;
    }


    if(arrLen > 15)
    {
        avg20Val = m_avgShortData.data.indicator1.at(arrLen-15);
        avg50Val = m_avgMidData.data.indicator1.at(arrLen-15);

        if(avg20Val < avg50Val)
        {
            avg20Val = m_avgShortData.data.indicator1.at(arrLen-1);
            avg50Val = m_avgMidData.data.indicator1.at(arrLen-1);

            if(avg20Val > avg50Val)
            {
                return true;
            }

        }
    }
    else
    {
        res = false;
    }


    return res;
}



/****************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
QString TaBuySellIdicator::convMomentumSellSignalToNumber(SellSignalMomentumST &sellSignals)
{
    QString sellSignalNo = "0";

    if(sellSignals.macdBelowZero == true)
    {
        sellSignalNo = "1 MBZ";
    }

    if(sellSignals.stochasticsBelow80 == true)
    {
        sellSignalNo = "2 SB80";
    }

    if(sellSignals.rsiBelow70 == true)
    {
        sellSignalNo = "3 RB70";
    }

    return sellSignalNo;
}



/****************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
QString TaBuySellIdicator::convMomentumSellSignalToNumber(BuySignalMomentumST &buySignals)
{
    QString buySignalNo = "0";

    if(buySignals.macdAboveZero == true)
    {
        buySignalNo = "1 MAZ";
    }

    if(buySignals.stochasticsAbove20 == true)
    {
        buySignalNo = "2 SA20";
    }

    if(buySignals.rsiAbove30 == true)
    {
        buySignalNo = " 3 RA30";
    }


    return buySignalNo;
}







/****************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
QString TaBuySellIdicator::convAvgSellSignalToNumber(SellSignalMovingAvgST &sellSignals)
{
    QString sellSignalNo = "0";

    if(sellSignals.priceDipBelowShortAvg == true)
    {
        sellSignalNo = "1 PBSA";
    }

    if(sellSignals.priceDipBelowMidAvg == true)
    {
        sellSignalNo = "2 PBMA";
    }


    if(sellSignals.ShortAvgDipBelowMidAvg == true)
    {
        sellSignalNo = "3 SABMA";
    }

    if(sellSignals.priceDipBelowLongAvg == true)
    {
        sellSignalNo = "4 PBLA";
    }

    if(sellSignals.ShortAvgDipBelowLongAvg == true)
    {
        sellSignalNo = "5 SABLA";
    }

    if(sellSignals.MidAvgDipBelowLongAvg == true)
    {
        sellSignalNo = "6 MABLA";
    }


    return sellSignalNo;
}



/****************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
QString TaBuySellIdicator::convAvgBuySignalToNumber(BuySignalMovingAvgST &buySignals)
{
    QString buySignalNo = "0";

    if(buySignals.priceRiseAboveShortAvg == true)
    {
        buySignalNo = "1 PASA";
    }

    if(buySignals.priceRiseAboveMidAvg == true)
    {
        buySignalNo = "2 PAMA";
    }


    if(buySignals.ShortAvgRiseAboveMidAvg == true)
    {
        buySignalNo = "3 SAAMA";
    }

    if(buySignals.priceRiseAboveLongAvg == true)
    {
        buySignalNo = "4 PALA";
    }


    if(buySignals.ShortAvgRiseAboveLongAvg == true)
    {
        buySignalNo = "5 SAALA";
    }

    if(buySignals.MidAvgRiseAboveLongAvg == true)
    {
        buySignalNo = "6 MAALA";
    }


    return buySignalNo;
}






/****************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
void TaBuySellIdicator::resetSellSignals(SellSignalMomentumST &sellSignals)
{
    // Reset status
    sellSignals.stochasticsBelow80 = false;
    sellSignals.rsiBelow70 = false;
    sellSignals.macdBelowZero = false;
}


/****************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
void TaBuySellIdicator::resetBuySignals(BuySignalMomentumST &buySignals)
{
    // Reset status
    buySignals.stochasticsAbove20 = false;
    buySignals.rsiAbove30 = false;
    buySignals.macdAboveZero = false;
}


/****************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
void TaBuySellIdicator::resetSellSignals(SellSignalMovingAvgST &sellSignals)
{
    // Reset status
    sellSignals.ShortAvgDipBelowLongAvg = false;
    sellSignals.ShortAvgDipBelowMidAvg = false;

    sellSignals.MidAvgDipBelowLongAvg = false;

    sellSignals.priceDipBelowLongAvg = false;
    sellSignals.priceDipBelowMidAvg = false;
    sellSignals.priceDipBelowShortAvg = false;
}




/****************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
void TaBuySellIdicator::resetBuySignals(BuySignalMovingAvgST &buySignals)
{
    // Reset status
    buySignals.ShortAvgRiseAboveLongAvg = false;
    buySignals.ShortAvgRiseAboveMidAvg = false;

    buySignals.MidAvgRiseAboveLongAvg = false;

    buySignals.priceRiseAboveLongAvg = false;
    buySignals.priceRiseAboveMidAvg = false;
    buySignals.priceRiseAboveShortAvg = false;

}





/****************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
bool TaBuySellIdicator::getAvgBuySellSignals(QString stockSymbol,
                                             SellSignalMovingAvgST &sellSignals,
                                             BuySignalMovingAvgST  &buySignals,
                                             SellSignalMomentumST &sellMomentumSignals,
                                             BuySignalMomentumST &buyMomentumSignals)
{
    CUtil cu;
    CDbHndl db;
    int intMonth = -10;
    int arrLenAvgShort;
    int arrLenAvgMid;
    int arrLenAvgLong;
    int arrLenStockPrice;
    int arrLenMacd;
    int arrLenRsi;
    // int arrLenStochastics;

    QString endDate;
    QString startDate;
    CYahooStockPlotUtil::PlotData_ST dummyQwtStockPlotData1;

    double stockVal1;
    double avgShortVal1;
    double avgMidVal1;
    double avgLongVal1;
    double macdVal1;
    double rsiVal1;

    double stockVal2;
    double avgShortVal2;
    double avgMidVal2;
    double avgLongVal2;
    double macdVal2;
    double rsiVal2;
   // double stochasticsVal2;

    // double stochasticsMin;
    // double stochasticsMax;

    double level;

#if 0
    m_file = fopen("debugSell.txt", "a+");
    if(!m_file)
    {
        QMessageBox::information(NULL, QString::fromUtf8("File error"), QString::fromUtf8("Fail to open file"));
        return false;
    }
#endif

    // Array with x, y data
    clearStockAndIndicatorMem(m_avgShortData);
    clearStockAndIndicatorMem(m_avgMidData);
    clearStockAndIndicatorMem(m_avgLongData);
    clearStockAndIndicatorMem(m_stockPrice);
    clearStockAndIndicatorMem(m_macdData);
    clearStockAndIndicatorMem(m_rsiData);
    // clearStockAndIndicatorMem(m_stochasticsData);



    // Reset Sell buy flags
    resetSellSignals(sellSignals);
    resetBuySignals(buySignals);
    resetBuySignals(buyMomentumSignals);
    resetSellSignals(sellMomentumSignals);


    // Calc start stop date
    cu.createStartEndDates(startDate, endDate, intMonth);

    // Gather stock price
    db.getYahooTaPriceData(stockSymbol, startDate, endDate, dummyQwtStockPlotData1, m_stockPrice);

    // Gather moving averages
    db.getYahooTaMovingAverage(stockSymbol, startDate, endDate, 20, dummyQwtStockPlotData1, m_avgShortData);
    db.getYahooTaMovingAverage(stockSymbol, startDate, endDate, 50, dummyQwtStockPlotData1, m_avgMidData);
    db.getYahooTaMovingAverage(stockSymbol, startDate, endDate, 150, dummyQwtStockPlotData1, m_avgLongData);

    // Gather MACD
    db.getYahooTaMacd(stockSymbol, startDate, endDate, 26, 12, 9, dummyQwtStockPlotData1, m_macdData);
    db.getYahooTaRsi(stockSymbol, startDate, endDate, 14, dummyQwtStockPlotData1, m_rsiData);
    // db.getYahooTaStochastics(stockSymbol, startDate, endDate,  9, 3, dummyQwtStockPlotData1, m_stochasticsData);


    CMyTaskQueData data;

    data.stockSymbol = stockSymbol;
    m_taTaskInputQue.addDataLast(data);


    // Nof of data stored in each array
    arrLenStockPrice    = m_stockPrice.data.x.size();
    arrLenAvgShort      = m_avgShortData.data.indicator1.size();
    arrLenAvgMid        = m_avgMidData.data.indicator1.size();
    arrLenAvgLong       = m_avgLongData.data.indicator1.size();
    arrLenMacd          = m_macdData.data.indicator3.size();
    arrLenRsi           = m_rsiData.data.indicator1.size();
    //arrLenStochastics   = m_stochasticsData.data.indicator2.size();


    // To little data to create sell/buy signal?
    if((arrLenStockPrice    < 15)  ||
       (arrLenAvgShort      < 15)  ||
       (arrLenAvgMid        < 15)  ||
       (arrLenAvgLong       < 15)  ||
       (arrLenRsi           < 15) /* ||
       (arrLenStochastics   < 15)*/  ||
       (arrLenMacd          < 15))
    {
        return false;
    }



    //=====================================================================
    // Get start stop data value
    //=====================================================================
    stockVal1 = m_stockPrice.data.y.at(arrLenStockPrice - 10);
    stockVal2 = m_stockPrice.data.y.at(arrLenStockPrice - 1);;

    avgShortVal1 = m_avgShortData.data.indicator1.at(arrLenAvgShort-10);
    avgShortVal2 = m_avgShortData.data.indicator1.at(arrLenAvgShort-1);

    avgMidVal1 = m_avgMidData.data.indicator1.at(arrLenAvgMid - 10);
    avgMidVal2 = m_avgMidData.data.indicator1.at(arrLenAvgMid - 1);

    avgLongVal1 = m_avgLongData.data.indicator1.at(arrLenAvgLong - 10);
    avgLongVal2 = m_avgLongData.data.indicator1.at(arrLenAvgLong - 1);

    macdVal1 = m_macdData.data.indicator3.at(arrLenMacd  - 10);
    macdVal2 = m_macdData.data.indicator3.at(arrLenMacd - 1);

    rsiVal1 = m_rsiData.data.indicator1.at(arrLenRsi  - 10);
    rsiVal2 = m_rsiData.data.indicator1.at(arrLenRsi - 1);

    // stochasticsVal2 = m_stochasticsData.data.indicator2.at(arrLenStochastics  - 1);
    // stochasticsMin = m_stochasticsData.data.indicator2.at(arrLenStochastics - 10);
    // stochasticsMax = stochasticsMin;
#if 0
    double data;
    for(int k = (arrLenStochastics - 9); k < (arrLenStochastics); k++)
    {
        data = m_stochasticsData.data.indicator2.at(k);
        if(data < stochasticsMin)
        {
            stochasticsMin = data;
        }
        else if(data < stochasticsMax)
        {
            stochasticsMax = data;
        }
    }

    if(stochasticsMax > 80 && stochasticsMin < 80 && stochasticsVal2 < 80)
    {
        sellMomentumSignals.stochasticsBelow80 = true;
    }
    else if(stochasticsMin < 20 && stochasticsMin > 20 && stochasticsVal2 > 20)
    {
        buyMomentumSignals.stochasticsAbove20 = true;
    }
#endif



    //==================================================================================
    // Sell signal (Momentum)
    //==================================================================================

#if 1
    level = 0;
    sellMomentumSignal(macdVal1,
                       macdVal2,
                       level,
                       sellMomentumSignals.macdBelowZero);

#endif




    level = 70;
    sellMomentumSignal(rsiVal1,
                       rsiVal2,
                       level,
                       sellMomentumSignals.rsiBelow70);





    //==================================================================================
    // Buy signal (Momentum)
    //==================================================================================

#if 1
    level = 0;
    buyMomentumSignal(macdVal1,
                      macdVal2,
                      level,
                      buyMomentumSignals.macdAboveZero);
#endif


    level = 30;
    buyMomentumSignal(rsiVal1,
                      rsiVal2,
                      level,
                      buyMomentumSignals.rsiAbove30);

#if 0
    fprintf(m_file, "macd\n");
    fprintf(m_file, "%s\n", stockSymbol.toStdString().c_str());
    fprintf(m_file, "sell = %d\n", sellMomentumSignals.macdBelowZero);
    fprintf(m_file, "buy = %d\n", buyMomentumSignals.macdAboveZero);
    fprintf(m_file, "macdVal1 = %.3f\n", macdVal1);
    fprintf(m_file, "macdVal2 ) %.3f\n", macdVal2);
    for(int k = (arrLenMacd - 10); k < (arrLenMacd - 1); k++)
    {
        fprintf(m_file, "macdVal ) %.3f\n", m_macdData.data.indicator3.at(k));
    }
    fprintf(m_file, "\n");



    fprintf(m_file, "rsi\n");
    fprintf(m_file, "%s\n", stockSymbol.toStdString().c_str());
    fprintf(m_file, "sell = %d\n", sellMomentumSignals.rsiBelow70);
    fprintf(m_file, "buy = %d\n", buyMomentumSignals.rsiAbove30);
    fprintf(m_file, "rsiVal1 = %.3f\n", rsiVal1);
    fprintf(m_file, "rsiVal2 ) %.3f\n", rsiVal2);
    for(int k = (arrLenRsi - 10); k < (arrLenRsi - 1); k++)
    {
        fprintf(m_file, "rsiVal ) %.3f\n", m_rsiData.data.indicator1.at(k));
    }
    fprintf(m_file, "\n");

    fclose(m_file);
#endif
    #if 0
    if(true == buyMomentumSignals.rsiAbove30)
    {
        qDebug() << "Buy rsi";
        qDebug() << stockSymbol;
        qDebug() << "rsiVal1" << rsiVal1;
        qDebug() << "rsiVal1" << rsiVal2;

        for(int k = (arrLenRsi - 10); k < (arrLenRsi - 1); k++)
        {
            qDebug() << m_rsiData.data.indicator1.at(k);
        }

    }
    #endif



    //==================================================================================
    // Sell signal (Trend)
    //==================================================================================

    sellSignalFastAvgCrossSlowAvg(stockVal2, avgShortVal1, avgShortVal2, avgMidVal1, avgMidVal2,
                                 sellSignals.ShortAvgDipBelowMidAvg);

    sellSignalFastAvgCrossSlowAvg(stockVal2, avgShortVal1, avgShortVal2, avgLongVal1, avgLongVal2,
                                 sellSignals.ShortAvgDipBelowLongAvg);

    sellSignalFastAvgCrossSlowAvg(stockVal2, avgMidVal1, avgMidVal2, avgLongVal1, avgLongVal2,
                                 sellSignals.MidAvgDipBelowLongAvg);


    sellSignalFastAvgCrossSlowAvg(stockVal1, stockVal2, avgShortVal1, avgShortVal2,
                                 sellSignals.priceDipBelowShortAvg);

    sellSignalFastAvgCrossSlowAvg(stockVal1, stockVal2, avgMidVal1, avgMidVal2,
                                 sellSignals.priceDipBelowMidAvg);

    sellSignalFastAvgCrossSlowAvg(stockVal1, stockVal2, avgLongVal1, avgLongVal2,
                                 sellSignals.priceDipBelowLongAvg);

    //==================================================================================
    // Buy signal (Trend)
    //==================================================================================

    buySignalFastAvgCrossSlowAvg(stockVal2, avgShortVal1, avgShortVal2, avgMidVal1, avgMidVal2,
                                 buySignals.ShortAvgRiseAboveMidAvg);

    buySignalFastAvgCrossSlowAvg(stockVal2, avgShortVal1, avgShortVal2, avgLongVal1, avgLongVal2,
                                 buySignals.ShortAvgRiseAboveLongAvg);

    buySignalFastAvgCrossSlowAvg(stockVal2, avgMidVal1, avgMidVal2, avgLongVal1, avgLongVal2,
                                 buySignals.MidAvgRiseAboveLongAvg);


    buySignalFastAvgCrossSlowAvg(stockVal1, stockVal2, avgShortVal1, avgShortVal2,
                                 buySignals.priceRiseAboveShortAvg);

    buySignalFastAvgCrossSlowAvg(stockVal1, stockVal2, avgMidVal1, avgMidVal2,
                                 buySignals.priceRiseAboveMidAvg);

    buySignalFastAvgCrossSlowAvg(stockVal1, stockVal2, avgLongVal1, avgLongVal2,
                                 buySignals.priceRiseAboveLongAvg);


    return true;
}




/****************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
void TaBuySellIdicator::sellMomentumSignal(double dataVal1,
                                           double dataVal2,
                                           double level,
                                           bool &sellSignal)
{

    // Is short avg crossing mid avg?
    if((dataVal1 > dataVal2) &&
       (dataVal1 > level)    &&
       (dataVal2 < level))
    {
        sellSignal = true;
    }
    else
    {
        sellSignal = false;
    }

}


/****************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
void TaBuySellIdicator::buyMomentumSignal(double dataVal1,
                                           double dataVal2,
                                           double level,
                                           bool &buySignal)
{

    // Is short avg crossing mid avg?
    if((dataVal1 < dataVal2) &&
       (dataVal1 < level)    &&
       (dataVal2 > level))
    {
        buySignal = true;
    }
    else
    {
        buySignal = false;
    }

}







/****************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
void TaBuySellIdicator::sellSignalFastAvgCrossSlowAvg(double priseVal2,
                                                      double avgFastVal1,
                                                      double avgFastVal2,
                                                      double avgSlowVal1,
                                                      double avgSlowVal2,
                                                      bool &sellSignalFastCrossSlow)
{

    // Is short avg crossing mid avg?
    if((avgFastVal1 > avgSlowVal1) &&
       (avgSlowVal2 > avgFastVal2) &&
       (avgFastVal1 > avgFastVal2) &&
       (avgSlowVal1 > avgSlowVal2) &&
       (priseVal2 < avgSlowVal2)   &&
       (priseVal2 < avgFastVal2) )
    {
        sellSignalFastCrossSlow = true;
    }
    else
    {
        sellSignalFastCrossSlow = false;
    }

}



/****************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
void TaBuySellIdicator::sellSignalFastAvgCrossSlowAvg(double avgFastVal1,
                                                      double avgFastVal2,
                                                      double avgSlowVal1,
                                                      double avgSlowVal2,
                                                      bool &sellSignalFastCrossSlow)
{

    // Is short avg crossing mid avg?
    if((avgFastVal1 > avgSlowVal1) &&
       (avgSlowVal2 > avgFastVal2) &&
       (avgFastVal1 > avgFastVal2) &&
       (avgSlowVal1 > avgSlowVal2))
    {
        sellSignalFastCrossSlow = true;
    }
    else
    {
        sellSignalFastCrossSlow = false;
    }

}



/****************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
void TaBuySellIdicator::buySignalFastAvgCrossSlowAvg(double priseVal2,
                                                     double avgFastVal1,
                                                     double avgFastVal2,
                                                     double avgSlowVal1,
                                                     double avgSlowVal2,
                                                     bool &buySignalFastCrossSlow)
{

    // Is short avg crossing mid avg?
    if((avgFastVal1 < avgSlowVal1) &&
       (avgSlowVal2 < avgFastVal2) &&
       (avgFastVal1 < avgFastVal2) &&
       (priseVal2 > avgFastVal2)   &&
       (priseVal2 > avgSlowVal2)  /* &&
       (avgSlowVal1 < avgSlowVal2)*/)
    {
        buySignalFastCrossSlow = true;
    }
    else
    {
        buySignalFastCrossSlow = false;
    }

}


/****************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
void TaBuySellIdicator::buySignalFastAvgCrossSlowAvg(double avgFastVal1,
                                                     double avgFastVal2,
                                                     double avgSlowVal1,
                                                     double avgSlowVal2,
                                                     bool &buySignalFastCrossSlow)
{

    // Is short avg crossing mid avg?
    if((avgFastVal1 < avgSlowVal1) &&
       (avgSlowVal2 < avgFastVal2) &&
       (avgFastVal1 < avgFastVal2) /*&&
       (avgSlowVal1 < avgSlowVal2)*/)
    {
        buySignalFastCrossSlow = true;
    }
    else
    {
        buySignalFastCrossSlow = false;
    }

}



/****************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
bool TaBuySellIdicator::buySignalAvgShortAboveMid(QString stockSymbol)
{
    CUtil cu;
    CDbHndl db;
    int intMonth = -6;
    int arrLen20;
    int arrLen50;
    QString endDate;
    QString startDate;
    CYahooStockPlotUtil::PlotData_ST dummyQwtStockPlotData1;
    CYahooStockPlotUtil::PlotData_ST dummyQwtStockPlotData2;
    double avg20Val1;
    double avg50Val1;
    double avg20Val2;
    double avg50Val2;


    clearStockAndIndicatorMem(m_avgShortData);
    clearStockAndIndicatorMem(m_avgMidData);


    cu.getCurrentDate(endDate);

    if(false == cu.addMonth(endDate, startDate, intMonth))
    {
        return false;
    }


    db.getYahooTaMovingAverage(stockSymbol, startDate, endDate, 20, dummyQwtStockPlotData1, m_avgShortData);
    db.getYahooTaMovingAverage(stockSymbol, startDate, endDate, 50, dummyQwtStockPlotData2, m_avgMidData);

    arrLen50 = m_avgMidData.data.x.size();
    arrLen20 = m_avgShortData.data.x.size();


    qDebug() << "len" << arrLen50;

    if(arrLen50 < 50)
    {
        return false;
    }



    if(arrLen50 > 10)
    {
        avg20Val1 = m_avgShortData.data.indicator1.at(arrLen20-10);
        avg50Val1 = m_avgMidData.data.indicator1.at(arrLen50-10);

        if(avg20Val1 < avg50Val1)
        {
            avg20Val2 = m_avgShortData.data.indicator1.at(arrLen20-1);
            avg50Val2 = m_avgMidData.data.indicator1.at(arrLen50-1);

            if((avg20Val2 > avg50Val2) &&
               (avg20Val2 > avg20Val1) &&
               (avg50Val2 > avg50Val1))
            {
                qDebug() << "avg20Val1 = " <<  avg20Val1 << "avg50Val1 = " << avg50Val1;
                qDebug() << "avg20Val2 = " <<  avg20Val2 << "avg50Val2 = " << avg50Val2;
                qDebug() << "20 Date1" << m_avgShortData.data.xDate.at(arrLen20-10);
                qDebug() << "20 Date2" << m_avgShortData.data.xDate.at(arrLen20-1);
                qDebug() << "50 Date1" << m_avgMidData.data.xDate.at(arrLen50-10);
                qDebug() << "50 Date2" << m_avgMidData.data.xDate.at(arrLen50-1);

                return true;
            }
        }
    }

     return false;
}



/*******************************************************************
 *
 * Function:    clearStockAndIndicatorTempMem()
 *
 * Description:
 *
 *
 *******************************************************************/
void TaBuySellIdicator::clearStockAndIndicatorMem(CYahooStockPlotUtil::StockData_ST &stockData)
{
    stockData.data.x.clear();
    stockData.data.y.clear();
    stockData.data.xDate.clear();
    stockData.data.indicator1.clear();
    stockData.data.indicator2.clear();
    stockData.data.indicator3.clear();
    stockData.data.indicator4.clear();

}
