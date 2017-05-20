#ifndef PARSEKEYNUMBERDATA_H
#define PARSEKEYNUMBERDATA_H

/*******************************************************************
 *
 * Filename: parseKeyNumberData.h
 * Date:     2012-07-08
 *
 * Description: This file reads the html page that contains stock key
 *              number information.
 *
 *
 *******************************************************************/

#include <QFile>
#include <QString>
#include <QDebug>
#include <QTextStream>
#include "dbHndl.h"



class CParseKeyNumberData
{
private:
    enum
    {
       KEY_VALUE_PE,
       KEY_VALUE_PS,
       KEY_VALUE_EARNINGS_PER_SHARE,
       KEY_VALUE_NAV_PER_SHARE,
       KEY_VALUE_DIVIDEND_PER_SHARE,
       KEY_VALUE_YIELD,
       KEY_VALUE_COURSE_PER_JEK,
       MAX_NOF_STOCK_DATA
    };

    bool parseCompanyName(QTextStream &inStream, QString &result);
    bool parseStockData(QTextStream &inStream,  QString &result, QString endTag);

public:
    CParseKeyNumberData();
    bool parseStockKeyNumberData(QString filename, CDbHndl &db, CDbHndl::DbInsertMode_ET dbUpdateMode);
};

#endif // PARSEKEYNUMBERDATA_H
