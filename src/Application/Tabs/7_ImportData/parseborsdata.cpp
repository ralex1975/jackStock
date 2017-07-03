#include "parseborsdata.h"
#include "QFile"
#include "QMessageBox"
#include "QTextStream"
#include "QDebug"
#include "dbHndl.h"
#include "util.h"

#define START_TAG_YEAR " <!-- ngRepeat: header in analysisCtrl.tableHeaders track by $index -->"




/****************************************************************
 *
 * Function:    ParseBorsData()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
ParseBorsData::ParseBorsData()
{

}


/****************************************************************
 *
 * Function:    helpParser()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
bool ParseBorsData::helpParser(QString &result,
                               QRegExp rx,
                               MainStateET mainState,
                               QString dbgStr,
                               StockParseDataST *parseDataArr,
                               int &nofParseArrData)
{
    int valueLen = 0;
    int index;
    int startIndex;
    bool res = false;
    QString value;
    int cnt = 0;

    if(mainState == MSTATE_YEAR)
    {
        nofParseArrData = 0;
    }


    do
    {
        res = false;
        index = rx.indexIn(result);

        if( index != -1 )
        {
            value = rx.cap(0);
            valueLen = value.length();

            switch(mainState)
            {
            case MSTATE_YEAR:
                qDebug() << dbgStr << value;
                parseDataArr[nofParseArrData++].year = value;
                res = true;
                break;

            case MSTATE_SOLIDITY:
                res = cleanupParsedProcentValue(value, dbgStr);
                if(res == true)
                {
                    parseDataArr[cnt++].solidity = value;
                }
                break;

            case MSTATE_EQUITY_PER_SHARE:
                res = cleanupParsedRealValue(value, dbgStr);
                if(res == true)
                {
                    parseDataArr[cnt++].equityPerShare = value;
                }
                break;

            case MSTATE_EARNINGS_PER_SHARE:
                res = cleanupParsedRealValue(value, dbgStr);
                if(res == true)
                {
                    parseDataArr[cnt++].earningPerShare = value;
                }
                break;

            case MSTATE_DIVIDEND:
                res = cleanupParsedRealValue(value, dbgStr);
                if(res == true)
                {
                    parseDataArr[cnt++].dividend = value;
                }
                break;
            }

            if(true == res)
            {
                startIndex = result.length() - index + 1;
                qDebug() << "startIndex" << startIndex;
                qDebug() << "result.length()" << result.length();
                qDebug() << "index" << index;
                qDebug() << "valueLen" << valueLen;

                if(startIndex > -1 && startIndex < result.length())
                {
                    result = result.right(startIndex - valueLen);
                }
            }
            else
            {
                return false;
            }
        }

    }while(index > -1);

    return true;
}



/****************************************************************
 *
 * Function:    cleanupParsedProcentData()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
bool ParseBorsData::cleanupParsedProcentValue(QString &value, QString str)
{
    bool res = false;
    double dbValue;

    if((value[0] == '>') && (value[value.length() -1] == '<'))
    {
        value = value.mid(1, value.length() - 3);
        value = value.replace(",",".");
        dbValue = value.toDouble();
        dbValue = dbValue / 100.0;
        value.sprintf("%.2f", dbValue);
        qDebug() << str << value;
        res = true;
    }

    return res;
}


/****************************************************************
 *
 * Function:    cleanupParsedRealValue()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
bool ParseBorsData::cleanupParsedRealValue(QString &value, QString str)
{
    bool res = false;

    if((value[0] == '>') && (value[value.length() -1] == '<'))
    {
        value = value.mid(1, value.length() - 2);
        value = value.replace(",",".");
        qDebug() << str << value;
        res = true;
    }

    return res;
}



/******************************************************************
 *
 * Function:    addDividendPerShare()
 *
 * Description: This function saves dividend data to db
 *
 *
 *
 *****************************************************************/
