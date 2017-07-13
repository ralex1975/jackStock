#include "parseomxallfinancelinkdata.h"
#include <QMessageBox>
#include <QtGui>
#include <QtCore>
#include <QString>
#include <QObject>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlError>
#include "util.h"
#include "dbHndl.h"
#include <QMessageBox>


#define TAG_PRICE_START_END_PART_SECOND_DATA QString::fromUtf8("%</DIV></TD>")

#define MAX_NOF_START_DATA_TAGS 2
#define MAX_STR_LEN_START_DATA_TAGS 200
const char startDataTagArr[MAX_NOF_START_DATA_TAGS][MAX_STR_LEN_START_DATA_TAGS] =
{
    "    <tr id=\"searchSharesListTable-",
    "          <a title=\"Fact Sheet\" href=\""
};


enum ParseStateET
{
    PSTATE_READ_STOCK_NAME_START_TAG,
    PSTATE_GET_STOCK_NAME,
    PSTATE_READ_LINK_START_TAG,
    PSTATE_GET_LINK

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
ParseOmxAllFinanceLinkData::ParseOmxAllFinanceLinkData()
{
  m_tmpEquityDatatmpEquityDatIsClear = false;
}




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
bool ParseOmxAllFinanceLinkData::parseFinanceLinks(QString filename)
{
        CUtil cu;
        QFile file(filename);
        QString errStr = QString("Fail to open file: %1").arg(filename);

        QString str;
        QString result;
        QString startTag;
        int index;
        ParseStateET state = PSTATE_READ_STOCK_NAME_START_TAG;

        if(!file.open(QFile::ReadOnly | QFile::Text))
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Fail to open file"), errStr);
            return false;
        }


        qDebug() << "============Start==================";
        QTextStream inStream(&file);
        inStream.setCodec("UTF-8");


        while(!inStream.atEnd())
        {
            switch(state)
            {
            case PSTATE_READ_STOCK_NAME_START_TAG:
                startTag = startDataTagArr[0];
                state = PSTATE_GET_STOCK_NAME;
                // break;

            case PSTATE_GET_STOCK_NAME:
                str = inStream.readLine();
                //qDebug() << str;
                index = str.indexOf(startTag);
                if(index > -1)
                {
                    index = str.indexOf("title=");
                    //qDebug() << index;

                    result = str.right(str.length() - index);
                    //qDebug() << "result" << result;

                    index = result.indexOf(" - ");
                    result = result.right(result.length() - index -3);
                    //qDebug() << "result1" << result;

                    index = result.indexOf("\" class=\"even\">");

                    if(index == -1)
                    {
                        index = result.indexOf("\" class=\"odd\">");
                    }

                    result = result.left(index);
                    cu.htmlCodesToChar(result, result);
                    qDebug() << "|" << result;
                    state = PSTATE_READ_LINK_START_TAG;
                }
                break;

            case PSTATE_READ_LINK_START_TAG:
                startTag = startDataTagArr[1];
                state = PSTATE_GET_LINK;
                // break;

            case PSTATE_GET_LINK:
                str = inStream.readLine();
                //qDebug() << str;
                index = str.indexOf(startTag);
                if(index > -1)
                {
                    // qDebug() << str;

                    index = str.indexOf("href=");
                    //qDebug() << index;
                    result = str.right(str.length() - index - 6);
                    // qDebug() << "result" << result;

                    index = result.indexOf("\" target=\"_blank\">");
                    result = result.left(index);
                    // qDebug() << "result1" << result;
                    cu.htmlCodesToChar(result, result);
                    result = result.replace(QString::fromUtf8("stockprofile"), QString::fromUtf8("stockreport"));
                    qDebug() << "|" << result << "|";
                    state = PSTATE_READ_STOCK_NAME_START_TAG;
                }
                break;
            }
        }

qDebug() << "============Stop==================";

        qDebug() << "close file\n";
        file.close();
        return true;
}





