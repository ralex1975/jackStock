#include "../../inc/GetHtmlPage/getYahooKeyStatistics.h"
#include <QMessageBox>
#include <QtCore>
#include <QMessageBox>
#include "dbHndl.h"

const QString GetYahooKeyStatistics::yahooKeyStatisticsArr[YAHOO_KEY_STATISTICS_ARR_SIZE] =
{
    QString::fromUtf8("Market Cap"),
    QString::fromUtf8("Enterprise Value"),
    QString::fromUtf8("Trailing P/E"),
    QString::fromUtf8("Forward P/E"),
    QString::fromUtf8("PEG Ratio"),
    QString::fromUtf8("Price/Sales"),
    QString::fromUtf8("Price/Book"),
    QString::fromUtf8("Enterprise Value/Revenue"),
    QString::fromUtf8("Enterprise Value/EBITDA"),
    QString::fromUtf8("Fiscal Year Ends"),
    QString::fromUtf8("Most Recent Quarter"),
    QString::fromUtf8("Profit Margin"),
    QString::fromUtf8("Operating Margin"),
    QString::fromUtf8("Return on Assets"),
    QString::fromUtf8("Return on Equity"),
    QString::fromUtf8("Revenue"),
    QString::fromUtf8("Revenue Per Share"),
    QString::fromUtf8("Qtrly Revenue Growth"),
    QString::fromUtf8("Gross Profit"),
    QString::fromUtf8("EBITDA"),
    QString::fromUtf8("Net Income Avl to Common"),
    QString::fromUtf8("Diluted EPS"),
    QString::fromUtf8("Qtrly Earnings Growth"),
    QString::fromUtf8("Total Cash"),
    QString::fromUtf8("Total Cash Per Share"),
    QString::fromUtf8("Total Debt"),
    QString::fromUtf8("Total Debt/Equity"),
    QString::fromUtf8("Current Ratio"),
    QString::fromUtf8("Book Value Per Share"),
    QString::fromUtf8("Operating Cash Flow"),
    QString::fromUtf8("Levered Free Cash Flow"),
    QString::fromUtf8("Beta"),
    QString::fromUtf8("52-Week Change"),
    QString::fromUtf8("S&amp;P500 52-Week Change"),
    QString::fromUtf8("52-Week High"),
    QString::fromUtf8("52-Week Low"),
    QString::fromUtf8("50-Day Moving Average"),
    QString::fromUtf8("200-Day Moving Average"),
    QString::fromUtf8("Avg Vol (3 month)"),
    QString::fromUtf8("Avg Vol (10 day)"),
    QString::fromUtf8("Shares Outstanding"),
    QString::fromUtf8("Float"),
    QString::fromUtf8("% Held by Insiders"),
    QString::fromUtf8("% Held by Institutions"),
    QString::fromUtf8("Shares Short"),
    QString::fromUtf8("Short Ratio"),
    QString::fromUtf8("Short % of Float"),
    QString::fromUtf8("Shares Short (prior month)"),
    QString::fromUtf8("Forward Annual Dividend Rate"),
    QString::fromUtf8("Forward Annual Dividend Yield"),
    QString::fromUtf8("Trailing Annual Dividend Yield"),
    QString::fromUtf8("Trailing Annual Dividend Yield"),
    QString::fromUtf8("5 Year Average Dividend Yield"),
    QString::fromUtf8("Payout Ratio"),
    QString::fromUtf8("Dividend Date"),
    QString::fromUtf8("Ex-Dividend Date"),
    QString::fromUtf8("Last Split Factor"),
    QString::fromUtf8("Last Split Date")
};






/****************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
GetYahooKeyStatistics::GetYahooKeyStatistics(QObject *parent) :
                                                        QObject(parent),
                                                        m_waitOnServerResp(false),
                                                        m_stockArrIndex(0)
{
    bool res;

    // Connect function in working thread with slotHtmlPageIsRecv() in this thread
    res = QObject::connect(&m_hw1, SIGNAL(sendSignalTextToDlg2(int)), this, SLOT(slotHtmlPageIsRecv(int)));

    if(false == res)
    {
        QMessageBox::information(NULL, QString::fromUtf8("Connect"), QString::fromUtf8("Fail to connect slotHtmlPageIsRecv"));
    }

    m_timeoutTimer = new QTimer;
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
 ****************************************************************/
