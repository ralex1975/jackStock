/*******************************************************************
 *
 * Filename: parsePriceData.cpp
 * Date:     2012-07-08
 *
 * Description: This file reads the html page that contains stock price
 *              information.
 *
 *
 *******************************************************************/


#include "parsePriceData.h"


#include <QMessageBox>
#include <QtGui>
#include <QtCore>
#include <QString>
#include <QObject>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlError>



//#define PRICE_MAX_NOF_STOCK_DATA 10
#define TAG_START_COMPANY_NAME QObject::tr("&marketplace=11\" class=\"underline\">")
#define TAG_START_COMPANY_NAME1 QObject::tr("&marketplace=14\" class=\"underline\">")
#define TAG_START_COMPANY_NAME2 QObject::tr("&marketplace=15\" class=\"underline\">")
#define TAG_START_COMPANY_NAME3 QObject::tr("&marketplace=24\" class=\"underline\">")


#define TAG_START_COMPANY_NAME_LEN strlen("&marketplace=11\" class=\"underline\">")
#define TAG_END_COMPANY_NAME QObject::tr("</a></div></td>")

#define TAG_PRICE_START_END_PART_SECOND_DATA QObject::tr("%</DIV></TD>")

#define MAX_NOF_START_DATA_TAGS 5
#define MAX_STR_LEN_START_DATA_TAGS 200
const char startDataTagArr[MAX_NOF_START_DATA_TAGS][MAX_STR_LEN_START_DATA_TAGS] =
{
    "				<td class=\"last\" >",
    "				<td  ><span class=\"kursNeutral\">",
    "				<td  ><span class=\"kursMinus\">",
    "				<td  ><span class=\"kursPlus\">",
    "				<td  >",
};