/****************************************************************
 *
 * Function:    getParserState()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
bool ParseOmxAllFinanceLinkData::getParserState(int indexHeader,
                                                int indexYear,
                                                int indexDataName,
                                                int indexData,
                                                ParseOmxAllFinanceLinkData::StateParseAnualDataET &cmd)
{
    bool res = false;
    cmd = ASTATE_PARSE_UNKNOWN;

    if(indexHeader  < 0)
    {
       indexHeader = 0x7FFFFFFF;
    }

    if(indexYear  < 0)
    {
       indexYear = 0x7FFFFFFF;
    }

    if(indexDataName  < 0)
    {
       indexDataName = 0x7FFFFFFF;
    }

    if(indexData  < 0)
    {
       indexData = 0x7FFFFFFF;
    }



    if(indexHeader < indexYear)
    {
        if(indexDataName < indexData)
        {
            if(indexHeader < indexDataName)
            {
               if((indexHeader > -1) && (indexHeader != 0x7FFFFFFF))
               {
                res = true;
                cmd = ASTATE_PARSE_HEADER;
               }
            }
            // indexDataName < indexHeader
            else
            {
                if((indexDataName > -1)  && (indexDataName != 0x7FFFFFFF))
                {
                    res = true;
                    cmd = ASTATE_PARSE_DATA_NAME;
                }
            }
        }
        // indexData < indexDataName
        else
        {
            if(indexHeader < indexData)
            {
                if((indexHeader > -1) && (indexHeader != 0x7FFFFFFF))
                {
                    res = true;
                    cmd = ASTATE_PARSE_HEADER;
                }
            }
            // indexData < indexHeader
            else
            {
                if((indexData > -1)  && (indexData !=  0x7FFFFFFF))
                {
                    res = true;
                    cmd = ASTATE_PARSE_DATA;
                }
            }
        }

    }
    // indexYear < indexHeader
    else
    {
        if(indexDataName < indexData)
        {
            if(indexYear < indexDataName)
            {
                if((indexYear > -1)  && (indexYear != 0x7FFFFFFF))
                {
                    res = true;
                    cmd = ASTATE_PARSE_YEAR;
                }
            }
            // indexDataName > indexYear
            else
            {
                if((indexDataName > -1)  && (indexDataName != 0x7FFFFFFF))
                {
                    res = true;
                    cmd = ASTATE_PARSE_DATA_NAME;
                }
            }
        }
        // indexData < indexDataName
        else
        {
            if(indexYear < indexData)
            {
                if((indexYear > -1)  && (indexYear != 0x7FFFFFFF))
                {
                    res = true;
                    cmd = ASTATE_PARSE_YEAR;
                }
            }
            // indexData < indexHeader
            else
            {
                if((indexData > -1)  && (indexData != 0x7FFFFFFF))
                {
                    res = true;
                    cmd = ASTATE_PARSE_DATA;
                }
            }
        }
    }

    return res;
}



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
bool ParseOmxAllFinanceLinkData::
parseAnualData(QString filename, struct OmxBridgeDataST *omxBridgeDataArr, int currArrIndex)
{
        StateParseAnualDataET cmd;
        unsigned char subCmd1 = -1;
        unsigned char subCmd2 = -1;
        QFile file(filename);
        QString errStr = QString("Fail to open file: %1").arg(filename);

        static QString sheetType;
        QString str;
        QString tmpStr;
        int index;
        int indexHeader;
        int indexYear;
        int indexDataName;
        int indexData;
        bool res;


        m_yearArrIndex = 0;
        m_dataArrIndex = 0;


      //  ParseStateET state = PSTATE_READ_STOCK_NAME_START_TAG;

        if(!file.open(QFile::ReadOnly | QFile::Text))
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Fail to open file"), errStr);
            return false;
        }


        qDebug() << "============Start==================";
        QTextStream inStream(&file);
        inStream.setCodec("UTF-8");




        str = inStream.readLine();
        while(!inStream.atEnd())
        {
            cmd = ASTATE_PARSE_UNKNOWN;
            indexHeader = str.indexOf("<caption class=\"sectionHeader\">");
            indexYear = str.indexOf("<th scope=\"col\" class=\"number\" id=\"MsStockReportFisY");

            // Data name
            // Income Statement
            subCmd1 = 0;
            indexDataName = str.indexOf("<th headers=\"MsStockReportFisDp\" scope=\"row\">");

            if(indexDataName < 0)
            {
                // Balance Sheet
                subCmd1 = 1;
                indexDataName = str.indexOf("<th headers=\"MsStockReportFbsDp\" scope=\"row\">");

                if(indexDataName < 0)
                {
                    // Cash Flow
                    subCmd1 = 2;
                    indexDataName = str.indexOf("<th headers=\"MsStockReportFcfDp\" scope=\"row\">");
                }
            }


            // Data value

            // Income Statement
            subCmd2 = 0;
            indexData = str.indexOf("<td class=\"number\" headers=\"MsStockReportFisY");
            if(indexData < 0)
            {
                // Balance Sheet
                subCmd2 = 1;
                indexData = str.indexOf("<td class=\"number\" headers=\"MsStockReportFbsY");

                if(indexData < 0)
                {
                    // Cash Flow
                    subCmd2 = 2;
                    indexData = str.indexOf("<td class=\"number\" headers=\"MsStockReportFcfY");
                }
            }


            if(indexHeader > -1)
            {
                cmd = ASTATE_PARSE_UNKNOWN;
            }

            if(indexYear > -1)
            {
                cmd = ASTATE_PARSE_UNKNOWN;
            }

            if(indexDataName > -1)
            {
                cmd = ASTATE_PARSE_UNKNOWN;
            }

            if(indexData > -1)
            {
                cmd = ASTATE_PARSE_UNKNOWN;
            }


            res = getParserState(indexHeader,
                                 indexYear,
                                 indexDataName,
                                 indexData,
                                 cmd);


            if(res == false)
            {
                str = inStream.readLine();
                continue;
            }


            switch(cmd)
            {
            case ASTATE_PARSE_HEADER:
                index = str.indexOf("<caption class=\"sectionHeader\">");
                if(index > -1)
                {
                    tmpStr = "<caption class=\"sectionHeader\">";
                    str = str.right(str.length() - index - tmpStr.length());
                    //qDebug() << "--Header\n" << str;
                    index = str.indexOf("<");

                    if(index > -1)
                    {

                        tmpStr = str.left(index);
                        qDebug() << "--Header" << tmpStr;
                        sheetType = tmpStr;
                    }
                }
                break;

            case ASTATE_PARSE_YEAR:
                index = str.indexOf("<th scope=\"col\" class=\"number\" id=\"MsStockReportFisY");
                tmpStr = "<th scope=\"col\" class=\"number\" id=\"MsStockReportFisY";
                str = str.right(str.length() - index- tmpStr.length());
                index = str.indexOf(">");
                str = str.right(str.length() - index - 1);
                // qDebug() << "--Year\n" << str;
                index = str.indexOf("<");

                if(index > -1)
                {
                    tmpStr = str.left(index);
                    qDebug() << "--Year" << tmpStr;

                    m_yearArr[m_yearArrIndex] = tmpStr;
                    qDebug() << m_yearArr[m_yearArrIndex];
                    m_yearArrIndex++;

                    if(m_yearArrIndex >= PARSE_ANUAL_ARR_SIZE)
                    {
                        QMessageBox::critical(NULL, QString::fromUtf8("Error"), QString::fromUtf8("yearArrIndex >= PARSE_ANUAL_ARR_SIZE"));
                        exit(0);
                    }

                }
                break;

            case ASTATE_PARSE_DATA_NAME:
                switch(subCmd1)
                {
                case 0:
                    index = str.indexOf("<th headers=\"MsStockReportFisDp\" scope=\"row\">");
                    tmpStr = "<th headers=\"MsStockReportFisDp\" scope=\"row\">";
                    break;
                case 1:
                    index = str.indexOf("<th headers=\"MsStockReportFbsDp\" scope=\"row\">");
                    tmpStr = "<th headers=\"MsStockReportFbsDp\" scope=\"row\">";
                    break;
                case 2:
                    index = str.indexOf("<th headers=\"MsStockReportFcfDp\" scope=\"row\">");
                    tmpStr = "<th headers=\"MsStockReportFcfDp\" scope=\"row\">";
                    break;
                }

                if(index > -1)
                {
                    str = str.right(str.length() - index - tmpStr.length());
                    index = str.indexOf("<");

                    if(index > -1)
                    {
                        if((m_dataName.length() > 0) && (m_yearArrIndex > 0) && (m_dataArrIndex > 0))
                        {
                            addDataToDb(omxBridgeDataArr, currArrIndex, sheetType);
                        }

                        // Reinit
                        m_dataArrIndex = 0;

                        tmpStr = str.left(index);
                        qDebug() << "--DataName" << tmpStr;
                        m_dataName = tmpStr;
                    }
                }
                break;

            case ASTATE_PARSE_DATA:

                switch(subCmd2)
                {
                case 0:
                    index = str.indexOf("<td class=\"number\" headers=\"MsStockReportFisY");
                    tmpStr = "<td class=\"number\" headers=\"MsStockReportFisY";
                    break;
                case 1:
                    index = str.indexOf("<td class=\"number\" headers=\"MsStockReportFbsY");
                    tmpStr = "<td class=\"number\" headers=\"MsStockReportFbsY";
                    break;
                case 2:
                    index = str.indexOf("<td class=\"number\" headers=\"MsStockReportFcfY");
                    tmpStr = "<td class=\"number\" headers=\"MsStockReportFcfY";
                    break;
                }


                str = str.right(str.length() - index- tmpStr.length());
                index = str.indexOf(">");
                str = str.right(str.length() - index - 1);
                // qDebug() << "--Year\n" << str;
                index = str.indexOf("<");

                if(index > -1)
                {
                    tmpStr = str.left(index);
                    tmpStr = tmpStr.replace(",","");
                    tmpStr = tmpStr.replace(" ","");
                    qDebug() << "--Data" << tmpStr;

                    m_dataArr[m_dataArrIndex] = tmpStr;
                    qDebug() << m_dataArr[m_dataArrIndex];
                    m_dataArrIndex++;

                    if(m_dataArrIndex >= PARSE_ANUAL_ARR_SIZE)
                    {
                        QMessageBox::critical(NULL, QString::fromUtf8("Error"), QString::fromUtf8("2. dataArrIndex >= PARSE_ANUAL_ARR_SIZE"));
                        exit(0);
                    }
                }
                break;

            case ASTATE_PARSE_UNKNOWN:
                QMessageBox::critical(NULL, QString::fromUtf8("Error"), QString::fromUtf8("invalid parse cmd"));
                break;
            };
        }

qDebug() << "============Stop==================";

        qDebug() << "close file\n";
        file.close();
        return true;
}








/****************************************************************
 *
 * Function:    addDataToDb()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
void ParseOmxAllFinanceLinkData::
addDataToDb(struct OmxBridgeDataST *omxBridgeDataArr, int currArrIndex, QString sheetType)
{
    int i = 0;
    QString tmp;
    qDebug() << "addDataToDb()";
    qDebug() << m_dataName;

    qDebug() << "yearArr";
    for( i = 0; i < m_yearArrIndex; i++)
    {
        qDebug()  << "yearArr" << m_yearArr[i];
    }

    qDebug() << "dataArr";
    for( i = 0; i < m_dataArrIndex; i++)
    {
        qDebug() << "dataArr" << m_dataArr[i];
    }

    if(m_dataName.compare("Basic") == 0)
    {
        insertEarningsPerShareIntoDb(omxBridgeDataArr, currArrIndex);

        // Clear EquityData used to calc solidity
        m_tmpEquityDatatmpEquityDatIsClear = false;
        for(i = 0; i < PARSE_ANUAL_ARR_SIZE; i++)
        {
            m_tmpEquityDataArr[i].clear();
        }
    }
    else if(m_dataName.compare("Total current assets") == 0)
    {
        insertTotalCurrentAssetIntoDb(omxBridgeDataArr, currArrIndex);
    }
    else if(m_dataName.compare("Total current liabilities") == 0)
    {
        insertTotalCurrentLiabilitiesIntoDb(omxBridgeDataArr, currArrIndex);
    }
    else if(m_dataName.compare("Total Liabilities") == 0)
    {
        insertTotalLiabilitiesIntoDb(omxBridgeDataArr, currArrIndex);
    }
    else if(m_dataName.compare("Total stockholders' equity") == 0)
    {
        insertEquityIntoDb(omxBridgeDataArr, currArrIndex);

        // Copy data to temp array so we can calc solidity later
        if(m_tmpEquityDatatmpEquityDatIsClear == false)
        {
            m_tmpEquityDatatmpEquityDatIsClear = true;
            m_tmpEquityDataArrIndex = m_yearArrIndex;

            for(i = 0; i < m_yearArrIndex; i++)
            {
                m_tmpEquityDataArr[i] = m_dataArr[i];
            }
        }
    }
    else if(m_dataName.compare("Total liabilities and stockholders' equity") == 0)
    {
        bool valid1;
        bool valid2;
        double realEquity;
        double realTotalCapital;
        double realSolidity;

        // Calc Solidity and convert it to QString array data
        for(i = 0; i < m_yearArrIndex; i++)
        {
            realEquity = m_tmpEquityDataArr[i].toDouble(&valid1);
            realTotalCapital = m_dataArr[i].toDouble(&valid2);

            if((valid1 == true) && (valid2 == true))
            {
                realSolidity = realEquity / realTotalCapital;
                m_dataArr[i].sprintf("%.2f", realSolidity);
            }
            else
            {
                m_dataArr[i] = "-";
            }
        }

        // Insert Solidity into database
        if(m_yearArrIndex > 0)
        {
            insertSolidityIntoDb(omxBridgeDataArr, currArrIndex);
        }

        // Clear temp array
        for( i= 0; i < PARSE_ANUAL_ARR_SIZE; i++)
        {
            m_tmpEquityDataArr[i].clear();
        }
    }
    else if(m_dataName.compare("Investment in property, plant and equipment") == 0)
    {
        insertCapexIntoDb(omxBridgeDataArr, currArrIndex);
    }
    else if(m_dataName.compare("Net cash provided by operating activities") == 0)
    {
        insertOperatingCashFlowIntoDb(omxBridgeDataArr, currArrIndex);
    }
    else if(m_dataName.compare("Dividends") == 0)
    {
        insertTotDividendsIntoDb(omxBridgeDataArr, currArrIndex);
    }
    else if(m_dataName.compare("Revenue") == 0)
    {
        insertRevenueIntoDb(omxBridgeDataArr, currArrIndex);
    }
    else if((m_dataName.compare("Net Income") == 0) && (sheetType.compare("Income Statement") == 0))
    {
        insertNetIncomeIntoDb(omxBridgeDataArr, currArrIndex);
    }


}




//======================
/******************************************************************
 *
 * Function:    on_pushButtonSaveNetIncome_clicked()
 *
 * Description: TotEarning or NetIncome
 *
 *
 *
 *****************************************************************/
