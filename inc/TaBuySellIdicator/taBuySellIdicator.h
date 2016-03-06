#ifndef TABUYSELLIDICATOR_H
#define TABUYSELLIDICATOR_H

#include "dbHndl.h"

class TaBuySellIdicator
{
    // Store one stock with data
    CYahooStockPlotUtil::StockData_ST m_avg20Data;
    CYahooStockPlotUtil::StockData_ST m_avg50Data;

    bool buyAvg20Above50(QString stockSymbol);


public:
    TaBuySellIdicator();
    bool buySignalAvg20Above50(QString stockSymbol);
    bool getBuySignals(QString stockSymbol);
    void clearStockAndIndicatorMem(CYahooStockPlotUtil::StockData_ST &stockData);

};

#endif // TABUYSELLIDICATOR_H
