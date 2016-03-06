#include "inc/TaBuySellIdicator/taBuySellIdicator.h"
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
QString TaBuySellIdicator::convAvgSellSignalToNumber(SellSignalMovingAvgST &sellSignals)
{
    QString sellSignalNo = "0";

    if(sellSignals.priceDipBelowShortAvg == true)
    {
        sellSignalNo = "1";
    }

    if(sellSignals.priceDipBelowMidAvg == true)
    {
        sellSignalNo = "2";
    }

    if(sellSignals.priceDipBelowLongAvg == true)
    {
        sellSignalNo = "3";
    }

    if(sellSignals.ShortAvgDipBelowMidAvg == true)
    {
        sellSignalNo = "4";
    }

    if(sellSignals.ShortAvgDipBelowLongAvg == true)
    {
        sellSignalNo = "5";
    }

    if(sellSignals.MidAvgDipBelowLongAvg == true)
    {
        sellSignalNo = "6";
    }

    #if 0
    if(sellSignals.priceContBelowShortAvg == true)
    {
        sellSignalNo = "7";
    }

    if(sellSignals.priceContBelowMidAvg  == true)
    {
        sellSignalNo = "8";
    }

    if(sellSignals.priceContBelowLongAvg  == true)
    {
        sellSignalNo = "9";
    }

    if(sellSignals.priceContBelowMidLongAvg  == true)
    {
        sellSignalNo = "10";
    }


    if(sellSignals.priceContBelowShortMidLongAvg == true)
    {
        sellSignalNo = "11";
    }
    #endif

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
        buySignalNo = "1";
    }

    if(buySignals.priceRiseAboveMidAvg == true)
    {
        buySignalNo = "2";
    }

    if(buySignals.priceRiseAboveLongAvg == true)
    {
        buySignalNo = "3";
    }

    if(buySignals.ShortAvgRiseAboveMidAvg == true)
    {
        buySignalNo = "4";
    }

    if(buySignals.ShortAvgRiseAboveLongAvg == true)
    {
        buySignalNo = "5";
    }

    if(buySignals.MidAvgRiseAboveLongAvg == true)
    {
        buySignalNo = "6";
    }

    #if 0
    if(buySignals.priceContAboveShortAvg == true)
    {
        buySignalNo = "7";
    }

    if(buySignals.priceContAboveMidAvg  == true)
    {
        buySignalNo = "8";
    }

    if(buySignals.priceContAboveLongAvg  == true)
    {
        buySignalNo = "9";
    }

    if(buySignals.priceContAboveMidLongAvg  == true)
    {
        buySignalNo = "10";
    }


    if(buySignals.priceContAboveShortMidLongAvg == true)
    {
        buySignalNo = "11";
    }
    #endif

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
bool TaBuySellIdicator::resetSellSignals(SellSignalMovingAvgST &sellSignals)
{
    // Reset status
    sellSignals.ShortAvgDipBelowLongAvg = false;
    sellSignals.ShortAvgDipBelowMidAvg = false;

    sellSignals.MidAvgDipBelowLongAvg = false;

    sellSignals.priceDipBelowLongAvg = false;
    sellSignals.priceDipBelowMidAvg = false;
    sellSignals.priceDipBelowShortAvg = false;

    sellSignals.priceContBelowShortAvg = false;
    sellSignals.priceContBelowMidAvg  = false;
    sellSignals.priceContBelowLongAvg  = false;

    sellSignals.priceContBelowShortMidLongAvg = false;

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
bool TaBuySellIdicator::resetBuySignals(BuySignalMovingAvgST &buySignals)
{
    // Reset status
    buySignals.ShortAvgRiseAboveLongAvg = false;
    buySignals.ShortAvgRiseAboveMidAvg = false;

    buySignals.MidAvgRiseAboveLongAvg = false;

    buySignals.priceRiseAboveLongAvg = false;
    buySignals.priceRiseAboveMidAvg = false;
    buySignals.priceRiseAboveShortAvg = false;

    buySignals.priceContAboveShortAvg = false;
    buySignals.priceContAboveMidAvg  = false;
    buySignals.priceContAboveLongAvg  = false;

    buySignals.priceContAboveShortMidLongAvg = false;

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
                                       BuySignalMovingAvgST  &buySignals)
{
    int i;
    int j;
    CUtil cu;
    CDbHndl db;
    int intMonth = -10;
    int arrLenAvgShort;
    int arrLenAvgMid;
    int arrLenAvgLong;
    int arrLenStockPrice;
    QString endDate;
    QString startDate;
    CYahooStockPlotUtil::PlotData_ST dummyQwtStockPlotData1;
    CYahooStockPlotUtil::PlotData_ST dummyQwtStockPlotData2;
    CYahooStockPlotUtil::PlotData_ST dummyQwtStockPlotData3;
    double stockVal1;
    double avgShortVal1;
    double avgMidVal1;
    double avgLongVal1;

    double stockVal2;
    double avgShortVal2;
    double avgMidVal2;
    double avgLongVal2;

    if(stockSymbol.compare(QString::fromUtf8("HLDX.ST")) == 0)
    {
        i = 12;
        j = i;
    }


    // Array with x, y data
    clearStockAndIndicatorMem(m_avgShortData);
    clearStockAndIndicatorMem(m_avgMidData);
    clearStockAndIndicatorMem(m_avgLongData);
    clearStockAndIndicatorMem(m_stockPrice);

    // Reset Sell buy flags
    resetSellSignals(sellSignals);
    resetBuySignals(buySignals);


    // Calc start stop date
    cu.createStartEndDates(startDate, endDate, intMonth);

    // Gather stock price
    db.getYahooTaPriceData(stockSymbol, startDate, endDate, dummyQwtStockPlotData1, m_stockPrice);

    // Calc moving averages
    db.getYahooTaMovingAverage(stockSymbol, startDate, endDate, 20, dummyQwtStockPlotData1, m_avgShortData);
    db.getYahooTaMovingAverage(stockSymbol, startDate, endDate, 50, dummyQwtStockPlotData2, m_avgMidData);
    db.getYahooTaMovingAverage(stockSymbol, startDate, endDate, 150, dummyQwtStockPlotData3, m_avgLongData);

    // Store nof of data in each array
    arrLenAvgShort      = m_avgShortData.data.x.size();
    arrLenAvgMid        = m_avgMidData.data.x.size();
    arrLenAvgLong        = m_avgLongData.data.x.size();
    arrLenStockPrice    = m_stockPrice.data.x.size();

    // To little data to create sell/buy signal?
    if((arrLenStockPrice < 15)  ||
       (arrLenAvgShort   < 15)  ||
       (arrLenAvgMid     < 15)  ||
       (arrLenAvgLong    < 15))
    {
        return false;
    }



    // Get nof data in stock price array
    stockVal1 = m_stockPrice.data.y.at(arrLenStockPrice - 10);
    stockVal2  = m_stockPrice.data.y.at(arrLenStockPrice - 1);;


    //=====================================================================
    // Check if price dips or is continuously below SHORT moving average
    //=====================================================================
    avgShortVal1 = m_avgShortData.data.indicator1.at(arrLenAvgShort-10);
    avgShortVal2 = m_avgShortData.data.indicator1.at(arrLenAvgShort-1);

    sellSignals.priceContBelowShortAvg = true;
    buySignals.priceContAboveShortAvg = true;
    j =  arrLenStockPrice-15;

    for(i = arrLenAvgShort-15; i < arrLenAvgShort; i++, j++)
    {
        if(m_stockPrice.data.y.at(j) > m_avgShortData.data.indicator1.at(i))
        {
            sellSignals.priceContBelowShortAvg = false;
        }

        if(m_stockPrice.data.y.at(j) < m_avgShortData.data.indicator1.at(i))
        {
            buySignals.priceContAboveShortAvg = false;
        }
    }

    //=====================================================================
    // Check if price dips or is continuously below MID moving average
    //=====================================================================
    avgMidVal1 = m_avgMidData.data.indicator1.at(arrLenAvgMid - 10);
    avgMidVal2 = m_avgMidData.data.indicator1.at(arrLenAvgMid - 1);


    sellSignals.priceContBelowMidAvg = true;
    buySignals.priceContAboveMidAvg = true;
    j =  arrLenStockPrice - 15;

    for(i = arrLenAvgMid - 15; i < arrLenAvgMid; i++, j++)
    {
        if(m_stockPrice.data.y.at(j) > m_avgMidData.data.indicator1.at(i))
        {
             sellSignals.priceContBelowMidAvg = false;
        }

        if(m_stockPrice.data.y.at(j) < m_avgMidData.data.indicator1.at(i))
        {
             buySignals.priceContAboveMidAvg = false;
        }

    }


    //=====================================================================
    // Check if price dips or is continuously below LONG moving average
    //=====================================================================
    avgLongVal1 = m_avgLongData.data.indicator1.at(arrLenAvgLong - 10);
    avgLongVal2 = m_avgLongData.data.indicator1.at(arrLenAvgLong - 1);

    sellSignals.priceContBelowLongAvg = true;
    buySignals.priceContAboveLongAvg = true;
    j =  arrLenStockPrice - 15;
    for(i = arrLenAvgLong - 15; i < arrLenAvgLong; i++, j++)
    {
        if(m_stockPrice.data.y.at(j) > m_avgLongData.data.indicator1.at(i))
        {
            sellSignals.priceContBelowLongAvg = false;
        }

        if(m_stockPrice.data.y.at(j) < m_avgLongData.data.indicator1.at(i))
        {
            buySignals.priceContAboveLongAvg = false;
        }
    }

    #if 0
    // Is price continuously below SHORT, MID, LONG avg?
    if((sellSignals.priceContBelowShortAvg == true) &&
       (sellSignals.priceContBelowMidAvg == true) &&
       (sellSignals.priceContBelowLongAvg == true))
    {
        sellSignals.priceContBelowShortMidLongAvg = true;
    }
    else if((sellSignals.priceContBelowMidAvg == true) &&
            (sellSignals.priceContBelowLongAvg == true))
    {
        sellSignals.priceContBelowMidLongAvg = true;
    }


    // Is price continuously above SHORT, MID, LONG avg?
    if((buySignals.priceContAboveShortAvg == true) &&
       (buySignals.priceContAboveMidAvg == true) &&
       (buySignals.priceContAboveLongAvg == true))
    {
        buySignals.priceContAboveShortMidLongAvg = true;
    }
    else if((buySignals.priceContAboveMidAvg == true) &&
            (buySignals.priceContAboveLongAvg == true))
    {
        buySignals.priceContAboveMidLongAvg = true;
    }
    #endif




    sellSignalFastAvgCrossSlowAvg(stockVal1, stockVal2, avgShortVal1, avgShortVal2, avgMidVal1, avgMidVal2,
                                 sellSignals.ShortAvgDipBelowMidAvg);

    sellSignalFastAvgCrossSlowAvg(stockVal1, stockVal2, avgShortVal1, avgShortVal2, avgLongVal1, avgLongVal2,
                                 sellSignals.ShortAvgDipBelowLongAvg);

    sellSignalFastAvgCrossSlowAvg(stockVal1, stockVal2, avgMidVal1, avgMidVal2, avgLongVal1, avgLongVal2,
                                 sellSignals.MidAvgDipBelowLongAvg);


    sellSignalFastAvgCrossSlowAvg(stockVal1, stockVal2, stockVal1, stockVal2, avgShortVal1, avgShortVal2,
                                 sellSignals.priceDipBelowShortAvg);

    sellSignalFastAvgCrossSlowAvg(stockVal1, stockVal2, stockVal1, stockVal2, avgMidVal1, avgMidVal2,
                                 sellSignals.priceDipBelowMidAvg);

    sellSignalFastAvgCrossSlowAvg(stockVal1, stockVal2, stockVal1, stockVal2, avgLongVal1, avgLongVal2,
                                 sellSignals.priceDipBelowLongAvg);

    //==================================================================================
    // Buy signal
    //==================================================================================

    buySignalFastAvgCrossSlowAvg(stockVal1, stockVal2, avgShortVal1, avgShortVal2, avgMidVal1, avgMidVal2,
                                 buySignals.ShortAvgRiseAboveMidAvg);

    buySignalFastAvgCrossSlowAvg(stockVal1, stockVal2, avgShortVal1, avgShortVal2, avgLongVal1, avgLongVal2,
                                 buySignals.ShortAvgRiseAboveLongAvg);

    buySignalFastAvgCrossSlowAvg(stockVal1, stockVal2, avgMidVal1, avgMidVal2, avgLongVal1, avgLongVal2,
                                 buySignals.MidAvgRiseAboveLongAvg);


    buySignalFastAvgCrossSlowAvg(stockVal1, stockVal2, stockVal1, stockVal2, avgShortVal1, avgShortVal2,
                                 buySignals.priceRiseAboveShortAvg);

    buySignalFastAvgCrossSlowAvg(stockVal1, stockVal2, stockVal1, stockVal2, avgMidVal1, avgMidVal2,
                                 buySignals.priceRiseAboveMidAvg);

    buySignalFastAvgCrossSlowAvg(stockVal1, stockVal2, stockVal1, stockVal2, avgLongVal1, avgLongVal2,
                                 buySignals.priceRiseAboveLongAvg);


    return true;
}



#if 0
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
bool TaBuySellIdicator::avgSellSignals(QString stockSymbol, SellSignalMovingAvgST &sellSignals)
{
    int i;
    int j;
    CUtil cu;
    CDbHndl db;
    int intMonth = -10;
    int arrLenAvgShort;
    int arrLenAvgMid;
    int arrLenAvgLong;
    int arrLenStockPrice;
    QString endDate;
    QString startDate;
    CYahooStockPlotUtil::PlotData_ST dummyQwtStockPlotData1;
    CYahooStockPlotUtil::PlotData_ST dummyQwtStockPlotData2;
    CYahooStockPlotUtil::PlotData_ST dummyQwtStockPlotData3;
    double stockVal1;
    double avgShortVal1;
    double avgMidVal1;
    double avgLongVal1;

    double stockVal2;
    double avgShortVal2;
    double avgMidVal2;
    double avgLongVal2;

    if(stockSymbol.compare(QString::fromUtf8("HLDX.ST")) == 0)
    {
        i = 12;
        j = i;
    }



    clearStockAndIndicatorMem(m_avgShortData);
    clearStockAndIndicatorMem(m_avgMidData);
    clearStockAndIndicatorMem(m_avgLongData);
    clearStockAndIndicatorMem(m_stockPrice);

    // Reset status
    sellSignals.ShortAvgDipBelowLongAvg = false;
    sellSignals.ShortAvgDipBelowMidAvg = false;

    sellSignals.MidAvgDipBelowLongAvg = false;

    sellSignals.priceDipBelowLongAvg = false;
    sellSignals.priceDipBelowMidAvg = false;
    sellSignals.priceDipBelowShortAvg = false;

    sellSignals.priceContBelowShortAvg = false;
    sellSignals.priceContBelowMidAvg  = false;
    sellSignals.priceContBelowLongAvg  = false;

    sellSignals.priceContBelowShortMidLongAvg = false;


    cu.createStartEndDates(startDate, endDate, intMonth);

    db.getYahooTaPriceData(stockSymbol, startDate, endDate, dummyQwtStockPlotData1, m_stockPrice);

    db.getYahooTaMovingAverage(stockSymbol, startDate, endDate, 20, dummyQwtStockPlotData1, m_avgShortData);
    db.getYahooTaMovingAverage(stockSymbol, startDate, endDate, 50, dummyQwtStockPlotData2, m_avgMidData);
    db.getYahooTaMovingAverage(stockSymbol, startDate, endDate, 150, dummyQwtStockPlotData3, m_avgLongData);

    arrLenAvgShort      = m_avgShortData.data.x.size();
    arrLenAvgMid        = m_avgMidData.data.x.size();
    arrLenAvgLong        = m_avgLongData.data.x.size();
    arrLenStockPrice    = m_stockPrice.data.x.size();

    // To little data to create sell signal
    if((arrLenStockPrice < 15)  ||
       (arrLenAvgShort   < 15)  ||
       (arrLenAvgMid     < 15)  ||
       (arrLenAvgLong    < 15))
    {
        return false;
    }



    // Get nof data in stock price array
    stockVal1 = m_stockPrice.data.y.at(arrLenStockPrice - 10);
    stockVal2  = m_stockPrice.data.y.at(arrLenStockPrice - 1);;


    //=====================================================================
    // Check if price dips or is continuously below SHORT moving average
    //=====================================================================
    avgShortVal1 = m_avgShortData.data.indicator1.at(arrLenAvgShort-10);
    avgShortVal2 = m_avgShortData.data.indicator1.at(arrLenAvgShort-1);

    sellSignals.priceContBelowShortAvg = true;
    j =  arrLenStockPrice-15;
    for(i = arrLenAvgShort-15; i < arrLenAvgShort; i++, j++)
    {
        if(m_stockPrice.data.y.at(j) > m_avgShortData.data.indicator1.at(i))
        {
            sellSignals.priceContBelowShortAvg = false;
        }
    }

    //=====================================================================
    // Check if price dips or is continuously below MID moving average
    //=====================================================================
    avgMidVal1 = m_avgMidData.data.indicator1.at(arrLenAvgMid - 10);
    avgMidVal2 = m_avgMidData.data.indicator1.at(arrLenAvgMid - 1);


    sellSignals.priceContBelowMidAvg = true;
    j =  arrLenStockPrice - 15;
    for(i = arrLenAvgMid - 15; i < arrLenAvgMid; i++, j++)
    {
        if(m_stockPrice.data.y.at(j) > m_avgMidData.data.indicator1.at(i))
        {
             sellSignals.priceContBelowMidAvg = false;
        }
    }


    //=====================================================================
    // Check if price dips or is continuously below LONG moving average
    //=====================================================================
    avgLongVal1 = m_avgLongData.data.indicator1.at(arrLenAvgLong - 10);
    avgLongVal2 = m_avgLongData.data.indicator1.at(arrLenAvgLong - 1);

    sellSignals.priceContBelowLongAvg = true;
    j =  arrLenStockPrice - 15;
    for(i = arrLenAvgLong - 15; i < arrLenAvgLong; i++, j++)
    {
        if(m_stockPrice.data.y.at(j) > m_avgLongData.data.indicator1.at(i))
        {
            sellSignals.priceContBelowLongAvg = false;
        }
    }

    // Is price continuously below SHORT, MID, LONG avg?
    if((sellSignals.priceContBelowShortAvg == true) &&
       (sellSignals.priceContBelowMidAvg == true) &&
       (sellSignals.priceContBelowLongAvg == true))
    {
        sellSignals.priceContBelowShortMidLongAvg = true;
    }
    else if((sellSignals.priceContBelowMidAvg == true) &&
            (sellSignals.priceContBelowLongAvg == true))
    {
        sellSignals.priceContBelowMidLongAvg = true;
    }



    sellSignalFastAvgCrossSlowAvg(avgShortVal1, avgShortVal2, avgMidVal1, avgMidVal2,
                                 sellSignals.ShortAvgDipBelowMidAvg);

    sellSignalFastAvgCrossSlowAvg(avgShortVal1, avgShortVal2, avgLongVal1, avgLongVal2,
                                 sellSignals.ShortAvgDipBelowLongAvg);

    sellSignalFastAvgCrossSlowAvg(avgMidVal1, avgMidVal2, avgLongVal1, avgLongVal2,
                                 sellSignals.MidAvgDipBelowLongAvg);


    sellSignalFastAvgCrossSlowAvg(stockVal1, stockVal2, avgShortVal1, avgShortVal2,
                                 sellSignals.priceDipBelowShortAvg);

    sellSignalFastAvgCrossSlowAvg(stockVal1, stockVal2, avgMidVal1, avgMidVal2,
                                 sellSignals.priceDipBelowMidAvg);

    sellSignalFastAvgCrossSlowAvg(stockVal1, stockVal2, avgLongVal1, avgLongVal2,
                                 sellSignals.priceDipBelowLongAvg);

    return true;
}
#endif

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
void TaBuySellIdicator::sellSignalFastAvgCrossSlowAvg(double priseVal1,
                                                      double priseVal2,
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
       //(priseVal1 > avgFastVal1)   &&
       //(priseVal1 > avgSlowVal1)   &&
       // (priseVal1 > priseVal2)   &&
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
void TaBuySellIdicator::buySignalFastAvgCrossSlowAvg(double priseVal1,
                                                     double priseVal2,
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
       (priseVal2 > avgSlowVal2)   &&
       (avgSlowVal1 < avgSlowVal2))
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

    if(arrLen50 < 50 )
    {
        return false;
    }

    if(stockSymbol.compare(QString::fromUtf8("INDT.ST")) == 0)
    {
      arrLen50++;
      arrLen50--;
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
    stockData.data.indicator1.clear();
    stockData.data.indicator2.clear();
    stockData.data.indicator3.clear();
    stockData.data.indicator4.clear();

}
