/*******************************************************************
 *
 * Filename: parseKeyNumberData.cpp
 * Date:     2012-07-08
 *
 * Description: This file reads the html page that contains stock key
 *              number information.
 *
 *
 *******************************************************************/
#include "parseKeyNumberData.h"

#include <QMessageBox>
#include <QtCore>
#include <QString>
#include <QObject>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlError>



//#define MAX_NOF_STOCK_DATA 7
#define TAG_START_COMPANY_NAME QObject::tr("&marketplace=11\" class=\"underline\">")
#define TAG_START_COMPANY_NAME1 QObject::tr("&marketplace=14\" class=\"underline\">")
#define TAG_START_COMPANY_NAME2 QObject::tr("&marketplace=15\" class=\"underline\">")
#define TAG_START_COMPANY_NAME3 QObject::tr("&marketplace=24\" class=\"underline\">")


#define TAG_START_COMPANY_NAME_LEN strlen("&marketplace=11\" class=\"underline\">")
#define TAG_END_COMPANY_NAME QObject::tr("</a></div></td>")

#define TAG_START_END_PART_SECOND_DATA QObject::tr("</td>")


#define MAX_NOF_START_DATA_TAGS 2
#define MAX_STR_LEN_START_DATA_TAGS 27
const char startDataTagArr[MAX_NOF_START_DATA_TAGS][MAX_STR_LEN_START_DATA_TAGS] =
{
    "				<td class=\"last\" >",
    "				<td  >",
};



