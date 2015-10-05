#include "parsekeytabridgedata.h"
#include <QMessageBox>
#include "dbHndl.h"



ParseKeyTaBridgeData::ParseKeyTaBridgeData()
{
}




/****************************************************************
 *
 * Function:    parseStockPrices()
 *
 * Description: This function Insert snapshot data name and
 *              Yahoo symbol in bridge table
 *
 *
 *
 ****************************************************************/
bool ParseKeyTaBridgeData::readFile(QString filename)
{
    CDbHndl db;
    QFile file(filename);
    QString errStr = QString("Fail to open file: %1").arg(filename);
    QString result;
    QString stockName;
    QString stockSymbol;
    QStringList resultList;


    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::critical(NULL, QObject::tr("Fail to open file"), errStr);
        return false;
    }

    db.delAllTblNordnetYahooBridge();

    QTextStream inStream(&file);
    inStream.setCodec("ISO 8859-1");
    //inStream.setCodec("UTF-8");


    QRegExp rx("(\\|)"); //RegEx for '\n' or ','

    while(!inStream.atEnd())
    {
        result = inStream.readLine();
        resultList = result.split(rx);
        if(true != resultList.isEmpty() || resultList.size() != 2)
        {
            qDebug() << resultList.size();
            stockName = resultList.at(0);
            stockSymbol = resultList.at(1);
            db.insertKeyTaBridgeData(stockName, stockSymbol);
        }
        else
        {
            errStr.sprintf("Line=%s, Name=%s, symbol=%s\n", result.toAscii().constData(), stockName.toAscii().constData(), stockSymbol.toAscii().constData());
            QMessageBox::critical(NULL, QObject::tr("Fail to parse keyTaBridgeData"), errStr);
        }
    }

    file.close();
    return true;
}

