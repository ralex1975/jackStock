/*******************************************************************
 *
 * Filename: parseYieldData.cpp
 * Date:     2012-07-08
 *
 * Description: This file reads the html page that contains stock
 *              yield information.
 *
 *
 *******************************************************************/
#include "parseYieldData.h"

#include <QMessageBox>
#include <QtCore>
#include <QString>
#include <QObject>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlError>

//#define YIELD_MAX_NOF_STOCK_DATA 9

#define TAG_YIELD_START_COMPANY_NAME  QString::fromUtf8("&marketplace=11\" class=\"underline\">")
#define TAG_YIELD_START_COMPANY_NAME1 QString::fromUtf8("&marketplace=14\" class=\"underline\">")
#define TAG_YIELD_START_COMPANY_NAME2 QString::fromUtf8("&marketplace=15\" class=\"underline\">")
#define TAG_YIELD_START_COMPANY_NAME3 QString::fromUtf8("&marketplace=24\" class=\"underline\">")


#define TAG_YIELD_START_END_PART_FIRST_DATA QString::fromUtf8("</a></div></td>")
#define TAG_YIELD_START_END_PART_SECOND_DATA QString::fromUtf8("%</div></td>")




#define MAX_NOF_START_DATA_TAGS 8
#define MAX_STR_LEN_START_DATA_TAGS 200
const char startDataTagArr[MAX_NOF_START_DATA_TAGS][MAX_STR_LEN_START_DATA_TAGS] =
{

    "				<td class=\"last\" ><div class=\"kursNeutral\">",
    "				<td class=\"last\" ><div class=\"kursMinus\">",
    "				<td class=\"last\" ><div class=\"kursPlus\">",
    "				<td  ><div class=\"kursNeutral\">",    // New
    " 				<td  ><div class=\"kursNeutral\">",
  //  "				<td  ><span class=\"kursMinus\">",      // New
    "				<td  ><div class=\"kursMinus\">",
    //"				<td  ><span class=\"kursPlus\">",       // New
    "				<td  ><div class=\"kursPlus\">",
    "				<td  >",                                // New

};


#define MAX_NOF_END_DATA_TAGS 4
static const char endDataTagArr[MAX_NOF_END_DATA_TAGS][MAX_STR_LEN_START_DATA_TAGS] =
{
    "</a></div></td>",
    "%</div></td>",
    "</div></td>",
    "</td>",
//    "				<td  >",
};


