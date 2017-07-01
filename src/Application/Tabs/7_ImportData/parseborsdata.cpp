#include "parseborsdata.h"
#include "QFile"
#include "QMessageBox"
#include "QTextStream"
#include "QDebug"

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
                               QString *resArr,
                               int &nofArrData)
{
    int index;
    int startIndex;
    bool res = false;
    QString value;

    nofArrData = 0;

    do
    {
        res = false;
        index = rx.indexIn(result);

        if( index != -1 )
        {
            value = rx.cap(0);

            switch(mainState)
            {
            case MSTATE_YEAR:
                qDebug() << dbgStr << value;
                resArr[nofArrData++] = value;
                res = true;
                break;

            case MSTATE_SOLIDITY:
                res = cleanupParsedProcentValue(value, dbgStr);
                break;

            case MSTATE_EQUITY_PER_SHARE:
                res = cleanupParsedRealValue(value, dbgStr);
                break;

            case MSTATE_EARNINGS_PER_SHARE:
                res = cleanupParsedRealValue(value, dbgStr);
                break;

            case MSTATE_DIVIDEND:
                res = cleanupParsedRealValue(value, dbgStr);
                break;
            }

            if(true == res)
            {
                resArr[nofArrData++] = value;
                startIndex = result.length() - index + 1;
                if(startIndex > -1 && startIndex < result.length())
                {
                    result = result.right(startIndex - value.length());
                }
            }
            else
            {
                nofArrData = 0;
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



/****************************************************************
 *
 * Function:    readFile()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
bool ParseBorsData::readFile(QString filename)
{
   // CDbHndl db;
    QFile file(filename);
    QString errStr = QString("Fail to open file: %1").arg(filename);
    QString result;
    QString value;
    double dbValue;
    int index;
    int nofRemoveChars;
    int startIndex;
    QString resArr[100];
    QString dbgStr;
    int nofArrData;
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
    QRegExp rx_procent(">[+-]?(\\d+([,]\\d+)?%)<");     // -31.2%
    QRegExp rx_real(">[+-]?(\\d+([,]\\d+))<");          // 27.37

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
                    res = helpParser(result, rx_year, m_mainState, dbgStr, resArr, nofArrData);
                    if(res == true)
                    {
                        m_mainState = MSTATE_SOLIDITY;
                    }
                    else
                    {
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
                res = helpParser(result, rx_procent, m_mainState, dbgStr, resArr, nofArrData);
                if(res == true)
                {
                    m_mainState = MSTATE_EQUITY_PER_SHARE;
                }
                else
                {
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
                res = helpParser(result, rx_real, m_mainState, dbgStr, resArr, nofArrData);
                if(res == true)
                {
                    m_mainState = MSTATE_EARNINGS_PER_SHARE;
                }
                else
                {
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
                res = helpParser(result, rx_real, m_mainState, dbgStr, resArr, nofArrData);
                if(res == true)
                {
                    m_mainState = MSTATE_DIVIDEND;
                }
                else
                {
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
                res = helpParser(result, rx_real, m_mainState, dbgStr, resArr, nofArrData);
                if(res == true)
                {
                    m_mainState = MSTATE_DIVIDEND;
                }
                else
                {
                    return false;
                }

                file.close();
                return true;
            }
            break;
        }
    }

    file.close();
    return false;
}



#if 0
/****************************************************************
 *
 * Function:    readFile()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
bool ParseBorsData::readFile(QString filename)
{
   // CDbHndl db;
    QFile file(filename);
    QString errStr = QString("Fail to open file: %1").arg(filename);
    QString result;
    QString value;
    double dbValue;
    int index;
    int nofRemoveChars;
    int startIndex;
    QString resArr[100];
    QString dbgStr;
    int nofArrData;
    bool res;

    m_mainState = MSTATE_YEAR;


    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::critical(NULL, QString::fromUtf8("Fail to open file"), errStr);
        return false;
    }


    QTextStream inStream(&file);
    inStream.setCodec("ISO 8859-1");

    QRegExp rx_year("\\b\\d{4}\\b");
    QRegExp rx_procent(">[+-]?(\\d+([,]\\d+)?%)<");
    QRegExp rx_real(">[+-]?(\\d+([,]\\d+))<");

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
                    #if 0
                    do
                    {
                        index = rx.indexIn(result);

                        if( index != -1 )
                        {
                            value = rx.cap(0);
                            nofRemoveChars = value.length();

                            qDebug() << value;
                            startIndex = result.length() - index + 1;
                            if(startIndex > -1 && startIndex < result.length())
                            {
                                result = result.right(startIndex - nofRemoveChars);
                            }
                        }
                    }while(index > -1);
                    #endif
                    dbgStr = "year";
                    res = helpParser(result, rx_year, m_mainState, dbgStr, resArr, nofArrData);
                    if(res == true)
                    {
                        m_mainState = MSTATE_SOLIDITY;
                    }
                    else
                    {
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

                #if 0
                do
                {
                    index = rx_procent.indexIn(result);

                    if( index != -1 )
                    {
                        value = rx_procent.cap(0);
                        nofRemoveChars = value.length();
                        if((value[0] == '>') && (value[value.length() -1] == '<'))
                        {
                            value = value.mid(1, value.length() - 3);
                            value = value.replace(",",".");
                            dbValue = value.toDouble();
                            dbValue = dbValue / 100.0;
                            value.sprintf("%.2f", dbValue);
                            qDebug() << "sol" << value;
                        }

                        // value = rx_procent.cap(0);
                        // qDebug() << "procent" << value;
                        startIndex = result.length() - index + 1;
                        if((startIndex > -1) &&
                           (startIndex < result.length()) &&
                           (nofRemoveChars > -1))
                        {
                            result = result.right(startIndex - nofRemoveChars);
                        }
                    }
                }while(index > -1);
                #endif

                dbgStr = "Sol";
                res = helpParser(result, rx_procent, m_mainState, dbgStr, resArr, nofArrData);
                if(res == true)
                {
                    m_mainState = MSTATE_EQUITY_PER_SHARE;
                }
                else
                {
                    return false;
                }


            }
            break;

        case MSTATE_EQUITY_PER_SHARE:
            if(result.indexOf("Eget Kapital/Aktie") > 0)
            {
                result = inStream.readLine();
                result = inStream.readLine();

                do
                {
                    index = rx_real.indexIn(result);

                    if( index != -1 )
                    {
                        value = rx_real.cap(0);
                        if((value[0] == '>') && (value[value.length() -1] == '<'))
                        {
                            value = value.mid(1, value.length() - 2);
                            value = value.replace(",",".");
                            qDebug() << "Eget" << value;
                        }

                        startIndex = result.length() - index + 1;
                        if(startIndex > -1 && startIndex < result.length())
                        {
                            result = result.right(startIndex - value.length());
                        }
                    }
                }while(index > -1);

                m_mainState = MSTATE_EARNINGS_PER_SHARE;

            }
            break;
        case MSTATE_EARNINGS_PER_SHARE:
            if(result.indexOf("Vinst/Aktie") > 0)
            {
                result = inStream.readLine();
                result = inStream.readLine();

                do
                {
                    index = rx_real.indexIn(result);

                    if( index != -1 )
                    {
                        value = rx_real.cap(0);
                        qDebug() << "Vinst" << value;
                        startIndex = result.length() - index + 1;
                        if(startIndex > -1 && startIndex < result.length())
                        {
                            result = result.right(startIndex - value.length());
                        }
                    }
                }while(index > -1);

                m_mainState = MSTATE_DIVIDEND;

            }
            break;
        case MSTATE_DIVIDEND:
            if(result.indexOf("Utdelning") > 0)
            {
                result = inStream.readLine();
                result = inStream.readLine();

                do
                {
                    index = rx_real.indexIn(result);

                    if( index != -1 )
                    {
                        value = rx_real.cap(0);
                        qDebug() << "Utdelning" << value;
                        startIndex = result.length() - index + 1;
                        if(startIndex > -1 && startIndex < result.length())
                        {
                            result = result.right(startIndex - value.length());
                        }
                    }
                }while(index > -1);

                file.close();
                return true;
            }
            break;
        }
    }

    file.close();
    return false;
}
#endif

#if 0
/****************************************************************
 *
 * Function:    readFile()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
bool ParseBorsData::readFile(QString filename)
{
   // CDbHndl db;
    QFile file(filename);
    QString errStr = QString("Fail to open file: %1").arg(filename);
    QString result;
    QString value;
    double dbValue;
    int index;
    int nofRemoveChars;
    int startIndex;


    m_mainState = MSTATE_YEAR;


    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::critical(NULL, QString::fromUtf8("Fail to open file"), errStr);
        return false;
    }


    QTextStream inStream(&file);
    inStream.setCodec("ISO 8859-1");

    QRegExp rx("\\b\\d{4}\\b");
    QRegExp rx_procent(">[+-]?(\\d+([,]\\d+)?%)<");
    QRegExp rx_real(">[+-]?(\\d+([,]\\d+))<");

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
                    do
                    {
                        index = rx.indexIn(result);

                        if( index != -1 )
                        {
                            qDebug() << rx.cap(0);
                            startIndex = result.length() - index + 1;
                            if(startIndex > -1 && startIndex < result.length())
                            {
                                result = result.right(startIndex - 5);
                            }
                        }
                    }while(index > -1);
                    m_mainState = MSTATE_SOLIDITY;
                }
            }

            break;
        case MSTATE_SOLIDITY:
            if(result.indexOf("Soliditet") > 0)
            {
                result = inStream.readLine();
                result = inStream.readLine();

                do
                {
                    index = rx_procent.indexIn(result);

                    if( index != -1 )
                    {
                        value = rx_procent.cap(0);
                        nofRemoveChars = value.length();
                        if((value[0] == '>') && (value[value.length() -1] == '<'))
                        {
                            value = value.mid(1, value.length() - 3);
                            value = value.replace(",",".");
                            dbValue = value.toDouble();
                            dbValue = dbValue / 100.0;
                            value.sprintf("%.2f", dbValue);
                            qDebug() << "sol" << value;
                        }

                        // value = rx_procent.cap(0);
                        // qDebug() << "procent" << value;
                        startIndex = result.length() - index + 1;
                        if((startIndex > -1) &&
                           (startIndex < result.length()) &&
                           (nofRemoveChars > -1))
                        {
                            result = result.right(startIndex - nofRemoveChars);
                        }
                    }
                }while(index > -1);

                m_mainState = MSTATE_EQUITY_PER_SHARE;

            }
            break;

        case MSTATE_EQUITY_PER_SHARE:
            if(result.indexOf("Eget Kapital/Aktie") > 0)
            {
                result = inStream.readLine();
                result = inStream.readLine();

                do
                {
                    index = rx_real.indexIn(result);

                    if( index != -1 )
                    {
                        value = rx_real.cap(0);
                        if((value[0] == '>') && (value[value.length() -1] == '<'))
                        {
                            value = value.mid(1, value.length() - 2);
                            value = value.replace(",",".");
                            qDebug() << "Eget" << value;
                        }

                        startIndex = result.length() - index + 1;
                        if(startIndex > -1 && startIndex < result.length())
                        {
                            result = result.right(startIndex - value.length());
                        }
                    }
                }while(index > -1);

                m_mainState = MSTATE_EARNINGS_PER_SHARE;

            }
            break;
        case MSTATE_EARNINGS_PER_SHARE:
            if(result.indexOf("Vinst/Aktie") > 0)
            {
                result = inStream.readLine();
                result = inStream.readLine();

                do
                {
                    index = rx_real.indexIn(result);

                    if( index != -1 )
                    {
                        value = rx_real.cap(0);
                        qDebug() << "Vinst" << value;
                        startIndex = result.length() - index + 1;
                        if(startIndex > -1 && startIndex < result.length())
                        {
                            result = result.right(startIndex - value.length());
                        }
                    }
                }while(index > -1);

                m_mainState = MSTATE_DIVIDEND;

            }
            break;
        case MSTATE_DIVIDEND:
            if(result.indexOf("Utdelning") > 0)
            {
                result = inStream.readLine();
                result = inStream.readLine();

                do
                {
                    index = rx_real.indexIn(result);

                    if( index != -1 )
                    {
                        value = rx_real.cap(0);
                        qDebug() << "Utdelning" << value;
                        startIndex = result.length() - index + 1;
                        if(startIndex > -1 && startIndex < result.length())
                        {
                            result = result.right(startIndex - value.length());
                        }
                    }
                }while(index > -1);

                file.close();
                return true;
                // m_mainState = MSTATE_DIVIDEND;

            }
            break;
        }
    }

    file.close();
    return false;
}
#endif


#if 0
/****************************************************************
 *
 * Function:    readFile()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
bool ParseBorsData::readFile(QString filename)
{
   // CDbHndl db;
    QFile file(filename);
    QString errStr = QString("Fail to open file: %1").arg(filename);
    QString result;
    QString value;
    int index;
    int startIndex;


    m_mainState = MSTATE_YEAR;



    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::critical(NULL, QString::fromUtf8("Fail to open file"), errStr);
        return false;
    }


    QTextStream inStream(&file);
    inStream.setCodec("ISO 8859-1");
    //inStream.setCodec("UTF-8");

    QRegExp rx("\\b\\d{4}\\b");
    QRegExp rx_procent(">[+-]?(\\d+([,]\\d+)?%)<");
    QRegExp rx_real(">[+-]?(\\d+([,]\\d+))<");

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
                    do
                    {
                        index = rx.indexIn(result);

                        if( index != -1 )
                        {
                            qDebug() << rx.cap(0);
                            startIndex = result.length() - index + 1;
                            if(startIndex > -1 && startIndex < result.length())
                            {
                                result = result.right(startIndex - 5);
                            }
                        }
                    }while(index > -1);
                    m_mainState = MSTATE_SOLIDITY;
                }
            }

            break;
        case MSTATE_SOLIDITY:
            if(result.indexOf("Soliditet") > 0)
            {
                result = inStream.readLine();
                result = inStream.readLine();

                do
                {
                    index = rx_procent.indexIn(result);

                    if( index != -1 )
                    {
                        value = rx_procent.cap(0);
                        qDebug() << "procent" << value;
                        startIndex = result.length() - index + 1;
                        if(startIndex > -1 && startIndex < result.length())
                        {
                            result = result.right(startIndex - value.length());
                        }
                    }
                }while(index > -1);

                m_mainState = MSTATE_EQUITY_PER_SHARE;

            }
            break;

        case MSTATE_EQUITY_PER_SHARE:
            if(result.indexOf("Eget Kapital/Aktie") > 0)
            {
                result = inStream.readLine();
                result = inStream.readLine();

                do
                {
                    index = rx_real.indexIn(result);

                    if( index != -1 )
                    {
                        value = rx_real.cap(0);
                        if((value[0] == '>') && (value[value.length() -1] == '<'))
                        {
                            value = value.mid(1, value.length() - 2);
                            value = value.replace(",",".");
                            qDebug() << "Eget" << value;
                        }
                        startIndex = result.length() - index + 1;
                        if(startIndex > -1 && startIndex < result.length())
                        {
                            result = result.right(startIndex - value.length());
                        }
                    }
                }while(index > -1);

                m_mainState = MSTATE_EARNINGS_PER_SHARE;

            }
            break;
        case MSTATE_EARNINGS_PER_SHARE:
            if(result.indexOf("Vinst/Aktie") > 0)
            {
                result = inStream.readLine();
                result = inStream.readLine();

                do
                {
                    index = rx_real.indexIn(result);

                    if( index != -1 )
                    {
                        value = rx_real.cap(0);
                        qDebug() << "Vinst" << value;
                        startIndex = result.length() - index + 1;
                        if(startIndex > -1 && startIndex < result.length())
                        {
                            result = result.right(startIndex - value.length());
                        }
                    }
                }while(index > -1);

                m_mainState = MSTATE_DIVIDEND;

            }
            break;
        case MSTATE_DIVIDEND:
            if(result.indexOf("Utdelning") > 0)
            {
                result = inStream.readLine();
                result = inStream.readLine();

                do
                {
                    index = rx_real.indexIn(result);

                    if( index != -1 )
                    {
                        value = rx_real.cap(0);
                        qDebug() << "Utdelning" << value;
                        startIndex = result.length() - index + 1;
                        if(startIndex > -1 && startIndex < result.length())
                        {
                            result = result.right(startIndex - value.length());
                        }
                    }
                }while(index > -1);

                file.close();
                return true;
                // m_mainState = MSTATE_DIVIDEND;

            }
            break;
        }
    }

    file.close();
    return false;
}
#endif