GetYahooKeyStatistics::~GetYahooKeyStatistics()
{
    delete m_timeoutTimer;
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
 ****************************************************************/
void GetYahooKeyStatistics::
getYahooKeyStatisticsInDb(QString stockListName, int stockListId)
{
     CDbHndl db;

    if(true == db.findTaStockListId(stockListName, stockListId))
    {
        int len = m_stockArr.size();
        for( int i = 0; i < len; i++)
        {
            m_stockArr[i].data.x.clear();
            m_stockArr[i].data.y.clear();
        }

        m_stockArr.clear();


        if(false == db.getAllSnapshotPlotStocksData(stockListName, m_stockArr))
        {
            QMessageBox::information(NULL, QString::fromUtf8("Min Portfölj"), QString::fromUtf8("Fel ingen data hittad"));
            return;
        }

    }
    else
    {
        QMessageBox::information(NULL, QString::fromUtf8("Min Portfölj"), QString::fromUtf8("Fel ingen data hittad"));
        return;
    }

    m_stockArrIndex = 0;
    if(m_stockArrIndex < m_stockArr.size()-1)
    {
        reqNextYahooKeyStatisticsFile(m_stockArr[m_stockArrIndex].stockSymbol);
    }
    else
    {
        QMessageBox::information(NULL, QString::fromUtf8("Yahoo Key Statistics"), QString::fromUtf8("Fel Finns inga symboler att parsa"));
    }
}



/****************************************************************
 *
 * Function:    ()
 *
 * Description: This function:
 *          - Request that Yahoo Key Statistics html page is downloaded
 *          - Set a local path and filename to the file that is downloaded
 *          - Create the html link with a unique stocksymbol to the requested html page.
 *
 *
 *
 *
 ****************************************************************/
void GetYahooKeyStatistics::reqNextYahooKeyStatisticsFile(QString stockSymbol)
{

    QString req;
    bool res;

    if(m_waitOnServerResp == true)
    {
        QMessageBox::information(NULL, QString::fromUtf8("Hämta data"), QString::fromUtf8("Vänta: Upptagen med hämta data"));
        return;
    }


    // Result filename
    QString path = PATH_REQ_YAHOO_KEY_STAT_HTML_PAGE;


    stockSymbol.trimmed();
    QByteArray ba = stockSymbol.toLocal8Bit();
    const char *c_stockSymbol = ba.data();

    qDebug() << "YahooKeyStat stockSymbol: " << stockSymbol;

    // Link used to request Html  page
    req.sprintf("http://finance.yahoo.com/q/ks?s=%s+Key+Statistics", c_stockSymbol);
    QUrl url = req;

    m_waitOnServerResp = false;

    // reqHtmlFile(path, url);

    m_hw1.startRequest(url, path, 0x01);
    m_waitOnServerResp = true;

    m_sendNextReq = true;

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
 ****************************************************************/
bool GetYahooKeyStatistics::parseYahooKeyStatistics(QString filename)
{
    QString outFilename("YahooKeyStat1.txt");
    QFile file(filename);
    QFile outFile(outFilename);

    QString str;
    QString errStr = QString("Fail to open file: %1").arg(filename);



    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::critical(NULL, QString::fromUtf8("Fail to open file"), errStr);
        return false;
    }

    if(!outFile.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::critical(NULL, QString::fromUtf8("Fail to open file"), errStr);
        return false;
    }


    QTextStream out(&outFile);   // we will serialize the data into the file
    QTextStream inStream(&file);

    inStream.setCodec("UTF-8");
    out.setCodec("UTF-8");


    int mainStrIndex;
    QString tmpStr;
    QString mainStr;
    QString subStr;
    QString startToken("<td class=\"yfnc_tablehead1\" width=\"74%\">");

    QString startDataToken("<td class=\"yfnc_tabledata1\">");

    int startTokenLen = startToken.length();
    int startDataTokenLen = startDataToken.length();
    int startIndex;
    int nofchars;
    bool found;
    int n = 0;

    m_yksState = YKS_STATE_READ_FILE;

    while(!inStream.atEnd())
    {
        switch(m_yksState)
        {
        case YKS_STATE_READ_FILE:
            mainStr = inStream.readLine();
            mainStrIndex = mainStr.indexOf(startToken);
            if((mainStrIndex > -1) && (mainStr.length() > 0))
            {
                qDebug() << mainStr;
                m_yksState = YKS_STATE_FIND_TOKEN;
            }
            break;

        case YKS_STATE_FIND_TOKEN:
            found = false;
            mainStrIndex = mainStr.indexOf(startToken);
            startIndex = mainStrIndex + startTokenLen;

            if(mainStrIndex > -1)
            {
                for(int i = startIndex; i < mainStr.length(); i++)
                {
                    if(mainStr.at(i) == '<')
                    {
                        nofchars = i - startIndex;
                        if(nofchars >= 0)
                        {
                            subStr = mainStr.mid(startIndex, nofchars);
                            qDebug() << subStr;
                            // QString::size_type pos = yahooKeyStatisticsArr[n+1].toStdString().find(subStr.toStdString());
                            qWarning() << subStr.contains(yahooKeyStatisticsArr[n]);

                            if(false == subStr.contains(yahooKeyStatisticsArr[n]))
                            {
                                str = QString::fromUtf8("Fail to parse: ");
                                str += yahooKeyStatisticsArr[n];
                                QMessageBox::information(NULL, QString::fromUtf8("Fail:"), str);

                                return false;
                            }

                            out << subStr.toUtf8() << "\n";
                            mainStr = mainStr.right(mainStr.length() - (mainStrIndex + startTokenLen));
                            found = true;
                            n++;
                            m_yksState = YKS_STATE_FIND_DATA_TOKEN;
                            break;
                        }
                    }
                }
            }

            if(found == false)
            {
                m_yksState = YKS_STATE_READ_FILE;
            }
           break;


         case YKS_STATE_FIND_DATA_TOKEN:
            found = false;
            mainStrIndex = mainStr.indexOf(startDataToken);
            startIndex = mainStrIndex + startDataTokenLen;
            tmpStr = mainStr.right(mainStr.length() - startIndex);

            if(mainStrIndex > -1)
            {
                for(int i = startIndex; i < mainStr.length(); i++)
                {
                    if(mainStr.at(i) == '<')
                    {
                        nofchars = i - startIndex;
                        if(nofchars >= 0)
                        {
                            subStr = mainStr.mid(startIndex, nofchars);
                            qDebug() << subStr;
                            out << subStr.toUtf8() << "\n";
                            mainStr = mainStr.right(mainStr.length() - (mainStrIndex + startDataTokenLen));
                            found = true;
                            m_yksState = YKS_STATE_FIND_TOKEN;
                            break;
                        }

                    }
                }
            }

            if(found == false)
            {
                m_yksState = YKS_STATE_READ_FILE;
            }
            break;

        }

    }



    //QMessageBox::information(NULL, QString::fromUtf8("Finish"), QString::fromUtf8("Finish"));
    file.close();
    outFile.close();
    return true;
}





/*******************************************************************
 *
 * Function:    slotHtmlPageIsRecv()
 *
 * Description: This function is invoked when a html page is
 *              completely received.
 *
 *
 *******************************************************************/
void GetYahooKeyStatistics::slotHtmlPageIsRecv(int number)
{

    number = number;
    m_timeoutTimer->stop();

    if(m_sendNextReq == true)
    {
         m_sendNextReq = false;
         // m_timeoutTimer->singleShot(400, this, SLOT(slotReqNextCompanyData()));
         slotReqNextCompanyData();
    }

}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void GetYahooKeyStatistics::slotReqNextCompanyData()
{
    // QString path = PATH_REQ_YAHOO_KEY_STAT_HTML_PAGE;

    // Parse data
    //parseYahooKeyStatistics(path);
    m_waitOnServerResp = false;

    m_stockArrIndex++;
    if(m_stockArrIndex < m_stockArr.size() - 1)
    {
        qDebug() << "next symbol " << m_stockArr[m_stockArrIndex].stockSymbol << m_stockArrIndex;
        // Request next html page
            reqNextYahooKeyStatisticsFile(m_stockArr[m_stockArrIndex].stockSymbol);

    }
    else
    {
        QMessageBox::information(NULL, QString::fromUtf8("Finish"), QString::fromUtf8("Finish"));
    }

}
