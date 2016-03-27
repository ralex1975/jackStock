/*******************************************************************
 *
 * Filename: parseTaData.cpp
 * Date:     2013-02-03
 *
 * Description: This function parse yahoo stock data
 *
 *
 *******************************************************************/

#include "parseTaData.h"
#include <iostream>
#include "dbHndl.h"
#include "common.h"

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
CParseTaData::CParseTaData()
{
}




/****************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *  SELECT id,date,grp FROM table group WHERE DATE=(SELECT max(date) FROM table group) GROUP BY GRP
 *
 *
 ****************************************************************/
bool CParseTaData::parseData(QString filename, QString assetName, QString assetSymbol, bool m_disableMsgBoxes)
{
    bool firstTime = true;
    bool res;

    QString str;
    int assetNameId;
    CDbHndl db;

    // just open database once when processing this data
    bool dbIsHandledExternly = true;
    db.openDb(PATH_JACK_STOCK_DB);

    if(m_disableMsgBoxes == true)
    {
       db.m_disableMsgBoxes = true;
    }



    if(filename.length() < 1)
    {
        db.closeDb(true);
        return false;
    }

    QFile file(filename);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QString errStr = QString("Fail to open file: %1").arg(filename);
        QMessageBox::critical(NULL, QString::fromUtf8("Fail to open file"), errStr);
        db.closeDb(true);
        return false;
    }


    res = db.findYahooTaStockSymbol(assetSymbol, assetNameId, dbIsHandledExternly);

    if(res == false)
    {
        if(false == db.insertYahooStockName(assetName, assetSymbol, assetNameId, dbIsHandledExternly))
        {
            db.closeDb(true);
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error"), QString::fromUtf8("Fail to insert assetName in TA list"));
            return false;
        }
    }


    QTextStream inStream(&file);
    inStream.setCodec("ISO 8859-1");

     while(!inStream.atEnd())
     {
         str = inStream.readLine();
         if(firstTime == true)
         {
            // Remove headline
            firstTime = false;
         }
         else
         {
                 QStringList myStringList = str.split(",");

                 qDebug() << myStringList.length();

                 if(myStringList.length() != 7)
                 {
                    db.closeDb(true);
                    QMessageBox::critical(NULL, QString::fromUtf8("Database Error"), QString::fromUtf8("Fail to insert data in TA list"));
                    return false;
                 }

                  if(false == db.findYahooTaData(myStringList.at(0), assetNameId, dbIsHandledExternly))
                  {

                     if(false == db.insertYahooStockData(myStringList.at(0),  // date
                                                         myStringList.at(1),  // open,
                                                         myStringList.at(2),  // high,
                                                         myStringList.at(3),  // low,
                                                         myStringList.at(4),  // close,
                                                         myStringList.at(5),  // volume,
                                                         myStringList.at(6),  // adjClose,
                                                         assetNameId,
                                                         dbIsHandledExternly))
                     {
                        db.closeDb(true);
                        QMessageBox::critical(NULL, QString::fromUtf8("Database Error"), QString::fromUtf8("Fail to insert data in TA list"));
                        return false;
                     }

                  }
//                 for(int index =0;index < myStringList.length();index++)
//                 {
//                    std::cout << myStringList.at(index).toStdString() << std::endl;
//                 }
         }
     }

     db.closeDb(true);
    // QMessageBox::information(NULL, QString::fromUtf8("Read file"), QString::fromUtf8("Finish reading file"));
     file.close();
     return true;
}


#if 0
/****************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *  SELECT id,date,grp FROM table group WHERE DATE=(SELECT max(date) FROM table group) GROUP BY GRP
 *
 *
 ****************************************************************/
bool CParseTaData::parseData(QString filename, QString assetName, QString assetSymbol, bool m_disableMsgBoxes)
{
    bool firstTime = true;
    bool res;

    QString str;
    int assetNameId;
    CDbHndl db;

    // just open database once when processing this data
    bool dbIsHandledExternly = true;
    db.openDb(PATH_JACK_STOCK_DB);

    if(m_disableMsgBoxes == true)
    {
       db.m_disableMsgBoxes = true;
    }



    if(filename.length() < 1)
    {
        db.closeDb(true);
        return false;
    }

    QFile file(filename);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QString errStr = QString("Fail to open file: %1").arg(filename);
        QMessageBox::critical(NULL, QString::fromUtf8("Fail to open file"), errStr);
        db.closeDb(true);
        return false;
    }


    res = db.findYahooTaStockSymbol(assetSymbol, assetNameId, dbIsHandledExternly);

    if(res == false)
    {
        if(false == db.insertYahooStockName(assetName, assetSymbol, assetNameId, dbIsHandledExternly))
        {
            db.closeDb(true);
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error"), QString::fromUtf8("Fail to insert assetName in TA list"));
            return false;
        }
    }


    QTextStream inStream(&file);
    inStream.setCodec("ISO 8859-1");

     while(!inStream.atEnd())
     {
         str = inStream.readLine();
         if(firstTime == true)
         {
            // Remove headline
            firstTime = false;
         }
         else
         {
                 QStringList myStringList = str.split(",");

                 qDebug() << myStringList.length();

                 if(myStringList.length() != 7)
                 {
                    db.closeDb(true);
                    QMessageBox::critical(NULL, QString::fromUtf8("Database Error"), QString::fromUtf8("Fail to insert data in TA list"));
                    return false;
                 }

                  if(false == db.findYahooTaData(myStringList.at(0), assetNameId, dbIsHandledExternly))
                  {

                     if(false == db.insertYahooStockData(myStringList.at(0),  // date
                                                         myStringList.at(1),  // open,
                                                         myStringList.at(2),  // high,
                                                         myStringList.at(3),  // low,
                                                         myStringList.at(4),  // close,
                                                         myStringList.at(5),  // volume,
                                                         myStringList.at(6),  // adjClose,
                                                         assetNameId,
                                                         dbIsHandledExternly))
                     {
                        db.closeDb(true);
                        QMessageBox::critical(NULL, QString::fromUtf8("Database Error"), QString::fromUtf8("Fail to insert data in TA list"));
                        return false;
                     }

                  }
//                 for(int index =0;index < myStringList.length();index++)
//                 {
//                    std::cout << myStringList.at(index).toStdString() << std::endl;
//                 }
         }
     }

     db.closeDb(true);
    // QMessageBox::information(NULL, QString::fromUtf8("Read file"), QString::fromUtf8("Finish reading file"));
     file.close();
     return true;
}

#endif