bool ParseBorsData::addDividendPerShare(StockParseDataST *parseDataArr,
                                        int nofParseArrData,
                                        QString stockSymbol,
                                        QString stockName)
{
    CDbHndl db;
    bool res;
    int mainAnalysisId;

    bool isValid;
    int dividendDateId;
    int inputDividendDataId;
    int dividendDataId;
    bool dividendDataIdIsValid = false;

    QString year;
    QString dividend;

    if((stockName.length() < 1) || stockSymbol.length() < 1)
    {
        QMessageBox::critical(NULL, QString::fromUtf8("Error"), QString::fromUtf8("Error: Stock data missing"));
        return false;
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


    for(int row = 0; row < nofParseArrData; row++)
    {
        year = parseDataArr[row].year;
        dividend = parseDataArr[row].dividend;

        year.toInt(&isValid);
        if (false == isValid)
        {
            QMessageBox::information(NULL, QString::fromUtf8("Error"), QString::fromUtf8("Year is not a number"));
            continue;
        }

        CUtil cu;
        cu.number2double(dividend, dividend);
        dividend.toDouble(&isValid);
        if (false == isValid)
        {
            QMessageBox::information(NULL, QString::fromUtf8("Error"), QString::fromUtf8("Dividend is not a number"));
            continue;
        }


        res = db.subAnalysisDividendDateExists(year,
                                               mainAnalysisId,
                                               dividendDateId);

        // Exist anaysis date?
        if(false == res)
        {
            res = db.insertSubAnalysisDividendDate(year,
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
                                               dividend,
                                               dividendDataId);
        }
    }

    return true;

}



/******************************************************************
 *
 * Function:    addEarningsPerShare()
 *
 * Description: This function saves earnings data to db
 *
 *
 *
 *****************************************************************/
bool ParseBorsData::addEarningsPerShare(StockParseDataST *parseDataArr,
                                        int nofParseArrData,
                                        QString stockSymbol,
                                        QString stockName)
{
    CDbHndl db;
    bool res;
    int mainAnalysisId;

    bool isValid;
    int earningsDateId;
    int inputEarningsDataId;
    int earningsDataId;
    bool earningsDataIdIsValid = false;

    QString year;
    QString earnings;

    if((stockName.length() < 1) || stockSymbol.length() < 1)
    {
        QMessageBox::critical(NULL, QString::fromUtf8("Error"), QString::fromUtf8("Stock data missing"));
        return false;
    }


    // Check if this stocksymbol and stockname is already added, if not add it
    res = db.mainAnalysisDataExists(stockName, stockSymbol, mainAnalysisId);
    if(false == res)
    {
        res = db.insertMainAnalysisData(stockName, stockSymbol, mainAnalysisId);
    }




    for(int row = 0; row < nofParseArrData; row++)
    {
        year = parseDataArr[row].year;
        earnings = parseDataArr[row].earningPerShare;


        year.toInt(&isValid);
        if (false == isValid)
        {
            QMessageBox::information(NULL, QString::fromUtf8("Error"), QString::fromUtf8("Year is not a number"));
            continue;
        }

        CUtil cu;
        cu.number2double(earnings, earnings);
        earnings.toDouble(&isValid);
        if (false == isValid)
        {
            QMessageBox::information(NULL, QString::fromUtf8("Error"), QString::fromUtf8("Earnings is not a number"));
            continue;
        }


        res = db.subAnalysisEarningsPerShareDateExists(year,
                                               mainAnalysisId,
                                               earningsDateId);

        // Exist anaysis date?
        if(false == res)
        {
            res = db.insertSubAnalysisEarningsPerShareDate(year,
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
                                               earnings,
                                               earningsDataId);
        }
    }

    return true;
}




/******************************************************************
 *
 * Function:    on_pushButtonSaveSolidity_clicked()
 *
 * Description: This function saves Solidity data to db
 *
 *
 *
 *****************************************************************/
bool ParseBorsData::addSolidityPerShare(StockParseDataST *parseDataArr,
                                                         int nofParseArrData,
                                                         QString stockSymbol,
                                                        QString stockName)
{
    CDbHndl db;
    bool res;
    int mainAnalysisId;

    bool isValid;
    int dateId;
    int inputDataId;
    int dataId;
    bool dataIdIsValid = false;

    QString year;
    QString solidity;

    if((stockName.length() < 1) || stockSymbol.length() < 1)
    {
        QMessageBox::critical(NULL, QString::fromUtf8("Error"), QString::fromUtf8("Stock data missing"));
        return false;
    }


    // Check if this stocksymbol and stockname is already added, if not add it
    res = db.mainAnalysisDataExists(stockName, stockSymbol, mainAnalysisId);

    if(false == res)
    {
        res = db.insertMainAnalysisData(stockName, stockSymbol, mainAnalysisId);
    }



    for(int row = 0; row < nofParseArrData; row++)
    {
        year = parseDataArr[row].year;
        solidity = parseDataArr[row].solidity;

        year.toInt(&isValid);
        if (false == isValid)
        {
           QMessageBox::information(NULL, QString::fromUtf8("Error"), QString::fromUtf8("Year is not a number"));
           continue;
        }

        CUtil cu;
        cu.number2double(solidity, solidity);
        solidity.toDouble(&isValid);
        if (false == isValid)
        {
           QMessageBox::information(NULL, QString::fromUtf8("Error"), QString::fromUtf8("Solidity is not a number"));
           continue;
        }


        res = db.subAnalysisSolidityDateExists(year,
                                               mainAnalysisId,
                                               dateId);

        // Exist anaysis date?
        if(false == res)
        {
            res = db.insertSubAnalysisSolidityDate(year,
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
                                                   solidity,
                                                   dataId);
        }
    }

    return true;

}



/******************************************************************
 *
 * Function:        addEquityPerShare()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool ParseBorsData::addEquityPerShare(StockParseDataST *parseDataArr,
                                      int nofParseArrData,
                                      QString stockSymbol,
                                      QString stockName)
{
    CDbHndl db;
    bool res;
    int mainAnalysisId;

    bool isValid;
    int dateId;
    int inputDataId;
    int dataId;
    bool dataIdIsValid = false;

    QString year;
    QString equity;


    if((stockName.length() < 1) || stockSymbol.length() < 1)
    {
        QMessageBox::critical(NULL, QString::fromUtf8("Error"), QString::fromUtf8("Stock data missing"));
        return false;
    }



    // Check if this stocksymbol and stockname is already added, if not add it
    res = db.mainAnalysisDataExists(stockName, stockSymbol, mainAnalysisId);

    if(false == res)
    {
        res = db.insertMainAnalysisData(stockName, stockSymbol, mainAnalysisId);
    }



    for(int row = 0; row < nofParseArrData; row++)
    {
        year = parseDataArr[row].year;
        equity = parseDataArr[row].equityPerShare;


            year.toInt(&isValid);
            if (false == isValid)
            {
                QMessageBox::information(NULL, QString::fromUtf8("Error"), QString::fromUtf8("Year is not a number"));
                continue;
            }

            CUtil cu;
            cu.number2double(equity, equity);
            equity.toDouble(&isValid);
            if (false == isValid)
            {
                QMessageBox::information(NULL, QString::fromUtf8("Error"), QString::fromUtf8("Equity is not a number"));
                continue;
            }


        res = db.subAnalysisEquityPerShareDateExists(year, mainAnalysisId, dateId);

        // Exist anaysis date?
        if(false == res)
        {
            res = db.insertSubAnalysisEquityPerShareDate(year, mainAnalysisId, dateId);
        }


        if(true == res)
        {
           dataIdIsValid = false;

           if( true == db.getSubAnalysisEquityPerShareDataId(mainAnalysisId, dateId, inputDataId))
           {
               dataIdIsValid = true;
           }

           res = db.insertSubAnalysisEquityPerShareData(dateId, mainAnalysisId, inputDataId, dataIdIsValid,
                                                       equity, dataId);
        }
    }

    return true;
}




/****************************************************************
 *
 * Function:    readStockSymbolsFile()
 *
 * Description: This file contains information of html pages
 *              and corresponding stock symbols.
 *
 *
 ****************************************************************/
bool ParseBorsData::readStockSymbolsFile(QString filename)
{

    QFile file(filename);
    QString errStr = QString("Fail to open file: %1").arg(filename);
    QString result;
    QStringList resultList;


    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::critical(NULL, QString::fromUtf8("Fail to open file"), errStr);
        return false;
    }

    QTextStream inStream(&file);
    inStream.setCodec("UTF-8");


    QRegExp rx("(\\|)"); //RegEx for '|'

    while(!inStream.atEnd())
    {
        result = inStream.readLine();
        resultList = result.split(rx);

        if(true != resultList.isEmpty() || resultList.size() == 3)
        {
            if((resultList.at(0).length() > 0)  &&
               (resultList.at(1).length() > 0)  &&
               (resultList.at(2).length() > 0))
            {
                StockDataST stockData;

                // Get stock symbol information
                stockData.stockSymbol = resultList.at(0);
                qDebug() << resultList.at(0);
                stockData.stockName = resultList.at(1);
                stockData.htmlFilename = "database/inputData/borsdata/";
                stockData.htmlFilename += resultList.at(2);
                qDebug() << stockData.htmlFilename;

                // Parse 10 year share key numbers
                m_nofParseArrData = 0;
                if( true == readParseHtmlFile(stockData.htmlFilename, m_parseDataArr, m_nofParseArrData))
                {
                    addDividendPerShare(m_parseDataArr,
                                        m_nofParseArrData,
                                        stockData.stockSymbol,
                                        stockData.stockName);

                    addEarningsPerShare(m_parseDataArr,
                                        m_nofParseArrData,
                                        stockData.stockSymbol,
                                        stockData.stockName);

                    addSolidityPerShare(m_parseDataArr,
                                        m_nofParseArrData,
                                        stockData.stockSymbol,
                                        stockData.stockName);

                    addEquityPerShare(m_parseDataArr,
                                      m_nofParseArrData,
                                      stockData.stockSymbol,
                                      stockData.stockName);


                }
            }

            //qDebug() << resultList.size();
        }
        else
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Error"), QString::fromUtf8("Fail to parse data"));
        }

    }

    file.close();
    return true;
}