#define MAX_NOF_END_DATA_TAGS 7
static const char endDataTagArr[MAX_NOF_END_DATA_TAGS][MAX_STR_LEN_START_DATA_TAGS] =
{
    "<img class=\"delayicon\" title=\"15 min. fördröjning\" alt=\"15 min. fördröjning\" src=\"/now/images/ikon15min.gif\" /></td>",
    " <img class=\"delayicon\" title=\"Stängningskurs\" alt=\"Stängningskurs\" src=\"/now/images/ikon_stangningskurs.gif\" /></td>",
    "<imgclass=\"delayicon\"title=\"15min.fördröjning\"alt=\"15min.fördröjning\"src=\"/now/images/ikon15min.gif\"/>",
    "%</span></td>",
    "</span></td>",
    "</td>",
    "				<td  >"

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
CParsePriceData::CParsePriceData()
{
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
bool CParsePriceData::
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
bool CParsePriceData::findEndTag(QString str, QString &endTag)
{
    for(int i = 0; i < MAX_NOF_END_DATA_TAGS; i++)
    {
        endTag = endDataTagArr[i];
        endTag =  endTag;//.toUtf8();
        int index = str.indexOf(endTag);
        if(index > -1)
        {
            return true;
        }
    }

    return false;
}

/****************************************************************
 *
 * Function:    parseStockPrices()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
bool CParsePriceData::parseStockPrices(QString filename, CDbHndl &db)
{
    QFile file(filename);
    QString errStr = QString("Fail to open file: %1").arg(filename);
    //QString str;
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
                QMessageBox::critical(NULL, QObject::tr("Price Parser Error"), "Company name is empty");
                db.m_snapshotStockData.companyName.clear();
            }
            else
            {
                db.m_snapshotStockData.companyName = result;
            }
            //qDebug() << j << "Company utf8:" << result.toUtf8().constData() << ", ";
            qDebug() << j++ << "Company:" << result << ", ";



            for(i=0; i <PRICE_MAX_NOF_STOCK_DATA;  )
            {
                res = parseStockData(inStream, result, TAG_PRICE_START_END_PART_SECOND_DATA);
                if(res)
                {
                    switch(i)
                    {
                    case LAST_PRICE:
                        if(result.length() < 1)
                        {
                            QMessageBox::critical(NULL, QObject::tr("Price Parser Error"), "Last price is empty");
                            db.m_snapshotStockData.lastPrice.clear();
                        }
                        else
                        {
                            result.replace(",",".");
                            db.m_snapshotStockData.lastPrice = result;
                        }
                        break;
                    case PRICE_CHANGE:
                        if(result.length() < 1)
                        {
                            QMessageBox::critical(NULL, QObject::tr("Price Parser Error"), "Price change is empty");
                            db.m_snapshotStockData.priceChange.clear();
                        }
                        else
                        {
                            result.replace(",",".");
                            db.m_snapshotStockData.priceChange = result;
                        }
                        break;
                    case PROCENT_CHANGE_ONE_DAY:
                        if(result.length() < 1)
                        {
                            QMessageBox::critical(NULL, QObject::tr("Price Parser Error"), "Price change one day is empty");
                            db.m_snapshotStockData.procentChangeOneDay.clear();
                        }
                        else
                        {
                            result.replace(",",".");
                            db.m_snapshotStockData.procentChangeOneDay = result;
                        }
                        break;
                    // Köp pris
                    case BID_PRICE:
                        if(result.length() < 1)
                        {
                            QMessageBox::critical(NULL, QObject::tr("Price Parser Error"), "Bid Price is empty");
                            db.m_snapshotStockData.bidPrice.clear();
                        }
                        else
                        {
                            result.replace(",",".");
                            db.m_snapshotStockData.bidPrice = result;
                        }
                        break;
                    // Sälj pris
                    case OFFER_PRICE:
                        if(result.length() < 1)
                        {
                            QMessageBox::critical(NULL, QObject::tr("Price Parser Error"), "Offer Price is empty");
                            db.m_snapshotStockData.offerPrice.clear();
                        }
                        else
                        {
                            result.replace(",",".");
                            db.m_snapshotStockData.offerPrice = result;
                        }
                        break;
                    case HIGHEST_PRICE:
                        if(result.length() < 1)
                        {
                            QMessageBox::critical(NULL, QObject::tr("Price Parser Error"), "Highest Price is empty");
                            db.m_snapshotStockData.highestPrice.clear();
                        }
                        else
                        {
                            result.replace(",",".");
                            db.m_snapshotStockData.highestPrice = result;
                        }
                        break;
                    case LOWEST_PRICE:
                        if(result.length() < 1)
                        {
                            QMessageBox::critical(NULL, QObject::tr("Price Parser Error"), "Lowest Price is empty");
                            db.m_snapshotStockData.lowestPrice.clear();
                        }
                        else
                        {
                            result.replace(",",".");
                            db.m_snapshotStockData.lowestPrice = result;
                        }
                        break;
                    case VOLUME:
                        if(result.length() < 1)
                        {
                            QMessageBox::critical(NULL, QObject::tr("Price Parser Error"), "Volume is empty");
                            db.m_snapshotStockData.volume.clear();
                        }
                        else
                        {
                            db.m_snapshotStockData.volume = result;
                        }
                        break;
                    case CURRENCY:
                        if(result.length() < 1)
                        {
                            QMessageBox::critical(NULL, QObject::tr("Price Parser Error"), "Currency is empty");
                            db.m_snapshotStockData.currency.clear();
                        }
                        else
                        {
                            result.replace(",",".");
                            db.m_snapshotStockData.currency = result;
                        }
                        break;
                    case TIME:
                        if(result.length() < 1)
                        {
                            QMessageBox::critical(NULL, QObject::tr("Price Parser Error"), "Time is empty");
                            db.m_snapshotStockData.time.clear();
                        }
                        else
                        {
                            db.m_snapshotStockData.time = result;
                            if(false == db.insertOneRowPriceData(true))
                                return false;
                        }
                        break;
                    default:
                        QMessageBox::critical(NULL, QObject::tr("Parser Error"),  QObject::tr("Invalid parsing state"));
                    }

                    qDebug() << i << "Data:" << result << ", ";
                    i++;
                }
            }
            //exit(0);
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
bool CParsePriceData::
parseCompanyName(QTextStream &inStream, QString &result)
{
    QString str;
    int index;

    str = inStream.readLine();
    //qDebug() << "str 123 =" << str << "\n";

    //str = str.toUtf8();
    //qDebug() << "str 456 =" << str << "\n";
    //str = inStream.readLine();

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
        result = str.right(str.length()- index - TAG_START_COMPANY_NAME_LEN);

        // Read next line to find the termination string of compny name
        //str = inStream.readLine();

        index = result.indexOf(TAG_END_COMPANY_NAME);
        if(index > -1)
        {
            result = result.left(index);
            result = result.trimmed();
           // result += str;
            qDebug() << "result =" << result << "\n\n";
            return true;
        }
    }


    return false;
}





