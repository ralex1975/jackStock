#include "parseavanzadata.h"
#include <QFileDialog>
#include <QtCore>
#include <QMessageBox>
#include "dbHndl.h"
#include "common.h"

#define PARSE_TOCKEN ";"
#define MAX_NOF_TOCKENS 10

/****************************************************************
 *
 * Function:        CParseAvanzaData()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
CParseAvanzaData::CParseAvanzaData()
{
}



/****************************************************************
 *
 * Function:    parseData()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
bool CParseAvanzaData::parseData(QString filename)
{
    bool res;
    QString str;
    bool firstTime = true;
    parserData_ST result;
    CDbHndl cdh;

    if(filename.length() < 1)
    {
        return false;
    }


    QFile file(filename);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QString errStr = QString("Fail to open file: %1").arg(filename);
        QMessageBox::critical(NULL, QObject::tr("Fail to open file"), errStr);
        return false;
    }

    QTextStream inStream(&file);
    inStream.setCodec("ISO 8859-1");

    // cdh.openDb(PATH_JACK_STOCK_DB);

     while(!inStream.atEnd())
     {
         str = inStream.readLine();
         if(firstTime == true)
         {
            // Remove head line
            firstTime = false;
         }
         else
         {
             res = parseOneLine(str, result);
             if(res == false)
             {
                 file.close();
                 // cdh.closeDb();
                 return false;
             }


             // lägg till dessa
             // result.nofItems;
             // result.totalPrice;
             if(false == cdh.insertAvanzaAssetData( result.account,
                                                    result.assetName,
                                                    result.date,
                                                    result.typeOfData,
                                                    result.assetPrice,
                                                    result.currency))
             {
                 // QMessageBox::critical(NULL, QObject::tr("Insert Avanza data"), QObject::tr("Fail to insert Avanza data"));
                 // file.close();
                 // cdh.closeDb();
                 // return false;
             }
         }
     }

     QMessageBox::information(NULL, QObject::tr("Read file"), QObject::tr("Finish reading file"));
     file.close();
     // cdh.closeDb();
     return true;
}


/****************************************************************
 *
 * Function:    parseOneLine()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
bool CParseAvanzaData::parseOneLine(QString str, parserData_ST &result)
{
    QString subResult;
    QString line;
    bool firstTime;
    bool res;
    int nofTocken;
    int index;


    index = str.indexOf(PARSE_TOCKEN);

    firstTime = true;
    nofTocken = 0;
    while(index > -1)
    {
        if(firstTime == true)
        {
           firstTime = false;
           line = str.right(str.length() - index -1);
           subResult = str.left(index);
        }
        else
        {
           line = str.right(str.length() - index - 1);
           subResult = str.left(index);
        }

        res = updateResult(subResult, result, nofTocken, MAX_NOF_TOCKENS);
        if(res == false)
        {
            return res;
        }

        str = line;
        index = str.indexOf(PARSE_TOCKEN);
    }

    // Get last tocken. Last data is not ended with PARSE_TOCKEN
    if(line.length() > 0)
    {
       subResult = line;
       res = updateResult(subResult, result, nofTocken, MAX_NOF_TOCKENS);

       if(res == false)
       {
           return res;
       }

       qDebug() << "result =" << subResult << "\n";
    }

    return res;
}


/****************************************************************
 *
 * Function:    ifNumberConvertCommaToDot()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
void CParseAvanzaData::ifnumberConvertCommaToDot(QString &data)
{
    if(data.at(0).isDigit() == true)
    {
        data.replace(",", ".");
    }

}



/****************************************************************
 *
 * Function:    validNofParsedTockens()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
bool CParseAvanzaData::validNofParsedTockens(const int i, const int maxNofLineTockens)
{
    bool res;

    if(i < maxNofLineTockens - 1)
    {
        res = true;
    }
    else
    {
        QMessageBox::critical(NULL, QObject::tr("Fail to parse line"), QObject::tr("Error parse line: Invalid number of tockens"));
        res = false;
    }
    return res;
}



/****************************************************************
 *
 * Function:    updateResult()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
bool CParseAvanzaData::updateResult(QString subresult, parserData_ST &result, int &i, const int maxNofLineTockens)
{
    bool res;

    res = validNofParsedTockens(i, maxNofLineTockens);

    if(res == true)
    {
        ifnumberConvertCommaToDot(subresult);
        switch(i)
        {
        case 0:
            result.date = subresult;
            qDebug() << "result.date =" << result.date << "\n";
            break;
        case 1:
            result.account = subresult;
            qDebug() << "result.account =" << result.account << "\n";
            break;
        case 2:
            result.typeOfData = subresult;
            qDebug() << "result.typeOfData =" << result.typeOfData << "\n";
            break;
        case 3:
            result.assetName = subresult;
            qDebug() << "result.assetName =" << result.assetName << "\n";
            break;
        case 4:
            result.nofItems = subresult;
            qDebug() << "result.nofItems =" << result.nofItems << "\n";
            break;
        case 5:
            result.assetPrice = subresult;
            qDebug() << "result.assetPrice =" << result.assetPrice << "\n";
            break;
        case 6:
            result.totalPrice = subresult;
            qDebug() << "result.totalPrice =" << result.totalPrice << "\n";
            break;
        case 7:
            result.currency = subresult;
            qDebug() << "result.currency =" << result.currency << "\n";
            break;
        default:
            QMessageBox::critical(NULL, QObject::tr("Line Parser Error"), QObject::tr("Line Parser Error: Unknown tocken"));
            res = false;
            break;
        }
        if(res == true)
        {
            qDebug() << "result =" << subresult << "\n";
        }
        i++;
    }

    return res;
}
