#include "dbHndl.h"
#include <QMessageBox>
#include <stdlib.h>



/****************************************************************
 *
 * Function:    subAnalysisEarningsDateExists()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::subAnalysisEarningsDateExists(QString date,
                                     int mainAnalysisId,
                                     int &earningsDateId)
{
    QSqlRecord rec;
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);



    QByteArray ba = date.toLocal8Bit();
    const char *c_date = ba.data();



    str.sprintf("SELECT * "
                " FROM  TblDateEarningsSubAnalysis "
                " WHERE DateEarnings = '%s' AND MainAnalysisId = %d;",
                c_date,
                mainAnalysisId);

    qDebug() << str;


    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblDateEarningsSubAnalysis error 1"), qry.lastError().text().toUtf8().constData());
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



            if(rec.value("DateEarnings").isNull() == true || true == rec.value("MainAnalysisId").isNull())
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                earningsDateId = rec.value("DateEarningsId").toInt();
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
 * Function:    insertSubAnalysisEarningsDate()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisEarningsDate(QString date,
                       int mainAnalysisId,
                       int &dateEarningsId,
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


    str.sprintf("INSERT OR REPLACE INTO TblDateEarningsSubAnalysis "
                "(DateEarnings, MainAnalysisId) "
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
            QMessageBox::critical(NULL, QString::fromUtf8("TblDateEarningsSubAnalysis"), qry.lastError().text().toUtf8().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    dateEarningsId = (int) qry.lastInsertId().toInt();

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
 * Function:		getSubAnalysisEarningsId()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisEarningsId(int mainAnalysisId,
                  int earningsDateId,
                  int &earningsDataId,
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


    str.sprintf("SELECT TblDataEarningsSubAnalysis.DataEarningsId, TblDataEarningsSubAnalysis.DateEarningsId, TblDataEarningsSubAnalysis.MainAnalysisId   "
                " FROM TblDataEarningsSubAnalysis   "
                " WHERE  "
                "       TblDataEarningsSubAnalysis.DateEarningsId = %d AND "
                "       TblDataEarningsSubAnalysis.MainAnalysisId = %d;",
                                                                           earningsDateId,
                                                                           mainAnalysisId);


    qDebug() << str << "\n";

    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toUtf8().constData());
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




            if(rec.value("DataEarningsId").isNull() == true)
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
                qDebug() << rec.value("DateDividendId").toString();
                qDebug() << rec.value("MainAnalysisId").toString();


                if(rec.value("DataEarningsId").isNull() == false)
                {
                    earningsDataId = rec.value("DataEarningsId").toInt();
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
 * Function:    insertSubEarnings()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisEarnings(int earningsDateId,
                          int mainAnalysisId,
                          int inputEarningsDataId,
                          bool earningsDataIdIsValid,
                          QString dataEarnings,
                          int &earningsDataId,
                          bool dbIsHandledExternly)
{
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }


    QSqlQuery qry(m_db);


    if(earningsDataIdIsValid == true)
    {
        str.sprintf("INSERT OR REPLACE INTO TblDataEarningsSubAnalysis "
                " (DateEarningsId, "
                "  DataEarnings, "
                 " DataEarningsId,"
                 " MainAnalysisId) "
                 " VALUES( %d,  '%s', %d, %d);",
                        earningsDateId,
                        dataEarnings.toLocal8Bit().constData(),
                        inputEarningsDataId,
                        mainAnalysisId);
    }
    // New data
    else
    {
        str.sprintf("INSERT INTO TblDataEarningsSubAnalysis "
                    " (DateEarningsId, "
                    " DataEarnings, "
                    " MainAnalysisId) "
                    " VALUES( %d, '%s', %d);",
                            earningsDateId,
                            dataEarnings.toLocal8Bit().constData(),
                            mainAnalysisId);
    }


    qDebug() << str;

    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblDataEarningsSubAnalysis"), qry.lastError().text().toLatin1().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    earningsDataId = (int) qry.lastInsertId().toInt();


    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }

    return true;
}



#if 0

// 2
//Date
        //-----------------------------------------------------------------------
        // TblDateEarningsSubAnalysis (Vinst)
        //-----------------------------------------------------------------------
        tmp.sprintf("CREATE TABLE IF NOT EXISTS TblDateEarningsSubAnalysis "
                    " (DateEarningsId INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "  DateEarnings DATE, "
                    "  MainAnalysisId INTEGER);");



//Data
        //-----------------------------------------------------------------------
        // TblDataEarningsSubAnalysis (Vinst)
        //-----------------------------------------------------------------------
        tmp.sprintf("CREATE TABLE IF NOT EXISTS TblDataEarningsSubAnalysis "
                    " (DataEarningsId INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "  DateEarningsId INTEGER, "
                    "  DataEarnings VARCHAR(255), "
                    "  MainAnalysisId INTEGER);");


#endif




/*****************************************************************
 *
 * Function:		getSubAnalysisEarningsData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisEarningsData(QString stockName,
                           QString stockSymbol,
                           EarningsDataST *earningsDataArr,
                           int &nofEarningsArrData,
                           bool dbIsHandledExternly)
{


    QSqlRecord rec;
    QString str;
    EarningsDataST data;

    nofEarningsArrData = 0;


    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

  bool found = false;


    str.sprintf("SELECT TblMainAnalysis.*, TblDateEarningsSubAnalysis.*, TblDataEarningsSubAnalysis.* "
                " FROM TblMainAnalysis, TblDateEarningsSubAnalysis, TblDataEarningsSubAnalysis  "
                " WHERE  "
                "       TblMainAnalysis.MainAnalysisId = TblDateEarningsSubAnalysis.MainAnalysisId AND "
                "       TblMainAnalysis.MainAnalysisId = TblDataEarningsSubAnalysis.MainAnalysisId AND "
                "       TblDateEarningsSubAnalysis.DateEarningsId = TblDataEarningsSubAnalysis.DateEarningsId AND "
                "       lower(TblMainAnalysis.stockName) = lower('%s') AND "
                "       lower(TblMainAnalysis.StockSymbol) = lower('%s') "
                " ORDER BY (CAST(TblDateEarningsSubAnalysis.DateEarnings AS REAL)) ASC;",              // DESC";",
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

            if((rec.value("DateEarnings").isNull() == true) ||  // Date
                (rec.value("DataEarnings").isNull() == true))   // Data
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
                qDebug() << rec.value("DateEarnings").toString() << "\n";
                qDebug() << rec.value("stockSymbol").toString();
                qDebug() << rec.value("stockName").toString();




                // Date
                data.date.clear();
                if(rec.value("DateEarnings").isNull() == false)
                {
                    data.date = rec.value("DateEarnings").toString();
                }

                // Data
                data.earnings.clear();
                if(rec.value("DataEarnings").isNull() == false)
                {
                    data.earnings = rec.value("DataEarnings").toString();
                }

                earningsDataArr[nofEarningsArrData] = data;
                nofEarningsArrData++;

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





#if 0










#endif



//========================
//========================



/****************************************************************
 *
 * Function:    subAnalysisDividendDateExists()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::subAnalysisDividendDateExists(QString date,
                                     int mainAnalysisId,
                                     int &dateDividendId)
{
    QSqlRecord rec;
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);



    QByteArray ba = date.toLocal8Bit();
    const char *c_date = ba.data();



    str.sprintf("SELECT * "
                " FROM  TblDateDividendSubAnalysis "
                " WHERE DateDividend = '%s' AND MainAnalysisId = %d;",
                c_date,
                mainAnalysisId);

    qDebug() << str;


    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblDateDividendSubAnalysis error 1"), qry.lastError().text().toUtf8().constData());
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



            if(rec.value("DateDividend").isNull() == true || true == rec.value("MainAnalysisId").isNull())
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                dateDividendId = rec.value("DateDividendId").toInt();
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
 * Function:    insertSubAnalysisDividendDate()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisDividendDate(QString date,
                       int mainAnalysisId,
                       int &dateDividendId,
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


    str.sprintf("INSERT OR REPLACE INTO TblDateDividendSubAnalysis "
                "(DateDividend, MainAnalysisId) "
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
            QMessageBox::critical(NULL, QString::fromUtf8("TblDateDividendSubAnalysis"), qry.lastError().text().toUtf8().constData());
        }
        if(dbIsHandledExternly==false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    dateDividendId = (int) qry.lastInsertId().toInt();

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
getSubAnalysisDividendId(int mainAnalysisId,
                  int dividendDateId,
                  int &dividendDataId,
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


    str.sprintf("SELECT TblDataDividendSubAnalysis.DataDividendId, TblDataDividendSubAnalysis.DateDividendId, TblDataDividendSubAnalysis.MainAnalysisId   "
                " FROM TblDataDividendSubAnalysis   "
                " WHERE  "
                "       TblDataDividendSubAnalysis.DateDividendId = %d AND "
                "       TblDataDividendSubAnalysis.MainAnalysisId = %d;",
                                                                           dividendDateId,
                                                                           mainAnalysisId);


    qDebug() << str << "\n";

    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toUtf8().constData());
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




            if(rec.value("DateDividendId").isNull() == true)
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
                qDebug() << rec.value("DateDividendId").toString();
                qDebug() << rec.value("MainAnalysisId").toString();


                if(rec.value("DataDividendId").isNull() == false)
                {
                    dividendDataId = rec.value("DataDividendId").toInt();
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
insertSubAnalysisDividend(int dividendDateId,
                          int mainAnalysisId,
                          int inputDividendDataId,
                          bool dividendDataIdIsValid,
                          QString dataDividend,
                          int &dividendDataId,
                          bool dbIsHandledExternly)
{
    QString str;

    if(dbIsHandledExternly==false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }


    QSqlQuery qry(m_db);


    if(dividendDataIdIsValid == true)
    {
        str.sprintf("INSERT OR REPLACE INTO TblDataDividendSubAnalysis "
                " (DateDividendId, "
                "  DataDividend, "
                 " DataDividendId,"
                 " MainAnalysisId) "
                 " VALUES( %d,  '%s', %d, %d);",
                        dividendDateId,
                        dataDividend.toLocal8Bit().constData(),
                        inputDividendDataId,
                        mainAnalysisId);
    }
    // New data
    else
    {
        str.sprintf("INSERT INTO TblDataDividendSubAnalysis "
                    " (DateDividendId, "
                    " DataDividend, "
                    " MainAnalysisId) "
                    " VALUES( %d, '%s', %d);",
                            dividendDateId,
                            dataDividend.toLocal8Bit().constData(),
                            mainAnalysisId);
    }


    qDebug() << str;

    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblDataDividendSubAnalysis"), qry.lastError().text().toLatin1().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    dividendDataId = (int) qry.lastInsertId().toInt();


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
 * Function:		getSubAnalysisDividendData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisDividendData(QString stockName,
                           QString stockSymbol,
                           DividendDataST *dividendDataArr,
                           int &nofDividendArrData,
                           bool dbIsHandledExternly)
{


    QSqlRecord rec;
    QString str;
    DividendDataST data;

    nofDividendArrData = 0;


    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

  bool found = false;


    str.sprintf("SELECT TblMainAnalysis.*, TblDateDividendSubAnalysis.*, TblDataDividendSubAnalysis.* "
                " FROM TblMainAnalysis, TblDateDividendSubAnalysis, TblDataDividendSubAnalysis  "
                " WHERE  "
                "       TblMainAnalysis.MainAnalysisId = TblDateDividendSubAnalysis.MainAnalysisId AND "
                "       TblMainAnalysis.MainAnalysisId = TblDataDividendSubAnalysis.MainAnalysisId AND "
                "       TblDateDividendSubAnalysis.DateDividendId = TblDataDividendSubAnalysis.DateDividendId AND "
                "       lower(TblMainAnalysis.stockName) = lower('%s') AND "
                "       lower(TblMainAnalysis.StockSymbol) = lower('%s') "
                " ORDER BY (CAST(TblDateDividendSubAnalysis.DateDividend AS REAL)) ASC;",              // DESC";",
                //" ORDER BY (TblDateDividendSubAnalysis.DateDividend) ASC;",              // DESC";",
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




            if((rec.value("DateDividend").isNull() == true) || (rec.value("DataDividend").isNull() == true))
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
                qDebug() << rec.value("DateDividend").toString() << "\n";
                qDebug() << rec.value("stockSymbol").toString();
                qDebug() << rec.value("stockName").toString();




                // Date
                data.date.clear();
                if(rec.value("DateDividend").isNull() == false)
                {
                    data.date = rec.value("DateDividend").toString();
                }

                // Data
                data.dividend.clear();
                if(rec.value("DataDividend").isNull() == false)
                {
                    data.dividend = rec.value("DataDividend").toString();
                }

                dividendDataArr[nofDividendArrData] = data;
                nofDividendArrData++;

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









//===================

#if 0


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
        // TblDateCoreCapitalRatioSubAnalysis (kärnprimärkapitalrelation används ej)
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

#endif

//===================

#if 0










#endif
