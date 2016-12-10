/*******************************************************************
 *
 * Filename: dbHndl.h
 * Date:     2012-07-08
 *
 * Description: Handles the sqlite databases
 *
 *              In the Sqlite database is all data stored in UTF-8
 *              formate.
 *
 *******************************************************************/


#include "dbHndl.h"
#include <QMessageBox>
#include "extendedTable.h"
#include <stdlib.h>
#include "inc/guiUtil/guiUtils.h"
#include <stdlib.h>
#include "linkedList.h"
#include "debug.h"
#include "util.h"
#include "common.h"


//#pragma journal_mode = memory;
//#pragma synchronous = off;

//QStringList _filter1DividendSortColParamLst;
TableColumnIndicatorInfo_ST m_tableHeaderList[CDbHndl::MAX_NOF_GPSEL_INDEX];
int m_maxTableColumns;


const quint64 CDbHndl::MASK_GPSEL_NOT_USED =                       0x00000000;    // 0
const quint64 CDbHndl::MASK_GPSEL_STOCK_NAME =                     0x00000001;    // 26
const quint64 CDbHndl::MASK_GPSEL_STOCK_PRICE =                    0x00000002;    // 1
const quint64 CDbHndl::MASK_GPSEL_ONE_DAY_PRICE_CHANGE  =          0x00000004;    // 2
const quint64 CDbHndl::MASK_GPSEL_BID_PRICE =                      0x00000008;    // 4
const quint64 CDbHndl::MASK_GPSEL_OFFER_PRICE =                    0x00000010;    // 5
const quint64 CDbHndl::MASK_GPSEL_HIGHEST_PRICE =                  0x00000020;    // 6
const quint64 CDbHndl::MASK_GPSEL_LOWEST_PRICE =                   0x00000040;    // 7
const quint64 CDbHndl::MASK_GPSEL_VOLUME =                         0x00000080;    // 8
const quint64 CDbHndl::MASK_GPSEL_CURRENCY =                       0x00000100;    // 9
const quint64 CDbHndl::MASK_GPSEL_TIME =                           0x00000200;    // 10
const quint64 CDbHndl::MASK_GPSEL_PE =                             0x00000400;    // 11
const quint64 CDbHndl::MASK_GPSEL_PS =                             0x00000800;    // 12
const quint64 CDbHndl::MASK_GPSEL_EARNING_DIVIDED_BY_DIVIDEND =    0x00001000;    // 27
const quint64 CDbHndl::MASK_GPSEL_NAV_DIV_LAST_STOCK_PRICE =       0x00002000;    // 28
const quint64 CDbHndl::MASK_GPSEL_EARN_PER_SHARE =                 0x00004000;    // 13
const quint64 CDbHndl::MASK_GPSEL_NAV_PER_SHARE =                  0x00008000;    // 14
const quint64 CDbHndl::MASK_GPSEL_DIV_PER_SHARE =                  0x00010000;    // 15
const quint64 CDbHndl::MASK_GPSEL_YIELD =                          0x00020000;    // 16
const quint64 CDbHndl::MASK_GPSEL_COURSE_PER_NET_EQUITY =          0x00040000;    // 17
const quint64 CDbHndl::MASK_GPSEL_ONE_DAY_PERCENT_CHANGE =         0x00080000;    // 3
const quint64 CDbHndl::MASK_GPSEL_ONE_WEEK_PROCENT_CHANGE =        0x00100000;    // 18
const quint64 CDbHndl::MASK_GPSEL_ONE_MONTH_PERCENT_CHANGE =       0x00200000;    // 19
const quint64 CDbHndl::MASK_GPSEL_THREE_MONTH_PERCENT_CHANGE =     0x00400000;    // 20
const quint64 CDbHndl::MASK_GPSEL_SIX_MONTH_PERCENT_CHANGE  =      0x00800000;    // 21
const quint64 CDbHndl::MASK_GPSEL_ONE_YEAR_PERCENT_CHANGE =        0x01000000;    // 22
const quint64 CDbHndl::MASK_GPSEL_TWO_YEAR_PERCENT_CHANGE =        0x02000000;    // 23
const quint64 CDbHndl::MASK_GPSEL_THREE_YEAR_PERCENT_CHANGE =      0x04000000;    // 24
const quint64 CDbHndl::MASK_GPSEL_FIVE_YEAR_PERCENT_CHANGE =       0x08000000;    // 25


//==========================================================================================
// Add user defined GUI parameters here:
//(This parameters is added as columns in tables and selectable items in comboBoxes)
// only valid value per nibble is 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
// max value is 0x80 00 00 00 00 00 00 00 (16 bytes)
//==========================================================================================
const quint64 MASK_GPSEL_MAX_NOF_GUI_PARAMETERS = CDbHndl::MASK_GPSEL_FIVE_YEAR_PERCENT_CHANGE;


#if 0
const CDbHndl::StockIndicatorlLookUpTable_ST CDbHndl::m_gpSelLookUpTable[CDbHndl::MAX_NOF_GPSEL_INDEX]=
{
    {QString::fromUtf8(" "),                   CDbHndl::MASK_GPSEL_NOT_USED,                       " ",                        SQL_STR_ASC, SISORT_TYPE_FLOAT,},                                   // GPSEL_INDEX_NOT_USED,                        // 0
    {QString::fromUtf8("Aktie"),               CDbHndl::MASK_GPSEL_STOCK_NAME,                     "companyName",              SQL_STR_ASC, SISORT_TYPE_FLOAT,},        // GPSEL_INDEX_FIVE_YEAR_PERCENT_CHANGE,
    {QString::fromUtf8("Senast"),              CDbHndl::MASK_GPSEL_STOCK_PRICE,                    "lastPrice",                SQL_STR_ASC, SISORT_TYPE_FLOAT,},                   // GPSEL_INDEX_STOCK_PRICE,                     // 1
    {QString::fromUtf8("+/-"),                 CDbHndl::MASK_GPSEL_ONE_DAY_PRICE_CHANGE,           "priceChange",              SQL_STR_ASC, SISORT_TYPE_FLOAT,},           // GPSEL_INDEX_ONE_DAY_PRICE_CHANGE,            // 2
    {QString::fromUtf8("Köp"),                 CDbHndl::MASK_GPSEL_BID_PRICE,                      "bidPrice",                 SQL_STR_ASC, SISORT_TYPE_FLOAT,},                         // GPSEL_INDEX_BID_PRICE,                       // 4
    {QString::fromUtf8("Sälj"),                CDbHndl::MASK_GPSEL_OFFER_PRICE,                    "offerPrice",               SQL_STR_ASC, SISORT_TYPE_FLOAT,},                    // GPSEL_INDEX_OFFER_PRICE,                     // 5
    {QString::fromUtf8("Högst"),               CDbHndl::MASK_GPSEL_HIGHEST_PRICE,                  "highestPrice",             SQL_STR_ASC, SISORT_TYPE_FLOAT,},               // GPSEL_INDEX_HIGHEST_PRICE,                   // 6
    {QString::fromUtf8("Lägst"),               CDbHndl::MASK_GPSEL_LOWEST_PRICE,                   "lowestPrice",              SQL_STR_ASC, SISORT_TYPE_FLOAT,},                // GPSEL_INDEX_LOWEST_PRICE,                    // 7
    {QString::fromUtf8("Omsatt"),              CDbHndl::MASK_GPSEL_VOLUME,                         "volume",                   SQL_STR_ASC, SISORT_TYPE_FLOAT,},                           // GPSEL_INDEX_VOLUME,                          // 8
    {QString::fromUtf8("Valuta"),              CDbHndl::MASK_GPSEL_CURRENCY,                       "currency",                 SQL_STR_ASC, SISORT_TYPE_FLOAT,},                       // GPSEL_INDEX_CURRENCY,                        // 9
    {QString::fromUtf8("Tid"),                 CDbHndl::MASK_GPSEL_TIME,                           "time",                     SQL_STR_ASC, SISORT_TYPE_FLOAT,},                                  // GPSEL_INDEX_TIME,                            // 10
    {QString::fromUtf8("P/E"),                 CDbHndl::MASK_GPSEL_PE,                             "keyValuePE",               SQL_STR_ASC, SISORT_TYPE_FLOAT,},                              // GPSEL_INDEX_PE,                              // 11
    {QString::fromUtf8("P/S"),                 CDbHndl::MASK_GPSEL_PS,                             "keyValuePS",               SQL_STR_ASC, SISORT_TYPE_FLOAT,},                              // GPSEL_INDEX_PS,                              // 12
    {QString::fromUtf8("Vinst/Utdelning"),     CDbHndl::MASK_GPSEL_EARNING_DIVIDED_BY_DIVIDEND,    "earningsDividedByDividend",SQL_STR_ASC, SISORT_TYPE_FLOAT,},        // GPSEL_INDEX_FIVE_YEAR_PERCENT_CHANGE,
    {QString::fromUtf8("Substans/kurs"),       CDbHndl::MASK_GPSEL_NAV_DIV_LAST_STOCK_PRICE,       "navDivLastStockPrice",     SQL_STR_ASC, SISORT_TYPE_FLOAT,},        // GPSEL_INDEX_FIVE_YEAR_PERCENT_CHANGE,
    {QString::fromUtf8("Vinst/aktie"),         CDbHndl::MASK_GPSEL_EARN_PER_SHARE,                 "keyValueEarningsPerShare", SQL_STR_ASC, SISORT_TYPE_FLOAT,},    // GPSEL_INDEX_EARN_PER_SHARE,                  // 13
    {QString::fromUtf8("Substans/aktie"),      CDbHndl::MASK_GPSEL_NAV_PER_SHARE,                  "keyValueNAVPerShare",      SQL_STR_ASC, SISORT_TYPE_FLOAT,},       // GPSEL_INDEX_NAV_PER_SHARE,                    // 14 //net asset
    {QString::fromUtf8("Utdel/aktie"),         CDbHndl::MASK_GPSEL_DIV_PER_SHARE,                  "keyValueDividendPerShare", SQL_STR_ASC, SISORT_TYPE_FLOAT,},  // GPSEL_INDEX_DIV_PER_SHARE,                   // 15
    {QString::fromUtf8("Direktavkast"),        CDbHndl::MASK_GPSEL_YIELD,                          "keyValueYield",            SQL_STR_ASC, SISORT_TYPE_FLOAT,},                   // GPSEL_INDEX_YIELD,                           // 16
    {QString::fromUtf8("Kurs/JEK"),            CDbHndl::MASK_GPSEL_COURSE_PER_NET_EQUITY,          "keyValueCoursePerJEK",     SQL_STR_ASC, SISORT_TYPE_FLOAT,},    // GPSEL_INDEX_COURSE_PER_NET_EQUITY,           // 17
    {QString::fromUtf8("Idag"),                CDbHndl::MASK_GPSEL_ONE_DAY_PERCENT_CHANGE,         "procentChangeOneDay",      SQL_STR_ASC, SISORT_TYPE_FLOAT,}, // GPSEL_INDEX_ONE_DAY_PERCENT_CHANGE,          // 3
    {QString::fromUtf8("En vecka"),            CDbHndl::MASK_GPSEL_ONE_WEEK_PROCENT_CHANGE,        "procentChangeOneWeek",     SQL_STR_ASC, SISORT_TYPE_FLOAT,},   // GPSEL_INDEX_ONE_WEEK_PROCENT_CHANGE,         // 18
    {QString::fromUtf8("En månad"),            CDbHndl::MASK_GPSEL_ONE_MONTH_PERCENT_CHANGE,       "procentChange1Month",      SQL_STR_ASC, SISORT_TYPE_FLOAT,},  // GPSEL_INDEX_ONE_MONTH_PERCENT_CHANGE,        // 19
    {QString::fromUtf8("3 månader"),           CDbHndl::MASK_GPSEL_THREE_MONTH_PERCENT_CHANGE,     "procentChange3Month",      SQL_STR_ASC, SISORT_TYPE_FLOAT,}, // GPSEL_INDEX_THREE_MONTH_PERCENT_CHANGE,      // 20
    {QString::fromUtf8("6 månader"),           CDbHndl::MASK_GPSEL_SIX_MONTH_PERCENT_CHANGE,       "procentChange6Month",      SQL_STR_ASC, SISORT_TYPE_FLOAT,},   // GPSEL_INDEX_SIX_MONTH_PERCENT_CHANGE,        // 21
    {QString::fromUtf8("1 år"),                CDbHndl::MASK_GPSEL_ONE_YEAR_PERCENT_CHANGE,        "procentChange1Year",       SQL_STR_ASC, SISORT_TYPE_FLOAT,},        // GPSEL_INDEX_ONE_YEAR_PERCENT_CHANGE,         // 22
    {QString::fromUtf8("2 år"),                CDbHndl::MASK_GPSEL_TWO_YEAR_PERCENT_CHANGE,        "procentChange2Year",       SQL_STR_ASC, SISORT_TYPE_FLOAT,},        // GPSEL_INDEX_TWO_YEAR_PERCENT_CHANGE,         // 23
    {QString::fromUtf8("3 år"),                CDbHndl::MASK_GPSEL_THREE_YEAR_PERCENT_CHANGE,      "procentChange3Year",       SQL_STR_ASC, SISORT_TYPE_FLOAT,},      // GPSEL_INDEX_THREE_YEAR_PERCENT_CHANGE,       // 24
    {QString::fromUtf8("5 år"),                CDbHndl::MASK_GPSEL_FIVE_YEAR_PERCENT_CHANGE,       "procentChange5Year",       SQL_STR_ASC, SISORT_TYPE_FLOAT}        // GPSEL_INDEX_FIVE_YEAR_PERCENT_CHANGE,
};
#endif

const CDbHndl::StockIndicatorlLookUpTable_ST CDbHndl::m_gpSelLookUpTable[CDbHndl::MAX_NOF_GPSEL_INDEX]=
{
    {QString::fromUtf8(" "),                   CDbHndl::MASK_GPSEL_NOT_USED,                       " ",                        SQL_STR_ASC, SISORT_TYPE_FLOAT,},                                   // GPSEL_INDEX_NOT_USED,                        // 0
    {QString::fromUtf8("Aktie"),               CDbHndl::MASK_GPSEL_STOCK_NAME,                     "companyName",              SQL_STR_ASC, SISORT_TYPE_TEXT,},        // GPSEL_INDEX_FIVE_YEAR_PERCENT_CHANGE,
    {QString::fromUtf8("Senast"),              CDbHndl::MASK_GPSEL_STOCK_PRICE,                    "lastPrice",                SQL_STR_ASC, SISORT_TYPE_FLOAT,},                   // GPSEL_INDEX_STOCK_PRICE,                     // 1
    {QString::fromUtf8("+/-"),                 CDbHndl::MASK_GPSEL_ONE_DAY_PRICE_CHANGE,           "priceChange",              SQL_STR_ASC, SISORT_TYPE_FLOAT,},           // GPSEL_INDEX_ONE_DAY_PRICE_CHANGE,            // 2
    {QString::fromUtf8("Kop"),                 CDbHndl::MASK_GPSEL_BID_PRICE,                      "bidPrice",                 SQL_STR_ASC, SISORT_TYPE_FLOAT,},                         // GPSEL_INDEX_BID_PRICE,                       // 4
    {QString::fromUtf8("Salj"),                CDbHndl::MASK_GPSEL_OFFER_PRICE,                    "offerPrice",               SQL_STR_ASC, SISORT_TYPE_FLOAT,},                    // GPSEL_INDEX_OFFER_PRICE,                     // 5
    {QString::fromUtf8("Hogst"),               CDbHndl::MASK_GPSEL_HIGHEST_PRICE,                  "highestPrice",             SQL_STR_ASC, SISORT_TYPE_FLOAT,},               // GPSEL_INDEX_HIGHEST_PRICE,                   // 6
    {QString::fromUtf8("Lagst"),               CDbHndl::MASK_GPSEL_LOWEST_PRICE,                   "lowestPrice",              SQL_STR_ASC, SISORT_TYPE_FLOAT,},                // GPSEL_INDEX_LOWEST_PRICE,                    // 7
    {QString::fromUtf8("Omsatt"),              CDbHndl::MASK_GPSEL_VOLUME,                         "volume",                   SQL_STR_ASC, SISORT_TYPE_FLOAT,},                           // GPSEL_INDEX_VOLUME,                          // 8
    {QString::fromUtf8("Valuta"),              CDbHndl::MASK_GPSEL_CURRENCY,                       "currency",                 SQL_STR_ASC, SISORT_TYPE_FLOAT,},                       // GPSEL_INDEX_CURRENCY,                        // 9
    {QString::fromUtf8("Tid"),                 CDbHndl::MASK_GPSEL_TIME,                           "time",                     SQL_STR_ASC, SISORT_TYPE_FLOAT,},                                  // GPSEL_INDEX_TIME,                            // 10
    {QString::fromUtf8("P/E"),                 CDbHndl::MASK_GPSEL_PE,                             "keyValuePE",               SQL_STR_ASC, SISORT_TYPE_FLOAT,},                              // GPSEL_INDEX_PE,                              // 11
    {QString::fromUtf8("P/S"),                 CDbHndl::MASK_GPSEL_PS,                             "keyValuePS",               SQL_STR_ASC, SISORT_TYPE_FLOAT,},                              // GPSEL_INDEX_PS,                              // 12
    {QString::fromUtf8("Vinst/Utdel"),         CDbHndl::MASK_GPSEL_EARNING_DIVIDED_BY_DIVIDEND,    "earningsDividedByDividend",SQL_STR_ASC, SISORT_TYPE_FLOAT,},        // GPSEL_INDEX_FIVE_YEAR_PERCENT_CHANGE,
    {QString::fromUtf8("Substans/kurs"),       CDbHndl::MASK_GPSEL_NAV_DIV_LAST_STOCK_PRICE,       "navDivLastStockPrice",     SQL_STR_ASC, SISORT_TYPE_FLOAT,},        // GPSEL_INDEX_FIVE_YEAR_PERCENT_CHANGE,
    {QString::fromUtf8("Vinst/aktie"),         CDbHndl::MASK_GPSEL_EARN_PER_SHARE,                 "keyValueEarningsPerShare", SQL_STR_ASC, SISORT_TYPE_FLOAT,},    // GPSEL_INDEX_EARN_PER_SHARE,                  // 13
    {QString::fromUtf8("Substans/aktie"),      CDbHndl::MASK_GPSEL_NAV_PER_SHARE,                  "keyValueNAVPerShare",      SQL_STR_ASC, SISORT_TYPE_FLOAT,},       // GPSEL_INDEX_NAV_PER_SHARE,                    // 14 //net asset
    {QString::fromUtf8("Utdel/aktie"),         CDbHndl::MASK_GPSEL_DIV_PER_SHARE,                  "keyValueDividendPerShare", SQL_STR_ASC, SISORT_TYPE_FLOAT,},  // GPSEL_INDEX_DIV_PER_SHARE,                   // 15
    {QString::fromUtf8("Direktavk"),           CDbHndl::MASK_GPSEL_YIELD,                          "keyValueYield",            SQL_STR_ASC, SISORT_TYPE_FLOAT,},                   // GPSEL_INDEX_YIELD,                           // 16
    {QString::fromUtf8("Kurs/JEK"),            CDbHndl::MASK_GPSEL_COURSE_PER_NET_EQUITY,          "keyValueCoursePerJEK",     SQL_STR_ASC, SISORT_TYPE_FLOAT,},    // GPSEL_INDEX_COURSE_PER_NET_EQUITY,           // 17
    {QString::fromUtf8("Idag"),                CDbHndl::MASK_GPSEL_ONE_DAY_PERCENT_CHANGE,         "procentChangeOneDay",      SQL_STR_ASC, SISORT_TYPE_FLOAT,}, // GPSEL_INDEX_ONE_DAY_PERCENT_CHANGE,          // 3
    {QString::fromUtf8("1 vecka"),             CDbHndl::MASK_GPSEL_ONE_WEEK_PROCENT_CHANGE,        "procentChangeOneWeek",     SQL_STR_ASC, SISORT_TYPE_FLOAT,},   // GPSEL_INDEX_ONE_WEEK_PROCENT_CHANGE,         // 18
    {QString::fromUtf8("1 man"),               CDbHndl::MASK_GPSEL_ONE_MONTH_PERCENT_CHANGE,       "procentChange1Month",      SQL_STR_ASC, SISORT_TYPE_FLOAT,},  // GPSEL_INDEX_ONE_MONTH_PERCENT_CHANGE,        // 19
    {QString::fromUtf8("3 man"),               CDbHndl::MASK_GPSEL_THREE_MONTH_PERCENT_CHANGE,     "procentChange3Month",      SQL_STR_ASC, SISORT_TYPE_FLOAT,}, // GPSEL_INDEX_THREE_MONTH_PERCENT_CHANGE,      // 20
    {QString::fromUtf8("6 man"),               CDbHndl::MASK_GPSEL_SIX_MONTH_PERCENT_CHANGE,       "procentChange6Month",      SQL_STR_ASC, SISORT_TYPE_FLOAT,},   // GPSEL_INDEX_SIX_MONTH_PERCENT_CHANGE,        // 21
    {QString::fromUtf8("1 ar"),                CDbHndl::MASK_GPSEL_ONE_YEAR_PERCENT_CHANGE,        "procentChange1Year",       SQL_STR_ASC, SISORT_TYPE_FLOAT,},        // GPSEL_INDEX_ONE_YEAR_PERCENT_CHANGE,         // 22
    {QString::fromUtf8("2 ar"),                CDbHndl::MASK_GPSEL_TWO_YEAR_PERCENT_CHANGE,        "procentChange2Year",       SQL_STR_ASC, SISORT_TYPE_FLOAT,},        // GPSEL_INDEX_TWO_YEAR_PERCENT_CHANGE,         // 23
    {QString::fromUtf8("3 ar"),                CDbHndl::MASK_GPSEL_THREE_YEAR_PERCENT_CHANGE,      "procentChange3Year",       SQL_STR_ASC, SISORT_TYPE_FLOAT,},      // GPSEL_INDEX_THREE_YEAR_PERCENT_CHANGE,       // 24
    {QString::fromUtf8("5 ar"),                CDbHndl::MASK_GPSEL_FIVE_YEAR_PERCENT_CHANGE,       "procentChange5Year",       SQL_STR_ASC, SISORT_TYPE_FLOAT}        // GPSEL_INDEX_FIVE_YEAR_PERCENT_CHANGE,
};


const unsigned short CDbHndl::MASK_GPCOMP_GREATER =            0x01;
const unsigned short CDbHndl::MASK_GPCOMP_GREATER_OR_EQUAL =   0x02;
const unsigned short CDbHndl::MASK_GPCOMP_EQUAL =              0x04;
const unsigned short CDbHndl::MASK_GPCOMP_LESS_OR_EQUAL =      0x08;
const unsigned short CDbHndl::MASK_GPCOMP_LESS =               0x10;
const unsigned short CDbHndl::MASK_GPCOMP_MAX_NOF_GUI_PARAMS = CDbHndl::MASK_GPCOMP_LESS;


const CDbHndl::GPCompLookUpTable_ST CDbHndl::m_gpCompLookUpTable[MAX_NOF_GPCOMP_INDEX]=
{
    {">",  CDbHndl::MASK_GPCOMP_GREATER,},
    {">=", CDbHndl::MASK_GPCOMP_GREATER_OR_EQUAL,},
    {"==", CDbHndl::MASK_GPCOMP_EQUAL,},
    {"<=", CDbHndl::MASK_GPCOMP_LESS_OR_EQUAL,},
    {"<",  CDbHndl::MASK_GPCOMP_LESS,}
};




/****************************************************************
 *
 * Function:    CDbHndl()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
CDbHndl::CDbHndl():
    m_disableMsgBoxes(false)
{

}

/****************************************************************
 *
 * Function:        disableMsgBoxes()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
void CDbHndl::disableMsgBoxes(bool status)
{
    m_disableMsgBoxes = status;
}



/****************************************************************
 *
 * Function:        ~CDbHndl()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
CDbHndl::~CDbHndl()
{

}






/****************************************************************
 *
 * Function:    createTable()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::createTable(void)
{
    QString tmp;
    QString tmp1;
    bool res;
    int i;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);

    //===================================================================
    // TblAvanzaAssetName
    //===================================================================
    tmp.sprintf("CREATE TABLE IF NOT EXISTS TblMainHelpSection "
                " (MainHelpSectionName VARCHAR(255), "
                " MainHelpSectionId INTEGER PRIMARY KEY AUTOINCREMENT);");

    qry.prepare(tmp);

    res = execSingleCmd(qry);

    if(res == false)
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 4a"), QString::fromUtf8("Fail create TblMainHelpSection"));
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }

    qry.finish();


    //========================================================================
    // Stock and company analysis
    //========================================================================


    //-----------------------------------------------------------------------
    // TblMainAnalysis
    //-----------------------------------------------------------------------
    tmp.sprintf("CREATE TABLE IF NOT EXISTS TblMainAnalysis "
                " (MainAnalysisId INTEGER PRIMARY KEY AUTOINCREMENT, "
                " stockName VARCHAR(255), "
                " StockSymbol VARCHAR(255) NOT NULL UNIQUE);");

    qDebug() << tmp;

    qry.prepare(tmp);

        res = execSingleCmd(qry);

        if(res == false)
        {
            qDebug() << qry.lastError();
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("TblMainAnalysis"), QString::fromUtf8("Fail create TblMainHelpSection"));
            }
            closeDb();
            m_mutex.unlock();
            return false;
        }

        qry.finish();



        //-----------------------------------------------------------------------
        // TblSubAnalysisCompanyType
        //-----------------------------------------------------------------------
        tmp.sprintf("CREATE TABLE IF NOT EXISTS TblSubAnalysisCompanyType "
                    " (CompanyTypeId INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "  CompanyType INTEGER DEFAULT 0, "
                    "  MainAnalysisId INTEGER);");

        qDebug() << tmp;


        qry.prepare(tmp);

        res = execSingleCmd(qry);

        if(res == false)
        {
            qDebug() << qry.lastError();
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("TblDateAnalysis"), QString::fromUtf8("Fail create TblDateEarningsSubAnalysis"));
            }
            closeDb();
            m_mutex.unlock();
            return false;
        }

        qry.finish();



        // -2
        // Date
                //-----------------------------------------------------------------------
                // TblDateOperatingCashFlowSubAnalysis (Operativt Kassaflöde)
                //-----------------------------------------------------------------------
                tmp.sprintf("CREATE TABLE IF NOT EXISTS TblDateOperatingCashFlowSubAnalysis "
                            " (DateOperatingCashFlowId INTEGER PRIMARY KEY AUTOINCREMENT, "
                            " DateOperatingCashFlow DATE, "
                            " MainAnalysisId INTEGER);");

                qDebug() << tmp;


                qry.prepare(tmp);

                res = execSingleCmd(qry);

                if(res == false)
                {
                    qDebug() << qry.lastError();
                    if(m_disableMsgBoxes == false)
                    {
                        QMessageBox::critical(NULL, QString::fromUtf8("TblDateOperatingCashFlowSubAnalysis"), QString::fromUtf8("Fail create TblDateEquitySubAnalysis"));
                    }
                    closeDb();
                    m_mutex.unlock();
                    return false;
                }

                qry.finish();


        // Data
                //-----------------------------------------------------------------------
                // TblDataOperatingCashFlowSubAnalysis (Operativt Kassaflöde)
                //-----------------------------------------------------------------------
                tmp.sprintf("CREATE TABLE IF NOT EXISTS TblDataOperatingCashFlowSubAnalysis "
                            " (DataOperatingCashFlowId INTEGER PRIMARY KEY AUTOINCREMENT, "
                            " DateOperatingCashFlowId INTEGER, "
                            " DataOperatingCashFlow VARCHAR(255), "
                            " MainAnalysisId INTEGER);");

                qDebug() << tmp;


                qry.prepare(tmp);

                res = execSingleCmd(qry);

                if(res == false)
                {
                    qDebug() << qry.lastError();
                    if(m_disableMsgBoxes == false)
                    {
                        QMessageBox::critical(NULL, QString::fromUtf8("TblDataOperatingCashFlowSubAnalysis"), QString::fromUtf8("Fail create TblOperatingCashFlowSubAnalysis"));
                    }
                    closeDb();
                    m_mutex.unlock();
                    return false;
                }

                qry.finish();





        // -1
        // Date
                //-----------------------------------------------------------------------
                // TblDateCashFlowCapexSubAnalysis (Kassaflöde Capex Investeringar/kapitalutgifter)
                //-----------------------------------------------------------------------
                tmp.sprintf("CREATE TABLE IF NOT EXISTS TblDateCashFlowCapexSubAnalysis "
                            " (DateCashFlowCapexId INTEGER PRIMARY KEY AUTOINCREMENT, "
                            " DateCashFlowCapex DATE, "
                            " MainAnalysisId INTEGER);");

                qDebug() << tmp;


                qry.prepare(tmp);

                res = execSingleCmd(qry);

                if(res == false)
                {
                    qDebug() << qry.lastError();
                    if(m_disableMsgBoxes == false)
                    {
                        QMessageBox::critical(NULL, QString::fromUtf8("TblDateCashFlowCapexSubAnalysis"), QString::fromUtf8("Fail create TblDateEquitySubAnalysis"));
                    }
                    closeDb();
                    m_mutex.unlock();
                    return false;
                }

                qry.finish();


        // Data
                //-----------------------------------------------------------------------
                // TblDataCashFlowCapexSubAnalysis (Kassaflöde Capex Investeringar/kapitalutgifter)
                //-----------------------------------------------------------------------
                tmp.sprintf("CREATE TABLE IF NOT EXISTS TblDataCashFlowCapexSubAnalysis "
                            " (DataCashFlowCapexId INTEGER PRIMARY KEY AUTOINCREMENT, "
                            " DateCashFlowCapexId INTEGER, "
                            " DataCashFlowCapex VARCHAR(255), "
                            " MainAnalysisId INTEGER);");

                qDebug() << tmp;


                qry.prepare(tmp);

                res = execSingleCmd(qry);

                if(res == false)
                {
                    qDebug() << qry.lastError();
                    if(m_disableMsgBoxes == false)
                    {
                        QMessageBox::critical(NULL, QString::fromUtf8("TblDataCapexSubAnalysis"), QString::fromUtf8("Fail create TblDataCapexSubAnalysis"));
                    }
                    closeDb();
                    m_mutex.unlock();
                    return false;
                }

                qry.finish();




        //================

        // 0
        // Date
                //-----------------------------------------------------------------------
                // TblDateEquitySubAnalysis (Eget kapital)
                //-----------------------------------------------------------------------
                tmp.sprintf("CREATE TABLE IF NOT EXISTS TblDateEquitySubAnalysis "
                            " (DateEquityId INTEGER PRIMARY KEY AUTOINCREMENT, "
                            " DateEquity DATE, "
                            " MainAnalysisId INTEGER);");

                qDebug() << tmp;


                qry.prepare(tmp);

                res = execSingleCmd(qry);

                if(res == false)
                {
                    qDebug() << qry.lastError();
                    if(m_disableMsgBoxes == false)
                    {
                        QMessageBox::critical(NULL, QString::fromUtf8("TblDateEquitySubAnalysis"), QString::fromUtf8("Fail create TblDateEquitySubAnalysis"));
                    }
                    closeDb();
                    m_mutex.unlock();
                    return false;
                }

                qry.finish();


        // Data
                //-----------------------------------------------------------------------
                // TblDataEquitySubAnalysis (Eget kapital)
                //-----------------------------------------------------------------------
                tmp.sprintf("CREATE TABLE IF NOT EXISTS TblDataEquitySubAnalysis "
                            " (DataEquityId INTEGER PRIMARY KEY AUTOINCREMENT, "
                            " DateEquityId INTEGER, "
                            " DataEquity VARCHAR(255), "
                            " MainAnalysisId INTEGER);");

                qDebug() << tmp;


                qry.prepare(tmp);

                res = execSingleCmd(qry);

                if(res == false)
                {
                    qDebug() << qry.lastError();
                    if(m_disableMsgBoxes == false)
                    {
                        QMessageBox::critical(NULL, QString::fromUtf8("TblDataEquitySubAnalysis"), QString::fromUtf8("Fail create TblDataEquitySubAnalysis"));
                    }
                    closeDb();
                    m_mutex.unlock();
                    return false;
                }

                qry.finish();




// 1
// Date
        //-----------------------------------------------------------------------
        // TblDateDividendSubAnalysis (Utdelning)
        //-----------------------------------------------------------------------
        tmp.sprintf("CREATE TABLE IF NOT EXISTS TblDateDividendSubAnalysis "
                    " (DateDividendId INTEGER PRIMARY KEY AUTOINCREMENT, "
                    " DateDividend DATE, "
                    " MainAnalysisId INTEGER);");

        qDebug() << tmp;


        qry.prepare(tmp);

        res = execSingleCmd(qry);

        if(res == false)
        {
            qDebug() << qry.lastError();
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("TblDateAnalysis"), QString::fromUtf8("Fail create TblDateDividendSubAnalysis"));
            }
            closeDb();
            m_mutex.unlock();
            return false;
        }

        qry.finish();


// Data
        //-----------------------------------------------------------------------
        // TblDataDividendSubAnalysis (Utdelning)
        //-----------------------------------------------------------------------
        tmp.sprintf("CREATE TABLE IF NOT EXISTS TblDataDividendSubAnalysis "
                    " (DataDividendId INTEGER PRIMARY KEY AUTOINCREMENT, "
                    " DateDividendId INTEGER, "
                    " DataDividend VARCHAR(255), "
                    " MainAnalysisId INTEGER);");

        qDebug() << tmp;


        qry.prepare(tmp);

        res = execSingleCmd(qry);

        if(res == false)
        {
            qDebug() << qry.lastError();
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("TblDateAnalysis"), QString::fromUtf8("Fail create TblDataDividendSubAnalysis"));
            }
            closeDb();
            m_mutex.unlock();
            return false;
        }

        qry.finish();



// 2
//Date
        //-----------------------------------------------------------------------
        // TblDateEarningsSubAnalysis (Vinst)
        //-----------------------------------------------------------------------
        tmp.sprintf("CREATE TABLE IF NOT EXISTS TblDateEarningsSubAnalysis "
                    " (DateEarningsId INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "  DateEarnings DATE, "
                    "  MainAnalysisId INTEGER);");

        qDebug() << tmp;


        qry.prepare(tmp);

        res = execSingleCmd(qry);

        if(res == false)
        {
            qDebug() << qry.lastError();
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("TblDateAnalysis"), QString::fromUtf8("Fail create TblDateEarningsSubAnalysis"));
            }
            closeDb();
            m_mutex.unlock();
            return false;
        }

        qry.finish();


//Data
        //-----------------------------------------------------------------------
        // TblDataEarningsSubAnalysis (Vinst)
        //-----------------------------------------------------------------------
        tmp.sprintf("CREATE TABLE IF NOT EXISTS TblDataEarningsSubAnalysis "
                    " (DataEarningsId INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "  DateEarningsId INTEGER, "
                    "  DataEarnings VARCHAR(255), "
                    "  MainAnalysisId INTEGER);");

        qDebug() << tmp;


        qry.prepare(tmp);

        res = execSingleCmd(qry);

        if(res == false)
        {
            qDebug() << qry.lastError();
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("TblDateAnalysis"), QString::fromUtf8("Fail create TblDataEarningsSubAnalysis"));
            }
            closeDb();
            m_mutex.unlock();
            return false;
        }

        qry.finish();



// 3

// Date
        //-----------------------------------------------------------------------
        // TblDateTotalCurrentAssetsSubAnalysis (Omsättningstillgångar)
        //-----------------------------------------------------------------------
        tmp.sprintf("CREATE TABLE IF NOT EXISTS TblDateTotalCurrentAssetsSubAnalysis "
                    " (DateTotalCurrentAssetsId INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "  DateTotalCurrentAssets DATE, "
                    "  MainAnalysisId INTEGER);");

        qDebug() << tmp;


        qry.prepare(tmp);

        res = execSingleCmd(qry);

        if(res == false)
        {
            qDebug() << qry.lastError();
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("TblDateAnalysis"), QString::fromUtf8("Fail create TblDateTotalCurrentAssetsSubAnalysis"));
            }
            closeDb();
            m_mutex.unlock();
            return false;
        }

        qry.finish();


// Data
        //-----------------------------------------------------------------------
        // TblDataTotalCurrentAssetsSubAnalysis (Omsättningstillgångar)
        //-----------------------------------------------------------------------
        tmp.sprintf("CREATE TABLE IF NOT EXISTS TblDataTotalCurrentAssetsSubAnalysis "
                    " (DataTotalCurrentAssetsId INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "  DateTotalCurrentAssetsId INTEGER, "
                    "  DataTotalCurrentAssets VARCHAR(255), "
                    "  MainAnalysisId INTEGER);");

        qDebug() << tmp;


        qry.prepare(tmp);

        res = execSingleCmd(qry);

        if(res == false)
        {
            qDebug() << qry.lastError();
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("TblDateAnalysis"), QString::fromUtf8("Fail create TblDataTotalCurrentAssetsSubAnalysis"));
            }
            closeDb();
            m_mutex.unlock();
            return false;
        }

        qry.finish();





// 4
// Date
        //-----------------------------------------------------------------------
        // TblDateTotalCurrentLiabilitiesSubAnalysis (Kortfristiga skulder)
        //-----------------------------------------------------------------------
        tmp.sprintf("CREATE TABLE IF NOT EXISTS TblDateTotalCurrentLiabilitiesSubAnalysis "
                    " (DateTotalCurrentLiabilitiesId INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "  DateTotalCurrentLiabilities DATE, "
                    "  MainAnalysisId INTEGER);");

        qDebug() << tmp;


        qry.prepare(tmp);

        res = execSingleCmd(qry);

        if(res == false)
        {
            qDebug() << qry.lastError();
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("TblDateAnalysis"), QString::fromUtf8("Fail create TblDateTotalCurrentLiabilitiesSubAnalysis"));
            }
            closeDb();
            m_mutex.unlock();
            return false;
        }

        qry.finish();


        // Data
                //-----------------------------------------------------------------------
                // TblDataTotalCurrentLiabilitiesSubAnalysis (Kortfristiga skulder)
                //-----------------------------------------------------------------------
                tmp.sprintf("CREATE TABLE IF NOT EXISTS TblDataTotalCurrentLiabilitiesSubAnalysis "
                            " (DataTotalCurrentLiabilitiesId INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "  DateTotalCurrentLiabilitiesId INTEGER, "
                            "  DataTotalCurrentLiabilities VARCHAR(255), "
                            "  MainAnalysisId INTEGER);");

                qDebug() << tmp;


                qry.prepare(tmp);

                res = execSingleCmd(qry);

                if(res == false)
                {
                    qDebug() << qry.lastError();
                    if(m_disableMsgBoxes == false)
                    {
                        QMessageBox::critical(NULL, QString::fromUtf8("TblDateAnalysis"), QString::fromUtf8("Fail create TblDataTotalCurrentLiabilitiesSubAnalysis"));
                    }
                    closeDb();
                    m_mutex.unlock();
                    return false;
                }

                qry.finish();




// 5
        //-----------------------------------------------------------------------
        // TblDateTotalLiabilitiesSubAnalysis (Totala skulder)
        //-----------------------------------------------------------------------
        tmp.sprintf("CREATE TABLE IF NOT EXISTS TblDateTotalLiabilitiesSubAnalysis "
                    " (DateTotalLiabilitiesId INTEGER PRIMARY KEY AUTOINCREMENT, "
                    " DateTotalLiabilities DATE, "
                    " MainAnalysisId INTEGER);");

        qDebug() << tmp;


        qry.prepare(tmp);

        res = execSingleCmd(qry);

        if(res == false)
        {
            qDebug() << qry.lastError();
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("TblDateAnalysis"), QString::fromUtf8("Fail create TblDateTotalLiabilitiesSubAnalysis"));
            }
            closeDb();
            m_mutex.unlock();
            return false;
        }

        qry.finish();


// Data
        //-----------------------------------------------------------------------
        // TblDataTotalLiabilitiesSubAnalysis (Totala skulder)
        //-----------------------------------------------------------------------
        tmp.sprintf("CREATE TABLE IF NOT EXISTS TblDataTotalLiabilitiesSubAnalysis "
                    " (DataTotalLiabilitiesId INTEGER PRIMARY KEY AUTOINCREMENT, "
                    " DateTotalLiabilitiesId INTEGER, "
                    " DataTotalLiabilities VARCHAR(255), "
                    " MainAnalysisId INTEGER);");

        qDebug() << tmp;


        qry.prepare(tmp);

        res = execSingleCmd(qry);

        if(res == false)
        {
            qDebug() << qry.lastError();
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("TblDateAnalysis"), QString::fromUtf8("Fail create TblDataTotalLiabilitiesSubAnalysis"));
            }
            closeDb();
            m_mutex.unlock();
            return false;
        }

        qry.finish();




// 6
// Date
        //-----------------------------------------------------------------------
        // TblDateSoliditySubAnalysis (soliditet)
        //-----------------------------------------------------------------------
        tmp.sprintf("CREATE TABLE IF NOT EXISTS TblDateSoliditySubAnalysis "
                    " (DateSolidityId INTEGER PRIMARY KEY AUTOINCREMENT, "
                    " DateSolidity DATE, "
                    " MainAnalysisId INTEGER);");

        qDebug() << tmp;


        qry.prepare(tmp);

        res = execSingleCmd(qry);

        if(res == false)
        {
            qDebug() << qry.lastError();
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("TblDateAnalysis"), QString::fromUtf8("Fail create TblDateSoliditySubAnalysis"));
            }
            closeDb();
            m_mutex.unlock();
            return false;
        }

        qry.finish();




// Data
        //-----------------------------------------------------------------------
        // TblDataSoliditySubAnalysis (soliditet)
        //-----------------------------------------------------------------------
        tmp.sprintf("CREATE TABLE IF NOT EXISTS TblDataSoliditySubAnalysis "
                    " (DataSolidityId INTEGER PRIMARY KEY AUTOINCREMENT, "
                    " DateSolidityId INTEGER, "
                    " DataSolidity VARCHAR(255), "
                    " MainAnalysisId INTEGER);");

        qDebug() << tmp;


        qry.prepare(tmp);

        res = execSingleCmd(qry);

        if(res == false)
        {
            qDebug() << qry.lastError();
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("TblDateAnalysis"), QString::fromUtf8("Fail create TblDataSoliditySubAnalysis"));
            }
            closeDb();
            m_mutex.unlock();
            return false;
        }

        qry.finish();



// 7

//Date
        //-----------------------------------------------------------------------
        // TblDateCoverageRatioSubAnalysis (räntetäckningsgraden)
        //-----------------------------------------------------------------------
        tmp.sprintf("CREATE TABLE IF NOT EXISTS TblDateCoverageRatioSubAnalysis "
                    " (CoverageRatioDateId INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "  CoverageRatioDate DATE, "
                    "  MainAnalysisId INTEGER);");

        qDebug() << tmp;


        qry.prepare(tmp);

        res = execSingleCmd(qry);

        if(res == false)
        {
            qDebug() << qry.lastError();
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("TblDateAnalysis"), QString::fromUtf8("Fail create TblDateCoverageRatioSubAnalysis"));
            }
            closeDb();
            m_mutex.unlock();
            return false;
        }

        qry.finish();


// Data
                //-----------------------------------------------------------------------
                // TblDataCoverageRatioSubAnalysis (räntetäckningsgraden)
                //-----------------------------------------------------------------------
                tmp.sprintf("CREATE TABLE IF NOT EXISTS TblDataCoverageRatioSubAnalysis "
                            " (CoverageRatioDataId INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "  CoverageRatioDateId INTEGER, "
                            "  CoverageRatioData VARCHAR(255), "
                            "  MainAnalysisId INTEGER);");

                qDebug() << tmp;


                qry.prepare(tmp);

                res = execSingleCmd(qry);

                if(res == false)
                {
                    qDebug() << qry.lastError();
                    if(m_disableMsgBoxes == false)
                    {
                        QMessageBox::critical(NULL, QString::fromUtf8("TblDateAnalysis"), QString::fromUtf8("Fail create TblDataCoverageRatioSubAnalysis"));
                    }
                    closeDb();
                    m_mutex.unlock();
                    return false;
                }

                qry.finish();



// 8
// Date

        //-----------------------------------------------------------------------
        // TblDateCoreCapitalRatioSubAnalysis (kärnprimärkapitalrelation används)
        //-----------------------------------------------------------------------
        tmp.sprintf("CREATE TABLE IF NOT EXISTS TblDateCoreCapitalRatioSubAnalysis "
                    " (DateCoreCapitalRatioId INTEGER PRIMARY KEY AUTOINCREMENT, "
                    " DateCoreCapitalRatio DATE, "
                    " MainAnalysisId INTEGER);");

        qDebug() << tmp;


        qry.prepare(tmp);

        res = execSingleCmd(qry);

        if(res == false)
        {
            qDebug() << qry.lastError();
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("TblDateAnalysis"), QString::fromUtf8("Fail create TblDateCoreCapitalRatioSubAnalysis"));
            }
            closeDb();
            m_mutex.unlock();
            return false;
        }

        qry.finish();


// Data
        //-----------------------------------------------------------------------
        // TblDataCoreCapitalRatioSubAnalysis (kärnprimärkapitalrelation används ej)
        //-----------------------------------------------------------------------
        tmp.sprintf("CREATE TABLE IF NOT EXISTS TblDataCoreCapitalRatioSubAnalysis "
                    " (DataCoreCapitalRatioId INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "  DateCoreCapitalRatioId INTEGER, "
                    "  DataCoreCapitalRatio VARCHAR(255), "
                    "  MainAnalysisId INTEGER);");

        qDebug() << tmp;


        qry.prepare(tmp);

        res = execSingleCmd(qry);

        if(res == false)
        {
            qDebug() << qry.lastError();
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("TblDateAnalysis"), QString::fromUtf8("Fail create TblDataCoreCapitalRatioSubAnalysis"));
            }
            closeDb();
            m_mutex.unlock();
            return false;
        }

        qry.finish();


// 9

// Date
        //-----------------------------------------------------------------------
        // TblDateCoreTier1RatioSubAnalysis (primärkapitalrelation, Lundaluppen)
        //-----------------------------------------------------------------------
        tmp.sprintf("CREATE TABLE IF NOT EXISTS TblDateTier1RatioSubAnalysis "
                    " (DateTier1RatioId INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "  DateTier1Ratio DATE, "
                    "  MainAnalysisId INTEGER);");

        qDebug() << tmp;


        qry.prepare(tmp);

        res = execSingleCmd(qry);

        if(res == false)
        {
            qDebug() << qry.lastError();
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("TblDateAnalysis"), QString::fromUtf8("Fail create TblDateTier1RatioSubAnalysis"));
            }
            closeDb();
            m_mutex.unlock();
            return false;
        }

        qry.finish();

// Data

        //-----------------------------------------------------------------------
        // TblDataCoreTier1RatioSubAnalysis (primärkapitalrelation, Lundaluppen)
        //-----------------------------------------------------------------------
        tmp.sprintf("CREATE TABLE IF NOT EXISTS TblDataTier1RatioSubAnalysis "
                    " (DataTier1RatioId INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "  DateTier1RatioId INTEGER, "                             // YYYY-MM-DD ID
                    "  DataTier1Ratio VARCHAR(255), "
                    "  MainAnalysisId INTEGER);");

        qDebug() << tmp;


        qry.prepare(tmp);

        res = execSingleCmd(qry);

        if(res == false)
        {
            qDebug() << qry.lastError();
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("TblDateAnalysis"), QString::fromUtf8("Fail create TblDataTier1RatioSubAnalysis"));
            }
            closeDb();
            m_mutex.unlock();
            return false;
        }

        qry.finish();



//===========================
        //-----------------------------------------------------------------------
        // TblDateAnalysis
        //-----------------------------------------------------------------------
        tmp.sprintf("CREATE TABLE IF NOT EXISTS TblDateAnalysis "
                    " (AnalysisDateId INTEGER PRIMARY KEY AUTOINCREMENT, "
                    " AnalysisDate DATE, "
                    " MainAnalysisId INTEGER);");

        qDebug() << tmp;


        qry.prepare(tmp);

        res = execSingleCmd(qry);

        if(res == false)
        {
            qDebug() << qry.lastError();
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("TblDateAnalysis"), QString::fromUtf8("Fail create TblMainHelpSection"));
            }
            closeDb();
            m_mutex.unlock();
            return false;
        }

        qry.finish();




        //-----------------------------------------------------------------------
        // TblAnalysisDate
        //-----------------------------------------------------------------------
         tmp.sprintf("CREATE TABLE IF NOT EXISTS TblAnalysisData "
                    " (AnalysisDataId INTEGER PRIMARY KEY AUTOINCREMENT , "
                    " AnalysisDateId INTEGER, "                             // YYYY-MM-DD ID
                    " companyDescription VARCHAR(255), "

                    " bigEnoughAnswer VARCHAR(255), "
                    " bigEnoughComment VARCHAR(255), "

                    " strongFinancialPositionAnswer VARCHAR(255), "
                    " strongFinancialPositionComment VARCHAR(255), "

                    " earningStabilityAnswer VARCHAR(255), "
                    " earningStabilityComment VARCHAR(255), "

                    " dividendStabilityAnswer VARCHAR(255), "
                    " dividendStabilityComment VARCHAR(255), "

                    " earningGrowthAnswer VARCHAR(255), "
                    " earningGrowthComment VARCHAR(255), "

                    " keyValuePe VARCHAR(255), "
                    " keyValuePs VARCHAR(255), "

                    " keyValueNavPriceRatio VARCHAR(255), "
                    " keyValueYield VARCHAR(255), "
                    " keyValuePriceJEKRatio VARCHAR(255), "
                    " keyValueerningsDividentRatio VARCHAR(255), "
                    " keyValueTotalDebtEquityRatio VARCHAR(255), "
                    " keyValueCurrentRatio VARCHAR(255), "
                    " trustworthyLeadershipAnswer VARCHAR(255), "
                    " trustworthyLeadershipComment VARCHAR(255), "
                    " goodOwnershipAnswer VARCHAR(255), "
                    " goodOwnershipComment VARCHAR(255),"
                    " otherInformation VARCHAR(255),"
                    " MainAnalysisId INTEGER);");

                qry.prepare(tmp);

                res = execSingleCmd(qry);

                if(res == false)
                {
                    qDebug() << qry.lastError();
                    if(m_disableMsgBoxes == false)
                    {
                        QMessageBox::critical(NULL, QString::fromUtf8("TblAnalysisData"), QString::fromUtf8("Fail create TblAnalysisData"));
                    }
                    closeDb();
                    m_mutex.unlock();
                    return false;
                }

                qry.finish();

//===========================


                    //-----------------------------------------------------------------------
                    // TblStrongFinancialPositionData
                    //-----------------------------------------------------------------------
                    tmp.sprintf("CREATE TABLE IF NOT EXISTS TblStrongFinancialPositionData "
                                " (AnalysisDataId INTEGER, "
                                " year_1 VARCHAR(255), "
                                " currentAssets_1 VARCHAR(255), "
                                " currentLiabilities_1 VARCHAR(255), "

                                " year_2 VARCHAR(255), "
                                " currentAssets_2 VARCHAR(255), "
                                " currentLiabilities_2 VARCHAR(255), "

                                " year_3 VARCHAR(255), "
                                " currentAssets_3 VARCHAR(255), "
                                " currentLiabilities_3 VARCHAR(255), "

                                " year_4 VARCHAR(255), "
                                " currentAssets_4 VARCHAR(255), "
                                " currentLiabilities_4 VARCHAR(255), "

                                " year_5 VARCHAR(255), "
                                " currentAssets_5 VARCHAR(255), "
                                " currentLiabilities_5 VARCHAR(255));");

                            qry.prepare(tmp);

                            res = execSingleCmd(qry);

                            if(res == false)
                            {
                                qDebug() << qry.lastError();
                                if(m_disableMsgBoxes == false)
                                {
                                    QMessageBox::critical(NULL, QString::fromUtf8("TblStrongFinancialPositionData"), QString::fromUtf8("Fail create TblStrongFinancialPositionData"));
                                }
                                closeDb();
                                m_mutex.unlock();
                                return false;
                            }

                            qry.finish();



                    //-----------------------------------------------------------------------
                    // TblEarningStabilityData
                    //-----------------------------------------------------------------------
                    tmp.sprintf("CREATE TABLE IF NOT EXISTS TblEarningStabilityData "
                                " (AnalysisDataId INTEGER, "
                                " year_1 VARCHAR(255), "
                                " erning_1 VARCHAR(255), "
                                " erningMargin_1 VARCHAR(255), "
                                " sale_1 VARCHAR(255), "

                                " year_2 VARCHAR(255), "
                                " erning_2 VARCHAR(255), "
                                " erningMargin_2 VARCHAR(255), "
                                " sale_2 VARCHAR(255), "

                                " year_3 VARCHAR(255), "
                                " erning_3 VARCHAR(255), "
                                " erningMargin_3 VARCHAR(255), "
                                " sale_3 VARCHAR(255), "

                                " year_4 VARCHAR(255), "
                                " erning_4 VARCHAR(255), "
                                " erningMargin_4 VARCHAR(255), "
                                " sale_4 VARCHAR(255), "

                                " year_5 VARCHAR(255), "
                                " erning_5 VARCHAR(255), "
                                " erningMargin_5 VARCHAR(255), "
                                " sale_5 VARCHAR(255));");

                            qry.prepare(tmp);

                            res = execSingleCmd(qry);

                            if(res == false)
                            {
                                qDebug() << qry.lastError();
                                if(m_disableMsgBoxes == false)
                                {
                                    QMessageBox::critical(NULL, QString::fromUtf8("TblEarningStabilityData"), QString::fromUtf8("Fail create TblEarningStabilityData"));
                                }
                                closeDb();
                                m_mutex.unlock();
                                return false;
                            }

                            qry.finish();



                        //-----------------------------------------------------------------------
                        // TblDividendStabilityData
                        //-----------------------------------------------------------------------
                        tmp.sprintf("CREATE TABLE IF NOT EXISTS TblDividendStabilityData "
                                    " (AnalysisDataId INTEGER, "
                                    " year_1 VARCHAR(255), "
                                    " dividend_1 VARCHAR(255), "

                                    " year_2 VARCHAR(255), "
                                    " dividend_2 VARCHAR(255), "

                                    " year_3 VARCHAR(255), "
                                    " dividend_3 VARCHAR(255), "

                                    " year_4 VARCHAR(255), "
                                    " dividend_4 VARCHAR(255), "

                                    " year_5 VARCHAR(255), "
                                    " dividend_5 VARCHAR(255));");

                                qry.prepare(tmp);

                                res = execSingleCmd(qry);

                                if(res == false)
                                {
                                    qDebug() << qry.lastError();
                                    if(m_disableMsgBoxes == false)
                                    {
                                        QMessageBox::critical(NULL, QString::fromUtf8("Database Error 4a"), QString::fromUtf8("Fail create TblMainHelpSection"));
                                    }
                                    closeDb();
                                    m_mutex.unlock();
                                    return false;
                                }

                                qry.finish();



    //========================================================================




    //===================================================================
    // TblYahooKeyStatistics
    //===================================================================
    tmp.sprintf("CREATE TABLE IF NOT EXISTS TblYahooKeyStatistics "
                " (StockSymbol VARCHAR(255) UNIQUE PRIMARY KEY, "
                " MarketCap VARCHAR(255), "
                " EnterpriseValue VARCHAR(255), "
                " TrailingPe VARCHAR(255), "
                " ForwardPe VARCHAR(255), "
                " PEGRatio VARCHAR(255), "
                " PriceDivSales VARCHAR(255), "
                " PriceDivBook VARCHAR(255), "
                " EnterpriseValueDivRevenue VARCHAR(255), "
                " EnterpriseValueDivEBITDA VARCHAR(255), "
                " FiscalYearEnds VARCHAR(255), "
                " MostRecentQuarter VARCHAR(255), "
                " ProfitMargin VARCHAR(255), "
                " OperatingMargin VARCHAR(255), "
                " ReturnOnAssets VARCHAR(255), "
                " ReturnOnEquity VARCHAR(255), "
                " Revenue VARCHAR(255), "
                " RevenuePerShare VARCHAR(255), "
                " QtrlyRevenueGrowth VARCHAR(255), "
                " GrossProfit VARCHAR(255), "
                " EBITDA VARCHAR(255), "
                " NetIncomeAvlToCommon VARCHAR(255), "
                " DilutedEPS VARCHAR(255), "
                " QtrlyEarningsGrowth VARCHAR(255), "
                " TotalCash VARCHAR(255), "
                " TotalCashPerShare VARCHAR(255), "
                " TotalDebt VARCHAR(255), "
                " TotalDebtDivEquity VARCHAR(255), "
                " CurrentRatio VARCHAR(255), "
                " BookValuePerShare VARCHAR(255), "
                " OperatingCashFlow VARCHAR(255), "
                " LeveredFreeCashFlow VARCHAR(255), "
                " Beta VARCHAR(255), "
                " Week52Change VARCHAR(255), "
                " S_And_P500_52WeekChange VARCHAR(255), "
                " ProcentHeldbyInsiders VARCHAR(255), "
                " ProcentHeldbyInstitutions VARCHAR(255), "
                " SharesShort VARCHAR(255), "
                " ShortRatio VARCHAR(255), "
                " ShortProcentOfFloat VARCHAR(255), "
                " SharesShortPriorMonth VARCHAR(255), "
                " ForwardAnnualDividendRate VARCHAR(255), "
                " ForwardAnnualDividendYield VARCHAR(255), "
                " Week52High VARCHAR(255), "
                " Week52Low VARCHAR(255), "
                " Day50MovingAverage VARCHAR(255), "
                " Day200MovingAverage VARCHAR(255), "
                " AvgVol3Month VARCHAR(255), "
                " AvgVol10Day VARCHAR(255), "
                " SharesOutstanding VARCHAR(255), "
                " Float_ VARCHAR(255), "
                " TrailingAnnualDividendYield VARCHAR(255), "
                " TrailingAnnualDividendYield1 VARCHAR(255), "
                " Year5AverageDividendYield VARCHAR(255), "
                " PayoutRatio VARCHAR(255), "
                " DividendDate VARCHAR(255), "
                " ExDividendDate VARCHAR(255), "
                " LastSplitFactor VARCHAR(255), "
                " LastSplitDate VARCHAR(255));");

    qDebug() << tmp;

    qry.prepare(tmp);

    res = execSingleCmd(qry);

    if(res == false)
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 4b"), QString::fromUtf8("Fail create TblYahooKeyStatistics"));
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }

    qry.finish();




    //===================================================================
    // TblAvanzaAssetName
    //===================================================================
    tmp.sprintf("CREATE TABLE IF NOT EXISTS TblSubHelpSection"
                " (SubHelpSectionName VARCHAR(255),"
                " MainHelpSectionId INTEGER, "
                " SubHelpSectionId INTEGER PRIMARY KEY AUTOINCREMENT);");

    qry.prepare(tmp);

    res = execSingleCmd(qry);

    if(res == false)
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 4b"), QString::fromUtf8("Fail create TblSubHelpSection"));
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }

    qry.finish();






    //===================================================================
    // TblAvanzaAssetData
    //===================================================================
    tmp.sprintf("CREATE TABLE IF NOT EXISTS TblHelpSectionTxt"
                " (HelpSectionTxt VARCHAR(255),"
                " SubHelpSectionId INTEGER,"
                " MainHelpSectionId INTEGER);");

    qry.prepare(tmp);

    res = execSingleCmd(qry);

    if(res == false)
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 4c"), QString::fromUtf8("Fail create TblHelpSectionTxt"));
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }

    qry.finish();



    //===================================================================
    // TblNordnetBalanceIncomeSheet
    //===================================================================
    tmp.sprintf("CREATE TABLE IF NOT EXISTS TblNordnetCompanyDescription"
                " (assetSymbol VARCHAR(255) UNIQUE PRIMARY KEY, "
                " assetName VARCHAR(255), "
                " assetGroup VARCHAR(255), "
                " assetBranch VARCHAR(255), "
                " executiveDirector VARCHAR(255), "
                " businessDescription VARCHAR(255));");


    qry.prepare(tmp);


    res = execSingleCmd(qry);

    if(res == false)
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 4bb3"), QString::fromUtf8("Fail create TblNordnetCompanyDescription"));
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }

    qry.finish();


    //===================================================================
    // TblNordnetBalanceIncomeSheet
    //===================================================================
    tmp.sprintf("CREATE TABLE IF NOT EXISTS TblProgressMyPortfolio "
                " (YearMonthTxt VARCHAR(255) UNIQUE PRIMARY KEY, "
                " Year INTEGER, "
                " Month INTEGER, "
                " MarketValue REAL, "
                " AddedMoney REAL, "
                " ReceivedDivident REAL,"
                " Withdrawing REAL)");

    qry.prepare(tmp);


    res = execSingleCmd(qry);

    if(res == false)
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 4bb3"), QString::fromUtf8("Fail create TblProgressMyPortfolio"));
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }

    qry.finish();





    //===================================================================
    // TblNordnetBalanceIncomeSheet
    //===================================================================
    tmp.sprintf("CREATE TABLE IF NOT EXISTS TblNordnetBalanceIncomeSheet"
                " (yearAssetName VARCHAR(255) UNIQUE PRIMARY KEY, "
                " year DATE, "
                " assetName VARCHAR(255), "
                " assetGroup VARCHAR(255), "
                " operatingIncome REAL, "
                " operatingExpensesAndDepreciation REAL, "          // Depreciation = avskrivning
                " operatingIncomeAfterDepreciation REAL, "
                " netProfitAfterTax REAL, "
                " earningsPerShare REAL, "
                " numberOfShares REAL, "
                " fixedAssets REAL, "                               // Anläggningstillgångar
                " currentAssets  REAL, "                            // Omsättningstillgångar
                " totalAssets REAL, "
                " equity REAL, "                                    // Eget kapital
                " liabilities REAL, "                               // Skulder
                " totalEquityAndLiabilities REAL)");

    qry.prepare(tmp);


    res = execSingleCmd(qry);

    if(res == false)
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 4bb3"), QString::fromUtf8("Fail create TblYahooTaStockName"));
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }

    qry.finish();




    //===================================================================
    // NordnetStockIds
    //===================================================================
    tmp.sprintf("CREATE TABLE IF NOT EXISTS TblNordnetStockIds"
                " (assetId VARCHAR(255) UNIQUE PRIMARY KEY, "
                " assetSymbol VARCHAR(255), "
                " assetName VARCHAR(255))");

    qry.prepare(tmp);


    res = execSingleCmd(qry);

    if(res == false)
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 4bb3"), QString::fromUtf8("Fail create TblYahooTaStockName"));
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }

    qry.finish();








    //===================================================================
    // NordnetYahooBridge
    //===================================================================
    tmp.sprintf("CREATE TABLE IF NOT EXISTS TblNordnetYahooBridge"
                " (assetSymbol VARCHAR(255) UNIQUE PRIMARY KEY, "
                " assetName VARCHAR(255))");

    qry.prepare(tmp);


    res = execSingleCmd(qry);

    if(res == false)
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 4bb3"), QString::fromUtf8("Fail create TblYahooTaStockName"));
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }

    qry.finish();



    //===================================================================
    // Temp Efficient portfolio
    //===================================================================
    tmp.sprintf("CREATE TABLE IF NOT EXISTS TblTmpEfficPortTreeWidget"
                " (assetSymbol VARCHAR(255) UNIQUE PRIMARY KEY, "
                " assetName VARCHAR(255),"
                " isUsed INTEGET ,"
                " defaultSettings INTEGER, "
                " risk REAL, "
                " meanReturn REAL, "
                " minProcent REAL, "
                " maxProcent REAL, "
                " selProcent REAL, "
                " nofItems REAL, "
                "colorNumber INTEGER)");


    qry.prepare(tmp);


    res = execSingleCmd(qry);

    if(res == false)
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 4bb3"), QString::fromUtf8("Fail create TblYahooTaStockName"));
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }

    qry.finish();




    //===================================================================
    // YahooTaData
    //===================================================================
    tmp.sprintf("CREATE TABLE IF NOT EXISTS TblYahooTaStockName"
                " (assetNameId INTEGER UNIQUE PRIMARY KEY, "
                " assetName VARCHAR(255),"
                " assetSymbol VARCHAR(255) ,"
                " marketList VARCHAR(255), "                    // OMX Large cap
                " assetType VARCHAR(255), "                     // Verkstad, läkemedel
                " yahooNordetInterconId INTEGER)");


    qry.prepare(tmp);


    res = execSingleCmd(qry);

    if(res == false)
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 4bb3"), QString::fromUtf8("Fail create TblYahooTaStockName"));
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }

    qry.finish();



    //===================================================================
    // YahooTaData
    //===================================================================
    tmp.sprintf("CREATE TABLE IF NOT EXISTS TblYahooTaData"
                " (date DATE , "
                " open REAL, "
                " High REAL, "
                " low REAL, "
                " close REAL, "
                " volume REAL, "
                " adjClose REAL, "
                " assetNameId INTEGER,"
                " PRIMARY KEY (date, assetNameId));");

    qry.prepare(tmp);

    res = execSingleCmd(qry);

    if(res == false)
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 4bb2"), QString::fromUtf8("Fail create TblYahooTaData"));
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }


    qry.finish();



    //===================================================================
    // YahooTaData
    //===================================================================
    tmp.sprintf("CREATE TABLE IF NOT EXISTS TblYahooNordetIntercon"
                " (assetName VARCHAR(255) UNIQUE PRIMARY KEY,"
                " yahooNordetInterconId INTEGER)");

    qry.prepare(tmp);

    res = execSingleCmd(qry);

    if(res == false)
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 4bb1"), QString::fromUtf8("Fail create TblYahooNordetIntercon"));
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }

    qry.finish();




    //===================================================================
    // TblAvanzaAssetName
    //===================================================================
    tmp.sprintf("CREATE TABLE IF NOT EXISTS TblAvanzaAccount"
                " (account INTEGER UNIQUE PRIMARY KEY);");

    qry.prepare(tmp);

    res = execSingleCmd(qry);

    if(res == false)
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 4a"), QString::fromUtf8("Fail create TblAvanzaAccount"));
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }

    qry.finish();






    //===================================================================
    // TblAvanzaAssetName
    //===================================================================
    tmp.sprintf("CREATE TABLE IF NOT EXISTS TblAvanzaAssetName"
                " (assetName VARCHAR(255) UNIQUE PRIMARY KEY,"
                " accountId INTEGER)");

    qry.prepare(tmp);

    res = execSingleCmd(qry);

    if(res == false)
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 4b"), QString::fromUtf8("Fail create TblAvanzaAssetName"));
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }

    qry.finish();




    //===================================================================
    // TblAvanzaAssetData
    //===================================================================
    tmp.sprintf("CREATE TABLE IF NOT EXISTS TblAvanzaAssetData"
                " (date DATE,"
                " typeOfData VARCHAR(255),"
                " typeOfDataId INTEGER,"
                " assetPrice REAL,"
                " currency VARCHAR(255),"
                " assetName VARCHAR(255))");

    qry.prepare(tmp);

    res = execSingleCmd(qry);

    if(res == false)
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 4c"), QString::fromUtf8("Fail create TblAvanzaAssetData"));
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }
    //    else
    //        qDebug( "Inserted!" );

    qry.finish();



    //===================================================================
    // TblStockDataSnapshot
    //===================================================================
    //m_mutex.lock();
    // openDb(PATH_JACK_STOCK_DB);
    // QSqlQuery qry(m_db);


    // Ta region
    qry.prepare( "CREATE TABLE IF NOT EXISTS TblTaStockList"
                 "(taStockListId INTEGER UNIQUE PRIMARY KEY,"
                 "stockListName VARCHAR(255)"
                 ")");

    res = execSingleCmd(qry);

    if(res == false)
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 3"), QString::fromUtf8("Fail create table"));
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }
    //    else
    //      qDebug( "Inserted!" );

    qry.finish();

    // Ta stock name
    qry.prepare( "CREATE TABLE IF NOT EXISTS TblTaStockName"
                 "(taStockNameId INTEGER UNIQUE PRIMARY KEY,"
                 "stockSymbol VARCHAR(255),"
                 "stockName VARCHAR(255),"
                 "taStockListId INTEGER"
                 ")");

    res = execSingleCmd(qry);

    if(res == false)
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 3"), QString::fromUtf8("Fail create table"));
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }
    //    else
    //      qDebug( "Inserted!" );

    qry.finish();



    qry.prepare( "CREATE TABLE IF NOT EXISTS TblTaStockData"
                 "(taStockDataId INTEGER UNIQUE PRIMARY KEY,"
                 "taStockNameId INTEGER,"
                 "Date DATE,"
                 "Open REAL,"
                 "High REAL,"
                 "Low REAL,"
                 "Close REAL,"
                 "Volume REAL,"
                 "AdjClose REAL"
                 ")");


    res = execSingleCmd(qry);

    if(res == false)
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 3"), QString::fromUtf8("Fail create table"));
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }
    //    else
    //      qDebug( "Inserted!" );

    qry.finish();




    //===================================================================
    // TblStockDataSnapshot
    //===================================================================
    // Stock market snap shop
    qry.prepare( "CREATE TABLE IF NOT EXISTS TblStockDataSnapshot"
                 "(internalStockId INTEGER UNIQUE PRIMARY KEY,"
                 "companyName VARCHAR(255),"
                 "lastPrice VARCHAR(255),"
                 "priceChange VARCHAR(255),"
                 "procentChangeOneDay VARCHAR(255),"
                 "bidPrice VARCHAR(255),"
                 "offerPrice VARCHAR(255),"
                 "highestPrice VARCHAR(255),"
                 "lowestPrice VARCHAR(255),"
                 "volume VARCHAR(255),"
                 "currency VARCHAR(255),"
                 "time VARCHAR(255),"
                 "date VARCHAR(255),"
                 "procentChangeOneWeek VARCHAR(255),"
                 "procentChange1Month VARCHAR(255),"
                 "procentChange3Month VARCHAR(255),"
                 "procentChange6Month VARCHAR(255),"
                 "procentChange1Year VARCHAR(255),"
                 "procentChange2Year VARCHAR(255),"
                 "procentChange3Year VARCHAR(255),"
                 "procentChange5Year VARCHAR(255),"
                 "keyValuePE VARCHAR(255),"
                 "keyValuePS VARCHAR(255),"
                 "keyValueEarningsPerShare VARCHAR(255),"
                 "keyValueNAVPerShare VARCHAR(255),"
                 "keyValueDividendPerShare VARCHAR(255),"
                 "keyValueYield VARCHAR(255),"
                 "keyValueCoursePerJEK VARCHAR(255),"
                 "earningsDividedByDividend VARCHAR(255),"
                 "navDivLastStockPrice VARCHAR(255)"
                 ")");


    res = execSingleCmd(qry);

    if(res == false)
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 3"), QString::fromUtf8("Fail create table"));
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }
    //    else
    //      qDebug( "Inserted!" );

    qry.finish();


    // TblUserSelectedSnapshotData
    tmp.sprintf("CREATE TABLE IF NOT EXISTS TblUserSelectedSnapshotData"
                "(internalStockId INTEGER UNIQUE PRIMARY KEY,"
                "IsInitialized INTEGER DEFAULT 0,"
                "NofUserSelectedSnapshotData INTEGER DEFAULT 0,");


    for(i = 1; i < CDbHndl::MAX_NOF_GPSEL_INDEX; i++)
    {
        if(i < CDbHndl::MAX_NOF_GPSEL_INDEX-1)
        {
            tmp1.sprintf("snapshotData%dIsSel INTEGER DEFAULT 0, "
                         "snapshotData%d VARCHAR(255), "
                         "snapshotDbName%d VARCHAR(255), "
                         , (i-1)
                         , (i-1)
                         , (i-1));
        }
        else
        {
            tmp1.sprintf("snapshotData%dIsSel INTEGER DEFAULT 0, "
                         "snapshotData%d VARCHAR(255), "
                         "snapshotDbName%d VARCHAR(255));"
                         , (i-1)
                         , (i-1)
                         , (i-1));
        }

        tmp += tmp1;

    }


    qry.prepare(tmp);


    res = execSingleCmd(qry);

    if(res == false)
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error"), QString::fromUtf8("Fail create table"));
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }
    //    else
    //      qDebug( "Inserted!" );

    qry.finish();



    //===================================================================
    // TblFilterNameSDSnapshot
    //===================================================================

    // Stock market snap shop
    tmp.sprintf("CREATE TABLE IF NOT EXISTS TblFilterNameSDSnapshot"
                " (FilterNameId INTEGER UNIQUE PRIMARY KEY,"
                " FilterSDSnapshotId INTEGER)");

    qry.prepare(tmp);

    res = execSingleCmd(qry);

    if(res == false)
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 4"), QString::fromUtf8("Fail create TblFilterNameSDSnapshot"));
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }
    //    else
    //      qDebug( "Inserted!" );
    qry.finish();



    //===================================================================
    // TblFilterStockDataSnapshot
    //===================================================================
    // Stock market snap shop
    tmp.sprintf("CREATE TABLE IF NOT EXISTS TblFilterStockDataSnapshot"
                "(internalStockId INTEGER UNIQUE PRIMARY KEY, "
                "totNofInicator VARCHAR(255), ");

    for(i=0; i < CDbHndl::MAX_NOF_TABLE_TAB_GUI_SEL_CTRLS-1; i++)
    {
        tmp1.sprintf("indicator%d VARCHAR(255),"
                     "nofCompareValues%d VARCHAR(255),"
                     "compare%dA VARCHAR(255),"
                     "compare%dB VARCHAR(255),"
                     "value%dA VARCHAR(255),"
                     "value%dB VARCHAR(255),", i, i, i, i, i, i);
        tmp += tmp1;
    }

    tmp1.sprintf("Indicator%d VARCHAR(255),"
                 "nofCompareValues%d VARCHAR(255),"
                 "compare%dA VARCHAR(255),"
                 "compare%dB VARCHAR(255),"
                 "value%dA VARCHAR(255),"
                 "value%dB VARCHAR(255));", i, i, i, i, i, i);

    tmp += tmp1;
    qry.prepare(tmp);

    res = execSingleCmd(qry);

    if(res == false)
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 5"), tmp);
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return res;

}



/****************************************************************
 *
 * Function:    mainAnalysisDataExists()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::mainAnalysisDataExists(QString stockName,
                                     QString stockSymbol,
                                     int &mainAnalysisId)
{
    QSqlRecord rec;
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);



    QByteArray ba = stockName.toLocal8Bit();
    const char *c_stockName = ba.data();

    QByteArray ba1 = stockSymbol.toLocal8Bit();
    const char *c_stockSymbol = ba1.data();



    str.sprintf("SELECT * "
                " FROM  TblMainAnalysis "
                " WHERE stockName = '%s' AND StockSymbol = '%s';",
                c_stockName,
                c_stockSymbol);

    qDebug() << str;


    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblMainAnalysis error 1"), qry.lastError().text().toUtf8().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        if(qry.next())
        {
            rec = qry.record();



            if(rec.value("stockName").isNull() == true || true == rec.value("StockSymbol").isNull())
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                mainAnalysisId = rec.value("MainAnalysisId").toInt();
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return true;
            }
        }
    }

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return false;
}







/****************************************************************
 *
 * Function:    insertMainAnalysisData()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::insertMainAnalysisData(QString stockName,
                                     QString stockSymbol,
                                     int &mainAnalysisId,
                                     bool dbIsHandledExternly)
{
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

    QByteArray ba = stockName.toLocal8Bit();
    const char *c_stockName = ba.data();

    QByteArray ba1 = stockSymbol.toLocal8Bit();
    const char *c_stockSymbol = ba1.data();


    str.sprintf("INSERT INTO TblMainAnalysis "
                "(stockName, StockSymbol) "
                " VALUES('%s', '%s');",
                c_stockName,
                c_stockSymbol);

    qDebug() << str;

    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblMainAnalysis"), qry.lastError().text().toLatin1().constData());
        }
        if(dbIsHandledExternly==false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    mainAnalysisId = (int) qry.lastInsertId().toInt();

    qry.finish();

    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }

    return true;
}



/****************************************************************
 *
 * Function:    getMainAnalysisData()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::mainAnalysisDateExists(QString date,
                                     int mainAnalysisId,
                                     int &analysisDateId)
{
    QSqlRecord rec;
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);



    QByteArray ba = date.toLocal8Bit();
    const char *c_date = ba.data();



    str.sprintf("SELECT * "
                " FROM  TblDateAnalysis "
                " WHERE AnalysisDate = '%s' AND MainAnalysisId = %d;",
                c_date,
                mainAnalysisId);

    qDebug() << str;


    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblMainAnalysis error 1"), qry.lastError().text().toUtf8().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        if(qry.next())
        {
            rec = qry.record();



            if(rec.value("AnalysisDate").isNull() == true || true == rec.value("MainAnalysisId").isNull())
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                analysisDateId = rec.value("AnalysisDateId").toInt();
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return true;
            }
        }
    }

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return false;
}




/****************************************************************
 *
 * Function:    insertMainAnalysisDate()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertMainAnalysisDate(QString date,
                       int mainAnalysisId,
                       int &analysisDateId,
                       bool dbIsHandledExternly)
{
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

    QByteArray ba = date.toLocal8Bit();
    const char *c_date = ba.data();


    str.sprintf("INSERT OR REPLACE INTO TblDateAnalysis "
                "(AnalysisDate, MainAnalysisId) "
                " VALUES('%s', %d);",
                c_date,
                mainAnalysisId);

    qDebug() << str;

    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblDateAnalysis"), qry.lastError().text().toUtf8().constData());
        }
        if(dbIsHandledExternly==false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    analysisDateId = (int) qry.lastInsertId().toInt();

    qry.finish();

    if(dbIsHandledExternly==false)
    {
        closeDb();
        m_mutex.unlock();
    }

    return true;
}




/*****************************************************************
 *
 * Function:		getAnalysisDataId()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getAnalysisDataId(int mainAnalysisId,
                  int analysisDateId,
                  int &analysisDataId,
                  bool dbIsHandledExternly)
{


    QSqlRecord rec;
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

  bool found = false;


    str.sprintf("SELECT TblAnalysisData.AnalysisDataId, TblAnalysisData.AnalysisDateId, TblAnalysisData.MainAnalysisId   "
                " FROM TblAnalysisData   "
                " WHERE  "
                "       TblAnalysisData.AnalysisDateId = %d AND "
                "       TblAnalysisData.MainAnalysisId = %d;",
                                                                           analysisDateId,
                                                                           mainAnalysisId);


    qDebug() << str << "\n";

    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }
    else
    {
        while(qry.next())
        {
            rec = qry.record();




            if(rec.value("AnalysisDateId").isNull() == true)
            {

                if(found == true)
                {
                    continue;
                }
                else
                {
                    qry.finish();
                    if(dbIsHandledExternly == false)
                    {
                        closeDb();
                        m_mutex.unlock();
                    }

                    return false;
                }
            }
            else
            {
                found = true;
                qDebug() << rec.value("AnalysisDateId").toString();
                qDebug() << rec.value("MainAnalysisId").toString();


                if(rec.value("AnalysisDataId").isNull() == false)
                {
                    analysisDataId = rec.value("AnalysisDataId").toInt();
                }

            }
        }
    }


    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }

    if(found == true)
    {
        return true;
    }

    return false;
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
 *
 ****************************************************************/
bool CDbHndl::
insertAnalysisData(int analysisDateId,
                   int mainAnalysisId,
                   int inputAnalysisDataId,
                   bool inputAnalysisDataIdIsValid,
                   QString companyDescription,
                   QString bigEnoughAnswer,
                   QString bigEnoughComment,
                   QString strongFinancialPositionAnswer,
                   QString strongFinancialPositionComment,
                   QString earningStabilityAnswer,
                   QString earningStabilityComment,
                   QString dividendStabilityAnswer,
                   QString dividendStabilityComment,
                   QString earningGrowthAnswer,
                   QString earningGrowthComment,
                   QString keyValuePe,
                   QString keyValuePs,
                   QString keyValueNavPriceRatio,
                   QString keyValueYield,
                   QString keyValuePriceJEKRatio,
                   QString keyValueerningsDividentRatio,
                   QString keyValueTotalDebtEquityRatio,
                   QString keyValueCurrentRatio,
                   QString trustworthyLeadershipAnswer,
                   QString trustworthyLeadershipComment,
                   QString goodOwnershipAnswer,
                   QString goodOwnershipComment,
                   QString otherInformation,
                   int &analysisDataId,
                   bool dbIsHandledExternly)
{
    QString str;

    if(dbIsHandledExternly==false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }


    QSqlQuery qry(m_db);


    if(inputAnalysisDataIdIsValid == true)
    {
        str.sprintf("INSERT OR REPLACE INTO TblAnalysisData "
                " (AnalysisDateId, "
                " companyDescription, "

                " bigEnoughAnswer, "
                " bigEnoughComment, "

                " strongFinancialPositionAnswer, "
                " strongFinancialPositionComment, "

                " earningStabilityAnswer, "
                " earningStabilityComment, "

                " dividendStabilityAnswer, "
                " dividendStabilityComment, "

                " earningGrowthAnswer, "
                " earningGrowthComment, "

                " keyValuePe, "
                " keyValuePs, "

                 " keyValueNavPriceRatio, "
                 " keyValueYield, "
                 " keyValuePriceJEKRatio, "
                 " keyValueerningsDividentRatio, "
                 " keyValueTotalDebtEquityRatio, "
                 " keyValueCurrentRatio, "
                 " trustworthyLeadershipAnswer, "
                 " trustworthyLeadershipComment, "
                 " goodOwnershipAnswer, "
                 " goodOwnershipComment,"
                 " otherInformation, "
                 " AnalysisDataId,"
                 " MainAnalysisId) "
                 " VALUES( %d,  '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', "
                       " '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', "
                       " '%s', '%s', '%s', '%s', '%s', %d, %d);",
                        analysisDateId,
                        companyDescription.toLocal8Bit().constData(),
                        bigEnoughAnswer.toLocal8Bit().constData(),
                        bigEnoughComment.toLocal8Bit().constData(),
                        strongFinancialPositionAnswer.toLocal8Bit().constData(),
                        strongFinancialPositionComment.toLocal8Bit().constData(),
                        earningStabilityAnswer.toLocal8Bit().constData(),
                        earningStabilityComment.toLocal8Bit().constData(),
                        dividendStabilityAnswer.toLocal8Bit().constData(),
                        dividendStabilityComment.toLocal8Bit().constData(),  // 10
                        earningGrowthAnswer.toLocal8Bit().constData(),
                        earningGrowthComment.toLocal8Bit().constData(),
                        keyValuePe.toLocal8Bit().constData(),
                        keyValuePs.toLocal8Bit().constData(),
                        keyValueNavPriceRatio.toLocal8Bit().constData(),
                        keyValueYield.toLocal8Bit().constData(),
                        keyValuePriceJEKRatio.toLocal8Bit().constData(),
                        keyValueerningsDividentRatio.toLocal8Bit().constData(),
                        keyValueTotalDebtEquityRatio.toLocal8Bit().constData(),
                        keyValueCurrentRatio.toLocal8Bit().constData(),              // 20
                        trustworthyLeadershipAnswer.toLocal8Bit().constData(),
                        trustworthyLeadershipComment.toLocal8Bit().constData(),
                        goodOwnershipAnswer.toLocal8Bit().constData(),
                        goodOwnershipComment.toLocal8Bit().constData(),
                        otherInformation.toLocal8Bit().constData(),
                        inputAnalysisDataId,
                        mainAnalysisId);
    }
    // New data
    else
    {
        str.sprintf("INSERT INTO TblAnalysisData "
                    " (AnalysisDateId, "
                    " companyDescription, "

                    " bigEnoughAnswer, "
                    " bigEnoughComment, "

                    " strongFinancialPositionAnswer, "
                    " strongFinancialPositionComment, "

                    " earningStabilityAnswer, "
                    " earningStabilityComment, "

                    " dividendStabilityAnswer, "
                    " dividendStabilityComment, "

                    " earningGrowthAnswer, "
                    " earningGrowthComment, "

                    " keyValuePe, "
                    " keyValuePs, "

                     " keyValueNavPriceRatio, "
                     " keyValueYield, "
                     " keyValuePriceJEKRatio, "
                     " keyValueerningsDividentRatio, "
                     " keyValueTotalDebtEquityRatio, "
                     " keyValueCurrentRatio, "
                     " trustworthyLeadershipAnswer, "
                     " trustworthyLeadershipComment, "
                     " goodOwnershipAnswer, "
                     " goodOwnershipComment,"
                     " otherInformation, "
                     " MainAnalysisId) "
                     " VALUES( %d,  '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', "
                           " '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', "
                           " '%s', '%s', '%s', '%s', '%s', %d);",
                            analysisDateId,
                            companyDescription.toLocal8Bit().constData(),
                            bigEnoughAnswer.toLocal8Bit().constData(),
                            bigEnoughComment.toLocal8Bit().constData(),
                            strongFinancialPositionAnswer.toLocal8Bit().constData(),
                            strongFinancialPositionComment.toLocal8Bit().constData(),
                            earningStabilityAnswer.toLocal8Bit().constData(),
                            earningStabilityComment.toLocal8Bit().constData(),
                            dividendStabilityAnswer.toLocal8Bit().constData(),
                            dividendStabilityComment.toLocal8Bit().constData(),  // 10
                            earningGrowthAnswer.toLocal8Bit().constData(),
                            earningGrowthComment.toLocal8Bit().constData(),
                            keyValuePe.toLocal8Bit().constData(),
                            keyValuePs.toLocal8Bit().constData(),
                            keyValueNavPriceRatio.toLocal8Bit().constData(),
                            keyValueYield.toLocal8Bit().constData(),
                            keyValuePriceJEKRatio.toLocal8Bit().constData(),
                            keyValueerningsDividentRatio.toLocal8Bit().constData(),
                            keyValueTotalDebtEquityRatio.toLocal8Bit().constData(),
                            keyValueCurrentRatio.toLocal8Bit().constData(),              // 20
                            trustworthyLeadershipAnswer.toLocal8Bit().constData(),
                            trustworthyLeadershipComment.toLocal8Bit().constData(),
                            goodOwnershipAnswer.toLocal8Bit().constData(),
                            goodOwnershipComment.toLocal8Bit().constData(),
                            otherInformation.toLocal8Bit().constData(),
                            mainAnalysisId);             // Main list primary id);
    }


    qDebug() << str;

    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblAnalysisData"), qry.lastError().text().toLatin1().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    analysisDataId = (int) qry.lastInsertId().toInt();


    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }

    return true;
}


/*****************************************************************
 *
 * Function:		getNordnetYahooKeyData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getStockAnalysisDateId(QString stockName,
                     QString stockSymbol,
                     QString analysisDate,
                     int &analysisDateId,
                     bool dbIsHandledExternly)
{


    QSqlRecord rec;
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

  bool found = false;


    str.sprintf("SELECT TblMainAnalysis.*, TblDateAnalysis.*  "
                " FROM TblMainAnalysis, TblDateAnalysis   "
                " WHERE  "
                "       TblMainAnalysis.MainAnalysisId = TblDateAnalysis.MainAnalysisId AND "
                "       lower(TblDateAnalysis.AnalysisDate) = lower('%s') AND "
                "       lower(TblMainAnalysis.stockName) = lower('%s') AND "
                "       lower(TblMainAnalysis.StockSymbol) = lower('%s');",
                                                                           analysisDate.toLocal8Bit().constData(),
                                                                           stockName.toLocal8Bit().constData(),
                                                                           stockSymbol.toLocal8Bit().constData());


    qDebug() << str << "\n";

    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }
    else
    {
        while(qry.next())
        {
            rec = qry.record();




            if(rec.value("AnalysisDate").isNull() == true)
            {

                if(found == true)
                {
                    continue;
                }
                else
                {
                    qry.finish();
                    if(dbIsHandledExternly == false)
                    {
                        closeDb();
                        m_mutex.unlock();
                    }

                    return false;
                }
            }
            else
            {
                found = true;
                qDebug() << rec.value("AnalysisDate").toString() << "\n";
                qDebug() << rec.value("stockSymbol").toString();
                qDebug() << rec.value("stockName").toString();


                if(rec.value("AnalysisDateId").isNull() == false)
                {
                    analysisDateId = rec.value("AnalysisDateId").toInt();
                }

            }
        }
    }


    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }

    if(found == true)
    {
        return true;
    }

    return false;
}


/****************************************************************
 *
 * Function:    deleteDataFromProgressMyPortfolio()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
bool CDbHndl::deleteStockAnalysisDateRecord(QString stockName,
                                            QString stockSymbol,
                                            QString analysisDate)
{
    QString str;
    int analysisDateId;

    if( false == getStockAnalysisDateId(stockName, stockSymbol, analysisDate, analysisDateId))
    {
     return false;

    }


    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    str.sprintf("DELETE FROM TblDateAnalysis WHERE AnalysisDateId = %d ", analysisDateId);

    qry.prepare(str);


    if( !qry.exec() )
    {
        qry.finish();
        closeDb();
        m_mutex.unlock();

        qDebug() << qry.lastError();
        return false;
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();

    return true;
}





/****************************************************************
 *
 * Function:    deleteDataFromProgressMyPortfolio()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
bool CDbHndl::deleteStockAnalysisDataRecord(QString stockName,
                                            QString stockSymbol,
                                            QString analysisDate)
{
    QString str;
    int analysisDateId;

    if( false == getStockAnalysisDateId(stockName, stockSymbol, analysisDate, analysisDateId))
    {
     return false;

    }


    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    str.sprintf("DELETE FROM TblAnalysisData WHERE analysisDateId = %d ", analysisDateId);



    qry.prepare(str);


    if( !qry.exec() )
    {
        qry.finish();
        closeDb();
        m_mutex.unlock();

        qDebug() << qry.lastError();
        return false;
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();

    return true;
}




/*****************************************************************
 *
 * Function:		getNordnetYahooKeyData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getStockAnalysisData(QString stockName,
                     QString stockSymbol,
                     QString analysisDate,
                     QString &companyDescription,
                     QString &bigEnoughAnswer,
                     QString &bigEnoughComment,
                     QString &strongFinancialPositionAnswer,
                     QString &strongFinancialPositionComment,
                     QString &earningStabilityAnswer,
                     QString &earningStabilityComment,
                     QString &dividendStabilityAnswer,
                     QString &dividendStabilityComment,
                     QString &earningGrowthAnswer,
                     QString &earningGrowthComment,
                     QString &keyValuePe,
                     QString &keyValuePs,
                     QString &keyValueNavPriceRatio,
                     QString &keyValueYield,
                     QString &keyValuePriceJEKRatio,
                     QString &keyValueErningsDividentRatio,
                     QString &keyValueTotalDebtEquityRatio,
                     QString &keyValueCurrentRatio,
                     QString &trustworthyLeadershipAnswer,
                     QString &trustworthyLeadershipComment,
                     QString &goodOwnershipAnswer,
                     QString &goodOwnershipComment,
                     QString &otherInformation,
                     bool dbIsHandledExternly)
{


    QSqlRecord rec;
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

  bool found = false;


    str.sprintf("SELECT TblMainAnalysis.*, TblDateAnalysis.*, TblAnalysisData.* "
                " FROM TblMainAnalysis, TblDateAnalysis, TblAnalysisData  "
                " WHERE  "
                "       TblMainAnalysis.MainAnalysisId = TblDateAnalysis.MainAnalysisId AND "
                "       TblDateAnalysis.AnalysisDateId = TblAnalysisData.AnalysisDateId AND "
                "       lower(TblDateAnalysis.AnalysisDate) = lower('%s') AND "
                "       lower(TblMainAnalysis.stockName) = lower('%s') AND "
                "       lower(TblMainAnalysis.StockSymbol) = lower('%s');",
                                                                           analysisDate.toLocal8Bit().constData(),
                                                                           stockName.toLocal8Bit().constData(),
                                                                           stockSymbol.toLocal8Bit().constData());


    qDebug() << str << "\n";

    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }
    else
    {
        while(qry.next())
        {
            rec = qry.record();




            if(rec.value("AnalysisDate").isNull() == true)
            {

                if(found == true)
                {
                    continue;
                }
                else
                {
                    qry.finish();
                    if(dbIsHandledExternly == false)
                    {
                        closeDb();
                        m_mutex.unlock();
                    }

                    return false;
                }
            }
            else
            {
                found = true;
                qDebug() << rec.value("AnalysisDate").toString() << "\n";
                qDebug() << rec.value("stockSymbol").toString();
                qDebug() << rec.value("stockName").toString();


                companyDescription.clear();
                if(rec.value("companyDescription").isNull() == false)
                {
                    companyDescription = rec.value("companyDescription").toString();
                }

                bigEnoughAnswer.clear();
                if(rec.value("bigEnoughAnswer").isNull() == false)
                {
                    bigEnoughAnswer = rec.value("bigEnoughAnswer").toString();
                }

                bigEnoughComment.clear();
                if(rec.value("bigEnoughComment").isNull() == false)
                {
                    bigEnoughComment = rec.value("bigEnoughComment").toString();
                }

                strongFinancialPositionAnswer.clear();
                if(rec.value("strongFinancialPositionAnswer").isNull() == false)
                {
                    strongFinancialPositionAnswer = rec.value("strongFinancialPositionAnswer").toString();
                }

                strongFinancialPositionComment.clear();
                if(rec.value("strongFinancialPositionComment").isNull() == false)
                {
                     strongFinancialPositionComment = rec.value("strongFinancialPositionComment").toString();
                }

                earningStabilityAnswer.clear();
                if(rec.value("earningStabilityAnswer").isNull() == false)
                {
                     earningStabilityAnswer = rec.value("earningStabilityAnswer").toString();
                }

                earningStabilityComment.clear();
                if(rec.value("earningStabilityComment").isNull() == false)
                {
                     earningStabilityComment = rec.value("earningStabilityComment").toString();
                }

                dividendStabilityAnswer.clear();
                if(rec.value("dividendStabilityAnswer").isNull() == false)
                {
                     dividendStabilityAnswer = rec.value("dividendStabilityAnswer").toString();
                }

                dividendStabilityComment.clear();
                if(rec.value("dividendStabilityComment").isNull() == false)
                {
                     dividendStabilityComment = rec.value("dividendStabilityComment").toString();
                }

                earningGrowthAnswer.clear();
                if(rec.value("earningGrowthAnswer").isNull() == false)
                {
                    earningGrowthAnswer = rec.value("earningGrowthAnswer").toString();
                }

                earningGrowthComment.clear();
                if(rec.value("earningGrowthComment").isNull() == false)
                {
                     earningGrowthComment = rec.value("earningGrowthComment").toString();
                }




                keyValuePe.clear();
                if(rec.value("keyValuePe").isNull() == false)
                {
                     keyValuePe = rec.value("keyValuePe").toString();
                }

                keyValuePs.clear();
                if(rec.value("keyValuePs").isNull() == false)
                {
                     keyValuePs = rec.value("keyValuePs").toString();
                }

                keyValueNavPriceRatio.clear();
                if(rec.value("keyValueNavPriceRatio").isNull() == false)
                {
                     keyValueNavPriceRatio = rec.value("keyValueNavPriceRatio").toString();
                }




                keyValueYield.clear();
                if(rec.value("keyValueYield").isNull() == false)
                {
                     keyValueYield = rec.value("keyValueYield").toString();
                }

                keyValuePriceJEKRatio.clear();
                if(rec.value("keyValuePriceJEKRatio").isNull() == false)
                {
                     keyValuePriceJEKRatio = rec.value("keyValuePriceJEKRatio").toString();
                }

                keyValueErningsDividentRatio.clear();
                if(rec.value("keyValueerningsDividentRatio").isNull() == false)
                {
                     keyValueErningsDividentRatio = rec.value("keyValueerningsDividentRatio").toString();
                }

                keyValueTotalDebtEquityRatio.clear();
                if(rec.value("keyValueTotalDebtEquityRatio").isNull() == false)
                {
                     keyValueTotalDebtEquityRatio = rec.value("keyValueTotalDebtEquityRatio").toString();
                }



                keyValueCurrentRatio.clear();
                if(rec.value("keyValueCurrentRatio").isNull() == false)
                {
                    keyValueCurrentRatio = rec.value("keyValueCurrentRatio").toString();
                }


                trustworthyLeadershipAnswer.clear();
                if(rec.value("trustworthyLeadershipAnswer").isNull() == false)
                {
                    trustworthyLeadershipAnswer = rec.value("trustworthyLeadershipAnswer").toString();
                }


                trustworthyLeadershipComment.clear();
                if(rec.value("trustworthyLeadershipComment").isNull() == false)
                {
                    trustworthyLeadershipComment = rec.value("trustworthyLeadershipComment").toString();
                }


                goodOwnershipAnswer.clear();
                if(rec.value("goodOwnershipAnswer").isNull() == false)
                {
                    goodOwnershipAnswer = rec.value("goodOwnershipAnswer").toString();
                }

                goodOwnershipComment.clear();
                if(rec.value("goodOwnershipComment").isNull() == false)
                {
                    goodOwnershipComment = rec.value("goodOwnershipComment").toString();
                }

                otherInformation.clear();
                if(rec.value("otherInformation").isNull() == false)
                {
                    otherInformation = rec.value("otherInformation").toString();
                }

            }
        }
    }


    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }

    if(found == true)
    {
        return true;
    }

    return false;
}




/*****************************************************************
 *
 * Function:		getStockAnalysisDate()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getStockAnalysisDate(QString stockName,
                     QString stockSymbol,
                     QTreeWidget *treeWidget,
                     bool dbIsHandledExternly)
{


    QSqlRecord rec;
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

  bool found = false;


    str.sprintf("SELECT TblMainAnalysis.*, TblDateAnalysis.* "
                " FROM TblMainAnalysis, TblDateAnalysis  "
                " WHERE  "
                "       TblMainAnalysis.MainAnalysisId = TblDateAnalysis.MainAnalysisId AND "
                "       lower(TblMainAnalysis.stockName) = lower('%s') AND "
                "       lower(TblMainAnalysis.StockSymbol) = lower('%s');",
                                                                           stockName.toLocal8Bit().constData(),
                                                                           stockSymbol.toLocal8Bit().constData());


    qDebug() << str << "\n";

    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }
    else
    {
        treeWidget->clear();
        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("AnalysisDate").isNull() == true)
            {

                if(found == true)
                {
                    continue;
                }
                else
                {
                    qry.finish();
                    if(dbIsHandledExternly == false)
                    {
                        closeDb();
                        m_mutex.unlock();
                    }

                    return false;
                }
            }
            else
            {
                found = true;
                qDebug() << rec.value("AnalysisDate").toString() << "\n";
                qDebug() << rec.value("stockSymbol").toString();
                qDebug() << rec.value("stockName").toString();

                QTreeWidgetItem *item = new QTreeWidgetItem;
                item->setText(0, (QString)rec.value("AnalysisDate").toString().toUtf8());
                treeWidget->addTopLevelItem(item);
            }
        }
    }


    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }

    if(found == true)
    {
        return true;
    }

    return false;
}






/*****************************************************************
 *
 * Function:		getYahooKeyData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::getYahooKeyData(QString stockSymbol, double &totalDebtDivEquity, double &currentRatio)
{
    QSqlRecord rec;
    QString str;


    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);



    stockSymbol.trimmed();

    QByteArray ba = stockSymbol.toLocal8Bit();
    const char *c_stockSymbol = ba.data();


    str.sprintf("SELECT TotalDebtDivEquity, "
                       "CurrentRatio "
               "FROM    TblYahooKeyStatistics "
               "WHERE   LOWER(StockSymbol) = LOWER('%s');",
                        c_stockSymbol);


    qry.prepare(str);

    qDebug() << str;

    if( !qry.exec() )
    {

        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("TotalDebtDivEquity").isNull()==true || (rec.value("CurrentRatio").isNull()==true))
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                CUtil cu;
                QString number;
                bool res;



                number= (QString) rec.value("TotalDebtDivEquity").toString();
                res = cu.number2double(number, totalDebtDivEquity);

                if(res == false)
                {
                    qry.finish();
                    closeDb();
                    m_mutex.unlock();
                    return false;
                }


                number = (QString) rec.value("CurrentRatio").toString();

                res = cu.number2double(number, currentRatio);

                if(res == false)
                {
                    qry.finish();
                    closeDb();
                    m_mutex.unlock();
                    return false;
                }
            }
        }
    }

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}




/*****************************************************************
 *
 * Function:		delAllTblYahooKeyStatistics()
 *
 * Description:		This function delete all data
 *
 *
 *
 *****************************************************************/
bool CDbHndl::delAllTblYahooKeyStatistics(void)
{
    QString str;
    // QSqlQuery qry;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    // Create sql question
    str.sprintf("DELETE "
                "FROM TblYahooKeyStatistics;");

    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error delete TblMainHelpSection"), qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
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
 *
 ****************************************************************/
bool CDbHndl::
insertYahooKeyStatistics(YahooKeyStatisticsST data, bool dbIsHandledExternly)
{
    QString str;

    if(true == data.StockSymbol.isEmpty())
    {
        return false;
    }

    if(true == data.MarketCap.isEmpty())
    {
        data.MarketCap = " ";
    }



    if(dbIsHandledExternly==false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }




    QSqlQuery qry(m_db);


    str.sprintf("INSERT INTO TblYahooKeyStatistics "
                " (StockSymbol, "
                " MarketCap, "
                " EnterpriseValue, "
                " TrailingPe, "
                " ForwardPe, "
                " PEGRatio, "
                " PriceDivSales, "
                " PriceDivBook, "
                " EnterpriseValueDivRevenue, "
                " EnterpriseValueDivEBITDA, " // 10
                " FiscalYearEnds, "
                " MostRecentQuarter, "
                " ProfitMargin, "
                " OperatingMargin, "
                " ReturnOnAssets, "
                " ReturnOnEquity, "
                " Revenue, "
                " RevenuePerShare, "
                " QtrlyRevenueGrowth, "
                " GrossProfit, "
                " EBITDA, "
                " NetIncomeAvlToCommon, "
                " DilutedEPS, "
                " QtrlyEarningsGrowth, "
                " TotalCash, "
                " TotalCashPerShare, "
                " TotalDebt, "
                " TotalDebtDivEquity, "
                " CurrentRatio, "
                " BookValuePerShare, "  // 30
                " OperatingCashFlow, "
                " LeveredFreeCashFlow, "
                " Beta, "
                " Week52Change, "
                " S_And_P500_52WeekChange, "
                " ProcentHeldbyInsiders, "
                " ProcentHeldbyInstitutions, "
                " SharesShort, "
                " ShortRatio, "
                " ShortProcentOfFloat, "    // 40
                " SharesShortPriorMonth, "
                " ForwardAnnualDividendRate, "
                " ForwardAnnualDividendYield, "
                " Week52High, "
                " Week52Low, "
                " Day50MovingAverage, "
                " Day200MovingAverage, "
                " AvgVol3Month, "
                " AvgVol10Day, "
                " SharesOutstanding, "  // 50
                " Float_, "
                " TrailingAnnualDividendYield, "
                " TrailingAnnualDividendYield1, "
                " Year5AverageDividendYield, "
                " PayoutRatio, "
                " DividendDate, "
                " ExDividendDate, "
                " LastSplitFactor, "
                " LastSplitDate) "  // 59

                " VALUES('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', "
                       " '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', "
                       " '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', "
                       " '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', "
                       " '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', "
                       " '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s');",
                        data.StockSymbol.toLocal8Bit().constData(),
                        data.MarketCap.toLocal8Bit().constData(),
                        data.EnterpriseValue.toLocal8Bit().constData(),
                        data.TrailingPe.toLocal8Bit().constData(),
                        data.ForwardPe.toLocal8Bit().constData(),
                        data.PEGRatio.toLocal8Bit().constData(),
                        data.PriceDivSales.toLocal8Bit().constData(),
                        data.PriceDivBook.toLocal8Bit().constData(),
                        data.EnterpriseValueDivRevenue.toLocal8Bit().constData(),
                        data.EnterpriseValueDivEBITDA.toLocal8Bit().constData(), // 10
                        data.FiscalYearEnds.toLocal8Bit().constData(),
                        data.MostRecentQuarter.toLocal8Bit().constData(),
                        data.ProfitMargin.toLocal8Bit().constData(),
                        data.OperatingMargin.toLocal8Bit().constData(),
                        data.ReturnOnAssets.toLocal8Bit().constData(),
                        data.ReturnOnEquity.toLocal8Bit().constData(),
                        data.Revenue.toLocal8Bit().constData(),
                        data.RevenuePerShare.toLocal8Bit().constData(),
                        data.QtrlyRevenueGrowth.toLocal8Bit().constData(),
                        data.GrossProfit.toLocal8Bit().constData(),
                        data.EBITDA.toLocal8Bit().constData(),
                        data.NetIncomeAvlToCommon.toLocal8Bit().constData(),
                        data.DilutedEPS.toLocal8Bit().constData(),
                        data.QtrlyEarningsGrowth.toLocal8Bit().constData(),
                        data.TotalCash.toLocal8Bit().constData(),
                        data.TotalCashPerShare.toLocal8Bit().constData(),
                        data.TotalDebt.toLocal8Bit().constData(),
                        data.TotalDebtDivEquity.toLocal8Bit().constData(),
                        data.CurrentRatio.toLocal8Bit().constData(),
                        data.BookValuePerShare.toLocal8Bit().constData(),  // 30
                        data.OperatingCashFlow.toLocal8Bit().constData(),
                        data.LeveredFreeCashFlow.toLocal8Bit().constData(),
                        data.Beta.toLocal8Bit().constData(),
                        data.Week52Change.toLocal8Bit().constData(),
                        data.S_And_P500_52WeekChange.toLocal8Bit().constData(),
                        data.ProcentHeldbyInsiders.toLocal8Bit().constData(),
                        data.ProcentHeldbyInstitutions.toLocal8Bit().constData(),
                        data.SharesShort.toLocal8Bit().constData(),
                        data.ShortRatio.toLocal8Bit().constData(),
                        data.ShortProcentOfFloat.toLocal8Bit().constData(),    // 40
                        data.SharesShortPriorMonth.toLocal8Bit().constData(),
                        data.ForwardAnnualDividendRate.toLocal8Bit().constData(),
                        data.ForwardAnnualDividendYield.toLocal8Bit().constData(),
                        data.Week52High.toLocal8Bit().constData(),
                        data.Week52Low.toLocal8Bit().constData(),
                        data.Day50MovingAverage.toLocal8Bit().constData(),
                        data.Day200MovingAverage.toLocal8Bit().constData(),
                        data.AvgVol3Month.toLocal8Bit().constData(),
                        data.AvgVol10Day.toLocal8Bit().constData(),
                        data.SharesOutstanding.toLocal8Bit().constData(),  // 50
                        data.Float_.toLocal8Bit().constData(),
                        data.TrailingAnnualDividendYield.toLocal8Bit().constData(),
                        data.TrailingAnnualDividendYield1.toLocal8Bit().constData(),
                        data.Year5AverageDividendYield.toLocal8Bit().constData(),
                        data.PayoutRatio.toLocal8Bit().constData(),
                        data.DividendDate.toLocal8Bit().constData(),
                        data.ExDividendDate.toLocal8Bit().constData(),
                        data.LastSplitFactor.toLocal8Bit().constData(),
                        data.LastSplitDate.toLocal8Bit().constData());


    qDebug() << str << "\n";
    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Delete"), qry.lastError().text().toLatin1().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }

    return true;
}




/*****************************************************************
 *
 * Function:		getNordnetYahooKeyData()
 *
 * Description:     This function set unused search parameters
 *                  to excessive values.
 *
 *
 *****************************************************************/
void CDbHndl::
setNordnetYahooInputNonValidKeyData(YahooNordnetInputkeyData_ST &inData)
{
    // Search criterias min, max values

    // Normal value > 5
    if(false == inData.peMinIsValid)
    {
        inData.peMin = "-1000.0";
    }

    // Normal value < 15
    if(false == inData.peMaxIsValid)
    {
        inData.peMax = "1000.0";
    }

    // Normal value < 0.75
    if(false == inData.psIsValid)
    {
        inData.psValue = "1000.0";
    }

    // Normal value > 2.0
    if(false == inData.earningsToDividendRatioIsValid)
    {
        inData.earningsToDividendRatio = "-1000.0";
    }


    // Normal value < 1.0
    if(false == inData.priceToJEKRatioIsValid)
    {
        inData.priceToJEKRatio = "1000.0";
    }


    // Normal value < 0.5
    if(false == inData.totalDebtDivEquityIsValid)
    {
        inData.totalDebtDivEquity = "1000.0";
    }

    // Normal value > 1.5
    if(false == inData.currentRatioIsValid)
    {
        inData.currentRatio = "-1000.0";
    }


    // Normal value > 1
    if(false == inData.netAssetValueToPriceRatioIsValid)
    {
        inData.netAssetValueToPriceRatio = "-1000.0";
    }


    // Normal value > 4
    if(false == inData.yieldIsValid)
    {
        inData.yield = "-1000.0";
    }

 }




/*****************************************************************
 *
 * Function:		getNordnetYahooKeyData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getNordnetYahooKeyData(YahooNordnetInputkeyData_ST inData,
                       QVector <YahooNordnetOutputkeyData_ST> &stockArr,
                       bool dbIsHandledExternly)
{

    YahooNordnetOutputkeyData_ST outData;

    // set unused search parameters to excessive values.
    setNordnetYahooInputNonValidKeyData(inData);


    QSqlRecord rec;
    QString str;
    CUtil cu;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

   cu.number2double(inData.currentRatio, inData.currentRatio);
   cu.number2double(inData.earningsToDividendRatio, inData.earningsToDividendRatio);
   cu.number2double(inData.netAssetValueToPriceRatio, inData.netAssetValueToPriceRatio);
   cu.number2double(inData.peMax, inData.peMax);
   cu.number2double(inData.peMin, inData.peMin);
   cu.number2double(inData.priceToJEKRatio, inData.priceToJEKRatio);
   cu.number2double(inData.psValue, inData.psValue);
   cu.number2double(inData.totalDebtDivEquity, inData.totalDebtDivEquity);
   cu.number2double(inData.yield, inData.yield);
  bool found = false;


    str.sprintf("SELECT TblNordnetYahooBridge.*, TblStockDataSnapshot.*, TblYahooKeyStatistics.*, "
                "       TblTaStockList.taStockListId, TblTaStockList.stockListName, "
                "       TblTaStockName.taStockListId, TblTaStockName.stockSymbol, TblTaStockName.stockName "
                " FROM TblNordnetYahooBridge, TblStockDataSnapshot, TblYahooKeyStatistics, TblTaStockList, TblTaStockName  "

                " WHERE CAST(TblStockDataSnapshot.keyValuePE AS REAL) >= %s AND "
                "       CAST(TblStockDataSnapshot.keyValuePE AS REAL) <= %s AND  "
                "       CAST(TblStockDataSnapshot.keyValuePS AS REAL) <= %s AND  "
                "       CAST(TblStockDataSnapshot.earningsDividedByDividend AS REAL) >= %s AND   "
                "       CAST(TblStockDataSnapshot.keyValueCoursePerJEK AS REAL) <= %s AND   "
                "       CAST(TblYahooKeyStatistics.TotalDebtDivEquity AS REAL) <= %s AND   "
                "       CAST(TblYahooKeyStatistics.CurrentRatio AS REAL) >= %s AND "
                "       CAST(TblStockDataSnapshot.navDivLastStockPrice AS REAL) >= %s AND "
                "       CAST(TblStockDataSnapshot.keyValueYield AS REAL) >= %s AND "
                "       TblTaStockList.taStockListId = TblTaStockName.taStockListId AND "
                "       TblTaStockName.stockSymbol = TblNordnetYahooBridge.assetSymbol AND "
                "       TblTaStockList.taStockListId = %d AND "
                "       lower(TblTaStockList.stockListName) = lower('%s') AND "
                "       lower(TblStockDataSnapshot.companyName) = lower(TblNordnetYahooBridge.assetName) AND "
                "       lower(TblNordnetYahooBridge.assetSymbol) = lower(TblYahooKeyStatistics.StockSymbol);",
                                                                           inData.peMin.toLocal8Bit().constData(),
                                                                           inData.peMax.toLocal8Bit().constData(),
                                                                           inData.psValue.toLocal8Bit().constData(),
                                                                           inData.earningsToDividendRatio.toLocal8Bit().constData(),
                                                                           inData.priceToJEKRatio.toLocal8Bit().constData(),
                                                                           inData.totalDebtDivEquity.toLocal8Bit().constData(),
                                                                           inData.currentRatio.toLocal8Bit().constData(),
                                                                           inData.netAssetValueToPriceRatio.toLocal8Bit().constData(),
                                                                           inData.yield.toLocal8Bit().constData(),
                                                                           inData.stockListId,
                                                                           inData.stockList.toLocal8Bit().constData());


    qDebug() << str << "\n";

    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }
    else
    {
        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("StockSymbol").isNull() == true ||
               rec.value("TotalDebtDivEquity").isNull() == true ||
               rec.value("CurrentRatio").isNull() == true ||

               rec.value("ProfitMargin").isNull() == true ||
               rec.value("OperatingMargin").isNull() == true ||
               rec.value("ReturnOnAssets").isNull() == true ||
               rec.value("ReturnOnEquity").isNull() == true ||
               rec.value("Week52High").isNull() == true ||
               rec.value("Week52Low").isNull() == true)
            {

                if(found == true)
                {
                    continue;
                    //return true;
                }
                else
                {
                    qry.finish();
                    if(dbIsHandledExternly == false)
                    {
                        closeDb();
                        m_mutex.unlock();
                    }

                    return false;
                }
            }
            else
            {
               found = true;
                qDebug() << rec.value("companyName").toString() << "\n";

                outData.stockSymbol = rec.value("stockSymbol").toString();
                outData.companyName = rec.value("companyName").toString();
                outData.lastPrice = rec.value("lastPrice").toString();
                outData.procentChangeOneDay = rec.value("procentChangeOneDay").toString();
                outData.volume = rec.value("volume").toString();
                outData.totalDebtToEquityRatio = rec.value("TotalDebtDivEquity").toString();
                outData.currentRatio = rec.value("CurrentRatio").toString();
                outData.earningsToDividendRatio = rec.value("earningsDividedByDividend").toString();
                outData.pe = rec.value("keyValuePE").toString();
                outData.ps = rec.value("keyValuePS").toString();
                outData.netAssetValueToPriceRatio = rec.value("navDivLastStockPrice").toString();
                outData.yield = rec.value("keyValueYield").toString();
                outData.priceToJEKRatio = rec.value("keyValueCoursePerJEK").toString();
                outData.profitMargin = rec.value("ProfitMargin").toString();
                outData.operatingMargin = rec.value("OperatingMargin").toString();
                outData.returnOnAssets = rec.value("ReturnOnAssets").toString();
                outData.returnOnEquity = rec.value("ReturnOnEquity").toString();
                outData.week52High = rec.value("Week52High").toString();
                outData.week52Low = rec.value("Week52Low").toString();

                stockArr.append(outData);

            }
        }



    }


    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }

    if(found == true)
    {
        return true;
    }

    return false;
}



/*****************************************************************
 *
 * Function:		getNordnetYahooKeyData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getNordnetYahooSingleKeyData(QString stockSymbol,
                             int stockListId,
                             QString stockListName,
                       YahooNordnetOutputkeyData_ST &outData,
                       bool dbIsHandledExternly)
{




    QSqlRecord rec;
    QString str;
    CUtil cu;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

  bool found = false;


    str.sprintf("SELECT TblNordnetYahooBridge.*, TblStockDataSnapshot.*, TblYahooKeyStatistics.*, "
                "       TblTaStockList.taStockListId, TblTaStockList.stockListName, "
                "       TblTaStockName.taStockListId, TblTaStockName.stockSymbol, TblTaStockName.stockName "
                " FROM TblNordnetYahooBridge, TblStockDataSnapshot, TblYahooKeyStatistics, TblTaStockList, TblTaStockName  "

                " WHERE "
                "       TblTaStockList.taStockListId = TblTaStockName.taStockListId AND "
                "       TblTaStockName.stockSymbol = TblNordnetYahooBridge.assetSymbol AND "
                "       TblTaStockList.taStockListId = %d AND "
                "       lower(TblTaStockList.stockListName) = lower('%s') AND "
                "       lower(TblNordnetYahooBridge.assetSymbol) = lower('%s') AND "
                "       lower(TblStockDataSnapshot.companyName) = lower(TblNordnetYahooBridge.assetName) AND "
                "       lower(TblNordnetYahooBridge.assetSymbol) = lower(TblYahooKeyStatistics.StockSymbol);",
                                                                           stockListId,
                                                                           stockListName.toLocal8Bit().constData(),
                                                                           stockSymbol.toLocal8Bit().constData());


    qDebug() << str << "\n";

    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }
    else
    {
        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("StockSymbol").isNull() == true ||
               rec.value("TotalDebtDivEquity").isNull() == true ||
               rec.value("CurrentRatio").isNull() == true ||

               rec.value("ProfitMargin").isNull() == true ||
               rec.value("OperatingMargin").isNull() == true ||
               rec.value("ReturnOnAssets").isNull() == true ||
               rec.value("ReturnOnEquity").isNull() == true ||
               rec.value("Week52High").isNull() == true ||
               rec.value("Week52Low").isNull() == true)
            {

                if(found == true)
                {
                    continue;
                    //return true;
                }
                else
                {
                    qry.finish();
                    if(dbIsHandledExternly == false)
                    {
                        closeDb();
                        m_mutex.unlock();
                    }

                    return false;
                }
            }
            else
            {
               found = true;
                qDebug() << rec.value("companyName").toString() << "\n";

                outData.stockSymbol = rec.value("stockSymbol").toString();
                outData.companyName = rec.value("companyName").toString();
                outData.lastPrice = rec.value("lastPrice").toString();
                outData.procentChangeOneDay = rec.value("procentChangeOneDay").toString();
                outData.volume = rec.value("volume").toString();
                outData.totalDebtToEquityRatio = rec.value("TotalDebtDivEquity").toString();
                outData.currentRatio = rec.value("CurrentRatio").toString();
                outData.earningsToDividendRatio = rec.value("earningsDividedByDividend").toString();
                outData.pe = rec.value("keyValuePE").toString();
                outData.ps = rec.value("keyValuePS").toString();
                outData.netAssetValueToPriceRatio = rec.value("navDivLastStockPrice").toString();
                outData.yield = rec.value("keyValueYield").toString();
                outData.priceToJEKRatio = rec.value("keyValueCoursePerJEK").toString();
                outData.profitMargin = rec.value("ProfitMargin").toString();
                outData.operatingMargin = rec.value("OperatingMargin").toString();
                outData.returnOnAssets = rec.value("ReturnOnAssets").toString();
                outData.returnOnEquity = rec.value("ReturnOnEquity").toString();
                outData.week52High = rec.value("Week52High").toString();
                outData.week52Low = rec.value("Week52Low").toString();
            }
        }



    }


    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }

    if(found == true)
    {
        return true;
    }

    return false;
}





/*****************************************************************
 *
 * Function:		getDebtToEquityRatio()
 *
 * Description:		Debt/Equity < 0.5 is good
 *
 *
 *
 *****************************************************************/
bool CDbHndl::getDebtToEquityRatio(QString assetSymbol, double &debtToEquityRatio)
{
    QSqlRecord rec;
    QString str;


    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);



    double fixedAssets;
    double currentAssets;
    double equity;
    double liabilities;



    assetSymbol.trimmed();

    QByteArray ba = assetSymbol.toLocal8Bit();
    const char *c_assetSymbol = ba.data();


    str.sprintf("SELECT TblNordnetBalanceIncomeSheet.assetName, "
                       "TblNordnetBalanceIncomeSheet.fixedAssets, "       // Anläggningstillgångar
                       "TblNordnetBalanceIncomeSheet.currentAssets, "     // Omsättningstillgångar
                       "TblNordnetBalanceIncomeSheet.equity, "            // Eget kapital
                       "TblNordnetBalanceIncomeSheet.liabilities, "       // Skulder
                       "TblNordnetYahooBridge.assetSymbol, "
                       "TblNordnetYahooBridge.assetName "
               "FROM    TblNordnetBalanceIncomeSheet, TblNordnetYahooBridge "
               "WHERE   TblNordnetYahooBridge.assetName == TblNordnetBalanceIncomeSheet.assetName AND "
                       "LOWER(TblNordnetYahooBridge.assetSymbol) = LOWER('%s');",
                        c_assetSymbol);




    qry.prepare(str);

    qDebug() << str;

    if( !qry.exec() )
    {

        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("fixedAssets").isNull()==true || (rec.value("currentAssets").isNull()==true) ||
              (rec.value("equity").isNull() ==true) || (rec.value("liabilities").isNull() ==true))
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                CUtil cu;
                QString number;
                bool res;



                number= (QString) rec.value("fixedAssets").toString();
                res = cu.number2double(number, fixedAssets);

                if(res == false)
                {
                    qry.finish();
                    closeDb();
                    m_mutex.unlock();
                    return false;
                }


                number = (QString) rec.value("currentAssets").toString();

                res = cu.number2double(number, currentAssets);

                if(res == false)
                {
                    qry.finish();
                    closeDb();
                    m_mutex.unlock();
                    return false;
                }

                number = (QString) rec.value("equity").toString();

                res = cu.number2double(number, equity);

                if(res == false)
                {
                    qry.finish();
                    closeDb();
                    m_mutex.unlock();
                    return false;
                }

                number = (QString) rec.value("liabilities").toString();

                res = cu.number2double(number, liabilities);

                if(res == false)
                {
                    qry.finish();
                    closeDb();
                    m_mutex.unlock();
                    return false;
                }

                double denominator = /*fixedAssets + currentAssets +*/ equity;
                 if(denominator == 0)
                {
                    denominator = 0.001;
                }
                debtToEquityRatio = liabilities / equity * 100;
            }
        }
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}


//========

/*****************************************************************
 *
 * Function:		delAllTblNordnetBalanceIncomeSheet()
 *
 * Description:		This function delete all data
 *
 *
 *
 *****************************************************************/
bool CDbHndl::delAllTblNordnetCompanyDescription(void)
{
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    // Create sql question
    str.sprintf("DELETE "
                "FROM TblNordnetCompanyDescription;");

    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 1aa2"), qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}


/****************************************************************
 *
 * Function:    addStockSymbolAndNameInTreeWidget1()
 *
 * Description: This combobox has a checkbox
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::getNordnetCompanyDescription(NordnetCompanyDescription_ST &data,
                                           QString assetName)
{
    QSqlRecord rec;
    QString str;


    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);



    str.sprintf("SELECT assetName, assetGroup, assetBranch, executiveDirector, businessDescription "
                " FROM TblNordnetCompanyDescription "
                " WHERE LOWER(assetName) = LOWER('%s');",
                assetName.toLocal8Bit().constData());

    qry.prepare(str);

    qDebug() << str;

    if( !qry.exec() )
    {

        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("assetName").isNull()==true || (rec.value("assetGroup").isNull()==true) ||
              (rec.value("assetBranch").isNull() ==true) || (rec.value("executiveDirector").isNull() ==true) ||
              (rec.value("businessDescription").isNull() ==true))
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                data.sector = (QString)rec.value("assetGroup").toString().toLatin1();
                data.assetName = (QString)rec.value("assetName").toString().toLatin1();
                data.bransch = (QString)rec.value("assetBranch").toString().toLatin1();
                data.businessDescription = (QString)rec.value("businessDescription").toString().toLatin1();
                data.executiveDirector = (QString)rec.value("executiveDirector").toString().toLatin1();
            }
        }
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}







/****************************************************************
 *
 * Function:    getAllProgressMyPortfolioData()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
bool CDbHndl::getMinDateProgressMyPortfolio(int &year, int &month)
{
    QSqlRecord rec;
    QString str;
    QString tmp;
    CUtil cu;
    //int row;
    //int col;



    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);



    str.sprintf(" SELECT YearMonthTxt, Year, Month "
                " FROM TblProgressMyPortfolio "
                " WHERE "
                " CAST(YearMonthTxt AS REAL) = (SELECT MIN(CAST(YearMonthTxt AS REAL)) FROM TblProgressMyPortfolio);");

    qry.prepare(str);

    qDebug() << str;

    if( !qry.exec() )
    {
        //table.deleteAllData(tableView);


        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error MyPortfolio"), qry.lastError().text().toUtf8().constData());
        }

        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        //table.deleteAllData(tableView);

      //  row = 0;
      //  col = 0;

        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("YearMonthTxt").isNull() == true)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("db error MyPortfolio  1"), QString::fromUtf8("Key info is missing in db"));
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                 if(false == rec.value("Year").isNull())
                 {
                    year = rec.value("Year").toInt();
                 }

                 if(false == rec.value("Month").isNull())
                 {
                    month = rec.value("month").toInt();

                 }
            }
        }
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}




/*****************************************************************
 *
 * Function:		delAllTblProgressMyPortfolio()
 *
 * Description:		This function delete all data
 *
 *
 *
 *****************************************************************/
bool CDbHndl::delAllTblProgressMyPortfolio(void)
{
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    // Create sql question
    str.sprintf("DELETE "
                "FROM TblProgressMyPortfolio;");

    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database TblProgressMyPortfolio"), qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}






/****************************************************************
 *
 * Function:    deleteDataFromProgressMyPortfolio()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
bool CDbHndl::deleteDataFromProgressMyPortfolio(QString year, QString month)
{
    QString yearMonth;
    QString str;

    yearMonth.sprintf("%4d%02d", year.toInt(), month.toInt());

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    str.sprintf( "DELETE FROM TblProgressMyPortfolio WHERE YearMonthTxt = %s ", yearMonth.toLocal8Bit().constData());



    qry.prepare(str);


    if( !qry.exec() )
    {
        qry.finish();
        closeDb();
        m_mutex.unlock();

        qDebug() << qry.lastError();
        return false;
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();

    return true;
}


/****************************************************************
 *
 * Function:    getNofData()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
bool CDbHndl::getNofRowsProgressMyPortfolioData(int &nofRows)
{
    QSqlRecord rec;
    QString str;



    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);



    str.sprintf("SELECT Count(*), YearMonthTxt  "
                " FROM TblProgressMyPortfolio;");

    qry.prepare(str);

    qDebug() << str;

    if( !qry.exec() )
    {
        //table.deleteAllData(tableView);


        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error MyPortfolio"), qry.lastError().text().toUtf8().constData());
        }

        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {

        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("YearMonthTxt").isNull() == true)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("db error MyPortfolio  1"), QString::fromUtf8("Key info is missing in db"));
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                // Nof data
                if(false == rec.value(0).isNull())
                {
                   nofRows = rec.value(0).toInt();
                }
                else
                {
                    qry.finish();
                    closeDb();
                    m_mutex.unlock();
                    return false;
                }

            }
        }
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}



/****************************************************************
 *
 * Function:    getAllProgressMyPortfolioData()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
bool CDbHndl::getAllProgressMyPortfolioData(CExtendedTable &table, QTableView *tableView)
{
    QSqlRecord rec;
    QString str;
    QString tmp;
    int row;
    int col;



    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);



    str.sprintf("SELECT YearMonthTxt, Year, Month, MarketValue, AddedMoney, ReceivedDivident, Withdrawing "
                " FROM TblProgressMyPortfolio "
                " ORDER BY CAST(YearMonthTxt AS REAL) DESC;");

    qry.prepare(str);

    qDebug() << str;

    if( !qry.exec() )
    {
        //table.deleteAllData(tableView);


        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error MyPortfolio"), qry.lastError().text().toUtf8().constData());
        }

        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        //table.deleteAllData(tableView);

        row = 0;
        col = 0;

        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("YearMonthTxt").isNull() == true)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("db error MyPortfolio  1"), QString::fromUtf8("Key info is missing in db"));
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                  if(false == rec.value("Year").isNull())
                 {
                    tmp = (QString)rec.value("Year").toString().toUtf8();

                    table.addDataSetTextColor(tableView, tmp, row, col, Qt::black);
                 }
                 col++;

                 if(false == rec.value("Month").isNull())
                 {
                    tmp = (QString)rec.value("Month").toString().toUtf8();

                    table.addDataSetTextColor(tableView, tmp, row, col, Qt::black);
                 }
                 col++;


                 if(false == rec.value("MarketValue").isNull())
                 {
                    tmp = (QString)rec.value("MarketValue").toString().toUtf8();

                    table.addDataSetTextColor(tableView, tmp, row, col, Qt::black);
                 }
                 col++;

                 if(false == rec.value("AddedMoney").isNull())
                 {
                    tmp = (QString)rec.value("AddedMoney").toString().toUtf8();

                    table.addDataSetTextColor(tableView, tmp, row, col, Qt::black);
                 }
                 col++;


                 if(false == rec.value("ReceivedDivident").isNull())
                 {
                    tmp = (QString)rec.value("ReceivedDivident").toString().toUtf8();

                    table.addDataSetTextColor(tableView, tmp, row, col, Qt::black);
                 }
                 col++;


                 if(false == rec.value("Withdrawing").isNull())
                 {
                    tmp = (QString)rec.value("Withdrawing").toString().toUtf8();

                    table.addDataSetTextColor(tableView, tmp, row, col, Qt::black);
                 }

                 col = 0;
                 row++;
            }
        }
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
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
 *
 ****************************************************************/
bool CDbHndl::
insertProgressMyPortfolioData(QString year,
                              QString month,
                              QString marketValue,
                              QString addedMoney,
                              QString receivedDivident,
                              QString withdrawing,
                              bool dbIsHandledExternly)
{
    QString str;
    QString yearMonth;

    if(!(year.size() >  0 ))
    {
        QMessageBox::information(NULL, QString::fromUtf8("Fel"), QString::fromUtf8("Ange år"));
        return false;
    }

    if(!(month.size() >  0 ))
    {
        QMessageBox::information(NULL, QString::fromUtf8("Fel"), QString::fromUtf8("Ange Månad"));
        return false;

    }

    if(!(marketValue.size() >  0 ))
    {
        marketValue = "0.0";

    }

    if(!(addedMoney.size() >  0 ))
    {
        addedMoney = "0.0";

    }


    if(!(receivedDivident.size() >  0 ))
    {
        receivedDivident = "0.0";

    }


    if(!(withdrawing.size() >  0 ))
    {
        withdrawing = "0.0";
    }




    yearMonth.sprintf("%4d%02d", year.toInt(), month.toInt());


    if(dbIsHandledExternly==false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }


    QSqlQuery qry(m_db);


    str.sprintf("INSERT OR REPLACE INTO TblProgressMyPortfolio "
                " (YearMonthTxt, Year, Month, MarketValue, AddedMoney, ReceivedDivident, Withdrawing) "
                " VALUES('%s', "
                " %d, %d, %.2f, %.2f, %.2f, %.2f); ",
                       yearMonth.toLocal8Bit().constData(),
                       year.toInt(),
                       month.toInt(),
                       marketValue.toDouble(),
                       addedMoney.toDouble(),
                       receivedDivident.toDouble(),
                       withdrawing.toDouble());



    qDebug() << str << "\n";
    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error"), qry.lastError().text().toLatin1().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }

    return true;
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
 *
 ****************************************************************/
bool CDbHndl::
insertNordnetCompanyDescriptionData(NordnetCompanyDescription_ST data, bool dbIsHandledExternly)
{
    QString str;

    if(!(data.assetName.size() >  0 /*&&
       data.assetSymbol.size() >  0 &&
       data.sector.size() >  0 &&
       data.bransch.size() >  0 &&
       data.executiveDirector.size() >  0 &&
       data.businessDescription.size() > 0*/))
    {
        return false;
    }


    if(dbIsHandledExternly==false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }




    QSqlQuery qry(m_db);


    str.sprintf("INSERT INTO TblNordnetCompanyDescription "
                "(assetName, assetSymbol, assetGroup, assetBranch, executiveDirector, businessDescription) "
                " VALUES('%s', '%s', '%s', '%s', '%s', '%s');",
                data.assetName.toLocal8Bit().constData(),
                data.assetSymbol.toLocal8Bit().constData(),
                data.sector.toLocal8Bit().constData(),
                data.bransch.toLocal8Bit().constData(),
                data.executiveDirector.toLocal8Bit().constData(),
                data.businessDescription.toLocal8Bit().constData());



    qDebug() << str << "\n";
    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error"), qry.lastError().text().toLatin1().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }

    return true;
}



//=========


/****************************************************************
 *
 * Function:    addFa2NetProfitAfterTaxToTreeWidget()
 *
 * Description: Page 2  vinst/aktie, vinst i miljoner kr, vinsttillväxt
 *              Adding  profitDivNofShares,
 *                      profitIncomeDiv1000
 *                      profit growth
 *
 *              Calc profit Growth = (profitDivNofShares - refProfit) / refProfit;
 *
 *
 ****************************************************************/
bool CDbHndl::addFa2NetProfitAfterTaxToTreeWidget(QTreeWidget *treeWidget,
                                                QString assetName, QString &assetGroup)
{
    QSqlRecord rec;
    QString str;
    QString tmp;
    CUtil cu;

    double growth;
    double profitDivNofShares;
    double profitIncomeDiv1000;
    double refProfit;
    bool profitRefIsInit = false;


    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);



    str.sprintf("SELECT assetName, year, numberOfShares, netProfitAfterTax, assetGroup "
                " FROM TblNordnetBalanceIncomeSheet "
                " WHERE LOWER(assetName) = LOWER('%s') "
                " ORDER BY LOWER(year) ASC;",              // DESC
                assetName.toLocal8Bit().constData());

    qry.prepare(str);

    qDebug() << str;

    if( !qry.exec() )
    {
        treeWidget->clear();


        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("assetName").isNull()==true || (rec.value("netProfitAfterTax").isNull()==true) ||
              (rec.value("year").isNull() ==true) || (rec.value("numberOfShares").isNull() ==true) ||
              (rec.value("assetGroup").isNull() ==true))
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {

                QTreeWidgetItem *item = new QTreeWidgetItem;

                assetGroup = (QString)rec.value("assetGroup").toString().toLatin1();


                item->setText(0, (QString)rec.value("year").toString().toUtf8());

                profitDivNofShares = rec.value("netProfitAfterTax").toDouble() / rec.value("numberOfShares").toDouble();
                tmp.sprintf("%.1f", (profitDivNofShares * 1000.0));
                item->setText(1, tmp);

                profitIncomeDiv1000 = rec.value("netProfitAfterTax").toDouble() / 1000.0;
                tmp.sprintf("%.0f", profitIncomeDiv1000);
                if(profitIncomeDiv1000 > 0)
                    item->setTextColor(2, Qt::darkGreen);
                else
                    item->setTextColor(2, Qt::red);
                item->setText(2, tmp);



                if(profitRefIsInit == true)
                {
                    if(refProfit > 0)
                    {
                        growth = (profitDivNofShares - refProfit) / refProfit;
                    }
                    else
                    {
                        double tmp = refProfit;
                        tmp = -tmp;
                        growth = (profitDivNofShares + refProfit) / refProfit;
                    }

                    tmp.sprintf("%.2f", growth*100);
                    item->setText(3, tmp);

                    if(growth*100 >= 10)
                        item->setTextColor(3, Qt::darkGreen);
                    else if(growth*100 < 10 && growth*100 >= 0)
                        item->setTextColor(3, cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3));
                    else
                        item->setTextColor(3, Qt::red);
                }
                else
                {
                    item->setText(3, " ");
                }

                if(profitRefIsInit == false)
                {
                    profitRefIsInit = true;
                }

                refProfit = profitDivNofShares;
                treeWidget->addTopLevelItem(item);
            }
        }
    }

    treeWidget->sortByColumn(0, Qt::DescendingOrder); // column/order to sort by
    treeWidget->setSortingEnabled(true);             // should cause sort on add

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}



/****************************************************************
 *
 * Function:    addFa3NetProfitAfterTaxToTreeWidget()
 *
 * Description: Page 3
 *              Vinstmarginal, ränta på eget kapital
 *
 *              Vinstmarginal =  Rörelseresultat efter avskrivningar / Rörelseintäkter
 *              operatingIncomeAfterDepDivTotalSales = operatingIncomeAfterDepDivTotalSales / operatingIncome;
 *              
 *              Årets resultat efter skatt/Eget kapital
 *              netProfitAfterTax / equity * 100
 *
 ****************************************************************/
bool CDbHndl::addFa3NetProfitAfterTaxToTreeWidget(QTreeWidget *treeWidget,
                                                QString assetName)
{
    QSqlRecord rec;
    QString str;
    QString tmp;



    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);



    str.sprintf("SELECT assetName, year, operatingIncome, operatingIncomeAfterDepreciation, "
                " netProfitAfterTax, equity "
                " FROM TblNordnetBalanceIncomeSheet "
                " WHERE LOWER(assetName) = LOWER('%s') "
                " ORDER BY LOWER(year) ASC;",              // DESC
                assetName.toLocal8Bit().constData());

    qry.prepare(str);

    qDebug() << str;

    if( !qry.exec() )
    {
        treeWidget->clear();


        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("assetName").isNull()==true || (rec.value("netProfitAfterTax").isNull()==true) ||
              (rec.value("year").isNull() ==true) || (rec.value("operatingIncome").isNull() ==true) ||
              (rec.value("equity").isNull() ==true) || (rec.value("operatingIncomeAfterDepreciation").isNull() ==true))
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {

                QTreeWidgetItem *item = new QTreeWidgetItem;


                item->setText(0, (QString)rec.value("year").toString().toUtf8());


                double operatingIncomeAfterDepDivTotalSales;
                double operatingIncome;
                operatingIncomeAfterDepDivTotalSales = rec.value("operatingIncomeAfterDepreciation").toDouble();
                operatingIncome = rec.value("operatingIncome").toDouble();
                if(operatingIncome != 0)
                {
                    operatingIncomeAfterDepDivTotalSales = operatingIncomeAfterDepDivTotalSales / operatingIncome;
                    tmp.sprintf("%.1f", (operatingIncomeAfterDepDivTotalSales * 100.0));
                    item->setText(1, tmp);
                }

                double netProfitAfterTax;
                double equity;

                netProfitAfterTax = rec.value("netProfitAfterTax").toDouble();
                equity = rec.value("equity").toDouble();
                if(equity != 0)
                {
                    tmp.sprintf("%.1f", netProfitAfterTax / equity * 100);
                    item->setText(2, tmp);
                }

                treeWidget->addTopLevelItem(item);
            }
        }
    }

    treeWidget->sortByColumn(0, Qt::DescendingOrder); // column/order to sort by
    treeWidget->setSortingEnabled(true);             // should cause sort on add

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}




/****************************************************************
 *
 * Function:    addFa3MinMaxPEAndPrice()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::addFa3MinMaxPEAndPrice(QTreeWidget *treeWidget, QTreeWidget *treeWidget1, QString assetName, QString assetSymbol)
{
    QSqlRecord rec;
    QString str;
    QString tmp;
    QString yearArr[40];
    double erningPerShare[40];
    int nofData = 0;



    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);




    str.sprintf(" SELECT assetName, year, earningsPerShare "
                " FROM TblNordnetBalanceIncomeSheet "
                " WHERE LOWER(assetName) = LOWER('%s') "
                " ORDER BY LOWER(year) ASC;",              // DESC
                assetName.toLocal8Bit().constData());

    qry.prepare(str);

    qDebug() << str;

    if( !qry.exec() )
    {
        treeWidget->clear();


        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("assetName").isNull()==true || (rec.value("earningsPerShare").isNull()==true) ||
              (rec.value("year").isNull() ==true))
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                yearArr[nofData] = (QString) rec.value("year").toString();
                yearArr[nofData] = yearArr[nofData].trimmed();

                erningPerShare[nofData] = rec.value("earningsPerShare").toDouble();
                qDebug() << erningPerShare[nofData];
                nofData++;
            }
        }
    }

    qry.finish();
    closeDb();
    m_mutex.unlock();


  QColor yearColor = Qt::black;
  QString startDate;
  QString endDate;
  double minPrice;
  double maxPrice;
  double avgPrice;




  for(int i = 0; i < nofData; i++)
  {
      QTreeWidgetItem *item = new QTreeWidgetItem;
      // ajn 160221 QTreeWidgetItem *item1 = new QTreeWidgetItem;

      startDate.sprintf("%s-01-01", yearArr[i].toLocal8Bit().constData());
      endDate.sprintf("%s-12-30", yearArr[i].toLocal8Bit().constData());

        if(true == getYahooTaMinMaxAvgPrice(assetSymbol, startDate, endDate, minPrice, maxPrice, avgPrice))
        {
            qDebug() << yearArr[i];
            qDebug() << minPrice;
            qDebug() << maxPrice;
            qDebug() << avgPrice;
            qDebug() << erningPerShare[i];

            m_oneLineTxtColorData.lineColor.clear();
            m_oneLineTxtColorData.lineData.clear();
            // m_oneLineTxtColorData.nofData = 4;


            item->setText(0, (QString)yearArr[i]);
            // ajn 160221 item1->setText(0, (QString)yearArr[i]);

            int len = yearArr[i].size();
            if(len > 0)
            {
                m_oneLineTxtColorData.lineData.insert(0, (QString)yearArr[i]);
                m_oneLineTxtColorData.lineColor.insert(0, yearColor);
            }
            else
            {
                tmp = " ";
                m_oneLineTxtColorData.lineData.insert(0, (QString)tmp);
                m_oneLineTxtColorData.lineColor.insert(0, yearColor);
            }


            tmp.sprintf("%.1f", (minPrice));
            item->setText(1, tmp);

            tmp.sprintf("%.1f", (avgPrice));
            item->setText(2, tmp);

            tmp.sprintf("%.1f", (maxPrice));
            item->setText(3, tmp);


            if(erningPerShare[i] != 0)
            {

                tmp.sprintf("%.1f", (minPrice/erningPerShare[i]));
                // ajn 160221 item1->setText(1, tmp);
                // ajn 160221 item1->setTextColor(1, m_gfc.getColorPe(tmp));
                qDebug() << tmp;

                if(tmp.size() > 0)
                {
                    m_oneLineTxtColorData.lineData.insert(1, tmp);
                    m_oneLineTxtColorData.lineColor.insert(1, m_gfc.getColorPe(tmp));
                }
                else
                {
                    tmp = " ";
                    m_oneLineTxtColorData.lineData.insert(1, tmp);
                    m_oneLineTxtColorData.lineColor.insert(1, m_gfc.getColorPe(tmp));
                }

                tmp.sprintf("%.1f", (avgPrice/erningPerShare[i]));
                // ajn 160221 item1->setText(2, tmp);
                // ajn 160221 item1->setTextColor(2, m_gfc.getColorPe(tmp));
                qDebug() << tmp;

                if(tmp.size() > 0)
                {
                    m_oneLineTxtColorData.lineData.insert(2, tmp);
                    m_oneLineTxtColorData.lineColor.insert(2, m_gfc.getColorPe(tmp));
                }
                else
                {
                    tmp = " ";
                    m_oneLineTxtColorData.lineData.insert(2, tmp);
                    m_oneLineTxtColorData.lineColor.insert(2, m_gfc.getColorPe(tmp));
                }


                tmp.sprintf("%.1f", (maxPrice/erningPerShare[i]));
                // ajn 160221 item1->setText(3, tmp);
                // ajn 160221 item1->setTextColor(3, m_gfc.getColorPe(tmp));
                qDebug() << tmp;

                if(tmp.size() > 0)
                {
                    m_oneLineTxtColorData.lineData.insert(3, tmp);
                    m_oneLineTxtColorData.lineColor.insert(3, m_gfc.getColorPe(tmp));
                }
                else
                {
                    tmp = " ";
                    m_oneLineTxtColorData.lineData.insert(3, tmp);
                    m_oneLineTxtColorData.lineColor.insert(3, m_gfc.getColorPe(tmp));
                }

                // Pe min avg max
                // ajn 160221 treeWidget1->addTopLevelItem(item1);
                m_mtw.addOneLineTxtColorData(treeWidget1, m_oneLineTxtColorData);

                m_oneLineTxtColorData.lineColor.clear();
                m_oneLineTxtColorData.lineData.clear();

            }

            treeWidget->addTopLevelItem(item);
        }
  }



  treeWidget->sortByColumn(0, Qt::DescendingOrder); // column/order to sort by
  treeWidget->setSortingEnabled(true);             // should cause sort on add

  treeWidget1->sortByColumn(0, Qt::DescendingOrder); // column/order to sort by
  treeWidget1->setSortingEnabled(true);             // should cause sort on add


    return true;
}



#if 0
/****************************************************************
 *
 * Function:    addStockSymbolAndNameInTreeWidget1()
 *
 * Description: This combobox has a checkbox
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::addFa3MinMaxPEAndPrice(QTreeWidget *treeWidget, QTreeWidget *treeWidget1, QString assetName, QString assetSymbol)
{
    QSqlRecord rec;
    QString str;
    QString tmp;
    QString yearArr[40];
    double erningPerShare[40];
    int nofData = 0;



    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);




    str.sprintf(" SELECT assetName, year, earningsPerShare "
                " FROM TblNordnetBalanceIncomeSheet "
                " WHERE LOWER(assetName) = LOWER('%s') "
                " ORDER BY LOWER(year) ASC;",              // DESC
                assetName.toLocal8Bit().constData());

    qry.prepare(str);

    qDebug() << str;

    if( !qry.exec() )
    {
        treeWidget->clear();


        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("assetName").isNull()==true || (rec.value("earningsPerShare").isNull()==true) ||
              (rec.value("year").isNull() ==true))
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                yearArr[nofData] = (QString) rec.value("year").toString();
                yearArr[nofData] = yearArr[nofData].trimmed();

                erningPerShare[nofData] = rec.value("earningsPerShare").toDouble();
                qDebug() << erningPerShare[nofData];
                nofData++;
            }
        }
    }

    qry.finish();
    closeDb();
    m_mutex.unlock();


  QString startDate;
  QString endDate;
  double minPrice;
  double maxPrice;
  double avgPrice;




  for(int i = 0; i < nofData; i++)
  {
      QTreeWidgetItem *item = new QTreeWidgetItem;
      QTreeWidgetItem *item1 = new QTreeWidgetItem;

      startDate.sprintf("%s-01-01", yearArr[i].toLocal8Bit().constData());
      endDate.sprintf("%s-12-30", yearArr[i].toLocal8Bit().constData());

        if(true == getYahooTaMinMaxAvgPrice(assetSymbol, startDate, endDate, minPrice, maxPrice, avgPrice))
        {
            qDebug() << yearArr[i];
            qDebug() << minPrice;
            qDebug() << maxPrice;
            qDebug() << avgPrice;
            qDebug() << erningPerShare[i];

            item->setText(0, (QString)yearArr[i]);
            item1->setText(0, (QString)yearArr[i]);


            tmp.sprintf("%.1f", (minPrice));
            item->setText(1, tmp);

            tmp.sprintf("%.1f", (avgPrice));
            item->setText(2, tmp);

            tmp.sprintf("%.1f", (maxPrice));
            item->setText(3, tmp);


            if(erningPerShare[i] != 0)
            {
                tmp.sprintf("%.1f", (minPrice/erningPerShare[i]));
                item1->setText(1, tmp);
                qDebug() << tmp;

                tmp.sprintf("%.1f", (avgPrice/erningPerShare[i]));
                item1->setText(2, tmp);
                qDebug() << tmp;

                tmp.sprintf("%.1f", (maxPrice/erningPerShare[i]));
                item1->setText(3, tmp);
                qDebug() << tmp;

            }

            treeWidget->addTopLevelItem(item);
            treeWidget1->addTopLevelItem(item1);
        }
  }



  treeWidget->sortByColumn(0, Qt::DescendingOrder); // column/order to sort by
  treeWidget->setSortingEnabled(true);             // should cause sort on add

  treeWidget1->sortByColumn(0, Qt::DescendingOrder); // column/order to sort by
  treeWidget1->setSortingEnabled(true);             // should cause sort on add


    return true;
}
#endif














/****************************************************************
 *
 * Function:    addStockSymbolAndNameInTreeWidget1()
 *
 * Description: This combobox has a checkbox
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::addFa2operatingIncomeToTreeWidget(QTreeWidget *treeWidget,
                                                QString assetName)
{
    CUtil cu;
    QSqlRecord rec;
    QString str;
    QString tmp;

    double growth;
    double operatingIncomeDivNofShares;
    double operatingIncomeDiv1000;
    double refOperatingIncome;
    bool operatingCostRefIsInit = false;


    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);



    str.sprintf("SELECT assetName, year, numberOfShares, operatingIncome "
                " FROM TblNordnetBalanceIncomeSheet "
                " WHERE LOWER(assetName) = LOWER('%s') "
                " ORDER BY LOWER(year) ASC;",              // DESC
                assetName.toLocal8Bit().constData());

    qry.prepare(str);

    qDebug() << str;

    if( !qry.exec() )
    {
        treeWidget->clear();


        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("assetName").isNull()==true || (rec.value("operatingIncome").isNull()==true) ||
              (rec.value("year").isNull() ==true) || (rec.value("numberOfShares").isNull() ==true))
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {

                QTreeWidgetItem *item = new QTreeWidgetItem;

                item->setText(0, (QString)rec.value("year").toString().toUtf8());

                operatingIncomeDivNofShares = rec.value("operatingIncome").toDouble() / rec.value("numberOfShares").toDouble();
                tmp.sprintf("%.1f", (operatingIncomeDivNofShares*1000.0));
                item->setText(1, tmp);

                operatingIncomeDiv1000 = rec.value("operatingIncome").toDouble() / 1000.0;
                tmp.sprintf("%.0f", operatingIncomeDiv1000);
                if(operatingIncomeDiv1000 >= 0)
                    item->setTextColor(2, Qt::darkGreen);
                else
                    item->setTextColor(2, Qt::red);
                item->setText(2, tmp);



                if(operatingCostRefIsInit == true)
                {
                    if(refOperatingIncome > 0)
                    {
                        growth = (operatingIncomeDivNofShares - refOperatingIncome) / refOperatingIncome;
                    }
                    else
                    {
                        double tmp = refOperatingIncome;
                        tmp = -tmp;
                        growth = (operatingIncomeDivNofShares + refOperatingIncome) / tmp;
                    }

                    tmp.sprintf("%.2f", growth*100);
                    item->setText(3, tmp);
                    if(growth*100 >= 10)
                        item->setTextColor(3, Qt::darkGreen);
                    else if(growth*100 < 10 && growth*100 >= 0)
                        item->setTextColor(3, cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3));
                    else
                        item->setTextColor(3, Qt::red);
                }
                else
                {
                    item->setText(3, " ");
                }

                if(operatingCostRefIsInit == false)
                {
                    operatingCostRefIsInit = true;
                }

                refOperatingIncome = operatingIncomeDivNofShares;
                treeWidget->addTopLevelItem(item);
            }
        }
    }

    treeWidget->sortByColumn(0, Qt::DescendingOrder); // column/order to sort by
    treeWidget->setSortingEnabled(true);             // should cause sort on add

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}




/*****************************************************************
 *
 * Function:		getYahooTaPriceData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::getYahooTaMinMaxAvgPrice(QString assetSymbol,
                                  QString startDate,
                                  QString endDate,
                                  double &minPrice,
                                  double &maxPrice,
                                  double &avgPrice)
{

    QSqlRecord rec;
    QString str;
    CYahooStockPlotUtil ypu;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);



    QByteArray ba = assetSymbol.toLocal8Bit();
    const char *c_assetSymbol = ba.data();

    QByteArray ba1 = startDate.toLocal8Bit();
    const char *c_startDate = ba1.data();

    QByteArray ba2 = endDate.toLocal8Bit();
    const char *c_endDate = ba2.data();



    str.sprintf("SELECT TblYahooTaStockName.assetNameId, TblYahooTaStockName.assetName, TblYahooTaStockName.assetSymbol, "
                " TblYahooTaData.date, TblYahooTaData.adjClose, TblYahooTaData.assetNameId, "
                " MAX(TblYahooTaData.adjClose) AS maxPrice, MIN(TblYahooTaData.adjClose) AS minPrice, "
                " AVG(TblYahooTaData.adjClose) AS avgPrice"
                " FROM  TblYahooTaStockName, TblYahooTaData "
                " WHERE TblYahooTaStockName.assetNameId = TblYahooTaData.assetNameId AND "
                " TblYahooTaStockName.assetSymbol = '%s' AND "
                " TblYahooTaData.date >= '%s' AND TblYahooTaData.date <= '%s' "
                " ORDER BY TblYahooTaData.date ASC;",
                c_assetSymbol, c_startDate, c_endDate);

    qDebug() << "str =" << str << "\n";

    qry.prepare(str);

    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {


        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("assetSymbol").isNull() == true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                qDebug() << (QString) rec.value("adjClose").toString() << "\n";
                qDebug() << (QString) rec.value("assetName").toString() << "\n";
                qDebug() << (QString) rec.value("assetSymbol").toString() << "\n";
                qDebug() << (QString) rec.value("date").toString() << "\n";


                minPrice = rec.value("minPrice").toDouble();
                maxPrice = rec.value("maxPrice").toDouble();
                avgPrice = rec.value("avgPrice").toDouble();
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return true;
            }
        }
    }

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return false;
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
 *
 ****************************************************************/
bool CDbHndl::
insertNordnetBalanceAndIncomeData(NordnetBalanceIncomeSheet_ST data, bool dbIsHandledExternly)
{
    QString str;

    if(!(data.assetName.size() > 0 && data.assetGroup.size() > 0 && data.yearAssetName.size() > 0))
        return false;

    if(dbIsHandledExternly==false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);



    str.sprintf("INSERT INTO TblNordnetBalanceIncomeSheet "
                "(assetName, assetGroup, currentAssets, earningsPerShare, equity, fixedAssets, liabilities,"
                "netProfitAfterTax, numberOfShares, operatingExpensesAndDepreciation, operatingIncome,"
                "operatingIncomeAfterDepreciation, totalAssets, totalEquityAndLiabilities, year, yearAssetName) "
                " VALUES('%s', '%s', %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %d, '%s');",
                data.assetName.toLocal8Bit().constData(),
                data.assetGroup.toLocal8Bit().constData(),
                data.currentAssets,
                data.earningsPerShare,
                data.equity,
                data.fixedAssets,
                data.liabilities,
                data.netProfitAfterTax,
                data.numberOfShares,
                data.operatingExpensesAndDepreciation,
                data.operatingIncome,
                data.operatingIncomeAfterDepreciation,
                data.totalAssets,
                data.totalEquityAndLiabilities,
                data.year,
                data.yearAssetName.toLocal8Bit().constData());



    qDebug() << str << "\n";
    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error"), qry.lastError().text().toLatin1().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }

    return true;
}


//=================
/****************************************************************
 *
 * Function:    addStockSymbolAndNameInTreeWidget()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::getAllMainHelpSectionNames(QComboBox *comboBox)
{
    QSqlRecord rec;
    QString str;



    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    str.sprintf("SELECT MainHelpSectionName "
                " FROM TblMainHelpSection"
                " ORDER BY LOWER(MainHelpSectionName) ASC;");


    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        //index = comboBox->currentIndex();
        comboBox->clear();
        qDebug( ) << "Clear stocklist combobox";
        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("MainHelpSectionName").isNull()==true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                str = rec.value("MainHelpSectionName").toString().toLatin1();
                qDebug( ) << str;
                comboBox->addItem(str);
                qDebug( ) << rec.value("MainHelpSectionName").toString().toLatin1();
            }
        }
    }

    comboBox->setCurrentIndex(0);
    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}


/****************************************************************
 *
 * Function:    addStockSymbolAndNameInTreeWidget()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::getAllSubHelpSectionNames(QComboBox *comboBox, int mainHelpSectionId)
{
    QSqlRecord rec;
    QString str;
    int index;


    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    str.sprintf("SELECT SubHelpSectionName "
                " FROM TblSubHelpSection"
                " WHERE MainHelpSectionId = %d"
                " ORDER BY LOWER(SubHelpSectionName) ASC;",
                mainHelpSectionId);


    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        index = comboBox->currentIndex();
        comboBox->clear();
        qDebug( ) << "Clear stocklist combobox";
        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("SubHelpSectionName").isNull()==true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                str = (QString)rec.value("SubHelpSectionName").toString();
                comboBox->addItem(str);
                qDebug( ) << rec.value("SubHelpSectionName").toString();
            }
        }
    }

    comboBox->setCurrentIndex(index);
    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}




/*****************************************************************
 *
 * Function:		delAllTblAvanzaAccount()
 *
 * Description:		This function delete all data
 *
 *
 *
 *****************************************************************/
bool CDbHndl::delAllTblMainHelpSection(void)
{
    QString str;
    // QSqlQuery qry;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    // Create sql question
    str.sprintf("DELETE "
                "FROM TblMainHelpSection;");

    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error delete TblMainHelpSection"), qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}


/*****************************************************************
 *
 * Function:		delAllTblAvanzaAssetName()
 *
 * Description:		This function delete all data
 *
 *
 *
 *****************************************************************/
bool CDbHndl::delAllTblSubHelpSection(void)
{
    QString str;
    // QSqlQuery qry;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    // Create sql question
    str.sprintf("DELETE "
                "FROM TblSubHelpSection;");

    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 1b"), qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}





/*****************************************************************
 *
 * Function:		delAllTblAvanzaAssetData()
 *
 * Description:		This function delete all data
 *
 *
 *
 *****************************************************************/
bool CDbHndl::delAllTblHelpSectionTxt(void)
{
    QString str;


    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    // Create sql question
    str.sprintf("DELETE "
                "FROM TblHelpSectionTxt;");

    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error delete TblHelpSectionTxt"), qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}

/****************************************************************
 *
 * Function:    updateData()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::deleteMainHelpSectionName(QString mainHelpSectionName)
{
    QString str;
    int mainHelpSectionId;


    if(false == findMainHelpSectionName(mainHelpSectionName, mainHelpSectionId))
    {
        return false;
    }

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);



    str.sprintf("DELETE FROM TblMainHelpSection "
                "WHERE MainHelpSectionId = %d;",
                 mainHelpSectionId);


    qry.prepare(str);


    if( !qry.exec() )
    {
        qry.finish();
        closeDb();
        m_mutex.unlock();

        qDebug() << qry.lastError();
        return false;
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();

    return true;
}






/****************************************************************
 *
 * Function:    findMainHelpSection()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
findMainHelpSectionInTblHelpSectionTxt(QString  mainHelpSectionName,
                                       int &mainHelpSectionId)
{
    QSqlRecord rec;
    QString str;

    if(true == findMainHelpSectionName(mainHelpSectionName, mainHelpSectionId))
    {


        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
        QSqlQuery qry(m_db);

        qDebug() << mainHelpSectionName;



        str.sprintf("SELECT * "
                    "FROM TblHelpSectionTxt "
                    " WHERE MainHelpSectionId = %d;",
                    mainHelpSectionId);


        qDebug() << str;

        qry.prepare(str);


        if( !qry.exec() )
        {
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
            }
            qDebug() << qry.lastError();
            closeDb();
            m_mutex.unlock();
            return false;
        }
        else
        {
            if(qry.next())
            {
                rec = qry.record();

                if(rec.value("MainHelpSectionId").isNull()==true)
                {
                    qry.finish();
                    closeDb();
                    m_mutex.unlock();
                    return false;
                }
                else
                {
                    if(mainHelpSectionName.compare((QString)rec.value("MainHelpSectionName").toString()) == 0)
                    {
                        mainHelpSectionId = rec.value("MainHelpSectionId").toInt();
                        qry.finish();
                        closeDb();
                        m_mutex.unlock();
                        return true;
                    }
                }
            }
        }

        qry.finish();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    return false;
}



/****************************************************************
 *
 * Function:    findMainHelpSection()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::findMainHelpSectionName(QString  mainHelpSectionName, int &mainHelpSectionId)
{
    QSqlRecord rec;
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);

    qDebug() << mainHelpSectionName;

    QByteArray ba = mainHelpSectionName.toLocal8Bit();
    const char *c_mainHelpSectionName = ba.data();


    str.sprintf("SELECT * "
                "FROM TblMainHelpSection "
                " WHERE MainHelpSectionName = '%s';",
                c_mainHelpSectionName);

    qDebug() << str;

    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        if(qry.next())
        {
            rec = qry.record();

            if(rec.value("MainHelpSectionName").isNull()==true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                if(mainHelpSectionName.compare((QString)rec.value("MainHelpSectionName").toString()) == 0)
                {
                    mainHelpSectionId = rec.value("MainHelpSectionId").toInt();
                    qry.finish();
                    closeDb();
                    m_mutex.unlock();
                    return true;
                }
            }
        }
    }

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return false;
}


/****************************************************************
 *
 * Function:    updateData()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::deleteSubHelpSectionName(QString subHelpSectionName)
{
    QString str;
    int subHelpSectionId;


    if(false == findSubHelpSectionName(subHelpSectionName, subHelpSectionId))
    {
        return false;
    }

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);



    str.sprintf("DELETE FROM TblSubHelpSection "
                "WHERE SubHelpSectionId = %d;",
                 subHelpSectionId);


    qry.prepare(str);


    if( !qry.exec() )
    {
        qry.finish();
        closeDb();
        m_mutex.unlock();

        qDebug() << qry.lastError();
        return false;
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();

    return true;
}



/****************************************************************
 *
 * Function:    updateData()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::deleteSubHelpSectionTxt(QString mainHelpSectionName,
                                      QString subHelpSectionName)
{
    int mainHelpSectionId;
    int subHelpSectionId;
    QString str;

    if(false == findMainHelpSectionName(mainHelpSectionName, mainHelpSectionId) ||
       false == findSubHelpSectionName(subHelpSectionName, subHelpSectionId))
    {
        return false;
    }


    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);

    str.sprintf("DELETE FROM TblHelpSectionTxt "
                "WHERE MainHelpSectionId = %d AND SubHelpSectionId = %d;",
                 mainHelpSectionId,
                 subHelpSectionId);


    qry.prepare(str);


    if( !qry.exec() )
    {
        qry.finish();
        closeDb();
        m_mutex.unlock();

        qDebug() << qry.lastError();
        return false;
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();

    return true;
}




/****************************************************************
 *
 * Function:    updateOneRowYieldData()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
modifySubHelpSectionName(QString oldSubHelpSectionName,
                         QString newSubHelpSectionName,
                         bool dbIsHandledExternly)
{

    QString str;

    int subHelpSectionId;


    if(false == findSubHelpSectionName(oldSubHelpSectionName, subHelpSectionId))
    {
        return false;
    }

    QByteArray ba1 = newSubHelpSectionName.toLocal8Bit();
    const char *c_newSubHelpSectionName = ba1.data();



    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }
    QSqlQuery qry(m_db);

    str.sprintf("UPDATE TblSubHelpSection "
                " SET SubHelpSectionName = '%s' "
                " WHERE  SubHelpSectionId = %d;",
                c_newSubHelpSectionName,
                subHelpSectionId);

    qDebug() << str;

    qry.prepare(str);

    if( !qry.exec() )
    {
        qDebug() << qry.lastError();
        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }



    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }
    return true;
}




/****************************************************************
 *
 * Function:    updateOneRowYieldData()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
modifyHelpSectionTxt(QString mainHelpSectionName,
                     QString subHelpSectionName,
                     QString helpSectionTxt,
                     bool dbIsHandledExternly)
{
    int mainHelpSectionId;
    int subHelpSectionId;
    QString str;


    QByteArray ba1 = helpSectionTxt.toLocal8Bit();
    const char *c_helpSectionTxt = ba1.data();

    qDebug() << "mainHelpSectionName =" << mainHelpSectionName;
    qDebug() << "subHelpSectionName =" << subHelpSectionName;
    qDebug() << "helpSectionTxt" << helpSectionTxt << "\n";


    if(findMainHelpSectionName(mainHelpSectionName, mainHelpSectionId) == false)
    {
        return false;
    }


    if(false == findSubHelpSectionName(subHelpSectionName, subHelpSectionId))
    {
        return false;
    }


    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }
    QSqlQuery qry(m_db);



    str.sprintf("UPDATE TblHelpSectionTxt "
                " SET HelpSectionTxt = '%s' "
                " WHERE  MainHelpSectionId = %d AND SubHelpSectionId = %d;",
                c_helpSectionTxt,
                mainHelpSectionId,
                subHelpSectionId
);

    qDebug() << str;

    qry.prepare(str);

    if( !qry.exec() )
    {
        qDebug() << qry.lastError();
        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }

    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }
    return true;
}





/****************************************************************
 *
 * Function:    findSubHelpSectionName()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::findSubHelpSectionName(QString subHelpSectionName, int &subHelpSectionId)
{
    QSqlRecord rec;
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);

    qDebug() << "subHelpSectionName =" << subHelpSectionName << "\n";
    qDebug() << "subHelpSectionId =" << subHelpSectionId << "\n";


    QByteArray ba = subHelpSectionName.toLocal8Bit();
    const char *c_subHelpSectionName = ba.data();


    str.sprintf("SELECT TblSubHelpSection.SubHelpSectionName, TblSubHelpSection.SubHelpSectionId "
                " FROM  TblSubHelpSection "
                " WHERE TblSubHelpSection.SubHelpSectionName = '%s';",
                c_subHelpSectionName);

    qDebug() << "str =" << str << "\n";


    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        if(qry.next())
        {
            rec = qry.record();

            qDebug() << (QString) rec.value("SubHelpSectionId").toString() << "\n";
            qDebug() << (QString) rec.value("SubHelpSectionName").toString() << "\n";


            if(rec.value("SubHelpSectionId").isNull() == true || rec.value("SubHelpSectionName").isNull())
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                subHelpSectionId = rec.value("SubHelpSectionId").toInt();
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return true;
            }
        }
    }

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return false;
}


#if 0
/****************************************************************
 *
 * Function:    getTotNofAvanzaPortoflioRecords()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::getTotNofAvanzaPortfolioRecords(int &nofRow)
{
    QString str;

    nofRow = 0;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);
    str.sprintf("SELECT Count(*) FROM TblAvanzaAssetData;");

    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 8a"), (QString) qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }


    if (qry.next())
    {
        nofRow = qry.value(0).toInt();
    }
    else
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 9a"), QString::fromUtf8("No data received"));
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }

    qDebug("Inserted!");
    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;

}




/****************************************************************
 *
 * Function:    findAvanzaAccount()
 *
 * Description: Check if account exist.
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::listAllAvanzaPortfolioRecords(QObject *thisPointer, int rows, int cols, QTableView *tableView)
{
    CExtendedTable et;
    QSqlRecord rec;
    QString str;
    int colCnt;
    int rowCnt;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    // AS assetName1,
    str.sprintf("SELECT TblAvanzaAccount.account, "
                "TblAvanzaAssetName.assetName AS assetName1, "
                "TblAvanzaAssetName.accountId, "
                "TblAvanzaAssetData.date, "
                "TblAvanzaAssetData.typeOfData, "
                "TblAvanzaAssetData.assetPrice, "
                "TblAvanzaAssetData.currency, "
                "TblAvanzaAssetData.assetName "
                " FROM TblAvanzaAccount, TblAvanzaAssetName, TblAvanzaAssetData "
                " WHERE TblAvanzaAccount.account = TblAvanzaAssetName.accountId AND "
                "TblAvanzaAssetName.assetName = TblAvanzaAssetData.assetName "
                " ORDER BY TblAvanzaAccount.account, TblAvanzaAssetData.date;");


    qDebug() << str << "\n";
    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), (QString)qry.lastError().text().toLocal8Bit().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {

        et.createTableModel(rows, cols, thisPointer);
        colCnt = 0;
        rowCnt = 0;

        while(qry.next())
        {
            rec = qry.record();
            if(rec.value("account").isNull() == true)
            {
                if(m_disableMsgBoxes == false)
                {
                    QMessageBox::critical(NULL, QString::fromUtf8("Aktie list"), QString::fromUtf8("Aktie listan saknas"));
                }
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                str = rec.value("account").toString();
                et.addData(tableView, str, rowCnt, colCnt);
                colCnt++;

                str = rec.value("assetName1").toString();
                et.addData(tableView, str, rowCnt, colCnt);
                colCnt++;

                str = rec.value("date").toString();
                et.addData(tableView, str, rowCnt, colCnt);
                colCnt++;

                str = rec.value("typeOfData").toString();
                et.addData(tableView, str, rowCnt, colCnt);
                colCnt++;

                str = rec.value("assetPrice").toString();
                et.addData(tableView, str, rowCnt, colCnt);
                colCnt++;

                str = rec.value("currency").toString();
                et.addData(tableView, str, rowCnt, colCnt);
                colCnt = 0;
                rowCnt++;

            }
        }
    }

    tableView->resizeColumnsToContents();

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}

#endif



/****************************************************************
 *
 * Function:    findHelpSectionTxt()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
getHelpSectionTxt(int mainHelpSectionId, int subHelpSectionId, QString &helpSectionTxt)
{
    QSqlRecord rec;
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);



    str.sprintf("SELECT TblMainHelpSection.MainHelpSectionId, TblSubHelpSection.SubHelpSectionId, TblHelpSectionTxt.MainHelpSectionId,  "
                " TblHelpSectionTxt.SubHelpSectionId, TblHelpSectionTxt.HelpSectionTxt  "
                " FROM TblMainHelpSection, TblSubHelpSection, TblHelpSectionTxt "
                "WHERE TblHelpSectionTxt.MainHelpSectionId = TblMainHelpSection.MainHelpSectionId AND "
                " TblHelpSectionTxt.SubHelpSectionId = TblSubHelpSection.SubHelpSectionId AND "
                "TblMainHelpSection.MainHelpSectionId = %d AND "
                "TblSubHelpSection.SubHelpSectionId = %d;",  mainHelpSectionId, subHelpSectionId);


    qDebug() << str << "\n";

    qry.prepare(str);

    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), (QString) qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        if(qry.next())
        {
            rec = qry.record();
            if(rec.value("HelpSectionTxt").isNull()==true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                helpSectionTxt = rec.value("HelpSectionTxt").toString();
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return true;
            }
        }
    }

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return false;
}




/****************************************************************
 *
 * Function:    insertAvanzaAssetData()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertHelpSectionTxt(QString mainHelpSectionName, QString subHelpSectionName, QString helpSectionTxt)
{
    int mainHelpSectionId;
    int subHelpSectionId;



    qDebug() << "mainHelpSectionName =" << mainHelpSectionName;
    qDebug() << "subHelpSectionName =" << subHelpSectionName;
    qDebug() << "HelpSectionTxt" << helpSectionTxt << "\n";

    if(findMainHelpSectionName(mainHelpSectionName, mainHelpSectionId) == true &&
       findSubHelpSectionName(subHelpSectionName, subHelpSectionId) == true)
    {
        if (QMessageBox::No == QMessageBox::question(NULL,
                                                     QString::fromUtf8("Text already exist"),
                                                     QString::fromUtf8("Text already exist update?"),
                                                     QMessageBox::Yes | QMessageBox::No))
        {
            return true;
        }

        if(false == modifyHelpSectionTxt(mainHelpSectionName, subHelpSectionName, helpSectionTxt))
        {
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("insertHelpSectionTxt"), QString::fromUtf8("Fail to update insertHelpSectionTxt"));
            }
            return false;
        }
        return true;
    }


    if(findMainHelpSectionName(mainHelpSectionName, mainHelpSectionId) == false)
    {
        if(false == insertMainHelpSection(mainHelpSectionName))
        {
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("insertHelpSectionTxt"), QString::fromUtf8("Fail to insert MainSection"));
            }
            return false;
        }

        // Get mainHelpSectionId
        if(findMainHelpSectionName(mainHelpSectionName, mainHelpSectionId) == false)
        {
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("insertHelpSectionTxt"), QString::fromUtf8("Fail to find main section"));
            }
            return false;
        }
    }



    if(false == findSubHelpSectionName(subHelpSectionName, subHelpSectionId))
    {

        if(false == insertSubHelpSection(subHelpSectionName, mainHelpSectionId))
        {
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("insertHelpSectionTxt"), QString::fromUtf8("Fail to insert sub section name"));
            }
            return false;
        }

        if(false == findSubHelpSectionName(subHelpSectionName, subHelpSectionId))
        {
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("insertHelpSectionTxt"), QString::fromUtf8("Fail to find subSectionName"));
            }
            return false;
        }
        else
        {
            QString str;

            m_mutex.lock();
            openDb(PATH_JACK_STOCK_DB);
            QSqlQuery qry(m_db);

            QByteArray ba = helpSectionTxt.toLocal8Bit();
            const char *c_helpSectionTxt = ba.data();

            str.sprintf("INSERT INTO TblHelpSectionTxt "
                        "(MainHelpSectionId, SubHelpSectionId, HelpSectionTxt) "
                        " VALUES(%d, %d, '%s');",
                        mainHelpSectionId,
                        subHelpSectionId,
                        c_helpSectionTxt);


            qry.prepare(str);


            if(!qry.exec())
            {
                qDebug() << qry.lastError();
                if(m_disableMsgBoxes == false)
                {
                    QMessageBox::critical(NULL, QString::fromUtf8("Database Error HelpSectionTxt"), qry.lastError().text().toLatin1().constData());
                }
                closeDb();
                m_mutex.unlock();
                return false;
            }


            qry.finish();
            closeDb();
            m_mutex.unlock();
            return true;
        }
    }

    return false;

}



#if 0
/****************************************************************
 *
 * Function:    insertAvanzaAssetData()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertAvanzaAssetData(QString date,
                      QString typeOfData,
                      int     typeOfDataId,
                      double assetPrice,
                      QString currency,
                      QString assetName)
{
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);

    QByteArray ba = assetName.toLocal8Bit();
    const char *c_assetName = ba.data();

    QByteArray ba1 = currency.toLocal8Bit();
    const char *c_currency = ba1.data();




    str.sprintf("INSERT INTO TblAvanzaAssetData "
                "(date, typeOfData, typeOfDataId, assetPrice, currency, assetName) "
                " VALUES('%s', '%s', %d, %g, '%s', '%s');",
                date.toLocal8Bit().constData(),
                typeOfData.toLocal8Bit().constData(),
                typeOfDataId,
                assetPrice,
                c_currency,
                c_assetName);


    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 20"), qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;

}
#endif


/****************************************************************
 *
 * Function:    insertAvanzaAssetData()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubHelpSection(QString subHelpSectionName, int mainHelpSectionId)
{
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);

    QByteArray ba = subHelpSectionName.toLocal8Bit();
    const char *c_subHelpSectionName = ba.data();


    str.sprintf("INSERT INTO TblSubHelpSection "
                "(SubHelpSectionName, MainHelpSectionId) "
                " VALUES('%s', %d);", c_subHelpSectionName, mainHelpSectionId);

    qDebug() << str << "\n";

    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 20"), qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}



/****************************************************************
 *
 * Function:    insertAvanzaAssetData()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertMainHelpSection(QString mainHelpSectionName)
{
    QString str;

    if(mainHelpSectionName.size() < 1)
        return false;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);




    str.sprintf("INSERT INTO TblMainHelpSection "
                "(MainHelpSectionName) "
                " VALUES('%s');", mainHelpSectionName.toLocal8Bit().constData());


    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 20"), qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;

}


//======================




/*****************************************************************
 *
 * Function:		delAllTblNordnetBalanceIncomeSheet()
 *
 * Description:		This function delete all data
 *
 *
 *
 *****************************************************************/
bool CDbHndl::delAllTblNordnetBalanceIncomeSheet(void)
{
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    // Create sql question
    str.sprintf("DELETE "
                "FROM TblNordnetBalanceIncomeSheet;");

    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 1aa2"), qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}





/****************************************************************
 *
 * Function:    getListWithNordnetStockId()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::getListWithNordnetStockId(QList <CompanyNameAndId_ST> &companyList,
                                        bool dbIsHandledExternly)
{
    CompanyNameAndId_ST data;
    bool found = false;
    QString str;
    QSqlRecord rec;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }
    QSqlQuery qry(m_db);


    str.sprintf("SELECT * "
                " FROM TblNordnetStockIds;");


    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 26"), qry.lastError().text().toLatin1().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    while(qry.next())
    {
        rec = qry.record();

        qDebug() << rec.value("assetId").toString() << "\n";
        qDebug() << rec.value("assetName").toString() << "\n";



        if(rec.value("assetId").isNull()==true)
        {
            qry.finish();
            if(dbIsHandledExternly == false)
            {
                closeDb();
                m_mutex.unlock();
            }
            return false;
        }
        else
        {
            found = true;
            data.assetId = rec.value("assetId").toString();
            data.AssetName = rec.value("assetName").toString();
            companyList.append(data);

        }
    }


    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }

    if(found == true)
        return true;
    else
        return false;
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
 *
 ****************************************************************/
bool CDbHndl::
insertNordnetStockIdData(QString assetId,
                         QString assetName,
                         bool dbIsHandledExternly)
{
    QString str;



    if(dbIsHandledExternly==false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);



    str.sprintf("INSERT INTO TblNordnetStockIds "
                "(assetName, assetId) "
                " VALUES('%s', '%s');",
                assetName.toLocal8Bit().constData(),
                assetId.toLocal8Bit().constData());



    qDebug() << str << "\n";
    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error"), qry.lastError().text().toLatin1().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }

    return true;
}



/*****************************************************************
 *
 * Function:		delAllTblYahooTaData()
 *
 * Description:		This function delete all data
 *
 *
 *
 *****************************************************************/
bool CDbHndl::delAllTblNordnetStockIds(void)
{
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    // Create sql question
    str.sprintf("DELETE "
                "FROM TblNordnetStockIds;");

    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 1aa2"), qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}


/*****************************************************************
 *
 * Function:		getAllSnapshotPlotStocksData()
 *
 * Description:		This function extract all stocks from the database
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getAllSnapshotPlotStocksData(QString stockListName,
                             QVector <CStockPlotUtil::StockData_ST> &stockArr)
{
    QSqlRecord rec;
    QString str;
    bool found;
    bool found1;
    CStockPlotUtil::StockData_ST data;
    int i;
    int j;


    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);

    // Remove space
    stockListName.trimmed();

    QByteArray ba = stockListName.toLocal8Bit();
    const char *c_stockListName = ba.data();




    str.sprintf("SELECT TblTaStockList.taStockListId, TblTaStockList.stockListName, "
                    " TblStockDataSnapshot.*, "
                    " TblTaStockName.taStockListId, TblTaStockName.stockSymbol, TblTaStockName.stockName, "
                    " TblNordnetYahooBridge.assetSymbol, TblNordnetYahooBridge.assetName "
                    " FROM TblTaStockList, TblTaStockName, TblNordnetYahooBridge, TblStockDataSnapshot "
                    " WHERE TblTaStockList.taStockListId = TblTaStockName.taStockListId AND "
                           "lower(TblStockDataSnapshot.companyName) = lower(TblNordnetYahooBridge.assetName) AND "
                           "TblTaStockName.stockSymbol = TblNordnetYahooBridge.assetSymbol AND "
                           "lower(TblTaStockList.stockListName) = lower('%s');", c_stockListName);


    qDebug() << str;




    qry.prepare(str);

    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
    }
    else
    {

        i = 0;
        j = 0;

        while(qry.next())
        {
            found = false;
            found1 = false;
            rec = qry.record();

            if(rec.value("companyName").isNull() == true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                found1 = true;
                data.stockName = rec.value("companyName").toString();
                qDebug() << rec.value("companyName").toString();
                qDebug() << rec.value("taStockListId").toString();
            }


            if(rec.value("stockSymbol").isNull() == true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                found1 = true;
                data.stockSymbol = rec.value("stockSymbol").toString();
                qDebug() << rec.value("stockSymbol").toString();
            }


            if(rec.value("procentChangeOneDay").isNull() != true)
            {
                found = true;
                data.data.y.insert(j, rec.value("procentChangeOneDay").toDouble());
                data.data.x.insert(j, 0); // (0) (time)
                j++;
            }



            if(rec.value("procentChangeOneWeek").isNull() != true)
            {
                found = true;
                data.data.y.insert(j, rec.value("procentChangeOneWeek").toDouble());
                data.data.x.insert(j, 7); // (0) (time)
                j++;
            }

            if(rec.value("procentChange1Month").isNull() != true)
            {
                found = true;
                data.data.y.insert(j, rec.value("procentChange1Month").toDouble());
                data.data.x.insert(j, 30); // (0) (time)
                j++;
            }


            if(rec.value("procentChange3Month").isNull() != true)
            {
                found = true;
                data.data.y.insert(j, rec.value("procentChange3Month").toDouble());
                data.data.x.insert(j, 90); // (0) (time)
                j++;
            }

            if(rec.value("procentChange6Month").isNull() != true)
            {
                found = true;
                data.data.y.insert(j, rec.value("procentChange6Month").toDouble());
                data.data.x.insert(j, 180); // (0) (time)
                j++;
            }


            if(rec.value("procentChange1Year").isNull() != true)
            {
                found = true;
                data.data.y.insert(j, rec.value("procentChange1Year").toDouble());
                data.data.x.insert(j, 365); // (0) (time)
                j++;
            }


            if(rec.value("procentChange2Year").isNull() != true)
            {
                found = true;
                data.data.y.insert(j, rec.value("procentChange2Year").toDouble());
                data.data.x.insert(j, 730); // (0) (time)
                j++;
            }


            if(rec.value("procentChange3Year").isNull() != true)
            {
                found = true;
                data.data.y.insert(j, rec.value("procentChange3Year").toDouble());
                data.data.x.insert(j, 1095); // (0) (time)
                j++;
            }

            if(rec.value("procentChange5Year").isNull() != true)
            {
                found = true;
                data.data.y.insert(j, rec.value("procentChange5Year").toDouble());
                data.data.x.insert(j, 1825); // (0) (time)
                j++;
            }

            // reset stock data index
            if(found == true && found1 == true)
            {
                found = false;
                found1 = false;
                stockArr.insert(i, data);
            }
            j = 0;
            i++;
        }
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}








/****************************************************************
 *
 * Function:    ()
 *
 * Description: 160317
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::assetSymbolGetKeyDataUseBridge(QString snapshotAssetSymbol,
                                             snapshotStockData_ST &keyData,
                                              bool dbIsHandledExternly)
{
    QSqlRecord rec;
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

    // Remove space
    snapshotAssetSymbol.trimmed();

    QByteArray ba = snapshotAssetSymbol.toLocal8Bit();
    const char *c_snapshotAssetSymbol = ba.data();


    str.sprintf("SELECT TblNordnetYahooBridge.*, TblStockDataSnapshot.* "
                " FROM TblNordnetYahooBridge, TblStockDataSnapshot "
                " WHERE lower(TblStockDataSnapshot.companyName) = lower(TblNordnetYahooBridge.assetName) AND "
                " lower('%s') = lower(TblNordnetYahooBridge.assetSymbol);", c_snapshotAssetSymbol);


    qDebug() << str << "\n";

    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }
    else
    {
        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("companyName").isNull()==true)
            {
                qry.finish();
                if(dbIsHandledExternly == false)
                {
                    closeDb();
                    m_mutex.unlock();
                }
                return false;
            }
            else
            {
                qDebug() << rec.value("companyName").toString() << "\n";
                keyData.companyName = rec.value("companyName").toString();
                keyData.lastPrice = rec.value("lastPrice").toString();
                keyData.priceChange = rec.value("priceChange").toString();
                keyData.procentChangeOneDay = rec.value("procentChangeOneDay").toString();
                keyData.bidPrice = rec.value("bidPrice").toString();
                keyData.offerPrice = rec.value("offerPrice").toString();
                keyData.highestPrice = rec.value("highestPrice").toString();
                keyData.lowestPrice = rec.value("lowestPrice").toString();
                keyData.volume = rec.value("volume").toString();
                keyData.currency = rec.value("currency").toString();
                keyData.time = rec.value("time").toString();
                keyData.date = rec.value("date").toString();
                keyData.procentChangeOneWeek = rec.value("procentChangeOneWeek").toString();
                keyData.procentChange1Month = rec.value("procentChange1Month").toString();
                keyData.procentChange3Month = rec.value("procentChange3Month").toString();
                keyData.procentChange6Month = rec.value("procentChange6Month").toString();
                keyData.procentChange1Year = rec.value("procentChange1Year").toString();
                keyData.procentChange2Year = rec.value("procentChange2Year").toString();
                keyData.procentChange3Year = rec.value("procentChange3Year").toString();
                keyData.procentChange5Year = rec.value("procentChange5Year").toString();
                keyData.keyValuePE = rec.value("keyValuePE").toString();
                keyData.keyValuePS = rec.value("keyValuePS").toString();
                keyData.keyValueEarningsPerShare = rec.value("keyValueEarningsPerShare").toString();
                keyData.keyValueNAVPerShare = rec.value("keyValueNAVPerShare").toString();
                keyData.keyValueDividendPerShare = rec.value("keyValueDividendPerShare").toString();
                keyData.keyValueYield = rec.value("keyValueYield").toString();
                keyData.keyValueCoursePerJEK = rec.value("keyValueCoursePerJEK").toString();
                keyData.earningsDividedByDividend = rec.value("earningsDividedByDividend").toString();
                keyData.navDivLastStockPrice = rec.value("navDivLastStockPrice").toString();
                keyData.assetSymbol = rec.value("assetSymbol").toString();

                qry.finish();
                if(dbIsHandledExternly == false)
                {
                    closeDb();
                    m_mutex.unlock();
                }
                return true;

            }

        }
    }

    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }
    return false;
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
 *
 ****************************************************************/
bool CDbHndl::companynameGetKeyDataUseBridge(QString snapshotCompanyName,
                                             snapshotStockData_ST &keyData,
                                              bool dbIsHandledExternly)
{
    QSqlRecord rec;
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

    // Remove space
    snapshotCompanyName.trimmed();

    QByteArray ba = snapshotCompanyName.toLocal8Bit();
    const char *c_snapshotCompanyName = ba.data();



    str.sprintf("SELECT TblNordnetYahooBridge.*, TblStockDataSnapshot.* "
                " FROM TblNordnetYahooBridge, TblStockDataSnapshot "
                " WHERE lower(TblStockDataSnapshot.companyName) = lower(TblNordnetYahooBridge.assetName) AND "
                " lower('%s') = lower(TblNordnetYahooBridge.assetName);", c_snapshotCompanyName);


    qDebug() << str << "\n";

    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }
    else
    {
        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("companyName").isNull()==true)
            {
                qry.finish();
                if(dbIsHandledExternly == false)
                {
                    closeDb();
                    m_mutex.unlock();
                }
                return false;
            }
            else
            {
                qDebug() << rec.value("companyName").toString() << "\n";
                keyData.companyName = rec.value("companyName").toString();
                keyData.lastPrice = rec.value("lastPrice").toString();
                keyData.priceChange = rec.value("priceChange").toString();
                keyData.procentChangeOneDay = rec.value("procentChangeOneDay").toString();
                keyData.bidPrice = rec.value("bidPrice").toString();
                keyData.offerPrice = rec.value("offerPrice").toString();
                keyData.highestPrice = rec.value("highestPrice").toString();
                keyData.lowestPrice = rec.value("lowestPrice").toString();
                keyData.volume = rec.value("volume").toString();
                keyData.currency = rec.value("currency").toString();
                keyData.time = rec.value("time").toString();
                keyData.date = rec.value("date").toString();
                keyData.procentChangeOneWeek = rec.value("procentChangeOneWeek").toString();
                keyData.procentChange1Month = rec.value("procentChange1Month").toString();
                keyData.procentChange3Month = rec.value("procentChange3Month").toString();
                keyData.procentChange6Month = rec.value("procentChange6Month").toString();
                keyData.procentChange1Year = rec.value("procentChange1Year").toString();
                keyData.procentChange2Year = rec.value("procentChange2Year").toString();
                keyData.procentChange3Year = rec.value("procentChange3Year").toString();
                keyData.procentChange5Year = rec.value("procentChange5Year").toString();
                keyData.keyValuePE = rec.value("keyValuePE").toString();
                keyData.keyValuePS = rec.value("keyValuePS").toString();
                keyData.keyValueEarningsPerShare = rec.value("keyValueEarningsPerShare").toString();
                keyData.keyValueNAVPerShare = rec.value("keyValueNAVPerShare").toString();
                keyData.keyValueDividendPerShare = rec.value("keyValueDividendPerShare").toString();
                keyData.keyValueYield = rec.value("keyValueYield").toString();
                keyData.keyValueCoursePerJEK = rec.value("keyValueCoursePerJEK").toString();
                keyData.earningsDividedByDividend = rec.value("earningsDividedByDividend").toString();
                keyData.navDivLastStockPrice = rec.value("navDivLastStockPrice").toString();
                keyData.assetSymbol = rec.value("assetSymbol").toString();

                qry.finish();
                if(dbIsHandledExternly == false)
                {
                    closeDb();
                    m_mutex.unlock();
                }
                return true;

            }

        }
    }

    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }
    return false;
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
 *
 ****************************************************************/
bool CDbHndl::getKeyDataUseBridge(QString assetSymbol,
                                  snapshotStockData_ST &keyData,
                                  bool dbIsHandledExternly)
{
    QSqlRecord rec;
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

    QByteArray ba = assetSymbol.toLocal8Bit();
    const char *c_assetSymbol = ba.data();



    str.sprintf("SELECT TblNordnetYahooBridge.*, TblStockDataSnapshot.* "
                " FROM TblNordnetYahooBridge, TblStockDataSnapshot "
                " WHERE lower(TblStockDataSnapshot.companyName) = lower(TblNordnetYahooBridge.assetName) AND "
                " lower('%s') = lower(TblNordnetYahooBridge.assetSymbol);", c_assetSymbol);


    qDebug() << str << "\n";

    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }
    else
    {
        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("companyName").isNull()==true)
            {
                qry.finish();
                if(dbIsHandledExternly == false)
                {
                    closeDb();
                    m_mutex.unlock();
                }
                return false;
            }
            else
            {
                qDebug() << rec.value("companyName").toString() << "\n";
                keyData.companyName = rec.value("companyName").toString();
                keyData.lastPrice = rec.value("lastPrice").toString();
                keyData.priceChange = rec.value("priceChange").toString();
                keyData.procentChangeOneDay = rec.value("procentChangeOneDay").toString();
                keyData.bidPrice = rec.value("bidPrice").toString();
                keyData.offerPrice = rec.value("offerPrice").toString();
                keyData.highestPrice = rec.value("highestPrice").toString();
                keyData.lowestPrice = rec.value("lowestPrice").toString();
                keyData.volume = rec.value("volume").toString();
                keyData.currency = rec.value("currency").toString();
                keyData.time = rec.value("time").toString();
                keyData.date = rec.value("date").toString();
                keyData.procentChangeOneWeek = rec.value("procentChangeOneWeek").toString();
                keyData.procentChange1Month = rec.value("procentChange1Month").toString();
                keyData.procentChange3Month = rec.value("procentChange3Month").toString();
                keyData.procentChange6Month = rec.value("procentChange6Month").toString();
                keyData.procentChange1Year = rec.value("procentChange1Year").toString();
                keyData.procentChange2Year = rec.value("procentChange2Year").toString();
                keyData.procentChange3Year = rec.value("procentChange3Year").toString();
                keyData.procentChange5Year = rec.value("procentChange5Year").toString();
                keyData.keyValuePE = rec.value("keyValuePE").toString();
                keyData.keyValuePS = rec.value("keyValuePS").toString();
                keyData.keyValueEarningsPerShare = rec.value("keyValueEarningsPerShare").toString();
                keyData.keyValueNAVPerShare = rec.value("keyValueNAVPerShare").toString();
                keyData.keyValueDividendPerShare = rec.value("keyValueDividendPerShare").toString();
                keyData.keyValueYield = rec.value("keyValueYield").toString();
                keyData.keyValueCoursePerJEK = rec.value("keyValueCoursePerJEK").toString();
                keyData.earningsDividedByDividend = rec.value("earningsDividedByDividend").toString();
                keyData.navDivLastStockPrice = rec.value("navDivLastStockPrice").toString();

                qry.finish();
                if(dbIsHandledExternly == false)
                {
                    closeDb();
                    m_mutex.unlock();
                }
                return true;

            }

        }
    }

    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }
    return false;
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
 *
 ****************************************************************/
bool CDbHndl::
insertKeyTaBridgeData(QString assetName,
                      QString assetSymbol,
                      bool dbIsHandledExternly)
{
    QString str;



    if(dbIsHandledExternly==false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);



    str.sprintf("INSERT INTO TblNordnetYahooBridge "
                "(assetName, assetSymbol) "
                " VALUES('%s', '%s');",
                assetName.toLocal8Bit().constData(),
                assetSymbol.toLocal8Bit().constData());



    qDebug() << str << "\n";
    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error"), qry.lastError().text().toLatin1().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }

    return true;
}





/*****************************************************************
 *
 * Function:		delAllTblYahooTaData()
 *
 * Description:		This function delete all data
 *
 *
 *
 *****************************************************************/
bool CDbHndl::delAllTblNordnetYahooBridge(void)
{
    QString str;
    //QSqlQuery qry;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    // Create sql question
    str.sprintf("DELETE "
                "FROM TblNordnetYahooBridge;");

    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 1aa2"), qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
        qDebug( "Inserted!" );

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}




/*****************************************************************
 *
 * Function:
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
efficPortfCalcCovar(QVector <CDbHndl::VarianceData_ST> &rowVarianseArr,
                         QVector <CDbHndl::VarianceData_ST> &colVarianseArr,
                         double &result)
{
    VarianceData_ST rowData;
    VarianceData_ST colData;

    result = 0;

    if(rowVarianseArr.size() != colVarianseArr.size())
    {
        return false;
    }

    for(int i = 0; i < rowVarianseArr.size(); i++)
    {
        rowData = rowVarianseArr[i];
        colData = colVarianseArr[i];

        if(rowData.date != colData.date)
        {
            return false;
        }

        result += (rowData.variance * colData.variance);
    }

    result = result / (double) rowVarianseArr.size();
    // result = result / (rowStdDev*colStdDev);

    return true;
}


/*****************************************************************
 *
 * Function:
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
efficPortfCreateVarianceArr(const QString startDate,
                            const QString endDate,
                            CDbHndl::EfficPortStockData_ST &stockData,
                            QVector <CDbHndl::VarianceData_ST> &tmpVarianseArr,
                            bool dbIsHandledExternly)
{
    QSqlRecord rec;
    QString str;
    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }
    QSqlQuery qry(m_db);

    // Data from database
    // Lets start -1 so we can handle price and time on same loaction
    int timeCnt = -1;
    double price;
    double subRes;
    VarianceData_ST resData;
    double refPrice;
    bool initStart = true;
    StateBollingerBand_ET state = BOLLSTATE_PREPARE;
    double result = 0;

    if(stockData.isValid == false)
    {
        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }

    tmpVarianseArr.clear();

    QByteArray ba = stockData.stockSymbol.toLocal8Bit();
    const char *c_assetSymbol = ba.data();

    QByteArray ba1 = startDate.toLocal8Bit();
    const char *c_startDate = ba1.data();

    QByteArray ba2 = endDate.toLocal8Bit();
    const char *c_endDate = ba2.data();


    str.sprintf("SELECT TblYahooTaStockName.assetNameId, TblYahooTaStockName.assetName, TblYahooTaStockName.assetSymbol, "
                " TblYahooTaData.date, TblYahooTaData.adjClose, TblYahooTaData.assetNameId "
                " FROM  TblYahooTaStockName, TblYahooTaData "
                " WHERE TblYahooTaStockName.assetNameId = TblYahooTaData.assetNameId AND "
                " TblYahooTaStockName.assetSymbol = '%s' AND "
                " TblYahooTaData.date >= '%s' AND TblYahooTaData.date <= '%s' "
                " ORDER BY TblYahooTaData.date ASC;",
                c_assetSymbol, c_startDate, c_endDate);

    // qDebug() << "str =" << str << "\n";

    qry.prepare(str);

    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }

        stockData.isValid = false;
        qDebug() << qry.lastError();
        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }
    else
    {
        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("assetSymbol").isNull() == true)
            {
                stockData.isValid = false;
                qry.finish();
                if(dbIsHandledExternly == false)
                {
                    closeDb();
                    m_mutex.unlock();
                }
                return false;
            }
            else
            {
                // qDebug() << (QString) rec.value("adjClose").toString() << "\n";
                // qDebug() << (QString) rec.value("assetName").toString() << "\n";
                // qDebug() << (QString) rec.value("assetSymbol").toString() << "\n";
                // qDebug() << (QString) rec.value("date").toString() << "\n";

                // Get data stored in database
                price = rec.value("adjClose").toDouble();
                timeCnt++; // Note: started on -1


                switch(state)
                {
                case BOLLSTATE_PREPARE:
                    if(initStart == true)
                    {
                        refPrice = price;
                        initStart = false;

                        if(refPrice == 0)
                        {
                            qry.finish();
                            if(dbIsHandledExternly == false)
                            {
                                closeDb();
                                m_mutex.unlock();
                            }
                            return false;
                        }
                    }
                    tmpVarianseArr.clear();
                    timeCnt = -1;
                    state = BOLLSTATE_CALC;
                    break;

                case BOLLSTATE_CALC:
                    subRes = log(price/refPrice);
                    //subRes = ((price - refPrice) / refPrice);
                    resData.variance = (subRes - stockData.meanReturns);
                    result += (resData.variance * resData.variance);
                    resData.date = rec.value("date").toString();
                    tmpVarianseArr.insert(timeCnt, resData);
                    // refPrice = price; // 140106
                    break;
                }
            }
        }
    }

    timeCnt++;

    result = result / (double) timeCnt;

    if(timeCnt < 1)
    {
        stockData.isValid = false;
        qry.finish();
        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }

    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }

    return true;
}




/*****************************************************************
 *
 * Function:
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
efficPortfCalcMeanAndStdDev(const QString startDate,
                            const QString endDate,
                            CDbHndl::EfficPortStockData_ST &stockData)
{
    QSqlRecord rec;
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);

    // Data from database
    // Lets start -1 so we can handle price and time on same loaction
    int timeCnt = -1;
    double price;
    double subRes;
    double refPrice;
    double sumOfPrice = 0;
    bool initStartDate = true;
    StateBollingerBand_ET state = BOLLSTATE_PREPARE;


    m_tmpArr.clear();

    QByteArray ba = stockData.stockSymbol.toLocal8Bit();
    const char *c_assetSymbol = ba.data();

    QByteArray ba1 = startDate.toLocal8Bit();
    const char *c_startDate = ba1.data();

    QByteArray ba2 = endDate.toLocal8Bit();
    const char *c_endDate = ba2.data();


    str.sprintf("SELECT TblYahooTaStockName.assetNameId, TblYahooTaStockName.assetName, TblYahooTaStockName.assetSymbol, "
                " TblYahooTaData.date, TblYahooTaData.adjClose, TblYahooTaData.assetNameId "
                " FROM  TblYahooTaStockName, TblYahooTaData "
                " WHERE TblYahooTaStockName.assetNameId = TblYahooTaData.assetNameId AND "
                " TblYahooTaStockName.assetSymbol = '%s' AND "
                " TblYahooTaData.date >= '%s' AND TblYahooTaData.date <= '%s' "
                " ORDER BY TblYahooTaData.date ASC;",
                c_assetSymbol, c_startDate, c_endDate);

    // qDebug() << "str =" << str << "\n";

    qry.prepare(str);

    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }

        stockData.isValid = false;
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("assetSymbol").isNull() == true)
            {
                stockData.isValid = false;
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                // qDebug() << (QString) rec.value("adjClose").toString() << "\n";
                // qDebug() << (QString) rec.value("assetName").toString() << "\n";
                // qDebug() << (QString) rec.value("assetSymbol").toString() << "\n";
                // qDebug() << (QString) rec.value("date").toString() << "\n";

                // Get data stored in database
                stockData.endDate = rec.value("date").toString();
                price = rec.value("adjClose").toDouble();
                timeCnt++;

                switch(state)
                {
                case BOLLSTATE_PREPARE:

                    if(initStartDate == true)
                    {
                        timeCnt = -1;
                        refPrice = price;
                        qDebug() << "refPrice" << refPrice;
                        initStartDate = false;
                        stockData.stockName = rec.value("assetName").toString();
                        stockData.startDate = rec.value("date").toString();

                        if(refPrice == 0)
                        {
                            stockData.isValid = false;
                            qry.finish();
                            closeDb();
                            m_mutex.unlock();
                            return false;
                        }
                        sumOfPrice = 0;
                        m_tmpArr.clear();
                        state = BOLLSTATE_CALC;
                    }
                    break;
                case BOLLSTATE_CALC:
                    subRes = log(price/refPrice);
                    //subRes = ((price - refPrice) / refPrice);
                    //qDebug() << "price" << price << "refPrice" << refPrice;
                    // qDebug() << "subRes" << subRes << "timeCnt" << timeCnt;
                    sumOfPrice += subRes; 	  // Sum as many data as time period says
                    m_tmpArr.insert(timeCnt, subRes);
                    // refPrice = price; // 140106
                    break;
                }

            }
        }
    }




    if(timeCnt < 1)
    {
        stockData.isValid = false;
        qry.finish();
        closeDb();
        m_mutex.unlock();
        return false;
    }

    timeCnt++;
    stockData.isValid = true;
    stockData.nofSamples = timeCnt;

    // Calc mean
    stockData.meanReturns =  sumOfPrice / (double) (timeCnt);

    qDebug() << "stockData.meanReturns" << stockData.meanReturns;
    qDebug() << "sumOfPrice" << sumOfPrice;
    qDebug() << "cnt" << timeCnt;

    // Calc std dev
    stockData.riskStdDev = 0;
    for(int i = 0; i < timeCnt; i++)
    {
        stockData.riskStdDev += pow((m_tmpArr.at(i) - stockData.meanReturns), 2);
    }

    stockData.riskStdDev = stockData.riskStdDev / (double) timeCnt;
    stockData.riskStdDev = sqrt(stockData.riskStdDev);

    m_tmpStdDev = stockData.riskStdDev;

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}





/*****************************************************************
 *
 * Function:		delAllTblYahooTaStockName()
 *
 * Description:		This function delete all data
 *
 *
 *
 *****************************************************************/
bool CDbHndl::delAllTblYahooTaStockName(void)
{
    QString str;
    //QSqlQuery qry;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    // Create sql question
    str.sprintf("DELETE "
                "FROM TblYahooTaStockName;");

    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 1aa3"), qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
        qDebug( "Inserted!" );

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}



/*****************************************************************
 *
 * Function:		delAllTblTmpEfficPortTreeWidget()
 *
 * Description:		This function delete all data
 *
 *
 *
 *****************************************************************/
bool CDbHndl::delAllTblTmpEfficPortTreeWidget(bool dbIsHandledExternly)
{
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }
    QSqlQuery qry(m_db);



    // Create sql question
    str.sprintf("DELETE "
                "FROM TblTmpEfficPortTreeWidget;");

    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 1aa3"), qry.lastError().text().toLatin1().constData());
        }
        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }
    return true;
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
 *
 ****************************************************************/
bool CDbHndl::
insertTmpEfficPortTreeWidgetData(QString assetName,
                                   QString assetSymbol,
                                   bool isUsed,
                                   bool defaultSettings,
                                   double risk,
                                   double meanReturn,
                                   int minProcent,
                                   int maxProcent,
                                   int selProcent,
                                   int nofItems,
                                   int colorNumber,
                                   bool dbIsHandledExternly)
{
    QString str;



    if(dbIsHandledExternly==false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);


    str.sprintf("INSERT INTO TblTmpEfficPortTreeWidget "
                "(assetName, assetSymbol, isUsed, defaultSettings, "
                "risk, meanReturn, minProcent, maxProcent, selProcent, "
                "nofItems, colorNumber) "
                " VALUES('%s', '%s', %d, %d, %g, %g, %d, %d, %d, %d, %d);",
                assetName.toLocal8Bit().constData(),
                assetSymbol.toLocal8Bit().constData(),
                isUsed,
                defaultSettings,
                risk,
                meanReturn,
                minProcent,
                maxProcent,
                selProcent,
                nofItems,
                colorNumber);



    qDebug() << str << "\n";
    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error"), qry.lastError().text().toLatin1().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }

    return true;
}




/****************************************************************
 *
 * Function:    addStockSymbolAndNameInTreeWidget()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
addTmpEfficPortDataToTreeWidget(QTreeWidget *treeWidget,
                                int column,
                                char *sortOrder,
                                bool dbIsHandledExternly)
{
    QSqlRecord rec;
    QString str;
    CUtil cu;
    QString sortOrder2;

    if(strcmp(sortOrder, SQL_STR_DESC)== 0)
        sortOrder2 = SQL_STR_ASC;
    else
        sortOrder2 = SQL_STR_DESC;



    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);


    switch(column)
    {

    case TWCOLUM_RISK:
        str.sprintf("SELECT * "
                    " FROM TblTmpEfficPortTreeWidget "
                    " ORDER BY risk %s, meanReturn %s, LOWER(assetName) %s  ;",
                    sortOrder, sortOrder2.toLocal8Bit().constData(), sortOrder);
        break;
    case TWCOLUM_RETURN:
        str.sprintf("SELECT * "
                    " FROM TblTmpEfficPortTreeWidget "
                    " ORDER BY meanReturn %s, risk %s,  LOWER(assetName) %s  ;",
                    sortOrder, sortOrder2.toLocal8Bit().constData(), sortOrder);
        break;

    case TWColum_Name:
    case TWColum_SYMBOL:  // Hidden
    case TWCOLUM_IS_SELECTED:
    case TWCOLUM_DEFAULT_SETTINGS:
    case TWCOLUM_MIN_PROCENT:
    case TWCOLUM_MAX_PROCENT:
    case TWCOLUM_SEL_PROCENT:
    case TWCOLUM_NOF_DATA:
    default:
        str.sprintf("SELECT * "
                    " FROM TblTmpEfficPortTreeWidget "
                    " ORDER BY LOWER(assetName) %s, meanReturn %s, risk %s  ;",
                    sortOrder, sortOrder, sortOrder2.toLocal8Bit().constData());

    }

    qry.prepare(str);

    qDebug() << str;

    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }
    else
    {
        treeWidget->clear();
        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("assetSymbol").isNull()==true)
            {
                qry.finish();
                if(dbIsHandledExternly == false)
                {
                    closeDb();
                    m_mutex.unlock();
                }
                return false;
            }
            else
            {
                QTreeWidgetItem *item = new QTreeWidgetItem;
                item->setText(TWColum_Name, (QString)rec.value("assetName").toString());
                item->setText(TWColum_SYMBOL, (QString)rec.value("assetSymbol").toString());

                item->setExpanded(true);
                               item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
                               item->setCheckState(CDbHndl::TWCOLUM_IS_SELECTED, (Qt::CheckState) rec.value("isUsed").toInt()); // Qt::Unchecked
                               item->setCheckState(CDbHndl::TWCOLUM_DEFAULT_SETTINGS, (Qt::CheckState) rec.value("defaultSettings").toInt()); // Qt::Unchecked
                               item->setText(CDbHndl::TWCOLUM_IS_SELECTED, " ");
                               item->setText(CDbHndl::TWCOLUM_DEFAULT_SETTINGS, " ");
                               item->setBackground(CDbHndl::TWCOLUM_IS_SELECTED,cu.getQColor((CUtil::ColorRgb_ET)rec.value("colorNumber").toInt()));


                item->setText(TWCOLUM_RISK, (QString)rec.value("risk").toString());
                item->setText(TWCOLUM_RETURN, (QString)rec.value("meanReturn").toString());
                item->setText(TWCOLUM_MIN_PROCENT, (QString)rec.value("minProcent").toString());
                item->setText(TWCOLUM_MAX_PROCENT, (QString)rec.value("maxProcent").toString());
                item->setText(TWCOLUM_SEL_PROCENT, (QString)rec.value("selProcent").toString());
                item->setText(TWCOLUM_NOF_DATA, (QString)rec.value("nofItems").toString());
                item->setText(TWCOLUM_COLOR_NUMBER, (QString)rec.value("colorNumber").toString());

                treeWidget->addTopLevelItem(item);
            }
        }
    }

    qry.finish();

    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }
    return true;
}








/*****************************************************************
 *
 * Function:		delAllTblYahooTaData()
 *
 * Description:		This function delete all data
 *
 *
 *
 *****************************************************************/
bool CDbHndl::delAllTblYahooTaData(void)
{
    QString str;
    //QSqlQuery qry;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    // Create sql question
    str.sprintf("DELETE "
                "FROM TblYahooTaData;");

    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 1aa2"), qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
        qDebug( "Inserted!" );

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}




/*****************************************************************
 *
 * Function:		delAllTbYahooNordetIntercon()
 *
 * Description:		This function delete all data
 *
 *
 *
 *****************************************************************/
bool CDbHndl::delAllTblYahooNordetIntercon(void)
{
    QString str;
    // QSqlQuery qry;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    // Create sql question
    str.sprintf("DELETE "
                "FROM TblYahooNordetIntercon;");

    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 1aa1"), qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }
    //else
    //    qDebug( "Inserted!" );

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}




/****************************************************************
 *
 * Function:    findYahooTaData()
 *
 * Description: Check if account exist.
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::getLatestDateYahooTaData(QString &date, QString assetSymbol, bool dbIsHandledExternly)
{
    QSqlRecord rec;
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

    QByteArray ba = assetSymbol.toLocal8Bit();
    const char *c_assetSymbol = ba.data();



//    str.sprintf("SELECT TblYahooTaData.assetNameId, TblYahooTaStockName.assetNameId, "
//                "       date, TblYahooTaStockName.assetSymbol "
//                " FROM TblYahooTaStockName, TblYahooTaData "
//                " WHERE TblYahooTaStockName.assetNameId = TblYahooTaData.assetNameId AND "
//                " TblYahooTaStockName.assetSymbol = '%s' AND DATE=(SELECT max(date) FROM TblYahooTaData);", c_assetSymbol);

    str.sprintf("SELECT TblYahooTaStockName.assetNameId, TblYahooTaStockName.assetName, TblYahooTaStockName.assetSymbol, "
                " TblYahooTaData.date, TblYahooTaData.adjClose, TblYahooTaData.assetNameId "
                " FROM  TblYahooTaStockName, TblYahooTaData "
                " WHERE TblYahooTaStockName.assetNameId = TblYahooTaData.assetNameId AND "
                " TblYahooTaStockName.assetSymbol = '%s' "
                " ORDER BY TblYahooTaData.date DESC;",
                c_assetSymbol);



//    str.sprintf("SELECT TblYahooTaData.date TblYahooTaData.assetNameId, TblYahooTaStockName.assetNameId, "
//                        " TblYahooTaStockName.assetSymbol "
//                        " FROM TblYahooTaStockName, TblYahooTaData "
//                        " WHERE TblYahooTaStockName.assetNameId = TblYahooTaData.assetNameId;");
//                     //   " WHERE TblYahooTaStockName.assetNameId = TblYahooTaData.assetNameId AND "
//                     //   " TblYahooTaStockName.assetSymbol = '%s' "
//                     //   ";", c_assetSymbol);
//                     //   " ORDER BY TblYahooTaData.date DESC;", c_assetSymbol);




    //SELECT id,date,grp FROM table group WHERE DATE=(SELECT max(date) FROM table group) GROUP BY GRP

    qDebug() << str << "\n";

    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }
    else
    {
        while(qry.next())
        {
            rec = qry.record();

            qDebug() << assetSymbol << "\n";
            qDebug() << rec.value("DATE").toString() << "\n";
            qDebug() << rec.value("assetSymbol").toString() << "\n";


            if(rec.value("DATE").isNull()==true)
            {
                qry.finish();
                if(dbIsHandledExternly == false)
                {
                    closeDb();
                    m_mutex.unlock();
                }
                return false;
            }
            else
            {
                date = rec.value("DATE").toString();
                qry.finish();
                if(dbIsHandledExternly == false)
                {
                    closeDb();
                    m_mutex.unlock();
                }
                qDebug() << "Found" << "\n";
                qDebug() << date;
                return true;

            }
        }
    }

    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }
    return false;
}




/****************************************************************
 *
 * Function:    findYahooTaData()
 *
 * Description: Check if account exist.
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::findYahooTaData(QString date,
                              int assetNameId,
                              bool dbIsHandledExternly)
{
    QSqlRecord rec;
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

    QByteArray ba = date.toLocal8Bit();
    const char *c_date = ba.data();



    str.sprintf("SELECT assetNameId, date "
                " FROM TblYahooTaData"
                " WHERE '%s' == date AND %d == assetNameId;", c_date, assetNameId);

    qDebug() << str << "\n";

    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }
    else
    {
        while(qry.next())
        {
            rec = qry.record();

            qDebug() << date << "\n";
            qDebug() << rec.value("date").toString() << "\n";
            qDebug() << rec.value("assetNameId").toString() << "\n";


            if(rec.value("assetNameId").isNull()==true)
            {
                qry.finish();
                if(dbIsHandledExternly == false)
                {
                    closeDb();
                    m_mutex.unlock();
                }
                return false;
            }
            else
            {
                if(assetNameId == rec.value("assetNameId").toInt())
                {
                    if(rec.value("date").isNull()==true)
                    {
                        qDebug() << "date = null" << "\n";
                        qry.finish();
                        if(dbIsHandledExternly == false)
                        {
                            closeDb();
                            m_mutex.unlock();
                        }
                        return false;
                    }
                    else
                    {
                        if(date == rec.value("date").toString())
                        {
                            qry.finish();
                            if(dbIsHandledExternly == false)
                            {
                                closeDb();
                                m_mutex.unlock();
                            }
                            qDebug() << "Found" << "\n";
                            return true;
                        }
                    }
                }
            }
        }
    }

    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }
    return false;
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
 *
 ****************************************************************/
bool CDbHndl::findYahooTaStockSymbol(QString assetSymbol, int &assetNameId, bool dbIsHandledExternly)
{
    QSqlRecord rec;
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

    QByteArray ba = assetSymbol.toLocal8Bit();
    const char *c_assetSymbol = ba.data();



    str.sprintf("SELECT assetNameId, assetSymbol "
                " FROM TblYahooTaStockName "
                " WHERE '%s' == assetSymbol;", c_assetSymbol);

    qDebug() << str << "\n";

    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }
    else
    {
        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("assetSymbol").isNull()==true)
            {
                qry.finish();
                if(dbIsHandledExternly == false)
                {
                    closeDb();
                    m_mutex.unlock();
                }
                return false;
            }
            else
            {
                qDebug() << assetSymbol << "\n";
                qDebug() << rec.value("assetSymbol").toString() << "\n";
                qDebug() << rec.value("assetNameId").toString() << "\n";

                if(assetSymbol == rec.value("assetSymbol").toString())
                {
                    if(rec.value("assetNameId").isNull()==true)
                    {
                        qry.finish();
                        if(dbIsHandledExternly == false)
                        {
                            closeDb();
                            m_mutex.unlock();
                        }
                        return false;
                    }
                    else
                    {
                        assetNameId = rec.value("assetNameId").toInt();
                        qry.finish();
                        if(dbIsHandledExternly == false)
                        {
                            closeDb();
                            m_mutex.unlock();
                        }
                        return true;
                    }
                }
            }

        }
    }

    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }
    return false;
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
 *
 ****************************************************************/
bool CDbHndl::insertYahooStockName(QString assetName,
                                   QString assetSymbol,
                                   int &assetNameId,
                                   bool dbIsHandledExternly,
                                   QString marketList,                   // Not implemented yet
                                   QString assetType,                    // Not implemented yet
                                   int yahooNordetInterconId)            // Not implemented yet
{
    QString str;

    if(dbIsHandledExternly==false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

    QByteArray ba = assetName.toLocal8Bit();
    const char *c_assetName = ba.data();

    QByteArray ba1 = assetSymbol.toLocal8Bit();
    const char *c_assetSymbol = ba1.data();

    QByteArray ba2 = marketList.toLocal8Bit();
    const char *c_marketList = ba2.data();

    QByteArray ba3 = assetType.toLocal8Bit();
    const char *c_assetType = ba3.data();

    str.sprintf("INSERT INTO TblYahooTaStockName "
                "(assetName, assetSymbol, marketList, assetType, yahooNordetInterconId) "
                " VALUES('%s', '%s', '%s', '%s', %d);",
                c_assetName,
                c_assetSymbol,
                c_marketList,
                c_assetType,
                yahooNordetInterconId);

    qDebug() << str << "\n";

    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error"), qry.lastError().text().toLatin1().constData());
        }
        if(dbIsHandledExternly==false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }

    // Get assetNameId
    assetNameId = (int) qry.lastInsertId().toInt();

    qry.finish();

    if(dbIsHandledExternly==false)
    {
        closeDb();
        m_mutex.unlock();
    }
    return true;
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
 *
 ****************************************************************/
bool CDbHndl::insertYahooStockData(QString date,
                                   QString open,
                                   QString high,
                                   QString low,
                                   QString close,
                                   QString volume,
                                   QString adjClose,
                                   int assetNameId,
                                   bool dbIsHandledExternly)
{
    bool res;
    CUtil ut;
    QString str;
    double d_open;
    double d_high;
    double d_low;
    double d_close;
    double d_volume;
    double d_adjClose;

    res = ut.number2double(open, d_open);
    res &= ut.number2double(high, d_high);
    res &= ut.number2double(low, d_low);
    res &= ut.number2double(close, d_close);
    res &= ut.number2double(volume, d_volume);
    res &= ut.number2double(adjClose, d_adjClose);

    if(res == false)
    {
        return false;
    }



    if(dbIsHandledExternly==false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);


    str.sprintf("INSERT INTO TblYahooTaData "
                "(date, open, high, low, close, volume, adjClose, assetNameId) "
                " VALUES('%s', %g, %g, %g, %g, %g, %g, %d);",
                date.toLocal8Bit().constData(),
                d_open,
                d_high,
                d_low,
                d_close,
                d_volume,
                d_adjClose,
                assetNameId);


    qDebug() << str << "\n";
    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error"), qry.lastError().text().toLatin1().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }

    return true;
}



/*****************************************************************
 *
 * Function:		getYahooTaPriceData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::getYahooTaPriceData(QString assetSymbol,
                                  QString startDate,
                                  QString endDate,
                                  CYahooStockPlotUtil::PlotData_ST &qwtStockPlotData,
                                  CYahooStockPlotUtil::StockData_ST &stockData)
{

    QSqlRecord rec;
    QString str;
    QString date;
    CYahooStockPlotUtil ypu;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);
    int cnt = 0;


    QByteArray ba = assetSymbol.toLocal8Bit();
    const char *c_assetSymbol = ba.data();

    QByteArray ba1 = startDate.toLocal8Bit();
    const char *c_startDate = ba1.data();

    QByteArray ba2 = endDate.toLocal8Bit();
    const char *c_endDate = ba2.data();



    str.sprintf("SELECT TblYahooTaStockName.assetNameId, TblYahooTaStockName.assetName, TblYahooTaStockName.assetSymbol, "
                " TblYahooTaData.date, TblYahooTaData.adjClose, TblYahooTaData.assetNameId "
                " FROM  TblYahooTaStockName, TblYahooTaData "
                " WHERE TblYahooTaStockName.assetNameId = TblYahooTaData.assetNameId AND "
                " TblYahooTaStockName.assetSymbol = '%s' AND "
                " TblYahooTaData.date >= '%s' AND TblYahooTaData.date <= '%s' "
                " ORDER BY TblYahooTaData.date ASC;",
                c_assetSymbol, c_startDate, c_endDate);

    qDebug() << "str =" << str << "\n";

    qry.prepare(str);

    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        // Contains nof data displayed in plot
        qwtStockPlotData.nofStocksToPlot = 0;

        // This variable is automaticly updated when first data is handled by pdateMinMaxAxis()
        qwtStockPlotData.axis.minMaxIsInit = false;


        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("assetSymbol").isNull() == true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                qDebug() << (QString) rec.value("adjClose").toString() << "\n";
                qDebug() << (QString) rec.value("assetName").toString() << "\n";
                qDebug() << (QString) rec.value("assetSymbol").toString() << "\n";
                qDebug() << (QString) rec.value("date").toString() << "\n";

                stockData.data.x.insert(cnt, (double) cnt);
                stockData.data.y.insert(cnt, rec.value("adjClose").toDouble());
                if(false == rec.value("date").isNull())
                {
                    date = rec.value("date").toString();
                    qDebug() << "b11 date" << date;
                    stockData.data.xDate.insert(cnt, date);
                    ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) cnt, rec.value("adjClose").toDouble(), date);
                }
                else
                {
                    ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) cnt, rec.value("adjClose").toDouble());
                }
                cnt++;
            }
        }
    }

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}




enum StateMovingAverage_ET
{
    AVGSTATE_PREPARE,
    AVGSTATE_CALC

};


enum StateMovingLeastSqrtFit_ET
{
    LSQFSTATE_PREPARE,
    LSQFSTATE_CALC

};



/*!
 *****************************************************************************
 *
 * This function
 *
 ****************************************************************************/
void CDbHndl::init1dLeastSrq(int &nofData,
                             double &meanXSum,
                             double &meanYSum,
                             double &prodXXSum,
                             double &prodYYSum,
                             double &prodXYSum)
{
    nofData = 0;
    meanXSum = 0;
    meanYSum = 0;
    prodXXSum = 0;
    prodYYSum = 0;
    prodXYSum = 0;
}


/*!
 *****************************************************************************
 *
 * This function
 *
 ****************************************************************************/
void CDbHndl::gather1dLeastSrqData(const double x,
                               double y,
                               int &nofData,
                               double &meanXSum,
                               double &meanYSum,
                               double &prodXXSum,
                               double &prodYYSum,
                               double &prodXYSum)
{
        meanXSum += x;
        meanYSum += y;
        prodXXSum += x * x;
        prodYYSum += y * y;
        prodXYSum += x * y;
        (nofData)++;

}



/*!
 *****************************************************************************
 *
 * This function
 *
 ****************************************************************************/
bool CDbHndl::calc1dLeastSrqFitParams(int &nofData,
                                      double &meanXSum,
                                      double &meanYSum,
                                      double &prodXXSum,
                                      double &prodYYSum,
                                      double &prodXYSum,
                                      double &m,
                                      double &k)
{
   double meanX;
   double meanY;

   if(nofData == 0)
       return false;


    meanX =  ((double) meanXSum / (double) nofData);
    meanY =  ((double) meanYSum / (double) nofData);

    if((double) (((double) prodXXSum) - ((double) nofData * (double) meanX * (double) meanX)) == 0)
        return false;

    k = ((double) ( (double) prodXYSum - ((double) nofData * (double) meanX * (double) meanY)) /
               (double) (((double) prodXXSum) - ((double) nofData * (double) meanX * (double) meanX)));

    m = meanY - (k * meanX);

    init1dLeastSrq(nofData, meanXSum, meanYSum, prodXXSum, prodYYSum, prodXYSum);
    return true;
}


/*!
 *****************************************************************************
 *
 * This function
 *
 ****************************************************************************/
bool CDbHndl::calc1dLeastSrqFitRParam(int &nofData,
                                      double &prodXXSum,
                                      double &prodYYSum,
                                      double &prodXYSum,
                                      double &r)
{

   if(nofData == 0)
       return false;

   if((prodXXSum * prodYYSum) == 0)
       return false;


   r = prodXYSum / sqrt((prodXXSum * prodYYSum));

     return true;
}




/*!
 *****************************************************************************
 *
 * This function
 *
 ****************************************************************************/
void CDbHndl::calcMovingLeastSrqFit(const double k, const double m, double x, double &y)
{
    y = k * x + m;
}


/*!
 *****************************************************************************
 *
 * This function
 *
 ****************************************************************************/
bool CDbHndl::getYahooTaLeastSqrFit(const QString assetSymbol,
                                    const QString startDate,
                                    const QString endDate,
                                    CYahooStockPlotUtil::PlotData_ST &qwtStockPlotData,
                                    CYahooStockPlotUtil::StockData_ST &stockData,
                                    QString &kValue,
                                    QString &rValue)
{
    QSqlRecord rec;
    QString str;
    CYahooStockPlotUtil ypu;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);

    // Data from database
    // Lets start -1 so we can handle price and time on same loaction
    int timeCnt = -1;
    double price;
    QString date;

    int nofData = 0;
    double meanXSum = 0;
    double meanYSum = 0;
    double prodXXSum = 0;
    double prodYYSum = 0;
    double prodXYSum = 0;
    double k;
    double m;
    double r;
    bool findStartDate = true;
    QString startDate1;
    QString endDate1;



    QByteArray ba = assetSymbol.toLocal8Bit();
    const char *c_assetSymbol = ba.data();

    QByteArray ba1 = startDate.toLocal8Bit();
    const char *c_startDate = ba1.data();

    QByteArray ba2 = endDate.toLocal8Bit();
    const char *c_endDate = ba2.data();


    str.sprintf("SELECT TblYahooTaStockName.assetNameId, TblYahooTaStockName.assetName, TblYahooTaStockName.assetSymbol, "
                " TblYahooTaData.date, TblYahooTaData.adjClose, TblYahooTaData.assetNameId "
                " FROM  TblYahooTaStockName, TblYahooTaData "
                " WHERE TblYahooTaStockName.assetNameId = TblYahooTaData.assetNameId AND "
                " TblYahooTaStockName.assetSymbol = '%s' AND "
                " TblYahooTaData.date >= '%s' AND TblYahooTaData.date <= '%s' "
                " ORDER BY TblYahooTaData.date ASC;",
                c_assetSymbol, c_startDate, c_endDate);

    qDebug() << "str =" << str << "\n";

    qry.prepare(str);

    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }

        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {

        // Contains nof data displayed in plot
        qwtStockPlotData.nofStocksToPlot = 0;

        // This variable is automaticly updated when first data is handled by pdateMinMaxAxis()
        // qwtStockPlotData.axis.minMaxIsInit = false;

        init1dLeastSrq(nofData, meanXSum, meanYSum, prodXXSum, prodYYSum, prodXYSum);


        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("assetSymbol").isNull() == true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                qDebug() << (QString) rec.value("adjClose").toString() << "\n";
                qDebug() << (QString) rec.value("assetName").toString() << "\n";
                qDebug() << (QString) rec.value("assetSymbol").toString() << "\n";
                qDebug() << (QString) rec.value("date").toString() << "\n";

                // Get data stored in database
                price = rec.value("adjClose").toDouble();
                timeCnt++; // Note: started on -1
                if(false == rec.value("date").isNull())
                {
                    date = rec.value("date").toString();
                    if(findStartDate == true)
                    {
                        findStartDate = false;
                        startDate1 = date;
                        endDate1 = date;
                    }
                    else
                    {
                        endDate1 = date;
                    }
                }


                gather1dLeastSrqData(timeCnt,
                                     price,
                                     nofData,
                                     meanXSum,
                                     meanYSum,
                                     prodXXSum,
                                     prodYYSum,
                                     prodXYSum);
            }
        }
    }


    // Have we any data to cal and plot?
    if(findStartDate == true && nofData > 2)
    {
        qry.finish();
        closeDb();
        m_mutex.unlock();
        return false;
    }

    int oldNofData = nofData;

    if(false == calc1dLeastSrqFitRParam(nofData,prodXXSum, prodYYSum, prodXYSum, r))
    {
        qry.finish();
        closeDb();
        m_mutex.unlock();
        return false;

    }

    if(false == calc1dLeastSrqFitParams(nofData, meanXSum, meanYSum, prodXXSum, prodYYSum, prodXYSum, m, k))
    {
        qry.finish();
        closeDb();
        m_mutex.unlock();
        return false;
    }


    kValue.sprintf("%.3f", k);
    rValue.sprintf("%.3f", r);

    for( int i = 0; i < oldNofData; i++)
    {
        // Calc the start point of the line
        calcMovingLeastSrqFit(k, m, (double)i, price);
        qDebug() << "price" << price << "time" << i;
        stockData.data.indicator1.insert(i, price);
        stockData.data.x.insert(i, double(i));
        // stockData.data.xDate.insert(i, startDate1);
        ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) i, price);
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}





#if 0
enum StateMovingLeastSqrtFit_ET
{
    LSQFSTATE_PREPARE,
    LSQFSTATE_CALC

};


// Använd denna när du gör sub indikators beräkna k-värdet för perioden x och plotta dessa
// i sub-fönstret så ser man när kvärdet lutar uppåt, nedåt och är noll.
// Som indata anger man hur många värden som ska användas för att räkna fram ett k-värde
// använd moving beräkning så räkna på x värden läg till ett nytt och ta bor det älsta
/*****************************************************************
 *
 * Function:		getYahooTaPriceData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::getYahooTaLeastSqrFit(const QString assetSymbol,
                                    const QString startDate,
                                    const QString endDate,
                                    const double dbTimePeriod,
                                    CYahooStockPlotUtil::PlotData_ST &qwtStockPlotData,
                                    CYahooStockPlotUtil::StockData_ST &stockData)
{

    QSqlRecord rec;
    QString str;
    CYahooStockPlotUtil ypu;
    StateMovingLeastSqrtFit_ET state;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);

    // Data from database
    // Lets start -1 so we can handle price and time on same loaction
    int timeCnt = -1;
    double price;
    double sumOfPrice;
    double average;
    QString date;

    int nofData;
    double meanXSum;
    double meanYSum;
    double prodXXSum;
    double prodXYSum;


    // Fifo help variable
    double oldPrice;
    int i;

    // Number of stored plot data
    int stockDataIndex;

    // Initilize que used to calc moving average data
    initializeQ(&m_Queue);
    int timePeriod = (unsigned long) dbTimePeriod;


    QByteArray ba = assetSymbol.toLocal8Bit();
    const char *c_assetSymbol = ba.data();

    QByteArray ba1 = startDate.toLocal8Bit();
    const char *c_startDate = ba1.data();

    QByteArray ba2 = endDate.toLocal8Bit();
    const char *c_endDate = ba2.data();


    str.sprintf("SELECT TblYahooTaStockName.assetNameId, TblYahooTaStockName.assetName, TblYahooTaStockName.assetSymbol, "
                " TblYahooTaData.date, TblYahooTaData.adjClose, TblYahooTaData.assetNameId "
                " FROM  TblYahooTaStockName, TblYahooTaData "
                " WHERE TblYahooTaStockName.assetNameId = TblYahooTaData.assetNameId AND "
                " TblYahooTaStockName.assetSymbol = '%s' AND "
                " TblYahooTaData.date >= '%s' AND TblYahooTaData.date <= '%s' "
                " ORDER BY TblYahooTaData.date ASC;",
                c_assetSymbol, c_startDate, c_endDate);

    qDebug() << "str =" << str << "\n";

    qry.prepare(str);

    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }

        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {

        // Contains nof data displayed in plot
        qwtStockPlotData.nofStocksToPlot = 0;

        // This variable is automaticly updated when first data is handled by pdateMinMaxAxis()
        // qwtStockPlotData.axis.minMaxIsInit = false;

        // This counter is needed until FIFO contains enought data
        i = 0;

        // Number of data to plot
        stockDataIndex = 0;

        sumOfPrice = 0;
        state = LSQFSTATE_PREPARE;


        init1dLeastSrq(nofData,
                       meanXSum,
                       meanYSum,
                       prodXXSum,
                       prodXYSum);
        {



        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("assetSymbol").isNull() == true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                qDebug() << (QString) rec.value("adjClose").toString() << "\n";
                qDebug() << (QString) rec.value("assetName").toString() << "\n";
                qDebug() << (QString) rec.value("assetSymbol").toString() << "\n";
                qDebug() << (QString) rec.value("date").toString() << "\n";

                // Get data stored in database
                price = rec.value("adjClose").toDouble();
                timeCnt++; // Note: started on -1


                switch(state)
                {
                case LSQFSTATE_PREPARE:
                    // Collect data so we are able to calc first mean
                    if(i < timePeriod)
                    {
                        Enqueue(&m_Queue, price);  // Add data to queue
                        sumOfPrice += price; 	  // Sum as many data as time period says

                        if(i == timePeriod - 1)
                        {
                            calcMovingAverage(sumOfPrice, timePeriod, average);

                            stockData.data.indicator1.insert(stockDataIndex, average);
                            stockData.data.x.insert(stockDataIndex, timeCnt);
                            if(false == rec.value("date").isNull())
                            {
                                date = rec.value("date").toString();
                                qDebug() << "b1 date" << date;
                                stockData.data.xDate.insert(stockDataIndex, date);
                                ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, average, date);
                            }
                            else
                            {
                                ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, average);
                            }

                            stockDataIndex++;
                            state = AVGSTATE_CALC;
                        }
                    }
                    i++;
                    break;

                case LSQFSTATE_CALC:


                    // Remove one data from queue
                    Dequeue(&m_Queue, oldPrice);
                    sumOfPrice -= oldPrice;

                    // Add new price to queue
                    Enqueue(&m_Queue, price);
                    sumOfPrice += price;

                    calcMovingAverage(sumOfPrice, timePeriod, average);

                    stockData.data.indicator1.insert(stockDataIndex, average);
                    stockData.data.x.insert(stockDataIndex, timeCnt);
                    if(false == rec.value("date").isNull())
                    {
                        date = rec.value("date").toString();
                        qDebug() << "b1 date" << date;
                        stockData.data.xDate.insert(stockDataIndex, date);
                        ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, average, date);
                    }
                    else
                    {
                        ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, average);
                    }


                    stockDataIndex++;
                    break;
                }
            }
        }
    }

    qDebug() << "lem x" << stockData.data.x.size();
    qDebug() << "len i1" << stockData.data.indicator1.size();
    qry.finish();
    closeDb();

    // Empty FIFO
    while( !EmptyQ(&m_Queue))
    {
        Dequeue(&m_Queue, price);
    }
    m_mutex.unlock();

    if(state == AVGSTATE_CALC)
    {
        return true;
    }

    return false;
}
#endif



/*****************************************************************
 *
 * Function:		getYahooTaPriceData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
void CDbHndl::calcBoillingerBand(const double sumOfPrice,
                                 const double timePeriod,
                                 const double numbStdDev,
                                 double &lowerBand,
                                 double &average,
                                 double &upperBand)
{
    double variance;
    double stdDev;
    double tmpPrice;
    int intTimePeriod = (int) timePeriod;

    // Inhibit dividing with zero
    if(timePeriod == 0)
        return;

    // Calc the middle bollinger band (i.e. the moving average)
    average = sumOfPrice / timePeriod;

    // Calc variance
    variance = 0;
    for(int i = 0; i < intTimePeriod; i++)
    {
        Dequeue(&m_Queue, tmpPrice); // remove
        Enqueue(&m_Queue, tmpPrice); // add
        variance += pow( (tmpPrice - average), 2);
    }

    // Calc standard deviation
    stdDev =  numbStdDev * sqrt(variance / timePeriod);

    // Calc upper bollinger band
    upperBand = average + stdDev;

    // Calc  lower bollinger band
    lowerBand = average - stdDev;


}



/*****************************************************************
 *
 * Function:		getYahooTaPriceData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
void CDbHndl::calcMovingAverage(const double sumOfPrice,
                                 const double timePeriod,
                                 double &average)
{

    // Inhibit dividing with zero
    if(timePeriod == 0)
        return;

    // Calc the middle bollinger band (i.e. the moving average)
    average = sumOfPrice / timePeriod;

}


/*****************************************************************
 *
 * Function:		getYahooTaPriceData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::getYahooTaBoillingerBand(const QString assetSymbol,
                                       const QString startDate,
                                       const QString endDate,
                                       const double dbTimePeriod,
                                       const double numbStdDev,
                                       CYahooStockPlotUtil::PlotData_ST &qwtStockPlotData,
                                       CYahooStockPlotUtil::StockData_ST &stockData)
{

    QSqlRecord rec;
    QString str;
    CYahooStockPlotUtil ypu;
    StateBollingerBand_ET state;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);

    // Data from database
    // Lets start -1 so we can handle price and time on same loaction
    int timeCnt = -1;
    double price;
    double sumOfPrice;
    QString date;

    // Fifo help variable
    double oldPrice;
    int i;

    // Bollingerband result data
    double lowerBand;
    double midBand;
    double upperBand;

    // Number of stored plot data
    int stockDataIndex;

    // Initilize que used to calc moving average data
    initializeQ(&m_Queue);
    int timePeriod = (unsigned long) dbTimePeriod;



    QByteArray ba = assetSymbol.toLocal8Bit();
    const char *c_assetSymbol = ba.data();

    QByteArray ba1 = startDate.toLocal8Bit();
    const char *c_startDate = ba1.data();

    QByteArray ba2 = endDate.toLocal8Bit();
    const char *c_endDate = ba2.data();


    str.sprintf("SELECT TblYahooTaStockName.assetNameId, TblYahooTaStockName.assetName, TblYahooTaStockName.assetSymbol, "
                " TblYahooTaData.date, TblYahooTaData.adjClose, TblYahooTaData.assetNameId "
                " FROM  TblYahooTaStockName, TblYahooTaData "
                " WHERE TblYahooTaStockName.assetNameId = TblYahooTaData.assetNameId AND "
                " TblYahooTaStockName.assetSymbol = '%s' AND "
                " TblYahooTaData.date >= '%s' AND TblYahooTaData.date <= '%s' "
                " ORDER BY TblYahooTaData.date ASC;",
                c_assetSymbol, c_startDate, c_endDate);

    qDebug() << "str =" << str << "\n";

    qry.prepare(str);

    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }

        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {

        // Contains nof data displayed in plot
        qwtStockPlotData.nofStocksToPlot = 0;

        // This variable is automaticly updated when first data is handled by pdateMinMaxAxis()
        // qwtStockPlotData.axis.minMaxIsInit = false;

        // This counter is needed until FIFO contains enought data
        i = 0;

        // Number of data to plot
        stockDataIndex = 0;

        sumOfPrice = 0;
        state = BOLLSTATE_PREPARE;


        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("assetSymbol").isNull() == true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                qDebug() << (QString) rec.value("adjClose").toString() << "\n";
                qDebug() << (QString) rec.value("assetName").toString() << "\n";
                qDebug() << (QString) rec.value("assetSymbol").toString() << "\n";
                qDebug() << (QString) rec.value("date").toString() << "\n";

                // Get data stored in database
                price = rec.value("adjClose").toDouble();
                timeCnt++; // Note: started on -1


                switch(state)
                {
                case BOLLSTATE_PREPARE:
                    // Collect data so we are able to calc first mean
                    if(i < timePeriod)
                    {
                        Enqueue(&m_Queue, price);  // Add data to queue
                        sumOfPrice += price; 	  // Sum as many data as time period says

                        if(i == timePeriod - 1)
                        {
                            calcBoillingerBand(sumOfPrice, timePeriod, numbStdDev, lowerBand, midBand, upperBand);

                            stockData.data.indicator3.insert(stockDataIndex, upperBand);
                            stockData.data.indicator2.insert(stockDataIndex, midBand);
                            stockData.data.indicator1.insert(stockDataIndex, lowerBand);
                            stockData.data.x.insert(stockDataIndex, timeCnt);

                            if(false == rec.value("date").isNull())
                            {
                                date = rec.value("date").toString();
                                qDebug() << "b1 date" << date;
                                stockData.data.xDate.insert(stockDataIndex, date);
                                ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, upperBand, date);
                                ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, lowerBand, date);

                            }
                            else
                            {
                                ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, upperBand);
                                ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, lowerBand);

                            }
                            stockDataIndex++;
                            state = BOLLSTATE_CALC;
                        }
                    }
                    i++;
                    break;

                case BOLLSTATE_CALC:
                    // Calc the Bollinger bands for the rest of the data

                    // Remove one data from queue
                    Dequeue(&m_Queue, oldPrice);
                    sumOfPrice -= oldPrice;

                    // Add new price to queue
                    Enqueue(&m_Queue, price);
                    sumOfPrice += price;

                    calcBoillingerBand(sumOfPrice, timePeriod, numbStdDev, lowerBand, midBand, upperBand);

                    stockData.data.indicator3.insert(stockDataIndex, upperBand);
                    stockData.data.indicator2.insert(stockDataIndex, midBand);
                    stockData.data.indicator1.insert(stockDataIndex, lowerBand);
                    stockData.data.x.insert(stockDataIndex, timeCnt);
                    if(false == rec.value("date").isNull())
                    {
                        date = rec.value("date").toString();
                        qDebug() << "b2 date" << date;
                        stockData.data.xDate.insert(stockDataIndex, date);
                        ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, upperBand, date);
                        ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, lowerBand, date);

                    }
                    else
                    {
                        ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, upperBand);
                        ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, lowerBand);
                    }
                    stockDataIndex++;
                    break;
                }
            }
        }
    }

    qDebug() << "lem x" << stockData.data.x.size();
    qDebug() << "len i1" << stockData.data.indicator1.size();
    qDebug() << "lem i2" << stockData.data.indicator2.size();
    qDebug() << "lem i3" << stockData.data.indicator3.size();
    qry.finish();
    closeDb();

    // Empty FIFO
    while( !EmptyQ(&m_Queue))
    {
        Dequeue(&m_Queue, price);
    }
    m_mutex.unlock();

    if(state == BOLLSTATE_CALC)
    {
        return true;
    }

    return false;

}


/*****************************************************************
 *
 * Function:		getYahooTaPriceData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::getYahooTaMacd(const QString assetSymbol,
                             const QString startDate,
                             const QString endDate,
                             const double dbLongTimePeriod,
                             const double dbMidTimePeriod,
                             const double dbshortTimePeriod,
                             CYahooStockPlotUtil::PlotData_ST &qwtStockPlotData,
                             CYahooStockPlotUtil::StockData_ST &stockData)
{
    bool res;

    m_longExpMovAvgQue.clear();
    m_shortExpMovAvgQue.clear();
    m_macdQue.clear();

    res = calcMacdMovAvg(assetSymbol, startDate, endDate, dbLongTimePeriod, m_longExpMovAvgQue);

    if(res == false)
    {
        return false;
    }

    res = calcMacdMovAvg(assetSymbol, startDate, endDate, dbMidTimePeriod, m_shortExpMovAvgQue);

    if(res == false)
    {
        return false;
    }

    res = clacMacdLine(m_shortExpMovAvgQue, m_longExpMovAvgQue, m_macdQue);

    if(res == false)
    {
        return false;
    }

    res = calcMacdSignalHist(dbshortTimePeriod, m_macdQue, qwtStockPlotData, stockData);

    if(res == false)
    {
        return false;
    }

    return true;
}





/*****************************************************************
 *
 * Function:		getYahooTaPriceData()
 *
 * Description: Calc exponentional moving average and store in queue
 *
 *              First EMA is an ordinary average of the price in the
 *              first time serie (dbTimePeriod).
 *
 *              EMA(n) = closePrice(n) * (2/(timePeriod+1)) +
 *                       EMA(n-1) * (1 - (2/(timePeriod+1)))
 *
 *****************************************************************/
bool CDbHndl::clacMacdLine(QQueue <MacdQueData_ST> shortExpMovAvgQue,
                           QQueue <MacdQueData_ST> longExpMovAvgQue,
                           QQueue <MacdQueData_ST> &macdExpMovAvgQue)
{
    StateMovingAverage_ET state;
    MacdQueData_ST qDataMacd;
    MacdQueData_ST qDataShort;
    MacdQueData_ST qDataLong;


    state = AVGSTATE_PREPARE;

    qDataLong = longExpMovAvgQue.dequeue();

    while(!longExpMovAvgQue.isEmpty() && !shortExpMovAvgQue.isEmpty())
    {

        switch(state)
        {
        case AVGSTATE_PREPARE:
            qDataShort = shortExpMovAvgQue.dequeue();
            qDebug() << qDataShort.date << qDataLong.date;

             if(qDataShort.date == qDataLong.date)
            {
                qDataMacd.data = qDataShort.data - qDataLong.data;
                qDataMacd.date = qDataShort.date;
                qDataMacd.strDate = qDataShort.strDate;
                macdExpMovAvgQue.enqueue(qDataMacd);
                state = AVGSTATE_CALC;
            }
            break;

        case AVGSTATE_CALC:
            qDataLong = longExpMovAvgQue.dequeue();
            qDataShort = shortExpMovAvgQue.dequeue();

            qDataMacd.data = qDataShort.data - qDataLong.data;
            qDataMacd.date = qDataShort.date;
            qDataMacd.strDate = qDataShort.strDate;
            macdExpMovAvgQue.enqueue(qDataMacd);
            break;
        }
    }

    if(state == AVGSTATE_CALC)
    {
        return true;
    }

    return false;
}



/*****************************************************************
 *
 * Function:		getYahooTaPriceData()
 *
 * Description: Calc exponentional moving average and store in queue
 *
 *              First EMA is an ordinary average of the price in the
 *              first time serie (dbTimePeriod).
 *
 *              EMA(n) = closePrice(n) * (2/(timePeriod+1)) +
 *                       EMA(n-1) * (1 - (2/(timePeriod+1)))
 *
 *****************************************************************/
bool CDbHndl::calcMacdMovAvg(const QString assetSymbol,
                             const QString startDate,
                             const QString endDate,
                             const double dbTimePeriod,
                             QQueue <MacdQueData_ST> &expMovAvgQue)
{

    QSqlRecord rec;
    QString str;
    CYahooStockPlotUtil ypu;
    StateMovingAverage_ET state;
    MacdQueData_ST qData;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);

    // Data from database
    // Lets start -1 so we can handle price and time on same loaction
    int timeCnt = -1;
    double price;
    double sumOfPrice;
    double average;
    double ema;
    double oldEma;
    QString date;

    // Fifo help variable
    int i;


    // Initilize que used to calc moving average data
    initializeQ(&m_Queue);
    int timePeriod = (unsigned long) dbTimePeriod;


    QByteArray ba = assetSymbol.toLocal8Bit();
    const char *c_assetSymbol = ba.data();

    QByteArray ba1 = startDate.toLocal8Bit();
    const char *c_startDate = ba1.data();

    QByteArray ba2 = endDate.toLocal8Bit();
    const char *c_endDate = ba2.data();


    str.sprintf("SELECT TblYahooTaStockName.assetNameId, TblYahooTaStockName.assetName, TblYahooTaStockName.assetSymbol, "
                " TblYahooTaData.date, TblYahooTaData.adjClose, TblYahooTaData.assetNameId "
                " FROM  TblYahooTaStockName, TblYahooTaData "
                " WHERE TblYahooTaStockName.assetNameId = TblYahooTaData.assetNameId AND "
                " TblYahooTaStockName.assetSymbol = '%s' AND "
                " TblYahooTaData.date >= '%s' AND TblYahooTaData.date <= '%s' "
                " ORDER BY TblYahooTaData.date ASC;",
                c_assetSymbol, c_startDate, c_endDate);

    qDebug() << "str =" << str << "\n";

    qry.prepare(str);

    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }

        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {

        // This counter is needed until FIFO contains enought data
        i = 0;

        sumOfPrice = 0;
        state = AVGSTATE_PREPARE;


        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("assetSymbol").isNull() == true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                qDebug() << (QString) rec.value("adjClose").toString() << "\n";
                qDebug() << (QString) rec.value("assetName").toString() << "\n";
                qDebug() << (QString) rec.value("assetSymbol").toString() << "\n";
                qDebug() << (QString) rec.value("date").toString() << "\n";

                // Get data stored in database
                price = rec.value("adjClose").toDouble();
                date = rec.value("date").toString();
                timeCnt++; // Note: started on -1


                switch(state)
                {
                case AVGSTATE_PREPARE:
                    // Collect data so we are able to calc first mean
                    sumOfPrice += price; 	  // Sum as many data as time period says

                    if(i == timePeriod - 1)
                    {
                        average = sumOfPrice / dbTimePeriod;
                        oldEma = average;
                        qData.data = average;
                        qData.date = timeCnt;
                        qData.strDate = date;
                        expMovAvgQue.enqueue(qData);
                        state = AVGSTATE_CALC;
                    }
                    i++;
                    break;

                case AVGSTATE_CALC:
                    ema = price * (2 / (dbTimePeriod + 1)) + oldEma * (1 - (2 / (dbTimePeriod + 1)) );

                    qData.data = ema;
                    qData.date = timeCnt;
                    qData.strDate = date;
                    expMovAvgQue.enqueue(qData);

                    oldEma = ema;
                    break;
                }
            }
        }
    }

    qry.finish();
    closeDb();
    m_mutex.unlock();

    if(state == AVGSTATE_CALC)
    {
        return true;
    }

    return false;
}


/*****************************************************************
 *
 * Function:		getYahooTaPriceData()
 *
 * Description: Calc exponentional moving average and store in queue
 *
 *              First EMA is an ordinary average of the price in the
 *              first time serie (dbTimePeriod).
 *
 *              EMA(n) = closePrice(n) * (2/(timePeriod+1)) +
 *                       EMA(n-1) * (1 - (2/(timePeriod+1)))
 *
 *****************************************************************/
bool CDbHndl::calcMacdSignalHist(const double dbTimePeriod,
                                      QQueue <MacdQueData_ST> macdExpMovAvgQue,
                                      CYahooStockPlotUtil::PlotData_ST &qwtStockPlotData,
                                      CYahooStockPlotUtil::StockData_ST &stockData)
{
    CYahooStockPlotUtil ypu;
    StateMovingAverage_ET state;
    MacdQueData_ST qData;


    // Data from database
    // Lets start -1 so we can handle price and time on same loaction
    double sumOfPrice;
    double histogram;
    double signal;
    double oldSignal;

    // Fifo help variable
    int i;

    // Number of stored plot data
    int stockDataIndex;

    // Initilize que used to calc moving average data
    int timePeriod = (unsigned long) dbTimePeriod;


    // Contains nof data displayed in plot
    qwtStockPlotData.nofStocksToPlot = 0;

    // This variable is automaticly updated when first data is handled by pdateMinMaxAxis()
    qwtStockPlotData.axis.minMaxIsInit = false;

    // This counter is needed until FIFO contains enought data
    i = 0;

    // Number of data to plot
    stockDataIndex = 0;

    sumOfPrice = 0;
    state = AVGSTATE_PREPARE;


    while(!macdExpMovAvgQue.isEmpty())
    {
        qData = macdExpMovAvgQue.dequeue(); // Macd data
        switch(state)
        {
        case AVGSTATE_PREPARE:

            sumOfPrice += qData.data;
            if(i == timePeriod - 1)
            {
                signal = sumOfPrice / dbTimePeriod;
                oldSignal = signal;
                histogram = qData.data - signal;

                stockData.data.indicator1.insert(stockDataIndex, qData.data); // MACD
                stockData.data.indicator2.insert(stockDataIndex, signal); // Signal
                stockData.data.indicator3.insert(stockDataIndex, histogram); // histogram
                stockData.data.x.insert(stockDataIndex, qData.date);
                stockData.data.xDate.insert(stockDataIndex, qData.strDate);
                ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) qData.date, qData.data, qData.strDate);
                ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) qData.date, signal, qData.strDate);
                ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) qData.date, histogram, qData.strDate);
                stockDataIndex++;
                state = AVGSTATE_CALC;
            }
            i++;
            break;

        case AVGSTATE_CALC:
            signal = qData.data * (2 / (dbTimePeriod + 1)) + oldSignal * (1 - (2 / (dbTimePeriod + 1)) );
            oldSignal = signal;
            histogram = qData.data - signal;

            stockData.data.indicator1.insert(stockDataIndex, qData.data); // MACD
            stockData.data.indicator2.insert(stockDataIndex, signal); // Signal
            stockData.data.indicator3.insert(stockDataIndex, histogram); // histogram
            stockData.data.x.insert(stockDataIndex, qData.date);
            stockData.data.xDate.insert(stockDataIndex, qData.strDate);
            ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) qData.date, qData.data, qData.strDate);
            ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) qData.date, signal, qData.strDate);
            ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) qData.date, histogram, qData.strDate);
            stockDataIndex++;
            break;
        }
    }



    if(state == AVGSTATE_CALC)
    {
        return true;
    }

    return false;
}





/*****************************************************************
 *
 * Function:		getYahooTaPriceData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::getYahooTaRateOfChange(const QString assetSymbol,
                                       const QString startDate,
                                       const QString endDate,
                                       const double dbTimePeriod,
                                       CYahooStockPlotUtil::PlotData_ST &qwtStockPlotData,
                                       CYahooStockPlotUtil::StockData_ST &stockData)
{

    QSqlRecord rec;
    QString str;
    CYahooStockPlotUtil ypu;
    StateMovingAverage_ET state;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);

    // Data from database
    // Lets start -1 so we can handle price and time on same loaction
    int timeCnt = -1;
    double price;
    double sumOfPrice;
    double res;
    QString date;

    // Fifo help variable
    double oldPrice;
    int i;

    // Number of stored plot data
    int stockDataIndex;

    // Initilize que used to calc moving average data
    initializeQ(&m_Queue);
    int timePeriod = (unsigned long) dbTimePeriod;


    QByteArray ba = assetSymbol.toLocal8Bit();
    const char *c_assetSymbol = ba.data();

    QByteArray ba1 = startDate.toLocal8Bit();
    const char *c_startDate = ba1.data();

    QByteArray ba2 = endDate.toLocal8Bit();
    const char *c_endDate = ba2.data();


    str.sprintf("SELECT TblYahooTaStockName.assetNameId, TblYahooTaStockName.assetName, TblYahooTaStockName.assetSymbol, "
                " TblYahooTaData.date, TblYahooTaData.adjClose, TblYahooTaData.assetNameId "
                " FROM  TblYahooTaStockName, TblYahooTaData "
                " WHERE TblYahooTaStockName.assetNameId = TblYahooTaData.assetNameId AND "
                " TblYahooTaStockName.assetSymbol = '%s' AND "
                " TblYahooTaData.date >= '%s' AND TblYahooTaData.date <= '%s' "
                " ORDER BY TblYahooTaData.date ASC;",
                c_assetSymbol, c_startDate, c_endDate);

    qDebug() << "str =" << str << "\n";

    qry.prepare(str);

    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }

        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {

        // Contains nof data displayed in plot
        qwtStockPlotData.nofStocksToPlot = 0;

        // This variable is automaticly updated when first data is handled by pdateMinMaxAxis()
        qwtStockPlotData.axis.minMaxIsInit = false;

        // This counter is needed until FIFO contains enought data
        i = 0;

        // Number of data to plot
        stockDataIndex = 0;

        sumOfPrice = 0;
        state = AVGSTATE_PREPARE;




        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("assetSymbol").isNull() == true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                qDebug() << (QString) rec.value("adjClose").toString() << "\n";
                qDebug() << (QString) rec.value("assetName").toString() << "\n";
                qDebug() << (QString) rec.value("assetSymbol").toString() << "\n";
                qDebug() << (QString) rec.value("date").toString() << "\n";

                // Get data stored in database
                price = rec.value("adjClose").toDouble();
                timeCnt++; // Note: started on -1


                switch(state)
                {
                case AVGSTATE_PREPARE:

                    Enqueue(&m_Queue, price);  // Add data to queue
                    sumOfPrice += price; 	  // Sum as many data as time period says

                    if(i == timePeriod - 1)
                    {

                        Dequeue(&m_Queue, oldPrice);
                        res = (price / oldPrice) * 100;

                        stockData.data.indicator1.insert(stockDataIndex, res);
                        stockData.data.indicator2.insert(stockDataIndex, 100);
                        stockData.data.x.insert(stockDataIndex, timeCnt);
                        if(false == rec.value("date").isNull())
                        {
                            date = rec.value("date").toString();
                            qDebug() << "b1 date" << date;
                            stockData.data.xDate.insert(stockDataIndex, date);
                            ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, res, date);
                            ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, 100, date);
                        }
                        else
                        {
                            ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, res);
                            ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, 100);
                        }

                        stockDataIndex++;
                        state = AVGSTATE_CALC;
                    }
                    i++;
                    break;

                case AVGSTATE_CALC:
                    Enqueue(&m_Queue, price);			// Add data to queue
                    Dequeue(&m_Queue, oldPrice);		// Get data from queue
                    res = (price / oldPrice) * 100;

                    stockData.data.indicator1.insert(stockDataIndex, res);
                    stockData.data.indicator2.insert(stockDataIndex, 100);
                    stockData.data.x.insert(stockDataIndex, timeCnt);
                    if(false == rec.value("date").isNull())
                    {
                        date = rec.value("date").toString();
                        qDebug() << "b1 date" << date;
                        stockData.data.xDate.insert(stockDataIndex, date);
                        ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, res, date);
                    }
                    else
                    {
                        ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, res);
                    }
                stockDataIndex++;
                break;
                }
            }
        }
    }

    qDebug() << "len x" << stockData.data.x.size();
    qDebug() << "len i1" << stockData.data.indicator1.size();
    qry.finish();
    closeDb();

    // Empty FIFO
    while( !EmptyQ(&m_Queue))
    {
        Dequeue(&m_Queue, price);
    }
    m_mutex.unlock();

    if(state == AVGSTATE_CALC)
    {
        return true;
    }

    return false;
}



#if 0
tmp.sprintf("CREATE TABLE IF NOT EXISTS TblYahooTaData"
            " (date DATE , "
            " open REAL, "
            " High REAL, "
            " low REAL, "
            " close REAL, "
            " volume REAL, "
            " adjClose REAL, "
            " assetNameId INTEGER,"
            " PRIMARY KEY (date, assetNameId));");

#endif




/*****************************************************************
 *
 * Function:		getYahooTaPriceData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::getYahooTaVolume(QString assetSymbol,
                                  QString startDate,
                                  QString endDate,
                                  CYahooStockPlotUtil::PlotData_ST &qwtStockPlotData,
                                  CYahooStockPlotUtil::StockData_ST &stockData)
{

    QSqlRecord rec;
    QString str;
    QString date;
    CYahooStockPlotUtil ypu;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);
    int cnt = 0;


    QByteArray ba = assetSymbol.toLocal8Bit();
    const char *c_assetSymbol = ba.data();

    QByteArray ba1 = startDate.toLocal8Bit();
    const char *c_startDate = ba1.data();

    QByteArray ba2 = endDate.toLocal8Bit();
    const char *c_endDate = ba2.data();



    str.sprintf("SELECT TblYahooTaStockName.assetNameId, TblYahooTaStockName.assetName, TblYahooTaStockName.assetSymbol, "
                " TblYahooTaData.date, TblYahooTaData.volume, TblYahooTaData.assetNameId "
                " FROM  TblYahooTaStockName, TblYahooTaData "
                " WHERE TblYahooTaStockName.assetNameId = TblYahooTaData.assetNameId AND "
                " TblYahooTaStockName.assetSymbol = '%s' AND "
                " TblYahooTaData.date >= '%s' AND TblYahooTaData.date <= '%s' "
                " ORDER BY TblYahooTaData.date ASC;",
                c_assetSymbol, c_startDate, c_endDate);

    qDebug() << "str =" << str << "\n";

    qry.prepare(str);

    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        // Contains nof data displayed in plot
        qwtStockPlotData.nofStocksToPlot = 0;

        // This variable is automaticly updated when first data is handled by pdateMinMaxAxis()
        qwtStockPlotData.axis.minMaxIsInit = false;


        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("assetSymbol").isNull() == true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                qDebug() << (QString) rec.value("volume").toString() << "\n";
                qDebug() << (QString) rec.value("assetName").toString() << "\n";
                qDebug() << (QString) rec.value("assetSymbol").toString() << "\n";
                qDebug() << (QString) rec.value("date").toString() << "\n";

                stockData.data.x.insert(cnt, (double) cnt);
                stockData.data.indicator1.insert(cnt, rec.value("volume").toDouble());
                if(false == rec.value("date").isNull())
                {
                    date = rec.value("date").toString();
                    qDebug() << "b11 date" << date;
                    stockData.data.xDate.insert(cnt, date);
                    ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) cnt, rec.value("volume").toDouble(), date);
                }
                else
                {
                    ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) cnt, rec.value("volume").toDouble());
                }
                cnt++;
            }
        }
    }

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}










/*****************************************************************
 *
 * Function:		getYahooTaRsi()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::getYahooTaRsi(const QString assetSymbol,
                            const QString startDate,
                            const QString endDate,
                            const double dbTimePeriod,
                            CYahooStockPlotUtil::PlotData_ST &qwtStockPlotData,
                            CYahooStockPlotUtil::StockData_ST &stockData)
{

    QSqlRecord rec;
    QString str;
    CYahooStockPlotUtil ypu;
    StateMovingAverage_ET state;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);

    // Data from database
    // Lets start -1 so we can handle price and time on same loaction
    int timeCnt = -1;
    double price;
    QString date;

     // Number of stored plot data
    int stockDataIndex;

    double res;
    double refDayPrice;
    double RSU;
    double RSD;
    int i;
    bool initRefPrice = true;
    int timePeriod;


    QByteArray ba = assetSymbol.toLocal8Bit();
    const char *c_assetSymbol = ba.data();

    QByteArray ba1 = startDate.toLocal8Bit();
    const char *c_startDate = ba1.data();

    QByteArray ba2 = endDate.toLocal8Bit();
    const char *c_endDate = ba2.data();


    str.sprintf("SELECT TblYahooTaStockName.assetNameId, TblYahooTaStockName.assetName, TblYahooTaStockName.assetSymbol, "
                " TblYahooTaData.date, TblYahooTaData.adjClose, TblYahooTaData.assetNameId "
                " FROM  TblYahooTaStockName, TblYahooTaData "
                " WHERE TblYahooTaStockName.assetNameId = TblYahooTaData.assetNameId AND "
                " TblYahooTaStockName.assetSymbol = '%s' AND "
                " TblYahooTaData.date >= '%s' AND TblYahooTaData.date <= '%s' "
                " ORDER BY TblYahooTaData.date ASC;",
                c_assetSymbol, c_startDate, c_endDate);

    qDebug() << "str =" << str << "\n";

    qry.prepare(str);

    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }

        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        // Contains nof data displayed in plot
        qwtStockPlotData.nofStocksToPlot = 0;

        // This variable is automaticly updated when first data is handled by pdateMinMaxAxis()
        qwtStockPlotData.axis.minMaxIsInit = false;

        // Number of data to plot
        stockDataIndex = 0;

        state = AVGSTATE_PREPARE;

        timePeriod = ( unsigned long ) dbTimePeriod;

        // This counter is needed until FIFO contains enought data
        i = 0;
        RSU = 0;
        RSD = 0;

        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("assetSymbol").isNull() == true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                qDebug() << (QString) rec.value("adjClose").toString() << "\n";
                qDebug() << (QString) rec.value("assetName").toString() << "\n";
                qDebug() << (QString) rec.value("assetSymbol").toString() << "\n";
                qDebug() << (QString) rec.value("date").toString() << "\n";

                // Get data stored in database
                price = rec.value("adjClose").toDouble();
                timeCnt++; // Note: started on -1

                if(initRefPrice == true)
                {
                    initRefPrice = false;
                    refDayPrice = price;
                }


                switch(state)
                {
                case AVGSTATE_PREPARE:
                    // Collect data so we are able to calc first mean
                    // Did price change up ?
                    if( (res = price - refDayPrice) >= 0)
                    {
                        RSU +=  res;
                    }
                    else // No, it change down
                    {
                        RSD -= res;
                    }
                    refDayPrice = price;

                    if(i == timePeriod - 1)
                    {
                        if( RSD == 0 )
                            RSD = 1e-16;

                        res = 100 - ( 100 / ( 1 + (RSU / RSD) ) );

                        stockData.data.indicator1.insert(stockDataIndex, res);
                        stockData.data.indicator2.insert(stockDataIndex, 30);
                        stockData.data.indicator3.insert(stockDataIndex, 70);
                        stockData.data.x.insert(stockDataIndex, timeCnt);
                        if(false == rec.value("date").isNull())
                        {
                            date = rec.value("date").toString();
                            qDebug() << "b1 date" << date;
                            stockData.data.xDate.insert(stockDataIndex, date);
                            ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, res, date);
                            ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, 30, date);
                            ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, 70, date);
                        }
                        else
                        {
                            ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, res);
                        }
                        stockDataIndex++;
                        state = AVGSTATE_CALC;
                    }
                    i++;
                    break;

                case AVGSTATE_CALC:
                    // Did price change up ?
                    if( (res = price - refDayPrice) >= 0)
                    {
                        RSU =  (RSU * (dbTimePeriod - 1) + res) / dbTimePeriod;
                        RSD =  (RSD * (dbTimePeriod - 1) - 0) / dbTimePeriod;
                    }
                    else // No, it changed down
                    {
                        RSU =  (RSU * (dbTimePeriod - 1) + 0) / dbTimePeriod;
                        RSD =  (RSD * (dbTimePeriod - 1) - res) / dbTimePeriod;
                    }

                    refDayPrice = price;

                    if( RSD == 0 )
                    {
                        RSD = 1e-16;
                    }

                    // Calc and store RSI data
                    res = 100 - ( 100 / ( 1 + (RSU / RSD) ) );

                    stockData.data.indicator1.insert(stockDataIndex, res);
                    stockData.data.indicator2.insert(stockDataIndex, 30);
                    stockData.data.indicator3.insert(stockDataIndex, 70);
                    stockData.data.x.insert(stockDataIndex, timeCnt);
                    if(false == rec.value("date").isNull())
                    {
                        date = rec.value("date").toString();
                        qDebug() << "b1 date" << date;
                        stockData.data.xDate.insert(stockDataIndex, date);
                        ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, res, date);
                    }
                    else
                    {
                        ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, res);
                    }
                    stockDataIndex++;
                break;
                }
            }
        }
    }

    qDebug() << "len x" << stockData.data.x.size();
    qDebug() << "len i1" << stockData.data.indicator1.size();
    qry.finish();
    closeDb();

    m_mutex.unlock();

    if(state == AVGSTATE_CALC)
    {
        return true;
    }

    return false;
}




/*****************************************************************
 *
 * Function:		getYahooTaPriceData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::getYahooTaStochastics(const QString assetSymbol,
                                    const QString startDate,
                                    const QString endDate,
                                    const double dbTimePeriod,
                                    double procDAvgTime,
                                    CYahooStockPlotUtil::PlotData_ST &qwtStockPlotData,
                                    CYahooStockPlotUtil::StockData_ST &stockData)
{

    QSqlRecord rec;
    QString str;
    CYahooStockPlotUtil ypu;
    StateMovingAverage_ET state;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);

    double expProc;
    double price;
    double oldPrice;
    double rawK;
    double procK;
    double procD;


    double minPrice;
    double maxPrice;
    bool initMinMaxPrice = true;


    // Data from database
    // Lets start -1 so we can handle price and time on same loaction
    int timeCnt = -1;
    QString date;

    // Fifo help variable
    int i;

    // Number of stored plot data
    int stockDataIndex;

    // Initilize que used to calc moving average data
    initializeQ(&m_Queue);
    int timePeriod = (unsigned long) dbTimePeriod;


    QByteArray ba = assetSymbol.toLocal8Bit();
    const char *c_assetSymbol = ba.data();

    QByteArray ba1 = startDate.toLocal8Bit();
    const char *c_startDate = ba1.data();

    QByteArray ba2 = endDate.toLocal8Bit();
    const char *c_endDate = ba2.data();


    str.sprintf("SELECT TblYahooTaStockName.assetNameId, TblYahooTaStockName.assetName, TblYahooTaStockName.assetSymbol, "
                " TblYahooTaData.date, TblYahooTaData.adjClose, TblYahooTaData.assetNameId "
                " FROM  TblYahooTaStockName, TblYahooTaData "
                " WHERE TblYahooTaStockName.assetNameId = TblYahooTaData.assetNameId AND "
                " TblYahooTaStockName.assetSymbol = '%s' AND "
                " TblYahooTaData.date >= '%s' AND TblYahooTaData.date <= '%s' "
                " ORDER BY TblYahooTaData.date ASC;",
                c_assetSymbol, c_startDate, c_endDate);

    qDebug() << "str =" << str << "\n";

    qry.prepare(str);

    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }

        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {

        // Contains nof data displayed in plot
        qwtStockPlotData.nofStocksToPlot = 0;

        // This variable is automaticly updated when first data is handled by pdateMinMaxAxis()
        qwtStockPlotData.axis.minMaxIsInit = false;

        // This counter is needed until FIFO contains enought data
        i = 0;

        // calculate the exponential percentage parameter
        expProc = 2 / ( procDAvgTime + 1);

        timePeriod = ( unsigned long ) dbTimePeriod;



        // Number of data to plot
        stockDataIndex = 0;

        state = AVGSTATE_PREPARE;


        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("assetSymbol").isNull() == true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                qDebug() << (QString) rec.value("adjClose").toString() << "\n";
                qDebug() << (QString) rec.value("assetName").toString() << "\n";
                qDebug() << (QString) rec.value("assetSymbol").toString() << "\n";
                qDebug() << (QString) rec.value("date").toString() << "\n";

                // Get data stored in database
                price = rec.value("adjClose").toDouble();
                timeCnt++; // Note: started on -1


                switch(state)
                {
                case AVGSTATE_PREPARE:
                    // Collect data so we are able to calc first mean
                    if(initMinMaxPrice == true)
                    {
                        initMinMaxPrice = false;
                        minPrice = price;
                        maxPrice = price;
                    }
                    else
                    {
                        if (price < minPrice )
                        {
                            minPrice = price;
                        }

                        if( price > maxPrice)
                        {
                            maxPrice = price;
                        }
                    }

                    Enqueue(&m_Queue, price);  // Add data to queue

                    if(i == timePeriod - 1)
                    {
                        // %K = 100 * (price - minPrice) / (maxPrice - minPrice)
                        procD = procK = 100 * (price - minPrice) / (maxPrice - minPrice);

                        // %K
                        stockData.data.indicator1.insert(stockDataIndex, procK);

                        // %D = %K
                        stockData.data.indicator2.insert(stockDataIndex, procD);
                        // limits
                        stockData.data.indicator3.insert(stockDataIndex, 20);
                        stockData.data.indicator4.insert(stockDataIndex, 80);


                        stockData.data.x.insert(stockDataIndex, timeCnt);
                        if(false == rec.value("date").isNull())
                        {
                            date = rec.value("date").toString();
                            qDebug() << "b1 date" << date;
                            stockData.data.xDate.insert(stockDataIndex, date);
                            ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, procK, date);
                            ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, procD, date);
                            ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, 20, date);
                            ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, 80, date);
                        }
                        else
                        {
                            ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, procK);
                            ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, procD);
                            ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, 20);
                            ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, 80);
                        }

                        stockDataIndex++;
                        state = AVGSTATE_CALC;
                    }
                    i++;
                    break;

                 case AVGSTATE_CALC:
                    // Store new price in queue and remove oldest price
                    Enqueue(&m_Queue, price);			// Add data to queue
                    Dequeue(&m_Queue, oldPrice);         // Remove one data from queue

                    // Check if oldest price is the min or max price. If that is the case
                    // update min and max price with use of current legal data
                    if (oldPrice == minPrice || oldPrice == maxPrice)
                    {
                        minPrice = price;
                        maxPrice = price;

                        for( i = 0; i < timePeriod; i++)
                        {
                            Dequeue(&m_Queue, oldPrice); // Remove data from queue
                            Enqueue(&m_Queue, oldPrice);  // Add data to queue

                            if (oldPrice < minPrice )
                                minPrice = oldPrice;

                            if( oldPrice > maxPrice)
                                maxPrice = oldPrice;
                        }

                    }
                    else if( price > maxPrice)
                    {
                        maxPrice = price;
                    }
                    else if ( price < minPrice)
                    {
                        minPrice = price;
                    }

                    // %K = 100 * (price - minPrice) / (maxPrice - minPrice)
                    rawK = 100 * (price - minPrice) / (maxPrice - minPrice);
                    procK = procK + (expProc * (rawK - procK));
                    stockData.data.indicator1.insert(stockDataIndex, procK);

                    // %D  (signal line)
                    procD = procD + (expProc * (procK - procD));
                    stockData.data.indicator2.insert(stockDataIndex, procD);

                    stockData.data.indicator3.insert(stockDataIndex, 20);
                    stockData.data.indicator4.insert(stockDataIndex, 80);

                    stockData.data.x.insert(stockDataIndex, timeCnt);
                    if(false == rec.value("date").isNull())
                    {
                        date = rec.value("date").toString();
                        qDebug() << "b1 date" << date;
                        stockData.data.xDate.insert(stockDataIndex, date);
                        ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, procK, date);
                        ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, procD, date);
                    }
                    else
                    {
                        ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, procK);
                        ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, procD);
                    }

                    stockDataIndex++;
                    break;
                }
            }
        }
    }

    qDebug() << "len x" << stockData.data.x.size();
    qDebug() << "len i1" << stockData.data.indicator1.size();
    qry.finish();
    closeDb();

    // Empty FIFO
    while( !EmptyQ(&m_Queue))
    {
        Dequeue(&m_Queue, price);
    }
    m_mutex.unlock();

    if(state == AVGSTATE_CALC)
    {
        return true;
    }

    return false;
}






/*****************************************************************
 *
 * Function:		getYahooTaPriceData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::getYahooTaMovingAverage(const QString assetSymbol,
                                       const QString startDate,
                                       const QString endDate,
                                       const double dbTimePeriod,
                                       CYahooStockPlotUtil::PlotData_ST &qwtStockPlotData,
                                       CYahooStockPlotUtil::StockData_ST &stockData)
{

    QSqlRecord rec;
    QString str;
    CYahooStockPlotUtil ypu;
    StateMovingAverage_ET state;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);

    // Data from database
    // Lets start -1 so we can handle price and time on same loaction
    int timeCnt = -1;
    double price;
    double sumOfPrice;
    double average;
    QString date;

    // Fifo help variable
    double oldPrice;
    int i;

    // Number of stored plot data
    int stockDataIndex;

    // Initilize que used to calc moving average data
    initializeQ(&m_Queue);
    int timePeriod = (unsigned long) dbTimePeriod;


    QByteArray ba = assetSymbol.toLocal8Bit();
    const char *c_assetSymbol = ba.data();

    QByteArray ba1 = startDate.toLocal8Bit();
    const char *c_startDate = ba1.data();

    QByteArray ba2 = endDate.toLocal8Bit();
    const char *c_endDate = ba2.data();


    str.sprintf("SELECT TblYahooTaStockName.assetNameId, TblYahooTaStockName.assetName, TblYahooTaStockName.assetSymbol, "
                " TblYahooTaData.date, TblYahooTaData.adjClose, TblYahooTaData.assetNameId "
                " FROM  TblYahooTaStockName, TblYahooTaData "
                " WHERE TblYahooTaStockName.assetNameId = TblYahooTaData.assetNameId AND "
                " TblYahooTaStockName.assetSymbol = '%s' AND "
                " TblYahooTaData.date >= '%s' AND TblYahooTaData.date <= '%s' "
                " ORDER BY TblYahooTaData.date ASC;",
                c_assetSymbol, c_startDate, c_endDate);

    qDebug() << "str =" << str << "\n";

    qry.prepare(str);

    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }

        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {

        // Contains nof data displayed in plot
        qwtStockPlotData.nofStocksToPlot = 0;

        // This variable is automaticly updated when first data is handled by pdateMinMaxAxis()
        // qwtStockPlotData.axis.minMaxIsInit = false;

        // This counter is needed until FIFO contains enought data
        i = 0;

        // Number of data to plot
        stockDataIndex = 0;

        sumOfPrice = 0;
        state = AVGSTATE_PREPARE;




        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("assetSymbol").isNull() == true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                qDebug() << (QString) rec.value("adjClose").toString() << "\n";
                qDebug() << (QString) rec.value("assetName").toString() << "\n";
                qDebug() << (QString) rec.value("assetSymbol").toString() << "\n";
                qDebug() << (QString) rec.value("date").toString() << "\n";

                // Get data stored in database
                price = rec.value("adjClose").toDouble();
                timeCnt++; // Note: started on -1


                switch(state)
                {
                case AVGSTATE_PREPARE:
                    // Collect data so we are able to calc first mean
                    if(i < timePeriod)
                    {
                        Enqueue(&m_Queue, price);  // Add data to queue
                        sumOfPrice += price; 	  // Sum as many data as time period says

                        if(i == timePeriod - 1)
                        {
                            calcMovingAverage(sumOfPrice, timePeriod, average);

                            stockData.data.indicator1.insert(stockDataIndex, average);
                            stockData.data.x.insert(stockDataIndex, timeCnt);
                            if(false == rec.value("date").isNull())
                            {
                                date = rec.value("date").toString();
                                qDebug() << "b1 date" << date;
                                stockData.data.xDate.insert(stockDataIndex, date);
                                ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, average, date);
                            }
                            else
                            {
                                ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, average);
                            }

                            stockDataIndex++;
                            state = AVGSTATE_CALC;
                        }
                    }
                    i++;
                    break;

                case AVGSTATE_CALC:
                    // Remove one data from queue
                    Dequeue(&m_Queue, oldPrice);
                    sumOfPrice -= oldPrice;

                    // Add new price to queue
                    Enqueue(&m_Queue, price);
                    sumOfPrice += price;

                    calcMovingAverage(sumOfPrice, timePeriod, average);

                    stockData.data.indicator1.insert(stockDataIndex, average);
                    stockData.data.x.insert(stockDataIndex, timeCnt);
                    if(false == rec.value("date").isNull())
                    {
                        date = rec.value("date").toString();
                        qDebug() << "b1 date" << date;
                        stockData.data.xDate.insert(stockDataIndex, date);
                        ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, average, date);
                    }
                    else
                    {
                        ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, average);
                    }


                    stockDataIndex++;
                    break;
                }
            }
        }
    }

    qDebug() << "len x" << stockData.data.x.size();
    qDebug() << "len i1" << stockData.data.indicator1.size();
    qry.finish();
    closeDb();

    // Empty FIFO
    while( !EmptyQ(&m_Queue))
    {
        Dequeue(&m_Queue, price);
    }
    m_mutex.unlock();

    if(state == AVGSTATE_CALC)
    {
        return true;
    }

    return false;
}




/*****************************************************************
 *
 * Function:		getYahooTaPriceData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::getYahooTaMomentum(const QString assetSymbol,
                                       const QString startDate,
                                       const QString endDate,
                                       const double dbTimePeriod,
                                       CYahooStockPlotUtil::PlotData_ST &qwtStockPlotData,
                                       CYahooStockPlotUtil::StockData_ST &stockData)
{

    QSqlRecord rec;
    QString str;
    CYahooStockPlotUtil ypu;
    StateMovingAverage_ET state;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);

    // Data from database
    // Lets start -1 so we can handle price and time on same loaction
    int timeCnt = -1;
    double price;
    double oldPrice;
    double momentum;
    QString date;



    // Fifo help variable
    int i;

    // Number of stored plot data
    int stockDataIndex;

    // Initilize que used to calc moving average data
    initializeQ(&m_Queue);
    int timePeriod = (unsigned long) dbTimePeriod;


    QByteArray ba = assetSymbol.toLocal8Bit();
    const char *c_assetSymbol = ba.data();

    QByteArray ba1 = startDate.toLocal8Bit();
    const char *c_startDate = ba1.data();

    QByteArray ba2 = endDate.toLocal8Bit();
    const char *c_endDate = ba2.data();


    str.sprintf("SELECT TblYahooTaStockName.assetNameId, TblYahooTaStockName.assetName, TblYahooTaStockName.assetSymbol, "
                " TblYahooTaData.date, TblYahooTaData.adjClose, TblYahooTaData.assetNameId "
                " FROM  TblYahooTaStockName, TblYahooTaData "
                " WHERE TblYahooTaStockName.assetNameId = TblYahooTaData.assetNameId AND "
                " TblYahooTaStockName.assetSymbol = '%s' AND "
                " TblYahooTaData.date >= '%s' AND TblYahooTaData.date <= '%s' "
                " ORDER BY TblYahooTaData.date ASC;",
                c_assetSymbol, c_startDate, c_endDate);

    qDebug() << "str =" << str << "\n";

    qry.prepare(str);

    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }

        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {

        // Contains nof data displayed in plot
        qwtStockPlotData.nofStocksToPlot = 0;

        // This variable is automaticly updated when first data is handled by pdateMinMaxAxis()
        qwtStockPlotData.axis.minMaxIsInit = false;

        // This counter is needed until FIFO contains enought data
        i = 0;

        // Number of data to plot
        stockDataIndex = 0;

        momentum = 0;
        state = AVGSTATE_PREPARE;




        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("assetSymbol").isNull() == true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                qDebug() << (QString) rec.value("adjClose").toString() << "\n";
                qDebug() << (QString) rec.value("assetName").toString() << "\n";
                qDebug() << (QString) rec.value("assetSymbol").toString() << "\n";
                qDebug() << (QString) rec.value("date").toString() << "\n";

                // Get data stored in database
                price = rec.value("adjClose").toDouble();
                timeCnt++; // Note: started on -1


                switch(state)
                {
                case AVGSTATE_PREPARE:
                    // Collect data so we are able to calc first mean
                    if(i < timePeriod)
                    {
                        Enqueue(&m_Queue, price);  // Add data to queue

                        if(i == timePeriod - 1)
                        {
                            Dequeue(&m_Queue, oldPrice);
                            momentum = price - oldPrice;
                            stockData.data.indicator1.insert(stockDataIndex, momentum);
                            // Zero line
                            stockData.data.indicator2.insert(stockDataIndex, 0);
                            stockData.data.x.insert(stockDataIndex, timeCnt);
                            if(false == rec.value("date").isNull())
                            {
                                date = rec.value("date").toString();
                                qDebug() << "b1 date" << date;
                                stockData.data.xDate.insert(stockDataIndex, date);
                                ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, momentum, date);
                                ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, 0, date);
                            }
                            else
                            {
                                ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, momentum);
                            }

                            stockDataIndex++;
                            state = AVGSTATE_CALC;
                        }
                    }
                    i++;
                    break;

                case AVGSTATE_CALC:
                    // Remove one data from queue
                    Dequeue(&m_Queue, oldPrice);

                    // Add new price to queue
                    Enqueue(&m_Queue, price);

                    momentum = price - oldPrice;
                    stockData.data.indicator1.insert(stockDataIndex, momentum);
                    stockData.data.indicator2.insert(stockDataIndex, 0);
                    stockData.data.x.insert(stockDataIndex, timeCnt);
                    if(false == rec.value("date").isNull())
                    {
                        date = rec.value("date").toString();
                        qDebug() << "b1 date" << date;
                        stockData.data.xDate.insert(stockDataIndex, date);
                        ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, momentum, date);
                    }
                    else
                    {
                        ypu.updateMinMaxAxis(qwtStockPlotData.axis, (double) timeCnt, momentum);
                    }


                    stockDataIndex++;
                    break;
                }
            }
        }
    }

    qDebug() << "lem x" << stockData.data.x.size();
    qDebug() << "len i1" << stockData.data.indicator1.size();
    qry.finish();
    closeDb();

    // Empty FIFO
    while( !EmptyQ(&m_Queue))
    {
        Dequeue(&m_Queue, price);
    }
    m_mutex.unlock();

    if(state == AVGSTATE_CALC)
    {
        return true;
    }

    return false;
}






/*****************************************************************
 *
 * Function:		delAllTblAvanzaAccount()
 *
 * Description:		This function delete all data
 *
 *
 *
 *****************************************************************/
bool CDbHndl::delAllTblAvanzaAccount(void)
{
    QString str;
    // QSqlQuery qry;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    // Create sql question
    str.sprintf("DELETE "
                "FROM TblAvanzaAccount;");

    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 1a"), qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
        qDebug( "Inserted!" );

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}


/*****************************************************************
 *
 * Function:		delAllTblAvanzaAssetName()
 *
 * Description:		This function delete all data
 *
 *
 *
 *****************************************************************/
bool CDbHndl::delAllTblAvanzaAssetName(void)
{
    QString str;
    // QSqlQuery qry;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    // Create sql question
    str.sprintf("DELETE "
                "FROM TblAvanzaAssetName;");

    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 1b"), qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
        qDebug( "Inserted!" );

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}





/*****************************************************************
 *
 * Function:		delAllTblAvanzaAssetData()
 *
 * Description:		This function delete all data
 *
 *
 *
 *****************************************************************/
bool CDbHndl::delAllTblAvanzaAssetData(void)
{
    QString str;
    // QSqlQuery qry;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    // Create sql question
    str.sprintf("DELETE "
                "FROM TblAvanzaAssetData;");

    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 1b"), qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
        qDebug( "Inserted!" );

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}


/****************************************************************
 *
 * Function:    findAvanzaAccount()
 *
 * Description: Check if account exist.
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::findAvanzaAccount(int account)
{
    QSqlRecord rec;
    // QSqlQuery qry;
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    str.sprintf("SELECT account "
                " FROM TblAvanzaAccount;");

    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        if(qry.next())
        {
            rec = qry.record();

            if(rec.value("account").isNull()==true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                if(account == rec.value("account"))
                {
                    qry.finish();
                    closeDb();
                    m_mutex.unlock();
                    return true;
                }
            }
        }
    }

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return false;
}




/****************************************************************
 *
 * Function:    findAvanzaAssetName()
 *
 * Description: Check if account exist.
 *              If this is the case extract
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::findAvanzaAssetName(int account, QString assetName)
{
    QSqlRecord rec;
    // QSqlQuery qry;
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);

    qDebug() << "assetName.toLocal8Bit().constData() =" << assetName << "\n";
    qDebug() << "account =" << account << "\n";


    QByteArray ba = assetName.toLocal8Bit();
    const char *c_assetName = ba.data();


    str.sprintf("SELECT TblAvanzaAssetName.accountId AS account1, TblAvanzaAssetName.assetName, TblAvanzaAccount.account "
                " FROM TblAvanzaAccount, TblAvanzaAssetName "
                " WHERE TblAvanzaAccount.account = %d AND TblAvanzaAssetName.accountId = %d AND TblAvanzaAssetName.assetName = '%s';",
                account, account, c_assetName);

    qDebug() << "str =" << str << "\n";


    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        if(qry.next())
        {
            rec = qry.record();

            qDebug() << (QString) rec.value("account").toString() << "\n";
            qDebug() << (QString) rec.value("account1").toString() << "\n";
            qDebug() << (QString) rec.value("assetName").toString() << "\n";

            if(rec.value("assetName").isNull() == true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return true;
            }
        }
    }

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return false;
}


/****************************************************************
 *
 * Function:    getTotNofAvanzaPortoflioRecords()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::getTotNofAvanzaPortfolioRecords(int &nofRow)
{
    //QSqlQuery qry;
    QString str;

    nofRow = 0;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);
    str.sprintf("SELECT Count(*) FROM TblAvanzaAssetData;");

    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 8a"), (QString) qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }


    if (qry.next())
    {
        nofRow = qry.value(0).toInt();
    }
    else
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 9a"), QString::fromUtf8("No data received"));
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }

    qDebug("Inserted!");
    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;

}




/****************************************************************
 *
 * Function:    findAvanzaAccount()
 *
 * Description: Check if account exist.
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::listAllAvanzaPortfolioRecords(QObject *thisPointer, int rows, int cols, QTableView *tableView)
{
    CExtendedTable et;
    QSqlRecord rec;
    // QSqlQuery qry;
    QString str;
    int colCnt;
    int rowCnt;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    // AS assetName1,
    str.sprintf("SELECT TblAvanzaAccount.account, "
                "TblAvanzaAssetName.assetName AS assetName1, "
                "TblAvanzaAssetName.accountId, "
                "TblAvanzaAssetData.date, "
                "TblAvanzaAssetData.typeOfData, "
                "TblAvanzaAssetData.assetPrice, "
                "TblAvanzaAssetData.currency, "
                "TblAvanzaAssetData.assetName "
                " FROM TblAvanzaAccount, TblAvanzaAssetName, TblAvanzaAssetData "
                " WHERE TblAvanzaAccount.account = TblAvanzaAssetName.accountId AND "
                "TblAvanzaAssetName.assetName = TblAvanzaAssetData.assetName "
                " ORDER BY TblAvanzaAccount.account, TblAvanzaAssetData.date;");


    qDebug() << str << "\n";
    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), (QString)qry.lastError().text().toLocal8Bit().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {

        et.createTableModel(rows, cols, thisPointer);
        colCnt = 0;
        rowCnt = 0;

        while(qry.next())
        {
            rec = qry.record();
            if(rec.value("account").isNull() == true)
            {
                if(m_disableMsgBoxes == false)
                {
                    QMessageBox::critical(NULL, QString::fromUtf8("Aktie list"), QString::fromUtf8("Aktie listan saknas"));
                }
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                str = rec.value("account").toString();
                et.addData(tableView, str, rowCnt, colCnt);
                colCnt++;

                str = rec.value("assetName1").toString();
                et.addData(tableView, str, rowCnt, colCnt);
                colCnt++;

                str = rec.value("date").toString();
                et.addData(tableView, str, rowCnt, colCnt);
                colCnt++;

                str = rec.value("typeOfData").toString();
                et.addData(tableView, str, rowCnt, colCnt);
                colCnt++;

                str = rec.value("assetPrice").toString();
                et.addData(tableView, str, rowCnt, colCnt);
                colCnt++;

                str = rec.value("currency").toString();
                et.addData(tableView, str, rowCnt, colCnt);
                colCnt = 0;
                rowCnt++;

            }
        }
    }

    tableView->resizeColumnsToContents();

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}





/****************************************************************
 *
 * Function:    findAvanzaAccount()
 *
 * Description: Check if account exist.
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::findAvanzaData(int account, QString assetName, QString date)
{
    QSqlRecord rec;
    //QSqlQuery qry;
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);

    QByteArray ba = assetName.toLocal8Bit();
    const char *c_assetName = ba.data();

    QByteArray ba1 = date.toLocal8Bit();
    const char *c_date = ba1.data();


    str.sprintf("SELECT TblAvanzaAccount.account, TblAvanzaAssetName.accountId, TblAvanzaAssetName.assetName,  "
                "TblAvanzaAssetData.assetName, TblAvanzaAssetData.date  "
                " FROM TblAvanzaAccount, TblAvanzaAssetName, TblAvanzaAssetData "
                "WHERE TblAvanzaAccount.account = TblAvanzaAssetName.accountId AND "
                "TblAvanzaAssetName.assetName = TblAvanzaAssetData.assetName AND "
                "TblAvanzaAccount.account = %d AND "
                "TblAvanzaAssetName.assetName = '%s' AND "
                "TblAvanzaAssetData.date = '%s';", account, c_assetName, c_date);


    qDebug() << str << "\n";

    qry.prepare(str);

    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), (QString) qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        if(qry.next())
        {
            rec = qry.record();
            if(rec.value("account").isNull()==true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return true;
            }
        }
    }

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return false;
}




/****************************************************************
 *
 * Function:    insertAvanzaAssetData()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertAvanzaAssetData(QString account,
                      QString assetName,
                      QString date,
                      QString typeOfData,
                      QString assetPrice,
                      QString currency)
{

    CUtil cu;
    int accountInt;
    int typeOfDataId;
    double assetPriceDouble;


    qDebug() << "account =" << account << "\n";
    qDebug() << "assetName =" << assetName << "\n";
    qDebug() << "date =" << date << "\n";
    qDebug() << "typeOfData =" << typeOfData << "\n";
    qDebug() << "assetPrice =" << assetPrice << "\n";
    qDebug() << "currency =" << currency << "\n";



    if(cu.number2Int(account, accountInt) == false)
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Convert Avanza account"), QString::fromUtf8("Fail to convert Avanza account from string to int"));
        }
        return false;
    }

    typeOfDataId = getAvanzaTypeOfDataId(typeOfData);

    if(findAvanzaAccount(accountInt) == false)
    {
        if(false == insertAvanzaAccount(accountInt))
        {
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("Insert Avanza account"), QString::fromUtf8("Fail to insert Avvanza account data"));
            }
            return false;
        }
    }


    if(false == findAvanzaAssetName(accountInt, assetName))
    {

        if(false == insertAvanzaAssetName(accountInt, assetName))
        {
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("Insert Avanza Asset Name"), QString::fromUtf8("Fail to insert Avanza Asset Name"));
            }
            return false;
        }
        if(false == cu.number2double(assetPrice, assetPriceDouble))
        {
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("convert Avanza price"), QString::fromUtf8("Fail to convert Avvanza price to double"));
            }
            return false;
        }
        if(false == findAvanzaData(accountInt, assetName, date))
        {
            if(false == insertAvanzaAssetData(date, typeOfData, typeOfDataId, assetPriceDouble, currency, assetName))
            {
                if(m_disableMsgBoxes == false)
                {
                    QMessageBox::critical(NULL, QString::fromUtf8("Insert Avanza asset data"), QString::fromUtf8("Fail to insert Avvanza asset data"));
                }
                return false;
            }
        }
    }
    else
    {
        if(false == findAvanzaData(accountInt, assetName, date))
        {
            if(false == cu.number2double(assetPrice, assetPriceDouble))
            {
                if(m_disableMsgBoxes == false)
                {
                    QMessageBox::critical(NULL, QString::fromUtf8("convert Avanza price"), QString::fromUtf8("Fail to convert Avvanza price to double"));
                }
                return false;
            }
            if(false == insertAvanzaAssetData(date, typeOfData, typeOfDataId, assetPriceDouble, currency, assetName))
            {
                if(m_disableMsgBoxes == false)
                {
                    QMessageBox::critical(NULL, QString::fromUtf8("Insert Avanza asset data"), QString::fromUtf8("Fail to insert Avvanza asset data"));
                }
                return false;
            }
            else
            {
                return true;
            }
        }
    }

    return false;

}




/****************************************************************
 *
 * Function:    getAvanzaTypeOfDataId()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
int CDbHndl::getAvanzaTypeOfDataId(QString typeOfData)
{

    if(QString::fromUtf8("Utdelning") == typeOfData.toLocal8Bit().constData())
    {
        return AVANZA_TYPE_DATA_DIVIDENDE;
    }
    else if(QString::fromUtf8("Köp") == typeOfData.toLocal8Bit().constData())
    {
        return AVANZA_TYPE_DATA_BUYING_PRICE;
    }
    else if(QString::fromUtf8("Sälj") == typeOfData.toLocal8Bit().constData())
    {
        return AVANZA_TYPE_DATA_SELLING_PRICE;
    }
    else if(QString::fromUtf8("Insättning") == typeOfData.toLocal8Bit().constData())
    {
        return AVANZA_TYPE_DATA_MONEY_INSERTION;
    }

    return AVANZA_TYPE_DATA_OTHER;

}


/****************************************************************
 *
 * Function:    insertAvanzaAssetData()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertAvanzaAssetData(QString date,
                      QString typeOfData,
                      int     typeOfDataId,
                      double assetPrice,
                      QString currency,
                      QString assetName)
{
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);

    QByteArray ba = assetName.toLocal8Bit();
    const char *c_assetName = ba.data();

    QByteArray ba1 = currency.toLocal8Bit();
    const char *c_currency = ba1.data();




    str.sprintf("INSERT INTO TblAvanzaAssetData "
                "(date, typeOfData, typeOfDataId, assetPrice, currency, assetName) "
                " VALUES('%s', '%s', %d, %g, '%s', '%s');",
                date.toLocal8Bit().constData(),
                typeOfData.toLocal8Bit().constData(),
                typeOfDataId,
                assetPrice,
                c_currency,
                c_assetName);


    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 20"), qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;

}


/****************************************************************
 *
 * Function:    insertAvanzaAssetData()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertAvanzaAssetName(int account, QString assetName)
{
    // QSqlQuery qry;
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);

    QByteArray ba = assetName.toLocal8Bit();
    const char *c_assetName = ba.data();


    str.sprintf("INSERT INTO TblAvanzaAssetName "
                "(assetName, accountId) "
                " VALUES('%s', %d);", c_assetName, account);

    qDebug() << str << "\n";

    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 20"), qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;

}



/****************************************************************
 *
 * Function:    insertAvanzaAssetData()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertAvanzaAccount(int account)
{
    // QSqlQuery qry;
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    str.sprintf("INSERT INTO TblAvanzaAccount "
                "(account) "
                " VALUES(%d);", account);


    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 20"), qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;

}







//==========================

/*****************************************************************
 *
 * Function:		delAllDtcSwVerPageData()
 *
 * Description:		This function delete all data in temp list
 *
 *
 *
 *****************************************************************/
bool CDbHndl::delAllTaStockLists(void)
{
    QString str;
    // QSqlQuery qry;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    // Create sql question
    str.sprintf("DELETE "
                "FROM TblTaStockList;");

    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 1"), qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
        qDebug() << "All TblTaStockList data are deleted\n";

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}


/*****************************************************************
 *
 * Function:		delAllDtcSwVerPageData()
 *
 * Description:		This function delete all data in temp list
 *
 *
 *
 *****************************************************************/
bool CDbHndl::delAllTaStockData(void)
{
    QString str;
    // QSqlQuery qry;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    // Create sql question
    str.sprintf("DELETE "
                "FROM TblTaStockData;");

    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 1"), qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
        qDebug() << "All TblTaStockList data are deleted\n";

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}






/****************************************************************
 *
 * Function:    addStockSymbolAndNameInTreeWidget()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::addStockListsToComboBox(QComboBox *comboBox)
{
    QSqlRecord rec;
    QString str;
    int index;


    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    str.sprintf("SELECT stockListName "
                " FROM TblTaStockList"
                " ORDER BY LOWER(stockListName) %s;",
                SQL_STR_ASC);


    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        index = comboBox->currentIndex();
        comboBox->clear();
        qDebug( ) << "Clear stocklist combobox";
        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("stockListName").isNull()==true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                comboBox->addItem((QString)rec.value("stockListName").toString().toLatin1());
                qDebug( ) << rec.value("stockListName").toString().toLatin1();
            }
        }
    }

    comboBox->setCurrentIndex(index);
    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}




/****************************************************************
 *
 * Function:    addStockSymbolAndNameInTreeWidget()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::addStockSymbolAndNameInTreeWidget(QTreeWidget *treeWidget,
                                                int stockListId,
                                                CDbHndl::SortSymbolNameTreeWidget_ET sortOn,
                                                char *sortOrder)
{
    QSqlRecord rec;
    QString str;
    bool found1 = false;
    bool found2 = false;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);



    if(sortOn == SORT_TWIDGET_SYMBOL)
    {
        str.sprintf("SELECT taStockListId, stockSymbol, stockName "
                    " FROM TblTaStockName"
                    " WHERE taStockListId = '%d' "
                    " ORDER BY LOWER(stockSymbol) %s;",
                    stockListId,
                    sortOrder);
    }
    else
    {
        str.sprintf("SELECT taStockListId, stockSymbol, stockName "
                    " FROM TblTaStockName"
                    " WHERE taStockListId = '%d' "
                    " ORDER BY LOWER(stockName) %s;",
                    stockListId,
                    sortOrder);
    }

    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("stockSymbol").isNull()==true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                found1 = true;
            }

            if(rec.value("stockName").isNull()==true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                found2 = true;
            }

            if(found1 == true && found2 == true)
            {
                found1 = false;
                found2 = false;

                QTreeWidgetItem *item = new QTreeWidgetItem;
                item->setText(0, (QString)rec.value("stockName").toString());
                item->setText(1, (QString)rec.value("stockSymbol").toString());
                treeWidget->addTopLevelItem(item);
            }


        }
    }

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}




/****************************************************************
 *
 * Function:    addStockSymbolAndNameInTreeWidget1()
 *
 * Description: This combobox has a checkbox
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::addStockSymbolAndNameInTreeWidget1(QTreeWidget *treeWidget,
                                                 QVector<CDbHndl::EfficPortStockData_ST> &efficPortStockArr,
                                                 int stockListId,
                                                 CDbHndl::SortSymbolNameTreeWidget_ET sortOn,
                                                 char *sortOrder)
{
    QSqlRecord rec;
    QString str;
    CDbHndl::EfficPortStockData_ST data;
    bool found1 = false;
    bool found2 = false;
    int i = 0;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);





    if(sortOn == SORT_TWIDGET_SYMBOL)
    {
        str.sprintf("SELECT taStockListId, stockSymbol, stockName "
                    " FROM TblTaStockName"
                    " WHERE taStockListId = '%d' "
                    " ORDER BY LOWER(stockSymbol) %s;",
                    stockListId,
                    sortOrder);
    }
    else
    {
        str.sprintf("SELECT taStockListId, stockSymbol, stockName "
                    " FROM TblTaStockName"
                    " WHERE taStockListId = '%d' "
                    " ORDER BY LOWER(stockName) %s;",
                    stockListId,
                    sortOrder);
    }

    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        efficPortStockArr.clear();

        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("stockSymbol").isNull()==true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                found1 = true;
            }

            if(rec.value("stockName").isNull()==true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                found2 = true;
            }

            if(found1 == true && found2 == true)
            {
                found1 = false;
                found2 = false;

                QTreeWidgetItem *item = new QTreeWidgetItem;
                item->setText(0, (QString)rec.value("stockName").toString().toLatin1());
                item->setText(1, (QString)rec.value("stockSymbol").toString().toLatin1());
                item->setExpanded(true);
                               item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
                               item->setCheckState(2, Qt::Unchecked);
                               item->setCheckState(3, Qt::Unchecked);
                               item->setText(2, " ");
                               item->setText(3, " ");
                treeWidget->addTopLevelItem(item);

                data.stockName = (QString)rec.value("stockName").toString().toLatin1();
                data.stockSymbol = (QString)rec.value("stockSymbol").toString().toLatin1();
                efficPortStockArr.insert(i, data);
                i++;
            }
        }
    }

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}





/****************************************************************
 *
 * Function:    addStockSymbolAndNameInTreeWidget1()
 *
 * Description: This combobox has a checkbox
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::addStockSymbolAndNameInArr(QVector<CDbHndl::StockNameAndSymbolST> &stockNameAndSymbolArr,
                                                int stockListId,
                                                CDbHndl::SortSymbolNameTreeWidget_ET sortOn,
                                                char *sortOrder)
{
    QSqlRecord rec;
    QString str;
    bool found1 = false;
    bool found2 = false;
    StockNameAndSymbolST data;
    int i;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);



    if(sortOn == SORT_TWIDGET_SYMBOL)
    {
        str.sprintf("SELECT taStockListId, stockSymbol, stockName "
                    " FROM TblTaStockName"
                    " WHERE taStockListId = '%d' "
                    " ORDER BY LOWER(stockSymbol) %s;",
                    stockListId,
                    sortOrder);
    }
    else
    {
        str.sprintf("SELECT taStockListId, stockSymbol, stockName "
                    " FROM TblTaStockName"
                    " WHERE taStockListId = '%d' "
                    " ORDER BY LOWER(stockName) %s;",
                    stockListId,
                    sortOrder);
    }

    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        stockNameAndSymbolArr.clear();
        i = 0;

        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("stockSymbol").isNull()==true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                found1 = true;
            }

            if(rec.value("stockName").isNull()==true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                found2 = true;
            }

            if((found1 == true) && (found2 == true))
            {
                found1 = false;
                found2 = false;

                data.name = (QString)rec.value("stockName").toString().toLatin1();
                data.symbol = (QString)rec.value("stockSymbol").toString().toLatin1();
                stockNameAndSymbolArr.insert(i++, data);
            }
        }
    }

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}






/****************************************************************
 *
 * Function:    addStockSymbolAndNameInTreeWidget1()
 *
 * Description: This combobox has a checkbox
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::addStockSymbolAndNameInTreeWidget1(QTreeWidget *treeWidget,
                                                int stockListId,
                                                CDbHndl::SortSymbolNameTreeWidget_ET sortOn,
                                                char *sortOrder)
{
    QSqlRecord rec;
    QString str;
    bool found1 = false;
    bool found2 = false;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);



    if(sortOn == SORT_TWIDGET_SYMBOL)
    {
        str.sprintf("SELECT taStockListId, stockSymbol, stockName "
                    " FROM TblTaStockName"
                    " WHERE taStockListId = '%d' "
                    " ORDER BY LOWER(stockSymbol) %s;",
                    stockListId,
                    sortOrder);
    }
    else
    {
        str.sprintf("SELECT taStockListId, stockSymbol, stockName "
                    " FROM TblTaStockName"
                    " WHERE taStockListId = '%d' "
                    " ORDER BY LOWER(stockName) %s;",
                    stockListId,
                    sortOrder);
    }

    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("stockSymbol").isNull()==true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                found1 = true;
            }

            if(rec.value("stockName").isNull()==true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                found2 = true;
            }

            if(found1 == true && found2 == true)
            {
                found1 = false;
                found2 = false;

                QTreeWidgetItem *item = new QTreeWidgetItem;
                item->setText(0, (QString)rec.value("stockName").toString().toLatin1());
                item->setText(1, (QString)rec.value("stockSymbol").toString().toLatin1());
                item->setExpanded(true);
                               item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
                               item->setCheckState(2, Qt::Unchecked);
                               item->setText(2, " ");
                treeWidget->addTopLevelItem(item);
            }
        }
    }

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}



/****************************************************************
 *
 * Function:    findTaStockListId()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::findTaStockListId(QString stockListName, int &stockListId, bool dbIsHandledExternly)
{
    QSqlRecord rec;
    // QSqlQuery qry;
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

    str.sprintf("SELECT taStockListId, stockListName "
                " FROM TblTaStockList"
                " WHERE stockListName = '%s';",
                stockListName.toLocal8Bit().constData());

    qry.prepare(str);

    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }
    else
    {
        if(qry.next())
        {
            rec = qry.record();

            if(rec.value("taStockListId").isNull()==true)
            {
                qry.finish();
                if(dbIsHandledExternly == false)
                {
                    closeDb();
                    m_mutex.unlock();
                }
                return false;
            }
            else
            {
                stockListId = rec.value("taStockListId").toInt();
            }

        }
    }

    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }
    return true;
}



/****************************************************************
 *
 * Function:    addTaStockList()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::addTaStockList(QString stockListName)
{
    // QSqlQuery qry;
    QString str;
    // QString debugStr;


    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);

    str.sprintf("INSERT INTO TblTaStockList "
                "( "
                "stockListName "
                ") "
                "VALUES("
                "'%s');",
                stockListName.toLocal8Bit().constData());


    qDebug() << str << "\n";
    qry.prepare(str);

    if( !qry.exec() )
    {
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;

}


/*****************************************************************
 *
 * Function:		delAllDtcSwVerPageData()
 *
 * Description:		This function delete all data in temp list
 *
 *
 *
 *****************************************************************/
bool CDbHndl::delAllTaStockNames(void)
{
    QString str;
    // QSqlQuery qry;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    // Create sql question
    str.sprintf("DELETE "
                "FROM TblTaStockName;");

    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 1"), qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
        qDebug( "Inserted!" );

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}



/****************************************************************
 *
 * Function:    addTaStockName()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::addTaStockName(QString stockSymbol, QString stockName, int stockListId, bool dbIsHandledExternly)
{
    QString str;


    if(dbIsHandledExternly ==false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);


    str.sprintf("INSERT INTO TblTaStockName "
                "( "
                "stockSymbol, "
                "stockName, "
                "taStockListId "
                ") "
                "VALUES( "
                "'%s',"
                "'%s',"
                "%d);",
                stockSymbol.toLocal8Bit().constData(),
                stockName.toLocal8Bit().constData(),
                stockListId);

    qry.prepare(str);

    if( !qry.exec() )
    {
        qDebug() << qry.lastError();

        if(dbIsHandledExternly ==false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    qry.finish();
    if(dbIsHandledExternly ==false)
    {
        closeDb();
        m_mutex.unlock();
    }
    return true;
}



/****************************************************************
 *
 * Function:    execSingleCmd()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::addTaData(QString stockList,
                        QString name,
                        QString  date,
                        QString open,
                        QString high,
                        QString low,
                        QString close,
                        QString volume,
                        QString adjClose)
{
    // QSqlQuery qry;
    QString str;
    // QString debugStr;



    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);

    str.sprintf("UPDATE TblTaStockList, TblTaStockName, TblTaStockData "
                "TblTaStockList.stockListName = '%s',"
                "TblTaStockName.stockName = %s,"
                /*     "TblTaStockData.date = date('%Y-%m-%d', strtotime('%s'))," */
                "TblTaStockData.date = '%s'',"
                "TblTaStockData.open = %g,"
                "TblTaStockData.high = %g,"
                "TblTaStockData.low = %g,"
                "TblTaStockData.close = %g,"
                "TblTaStockData.volume = %g,"
                "TblTaStockData.adjClose = %g "
                " WHERE TblTaStockName.taStockListId = TblTaStockList.taStockListId AND "
                " TblTaStockData.taStockNameId = TblTaStockName.taStockNameId;",
                stockList.toLocal8Bit().constData(),
                name.toLocal8Bit().constData(),
                date.toStdString().c_str(),
                /*date.toStdString().c_str(),*/
                atof(open.toStdString().c_str()),
                atof(high.toStdString().c_str()),
                atof(low.toStdString().c_str()),
                atof(close.toStdString().c_str()),
                atof(volume.toStdString().c_str()),
                atof(adjClose.toStdString().c_str()));

    qry.prepare(str);

    if( !qry.exec() )
    {
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }

    closeDb();
    qry.finish();
    m_mutex.unlock();
    return true;

}




/****************************************************************
 *
 * Function:    execSingleCmd()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::execSingleCmd(QSqlQuery qry)
{

    if( !qry.exec() )
    {
        qDebug() << qry.lastError();
        return false;
    }
    else
    {
        //qDebug() << "Table created!";
    }

    return true;
}


/*****************************************************************
 *
 * Function:		getAllSnapshotPlotStocksData()
 *
 * Description:		This function extract all stocks from the database
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getAllSnapshotPlotStocksData(QVector <CStockPlotUtil::StockData_ST> &stockArr)
{
    QSqlRecord rec;
    // QSqlQuery qry;
    QString str;
    bool found;
    bool found1;
    CStockPlotUtil::StockData_ST data;
    int i;
    int j;


    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);

    str.sprintf("SELECT companyName,"
                "procentChangeOneDay,"
                "procentChangeOneWeek,"
                "procentChange1Month,"
                "procentChange3Month,"
                "procentChange6Month,"
                "procentChange1Year,"
                "procentChange2Year,"
                "procentChange3Year,"
                "procentChange5Year"
                " FROM TblStockDataSnapshot "
                " ORDER BY companyName;");

    qry.prepare(str);

    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
    }
    else
    {

        i = 0;
        j = 0;

        while(qry.next())
        {
            found = false;
            found1 = false;
            rec = qry.record();

            if(rec.value("companyName").isNull() == true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                found1 = true;
                data.stockName = rec.value("companyName").toString();
            }


            if(rec.value("procentChangeOneDay").isNull() != true)
            {
                found = true;
                data.data.y.insert(j, rec.value("procentChangeOneDay").toDouble());
                data.data.x.insert(j, 0); // (0) (time)
                j++;
            }



            if(rec.value("procentChangeOneWeek").isNull() != true)
            {
                found = true;
                data.data.y.insert(j, rec.value("procentChangeOneWeek").toDouble());
                data.data.x.insert(j, 7); // (0) (time)
                j++;
            }

            if(rec.value("procentChange1Month").isNull() != true)
            {
                found = true;
                data.data.y.insert(j, rec.value("procentChange1Month").toDouble());
                data.data.x.insert(j, 30); // (0) (time)
                j++;
            }


            if(rec.value("procentChange3Month").isNull() != true)
            {
                found = true;
                data.data.y.insert(j, rec.value("procentChange3Month").toDouble());
                data.data.x.insert(j, 90); // (0) (time)
                j++;
            }

            if(rec.value("procentChange6Month").isNull() != true)
            {
                found = true;
                data.data.y.insert(j, rec.value("procentChange6Month").toDouble());
                data.data.x.insert(j, 180); // (0) (time)
                j++;
            }


            if(rec.value("procentChange1Year").isNull() != true)
            {
                found = true;
                data.data.y.insert(j, rec.value("procentChange1Year").toDouble());
                data.data.x.insert(j, 365); // (0) (time)
                j++;
            }


            if(rec.value("procentChange2Year").isNull() != true)
            {
                found = true;
                data.data.y.insert(j, rec.value("procentChange2Year").toDouble());
                data.data.x.insert(j, 730); // (0) (time)
                j++;
            }


            if(rec.value("procentChange3Year").isNull() != true)
            {
                found = true;
                data.data.y.insert(j, rec.value("procentChange3Year").toDouble());
                data.data.x.insert(j, 1095); // (0) (time)
                j++;
            }

            if(rec.value("procentChange5Year").isNull() != true)
            {
                found = true;
                data.data.y.insert(j, rec.value("procentChange5Year").toDouble());
                data.data.x.insert(j, 1825); // (0) (time)
                j++;
            }

            // reset stock data index
            if(found == true && found1 == true)
            {
                found = false;
                found1 = false;
                stockArr.insert(i, data);
            }
            j = 0;
            i++;
        }
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}




/*****************************************************************
 *
 * Function:		getOneSnapshotStock()
 *
 * Description:		This function extract data for one stock to plot
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getOneSnapshotStock(CStockPlotUtil::StockData_ST &stock, CStockPlotUtil::MinMaxAxisValues_ST &axis)
{
    QSqlRecord rec;
    //QSqlQuery qry;
    QString str;
    double x;
    double y;
    int j;



    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    str.sprintf("SELECT companyName,"
                "procentChangeOneDay,"
                "procentChangeOneWeek,"
                "procentChange1Month,"
                "procentChange3Month,"
                "procentChange6Month,"
                "procentChange1Year,"
                "procentChange2Year,"
                "procentChange3Year,"
                "procentChange5Year"
                " FROM TblStockDataSnapshot "
                "WHERE companyName = '%s';", stock.stockName.toStdString().c_str());

    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }

        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {

        j = 0;

        while(qry.next())
        {
            rec = qry.record();


            if(rec.value("procentChangeOneDay").isNull() != true)
            {
                y = rec.value("procentChangeOneDay").toDouble();
                x = 0;
                stock.data.y.insert(j, y);
                stock.data.x.insert(j, x); // 365 * 5 (time)
                j++;

                updateMinMaxData(axis, x, y);
            }



            if(rec.value("procentChangeOneWeek").isNull() != true)
            {
                y = rec.value("procentChangeOneWeek").toDouble();
                x = 7; // (365 * 5) - 7 (time)
                stock.data.y.insert(j, y);
                stock.data.x.insert(j, x);
                j++;

                updateMinMaxData(axis, x, y);
            }

            if(rec.value("procentChange1Month").isNull() != true)
            {
                y = rec.value("procentChange1Month").toDouble();
                x = 30; // (365 * 5) - 30 (time)
                stock.data.y.insert(j, y);
                stock.data.x.insert(j, x);
                j++;

                updateMinMaxData(axis, x, y);
            }


            if(rec.value("procentChange3Month").isNull() != true)
            {
                y = rec.value("procentChange3Month").toDouble();
                x = 90; // (365 * 5) - 90 (time)
                stock.data.y.insert(j, y);
                stock.data.x.insert(j, x);
                j++;

                updateMinMaxData(axis, x, y);
            }

            if(rec.value("procentChange6Month").isNull() != true)
            {
                y = rec.value("procentChange6Month").toDouble();
                x = 180; // (365 * 5) - 180 (time)
                stock.data.y.insert(j, y);
                stock.data.x.insert(j, x);
                j++;

                updateMinMaxData(axis, x, y);
            }


            if(rec.value("procentChange1Year").isNull() != true)
            {
                y = rec.value("procentChange1Year").toDouble();
                x = 365; // (365 * 4) (time)
                stock.data.y.insert(j, y);
                stock.data.x.insert(j, x);
                j++;

                updateMinMaxData(axis, x, y);
            }


            if(rec.value("procentChange2Year").isNull() != true)
            {
                y = rec.value("procentChange2Year").toDouble();
                x = 730; // (365 * 3) (time)
                stock.data.y.insert(j, y);
                stock.data.x.insert(j, x);
                j++;

                updateMinMaxData(axis, x, y);
            }


            if(rec.value("procentChange3Year").isNull() != true)
            {
                y = rec.value("procentChange3Year").toDouble();
                x = 1095; // (365 * 2) (time)
                stock.data.y.insert(j, y);
                stock.data.x.insert(j, x);
                j++;

                updateMinMaxData(axis, x, y);
            }

            if(rec.value("procentChange5Year").isNull() != true)
            {
                y = rec.value("procentChange5Year").toDouble();
                x = 1825; // (0) (time)
                stock.data.y.insert(j, y);
                stock.data.x.insert(j, x);
                j++;

                updateMinMaxData(axis, x, y);
            }

            // reset stock data index
            j = 0;
        }
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}




/*****************************************************************
 *
 * Function:		()
 *
 * Description:		This function
 *
 *
 *
 *****************************************************************/
void CDbHndl::updateMinMaxData(CStockPlotUtil::MinMaxAxisValues_ST &axis, double x, double y)
{

    if(axis.minMaxIsInit == false)
    {
        axis.minY = y;
        axis.maxY = y;
        axis.minX = x;
        axis.maxX = x;
        axis.minMaxIsInit = true;
    }
    else
    {
        if(axis.minY > y)
            axis.minY = y;

        if(axis.maxY < y)
            axis.maxY = y;

        if(axis.minX > x)
            axis.minX = x;

        if (axis.maxX < x)
            axis.maxX = x;
    }

}



/*****************************************************************
 *
 * Function:		delAllDtcSwVerPageData()
 *
 * Description:		This function delete all data in temp list
 *
 *
 *
 *****************************************************************/
bool CDbHndl::delAllUserSelSnapshotData(void)
{
    QString str;
    //QSqlQuery qry;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    // Create sql question
    str.sprintf("DELETE "
                "FROM TblUserSelectedSnapshotData;");

    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 1"), qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}




/****************************************************************
 *
 * Function:    userSelSnapshotDataIsInitialized()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::userSelSnapshotDataIsInitialized(void)
{
    QSqlRecord rec;
    // QSqlQuery qry;
    QString str;
    int nofData = -1;
    int isInit = -1;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);




    str.sprintf("SELECT IsInitialized, "
                "NofUserSelectedSnapshotData"
                " FROM TblUserSelectedSnapshotData;");


    // qDebug() << str << "\n";
    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toLatin1().constData());
        }
        qDebug() << qry.lastError();
    }
    else
    {
        if(qry.next())
        {
            rec = qry.record();

            if(rec.value("NofUserSelectedSnapshotData").isNull()==true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                nofData = rec.value("NofUserSelectedSnapshotData").toInt();
            }


            if(rec.value("IsInitialized").isNull()==true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                isInit = rec.value("IsInitialized").toInt();
            }
        }
    }



    qry.finish();


    if(nofData > 0 && isInit == 1)
    {
        closeDb();
        m_mutex.unlock();
        return true;
    }

    closeDb();
    m_mutex.unlock();
    return false;
}


struct TempUserSelSnapshotData_ST
{
    int isInitialized;
    int nofUserSelectedSnapshotData;
    int isSelected[CDbHndl::MAX_NOF_GPSEL_INDEX];
    QString name[CDbHndl::MAX_NOF_GPSEL_INDEX];
    QString nameInDb[CDbHndl::MAX_NOF_GPSEL_INDEX];
    QString sortOrder[CDbHndl::MAX_NOF_GPSEL_INDEX];
    QString sortType[CDbHndl::MAX_NOF_GPSEL_INDEX];
};



/****************************************************************
 *
 * Function:    initHeaderList()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
bool CDbHndl::saveUserSelectStockParamsInDb(QStandardItemModel *tableModel)
{
    int row;
    int i;
    QVariant vData;
    QString data;
    bool found;
    TempUserSelSnapshotData_ST tmpData;

    m_maxTableColumns = 0;


    for(row=0; row < tableModel->rowCount(); row++)
    {
        found = false;

        // Make sure that the item is checkable
        Qt::ItemFlags flags = tableModel->item(row, 0)->flags();

        if (!(flags & Qt::ItemIsUserCheckable) || !(flags & Qt::ItemIsEnabled))
        {
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::information(0, "Error", "No checkable items in table (1)");
            }
            return false;
        }

        // Make sure that we have a check state
        QVariant value = tableModel->item(row, 0)->data(Qt::CheckStateRole);
        if (!value.isValid())
        {
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::information(0, "Error", "No checkable items in table (2)");
            }
            return false;
        }

        if((static_cast<Qt::CheckState>(value.toInt()) == Qt::Checked))
        {
            vData = tableModel->item(row, 1)->text();
            data = vData.toString();

            for(i = 0; i < CDbHndl::MAX_NOF_GPSEL_INDEX; i++)
            {
                if(m_gpSelLookUpTable[i].name.compare(data)== 0)
                {
                    found = true;
                    tmpData.isSelected[m_maxTableColumns] = 1;
                    tmpData.name[m_maxTableColumns] = m_gpSelLookUpTable[i].name;
                    tmpData.nameInDb[m_maxTableColumns] = m_gpSelLookUpTable[i].nameInDb;
                    tmpData.sortOrder[m_maxTableColumns] = m_gpSelLookUpTable[i].sortOrder;
                    tmpData.sortType[m_maxTableColumns] = m_gpSelLookUpTable[i].sortType;
                    m_maxTableColumns++;
                    //addOneSlotTableHeaderData(m_maxTableColumns++, (CDbHndl::GuiParamSelIndex_ET) i);
                    break;
                }
            }

            if(found == false)
            {
                if(m_disableMsgBoxes == false)
                {
                    QMessageBox::information(0, "Error", "Unknown parameter name");
                    QMessageBox::information(0, "Error", data);
                }
                return false;
            }
        }
        else
        {
            vData = tableModel->item(row, 1)->text();
            data = vData.toString();

            for(i = 0; i < CDbHndl::MAX_NOF_GPSEL_INDEX; i++)
            {
                if(m_gpSelLookUpTable[i].name.compare(data)== 0)
                {
                    found = true;
                    tmpData.isSelected[m_maxTableColumns] = 0;
                    tmpData.name[m_maxTableColumns] = m_gpSelLookUpTable[i].name;
                    tmpData.nameInDb[m_maxTableColumns] = m_gpSelLookUpTable[i].nameInDb;
                    tmpData.sortOrder[m_maxTableColumns] = m_gpSelLookUpTable[i].sortOrder;
                    tmpData.sortType[m_maxTableColumns] = m_gpSelLookUpTable[i].sortType;
                    m_maxTableColumns++;
                    //addOneSlotTableHeaderData(m_maxTableColumns++, (CDbHndl::GuiParamSelIndex_ET) i);
                    break;
                }
            }

            if(found == false)
            {
                if(m_disableMsgBoxes == false)
                {
                    QMessageBox::information(0, "Error", "Unknown parameter name");
                    QMessageBox::information(0, "Error", data);
                }
                return false;
            }

        }
    }


    //QSqlQuery qry;
    QString str;
    QString str1;


    if(m_maxTableColumns < 1)
        return false;

    delAllUserSelSnapshotData();

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);



    // Prepare query that is not part of any loops
    str.sprintf("INSERT INTO TblUserSelectedSnapshotData"
                "( IsInitialized, "
                "NofUserSelectedSnapshotData, ");



    // Insert all user selected parameters
    for(i = 0; i < m_maxTableColumns; i++)
    {
        if(i < m_maxTableColumns-1)
        {
            str1.sprintf("snapshotData%dIsSel,"
                         "snapshotData%d,"
                         "snapshotDbName%d, "
                         , i, i, i);
        }
        else
        {
            str1.sprintf("snapshotData%dIsSel, "
                         "snapshotData%d, "
                         "snapshotDbName%d) "
                         , i, i, i);
        }

        str += str1;
    }

    // Insert corresponding values
    str1.sprintf("VALUES ( 1, %d, ", m_maxTableColumns);
    str += str1;

    for(i = 0; i < m_maxTableColumns; i++)
    {
        if(i < m_maxTableColumns - 1)
        {
            str1.sprintf("%d, ", tmpData.isSelected[i]);
            str1 += "'";
            str1 += tmpData.name[i];
            str1 += "'";
            str1 += ", ";
            str1 += "'";
            str1 += tmpData.nameInDb[i];
            str1 += "'";
            str1 += ", ";
        }
        else
        {
            str1.sprintf("%d, '%s', '%s');", tmpData.isSelected[i]
                         , tmpData.name[i].toStdString().c_str()
                         , tmpData.nameInDb[i].toStdString().c_str());
        }
        str += str1;
    }


    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error"), qry.lastError().text().toLatin1().constData());
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error"), str);
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;

}





/****************************************************************
 *
 * Function:    initKeyNumberList()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
void CDbHndl::initKeyNumberList(QTableView *tableView, QStandardItemModel **tableModel)
{
    QString item;

    for(m_maxTableColumns = 1; m_maxTableColumns < CDbHndl::MAX_NOF_GPSEL_INDEX; m_maxTableColumns++)
    {
        // create check box item
        QStandardItem* item0 = new QStandardItem(true);
        item0->setCheckable(true);
        item0->setCheckState(Qt::Checked);  //Qt::Checked  //Qt::Unchecked
        item0->setText("");
        (*tableModel)->setItem(m_maxTableColumns - 1, 0, item0);

        item = m_gpSelLookUpTable[m_maxTableColumns].name;

        m_tableHeaderList[m_maxTableColumns-1].name = m_gpSelLookUpTable[m_maxTableColumns].name;
        m_tableHeaderList[m_maxTableColumns-1].nameInDb = m_gpSelLookUpTable[m_maxTableColumns].nameInDb;
        m_tableHeaderList[m_maxTableColumns-1].sortOrder = m_gpSelLookUpTable[m_maxTableColumns].sortOrder;
        m_tableHeaderList[m_maxTableColumns-1].sortType = m_gpSelLookUpTable[m_maxTableColumns].sortType;

        // create text item
        (*tableModel)->setItem(m_maxTableColumns - 1, 1, new QStandardItem(item));

    }

    if(m_maxTableColumns>0)
        m_maxTableColumns--;

    tableView->setModel(*tableModel);
    tableView->resizeColumnsToContents();
}




/****************************************************************
 *
 * Function:    initKeyNumberListFromDb()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
void CDbHndl::initKeyNumberListFromDb(QTableView *tableView, QStandardItemModel **tableModel)
{
    int i;
    int isSelected;
    QString stockIndicator;
    QString str;
    QString str1;
    QString str2;
    // QSqlQuery qry;
    QSqlRecord rec;
    int cnt = 0;


    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);

    m_maxTableColumns = 0;

    // Create sql question
    str.sprintf("SELECT *"
                " FROM TblUserSelectedSnapshotData");


    // Execute sql question
    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 26"), qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return;
    }


    if(!qry.next())
    {
        closeDb();
        m_mutex.unlock();
        return;
    }

    m_maxTableColumns = 0;
    rec = qry.record();

    if(rec.value("IsInitialized").isNull() == true)
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error"), QString::fromUtf8("ERROR: IsInitialized is NULL"));
        }
        return;
    }


    if(rec.value("NofUserSelectedSnapshotData").isNull()==true)
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error"), QString::fromUtf8("ERROR: NofUserSelectedSnapshotData is NULL"));
        }
        return;
    }

    m_maxTableColumns = rec.value("NofUserSelectedSnapshotData").toInt();

    for(i = 0; i < m_maxTableColumns; i++)
    {
        str1.sprintf("snapshotData%dIsSel", i);
        if(rec.value(str1).isNull()==true)
        {
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("Database Error"), QString::fromUtf8("ERROR: str1 is NULL"));
            }
            return;
        }


        isSelected = rec.value(str1).toInt();


        QStandardItem* item0 = new QStandardItem(true);
        item0->setCheckable(true);
        if(isSelected == 1)
        {
            item0->setCheckState(Qt::Checked);  //Qt::Checked  //Qt::Unchecked
        }
        else
        {
            item0->setCheckState(Qt::Unchecked);  //Qt::Checked  //Qt::Unchecked
        }
        item0->setText("");
        (*tableModel)->setItem(i, 0, item0);



        str2.sprintf("snapshotData%d", i);
        if(rec.value(str2).isNull()==true)
        {
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("Database Error"), QString::fromUtf8("ERROR: str2 is NULL"));
            }
            return;
        }

        stockIndicator = rec.value(str2).toString();


        // create text item
        (*tableModel)->setItem(i, 1, new QStandardItem(stockIndicator));

        for(int j=0; j < CDbHndl::MAX_NOF_GPSEL_INDEX; j++)
        {
            if(m_gpSelLookUpTable[j].name.compare(stockIndicator)== 0)
            {
                if(isSelected == 1)
                {
                    m_tableHeaderList[cnt].name = m_gpSelLookUpTable[j].name;
                    m_tableHeaderList[cnt].nameInDb = m_gpSelLookUpTable[j].nameInDb;
                    m_tableHeaderList[cnt].sortOrder = m_gpSelLookUpTable[j].sortOrder;
                    m_tableHeaderList[cnt].sortType = m_gpSelLookUpTable[j].sortType;
                    cnt++;
                }
            }
        }
    }

    tableView->setModel(*tableModel);
    m_maxTableColumns = cnt;

    qry.finish();
    closeDb();
    m_mutex.unlock();
    tableView->resizeColumnsToContents();
    tableView->resize(1100,600);
    return;

}




/****************************************************************
 *
 * Function:    getFilterMask()
 *
 * Description: This function return mask with all parameter
 *              that are included in filter
 *
 *
 ****************************************************************/
qint64 CDbHndl::getFilterMask(void)
{
    return m_filter.mainStockIndicatorMask;
}

/****************************************************************
 *
 * Function:    getGuiCompareParamMask()
 *
 * Description: This function gets search filter compare parameter
 *              as a search string and return corresponing mask
 *              number.
 *
 *              The parameter is used when data is filtered.
 *
 *              <, ==, > are example on this params
 *
 ****************************************************************/
bool CDbHndl::getGuiCompareParamMask(char *param, quint64 &mask)
{
    int i;


    for( i= 0; i < MAX_NOF_GPCOMP_INDEX; i++)
    {
        if(strcmp(m_gpCompLookUpTable[i].compName, param) == 0)
        {
            mask = m_gpCompLookUpTable[i].compMaskBit;
            return true;
        }
    }

    return false;
}


/****************************************************************
 *
 * Function:    comboBoxSetSelStr()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
bool CDbHndl::comboBoxSetSelStr(QComboBox* combo, QString str)
{
    int index;

    index = combo->findText(str);

    if( index != -1 )
    {
        combo->setCurrentIndex(index);
        return true;
    }

    return false;
}


/****************************************************************
 *
 * Function:    filterHelpCreateWherePart()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
bool CDbHndl::filterCreateWherePart(QString &str)
{
    int i;
    QString tmp;

    qDebug() << "checkFilter";
    debugPrintFilter();

    if(m_filter.nofStockIndicators < 1)
        return false;


    if(m_filter.indicatorlSetting[0].nofCompareValues == 1)
    {
        str.sprintf(" WHERE CAST(%s AS REAL) %s %g "
                    , m_filter.indicatorlSetting[0].indicator.nameInDb
                , m_filter.indicatorlSetting[0].compareParam[0].compName
                , atof(m_filter.indicatorlSetting[0].indicatorValue[0].toStdString().c_str()));
    }
    else
    {
        str.sprintf(" WHERE CAST(%s AS REAL) %s %g "
                    " AND CAST(%s AS REAL) %s %g"
                    , m_filter.indicatorlSetting[0].indicator.nameInDb
                , m_filter.indicatorlSetting[0].compareParam[0].compName
                , atof(m_filter.indicatorlSetting[0].indicatorValue[0].toStdString().c_str())
                , m_filter.indicatorlSetting[0].indicator.nameInDb
                , m_filter.indicatorlSetting[0].compareParam[1].compName
                , atof(m_filter.indicatorlSetting[0].indicatorValue[1].toStdString().c_str()));
    }

    for(i = 1; i < m_filter.nofStockIndicators; i++)
    {
        if(m_filter.indicatorlSetting[i].nofCompareValues == 1)
        {
            tmp.sprintf(" AND CAST(%s AS REAL) %s %g"
                        , m_filter.indicatorlSetting[i].indicator.nameInDb
                        , m_filter.indicatorlSetting[i].compareParam[0].compName
                    , atof(m_filter.indicatorlSetting[i].indicatorValue[0].toStdString().c_str()));
        }
        else
        {
            tmp.sprintf(" AND CAST(%s AS REAL) %s %g"
                        " AND CAST(%s AS REAL) %s %g"
                        , m_filter.indicatorlSetting[0].indicator.nameInDb
                    , m_filter.indicatorlSetting[0].compareParam[0].compName
                    , atof(m_filter.indicatorlSetting[0].indicatorValue[0].toStdString().c_str())
                    , m_filter.indicatorlSetting[0].indicator.nameInDb
                    , m_filter.indicatorlSetting[0].compareParam[1].compName
                    , atof(m_filter.indicatorlSetting[0].indicatorValue[1].toStdString().c_str()));
        }
        str += tmp;
    }

    return true;
}

/****************************************************************
 *
 * Function:    updateFilterParam()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
updateFilterParam(QComboBox *indicatorComboBox,
                  QComboBox *compareComboBox,
                  QLineEdit *IndicatorValueLineEdit,
                  const int index)
{

    FilterSettings_ET *f;
    QString str;
    quint64 compParamMask;
    int len;


    f = &m_filter;

    qDebug() << "Update filter" << "index" << index << "\n";
    qDebug() << f->indicatorlSetting[index].indicator.name << "\n";


    //str = m_gpSelLookUpTable[index].indicatorName;
    str = f->indicatorlSetting[index].indicator.name;
    if(str.compare(indicatorComboBox->currentText()) != 0)
    {
        qDebug() << "Fail to update"  << indicatorComboBox->currentText() << "\n";
        return false;
    }



    if(getGuiCompareParamMask((char *)compareComboBox->currentText().toStdString().c_str(), compParamMask) == false)
    {
        qDebug() << "Fail to update mask"  << indicatorComboBox->currentText() << "\n";
        return false;
    }

    if(IndicatorValueLineEdit->text().length() < 1)
    {
        return false;
    }


    //Just OR Select parameter integer mask here
    // Copy compare parameter as text and as integer mask
    len = strlen((char *)compareComboBox->currentText().toStdString().c_str());
    len++;
    strcpy(f->indicatorlSetting[index].compareParam[1].compName, (char *) compareComboBox->currentText().toStdString().c_str());
    f->indicatorlSetting[index].compareParam[1].compMaskBit = compParamMask;

    // Copy data that shall be compared
    f->indicatorlSetting[index].indicatorValue[1].append(IndicatorValueLineEdit->text());

    // Incremet number of compare data.
    f->indicatorlSetting[index].nofCompareValues++;

    return true;
}



/****************************************************************
 *
 * Function:    insertFilterParam()
 *
 * Description: Use this function when parameter does Not exist
 *              in filter.
 *
 *
 ****************************************************************/
bool CDbHndl::
insertFilterParam(QComboBox *indicatorComboBox,
                  QComboBox *compareComboBox,
                  QLineEdit *IndicatorValueLineEdit,
                  CDbHndl::GuiParamSelIndex_ET index)
{
    FilterSettings_ET *f;
    char dbIndictorName[GUI_PARAM_MAX_STR_SIZE];
    quint64 selParamMask;
    quint64 compParamMask;
    int len;

    f = &m_filter;

    if(getGuiSelectParamMask((char *)indicatorComboBox->currentText().toStdString().c_str(), selParamMask, dbIndictorName)==false)
    {
        return false;
    }

    if(getGuiCompareParamMask((char *)compareComboBox->currentText().toStdString().c_str(), compParamMask) == false)
    {
        return false;
    }

    if(IndicatorValueLineEdit->text().length() < 1)
    {
        return false;
    }


    qDebug() << "dbIndictorName" << dbIndictorName;
    qDebug() << "indicator" << indicatorComboBox->currentText().toStdString().c_str();

    //Copy Select parameter as text and as integer mask
    len = strlen((char *)indicatorComboBox->currentText().toStdString().c_str());
    len++;
    f->indicatorlSetting[f->nofStockIndicators].indicator.name = indicatorComboBox->currentText();
    f->indicatorlSetting[f->nofStockIndicators].indicator.indicatorlMask = selParamMask;
    strcpy(f->indicatorlSetting[f->nofStockIndicators].indicator.nameInDb, dbIndictorName);
    f->index[f->nofStockIndicators] = index;

    // Copy compare parameter as text and as integer mask
    len = strlen((char *)compareComboBox->currentText().toStdString().c_str());
    len++;
    strcpy(f->indicatorlSetting[f->nofStockIndicators].compareParam[0].compName,
            (char *)compareComboBox->currentText().toStdString().c_str());
    f->indicatorlSetting[f->nofStockIndicators].compareParam[0].compMaskBit = compParamMask;

    // Copy data that shall be compared
    f->indicatorlSetting[f->nofStockIndicators].indicatorValue[0].append(IndicatorValueLineEdit->text());

    // Incremet number of compare data.
    f->indicatorlSetting[f->nofStockIndicators].nofCompareValues++;

    // Increment number of filter parameters
    f->nofStockIndicators++;

    return true;
}


/****************************************************************
 *
 * Function:    guiCntrlDataIsValid()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
guiCntrlDataIsValid(QComboBox *indicatorComboBox, QComboBox *compareComboBox, QLineEdit *IndicatorValueLineEdit)
{
    char str[255];

    strcpy(str, indicatorComboBox->currentText().toStdString().c_str());

    if(m_gpSelLookUpTable[CDbHndl::MASK_GPSEL_NOT_USED].name.compare(str)== 0)
        return false;

    if(indicatorComboBox->currentText().length() < 1)
        return false;

    if(compareComboBox->currentText().length() < 1)
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::warning(NULL, "Filter fel", "Sk villkor saknas");
        }
        return false;
    }

    if(IndicatorValueLineEdit->text().length() < 1)
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::warning(NULL, "Filter fel", "filter Ange vrde");
        }
        return false;
    }

    return true;
}



/****************************************************************
 *
 * Function:    guiFilterCtrlResetContents()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
void CDbHndl::guiFilterCtrlResetContents(CDbHndl::TableTabWidget_ST *filterCtrls)
{
    int i;
    for(i = 0; i < CDbHndl::MAX_NOF_TABLE_TAB_GUI_SEL_CTRLS; i++)
    {
        filterCtrls->indicatorComboBox[i]->clear();
        filterCtrls->compareComboBox[i]->clear();
        filterCtrls->IndicatorValueLineEdit[i]->clear();
    }
}




/****************************************************************
 *
 * Function:    filterHelpCreateWherePart()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
bool CDbHndl::filterAndSortCreateWherePart(QString &str, QString sortParam, QString sortOrder)
{
    int i;
    int j;
    QString tmp;
    bool found = false;

    qDebug() << "checkFilter";
    debugPrintFilter();

    if(strcmp(sortParam.toStdString().c_str(), "N/A") == 0)
        return true;

    if(m_filter.nofStockIndicators < 1)
    {
        for(j = 0; j < CDbHndl::MAX_NOF_GPSEL_INDEX; j++)
        {
            if(strcmp(m_gpSelLookUpTable[j].nameInDb, sortParam.toStdString().c_str()) == 0)
            {
                switch(m_gpSelLookUpTable[j].sortType)
                {
                case SISORT_TYPE_FLOAT:
                    tmp.sprintf(" ORDER BY CAST(%s AS REAL) %s"
                                , sortParam.toStdString().c_str()
                                , sortOrder.toStdString().c_str());
                    str = tmp;
                    return true;

                case SISORT_TYPE_TEXT:
                    tmp.sprintf(" ORDER BY LOWER(%s) %s"
                                , sortParam.toStdString().c_str()
                                , sortOrder.toStdString().c_str());
                    str = tmp;
                    return true;

                }
            }
        }

        if(found == false)
        {
            str.sprintf(" ORDER BY LOWER(%s) %s;"
                        , sortParam.toStdString().c_str()
                        , sortOrder.toStdString().c_str());
            return true;
        }
    }



    if(m_filter.indicatorlSetting[0].nofCompareValues == 1)
    {
        str.sprintf(" WHERE CAST(%s AS REAL) %s %g "
                    , m_filter.indicatorlSetting[0].indicator.nameInDb
                , m_filter.indicatorlSetting[0].compareParam[0].compName
                , atof(m_filter.indicatorlSetting[0].indicatorValue[0].toStdString().c_str()));
    }
    else
    {
        str.sprintf(" WHERE CAST(%s AS REAL) %s %g "
                    " AND CAST(%s AS REAL) %s %g"
                    , m_filter.indicatorlSetting[0].indicator.nameInDb
                , m_filter.indicatorlSetting[0].compareParam[0].compName
                , atof(m_filter.indicatorlSetting[0].indicatorValue[0].toStdString().c_str())
                , m_filter.indicatorlSetting[0].indicator.nameInDb
                , m_filter.indicatorlSetting[0].compareParam[1].compName
                , atof(m_filter.indicatorlSetting[0].indicatorValue[1].toStdString().c_str()));
    }

    for(i = 1; i < m_filter.nofStockIndicators; i++)
    {
        if(m_filter.indicatorlSetting[i].nofCompareValues == 1)
        {
            tmp.sprintf(" AND CAST(%s AS REAL) %s %g"
                        , m_filter.indicatorlSetting[i].indicator.nameInDb
                        , m_filter.indicatorlSetting[i].compareParam[0].compName
                    , atof(m_filter.indicatorlSetting[i].indicatorValue[0].toStdString().c_str()));
        }
        else
        {
            tmp.sprintf(" AND CAST(%s AS REAL) %s %g"
                        " AND CAST(%s AS REAL) %s %g"
                        , m_filter.indicatorlSetting[0].indicator.nameInDb
                    , m_filter.indicatorlSetting[0].compareParam[0].compName
                    , atof(m_filter.indicatorlSetting[0].indicatorValue[0].toStdString().c_str())
                    , m_filter.indicatorlSetting[0].indicator.nameInDb
                    , m_filter.indicatorlSetting[0].compareParam[1].compName
                    , atof(m_filter.indicatorlSetting[0].indicatorValue[1].toStdString().c_str()));
        }
        str += tmp;
    }

    for(j = 0; j < CDbHndl::MAX_NOF_GPSEL_INDEX; j++)
    {
        if(strcmp(m_gpSelLookUpTable[j].nameInDb, sortParam.toStdString().c_str()) == 0)
        {
            switch(m_gpSelLookUpTable[j].sortType)
            {
            case SISORT_TYPE_FLOAT:
                tmp.sprintf(" ORDER BY CAST(%s AS REAL) %s"
                            , sortParam.toStdString().c_str()
                            , sortOrder.toStdString().c_str());
                found = true;
                break;
            case SISORT_TYPE_TEXT:
                tmp.sprintf(" ORDER BY LOWER(%s) %s"
                            , sortParam.toStdString().c_str()
                            , sortOrder.toStdString().c_str());
                found = true;
                break;

            }
        }
    }

    if(found == false)
    {
        tmp.sprintf(" ORDER BY LOWER(%s) %s"
                    , sortParam.toStdString().c_str()
                    , sortOrder.toStdString().c_str());
    }

    str += tmp;

    return true;
}




/****************************************************************
 *
 * Function:    initGuiCtrlSettings()
 *
 * Description: This function reset the array that tells how
 *              user has selected stock parmeters in the user
 *              controls.
 *
 *
 *
 ****************************************************************/
void CDbHndl::initGuiCtrlSettings(void)
{
    int i;
    for(i = 0; i < CDbHndl::MAX_NOF_TABLE_TAB_GUI_SEL_CTRLS; i++)
    {
        m_filter.indicatorlSetting[i].nofCompareValues = 0;
    }
    m_filter.nofStockIndicators = 0;
    m_filter.mainStockIndicatorMask = 0;

}


/****************************************************************
 *
 * Function:    getGuiSelectParamMask()
 *
 * Description: This function get search filter parameter as
 *              a search string and return corresponing
 *              mask number.
 *
 *              The parameter is used when data is filtered.
 *              PE, PS, Vinst/aktie are example on this params
 *
 ****************************************************************/
bool CDbHndl::getGuiSelectParamMask(char *param, quint64 &mask, char *dbIndicatorName)
{
    int i;

    for( i= 0; i < MAX_NOF_GPSEL_INDEX; i++ )
    {
        if(m_gpSelLookUpTable[i].name.compare(param) == 0)
        {
            mask = m_gpSelLookUpTable[i].indicatorlMask;
            strcpy(dbIndicatorName, m_gpSelLookUpTable[i].nameInDb);
            return true;
        }
    }

    return false;
}

/****************************************************************
 *
 * Function:    findFilterParam()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
bool CDbHndl::findFilterParam(QComboBox *indicatorComboBox, int &index)
{

    FilterSettings_ET *f;
    QString str;


    if(indicatorComboBox->currentText().length()< 1)
        return false;

    qDebug() << "find" << indicatorComboBox->currentText() << "\n";

    f = &m_filter;

    // Check if filter parameter already has been added to filter
    for(index = 0; index < f->nofStockIndicators; index++)
    {
        str = f->indicatorlSetting[index].indicator.name;
        if(str.compare(indicatorComboBox->currentText())== 0)
        {
            qDebug() << "has found" << indicatorComboBox->currentText() << "\n";
            return true;
        }
    }

    return false;
}

/****************************************************************
 *
 * Function:    getFilterNofCompareVal()
 *
 * Description: This function return the number of values a parameter
 *              should be compared with (1, or 2)
 *
 * Example:      PE < 5              (function return 1)
 *               PE < 15 AND PE >= 5 (function return 2)
 *
 ****************************************************************/
bool CDbHndl::getFilterNofCompareVal(GuiParamSelIndex_ET paramIndex, unsigned char &nofCompareVal)
{
    int i;

    for(i = 0; i < m_filter.nofStockIndicators; i++)
    {
        if(m_filter.indicatorlSetting[i].indicator.indicatorlMask ==
                m_gpSelLookUpTable[paramIndex].indicatorlMask)
        {
            nofCompareVal = m_filter.indicatorlSetting[i].nofCompareValues;
            return true;
        }
    }
    return false;
}


/****************************************************************
 *
 * Function:    debugPrintFilter()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
void CDbHndl::debugPrintFilter(void)
{
    int i, j;

    qDebug() << "m_filter.nofStockIndicators" << m_filter.nofStockIndicators;


    for(i = 0; i < m_filter.nofStockIndicators; i++)
    {
        qDebug() << "indicatorlMask" << m_filter.indicatorlSetting[i].indicator.indicatorlMask;
        qDebug() << "indicatorlName" << m_filter.indicatorlSetting[i].indicator.name;
        qDebug() << "indicatorlDbName" << m_filter.indicatorlSetting[i].indicator.nameInDb;

        for(j=0; j < m_filter.indicatorlSetting[i].nofCompareValues; j++)
        {
            qDebug() << "compName" << m_filter.indicatorlSetting[i].compareParam[j].compName;
            qDebug() << "indicatorValue" << m_filter.indicatorlSetting[i].indicatorValue[j];
        }

    }

}

/****************************************************************
 *
 * Function:    getFilterSetting()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
getFilterSetting(int i, CDbHndl::Filter_ST &data)
{
    int j;

    if(m_filter.nofStockIndicators > CDbHndl::MAX_NOF_TABLE_TAB_GUI_SEL_CTRLS)
        return false;

    if(i >= m_filter.nofStockIndicators || i < 0)
        return false;

    data.indikator = m_filter.indicatorlSetting[i].indicator.name;
    data.nofcompareValues.sprintf("%d", m_filter.indicatorlSetting[i].nofCompareValues);
    for(j=0; j < m_filter.indicatorlSetting[i].nofCompareValues; j++)
    {
        data.compare[j] = m_filter.indicatorlSetting[i].compareParam[j].compName;
        data.value[j] = m_filter.indicatorlSetting[i].indicatorValue[j];
    }

    return true;
}


/****************************************************************
 *
 * Function:    getNofFilterIndicators()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
int CDbHndl::
getNofFilterIndicators(void)
{
    return m_filter.nofStockIndicators;
}


/****************************************************************
 *
 * Function:    getFilterNofCompareVal()
 *
 * Description: This function return the number of values a parameter
 *              should be compared with (1, or 2)
 *
 * Example:      PE < 5              (function return 1)
 *               PE < 15 AND PE >= 5 (function return 2)
 *
 ****************************************************************/
bool CDbHndl::getFilterCompareInfo(GuiParamSelIndex_ET paramIndex, StockIndicatorlSetting_ST &compInfo)
{
    int i;
    int j;
    int len;

    for(i = 0; i < m_filter.nofStockIndicators; i++)
    {
        if(m_filter.indicatorlSetting[i].indicator.indicatorlMask ==
                m_gpSelLookUpTable[paramIndex].indicatorlMask)
        {

            strcpy(compInfo.indicator.nameInDb,  m_gpSelLookUpTable[paramIndex].nameInDb);
            compInfo.indicator.name = m_filter.indicatorlSetting[i].indicator.name;
            // Get number of compare variables
            compInfo.nofCompareValues = m_filter.indicatorlSetting[i].nofCompareValues;

            for(j = 0; j < m_filter.indicatorlSetting[i].nofCompareValues; j++)
            {
                // Get value
                compInfo.indicatorValue[j].append(m_filter.indicatorlSetting[i].indicatorValue[j]);

                // Get compare sign
                len = strlen(m_filter.indicatorlSetting[i].compareParam[j].compName);
                len++;
                strcpy(compInfo.compareParam[j].compName,
                       m_filter.indicatorlSetting[i].compareParam[j].compName);
            }
            return true;
        }
    }
    return false;
}

/****************************************************************
 *
 * Function:    addDb()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
void CDbHndl::addDb(QString path)
{
    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    m_db = QSqlDatabase::addDatabase( "QSQLITE" );
    m_db.setDatabaseName(path);

    closeDb();
    m_mutex.unlock();
}




/****************************************************************
 *
 * Function:    openDb()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::openDb(QString path, bool useMutex)
{
    if(useMutex==true)
        m_mutex.lock();

    m_connection = "jackStock";
    m_db = QSqlDatabase::addDatabase("QSQLITE", m_connection);
    m_db.setDatabaseName(path);

    if( !m_db.open() )
    {
        qDebug() << m_db.lastError();
        qFatal( "Failed to connect." );

        if(useMutex==true)
            m_mutex.unlock();

        return false;
    }


    m_db.exec("BEGIN TRANSACTION;");
    return true;
}


/****************************************************************
 *
 * Function:    closeDb()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
void CDbHndl::closeDb(bool useMutex)
{
    m_db.exec("END TRANSACTION");
    //qDebug() << "remove database connection name" << m_connection;
    m_db.close();
    m_db = QSqlDatabase();
    m_db.removeDatabase(m_connection);

    if(useMutex==true)
        m_mutex.unlock();



}





/*****************************************************************
 *
 * Function:		delAllDtcSwVerPageData()
 *
 * Description:		This function delete all data in temp list
 *
 *
 *
 *****************************************************************/
bool CDbHndl::delAllStockSnapshotData(bool dbIsHandledExternly)
{
    QString str;
    // QSqlQuery qry;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }
    QSqlQuery qry(m_db);


    // Create sql question
    str.sprintf("DELETE "
                "FROM TblStockDataSnapshot;");

    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 1"), qry.lastError().text().toLatin1().constData());
        }
        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }
    else
        qDebug( "Inserted!" );

    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }
    return true;
}


/*****************************************************************
 *
 * Function:		delAllFilterStockSnapshotData()
 *
 * Description:		This function delete all data in:
 *                  TblFilterStockDataSnapshot
 *
 *
 *
 *****************************************************************/
bool CDbHndl::delAllFilterStockSnapshotData(void)
{
    QString str;
    //QSqlQuery qry;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    // Create sql question
    str.sprintf("DELETE "
                "FROM TblFilterStockDataSnapshot;");

    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 2"), qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
        qDebug( "Inserted!" );

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}






/****************************************************************
 *
 * Function:    createTabelTabFilter()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
void CDbHndl::createTabelTabFilter(CDbHndl::TableTabWidget_ST tableTabW)
{
    int index;
    int i;
    bool found = false;


    initGuiCtrlSettings();

    for(i = 0; i < CDbHndl::MAX_NOF_TABLE_TAB_GUI_SEL_CTRLS; i++)
    {
        // contains GUI controls data?
        if(false == guiCntrlDataIsValid(tableTabW.indicatorComboBox[i], tableTabW.compareComboBox[i], tableTabW.IndicatorValueLineEdit[i]))
        {
            if(found == true)
                break;
            return;
        }

        // Contains filter this parameter already
        if(false == findFilterParam(tableTabW.indicatorComboBox[i], index))
        {
            if(false == insertFilterParam(tableTabW.indicatorComboBox[i], tableTabW.compareComboBox[i], tableTabW.IndicatorValueLineEdit[i], (GuiParamSelIndex_ET)i))
            {
                if(m_disableMsgBoxes == false)
                {
                    QMessageBox::warning(NULL, "Filter fel", "Kan inte lägga till filter parameter");
                }
                return;
            }

        }
        else
        {
            if(false == updateFilterParam(tableTabW.indicatorComboBox[i], tableTabW.compareComboBox[i], tableTabW.IndicatorValueLineEdit[i], index))
            {
                if(m_disableMsgBoxes == false)
                {
                    QMessageBox::warning(NULL, "Filter fel", "Kan inte uppdatera filter parameter");
                }
                return;
            }
        }

        qDebug() << "update filter\n";
        qDebug() << "combo sel" << tableTabW.indicatorComboBox[i]->currentText() << "\n";
        qDebug() << "combo comp" << tableTabW.compareComboBox[i]->currentText() << "\n";
        qDebug() << "combo value" << tableTabW.IndicatorValueLineEdit[i]->text() << "\n";
        found = true;
    }

    // Create mask that contains all filtred parameters
    m_filter.mainStockIndicatorMask = 0;
    for(i = 0; i < m_filter.nofStockIndicators; i++)
        m_filter.mainStockIndicatorMask |= m_filter.indicatorlSetting[i].indicator.indicatorlMask;



    debugPrintFilter();

    return;
}





/****************************************************************
 *
 * Function:    insertOneRowPriceData()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::insertFilterDataInDb(void)
{

    // QSqlQuery qry;
    QString str;
    QString str1;
    CDbHndl::Filter_ST data[CDbHndl::MAX_NOF_TABLE_TAB_GUI_SEL_CTRLS];
    int totNofInicator;
    int i;

    // Get filter settings
    totNofInicator = getNofFilterIndicators();

    if(totNofInicator < 1)
        return false;

    for(i = 0; i < CDbHndl::MAX_NOF_TABLE_TAB_GUI_SEL_CTRLS; i++)
        getFilterSetting(i, data[i]);


    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);
    // Prepare query that is not part of any loops
    str.sprintf("INSERT INTO TblFilterStockDataSnapshot"
                "(totNofInicator,");


    // Create part of query that contains database variable names
    for(i=0; i < totNofInicator; i++)
    {
        if(i == totNofInicator -1)
        {
            // If last data in loop replace ',' with ')'
            str1.sprintf("indicator%d,"
                         "compare%dA,"
                         "compare%dB,"
                         "nofCompareValues%d,"
                         "value%dA,"
                         "value%dB)", i, i, i, i, i, i);
        }
        else
        {
            str1.sprintf("indicator%d,"
                         "compare%dA,"
                         "compare%dB,"
                         "nofCompareValues%d,"
                         "value%dA,"
                         "value%dB,", i, i, i, i, i, i);
        }
        str += str1;
    }

    // Prepare query that is not part of any loops
    str1.sprintf( "VALUES('%d',", totNofInicator);
    str += str1;


    // Create part of query that contains data that shall be inserted in database
    for(i=0; i < totNofInicator; i++)
    {
        if(i == totNofInicator - 1)
        {
            // If last data in loop replace ',' with ')'
            str1.sprintf("'%s',"
                         "'%s',"
                         "'%s',"
                         "'%s',"
                         "'%s',"
                         "'%s')",
                         data[i].indikator.toStdString().c_str(),
                         data[i].compare[0].toStdString().c_str(),
                    data[i].compare[1].toStdString().c_str(),
                    data[i].nofcompareValues.toStdString().c_str(),
                    data[i].value[0].toStdString().c_str(),
                    data[i].value[1].toStdString().c_str());
        }
        else
        {
            str1.sprintf("'%s',"
                         "'%s',"
                         "'%s',"
                         "'%s',"
                         "'%s',"
                         "'%s',",
                         data[i].indikator.toStdString().c_str(),
                         data[i].compare[0].toStdString().c_str(),
                    data[i].compare[1].toStdString().c_str(),
                    data[i].nofcompareValues.toStdString().c_str(),
                    data[i].value[0].toStdString().c_str(),
                    data[i].value[1].toStdString().c_str());
        }
        str += str1;
    }


    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 6"), qry.lastError().text().toLatin1().constData());
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 6a"), str);
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }

    qry.finish();
    {
        closeDb();
        m_mutex.unlock();
    }
    return true;
}




/****************************************************************
 *
 * Function:    dbDataSetFilterComboSel()
 *
 * Description: This function request data from database and update
 *              filter combo boxes with last user selection.
 *
 *
 *
 ****************************************************************/
bool  CDbHndl::dbDataSetFilterComboSel(CDbHndl::TableTabWidget_ST *tableTabW)
{
    QString indicator;
    QString str;
    QString str1;
    // QSqlQuery qry;
    QSqlRecord rec;
    int nofCompareValues;
    int i, j;



    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    // Create sql question
    str.sprintf("SELECT * FROM TblFilterStockDataSnapshot");


    // Execute sql question
    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 7"), qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }


    if(!qry.next())
    {
        closeDb();
        m_mutex.unlock();
        return false;
    }

    rec = qry.record();

    i = 0;
    j = 0;
    //QString debug;
    while( j < CDbHndl::MAX_NOF_TABLE_TAB_GUI_SEL_CTRLS)
    {
        // debug.sprintf("i=%d, j=%d", i, j);

        // Extract indicator
        indicator.sprintf("indicator%d", i);
        str1 = rec.value(indicator).toString();
        (void)comboBoxSetSelStr(tableTabW->indicatorComboBox[j], str1);


        // Extract first compare sign
        str.sprintf("compare%dA", i);
        str1 = rec.value(str).toString();
        (void)comboBoxSetSelStr(tableTabW->compareComboBox[j], str1);

        // Extract first indicator value
        str.sprintf("value%dA", i);
        str1 = rec.value(str).toString();
        tableTabW->IndicatorValueLineEdit[j]->setText(str1);


        // Extract number of compare values
        str.sprintf("nofCompareValues%d", i);
        str1 = rec.value(str).toString();
        nofCompareValues = atoi(str1.toStdString().c_str());


        if(nofCompareValues > 1)
        {
            j++;

            if(i >= CDbHndl::MAX_NOF_TABLE_TAB_GUI_SEL_CTRLS)
            {
                if(m_disableMsgBoxes == false)
                {
                    QMessageBox::critical(NULL, QString::fromUtf8("Error Combobox index"), QString::fromUtf8("Error Combobox index to big"));
                }
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }

            // Extract indicator
            //str.sprintf("indicator%d", i);
            str1 = rec.value(indicator).toString();
            (void)comboBoxSetSelStr(tableTabW->indicatorComboBox[j], str1);

            // Extract second compare sign
            str.sprintf("compare%dB", i);
            str1 = rec.value(str).toString();
            (void)comboBoxSetSelStr(tableTabW->compareComboBox[j], str1);

            // Extract second indicator value
            str.sprintf("value%dB", i);
            str1 = rec.value(str).toString();
            tableTabW->IndicatorValueLineEdit[j]->setText(str1);
        }
        i++;
        j++;
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;

}



/****************************************************************
 *
 * Function:    getNofStockSnapshotTblRecords()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::getNofStockSnapshotTblRecords(int &nofRow)
{
    // QSqlQuery qry;
    QString str;

    nofRow = 0;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);
    str.sprintf("SELECT Count(*) FROM TblStockDataSnapshot;");

    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 8"), qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }


    if (qry.next())
    {
        nofRow = qry.value(0).toInt();
    }
    else
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 9"), QString::fromUtf8("No data received"));
        }
        closeDb();
        m_mutex.unlock();
        return false;

    }

    qDebug( "Inserted!" );
    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;

}



/****************************************************************
 *
 * Function:    addCustomerSnabshotData()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::addCustomerSnabshotData(bool dbIsHandledExternly)
{
    QSqlRecord rec;
    // QSqlQuery qry;
    QString str;
    CData data;
    int row;
    bool res;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

    m_lst.deleteList();

    str.sprintf("SELECT companyName,"
                "lastPrice,"
                "keyValueEarningsPerShare,"
                "keyValueNAVPerShare,"
                "keyValueDividendPerShare,"
                "earningsDividedByDividend,"
                "navDivLastStockPrice"
                " FROM TblStockDataSnapshot;");

    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 10"), qry.lastError().text().toLatin1().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    for(row = 0;  qry.next(); row++)
    {
        rec = qry.record();

        if(rec.value("companyName").isNull()==true)
        {
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("addCustomerSnabshotData()"), QString::fromUtf8("Error: Stock name missing"));
            }
            data.stockName = QString::fromUtf8("-");
        }
        else
        {
            data.stockName = rec.value("companyName").toString();
        }


        if(rec.value("keyValueDividendPerShare").isNull()==true)
        {
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("addCustomerSnabshotData()"), QString::fromUtf8("Error: DividendPerShare is missing"));
            }
            data.keyValueDividendPerShare = QString::fromUtf8("-");
        }
        else
        {
            data.keyValueDividendPerShare = rec.value("keyValueDividendPerShare").toString();
        }


        if(rec.value("keyValueEarningsPerShare").isNull()==true)
        {
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("addCustomerSnabshotData()"), QString::fromUtf8("Error: EarningsPerShare is missing"));
            }
            data.keyValueEarningsPerShare = QString::fromUtf8("-");
        }
        else
        {
            data.keyValueEarningsPerShare = rec.value("keyValueEarningsPerShare").toString();
        }


        if(rec.value("keyValueNAVPerShare").isNull()==true)
        {
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("addCustomerSnabshotData()"), QString::fromUtf8("Error: NAVPerShare is missing"));
            }
            data.keyValueNAVPerShare = QString::fromUtf8("-");
        }
        else
        {
            data.keyValueNAVPerShare = rec.value("keyValueNAVPerShare").toString();
        }


        if(rec.value("lastPrice").isNull()==true)
        {
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("addCustomerSnabshotData()"), QString::fromUtf8("Error: lastPrice is missing"));
            }
            data.lastPrice = QString::fromUtf8("-");
        }
        else
        {
            data.lastPrice = rec.value("lastPrice").toString();
        }


        if(data.keyValueEarningsPerShare.compare(QString::fromUtf8("-")) != 0 && data.keyValueDividendPerShare.compare(QString::fromUtf8("-")) != 0)
        {
            str.sprintf("%.3lf", (data.keyValueEarningsPerShare.toDouble() / data.keyValueDividendPerShare.toDouble()));
            data.earningsDividedByDividend = str;
        }
        else
        {
            data.earningsDividedByDividend = QString::fromUtf8("-");
        }



        if(data.lastPrice.compare(QString::fromUtf8("-")) != true && data.keyValueNAVPerShare.compare(QString::fromUtf8("-")) != true)
        {
            str.sprintf("%.3lf", data.keyValueNAVPerShare.toDouble() / data.lastPrice.toDouble());
            data.navDivLastStockPrice = str;
        }
        else
        {
            data.navDivLastStockPrice = QString::fromUtf8("-");
        }

        m_lst.addNodeLast(data);

    }


    qry.finish();

#if 1

    res = m_lst.getFirst(data);

    while(res == true)
    {
        str.sprintf("UPDATE TblStockDataSnapshot "
                    " SET "
                    "earningsDividedByDividend = '%s', "
                    "navDivLastStockPrice = '%s' "
                    "WHERE companyName = '%s';"
                    , data.earningsDividedByDividend.toAscii().constData()
                    , data.navDivLastStockPrice.toAscii().constData()
                    , data.stockName.toLocal8Bit().constData());

        qry.prepare(str);


        if(!qry.exec())
        {
            qDebug() << qry.lastError();
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("Database Error 12"), qry.lastError().text().toLatin1().constData());
            }
            m_lst.deleteList();
            if(dbIsHandledExternly == false)
            {
                closeDb();
                m_mutex.unlock();
            }
            return false;
        }

        qry.finish();
        res = m_lst.getNext(data);
    }
#endif

    m_lst.deleteList();
    if(dbIsHandledExternly == false)
    {

        closeDb();
        m_mutex.unlock();
    }
    return true;
}




/****************************************************************
 *
 * Function:    insertOneRowKeyNumberData()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::insertOneRowKeyNumberData(bool dbIsHandledExternly)
{
    // QSqlQuery qry;
    QString str;


    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }
    QSqlQuery qry(m_db);
    str.sprintf("INSERT INTO TblStockDataSnapshot "
                "("
                "companyName,"
                "keyValuePE,"
                "keyValuePS,"
                "keyValueEarningsPerShare,"
                "keyValueNAVPerShare,"
                "keyValueDividendPerShare,"
                "keyValueYield,"
                "keyValueCoursePerJEK"
                ") "
                "VALUES("
                "'%s',"
                "'%s',"
                "'%s',"
                "'%s',"
                "'%s',"
                "'%s',"
                "'%s',"
                "'%s')",
                m_snapshotStockData.companyName.toLocal8Bit().constData(),
                //m_snapshotStockData.companyName.toLatin1().constData(),
                m_snapshotStockData.keyValuePE.toAscii().constData(),
                m_snapshotStockData.keyValuePS.toAscii().constData(),
                m_snapshotStockData.keyValueEarningsPerShare.toAscii().constData(),
                m_snapshotStockData.keyValueNAVPerShare.toAscii().constData(),
                m_snapshotStockData.keyValueDividendPerShare.toAscii().constData(),
                m_snapshotStockData.keyValueYield.toAscii().constData(),
                m_snapshotStockData.keyValueCoursePerJEK.toAscii().constData());

    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 14"), qry.lastError().text().toLatin1().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }

    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }
    return true;
}



/****************************************************************
 *
 * Function:    updateOneRowKeyNumberData()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::updateOneRowKeyNumberData(bool dbIsHandledExternly)
{
    //QSqlQuery qry;
    QString str;
    QString debugStr;


    if(false == findStockName(m_snapshotStockData.companyName, dbIsHandledExternly))
    {

        debugStr.sprintf("Update Row Key number. Stock name not found: %s\n", m_snapshotStockData.companyName.toAscii().constData());
        debugPrintToFile(debugStr, __LINE__, __FILE__);
    }

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }
    QSqlQuery qry(m_db);
    str.sprintf("UPDATE TblStockDataSnapshot "
                "SET "
                "keyValuePE = '%s',"
                "keyValuePS = '%s',"
                "keyValueEarningsPerShare = '%s',"
                "keyValueNAVPerShare = '%s',"
                "keyValueDividendPerShare = '%s',"
                "keyValueYield = '%s',"
                "keyValueCoursePerJEK = '%s' "
                "WHERE  companyName = '%s'",
                m_snapshotStockData.keyValuePE.toAscii().constData(),
                m_snapshotStockData.keyValuePS.toAscii().constData(),
                m_snapshotStockData.keyValueEarningsPerShare.toAscii().constData(),
                m_snapshotStockData.keyValueNAVPerShare.toAscii().constData(),
                m_snapshotStockData.keyValueDividendPerShare.toAscii().constData(),
                m_snapshotStockData.keyValueYield.toAscii().constData(),
                m_snapshotStockData.keyValueCoursePerJEK.toAscii().constData(),
                m_snapshotStockData.companyName.toLocal8Bit().constData());

    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 16"), qry.lastError().text().toLatin1().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }

    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }
    return true;
}






/****************************************************************
 *
 * Function:    insertOneRowYieldData()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::insertOneRowYieldData(bool dbIsHandledExternly)
{
    QSqlQuery qry;
    QString str;


    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    str.sprintf("INSERT INTO TblStockDataSnapshot "
                "("
                "companyName,"
                "procentChangeOneDay,"
                "procentChangeOneWeek,"
                "procentChange1Month,"
                "procentChange3Month,"
                "procentChange6Month,"
                "procentChange1Year,"
                "procentChange2Year,"
                "procentChange3Year,"
                "procentChange5Year"
                ") "
                "VALUES("
                "'%s',"
                "'%s',"
                "'%s',"
                "'%s',"
                "'%s',"
                "'%s',"
                "'%s',"
                "'%s',"
                "'%s',"
                "'%s')",
                m_snapshotStockData.companyName.toLocal8Bit().constData(),
                m_snapshotStockData.procentChangeOneDay.toAscii().constData(),
                m_snapshotStockData.procentChangeOneWeek.toAscii().constData(),
                m_snapshotStockData.procentChange1Month.toAscii().constData(),
                m_snapshotStockData.procentChange3Month.toAscii().constData(),
                m_snapshotStockData.procentChange6Month.toAscii().constData(),
                m_snapshotStockData.procentChange1Year.toAscii().constData(),
                m_snapshotStockData.procentChange2Year.toAscii().constData(),
                m_snapshotStockData.procentChange3Year.toAscii().constData(),
                m_snapshotStockData.procentChange5Year.toAscii().constData());

    qry.prepare(str);



    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 18"), qry.lastError().text().toLatin1().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }
    return true;
}



/****************************************************************
 *
 * Function:    findStockName()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::findStockName(QString stockName, bool dbIsHandledExternly)
{

    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }
    QSqlQuery qry(m_db);


    str.sprintf("SELECT companyName "
                " FROM TblStockDataSnapshot "
                " WHERE companyName = '%s';", stockName.toLocal8Bit().constData());


    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 26"), qry.lastError().text().toLatin1().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    if(!qry.next())
    {
        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }
    return true;
}



/****************************************************************
 *
 * Function:    updateOneRowYieldData()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::updateOneRowYieldData(bool dbIsHandledExternly)
{

    QString str;
    QString debugStr;

    if(false == findStockName(m_snapshotStockData.companyName, dbIsHandledExternly))
    {

        debugStr.sprintf("Update Row Yield. Stock name not found: %s\n", m_snapshotStockData.companyName.toAscii().constData());
        debugPrintToFile(debugStr, __LINE__, __FILE__);
    }


    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }
    QSqlQuery qry(m_db);

    str.sprintf("UPDATE TblStockDataSnapshot "
                "SET "
                "procentChangeOneDay = '%s', "
                "procentChangeOneWeek = '%s', "
                "procentChange1Month = '%s', "
                "procentChange3Month = '%s', "
                "procentChange6Month = '%s', "
                "procentChange1Year = '%s', "
                "procentChange2Year = '%s', "
                "procentChange3Year = '%s', "
                "procentChange5Year  = '%s' "
                "WHERE  companyName = '%s'",
                m_snapshotStockData.procentChangeOneDay.toAscii().constData(),
                m_snapshotStockData.procentChangeOneWeek.toAscii().constData(),
                m_snapshotStockData.procentChange1Month.toAscii().constData(),
                m_snapshotStockData.procentChange3Month.toAscii().constData(),
                m_snapshotStockData.procentChange6Month.toAscii().constData(),
                m_snapshotStockData.procentChange1Year.toAscii().constData(),
                m_snapshotStockData.procentChange2Year.toAscii().constData(),
                m_snapshotStockData.procentChange3Year.toAscii().constData(),
                m_snapshotStockData.procentChange5Year.toAscii().constData(),
                m_snapshotStockData.companyName.toLocal8Bit().constData());


    qry.prepare(str);

    if( !qry.exec() )
    {
        qDebug() << qry.lastError();
        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }



    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }
    return true;
}




/****************************************************************
 *
 * Function:    insertOneRowPriceData()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::insertOneRowPriceData(bool dbIsHandledExternly)
{
    //QSqlQuery qry;
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }
    QSqlQuery qry(m_db);
    str.sprintf("INSERT INTO TblStockDataSnapshot"
                "("
                "companyName,"
                "lastPrice,"
                "priceChange,"
                "procentChangeOneDay,"
                "bidPrice,"
                "offerPrice,"
                "highestPrice,"
                "lowestPrice,"
                "volume,"
                "currency,"
                "time"
                //",date,"
                ") "
                "VALUES("
                "'%s',"
                "'%s',"
                "'%s',"
                "'%s',"
                "'%s',"
                "'%s',"
                "'%s',"
                "'%s',"
                "'%s',"
                "'%s',"
                "'%s')",
                m_snapshotStockData.companyName.toLocal8Bit().constData(),
                m_snapshotStockData.lastPrice.toAscii().constData(),
                m_snapshotStockData.priceChange.toAscii().constData(),
                m_snapshotStockData.procentChangeOneDay.toAscii().constData(),
                m_snapshotStockData.bidPrice.toAscii().constData(),
                m_snapshotStockData.offerPrice.toAscii().constData(),
                m_snapshotStockData.highestPrice.toAscii().constData(),
                m_snapshotStockData.lowestPrice.toAscii().constData(),
                m_snapshotStockData.volume.toAscii().constData(),
                m_snapshotStockData.currency.toAscii().constData(),
                m_snapshotStockData.time.toAscii().constData());

    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 20"), qry.lastError().text().toLatin1().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }
    return true;
}




/****************************************************************
 *
 * Function:    initIndicatorToComboBoxes()
 *
 * Description:.
 *
 *
 *
 ****************************************************************/
void CDbHndl::
initIndicatorToComboBoxes(CDbHndl::TableTabWidget_ST *w)
{
    CGuiUtils gu;


    gu.addindicatorToComboBox(w->indicatorComboBox[TTAB_GUI_CTRL1], m_tableHeaderList, m_maxTableColumns);
    gu.addindicatorToComboBox(w->indicatorComboBox[TTAB_GUI_CTRL2], m_tableHeaderList, m_maxTableColumns);
    gu.addindicatorToComboBox(w->indicatorComboBox[TTAB_GUI_CTRL3], m_tableHeaderList, m_maxTableColumns);
    gu.addindicatorToComboBox(w->indicatorComboBox[TTAB_GUI_CTRL4], m_tableHeaderList, m_maxTableColumns);
    gu.addindicatorToComboBox(w->indicatorComboBox[TTAB_GUI_CTRL5], m_tableHeaderList, m_maxTableColumns);
    gu.addindicatorToComboBox(w->indicatorComboBox[TTAB_GUI_CTRL6], m_tableHeaderList, m_maxTableColumns);
    gu.addindicatorToComboBox(w->indicatorComboBox[TTAB_GUI_CTRL7], m_tableHeaderList, m_maxTableColumns);

}





/****************************************************************
 *
 * Function:    initSortCompareToComboBoxes()
 *
 * Description:.
 *
 *
 *
 ****************************************************************/
void CDbHndl::initSortCompareToComboBoxes(CDbHndl::TableTabWidget_ST *w)
{
    CGuiUtils gu;

    gu.addSortCompareToComboBox(w->compareComboBox[TTAB_GUI_CTRL1]);
    gu.addSortCompareToComboBox(w->compareComboBox[TTAB_GUI_CTRL2]);
    gu.addSortCompareToComboBox(w->compareComboBox[TTAB_GUI_CTRL3]);
    gu.addSortCompareToComboBox(w->compareComboBox[TTAB_GUI_CTRL4]);
    gu.addSortCompareToComboBox(w->compareComboBox[TTAB_GUI_CTRL5]);
    gu.addSortCompareToComboBox(w->compareComboBox[TTAB_GUI_CTRL6]);
    gu.addSortCompareToComboBox(w->compareComboBox[TTAB_GUI_CTRL7]);

}





/****************************************************************
 *
 * Function:    addOneSlotTableHeaderData()
 *
 * Description:.
 *
 *
 *
 ****************************************************************/
void CDbHndl::
addOneSlotTableHeaderData(int tableHeaderIndex, CDbHndl::GuiParamSelIndex_ET indicatorIndex)
{
    m_tableHeaderList[tableHeaderIndex].indicatorlMask = m_gpSelLookUpTable[indicatorIndex].indicatorlMask;
    m_tableHeaderList[tableHeaderIndex].name = (QString)m_gpSelLookUpTable[indicatorIndex].name;
    m_tableHeaderList[tableHeaderIndex].nameInDb = (QString)m_gpSelLookUpTable[indicatorIndex].nameInDb;
    m_tableHeaderList[tableHeaderIndex].sortType = m_gpSelLookUpTable[indicatorIndex].sortType;
}




/****************************************************************
 *
 * Function:    filterAndSortCreateSqlQry()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
filterAndSortCreateSqlQry(QString &str, QString sortParam, QString sortOrder)
{
    QString tmp;

    createSelectQryUserSelStockParams(str);


    if(false == filterAndSortCreateWherePart(tmp, sortParam, sortOrder))
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::information(NULL, QString::fromUtf8("Error"), QString::fromUtf8("false == filterAndSortCreateWherePart(tmp, sortParam, sortOrder)") );
        }
        return false;
    }

    str += tmp;
    return true;

}



/****************************************************************
 *
 * Function:    filterCreateSqlQry()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
bool CDbHndl::filterCreateSqlQry(QString &str)
{
    QString tmp;

    createSelectQryUserSelStockParams(str);

    if(false == filterCreateWherePart(tmp))
    {
        tmp = ";";
        str += tmp;
        return true;
    }

    str += tmp;
    return true;

}



/****************************************************************
 *
 * Function:    filter1DividendGetColumnSortParameter()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
bool CDbHndl::filter1DividendGetColumnSortParameter(int index, QString &result)
{
    if(m_maxTableColumns > index)
    {
        result = m_tableHeaderList[index].nameInDb;
        return true;
    }

    return false;

}



/****************************************************************
 *
 * Function:    filter1Dividend()
 *
 * Description:.
 *
 *
 * tableView->resizeRowsToContents(); // Adjust the row height.
 * tableView->resizeColumnsToContents(); // Adjust the column width.
 * tableView->setColumnWidth( 0, 45 );
 * tableView->setColumnWidth( 1, 130 );
 *
 ****************************************************************/
bool CDbHndl::filter1Dividend(QObject *thisPointer, QTableView *tableView, CDbHndl::TableTabWidget_ST *w,
                              QString sortParam,
                              QString sortOrder, bool useFilter)
{
    QString str;
    CExtendedTable et;

    QSqlRecord rec;
    int row;
    int col;

    guiFilterCtrlResetContents(w);
    getNofStockSnapshotTblRecords(row);
    et.createTableModel(row, m_maxTableColumns, thisPointer);
    et.addHeaders(tableView, m_tableHeaderList, m_maxTableColumns);

    initSortCompareToComboBoxes(w);
    initIndicatorToComboBoxes(w);

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    if(useFilter == false)
    {
        if(false == filterCreateSqlQry(str))
        {
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::information(NULL, QString::fromUtf8("Fail 1 filterAndSortCreateSqlQry()"), str );
            }

            closeDb();
            m_mutex.unlock();
            return false;
        }
    }
    else
    {
        if(false == filterAndSortCreateSqlQry(str, sortParam, sortOrder))
        {
            if(m_disableMsgBoxes == false)
            {
                QMessageBox::information(NULL, QString::fromUtf8("Fail 2 filterAndSortCreateSqlQry()"), str );
            }

            closeDb();
            m_mutex.unlock();
            return false;
        }
    }

    // Execute sql question
    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 22"), qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }



    for(row = 0;  qry.next(); row++)
    {
        rec = qry.record();

        for(col = 0; col < m_maxTableColumns; col++)
        {

            str = rec.value(m_tableHeaderList[col].nameInDb).toString();
            et.addSnappshotData(tableView, m_tableHeaderList[col].nameInDb, str, row, col, rec);
        }
    }



    qry.finish();
    closeDb();
    m_mutex.unlock();
    tableView->resizeColumnsToContents();
    tableView->resize(1100,500);
    return true;
}


/****************************************************************
 *
 * Function:    initFilter1Dividend()
 *
 * Description:.
 *
 *
 * tableView->resizeRowsToContents(); // Adjust the row height.
 * tableView->resizeColumnsToContents(); // Adjust the column width.
 * tableView->setColumnWidth( 0, 45 );
 * tableView->setColumnWidth( 1, 130 );
 *
 ****************************************************************/
void CDbHndl::
createSelectQryUserSelStockParams(QString &str)
{
    int i;


    str.sprintf("SELECT ");

    for(i=0; i < m_maxTableColumns; i++)
    {
        //str += "'";
        str += m_tableHeaderList[i].nameInDb;
        //str += "' ";

        if(i < (m_maxTableColumns - 1))
            str += ", ";
        else
            str += " ";
    }

    str += " FROM TblStockDataSnapshot";
}





/****************************************************************
 *
 * Function:    initFilter1Dividend()
 *
 * Description:.
 *
 *
 * tableView->resizeRowsToContents(); // Adjust the row height.
 * tableView->resizeColumnsToContents(); // Adjust the column width.
 * tableView->setColumnWidth( 0, 45 );
 * tableView->setColumnWidth( 1, 130 );
 *
 ****************************************************************/
bool CDbHndl::
initFilter1Dividend(QObject *thisPointer, QTableView *tableView, CDbHndl::TableTabWidget_ST *w, bool dbIsHandledExternly)
{
    QString str;
    CExtendedTable et;

    //QSqlQuery qry;
    QSqlRecord rec;
    int row;
    int col;


    // Crete table model with enough space
    getNofStockSnapshotTblRecords(row);
    et.createTableModel(row, m_maxTableColumns, thisPointer);
    et.addHeaders(tableView, m_tableHeaderList, m_maxTableColumns);

    initSortCompareToComboBoxes(w);
    initIndicatorToComboBoxes(w);
    (void)dbDataSetFilterComboSel(w);

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }
    QSqlQuery qry(m_db);

    createSelectQryUserSelStockParams(str);

    // Execute sql question
    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 23"), qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }


    for(row = 0;  qry.next(); row++)
    {
        rec = qry.record();

        for(col = 0; col < m_maxTableColumns; col++)
        {
            str = rec.value(m_tableHeaderList[col].nameInDb).toString();
            et.addSnappshotData(tableView, m_tableHeaderList[col].nameInDb, str, row, col, rec);
        }
    }

    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
    }
    m_mutex.unlock();
    tableView->resizeColumnsToContents();
    tableView->resize(1100,500);
    return true;
}

/****************************************************************
 *
 * Function:    copyDbPriceToTable()
 *
 * Description:.
 *
 *
 * tableView->resizeRowsToContents(); // Adjust the row height.
 * tableView->resizeColumnsToContents(); // Adjust the column width.
 * tableView->setColumnWidth( 0, 45 );
 * tableView->setColumnWidth( 1, 130 );
 *
 ****************************************************************/
bool  CDbHndl::copyDbPriceToTable(QObject *thisPointer, QTableView *tableView)
{
    QString str;
    CExtendedTable et;

    //QSqlQuery qry;
    QSqlRecord rec;
    int row;
    int col;


    // Crete table model with enough space
    getNofStockSnapshotTblRecords(row);
    col = MAX_NOF_SNAPSHOT_PRICE_DATA_COLUMNS;
    et.createTableModel(row, col, thisPointer);


    m_maxTableColumns = 0;

    m_tableHeaderList[m_maxTableColumns++].name = QString::fromUtf8("Company");
    m_tableHeaderList[m_maxTableColumns++].name = QString::fromUtf8("Price");
    m_tableHeaderList[m_maxTableColumns++].name = QString::fromUtf8("+/-");
    m_tableHeaderList[m_maxTableColumns++].name = QString::fromUtf8("%");
    m_tableHeaderList[m_maxTableColumns++].name = QString::fromUtf8("Bid");
    m_tableHeaderList[m_maxTableColumns++].name = QString::fromUtf8("Offer");
    m_tableHeaderList[m_maxTableColumns++].name = QString::fromUtf8("Highest");
    m_tableHeaderList[m_maxTableColumns++].name = QString::fromUtf8("Lowest");
    m_tableHeaderList[m_maxTableColumns++].name = QString::fromUtf8("Volume");
    m_tableHeaderList[m_maxTableColumns++].name = QString::fromUtf8("Currency");
    m_tableHeaderList[m_maxTableColumns++].name = QString::fromUtf8("Time");

    // et.addHeaders(tableView/*, headerList*/);
    et.addHeaders(tableView, m_tableHeaderList, m_maxTableColumns);

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    // Create sql question
    str.sprintf("SELECT "
                "companyName,"
                "lastPrice,"
                "priceChange,"
                "procentChangeOneDay,"
                "bidPrice,"
                "offerPrice,"
                "highestPrice,"
                "lowestPrice,"
                "volume,"
                "currency,"
                "time"
                " FROM TblStockDataSnapshot");


    // Execute sql question
    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 24"), qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }


    for(row = 0;  qry.next(); row++)
    {
        rec = qry.record();

        col = 0;
        str = rec.value("companyName").toString();
        et.addData(tableView, str, row, col);

        col++;
        str = rec.value("lastPrice").toString();
        et.addData(tableView, str, row, col);

        col++;
        str = rec.value("priceChange").toString();
        et.addNumberIndicateSignWithColor(tableView, str, row, col);
        //et.addData(tableView, str, row, col);

        col++;
        str = rec.value("procentChangeOneDay").toString();
        et.addNumberIndicateSignWithColor(tableView, str, row, col);
        //et.addData(tableView, str, row, col);

        col++;
        str = rec.value("bidPrice").toString();
        et.addData(tableView, str, row, col);

        col++;
        str = rec.value("offerPrice").toString();
        et.addData(tableView, str, row, col);

        col++;
        str = rec.value("highestPrice").toString();
        et.addData(tableView, str, row, col);


        col++;
        str = rec.value("lowestPrice").toString();
        et.addData(tableView, str, row, col);


        col++;
        str = rec.value("volume").toString();
        et.addData(tableView, str, row, col);

        col++;
        str = rec.value("currency").toString();
        et.addData(tableView, str, row, col);

        col++;
        str = rec.value("time").toString();
        et.addData(tableView, str, row, col);


    }

    qry.finish();
    closeDb();
    m_mutex.unlock();
    tableView->resizeColumnsToContents();
    tableView->resize(1100,600);
    return true;

}




/****************************************************************
 *
 * Function:    copyDbYieldToTable()
 *
 * Description:.
 *
 *
 * tableView->resizeRowsToContents(); // Adjust the row height.
 * tableView->resizeColumnsToContents(); // Adjust the column width.
 * tableView->setColumnWidth( 0, 45 );
 * tableView->setColumnWidth( 1, 130 );
 *
 ****************************************************************/
bool  CDbHndl::copyDbYieldToTable(QObject *thisPointer, QTableView *tableView)
{
    QString str;
    CExtendedTable et;

    // QSqlQuery qry;
    QSqlRecord rec;
    int row;
    int col;


    // Crete table model with enough space
    getNofStockSnapshotTblRecords(row);
    col = MAX_NOF_SNAPSHOT_YIELD_DATA_COLUMNS;
    et.createTableModel(row, col, thisPointer/*g_tableTabThisPointer*/);



    m_maxTableColumns = 0;

    m_tableHeaderList[m_maxTableColumns++].name = QString::fromUtf8("Company");
    m_tableHeaderList[m_maxTableColumns++].name = QString::fromUtf8("1 Day");
    m_tableHeaderList[m_maxTableColumns++].name = QString::fromUtf8("1 Week");
    m_tableHeaderList[m_maxTableColumns++].name = QString::fromUtf8("1 Month");
    m_tableHeaderList[m_maxTableColumns++].name = QString::fromUtf8("3 Month");
    m_tableHeaderList[m_maxTableColumns++].name = QString::fromUtf8("6 Month");
    m_tableHeaderList[m_maxTableColumns++].name = QString::fromUtf8("1 Year");
    m_tableHeaderList[m_maxTableColumns++].name = QString::fromUtf8("2 Year");
    m_tableHeaderList[m_maxTableColumns++].name = QString::fromUtf8("3 Year");
    m_tableHeaderList[m_maxTableColumns++].name = QString::fromUtf8("5 Year");


    //et.addHeaders(tableView, headerList);

    et.addHeaders(tableView, m_tableHeaderList, m_maxTableColumns);

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    // Create sql question
    str.sprintf("SELECT "
                "companyName,"
                "procentChangeOneDay,"
                "procentChangeOneWeek,"
                "procentChange1Month,"
                "procentChange3Month,"
                "procentChange6Month,"
                "procentChange1Year,"
                "procentChange2Year,"
                "procentChange3Year,"
                "procentChange5Year"
                " FROM TblStockDataSnapshot");


    // Execute sql question
    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 25"), qry.lastError().text().toLatin1().constData());
        }

        closeDb();
        m_mutex.unlock();
        return false;
    }


    for(row = 0;  qry.next(); row++)
    {
        rec = qry.record();

        col = 0;
        str = rec.value("companyName").toString();
        et.addData(tableView, str, row, col);

        col++;
        str = rec.value("procentChangeOneDay").toString();
        et.addNumberIndicateSignWithColor(tableView, str, row, col);

        col++;
        str = rec.value("procentChangeOneWeek").toString();
        et.addNumberIndicateSignWithColor(tableView, str, row, col);

        col++;
        str = rec.value("procentChange1Month").toString();
        et.addNumberIndicateSignWithColor(tableView, str, row, col);


        col++;
        str = rec.value("procentChange3Month").toString();
        et.addNumberIndicateSignWithColor(tableView, str, row, col);

        col++;
        str = rec.value("procentChange6Month").toString();
        et.addNumberIndicateSignWithColor(tableView, str, row, col);

        col++;
        str = rec.value("procentChange1Year").toString();
        et.addNumberIndicateSignWithColor(tableView, str, row, col);


        col++;
        str = rec.value("procentChange2Year").toString();
        et.addNumberIndicateSignWithColor(tableView, str, row, col);


        col++;
        str = rec.value("procentChange3Year").toString();
        et.addNumberIndicateSignWithColor(tableView, str, row, col);

        col++;
        str = rec.value("procentChange5Year").toString();
        et.addNumberIndicateSignWithColor(tableView, str, row, col);
    }

    qry.finish();
    closeDb();
    m_mutex.unlock();
    tableView->resizeColumnsToContents();
    tableView->resize(1100,600);
    return true;

}



/****************************************************************
 *
 * Function:    copyDbKeyNumberDataToTable()
 *
 * Description:.
 *
 *
 * tableView->resizeRowsToContents(); // Adjust the row height.
 * tableView->resizeColumnsToContents(); // Adjust the column width.
 * tableView->setColumnWidth( 0, 45 );
 * tableView->setColumnWidth( 1, 130 );
 *
 ****************************************************************/
bool  CDbHndl::copyDbKeyNumberToTable(QObject *thisPointer, QTableView *tableView)
{
    QString str;
    CExtendedTable et;

    //QSqlQuery qry;
    QSqlRecord rec;
    int row;
    int col;


    // Crete table model with enough space
    getNofStockSnapshotTblRecords(row);
    col = MAX_NOF_SNAPSHOT_KEY_NUMBER_DATA_COLUMNS;
    et.createTableModel(row, col, thisPointer);



    m_maxTableColumns = 0;

    m_tableHeaderList[m_maxTableColumns++].name = QString::fromUtf8("Company");
    m_tableHeaderList[m_maxTableColumns++].name = QString::fromUtf8("PE");
    m_tableHeaderList[m_maxTableColumns++].name = QString::fromUtf8("PS");
    m_tableHeaderList[m_maxTableColumns++].name = QString::fromUtf8("Earn/Share");
    m_tableHeaderList[m_maxTableColumns++].name = QString::fromUtf8("NAV/Share");
    m_tableHeaderList[m_maxTableColumns++].name = QString::fromUtf8("Div/Share");
    m_tableHeaderList[m_maxTableColumns++].name = QString::fromUtf8("Yield");
    m_tableHeaderList[m_maxTableColumns++].name = QString::fromUtf8("Course/JEK");



    // et.addHeaders(tableView, headerList);
    et.addHeaders(tableView, m_tableHeaderList, m_maxTableColumns);

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    // Create sql question
    str.sprintf("SELECT "
                "companyName,"
                "keyValuePE,"
                "keyValuePS,"
                "keyValueEarningsPerShare,"
                "keyValueNAVPerShare,"
                "keyValueDividendPerShare,"
                "keyValueYield,"
                "keyValueCoursePerJEK"
                " FROM TblStockDataSnapshot");


    // Execute sql question
    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 26"), qry.lastError().text().toLatin1().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }


    for(row = 0;  qry.next(); row++)
    {
        rec = qry.record();

        col = 0;
        str = rec.value("companyName").toString();
        et.addData(tableView, str, row, col);

        col++;
        str = rec.value("keyValuePE").toString();
        et.addData(tableView, str, row, col);

        col++;
        str = rec.value("keyValuePS").toString();
        et.addData(tableView, str, row, col);

        col++;
        str = rec.value("keyValueEarningsPerShare").toString();
        et.addData(tableView, str, row, col);

        col++;
        str = rec.value("keyValueNAVPerShare").toString();
        et.addData(tableView, str, row, col);

        col++;
        str = rec.value("keyValueDividendPerShare").toString();
        et.addData(tableView, str, row, col);

        col++;
        str = rec.value("keyValueYield").toString();
        et.addData(tableView, str, row, col);


        col++;
        str = rec.value("keyValueCoursePerJEK").toString();
        et.addData(tableView, str, row, col);


    }

    qry.finish();
    closeDb();
    m_mutex.unlock();
    tableView->resizeColumnsToContents();
    tableView->resize(1100,600);
    return true;

}





/****************************************************************
 *
 * Function:    selectData()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::selectData(void)
{
    QSqlQuery qry;

    qry.prepare( "SELECT * FROM names" );

    if( !qry.exec() )
        qDebug() << qry.lastError();
    else
    {
        qDebug( "Selected!" );

        QSqlRecord rec = qry.record();

        int cols = rec.count();

        for( int c=0; c<cols; c++ )
            qDebug() << QString( "Column %1: %2" ).arg( c ).arg( rec.fieldName(c) );

        for( int r=0; qry.next(); r++ )
            for( int c=0; c<cols; c++ )
                qDebug() << QString( "Row %1, %2: %3" ).arg( r ).arg( rec.fieldName(c) ).arg( qry.value(c).toString() );
    }


    qry.prepare( "SELECT firstname, lastname FROM names WHERE lastname = 'Roe'" );
    if( !qry.exec() )
        qDebug() << qry.lastError();
    else
    {
        qDebug( "Selected!" );

        QSqlRecord rec = qry.record();

        int cols = rec.count();

        for( int c=0; c<cols; c++ )
            qDebug() << QString( "Column %1: %2" ).arg( c ).arg( rec.fieldName(c) );

        for( int r=0; qry.next(); r++ )
            for( int c=0; c<cols; c++ )
                qDebug() << QString( "Row %1, %2: %3" ).arg( r ).arg( rec.fieldName(c) ).arg( qry.value(c).toString() );
    }


    qry.prepare( "SELECT firstname, lastname FROM names WHERE lastname = 'Roe' ORDER BY firstname" );
    if( !qry.exec() )
        qDebug() << qry.lastError();
    else
    {
        qDebug( "Selected!" );

        QSqlRecord rec = qry.record();

        int cols = rec.count();

        for( int c=0; c<cols; c++ )
            qDebug() << QString( "Column %1: %2" ).arg( c ).arg( rec.fieldName(c) );

        for( int r=0; qry.next(); r++ )
            for( int c=0; c<cols; c++ )
                qDebug() << QString( "Row %1, %2: %3" ).arg( r ).arg( rec.fieldName(c) ).arg( qry.value(c).toString() );
    }



    qry.prepare( "SELECT lastname, COUNT(*) as 'members' FROM names GROUP BY lastname ORDER BY lastname" );
    if( !qry.exec() )
        qDebug() << qry.lastError();
    else
    {
        qDebug( "Selected!" );

        QSqlRecord rec = qry.record();

        int cols = rec.count();

        for( int c=0; c<cols; c++ )
            qDebug() << QString( "Column %1: %2" ).arg( c ).arg( rec.fieldName(c) );

        for( int r=0; qry.next(); r++ )
            for( int c=0; c<cols; c++ )
                qDebug() << QString( "Row %1, %2: %3" ).arg( r ).arg( rec.fieldName(c) ).arg( qry.value(c).toString() );
    }

    return true;

}


/****************************************************************
 *
 * Function:    updateData()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::updateData(void)
{

    QSqlQuery qry;

    qry.prepare( "UPDATE names SET firstname = 'Nisse', lastname = 'Svensson' WHERE id = 7" );
    if( !qry.exec() )
        qDebug() << qry.lastError();
    else
        qDebug( "Updated!" );

    qry.prepare( "UPDATE names SET lastname = 'Johnson' WHERE firstname = 'Jane'" );
    if( !qry.exec() )
        qDebug() << qry.lastError();
    else
        qDebug( "Updated!" );

    return true;
}



/****************************************************************
 *
 * Function:    updateData()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::deleteData(void)
{

    QSqlQuery qry;

    qry.prepare( "DELETE FROM names WHERE id = 7" );
    if( !qry.exec() )
        qDebug() << qry.lastError();
    else
        qDebug( "Deleted!" );

    qry.prepare( "DELETE FROM names WHERE lastname = 'Johnson'" );
    if( !qry.exec() )
        qDebug() << qry.lastError();
    else
        qDebug( "Deleted!" );

    return true;
}


/****************************************************************
 *
 * Function:    resetSnapshotRamData()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
void CDbHndl::resetSnapshotRamData(snapshotStockData_ST &snapshotStockData, bool dbIsHandledExternly)
{
    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
    }

    snapshotStockData.bidPrice.clear();
    snapshotStockData.companyName.clear();
    snapshotStockData.currency.clear();
    snapshotStockData.date.clear();
    snapshotStockData.highestPrice.clear();
    snapshotStockData.keyValueCoursePerJEK.clear();
    snapshotStockData.keyValueDividendPerShare.clear();
    snapshotStockData.keyValueEarningsPerShare.clear();
    snapshotStockData.keyValueNAVPerShare.clear();
    snapshotStockData.keyValuePE.clear();
    snapshotStockData.keyValuePS.clear();
    snapshotStockData.keyValueYield.clear();
    snapshotStockData.lastPrice.clear();
    snapshotStockData.lowestPrice.clear();
    snapshotStockData.offerPrice.clear();
    snapshotStockData.priceChange.clear();
    snapshotStockData.procentChange1Month.clear();
    snapshotStockData.procentChange1Year.clear();
    snapshotStockData.procentChange2Year.clear();
    snapshotStockData.procentChange3Month.clear();
    snapshotStockData.procentChange3Year.clear();
    snapshotStockData.procentChange5Year.clear();
    snapshotStockData.procentChange6Month.clear();
    snapshotStockData.procentChangeOneDay.clear();
    snapshotStockData.procentChangeOneWeek.clear();
    snapshotStockData.time.clear();
    snapshotStockData.volume.clear();

    if(dbIsHandledExternly == false)
    {
        m_mutex.unlock();
    }
}




/****************************************************************
 *
 * Function:    insertData()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::insertData(void)
{
    QSqlQuery qry;

    qry.prepare( "INSERT INTO names (id, firstname, lastname) VALUES (1, 'John', 'Doe')" );
    if( !qry.exec() )
        qDebug() << qry.lastError();
    else
        qDebug( "Inserted!" );

    qry.prepare( "INSERT INTO names (id, firstname, lastname) VALUES (2, 'Jane', 'Doe')" );
    if( !qry.exec() )
        qDebug() << qry.lastError();
    else
        qDebug( "Inserted!" );

    qry.prepare( "INSERT INTO names (id, firstname, lastname) VALUES (3, 'James', 'Doe')" );
    if( !qry.exec() )
        qDebug() << qry.lastError();
    else
        qDebug( "Inserted!" );

    qry.prepare( "INSERT INTO names (id, firstname, lastname) VALUES (4, 'Judy', 'Doe')" );
    if( !qry.exec() )
        qDebug() << qry.lastError();
    else
        qDebug( "Inserted!" );

    qry.prepare( "INSERT INTO names (id, firstname, lastname) VALUES (5, 'Richard', 'Roe')" );
    if( !qry.exec() )
        qDebug() << qry.lastError();
    else
        qDebug( "Inserted!" );

    qry.prepare( "INSERT INTO names (id, firstname, lastname) VALUES (6, 'Jane', 'Roe')" );
    if( !qry.exec() )
        qDebug() << qry.lastError();
    else
        qDebug( "Inserted!" );

    qry.prepare( "INSERT INTO names (id, firstname, lastname) VALUES (7, 'John', 'Noakes')" );
    if( !qry.exec() )
        qDebug() << qry.lastError();
    else
        qDebug( "Inserted!" );

    qry.prepare( "INSERT INTO names (id, firstname, lastname) VALUES (8, 'Donna', 'Doe')" );
    if( !qry.exec() )
        qDebug() << qry.lastError();
    else
        qDebug( "Inserted!" );

    qry.prepare( "INSERT INTO names (id, firstname, lastname) VALUES (9, 'Ralph', 'Roe')" );
    if( !qry.exec() )
        qDebug() << qry.lastError();
    else
        qDebug( "Inserted!" );

    return true;
}
