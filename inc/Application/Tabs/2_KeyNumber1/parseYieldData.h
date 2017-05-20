/*******************************************************************
 *
 * Filename: parseYieldData.h
 * Date:     2012-07-08
 *
 * Description: This file reads the html page that contains stock
 *              yield information.
 *
 *
 *******************************************************************/

#ifndef PARSEYIELDDATA_H
#define PARSEYIELDDATA_H

#include <QFile>
#include <QString>
#include <QDebug>
#include <QTextStream>
#include "dbHndl.h"

class CParseYieldData
{
private:
    enum
    {
        PROCENT_CHANGE_1_DAY,
        PROCENT_CHANGE_1_WEEK,
        PROCENT_CHANGE_1_MONTH,
        PROCENT_CHANGE_3_MONTH,
        PROCENT_CHANGE_6_MONTH,
        PROCENT_CHANGE_1_YEAR,
        PROCENT_CHANGE_2_YEAR,
        PROCENT_CHANGE_3_YEAR,
        PROCENT_CHANGE_5_YEAR,
        YIELD_MAX_NOF_STOCK_DATA
    };

    bool yieldPageParseCompanyName(QTextStream &inStream, QString &result, QString startTag, QString endTagLen);
    //bool parseStockData(QTextStream &inStream, QString &result, QString endTag);

    bool findEndTag(QString str, QString &endTag);
    bool parseStockData(QTextStream &inStream,  QString &result, QString endTag);



public:
    CParseYieldData();
    bool parseStockYields(QString filename, CDbHndl &db, CDbHndl::DbInsertMode_ET dbUpdateMode);
};

#endif // PARSEYIELDDATA_H