/****************************************************************
 *
 * Function:    CParsePriceData()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
CParseKeyNumberData::CParseKeyNumberData()
{
}




/****************************************************************
 *
 * Function:    parseStockKeyNumberData()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
bool CParseKeyNumberData::
parseStockKeyNumberData(QString filename, CDbHndl &db, CDbHndl::DbInsertMode_ET dbUpdateMode)
{
    QFile file(filename);
    QString errStr = QString("Fail to open file: %1").arg(filename);
    QString result;
    bool res;
    int i;
    int j=0;


    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::critical(NULL, QObject::tr("Fail to open file"), errStr);
        return false;
    }

    QTextStream inStream(&file);
     inStream.setCodec("ISO 8859-1");
    //inStream.setCodec("UTF-8");

    while(!inStream.atEnd())
    {
        res = parseCompanyName(inStream, result);

        if(res)
        {

            if(result.length() < 1)
            {
                QMessageBox::critical(NULL, QObject::tr("Key Numbers Parser Error"), "Company name is empty");
                db.m_snapshotStockData.companyName.clear();
            }
            else
            {
                db.m_snapshotStockData.companyName = result;
            }

            qDebug() << j++ << "Company:" << result << ", ";

            for(i=0; i < MAX_NOF_STOCK_DATA;  )
            {
                res = parseStockData(inStream, result, TAG_START_END_PART_SECOND_DATA);
                if(res)
                {
                    switch(i)
                    {
                    case KEY_VALUE_PE:
                        if(result.length() < 1)
                        {
                            QMessageBox::critical(NULL, QObject::tr("Key numbers Parser Error"), "PE is empty");
                            db.m_snapshotStockData.keyValuePE.clear();
                        }
                        else
                        {
                            result.replace(",",".");
                            db.m_snapshotStockData.keyValuePE = result;
                        }
                        break;
                    case KEY_VALUE_PS:
                        if(result.length() < 1)
                        {
                            QMessageBox::critical(NULL, QObject::tr("Key numbers Parser Error"), "PS is empty");
                            db.m_snapshotStockData.keyValuePS.clear();
                        }
                        else
                        {
                            result.replace(",",".");
                            db.m_snapshotStockData.keyValuePS = result;
                        }
                        break;
                    case KEY_VALUE_EARNINGS_PER_SHARE:
                        if(result.length() < 1)
                        {
                            QMessageBox::critical(NULL, QObject::tr("Key numbers Parser Error"), "Earnings Per Share is empty");
                            db.m_snapshotStockData.keyValueEarningsPerShare.clear();
                        }
                        else
                        {
                            result.replace(",",".");
                            db.m_snapshotStockData.keyValueEarningsPerShare = result;
                        }
                        break;
                    case KEY_VALUE_NAV_PER_SHARE:
                        if(result.length() < 1)
                        {
                            QMessageBox::critical(NULL, QObject::tr("Key numbers Parser Error"), "NAV Per Share is empty");
                            db.m_snapshotStockData.keyValueNAVPerShare.clear();
                        }
                        else
                        {
                            result.replace(",",".");
                            db.m_snapshotStockData.keyValueNAVPerShare = result;
                        }
                        break;
                    case KEY_VALUE_DIVIDEND_PER_SHARE:
                        if(result.length() < 1)
                        {
                            QMessageBox::critical(NULL, QObject::tr("Key numbers Parser Error"), "Dividend Per Share is empty");
                            db.m_snapshotStockData.keyValueDividendPerShare.clear();
                        }
                        else
                        {
                            result.replace(",",".");
                            db.m_snapshotStockData.keyValueDividendPerShare = result;
                        }
                        break;
                    case KEY_VALUE_YIELD:
                        if(result.length() < 1)
                        {
                            QMessageBox::critical(NULL, QObject::tr("Key numbers Parser Error"), "Yield is empty");
                            db.m_snapshotStockData.keyValueYield.clear();
                        }
                        else
                        {
                            result.replace(",",".");
                            db.m_snapshotStockData.keyValueYield = result;
                        }
                        break;
                    case KEY_VALUE_COURSE_PER_JEK:
                        if(result.length() < 1)
                        {
                            QMessageBox::critical(NULL, QObject::tr("Key numbers Parser Error"), "Course Per JEK is empty");
                            db.m_snapshotStockData.keyValueCoursePerJEK.clear();
                        }
                        else
                        {
                            result.replace(",",".");
                            db.m_snapshotStockData.keyValueCoursePerJEK = result;

                            if(dbUpdateMode == CDbHndl::DB_MODE_INSERT_DATA)
                                db.insertOneRowKeyNumberData(true);
                            else
                                db.updateOneRowKeyNumberData(true);
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
 * Function:    parseCompanyName()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
bool CParseKeyNumberData::
parseCompanyName(QTextStream &inStream, QString &result)
{
    QString str;
    int index;

    str = inStream.readLine(); //.toUtf8();
    //qDebug() << "str =" << str << "\n";

    index = str.indexOf(TAG_START_COMPANY_NAME);
    if(index == -1)
        index = str.indexOf(TAG_START_COMPANY_NAME1);
    if(index == -1)
        index = str.indexOf(TAG_START_COMPANY_NAME2);
    if(index == -1)
        index = str.indexOf(TAG_START_COMPANY_NAME3);


    //qDebug() << "index =" << index << "\n";

    if(index > -1)
    {
        result = str.right(str.length() - index - TAG_START_COMPANY_NAME_LEN);

        index = result.indexOf(TAG_END_COMPANY_NAME);
        if(index > -1)
        {
            result = result.left(index);
            result = result.trimmed();
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
bool CParseKeyNumberData::
parseStockData(QTextStream &inStream,  QString &result, QString endTag)
{
    QString startTag;
    QString str;
    int index;
    int i;


    str = inStream.readLine();//.toUtf8();
    //str = inStream.readLine();

    for(i = 0; i < MAX_NOF_START_DATA_TAGS; i++)
    {
        startTag = startDataTagArr[i];
        index = str.indexOf(startTag);

        //qDebug() << "startTag" << startTag << "str =" << str << "index" << index << "\n\n";
        //qDebug() << "startTag" << startTag << "str =" << str << "index" << index << "\n\n";

        // Time to remove end tag
        if(index > -1)
        {

            index = str.indexOf("</td>");

            if(index == -1)
            {
                index = str.indexOf(endTag);
            }
            else
            {
                endTag = "</td>";
            }


            index = str.indexOf(endTag);
            //qDebug() << "endtag=" << endTag  << "index=" << index << "\n";

            if(index > -1)
            {
                result = str.right(str.length() - startTag.length());
                result = result.left(result.length() - endTag.length());

                //qDebug() << "result =" << result << "\n\n";
                return true;
            }
        }
    }

    return false;

}




