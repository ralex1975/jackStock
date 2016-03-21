#include "parsecompanyeconomyinfo.h"
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

#define TAG_START_COMPANY_NAME QObject::tr("&marketplace=11\" class=\"underline\">")
#define TAG_START_COMPANY_NAME1 QObject::tr("&marketplace=14\" class=\"underline\">")
#define TAG_START_COMPANY_NAME2 QObject::tr("&marketplace=15\" class=\"underline\">")
#define TAG_START_COMPANY_NAME3 QObject::tr("&marketplace=24\" class=\"underline\">")

#define TAG_START_COMPANY_NAME_LEN strlen("&marketplace=11\" class=\"underline\">")
#define TAG_END_COMPANY_NAME QObject::tr("</a></div></td>")

#define TAG_BALANS_RAKNING QString::fromUtf8("flerårsöversikt")
#define TAG_RESULT_CALC QString::fromUtf8("Resultaträkning (kSEK)")

#define TAG_TH_END QString::fromUtf8("</th>")
#define TAG_TH_START QString::fromUtf8("<th>")
#define TAG_YEAR_DATA_END QString::fromUtf8("Rörelseintäkter")

#define TAG_TD_END QString::fromUtf8("</td>")

#define TAG_DD_START QString::fromUtf8("<dd>")

#define TAG_OPERATING_REVENUES QString::fromUtf8("Rörelseintäkter")
#define TAG_BALANS_RAKNING1 QString::fromUtf8("Balansräkning (kSEK)")
#define TAG_FIXED_ASSETS QString::fromUtf8("Anläggningstillgångar")
#define TAG_COMPANY_INFO QString::fromUtf8("<div class=\"nyckeltalBgPadding\">")


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
ParseCompanyEconomyInfo::ParseCompanyEconomyInfo(QObject *parent) :
    QObject(parent)
{
    // "Resultaträkning (kSEK)"
    m_tagProfitLossArr.insert(0, QString::fromUtf8("Rörelseintäkter"));
    m_tagProfitLossArr.insert(1, QString::fromUtf8("Rörelsekostnader + avskrivningar"));
    m_tagProfitLossArr.insert(2, QString::fromUtf8("Rörelseresultat efter avskrivningar"));
    m_tagProfitLossArr.insert(3, QString::fromUtf8("Årets resultat efter skatt"));
    m_tagProfitLossArr.insert(4, QString::fromUtf8("Resultat per aktie"));
    m_tagProfitLossArr.insert(5, QString::fromUtf8("Antal aktier"));

    // "Balansräkning (kSEK)"
    m_tagBalanceArr.insert(0, QString::fromUtf8("Anläggningstillgångar"));
    m_tagBalanceArr.insert(1, QString::fromUtf8("Omsättningstillgångar"));
    m_tagBalanceArr.insert(2, QString::fromUtf8("Summa tillgångar"));
    m_tagBalanceArr.insert(3, QString::fromUtf8("Eget kapital"));
    m_tagBalanceArr.insert(4, QString::fromUtf8("Skulder"));
    m_tagBalanceArr.insert(5, QString::fromUtf8("Summa eget kapital och skulder"));

    m_mainState = STATE_MULTI_YEAR_SUMMARY;
}


/****************************************************************
 *
 * Function:    ()
 *
 * Description: Parse Resultaträkning (kSEK)
 *
 *
 *
 *
 ****************************************************************/
