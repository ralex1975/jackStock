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
#define TAG_START_COMPANY_NAME QString::fromUtf8("&marketplace=11\" class=\"underline\">")
#define TAG_START_COMPANY_NAME1 QString::fromUtf8("&marketplace=14\" class=\"underline\">")
#define TAG_START_COMPANY_NAME2 QString::fromUtf8("&marketplace=15\" class=\"underline\">")
#define TAG_START_COMPANY_NAME3 QString::fromUtf8("&marketplace=24\" class=\"underline\">")


#define TAG_START_COMPANY_NAME_LEN strlen("&marketplace=11\" class=\"underline\">")
#define TAG_END_COMPANY_NAME QString::fromUtf8("</a></div></td>")

#define TAG_START_END_PART_SECOND_DATA QString::fromUtf8("</td>")


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


typedef enum
{
    STATE_IDLE,
    STATE_PARSE_NAME,
    STATE_PARSE_PE,
    STATE_PARSE_PROCENT_CHANGE,
    STATE_PARSE_PRICE_CHANGE,
    STATE_PARSE_PRICE,
    STATE_PARSE_EARNING_PER_SHARE,
    STATE_PARSE_YIELD,
    STATE_PARSE_DIVIDEND,
    STATE_PARSE_PROFIT_MARGIN,
    STATE_PARSE_CAPITAL_PER_SHARE
}Parse_state_et;

typedef struct
{
    QString name;
    QString pe;
    QString procent_change;
    QString price_change;
    QString last_price;
    QString earning_per_share;
    QString yield;
    QString dividend;
    QString profit_marging;
    QString capital_per_share;
    QString price_to_book_ratio;  // Pris / substansvärde
    QString earning_div_dividend; // Vinst / utdelning
}Parser_data;

#if 0
#define END_TAG                         "</td>"
#define STOCK_NAME_START_TAG            "<td class=\" main txt \">"
#define LAST_PE_START_TAG               "<td class=\" small \">"
#define LAST_PROCENT_CHANGE_START_TAG   "<td class=\" small quantity  mwc-color-up \">"
#define LAST_PRICE_CHANGE_START_TAG		"<td class=\" kopsalj  mwc-color-up \">"
#define LAST_PRICE_START_TAG			"<td class=\" kopsalj \">"
#define EARNING_PER_SHARE_START_TAG     "<td class=\"\">"
#define YIELD_START_TAG					"<td class=\" per \">"  // Direktavkastning
#define DIVIDEND_START_TAG              "<td class=\" diff \">"
#define PROFIT_MARGIN_START_TAG        "<td class=\" highlow \">"
#define CAPITAL_PER_SHARE_START_TAG    "<td class=\" highlow \">"
#endif

#define END_TAG                         "</td>"
#define STOCK_NAME_START_TAG            "<td   class=' main txt '  >"
#define LAST_PE_START_TAG               "<td   class=' small '  >"
#define LAST_PROCENT_CHANGE_START_TAG   "<td   class=' small quantity  mwc-color-up '  >"
#define LAST_PROCENT_CHANGE_START_TAG1  "<td   class=' small quantity  mwc-color-down '  >"
#define LAST_PROCENT_CHANGE_START_TAG2  "<td   class=' small quantity '  >"
#define LAST_PRICE_CHANGE_START_TAG		"<td   class=' kopsalj  mwc-color-up '  >"
#define LAST_PRICE_CHANGE_START_TAG1    "<td   class=' kopsalj  mwc-color-down '  >"
#define LAST_PRICE_CHANGE_START_TAG2    "<td   class=' kopsalj '  >"

#define LAST_PRICE_START_TAG			"<td   class=' kopsalj '  >"
#define EARNING_PER_SHARE_START_TAG     "<td   class=''  >"
#define YIELD_START_TAG					"<td   class=' per '  >"  // Direktavkastning
#define DIVIDEND_START_TAG              "<td   class=' diff '  >"
#define PROFIT_MARGIN_START_TAG        "<td   class=' highlow '  >"
#define CAPITAL_PER_SHARE_START_TAG    "<td   class=' highlow '  >"


QString m_filename;
QFile   m_file;
QTextStream *m_in = NULL;

#define PARSER_DATA_ARR_SIZE 500
Parser_data m_parser_data_arr[PARSER_DATA_ARR_SIZE];
int parser_data_arr_size;

Parse_state_et m_parse_state = STATE_IDLE;


//#define FILENAME "key_numbers.html"

bool parse_item(QString str, QString start_tag, QString &result);
bool parse_file(Parse_state_et &state, Parser_data *parser_data_arr, int &arr_size,
                CDbHndl &db,
                CDbHndl::DbInsertMode_ET dbUpdateMode);