/****************************************************************
 *
 * Function:    readFile()
 *
 * Description:
 *
 * QRegExp rx_procent(">[+-]?(\\d+([,]\\d+)?%)<");     // -31.2%
 * QRegExp rx_real(">[+-]?(\\d+([,]\\d+))<");          // 27.37
 *
 ****************************************************************/
bool ParseBorsData::readParseHtmlFile(QString filename,
                                      StockParseDataST *parseDataArr,
                                      int &nofParseArrData)
{
   // CDbHndl db;
    QFile file(filename);
    QString errStr = QString("Fail to open file: %1").arg(filename);
    QString result;
    QString dbgStr;
    bool res;

    m_mainState = MSTATE_YEAR;


    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::critical(NULL, QString::fromUtf8("Fail to open file"), errStr);
        return false;
    }


    QTextStream inStream(&file);
    inStream.setCodec("ISO 8859-1");

    QRegExp rx_year("\\b\\d{4}\\b");                    // 2017
    QRegExp rx_procent(">[+-]?(\\d*([,]\\d+)?%)<");     // -31.2%
    //QRegExp rx_real("(>[0]<)|(>[+-]?(\\d*([,]\\d+))<)"); // 27.37

    QRegExp rx_real(">([+-]?(\\d+(\\,\\d+)?))<"); // 27.37

    while(!inStream.atEnd())
    {
        result = inStream.readLine();

        switch(m_mainState)
        {
        case MSTATE_YEAR:

            if(result.indexOf(START_TAG_YEAR) > 0)
            {
                if(result.indexOf("MSEK") > 0)
                {
                    dbgStr = "year";
                    res = helpParser(result, rx_year, m_mainState, dbgStr, parseDataArr, nofParseArrData);
                    if(res == true)
                    {
                        // Skip last data (not anual year data)
                        nofParseArrData = nofParseArrData - 1;

                        m_mainState = MSTATE_SOLIDITY;
                    }
                    else
                    {
                        file.close();
                        return false;
                    }
                }
            }

            break;
        case MSTATE_SOLIDITY:
            if(result.indexOf("Soliditet") > 0)
            {
                result = inStream.readLine();
                result = inStream.readLine();

                dbgStr = "Sol";
                res = helpParser(result, rx_procent, m_mainState, dbgStr, parseDataArr, nofParseArrData);
                if(res == true)
                {
                    m_mainState = MSTATE_EQUITY_PER_SHARE;
                }
                else
                {
                    file.close();
                    return false;
                }
            }
            break;

        case MSTATE_EQUITY_PER_SHARE:
            if(result.indexOf("Eget Kapital/Aktie") > 0)
            {
                result = inStream.readLine();
                result = inStream.readLine();

                dbgStr = "Eget";
                res = helpParser(result, rx_real, m_mainState, dbgStr, parseDataArr, nofParseArrData);
                if(res == true)
                {
                    m_mainState = MSTATE_EARNINGS_PER_SHARE;
                }
                else
                {
                    file.close();
                    return false;
                }
            }
            break;

        case MSTATE_EARNINGS_PER_SHARE:
            if(result.indexOf("Vinst/Aktie") > 0)
            {
                result = inStream.readLine();
                result = inStream.readLine();

                dbgStr = "vinst";
                res = helpParser(result, rx_real, m_mainState, dbgStr, parseDataArr, nofParseArrData);
                if(res == true)
                {
                    m_mainState = MSTATE_DIVIDEND;
                }
                else
                {
                    file.close();
                    return false;
                }
            }
            break;

        case MSTATE_DIVIDEND:
            if(result.indexOf("Utdelning") > 0)
            {
                result = inStream.readLine();
                result = inStream.readLine();

                dbgStr = "utdel";
                res = helpParser(result, rx_real, m_mainState, dbgStr, parseDataArr, nofParseArrData);
                if(res == true)
                {
                    file.close();
                    return true;
                }

                file.close();
                return false;
            }
            break;
        }
    }

    file.close();
    return false;
}



