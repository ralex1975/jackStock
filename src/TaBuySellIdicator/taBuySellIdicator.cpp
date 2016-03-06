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
 * Description: debug ska tas bort
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


    clearStockAndIndicatorMem(m_avg20Data);
    clearStockAndIndicatorMem(m_avg50Data);


    cu.getCurrentDate(endDate);

    if(false == cu.addMonth(endDate, startDate, intMonth))
    {
        return false;
    }


    db.getYahooTaMovingAverage(stockSymbol, startDate, endDate, 20, dummyQwtStockPlotData1, m_avg20Data);
    db.getYahooTaMovingAverage(stockSymbol, startDate, endDate, 50, dummyQwtStockPlotData2, m_avg50Data);

    arrLen = m_avg50Data.data.x.size();

    if(arrLen < 50 )
    {
        return false;
    }


    if(arrLen > 15)
    {
        avg20Val = m_avg20Data.data.indicator1.at(arrLen-15);
        avg50Val = m_avg50Data.data.indicator1.at(arrLen-15);

        if(avg20Val < avg50Val)
        {
            avg20Val = m_avg20Data.data.indicator1.at(arrLen-1);
            avg50Val = m_avg50Data.data.indicator1.at(arrLen-1);

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

    //db.getYahooTaPriceData(stockSymbol, startDate, endDate, dummyQwtStockPlotData, m_avg20Data);

    return res;
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

    // stockData.clear();
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
bool TaBuySellIdicator::buySignalAvg20Above50(QString stockSymbol)
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


    clearStockAndIndicatorMem(m_avg20Data);
    clearStockAndIndicatorMem(m_avg50Data);


    cu.getCurrentDate(endDate);

    if(false == cu.addMonth(endDate, startDate, intMonth))
    {
        return false;
    }


    //db.getYahooTaPriceData(stockSymbol, startDate, endDate, dummyQwtStockPlotData1, m_avg20Data);
    //db.getYahooTaPriceData(stockSymbol, startDate, endDate, dummyQwtStockPlotData2, m_avg50Data);

    db.getYahooTaMovingAverage(stockSymbol, startDate, endDate, 20, dummyQwtStockPlotData1, m_avg20Data);
    db.getYahooTaMovingAverage(stockSymbol, startDate, endDate, 50, dummyQwtStockPlotData2, m_avg50Data);

    arrLen50 = m_avg50Data.data.x.size();
    arrLen20 = m_avg20Data.data.x.size();


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
        avg20Val1 = m_avg20Data.data.indicator1.at(arrLen20-10);
        avg50Val1 = m_avg50Data.data.indicator1.at(arrLen50-10);

        if(avg20Val1 < avg50Val1)
        {
            avg20Val2 = m_avg20Data.data.indicator1.at(arrLen20-1);
            avg50Val2 = m_avg50Data.data.indicator1.at(arrLen50-1);

            if((avg20Val2 > avg50Val2) &&
               (avg20Val2 > avg20Val1) &&
               (avg50Val2 > avg50Val1))
            {
                qDebug() << "avg20Val1 = " <<  avg20Val1 << "avg50Val1 = " << avg50Val1;
                qDebug() << "avg20Val2 = " <<  avg20Val2 << "avg50Val2 = " << avg50Val2;
                qDebug() << "20 Date1" << m_avg20Data.data.xDate.at(arrLen20-10);
                qDebug() << "20 Date2" << m_avg20Data.data.xDate.at(arrLen20-1);
                qDebug() << "50 Date1" << m_avg50Data.data.xDate.at(arrLen50-10);
                qDebug() << "50 Date2" << m_avg50Data.data.xDate.at(arrLen50-1);

                return true;
            }
        }
    }

     return false;
}