bool open_file(QString filename, QFile &file, QTextStream **in);
void close_file(QFile &file, QTextStream **in);
bool read_line(QString &str, QTextStream *in);

bool parse_file(Parse_state_et &state,
                Parser_data *parser_data_arr,
                int &arr_size,
                CDbHndl &db,
                CDbHndl::DbInsertMode_ET dbUpdateMode)
{
    volatile bool res;
    QString str;
    double price;
    double dividend;
    double earning;
    double capital_per_share;


    arr_size = 0;

    while(true)
    {
        res = read_line(str, m_in);
        if(false == res)
        {
            return false;
        }

        if(arr_size >= PARSER_DATA_ARR_SIZE)
        {
            return false;
        }

        switch(state)
        {
        case STATE_IDLE:
            break;

        case STATE_PARSE_NAME:
            res = parse_item(str, STOCK_NAME_START_TAG,  parser_data_arr[arr_size].name);
            if(false == res)
            {
                //state = idle;
                //return false;
            }
            else
            {
                qDebug() << "Aktie: " << parser_data_arr[arr_size].name;
                db.m_snapshotStockData.companyName = parser_data_arr[arr_size].name;
                state = STATE_PARSE_PE;
            }
            break;

        case STATE_PARSE_PE:
            res = parse_item(str, LAST_PE_START_TAG,  parser_data_arr[arr_size].pe);
            if(false == res)
            {
                //state = idle;
                //return false;
            }
            else
            {
                qDebug() << "PE: " << parser_data_arr[arr_size].pe;
                parser_data_arr[arr_size].pe.replace(",",".");
                db.m_snapshotStockData.keyValuePE = parser_data_arr[arr_size].pe;
                state = STATE_PARSE_PROCENT_CHANGE;
            }
            break;

        case STATE_PARSE_PROCENT_CHANGE:
            res = parse_item(str, LAST_PROCENT_CHANGE_START_TAG,  parser_data_arr[arr_size].procent_change);
            if(false == res)
            {
                res = parse_item(str, LAST_PROCENT_CHANGE_START_TAG1,  parser_data_arr[arr_size].procent_change);
                if(true == res)
                {
                    qDebug() << parser_data_arr[arr_size].procent_change << " %";
                    parser_data_arr[arr_size].procent_change.replace(",",".");
                    db.m_snapshotStockData.procentChangeOneDay = parser_data_arr[arr_size].procent_change;
                    state = STATE_PARSE_PRICE_CHANGE;
                }
                else
                {
                    res = parse_item(str, LAST_PROCENT_CHANGE_START_TAG2,  parser_data_arr[arr_size].procent_change);
                    if(true == res)
                    {
                        qDebug() << parser_data_arr[arr_size].procent_change << " %";
                        parser_data_arr[arr_size].procent_change.replace(",",".");
                        db.m_snapshotStockData.procentChangeOneDay = parser_data_arr[arr_size].procent_change;
                        state = STATE_PARSE_PRICE_CHANGE;
                    }
                }
            }
            else
            {
                qDebug() << parser_data_arr[arr_size].procent_change << " %";
                parser_data_arr[arr_size].procent_change.replace(",",".");
                db.m_snapshotStockData.procentChangeOneDay = parser_data_arr[arr_size].procent_change;
                state = STATE_PARSE_PRICE_CHANGE;
            }
            break;

        case STATE_PARSE_PRICE_CHANGE:
            res = parse_item(str, LAST_PRICE_CHANGE_START_TAG,  parser_data_arr[arr_size].price_change);
            if(false == res)
            {
                res = parse_item(str, LAST_PRICE_CHANGE_START_TAG1,  parser_data_arr[arr_size].price_change);
                if(true == res)
                {
                    qDebug() << parser_data_arr[arr_size].price_change << "+/-";
                    parser_data_arr[arr_size].price_change.replace(",",".");
                    db.m_snapshotStockData.priceChange = parser_data_arr[arr_size].price_change;
                    state = STATE_PARSE_PRICE;
                }
                else
                {
                    res = parse_item(str, LAST_PRICE_CHANGE_START_TAG2,  parser_data_arr[arr_size].price_change);
                    if(true == res)
                    {
                        qDebug() << parser_data_arr[arr_size].price_change << "+/-";
                        parser_data_arr[arr_size].price_change.replace(",",".");
                        db.m_snapshotStockData.priceChange = parser_data_arr[arr_size].price_change;
                        state = STATE_PARSE_PRICE;
                    }
                }
            }
            else
            {
                qDebug() << parser_data_arr[arr_size].price_change << "+/-";
                parser_data_arr[arr_size].price_change.replace(",",".");
                db.m_snapshotStockData.priceChange = parser_data_arr[arr_size].price_change;
                state = STATE_PARSE_PRICE;
            }
            break;

        case STATE_PARSE_PRICE:
            res = parse_item(str, LAST_PRICE_START_TAG,  parser_data_arr[arr_size].last_price);
            if(false == res)
            {
                //state = idle;
                //return false;
            }
            else
            {
                qDebug() << "Senast" << parser_data_arr[arr_size].last_price;
                parser_data_arr[arr_size].last_price.replace(",",".");
                db.m_snapshotStockData.lastPrice = parser_data_arr[arr_size].last_price;
                price = parser_data_arr[arr_size].last_price.toDouble();
                state = STATE_PARSE_EARNING_PER_SHARE;
            }
            break;

        case STATE_PARSE_EARNING_PER_SHARE:
            res = parse_item(str, EARNING_PER_SHARE_START_TAG,  parser_data_arr[arr_size].earning_per_share);
            if(false == res)
            {
                //state = idle;
                //return false;
            }
            else
            {
                qDebug() << "Vinst/aktie: " << parser_data_arr[arr_size].earning_per_share;
                parser_data_arr[arr_size].earning_per_share.replace(",",".");
                db.m_snapshotStockData.keyValueEarningsPerShare = parser_data_arr[arr_size].earning_per_share;
                earning = parser_data_arr[arr_size].earning_per_share.toDouble();
                state = STATE_PARSE_YIELD;
            }
            break;

        case STATE_PARSE_YIELD:
            res = parse_item(str, YIELD_START_TAG,  parser_data_arr[arr_size].yield);
            if(false == res)
            {
                //state = idle;
                //return false;
            }
            else
            {
                qDebug() << "Direktavkastning: " << parser_data_arr[arr_size].yield;
                parser_data_arr[arr_size].yield.replace(",",".");
                db.m_snapshotStockData.keyValueYield = parser_data_arr[arr_size].yield;
                state = STATE_PARSE_DIVIDEND;
            }
            break;

        case STATE_PARSE_DIVIDEND:
            res = parse_item(str, DIVIDEND_START_TAG,  parser_data_arr[arr_size].dividend);
            if(false == res)
            {
                //state = idle;
                //return false;
            }
            else
            {
                parser_data_arr[arr_size].dividend.replace(",",".");
                db.m_snapshotStockData.keyValueDividendPerShare = parser_data_arr[arr_size].dividend;

                dividend = parser_data_arr[arr_size].dividend.toDouble();

               // QString price_to_book_ratio;  // Pris / substansvärde
                if(dividend != 0.0)
                {
                    qDebug() << "Utdelning/aktie: " << parser_data_arr[arr_size].dividend;

                    parser_data_arr[arr_size].earning_div_dividend.sprintf("%.2f",  double ((double)earning/(double)dividend));// Vinst / utdelning
                    qDebug() << "Vinst/utdelning" << parser_data_arr[arr_size].earning_div_dividend;

                    parser_data_arr[arr_size].earning_div_dividend.replace(",",".");
                    db.m_snapshotStockData.earningsDividedByDividend = parser_data_arr[arr_size].earning_div_dividend;



                }
                state = STATE_PARSE_PROFIT_MARGIN;
            }
            break;

        case STATE_PARSE_PROFIT_MARGIN:
            res = parse_item(str, PROFIT_MARGIN_START_TAG,  parser_data_arr[arr_size].profit_marging);
            if(false == res)
            {
                //state = idle;
                //return false;
            }
            else
            {
                qDebug() << "Vinstmarginal: " << parser_data_arr[arr_size].profit_marging;
                state = STATE_PARSE_CAPITAL_PER_SHARE;
            }
            break;

        case STATE_PARSE_CAPITAL_PER_SHARE:
            res = parse_item(str, CAPITAL_PER_SHARE_START_TAG,  parser_data_arr[arr_size].capital_per_share);
            if(false == res)
            {
                //state = idle;
                //return false;
            }
            else
            {
                // NAV = Substans/aktie
                capital_per_share = parser_data_arr[arr_size].capital_per_share.toDouble();
                if(capital_per_share != 0.0)
                {
                    qDebug() << "Eget kapital/aktie" << parser_data_arr[arr_size].capital_per_share;

                    // Är detta rätt?
                    parser_data_arr[arr_size].capital_per_share.replace(",",".");
                    db.m_snapshotStockData.keyValueNAVPerShare = parser_data_arr[arr_size].capital_per_share;

                    parser_data_arr[arr_size].price_to_book_ratio.sprintf("%.2f", price/capital_per_share);
                    qDebug() << "pris/eget kapital" << parser_data_arr[arr_size].price_to_book_ratio;
                    parser_data_arr[arr_size].price_to_book_ratio.replace(",",".");
                    db.m_snapshotStockData.keyValueCoursePerJEK = parser_data_arr[arr_size].price_to_book_ratio;

                    if(dbUpdateMode == CDbHndl::DB_MODE_INSERT_DATA)
                        db.insertOneRowKeyNumberData(true);
                    else
                        db.updateOneRowKeyNumberData(true);

                }
                state = STATE_PARSE_NAME;
            }
            break;
        }
    }
}


