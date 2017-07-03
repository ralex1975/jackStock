#include "parseomxbridgedata.h"

#include <QStringList>
#include <QMessageBox>

/****************************************************************
 *
 * Function:    ParseOmxBridgeData()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
ParseOmxBridgeData::ParseOmxBridgeData()
{

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
bool ParseOmxBridgeData::readFile(QString filename,
                                  struct OmxBridgeDataST *omxBridgeDataArr,
                                  int &nofReadData,
                                  int maxSize)
{
    QFile file(filename);
    QString errStr = QString("Fail to open file: %1").arg(filename);
    QString result;
    QStringList resultList;
    nofReadData = 0;


    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::critical(NULL, QString::fromUtf8("Fail to open file"), errStr);
        return false;
    }

    QTextStream inStream(&file);
    //inStream.setCodec("ISO 8859-1");
    inStream.setCodec("UTF-8");


    QRegExp rx("(\\|)"); //RegEx for '|'

    while(!inStream.atEnd())
    {
        if(nofReadData >= maxSize)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Fail to parse OmxTaBridgeData"), QString::fromUtf8("omxBridgeData array to small"));
            return false;
        }


        result = inStream.readLine();
        resultList = result.split(rx);
        if(true != resultList.isEmpty() || resultList.size() != 2)
        {
            if((resultList.at(0).length() >  0) &&
               (resultList.at(1) > 0) &&
               (resultList.at(3).length() > 0))
            {
                omxBridgeDataArr[nofReadData].stockSymbol = resultList.at(0);
                omxBridgeDataArr[nofReadData].yahooStockName = resultList.at(1);
                omxBridgeDataArr[nofReadData].omxStockName = resultList.at(2);
                omxBridgeDataArr[nofReadData].omxLink = resultList.at(3);
                qDebug() << omxBridgeDataArr[nofReadData].stockSymbol;
                nofReadData++;
            }

            //qDebug() << resultList.size();
        }
        else
        {
            // errStr.sprintf("Line=%s, Name=%s, symbol=%s\n", result.toAscii().constData(), stockName.toAscii().constData(), stockSymbol.toAscii().constData());
            QMessageBox::critical(NULL, QString::fromUtf8("Fail to parse OmxTaBridgeData"), QString::fromUtf8("Fail to parse OmxTaBridgeData"));
        }

    }

    file.close();
    return true;
}