/****************************************************************
 *
 * Function:    CParseYieldData()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
CParseYieldData::CParseYieldData()
{
}


/****************************************************************
 *
 * Function:    parseStockYields()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
bool CParseYieldData::
parseStockYields(QString filename, CDbHndl &db, CDbHndl::DbInsertMode_ET dbUpdateMode)
{
    QFile file(filename);
    QString errStr = QString("Fail to open file: %1").arg(filename);
    QString result;
    bool res;
    int i;
    int j=0;


    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::critical(NULL, QString::fromUtf8("Fail to open file"), errStr);
        return false;
    }

    QTextStream inStream(&file);
    inStream.setCodec("ISO 8859-1");
    // inStream.setCodec("UTF-8");


    while(!inStream.atEnd())
    {
        res = yieldPageParseCompanyName(inStream, result, TAG_YIELD_START_COMPANY_NAME, TAG_YIELD_START_END_PART_FIRST_DATA);



        if(res)
        {
            if(result.length() < 1)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("Yields Parser Error"), "Company name is empty");
                db.m_snapshotStockData.companyName.clear();
            }
            else
            {
                db.m_snapshotStockData.companyName = result;
            }

            qDebug() << j++ << "Company:" << result << ", ";

            for(i=0; i <YIELD_MAX_NOF_STOCK_DATA;  )
            {
                res = parseStockData(inStream, result, TAG_YIELD_START_END_PART_SECOND_DATA);
                if(res)
                {
                    switch(i)
                    {
                    case PROCENT_CHANGE_1_DAY:
                        if(result.length() < 1)
                        {
                            QMessageBox::critical(NULL, QString::fromUtf8("Yields Parser Error"), "percent change 1 day");
                            db.m_snapshotStockData.procentChangeOneDay.clear();
                        }
                        else
                        {
                            result.replace(",",".");
                            db.m_snapshotStockData.procentChangeOneDay = result;
                        }
                        break;

                    case PROCENT_CHANGE_1_WEEK:
                        if(result.length() < 1)
                        {
                            QMessageBox::critical(NULL, QString::fromUtf8("Yields Parser Error"), "percent change 1 week");
                            db.m_snapshotStockData.procentChangeOneWeek.clear();
                        }
                        else
                        {
                            result.replace(",",".");
                            db.m_snapshotStockData.procentChangeOneWeek = result;
                        }
                        break;
                    case PROCENT_CHANGE_1_MONTH:
                        if(result.length() < 1)
                        {
                            QMessageBox::critical(NULL, QString::fromUtf8("Yields Parser Error"), "percent change 1 month");
                            db.m_snapshotStockData.procentChange1Month.clear();
                        }
                        else
                        {
                            result.replace(",",".");
                            db.m_snapshotStockData.procentChange1Month = result;
                        }
                        break;
                    case PROCENT_CHANGE_3_MONTH:
                        if(result.length() < 1)
                        {
                            QMessageBox::critical(NULL, QString::fromUtf8("Yields Parser Error"), "percent change 3 month");
                            db.m_snapshotStockData.procentChange3Month.clear();
                        }
                        else
                        {
                            result.replace(",",".");
                            db.m_snapshotStockData.procentChange3Month = result;
                        }
                        break;
                    case PROCENT_CHANGE_6_MONTH:
                        if(result.length() < 1)
                        {
                            QMessageBox::critical(NULL, QString::fromUtf8("Yields Parser Error"), "percent change 6 month");
                            db.m_snapshotStockData.procentChange6Month.clear();
                        }
                        else
                        {
                            result.replace(",",".");
                            db.m_snapshotStockData.procentChange6Month = result;
                        }
                        break;
                    case PROCENT_CHANGE_1_YEAR:
                        if(result.length() < 1)
                        {
                            QMessageBox::critical(NULL, QString::fromUtf8("Yields Parser Error"), "percent change 1 year");
                            db.m_snapshotStockData.procentChange1Year.clear();
                        }
                        else
                        {
                            result.replace(",",".");
                            db.m_snapshotStockData.procentChange1Year = result;
                        }
                        break;
                    case PROCENT_CHANGE_2_YEAR:
                        if(result.length() < 1)
                        {
                            QMessageBox::critical(NULL, QString::fromUtf8("Yields Parser Error"), "percent change 2 year");
                            db.m_snapshotStockData.procentChange2Year.clear();
                        }
                        else
                        {
                            result.replace(",",".");
                            db.m_snapshotStockData.procentChange2Year = result;
                        }
                        break;
                    case PROCENT_CHANGE_3_YEAR:
                        if(result.length() < 1)
                        {
                            QMessageBox::critical(NULL, QString::fromUtf8("Yields Parser Error"), "percent change 3 year");
                            db.m_snapshotStockData.procentChange3Year.clear();
                        }
                        else
                        {
                            result.replace(",",".");
                            db.m_snapshotStockData.procentChange3Year = result;
                        }
                        break;
                    case PROCENT_CHANGE_5_YEAR:
                        if(result.length() < 1)
                        {
                            QMessageBox::critical(NULL, QString::fromUtf8("Yields Parser Error"), "percent change 5 year");
                            db.m_snapshotStockData.procentChange5Year.clear();
                        }
                        else
                        {
                            result.replace(",",".");
                            db.m_snapshotStockData.procentChange5Year = result;

                            if(dbUpdateMode == CDbHndl::DB_MODE_INSERT_DATA)
                            {
                                db.insertOneRowYieldData(true);
                            }
                            else
                            {
                                db.updateOneRowYieldData(true);
                            }
                        }
                        break;
                    }

                    qDebug() << i << "Data:" << result << ", ";
                    i++;
                }
            }
            qDebug() << "\n";
        }
    }

    qDebug() << "close file\n";
    file.close();
    return true;
}


/****************************************************************
 *
 * Function:    yieldPageParseCompanyName()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
bool CParseYieldData::
yieldPageParseCompanyName(QTextStream &inStream, QString &result, QString startTag, QString endTagLen)
{
    QString str;
    int index;

    str = inStream.readLine(); //.toUtf8();
    //str = inStream.readLine();

    //qDebug() << "str =" << str << "\n";

    index = str.indexOf(startTag);
    if(index == -1)
        index = str.indexOf(TAG_YIELD_START_COMPANY_NAME1);
    else if(index == -1)
        index = str.indexOf(TAG_YIELD_START_COMPANY_NAME2);
    else if(index== -1)
        index = str.indexOf(TAG_YIELD_START_COMPANY_NAME3);


    // qDebug() << "startTag=" << startTag << "index=" << index << "\n";

    if(index > -1)
    {
        str = str.right(str.length() - index - startTag.length());
        //qDebug() << "result=" << result << "\n";

        // Read next line to find the termination string of compny name
        //str = inStream.readLine();

        index = str.indexOf(endTagLen);
        // qDebug() << "endtag=" << endTagLen  << "index=" << index << "\n";
        if(index > -1)
        {
            str = str.left(index);
            str = str.trimmed();
            result = str;
            //qDebug() << "result =" << result << "\n\n";
            return true;
        }
    }
    return false;
}



/****************************************************************
 *
 * Function:    parseStockData()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
bool CParseYieldData::
parseStockData(QTextStream &inStream,  QString &result, QString endTag)
{
    QString startTag;
    QString str;
    int index;
    int i;

    str = inStream.readLine(); //.toUtf8();
    //str = inStream.readLine();
    //qDebug() << "str =" << str << "\n\n";

    for(i = 0; i < MAX_NOF_START_DATA_TAGS; i++)
    {
        startTag = startDataTagArr[i];
        index = str.indexOf(startTag);

        //qDebug() << "startTag" << startTag << "str =" << str << "index" << index << "i" << i << "\n\n";
        //qDebug() << "startTag" << startTag << "str =" << str << "index" << index << "\n\n";

        if(index > -1)
        {
            if(true == findEndTag(str, endTag))
            {
                result = str.right(str.length() - str.indexOf(startTag) - startTag.length());
                result = result.left(result.length() - endTag.length());
                result = result.remove(" ");

                //qDebug() << "result =" << result << "\n\n";
                return true;
            }
        }
    }
    return false;
}


/****************************************************************
 *
 * Function:    findEndTag()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
bool CParseYieldData::findEndTag(QString str, QString &endTag)
{
    for(int i = 0; i < MAX_NOF_END_DATA_TAGS; i++)
    {
        endTag = endDataTagArr[i];
        endTag =  endTag; //.toUtf8();
        int index = str.indexOf(endTag);
        if(index > -1)
        {
            return true;
        }
    }

    return false;
}