bool parse_item(QString str, QString start_tag, QString &result)
{
    int right_index;
    int left_index;
    QString tmp;


    if(-1 != str.indexOf(start_tag))
    {
        right_index = str.length() - str.indexOf(start_tag) - start_tag.size();
        if(-1 != right_index)
        {
            tmp = str.right(right_index);
            left_index = tmp.indexOf(END_TAG);
            if(-1 != left_index)
            {
                result = tmp.left(left_index);
                //qDebug() << result;
                return true;
            }
        }
    }

    return false;

}



bool open_file(QString filename, QFile &file, QTextStream **in)
{


    file.setFileName(filename);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << " Could not open the file for reading";
        return false;
    }

    *in = new QTextStream(&file);

    if(in == NULL)
    {
        return false;
    }

    return true;
}



void close_file(QFile &file, QTextStream **in)
{
    file.close();
    delete *in;
}


bool read_line(QString &str, QTextStream *in)
{
    if(in == NULL)
    {
        return false;
    }

    if(in->atEnd())
    {
        return false;
    }

    str = in->readLine();

    //qDebug() << str;
    return true;
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

    bool res;

        qDebug() << "App path: " << qApp->applicationDirPath();
        //qDebug() << "filename: " << filename;

        //filename = FILENAME;
        res = open_file(filename, m_file, &m_in);
        if(false == res)
        {
            qDebug() << "Failed to open filename: " << filename;
            return false;
        }
        m_parse_state = STATE_PARSE_NAME;

        do
        {
            res = parse_file(m_parse_state, m_parser_data_arr, parser_data_arr_size, db, dbUpdateMode);
        }
        while (true == res);

        close_file(m_file, &m_in);


        qDebug("Finish");



   #if 0
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
    //inStream.setCodec("UTF-8");

    while(!inStream.atEnd())
    {
        res = parseCompanyName(inStream, result);

        if(res)
        {

            if(result.length() < 1)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("Key Numbers Parser Error"), "Company name is empty");
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
                            QMessageBox::critical(NULL, QString::fromUtf8("Key numbers Parser Error"), "PE is empty");
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
                            QMessageBox::critical(NULL, QString::fromUtf8("Key numbers Parser Error"), "PS is empty");
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
                            QMessageBox::critical(NULL, QString::fromUtf8("Key numbers Parser Error"), "Earnings Per Share is empty");
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
                            QMessageBox::critical(NULL, QString::fromUtf8("Key numbers Parser Error"), "NAV Per Share is empty");
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
                            QMessageBox::critical(NULL, QString::fromUtf8("Key numbers Parser Error"), "Dividend Per Share is empty");
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
                            QMessageBox::critical(NULL, QString::fromUtf8("Key numbers Parser Error"), "Yield is empty");
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
                            QMessageBox::critical(NULL, QString::fromUtf8("Key numbers Parser Error"), "Course Per JEK is empty");
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
#endif
    return true;
}


#if 0
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
        QMessageBox::critical(NULL, QString::fromUtf8("Fail to open file"), errStr);
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
                QMessageBox::critical(NULL, QString::fromUtf8("Key Numbers Parser Error"), "Company name is empty");
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
                            QMessageBox::critical(NULL, QString::fromUtf8("Key numbers Parser Error"), "PE is empty");
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
                            QMessageBox::critical(NULL, QString::fromUtf8("Key numbers Parser Error"), "PS is empty");
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
                            QMessageBox::critical(NULL, QString::fromUtf8("Key numbers Parser Error"), "Earnings Per Share is empty");
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
                            QMessageBox::critical(NULL, QString::fromUtf8("Key numbers Parser Error"), "NAV Per Share is empty");
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
                            QMessageBox::critical(NULL, QString::fromUtf8("Key numbers Parser Error"), "Dividend Per Share is empty");
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
                            QMessageBox::critical(NULL, QString::fromUtf8("Key numbers Parser Error"), "Yield is empty");
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
                            QMessageBox::critical(NULL, QString::fromUtf8("Key numbers Parser Error"), "Course Per JEK is empty");
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
#endif




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