bool ParseCompanyEconomyInfo::
parseProfitloss(QTextStream &inStream, int &dateIndex)
{
    QString str;
    CUtil cu;
    int intNumber;
    static int i = 0;
    bool isValidNo;

    str = inStream.readLine();
    str = str.trimmed();

    //qDebug() << "str" << str;

    switch(m_mainState)
    {
    default:
        break;

    case STATE_MULTI_YEAR_SUMMARY:
        if(str.compare(TAG_BALANS_RAKNING) == 0)
        {
           // qDebug() << "str" << str;
           m_mainState = STATE_FIND_BALANCE_SHEET;
        }
        break;

    case STATE_FIND_BALANCE_SHEET:
        if(str.compare(TAG_RESULT_CALC) == 0)
        {
           // qDebug() << "str" << str;
           i = 0;
           m_subState = SUB_STATE_TH_END;
           m_mainState = STATE_FIND_YEAR;
        }
        break;

    case STATE_FIND_YEAR:
        switch(m_subState)
        {
        default:
            break;

        case SUB_STATE_TH_END:
            if(str.compare(TAG_YEAR_DATA_END) == 0)
            {
               // qDebug() << "str" << str;
               m_subState = SUB_STATE_TD_END;
               m_mainState = STATE_GET_RESULT_NUMBER;
            }
            else if(str.compare(TAG_TH_END) == 0)
            {
               // qDebug() << "str" << str;
               m_subState = SUB_STATE_TH_START;
            }
            break;

        case SUB_STATE_TH_START:
            if(str.compare(TAG_YEAR_DATA_END) == 0)
            {
               // qDebug() << "str" << str;
               m_subState = SUB_STATE_TD_END;
               m_mainState = STATE_GET_RESULT_NUMBER;
            }
            else if(str.compare(TAG_TH_START) == 0)
            {
               // qDebug() << "str" << str;
               m_subState = SUB_STATE_YEAR;
            }
            break;

        case SUB_STATE_YEAR:
            if(str.compare(TAG_YEAR_DATA_END) == 0)
            {
               // qDebug() << "str" << str;
               m_subState = SUB_STATE_TD_END;
               m_mainState = STATE_GET_RESULT_NUMBER;
            }
            else if(true == cu.number2Int(str, intNumber))
            {
                // qDebug() << "str aa" << str;
                QString year;
                year.sprintf("%d", intNumber);
                year.toInt(&isValidNo);
                if(true == isValidNo)
                {
                    m_resultProfitLossDateArr.append(year);
                }
                else
                {
                    m_mainState = STATE_MULTI_YEAR_SUMMARY;
                    i = 0;
                    qDebug() << "Error Not a valid no\n" << str;
                    break;
                }
                dateIndex++;
                m_subState = SUB_STATE_TH_END;
            }
            break;
        }
        break;

    case STATE_GET_START_TAG_RESULT_NUMBER:
        if(m_tagProfitLossArr.at(i) == str)
        {
            m_subState = SUB_STATE_TD_END;
            m_mainState = STATE_GET_RESULT_NUMBER;
        }
        break;

    case STATE_GET_RESULT_NUMBER:
        if(true == parseFinNumberInfo(str))
        {
            m_mainState = STATE_GET_START_TAG_RESULT_NUMBER;
            i++;
            return true;
        }
        break;
   }
    return false;
}


/****************************************************************
 *
 * Function:    ()
 *
 * Description: Parse Balansräkning (kSEK)
 *
 *
 *
 *
 ****************************************************************/
bool ParseCompanyEconomyInfo::
parseBalance(QTextStream &inStream, int &dateIndex)
{
    QString str;
    CUtil cu;
    int intNumber;
    static int i = 0;
    bool dataIsValid;

    str = inStream.readLine();
    str = str.trimmed();

    //qDebug() << "str" << str;

    switch(m_mainState)
    {
    default:
        break;

    case STATE_MULTI_YEAR_SUMMARY:
        if(str.compare(TAG_BALANS_RAKNING1) == 0)
        {
           // qDebug() << "str" << str;
           i = 0;
           m_subState = SUB_STATE_TH_END;
           m_mainState = STATE_FIND_YEAR;
        }
        break;

    case STATE_FIND_YEAR:
        switch(m_subState)
        {
        default:
            break;

        case SUB_STATE_TH_END:
            if(str.compare(TAG_FIXED_ASSETS) == 0)
            {
               qDebug() << "str" << str;
               m_subState = SUB_STATE_TD_END;
               m_mainState = STATE_GET_RESULT_NUMBER;
            }
            else if(str.compare(TAG_TH_END) == 0)
            {
               qDebug() << "str" << str;
               m_subState = SUB_STATE_TH_START;
            }
            break;

        case SUB_STATE_TH_START:
            if(str.compare(TAG_FIXED_ASSETS) == 0)
            {
               qDebug() << "str" << str;
               m_subState = SUB_STATE_TD_END;
               m_mainState = STATE_GET_RESULT_NUMBER;
            }
            else if(str.compare(TAG_TH_START) == 0)
            {
               // qDebug() << "str" << str;
               m_subState = SUB_STATE_YEAR;
            }
            break;

        case SUB_STATE_YEAR:
            if(str.compare(TAG_FIXED_ASSETS) == 0)
            {
               // qDebug() << "str" << str;
               m_subState = SUB_STATE_TD_END;
               m_mainState = STATE_GET_RESULT_NUMBER;
            }
            else if(true == cu.number2Int(str, intNumber))
            {
                qDebug() << "Balance year:" << str;
                QString year;
                year.sprintf("%d", intNumber);
                year.toInt(&dataIsValid);

                if(true == dataIsValid)
                {
                    m_resultProfitLossDateArr.append(year);
                }
                else
                {
                    i = 0;
                    m_mainState = STATE_MULTI_YEAR_SUMMARY;
                    return false;
                }

                dateIndex++;
                m_subState = SUB_STATE_TH_END;
            }
            break;
        }
        break;

    case STATE_GET_START_TAG_RESULT_NUMBER:
        qDebug() << str << "==" << m_tagBalanceArr.at(i);
        //qDebug() << str;
        if(m_tagBalanceArr.at(i) == str)
        {
            m_subState = SUB_STATE_TD_END;
            m_mainState = STATE_GET_RESULT_NUMBER;
        }
        break;

    case STATE_GET_RESULT_NUMBER:
        if(true == parseFinNumberInfo(str))
        {
          m_mainState = STATE_GET_START_TAG_RESULT_NUMBER;
          i++;
          return true;
        }
        break;
   }
    return false;
}


