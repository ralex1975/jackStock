/*******************************************************************
 *
 * Filename: parsePriceData.h
 * Date:     2012-07-08
 *
 * Description: This file reads the html page that contains stock price
 *              information.
 *
 *
 *******************************************************************/

#ifndef PARSEPRICEDATA_H
#define PARSEPRICEDATA_H

#include <QFile>
#include <QString>
#include <QDebug>
#include <QTextStream>
#include "dbHndl.h"

class CParsePriceData
{
private:
    enum PriceParseState_ET
    {
        LAST_PRICE,
        PRICE_CHANGE,
        PROCENT_CHANGE_ONE_DAY,
        BID_PRICE,
        OFFER_PRICE,
        HIGHEST_PRICE,
        LOWEST_PRICE,
        VOLUME,
        CURRENCY,
        TIME,
        PRICE_MAX_NOF_STOCK_DATA
    };

    bool parseCompanyName(QTextStream &inStream, QString &result);
    bool parseStockData(QTextStream &inStream,  QString &result, QString endTag);
    bool findEndTag(QString str, QString &endTag);

public:
    CParsePriceData();
    bool parseStockPrices(QString filename, CDbHndl &db);
};

#endif // PARSEPRICEDATA_H