void ParseOmxAllFinanceLinkData::insertNetIncomeIntoDb(struct OmxBridgeDataST *omxBridgeDataArr, int currArrIndex)
{

    CDbHndl db;
    bool res;
    int mainAnalysisId;
    QString str;

    bool isValid;
    int dateId;
    int inputDataId;
    int dataId;
    bool dataIdIsValid = false;

  //  int nofData;
    QString date;
    QString data;
    QString stockName;
    QString stockSymbol;

    stockName = omxBridgeDataArr[currArrIndex].yahooStockName;
    stockSymbol = omxBridgeDataArr[currArrIndex].stockSymbol;

    if(m_yearArrIndex != m_dataArrIndex)
    {
        return;
    }


    if((stockName.length() < 1) || stockSymbol.length() < 1)
    {
        QMessageBox::critical(NULL, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Välj aktie"));
        return;
    }



    // Check if this stocksymbol and stockname is already added, if not add it
    res = db.mainAnalysisDataExists(stockName, stockSymbol, mainAnalysisId);

    if(false == res)
    {
        res = db.insertMainAnalysisData(stockName, stockSymbol, mainAnalysisId);
    }


    for(int row = 0; row < m_yearArrIndex; row++)
    {
        date = m_yearArr[row];
        data = m_dataArr[row];

        date.toInt(&isValid);
        if (false == isValid)
        {
            QMessageBox::information(NULL, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Year is not a number"));
            continue;
        }

        CUtil cu;
        cu.number2double(data, data);
        data.toDouble(&isValid);
        if (false == isValid)
        {
            continue;
        }


        res = db.subAnalysisTotEarningsDateExists(date, mainAnalysisId, dateId);

        // Exist anaysis date?
        if(false == res)
        {
            res = db.insertSubAnalysisTotEarningsDate(date, mainAnalysisId, dateId);
        }


        if(true == res)
        {
           dataIdIsValid = false;

           if( true == db.getSubAnalysisTotEarningsDataId(mainAnalysisId, dateId, inputDataId))
           {
               dataIdIsValid = true;
           }

           res = db.insertSubAnalysisTotEarningsData(dateId, mainAnalysisId, inputDataId, dataIdIsValid,
                                                       data, dataId);
        }
    }
}




/******************************************************************
 *
 * Function:    insertRevenueIntoDb()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
void ParseOmxAllFinanceLinkData::insertRevenueIntoDb(struct OmxBridgeDataST *omxBridgeDataArr, int currArrIndex)
{

    CDbHndl db;
    bool res;
    int mainAnalysisId;

    bool isValid;
    int dateId;
    int inputDataId;
    int dataId;
    bool dataIdIsValid = false;

    QString date;
    QString data;
    QString stockName;
    QString stockSymbol;

    stockName = omxBridgeDataArr[currArrIndex].yahooStockName;
    stockSymbol = omxBridgeDataArr[currArrIndex].stockSymbol;

    if(m_yearArrIndex != m_dataArrIndex)
    {
        return;
    }


    if((stockName.length() < 1) || stockSymbol.length() < 1)
    {
        return;
    }



    // Check if this stocksymbol and stockname is already added, if not add it
    res = db.mainAnalysisDataExists(stockName, stockSymbol, mainAnalysisId);

    if(false == res)
    {
        res = db.insertMainAnalysisData(stockName, stockSymbol, mainAnalysisId);
    }


    for(int row = 0; row < m_yearArrIndex; row++)
    {
        date = m_yearArr[row];
        data = m_dataArr[row];


        date.toInt(&isValid);
        if (false == isValid)
        {
            QMessageBox::information(NULL, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Year is not a number"));
            continue;
        }

        CUtil cu;
        cu.number2double(data, data);
        data.toDouble(&isValid);
        if (false == isValid)
        {
            continue;
        }


        res = db.subAnalysisRevenueDateExists(date, mainAnalysisId, dateId);

        // Exist anaysis date?
        if(false == res)
        {
            res = db.insertSubAnalysisRevenueDate(date, mainAnalysisId, dateId);
        }


        if(true == res)
        {
           dataIdIsValid = false;

           if( true == db.getSubAnalysisRevenueDataId(mainAnalysisId, dateId, inputDataId))
           {
               dataIdIsValid = true;
           }

           res = db.insertSubAnalysisRevenueData(dateId, mainAnalysisId, inputDataId, dataIdIsValid,
                                                       data, dataId);
        }
    }
}



/******************************************************************
 *
 * Function:    insertTotDividendsIntoDb()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
void ParseOmxAllFinanceLinkData::insertTotDividendsIntoDb(struct OmxBridgeDataST *omxBridgeDataArr, int currArrIndex)
{

    CDbHndl db;
    bool res;
    int mainAnalysisId;

    bool isValid;
    int dateId;
    int inputDataId;
    int dataId;
    bool dataIdIsValid = false;

    QString date;
    QString data;
    QString stockName;
    QString stockSymbol;

    stockName = omxBridgeDataArr[currArrIndex].yahooStockName;
    stockSymbol = omxBridgeDataArr[currArrIndex].stockSymbol;

    if(m_yearArrIndex != m_dataArrIndex)
    {
        return;
    }



    if((stockName.length() < 1) || stockSymbol.length() < 1)
    {
        return;
    }



    // Check if this stocksymbol and stockname is already added, if not add it
    res = db.mainAnalysisDataExists(stockName, stockSymbol, mainAnalysisId);

    if(false == res)
    {
        res = db.insertMainAnalysisData(stockName, stockSymbol, mainAnalysisId);
    }


    for(int row = 0; row < m_yearArrIndex; row++)
    {
        date = m_yearArr[row];
        data = m_dataArr[row];

        date.toInt(&isValid);
        if (false == isValid)
        {
            QMessageBox::information(NULL, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Year is not a number"));
            continue;
        }

        CUtil cu;
        cu.number2double(data, data);
        data.toDouble(&isValid);
        if (false == isValid)
        {
            continue;
        }


        res = db.subAnalysisTotDividendsDateExists(date, mainAnalysisId, dateId);

        // Exist anaysis date?
        if(false == res)
        {
            res = db.insertSubAnalysisTotDividendsDate(date, mainAnalysisId, dateId);
        }


        if(true == res)
        {
           dataIdIsValid = false;

           if( true == db.getSubAnalysisTotDividendsDataId(mainAnalysisId, dateId, inputDataId))
           {
               dataIdIsValid = true;
           }

           res = db.insertSubAnalysisTotDividendsData(dateId, mainAnalysisId, inputDataId, dataIdIsValid,
                                                       data, dataId);
        }
    }

}


/******************************************************************
 *
 * Function:    insertOperatingCashFlowIntoDb()
 *
 * Description: This function saves Operating Cash Flow data to db
 *
 *
 *
 *****************************************************************/
void ParseOmxAllFinanceLinkData::insertOperatingCashFlowIntoDb(struct OmxBridgeDataST *omxBridgeDataArr, int currArrIndex)
{

    CDbHndl db;
    bool res;
    int mainAnalysisId;

    bool isValid;
    int dateId;
    int inputDataId;
    int dataId;
    bool dataIdIsValid = false;

    QString date;
    QString data;
    QString stockName;
    QString stockSymbol;

    stockName = omxBridgeDataArr[currArrIndex].yahooStockName;
    stockSymbol = omxBridgeDataArr[currArrIndex].stockSymbol;

    if(m_yearArrIndex != m_dataArrIndex)
    {
        return;
    }



    if((stockName.length() < 1) || stockSymbol.length() < 1)
    {
        return;
    }



    // Check if this stocksymbol and stockname is already added, if not add it
    res = db.mainAnalysisDataExists(stockName, stockSymbol, mainAnalysisId);

    if(false == res)
    {
        res = db.insertMainAnalysisData(stockName, stockSymbol, mainAnalysisId);
    }


    for(int row = 0; row < m_yearArrIndex; row++)
    {
        date = m_yearArr[row];
        data = m_dataArr[row];

        date.toInt(&isValid);
        if (false == isValid)
        {
            QMessageBox::information(NULL, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Year is not a number"));
            continue;
        }

        CUtil cu;
        cu.number2double(data, data);
        data.toDouble(&isValid);
        if (false == isValid)
        {
            continue;
        }


        res = db.subAnalysisOperatingCashFlowDateExists(date, mainAnalysisId, dateId);

        // Exist anaysis date?
        if(false == res)
        {
            res = db.insertSubAnalysisOperatingCashFlowDate(date, mainAnalysisId, dateId);
        }


        if(true == res)
        {
           dataIdIsValid = false;

           if( true == db.getSubAnalysisOperatingCashFlowDataId(mainAnalysisId, dateId, inputDataId))
           {
               dataIdIsValid = true;
           }

           res = db.insertSubAnalysisOperatingCashFlowData(dateId, mainAnalysisId, inputDataId, dataIdIsValid,
                                                       data, dataId);
        }
    }
}


/******************************************************************
 *
 * Function:    insertCapexIntoDb()
 *
 * Description: This function saves CashFlowCapex data to db
 *
 *
 *
 *****************************************************************/
void ParseOmxAllFinanceLinkData::insertCapexIntoDb(struct OmxBridgeDataST *omxBridgeDataArr, int currArrIndex)
{
    CDbHndl db;
    bool res;
    int mainAnalysisId;

    bool isValid;
    int dateId;
    int inputDataId;
    int dataId;
    bool dataIdIsValid = false;

    QString date;
    QString data;
    QString stockName;
    QString stockSymbol;

    stockName = omxBridgeDataArr[currArrIndex].yahooStockName;
    stockSymbol = omxBridgeDataArr[currArrIndex].stockSymbol;

    if(m_yearArrIndex != m_dataArrIndex)
    {
        return;
    }


    if((stockName.length() < 1) || stockSymbol.length() < 1)
    {
        return;
    }


    // Check if this stocksymbol and stockname is already added, if not add it
    res = db.mainAnalysisDataExists(stockName, stockSymbol, mainAnalysisId);

    if(false == res)
    {
        res = db.insertMainAnalysisData(stockName, stockSymbol, mainAnalysisId);
    }


    for(int row = 0; row < m_yearArrIndex; row++)
    {
        date = m_yearArr[row];
        data = m_dataArr[row];

        date.toInt(&isValid);
        if (false == isValid)
        {
            QMessageBox::information(NULL, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Year is not a number"));
            continue;
        }

        CUtil cu;
        cu.number2double(data, data);
        data.toDouble(&isValid);
        if (false == isValid)
        {
            continue;
        }


        res = db.subAnalysisCashFlowCapexDateExists(date, mainAnalysisId, dateId);

        // Exist anaysis date?
        if(false == res)
        {
            res = db.insertSubAnalysisCashFlowCapexDate(date, mainAnalysisId, dateId);
        }


        if(true == res)
        {
           dataIdIsValid = false;

           if( true == db.getSubAnalysisCashFlowCapexDataId(mainAnalysisId, dateId, inputDataId))
           {
               dataIdIsValid = true;
           }

           res = db.insertSubAnalysisCashFlowCapexData(dateId, mainAnalysisId, inputDataId, dataIdIsValid,
                                                       data, dataId);
        }
    }
}


/******************************************************************
 *
 * Function:    insertSolidityIntoDb()
 *
 * Description: This function saves Solidity data to db
 *
 *
 *
 *****************************************************************/
void ParseOmxAllFinanceLinkData::insertSolidityIntoDb(struct OmxBridgeDataST *omxBridgeDataArr, int currArrIndex)
{
    CDbHndl db;
    bool res;
    int mainAnalysisId;

    bool isValid;
    int dateId;
    int inputDataId;
    int dataId;
    bool dataIdIsValid = false;

    QString date;
    QString data;
    QString stockName;
    QString stockSymbol;

    stockName = omxBridgeDataArr[currArrIndex].yahooStockName;
    stockSymbol = omxBridgeDataArr[currArrIndex].stockSymbol;

    if(m_yearArrIndex != m_dataArrIndex)
    {
        return;
    }


    if((stockName.length() < 1) || stockSymbol.length() < 1)
    {
        return;
    }



    // Check if this stocksymbol and stockname is already added, if not add it
    res = db.mainAnalysisDataExists(stockName,
                                stockSymbol,
                                mainAnalysisId);
    if(false == res)
    {
        res = db.insertMainAnalysisData(stockName,
                                     stockSymbol,
                                     mainAnalysisId);
    }



    for(int row = 0; row < m_yearArrIndex; row++)
    {
        date = m_yearArr[row];
        data = m_dataArr[row];


        date.toInt(&isValid);
        if (false == isValid)
        {
            QMessageBox::information(NULL, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Year is not a number"));
            continue;
        }

        CUtil cu;
        cu.number2double(data, data);
        data.toDouble(&isValid);
        if (false == isValid)
        {
           continue;
        }


        res = db.subAnalysisSolidityDateExists(date,
                                               mainAnalysisId,
                                               dateId);

        // Exist anaysis date?
        if(false == res)
        {
            res = db.insertSubAnalysisSolidityDate(date,
                                                   mainAnalysisId,
                                                   dateId);
        }


        if(true == res)
        {
           dataIdIsValid = false;

           if( true == db.getSubAnalysisSolidityDataId(mainAnalysisId, dateId, inputDataId))
           {
               dataIdIsValid = true;
           }

            res = db.insertSubAnalysisSolidityData(dateId,
                                               mainAnalysisId,
                                               inputDataId,
                                               dataIdIsValid,
                                               data,
                                               dataId);
        }
    }

}




/******************************************************************
 *
 * Function:    insertEquityIntoDb()
 *
 * Description: This function saves Equity data to db
 *
 *
 *
 *****************************************************************/
void ParseOmxAllFinanceLinkData::insertEquityIntoDb(struct OmxBridgeDataST *omxBridgeDataArr, int currArrIndex)
{
    CDbHndl db;
    bool res;
    int mainAnalysisId;

    bool isValid;
    int dateId;
    int inputDataId;
    int dataId;
    bool dataIdIsValid = false;

    QString date;
    QString data;
    QString stockName;
    QString stockSymbol;

    stockName = omxBridgeDataArr[currArrIndex].yahooStockName;
    stockSymbol = omxBridgeDataArr[currArrIndex].stockSymbol;

    if(m_yearArrIndex != m_dataArrIndex)
    {
        return;
    }


    if((stockName.length() < 1) || stockSymbol.length() < 1)
    {
        return;
    }



    // Check if this stocksymbol and stockname is already added, if not add it
    res = db.mainAnalysisDataExists(stockName, stockSymbol, mainAnalysisId);

    if(false == res)
    {
        res = db.insertMainAnalysisData(stockName, stockSymbol, mainAnalysisId);
    }




    for(int row = 0; row < m_yearArrIndex; row++)
    {
        date = m_yearArr[row];
        data = m_dataArr[row];


        date.toInt(&isValid);
        if (false == isValid)
        {
            QMessageBox::information(NULL, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Year is not a number"));
            continue;
        }

        CUtil cu;
        cu.number2double(data, data);
        data.toDouble(&isValid);
        if (false == isValid)
        {
            continue;
        }


        res = db.subAnalysisEquityDateExists(date, mainAnalysisId, dateId);

        // Exist anaysis date?
        if(false == res)
        {
            res = db.insertSubAnalysisEquityDate(date, mainAnalysisId, dateId);
        }


        if(true == res)
        {
           dataIdIsValid = false;

           if( true == db.getSubAnalysisEquityDataId(mainAnalysisId, dateId, inputDataId))
           {
               dataIdIsValid = true;
           }

           res = db.insertSubAnalysisEquityData(dateId, mainAnalysisId, inputDataId, dataIdIsValid,
                                                       data, dataId);
        }
    }
}



/******************************************************************
 *
 * Function:    insertTotalLiabilitiesIntoDb()
 *
 * Description: This function saves Total Liabilities data to db
 *
 *
 *
 *****************************************************************/
void ParseOmxAllFinanceLinkData::insertTotalLiabilitiesIntoDb(struct OmxBridgeDataST *omxBridgeDataArr, int currArrIndex)
{
    CDbHndl db;
    bool res;
    int mainAnalysisId;

    bool isValid;
    int dateId;
    int inputDataId;
    int dataId;
    bool dataIdIsValid = false;

    QString date;
    QString data;
    QString stockName;
    QString stockSymbol;

    stockName = omxBridgeDataArr[currArrIndex].yahooStockName;
    stockSymbol = omxBridgeDataArr[currArrIndex].stockSymbol;

    if(m_yearArrIndex != m_dataArrIndex)
    {
        return;
    }


    if((stockName.length() < 1) || stockSymbol.length() < 1)
    {
        return;
    }



    // Check if this stocksymbol and stockname is already added, if not add it
    res = db.mainAnalysisDataExists(stockName,
                                stockSymbol,
                                mainAnalysisId);
    if(false == res)
    {
        res = db.insertMainAnalysisData(stockName,
                                     stockSymbol,
                                     mainAnalysisId);
    }






    for(int row = 0; row < m_yearArrIndex; row++)
    {
        date = m_yearArr[row];
        data = m_dataArr[row];

        date.toInt(&isValid);
        if (false == isValid)
        {
           QMessageBox::information(NULL, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Year is not a number"));
           continue;
        }

        CUtil cu;
        cu.number2double(data, data);
        data.toDouble(&isValid);
        if (false == isValid)
        {
           continue;
        }


        res = db.subAnalysisTotalLiabilitiesDateExists(date,
                                                       mainAnalysisId,
                                                       dateId);

        // Exist anaysis date?
        if(false == res)
        {
            res = db.insertSubAnalysisTotalLiabilitiesDate(date,
                                                           mainAnalysisId,
                                                           dateId);
        }


        if(true == res)
        {
           dataIdIsValid = false;

           if( true == db.getSubAnalysisTotalLiabilitiesDataId(mainAnalysisId, dateId, inputDataId))
           {
               dataIdIsValid = true;
           }

            res = db.insertSubAnalysisTotalLiabilities(dateId,
                                                       mainAnalysisId,
                                                       inputDataId,
                                                       dataIdIsValid,
                                                       data,
                                                       dataId);
        }
    }

}




/******************************************************************
 *
 * Function:    insertDividendPerShareIntoDb()
 *
 * Description: This function saves dividend data to db
 *
 * Note_        (This function is not used) There is no parsed omx data for this.
 *
 *****************************************************************/
void ParseOmxAllFinanceLinkData::insertDividendPerShareIntoDb(struct OmxBridgeDataST *omxBridgeDataArr, int currArrIndex)
{
    CDbHndl db;
    bool res;
    int mainAnalysisId;

    bool isValid;
    int dividendDateId;
    int inputDividendDataId;
    int dividendDataId;
    bool dividendDataIdIsValid = false;

    QString dividendDate;
    QString dividendData;
    QString stockName;
    QString stockSymbol;


    stockName = omxBridgeDataArr[currArrIndex].yahooStockName;
    stockSymbol = omxBridgeDataArr[currArrIndex].stockSymbol;



    if((stockName.length() < 1) || stockSymbol.length() < 1)
    {
        return;
    }

    if(m_yearArrIndex != m_dataArrIndex)
    {
        return;
    }


    // Check if this stocksymbol and stockname is already added, if not add it
    res = db.mainAnalysisDataExists(stockName,
                                stockSymbol,
                                mainAnalysisId);
    if(false == res)
    {
        res = db.insertMainAnalysisData(stockName,
                                     stockSymbol,
                                     mainAnalysisId);
    }



    for(int row = 0; row < m_yearArrIndex; row++)
    {
           dividendDate = m_yearArr[row];
           dividendData = m_dataArr[row];

            dividendDate.toInt(&isValid);
            if (false == isValid)
            {
                QMessageBox::information(NULL, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Year is not a number"));
                continue;
            }

            CUtil cu;
            cu.number2double(dividendData, dividendData);
            dividendData.toDouble(&isValid);
            if (false == isValid)
            {
                //QMessageBox::information(NULL, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Dividend is not a number"));
                continue;
            }



        res = db.subAnalysisDividendDateExists(dividendDate,
                                                   mainAnalysisId,
                                                   dividendDateId);
        // Exist anaysis date?
        if(false == res)
        {
            res = db.insertSubAnalysisDividendDate(dividendDate,
                                                   mainAnalysisId,
                                                   dividendDateId);
        }


        if(true == res)
        {
           dividendDataIdIsValid = false;

           if( true == db.getSubAnalysisDividendId(mainAnalysisId, dividendDateId, inputDividendDataId))
           {
               dividendDataIdIsValid = true;
           }

            res = db.insertSubAnalysisDividend(dividendDateId,
                                               mainAnalysisId,
                                               inputDividendDataId,
                                               dividendDataIdIsValid,
                                               dividendData,
                                               dividendDataId);
        }
    }
}




/******************************************************************
 *
 * Function:    insertEarningsPerShareIntoDb()
 *
 * Description: This function saves earnings data to db
 *
 *
 *
 *****************************************************************/
void ParseOmxAllFinanceLinkData::insertEarningsPerShareIntoDb(struct OmxBridgeDataST *omxBridgeDataArr, int currArrIndex)
{
    CDbHndl db;
    bool res;
    int mainAnalysisId;

    bool isValid;
    int earningsDateId;
    int inputEarningsDataId;
    int earningsDataId;
    bool earningsDataIdIsValid = false;

    QString earningsDate;
    QString earningsData;

    QString stockName;
    QString stockSymbol;


    stockName = omxBridgeDataArr[currArrIndex].yahooStockName;
    stockSymbol = omxBridgeDataArr[currArrIndex].stockSymbol;

    if(m_yearArrIndex != m_dataArrIndex)
    {
        return;
    }



    if((stockName.length() < 1) || stockSymbol.length() < 1)
    {
        return;
    }

    // Check if this stocksymbol and stockname is already added, if not add it
    res = db.mainAnalysisDataExists(stockName,
                                stockSymbol,
                                mainAnalysisId);
    if(false == res)
    {
        res = db.insertMainAnalysisData(stockName,
                                     stockSymbol,
                                     mainAnalysisId);
    }




    for(int row = 0; row < m_yearArrIndex; row++)
    {
        earningsDate = m_yearArr[row];
        earningsData = m_dataArr[row];

        earningsDate.toInt(&isValid);
        if (false == isValid)
        {
            QMessageBox::information(NULL, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Year is not a number"));
            continue;
        }

        CUtil cu;
        cu.number2double(earningsData, earningsData);
        earningsData.toDouble(&isValid);
        if (false == isValid)
        {
            //QMessageBox::information(NULL, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Earnings is not a number"));
            continue;
        }


        res = db.subAnalysisEarningsPerShareDateExists(earningsDate,
                                               mainAnalysisId,
                                               earningsDateId);

        // Exist anaysis date?
        if(false == res)
        {
            res = db.insertSubAnalysisEarningsPerShareDate(earningsDate,
                                                   mainAnalysisId,
                                                   earningsDateId);
        }


        if(true == res)
        {
           earningsDataIdIsValid = false;

           if( true == db.getSubAnalysisEarningsPerShareId(mainAnalysisId, earningsDateId, inputEarningsDataId))
           {
               earningsDataIdIsValid = true;
           }

            res = db.insertSubAnalysisEarningsPerShare(earningsDateId,
                                                       mainAnalysisId,
                                                        inputEarningsDataId,
                                                        earningsDataIdIsValid,
                                               earningsData,
                                               earningsDataId);
        }
    }

}




/******************************************************************
 *
 * Function:    insertTotalCurrentAssetIntoDb()
 *
 * Description: This function saves earnings data to db
 *
 *
 *
 *****************************************************************/
void ParseOmxAllFinanceLinkData::insertTotalCurrentAssetIntoDb(struct OmxBridgeDataST *omxBridgeDataArr, int currArrIndex)
{
    CDbHndl db;
    bool res;
    int mainAnalysisId;

    bool isValid;
    int totalCurrentAssetsDateId;
    int inputTotalCurrentAssetsDataId;
    int totalCurrentAssetsDataId;
    bool totalCurrentAssetsDataIdIsValid = false;

    QString totalCurrentAssetsDate;
    QString totalCurrentAssetsData;
    QString stockName;
    QString stockSymbol;

    stockName = omxBridgeDataArr[currArrIndex].yahooStockName;
    stockSymbol = omxBridgeDataArr[currArrIndex].stockSymbol;

    if(m_yearArrIndex != m_dataArrIndex)
    {
        return;
    }


    if((stockName.length() < 1) || stockSymbol.length() < 1)
    {
        return;
    }



    // Check if this stocksymbol and stockname is already added, if not add it
    res = db.mainAnalysisDataExists(stockName,
                                stockSymbol,
                                mainAnalysisId);
    if(false == res)
    {
        res = db.insertMainAnalysisData(stockName,
                                     stockSymbol,
                                     mainAnalysisId);
    }



    for(int row = 0; row < m_yearArrIndex; row++)
    {
        totalCurrentAssetsDate = m_yearArr[row];
        totalCurrentAssetsData = m_dataArr[row];

        totalCurrentAssetsDate.toInt(&isValid);
        if (false == isValid)
        {
           QMessageBox::information(NULL, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Year is not a number"));
           continue;
        }

        CUtil cu;
        cu.number2double(totalCurrentAssetsData, totalCurrentAssetsData);
        totalCurrentAssetsData.toDouble(&isValid);
        if (false == isValid)
        {
           // QMessageBox::information(NULL, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Earnings is not a number"));
           continue;
        }


        res = db.subAnalysisTotalCurrentAssetsDateExists(totalCurrentAssetsDate,
                                                        mainAnalysisId,
                                                        totalCurrentAssetsDateId);

        // Exist anaysis date?
        if(false == res)
        {
            res = db.insertSubAnalysisTotalCurrentAssetsDate(totalCurrentAssetsDate,
                                                            mainAnalysisId,
                                                            totalCurrentAssetsDateId);
        }


        if(true == res)
        {
           totalCurrentAssetsDataIdIsValid = false;

           if( true == db.getSubAnalysisTotalCurrentAssetsDataId(mainAnalysisId, totalCurrentAssetsDateId, inputTotalCurrentAssetsDataId))
           {
               totalCurrentAssetsDataIdIsValid = true;
           }

            res = db.insertSubAnalysisTotalCurrentAssets(totalCurrentAssetsDateId,
                                                        mainAnalysisId,
                                                        inputTotalCurrentAssetsDataId,
                                                        totalCurrentAssetsDataIdIsValid,
                                                        totalCurrentAssetsData,
                                                        totalCurrentAssetsDataId);
        }
    }
}



/******************************************************************
 *
 * Function:    on_pushButtonSaveTotalCurrentLiabilities_clicked()
 *
 * Description: This function saves Total Current Liabilities data to db
 *
 *
 *
 *****************************************************************/
void ParseOmxAllFinanceLinkData::insertTotalCurrentLiabilitiesIntoDb(struct OmxBridgeDataST *omxBridgeDataArr, int currArrIndex)
{
    CDbHndl db;
    bool res;
    int mainAnalysisId;

    bool isValid;
    int dateId;
    int inputDataId;
    int dataId;
    bool dataIdIsValid = false;

    QString date;
    QString data;
    QString stockName;
    QString stockSymbol;

    stockName = omxBridgeDataArr[currArrIndex].yahooStockName;
    stockSymbol = omxBridgeDataArr[currArrIndex].stockSymbol;

    if(m_yearArrIndex != m_dataArrIndex)
    {
        return;
    }


    if((stockName.length() < 1) || stockSymbol.length() < 1)
    {
        return;
    }




    // Check if this stocksymbol and stockname is already added, if not add it
    res = db.mainAnalysisDataExists(stockName,
                                stockSymbol,
                                mainAnalysisId);
    if(false == res)
    {
        res = db.insertMainAnalysisData(stockName,
                                     stockSymbol,
                                     mainAnalysisId);
    }


    for(int row = 0; row < m_yearArrIndex; row++)
    {

        date = m_yearArr[row];
        data = m_dataArr[row];

        date.toInt(&isValid);
        if (false == isValid)
        {
           QMessageBox::information(NULL, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Year is not a number"));
           continue;
        }

        CUtil cu;
        cu.number2double(data, data);
        data.toDouble(&isValid);
        if (false == isValid)
        {
           //QMessageBox::information(NULL, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Earnings is not a number"));
           continue;
        }



        res = db.subAnalysisTotalCurrentLiabilitiesDateExists(date,
                                                        mainAnalysisId,
                                                        dateId);

        // Exist anaysis date?
        if(false == res)
        {
            res = db.insertSubAnalysisTotalCurrentLiabilitiesDate(date,
                                                            mainAnalysisId,
                                                            dateId);
        }


        if(true == res)
        {
           dataIdIsValid = false;

           if( true == db.getSubAnalysisTotalCurrentLiabilitiesDataId(mainAnalysisId, dateId, inputDataId))
           {
               dataIdIsValid = true;
           }

            res = db.insertSubAnalysisTotalCurrentLiabilities(dateId,
                                                              mainAnalysisId,
                                                              inputDataId,
                                                              dataIdIsValid,
                                                              data,
                                                              dataId);
        }
    }

}