/****************************************************************
 *
 * Function:    ()
 *
 * Description: Parse Company info
 *
 *
 *
 *
 ****************************************************************/
bool ParseCompanyEconomyInfo::
parseFile2(QTextStream &inStream, QString &result)
{
    QString str;
    static int i = 0;

    str = inStream.readLine();
    str = str.trimmed();

    //qDebug() << "str" << str;

    switch(m_mainState)
    {
    default:
        break;

    case STATE_MULTI_YEAR_SUMMARY:
        if(str.compare(TAG_COMPANY_INFO) == 0)
        {
           // qDebug() << "str" << str;
           i = 0;
           m_mainState = STATE_GET_RESULT_NUMBER;
        }
        break;

    case STATE_GET_RESULT_NUMBER:
        if(true == parseCompanyInfo(str, result))
        {
            i++;
            return true;
        }
        break;
   }
    return false;
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
bool ParseCompanyEconomyInfo::
parseFinNumberInfo(QString str)
{
    CUtil cu;
    double dbNumber;
    int len;
    int i = 0;
    int start;
    int end;
    QString number;
    QString tmpNumber;
    bool res = false;
    bool IsValidNo;


    switch(m_subState)
    {
    default:
        break;

    case SUB_STATE_TD_END:
        if(str.compare(TAG_TD_END) == 0)
        {
           // qDebug() << "str" << str;
           m_subState = SUB_STATE_NUMBER;
        }
        break;

    case SUB_STATE_NUMBER:

        while(str.indexOf(QString::fromUtf8("<td>")) > -1)
        {
            len = str.length();
            // qDebug() << len;
            // qDebug() << str;
            start = str.indexOf(QString::fromUtf8("<td>"));
            if(start <= -1)
            {
                return false;
            }
            start += QString::fromUtf8("<td>").length();
            // qDebug() << start;



            end = str.indexOf(QString::fromUtf8("</td>"));
            if(end <= -1)
            {
                return false;
            }
            end -= QString::fromUtf8("</td>").length();
            end += 1;
            // qDebug() << end;

            number = str.mid(start, end);
            // qDebug() << number;

            if(true == cu.number2DoubleRemoveSpace(number, dbNumber))
            {
                // qDebug() << number;

                if(number.indexOf(",") > -1)
                    tmpNumber.sprintf("%.2f",dbNumber);
                else
                    tmpNumber.sprintf("%.0f",dbNumber);
                     qDebug() << "tmpNumber" << tmpNumber;
                tmpNumber.toDouble(&IsValidNo);
                if(true == IsValidNo)
                {
                    m_resultProfitLossNumberArr.append(tmpNumber);
                    res = true;
                }
                else
                {
                    tmpNumber.append("-9e9");
                    m_resultProfitLossNumberArr.append(tmpNumber);
                    // res = true;
                    i++;
                    return true;
                }
                i++;

            }
            else
            {
                if(number.size() < 1)
                {
                    tmpNumber.append("-9e9");
                    m_resultProfitLossNumberArr.append(tmpNumber);
                    // res = true;
                    i++;
                    return true;
                }
            }
            end += QString::fromUtf8("</td>").length();
            str = str.mid((end-1), len);
            // qDebug() << str;

            start = str.indexOf(QString::fromUtf8("</td>"));
            // qDebug() << start;
            start += QString::fromUtf8("</td>").length();
            str = str.mid((start), len);
            // qDebug() << str;
        }
        break;
    }

    return res;
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
bool ParseCompanyEconomyInfo::
parseCompanyInfo(QString str, QString &result)
{
    int len;
    int start;
    int end;


    len = str.length();
    qDebug() << len;
    qDebug() << str;
    start = str.indexOf(QString::fromUtf8("<dd>"));
    if(start <= -1)
    {
        return false;
    }
    start += QString::fromUtf8("<dd>").length();
    qDebug() << start;

    end = str.indexOf(QString::fromUtf8("</dd>"));
    if(end <= -1)
    {
        return false;
    }
    end -= QString::fromUtf8("</dd>").length();
    end += 1;
    qDebug() << end;

    result = str.mid(start, end);
    qDebug() << result;

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
bool ParseCompanyEconomyInfo::
parseFileGetId(QTextStream &inStream, QString &result)
{
    QString str;
    QString id;
    int index;

    str = inStream.readLine();

    index = str.indexOf(TAG_START_COMPANY_NAME);
    if(index == -1)
        index = str.indexOf(TAG_START_COMPANY_NAME1);
    if(index == -1)
        index = str.indexOf(TAG_START_COMPANY_NAME2);
    if(index == -1)
        index = str.indexOf(TAG_START_COMPANY_NAME3);

    if(index > -1)
    {
        qDebug() << "str" << str;
        result = str.right(str.length()- index - TAG_START_COMPANY_NAME_LEN);

        id = str.left(str.length()- TAG_START_COMPANY_NAME_LEN);

        QString substring("			<td class=\"text\"><div class=\"truncate\"><a href=\"/mux/web/marknaden/aktiehemsidan/index.html?identifier=");

        id = str.right(str.length() - substring.length());

        id = id.left(id.indexOf("&"));

        qDebug() << "id =" << id;
        qDebug() << "result =" << result;


        index = result.indexOf(TAG_END_COMPANY_NAME);
        if(index > -1)
        {
            result = result.left(index);
            result = result.trimmed();
            qDebug() << "result =" << result << "\n\n";
            return true;
        }
    }
    return false;
}

#define STOP_GOT_ERROR  fclose(fd); \
                        file.close(); \
                        return false;

/****************************************************************
 *
 * Function:    parseStockPrices()
 *
 * Description:
 *
 * Link to get Stock ids: https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Sverige&lista=1_1&large=on&mid=on&sektor=0&subtyp=price&sortera=aktie&sorteringsordning=stigande
 * Company info text:     https://www.nordnet.se/mux/web/marknaden/aktiehemsidan/bolagsfakta.html?identifier=3966&marketplace=11
 * History:               https://www.nordnet.se/mux/web/marknaden/aktiehemsidan/nyckeltal.html?identifier=3966&marketplace=11
 *
 ****************************************************************/
bool ParseCompanyEconomyInfo::
readFile(QString filename, QString assetName)
{
    int k;
    QFile file(filename);
    QString errStr = QString("Fail to open file: %1").arg(filename);
    bool res;
    FILE *fd;
    CDbHndl db;
    NordnetBalanceIncomeSheet_ST dataArr[80];
    bool isValid;


    fd = fopen("ecoRes.txt", "w+");


    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::critical(NULL, QObject::tr("Fail to open file"), errStr);
        return false;
    }

    QTextStream inStream(&file);
    inStream.setCodec("ISO 8859-1");

    int maxIndex;
    int dateIndex = 0;
    int i = 0;
    int j = 0;

    m_resultProfitLossNumberArr.clear();
    m_resultProfitLossDateArr.clear();

    while(!inStream.atEnd())
    {
        res = parseProfitloss(inStream, dateIndex);

        // qDebug() << "dateIndex" << dateIndex;
        // qDebug() << "j" << j;
        if(res == true)
        {
            if(j == 0)
            {
                maxIndex = m_resultProfitLossDateArr.size();
                fprintf(fd, "Datum:\t\t");
                for(i= 0; i < m_resultProfitLossDateArr.size(); i++)
                {
					qDebug() << "i" << i << "resultProfitLossDateArr" << m_resultProfitLossDateArr.at(i).toAscii().constData();
                    dataArr[i].year = m_resultProfitLossDateArr.at(i).toInt(&isValid);
                    if(true == isValid)
                    {
                        // qDebug() << "i" << i << "resultProfitLossDateArr" << m_resultProfitLossDateArr.at(i).toAscii().constData();
                        fprintf(fd, "%s,\t\t", m_resultProfitLossDateArr.at(i).toAscii().constData());
                    }
                    else
                    {
                        STOP_GOT_ERROR;
                    }
                }
                fprintf(fd, "\n");
            }


            switch(j)
            {
            case 0:
                for(k = 0; k < m_resultProfitLossNumberArr.size(); k++)
                {

                    dataArr[k].operatingIncome = m_resultProfitLossNumberArr.at(k).toDouble(&isValid);
                    if(false == isValid)
                    {
                        STOP_GOT_ERROR;
                    }
                }
                break;

            case 1:
                for(k = 0; k < m_resultProfitLossNumberArr.size(); k++)
                {
                    // m_resultProfitLossNumberArr.at(k).is
                    dataArr[k].operatingExpensesAndDepreciation = m_resultProfitLossNumberArr.at(k).toDouble(&isValid);
                    if(false == isValid)
                    {
                        STOP_GOT_ERROR;
                    }
                }
                break;

            case 2:
                for(k = 0; k < m_resultProfitLossNumberArr.size(); k++)
                {
                    dataArr[k].operatingIncomeAfterDepreciation = m_resultProfitLossNumberArr.at(k).toDouble(&isValid);
                    if(false == isValid)
                    {
                        STOP_GOT_ERROR;
                    }
                }
                break;

            case 3:
                for(k = 0; k < m_resultProfitLossNumberArr.size(); k++)
                {
                    dataArr[k].netProfitAfterTax = m_resultProfitLossNumberArr.at(k).toDouble(&isValid);
                    if(false == isValid)
                    {
                        STOP_GOT_ERROR;
                    }
                }
                break;

            case 4:
                for(k = 0; k < m_resultProfitLossNumberArr.size(); k++)
                {
                    dataArr[k].earningsPerShare = m_resultProfitLossNumberArr.at(k).toDouble(&isValid);
                    if(false == isValid)
                    {
                        STOP_GOT_ERROR;
                    }
                }
                break;


            case 5:
                for(k = 0; k < m_resultProfitLossNumberArr.size(); k++)
                {
                    dataArr[k].numberOfShares = m_resultProfitLossNumberArr.at(k).toDouble(&isValid);
                    if(false == isValid)
                    {
                        STOP_GOT_ERROR;
                    }
                }
                break;
            }


            if(j < 6)
            {
                qDebug() << "1 tagProfitLossArr" << m_tagProfitLossArr.at(j).toAscii().constData();
                fprintf(fd, "%s: ", m_tagProfitLossArr.at(j).toAscii().constData());
            }

            for(i= 0; i < m_resultProfitLossNumberArr.size(); i++)
            {
                qDebug() << "1 resultProfitLossNumberArr" << m_resultProfitLossNumberArr.at(i).toAscii().constData();
                fprintf(fd, "%s,\t", m_resultProfitLossNumberArr.at(i).toAscii().constData());
            }

            fprintf(fd, "\n");

            m_resultProfitLossNumberArr.clear();
            j++;
            if(j>= 6)
            {
                break;
            }
        }
    }

    dateIndex = 0;
    i = 0;
    j = 0;

    m_resultProfitLossNumberArr.clear();
    m_resultProfitLossDateArr.clear();
    m_mainState = STATE_MULTI_YEAR_SUMMARY;

    qDebug() << "1. ProfitLossDateArr.size()" << m_resultProfitLossDateArr.size();

    while(!inStream.atEnd())
    {
        res = parseBalance(inStream, dateIndex);

        if(res == true)
        {
            if(j == 0)
            {
                fprintf(fd, "Datum:\t\t");
                qDebug() << "ProfitLossDateArr.size()" << m_resultProfitLossDateArr.size();
                for(i= 0; i < m_resultProfitLossDateArr.size(); i++)
                {
                    qDebug() << "i" << i << m_resultProfitLossDateArr.at(i).toAscii().constData();
                    fprintf(fd, "%s,\t\t", m_resultProfitLossDateArr.at(i).toAscii().constData());

                }
                fprintf(fd, "\n");
            }


            switch(j)
            {
            case 0:
                for(k = 0; k < m_resultProfitLossNumberArr.size(); k++)
                {
                    dataArr[k].fixedAssets = m_resultProfitLossNumberArr.at(k).toDouble(&isValid);
                    if(false == isValid)
                    {
                        STOP_GOT_ERROR;
                    }
                }
                break;

            case 1:
                for(k = 0; k < m_resultProfitLossNumberArr.size(); k++)
                {
                    //dataArr[j].currentAssets = m_resultProfitLossNumberArr.at(k).toDouble(&isValid);
                    dataArr[k].currentAssets = m_resultProfitLossNumberArr.at(k).toDouble(&isValid);
                    qDebug() << "currentAssets" << m_resultProfitLossNumberArr.at(k).toDouble(&isValid);

                    if(false == isValid)
                    {
                        STOP_GOT_ERROR;
                    }
                }
                break;

            case 2:
                for(k = 0; k < m_resultProfitLossNumberArr.size(); k++)
                    dataArr[k].totalAssets = dataArr[k].fixedAssets + dataArr[k].currentAssets;
                break;

            case 3:
                for(k = 0; k < m_resultProfitLossNumberArr.size(); k++)
                {
                    dataArr[k].equity = m_resultProfitLossNumberArr.at(k).toDouble(&isValid);
                    if(false == isValid)
                    {
                        STOP_GOT_ERROR;
                    }
                }
                break;

            case 4:
                for(k = 0; k < m_resultProfitLossNumberArr.size(); k++)
                {
                    dataArr[k].liabilities = m_resultProfitLossNumberArr.at(k).toDouble(&isValid);
                    if(false == isValid)
                    {
                        STOP_GOT_ERROR;
                    }
                }
                break;

            case 5:
                for(k = 0; k < m_resultProfitLossNumberArr.size(); k++)
                {
                    dataArr[k].totalEquityAndLiabilities = m_resultProfitLossNumberArr.at(k).toDouble(&isValid);
                    if(false == isValid)
                    {
                        STOP_GOT_ERROR;
                    }
                }
                break;
            }



            if(j < 6)
            {
                fprintf(fd, "%s: ", m_tagBalanceArr.at(j).toAscii().constData());
            }

            for(i= 0; i < m_resultProfitLossNumberArr.size(); i++)
            {
                fprintf(fd, "%s,\t", m_resultProfitLossNumberArr.at(i).toAscii().constData());
            }

            fprintf(fd, "\n");
            m_resultProfitLossNumberArr.clear();
            j++;

            if(j>= 6)
            {
                 break;
            }
        }
    }



    //==============
    m_mainState = STATE_MULTI_YEAR_SUMMARY;
    QString result;
    i = 0;


    while(!inStream.atEnd())
    {
        res = parseFile2(inStream, result);
        if(res == true)
        {
            qDebug() << "Company Result" << result;
            fprintf(fd, "%s\n", result.toAscii().constData());
            switch(i)
            {
            case 0:
                dataArr[0].assetName = assetName; // result.toAscii().constData();
                break;
            case 1:
                break;
            case 2:
                dataArr[0].assetGroup = result.toAscii().constData();
                break;
            }
            i++;
            if(i == 3)
                break;
        }
    }

    for(i = 0; i < maxIndex; i++)
    {
        dataArr[i].assetName = dataArr[0].assetName;
        dataArr[i].assetGroup = dataArr[0].assetGroup;
        dataArr[i].yearAssetName.sprintf("%d%s",dataArr[i].year, dataArr[i].assetName.toLocal8Bit().constData());
    }


    qDebug() << "maxIndex" << maxIndex;
    for(i = 0; i < maxIndex; i++)
    {
        db.insertNordnetBalanceAndIncomeData(dataArr[i]);
        qDebug() << "i" << i;
    }


    fclose(fd);
    file.close();
    return true;
}




