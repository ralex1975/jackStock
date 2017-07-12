#include "dbHndl.h"
#include <QMessageBox>
#include <stdlib.h>





/*****************************************************************
 *
 * Function:		getTaCrumbCookie()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getTaCrumbCookie(QString stockName,
                 QString stockSymbol,
                 QString &crumb,
                 QString &cookie,
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


    str.sprintf("SELECT TblMainAnalysis.*, TblTaCrumbCookie.* "
                " FROM TblMainAnalysis, TblTaCrumbCookie  "
                " WHERE  "
                "       TblMainAnalysis.MainAnalysisId = TblTaCrumbCookie.MainAnalysisId AND "
                "       lower(TblMainAnalysis.stockName) = lower('%s') AND "
                "       lower(TblMainAnalysis.StockSymbol) = lower('%s'); ",
                stockName.toLocal8Bit().constData(),
                stockSymbol.toLocal8Bit().constData());


    qDebug() << str << "\n";

    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"),
                                        qry.lastError().text().toLatin1().constData());
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
        // Should only be one record so this is unnecessary
        if(qry.next())
        {
            rec = qry.record();

            if( (rec.value("Crumb").isNull() == true)||
                (rec.value("Cookie").isNull() == true))
            {
                qry.finish();
                if(dbIsHandledExternly == false)
                {
                    closeDb();
                    m_mutex.unlock();
                }

                return false;
            }
            else
            {
                found = true;
                qDebug() << rec.value("Crumb").toString();
                qDebug() << rec.value("Cookie").toString();

                crumb = rec.value("Crumb").toString();
                cookie = rec.value("Cookie").toString();
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
 * Function:    insertTaCrumbCookieData()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertTaCrumbCookieData(QString stockName,
                        QString stockSymbol,
                        QString &crumb,
                        QString &cookie,
                        bool dbIsHandledExternly)
{
    QString str;
    int mainAnalysisId = 0;

    if(false == mainAnalysisDataExists(stockName, stockSymbol, mainAnalysisId))
    {
        return false;
    }


    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);


    str.sprintf("INSERT OR REPLACE INTO TblTaCrumbCookie "
                "(Crumb, Cookie, MainAnalysisId) "
                " VALUES('%s', '%s', %d);",
                crumb.toLocal8Bit().constData(),
                cookie.toLocal8Bit().constData(),
                mainAnalysisId);

    qDebug() << str;

    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();

        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblTaCrumbCookie"), qry.lastError().text().toUtf8().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    qry.finish();

    if(dbIsHandledExternly==false)
    {
        closeDb();
        m_mutex.unlock();
    }

    return true;
}




#if 1
/****************************************************************
 *
 * Function:    subAnalysisEfficientRatioDateExists()
 *
 * Description: Efficient Ratio
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::subAnalysisEfficientRatioDateExists(QString date,
                                     int mainAnalysisId,
                                     int &dateId)
{
    QSqlRecord rec;
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);



    QByteArray ba = date.toLocal8Bit();
    const char *c_date = ba.data();



    str.sprintf("SELECT * "
                " FROM  TblDateEfficientRatio "
                " WHERE Date = '%s' AND MainAnalysisId = %d;",
                c_date,
                mainAnalysisId);

    qDebug() << str;


    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Error:"), qry.lastError().text().toUtf8().constData());
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



            if((rec.value("Date").isNull() == true) || (true == rec.value("MainAnalysisId").isNull()))
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                dateId = rec.value("DateId").toInt();
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
 * Function:    insertSubAnalysisEfficientRatioDate()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisEfficientRatioDate(QString date,
                                    int mainAnalysisId,
                                    int &dateId,
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


    str.sprintf("INSERT OR REPLACE INTO TblDateEfficientRatio "
                "(Date, MainAnalysisId) "
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
            QMessageBox::critical(NULL, QString::fromUtf8("TblDateEfficientRatio"), qry.lastError().text().toUtf8().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    dateId = (int) qry.lastInsertId().toInt();

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
 * Function:		getSubAnalysisEfficientRatioDataId()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisEfficientRatioDataId(int mainAnalysisId,
                                   int dateId,
                                   int &dataId,
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


    str.sprintf("SELECT TblDataEfficientRatio.DataId, TblDataEfficientRatio.DateId, TblDataEfficientRatio.MainAnalysisId   "
                " FROM TblDataEfficientRatio   "
                " WHERE  "
                "       TblDataEfficientRatio.DateId = %d AND "
                "       TblDataEfficientRatio.MainAnalysisId = %d;", dateId
                                                                   , mainAnalysisId);


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

            if(rec.value("DataId").isNull() == true)
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
                qDebug() << rec.value("DataId").toString();
                qDebug() << rec.value("MainAnalysisId").toString();


                if(rec.value("DataId").isNull() == false)
                {
                    dataId = rec.value("DataId").toInt();
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
 * Function:    insertSubAnalysisEfficientRatioData()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisEfficientRatioData(int dateId,
                                    int mainAnalysisId,
                                    int inputDataId,
                                    bool idIsValid,
                                    QString efficientRatio,
                                    int &dataId,
                                    bool dbIsHandledExternly)
{
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }


    QSqlQuery qry(m_db);


    if(idIsValid == true)
    {
        str.sprintf("INSERT OR REPLACE INTO TblDataEfficientRatio "
                    " (DateId, "
                    "  EfficientRatioValue, "
                    " DataId,"
                    " MainAnalysisId) "
                    " VALUES( %d, '%s', %d, %d);",
                            dateId,
                            efficientRatio.toLocal8Bit().constData(),
                            inputDataId,
                            mainAnalysisId);
    }
    // New data
    else
    {
        str.sprintf("INSERT INTO TblDataEfficientRatio "
                    " (DateId, "
                    "  EfficientRatioValue, "
                    " MainAnalysisId) "
                    " VALUES( %d, '%s', %d);",
                            dateId,
                            efficientRatio.toLocal8Bit().constData(),
                            mainAnalysisId);
    }


    qDebug() << str;

    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblDataEfficientRatio"), qry.lastError().text().toLatin1().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    dataId = (int) qry.lastInsertId().toInt();


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
 * Function:		getSubAnalysisIntrinsicValueData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisEfficientRatioData(QString stockName,
                           QString stockSymbol,
                           SubAnalysDataST *dataArr,
                           int &nofArrData,
                           bool dbIsHandledExternly)
{


    QSqlRecord rec;
    QString str;

    nofArrData = 0;



    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

  bool found = false;


    str.sprintf("SELECT TblMainAnalysis.*, TblDateEfficientRatio.*, TblDataEfficientRatio.* "
                " FROM TblMainAnalysis, TblDateEfficientRatio, TblDataEfficientRatio  "
                " WHERE  "
                "       TblMainAnalysis.MainAnalysisId = TblDateEfficientRatio.MainAnalysisId AND "
                "       TblMainAnalysis.MainAnalysisId = TblDataEfficientRatio.MainAnalysisId AND "
                "       TblDateEfficientRatio.DateId = TblDataEfficientRatio.DateId AND "
                "       lower(TblMainAnalysis.stockName) = lower('%s') AND "
                "       lower(TblMainAnalysis.StockSymbol) = lower('%s') "
                " ORDER BY (CAST(TblDateEfficientRatio.Date AS REAL)) ASC;", stockName.toLocal8Bit().constData()
                                                                           , stockSymbol.toLocal8Bit().constData());


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
        // Should only be one record so this is unnecessary
        while(qry.next())
        {
            rec = qry.record();

            if( (rec.value("EfficientRatioValue").isNull() == true))
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
                qDebug() << rec.value("Date").toString() << "\n";
                qDebug() << rec.value("EfficientRatioValue").toString() << "\n";
                qDebug() << rec.value("stockSymbol").toString();
                qDebug() << rec.value("stockName").toString();


                dataArr[nofArrData].date.clear();
                if(rec.value("Date").isNull() == false)
                {
                    dataArr[nofArrData].date = rec.value("Date").toString();
                }

                dataArr[nofArrData].data.clear();
                if(rec.value("EfficientRatioValue").isNull() == false)
                {
                    dataArr[nofArrData].data = rec.value("EfficientRatioValue").toString();
                }
                nofArrData++;
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
#endif




//=========================================


/****************************************************************
 *
 * Function:    subAnalysisTotDividendsDateExists()
 *
 * Description: Operating Cash Flow
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::subAnalysisTotDividendsDateExists(QString date,
                                     int mainAnalysisId,
                                     int &OperatingCashFlowDateId)
{
    QSqlRecord rec;
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);



    QByteArray ba = date.toLocal8Bit();
    const char *c_date = ba.data();



    str.sprintf("SELECT * "
                " FROM  TblDateTotDividendsSubAnalysis "
                " WHERE Date = '%s' AND MainAnalysisId = %d;",
                c_date,
                mainAnalysisId);

    qDebug() << str;


    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblDateTotDividendsSubAnalysis error 1"), qry.lastError().text().toUtf8().constData());
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



            if(rec.value("Date").isNull() == true || true == rec.value("MainAnalysisId").isNull())
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                OperatingCashFlowDateId = rec.value("DateId").toInt();
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
 * Function:    insertSubAnalysisTotDividendsDate()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisTotDividendsDate(QString date,
                       int mainAnalysisId,
                       int &dateId,
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


    str.sprintf("INSERT OR REPLACE INTO TblDateTotDividendsSubAnalysis "
                "(Date, MainAnalysisId) "
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
            QMessageBox::critical(NULL, QString::fromUtf8("TblDateTotDividendsSubAnalysis"), qry.lastError().text().toUtf8().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    dateId = (int) qry.lastInsertId().toInt();

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
 * Function:		getSubAnalysisTotDividendsId()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisTotDividendsDataId(int mainAnalysisId,
                  int dateId,
                  int &dataId,
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


    str.sprintf("SELECT TblDataTotDividendsSubAnalysis.DataId, TblDataTotDividendsSubAnalysis.DateId, TblDataTotDividendsSubAnalysis.MainAnalysisId   "
                " FROM TblDataTotDividendsSubAnalysis   "
                " WHERE  "
                "       TblDataTotDividendsSubAnalysis.DateId = %d AND "
                "       TblDataTotDividendsSubAnalysis.MainAnalysisId = %d;",
                                                                           dateId,
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




            if(rec.value("DataId").isNull() == true)
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
                qDebug() << rec.value("DataId").toString();
                qDebug() << rec.value("MainAnalysisId").toString();


                if(rec.value("DataId").isNull() == false)
                {
                    dataId = rec.value("DataId").toInt();
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
 * Function:    insertSubAnalysisTotDividendsData()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisTotDividendsData(int dateId,
                          int mainAnalysisId,
                          int inputDataId,
                          bool idIsValid,
                          QString data,
                          int &dataId,
                          bool dbIsHandledExternly)
{
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }


    QSqlQuery qry(m_db);


    if(idIsValid == true)
    {
        str.sprintf("INSERT OR REPLACE INTO TblDataTotDividendsSubAnalysis "
                " (DateId, "
                "  Data, "
                 " DataId,"
                 " MainAnalysisId) "
                 " VALUES( %d,  '%s', %d, %d);",
                        dateId,
                        data.toLocal8Bit().constData(),
                        inputDataId,
                        mainAnalysisId);
    }
    // New data
    else
    {
        str.sprintf("INSERT INTO TblDataTotDividendsSubAnalysis "
                    " (DateId, "
                    " Data, "
                    " MainAnalysisId) "
                    " VALUES( %d, '%s', %d);",
                            dateId,
                            data.toLocal8Bit().constData(),
                            mainAnalysisId);
    }


    qDebug() << str;

    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblDataTotDividendsSubAnalysis"), qry.lastError().text().toLatin1().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    dataId = (int) qry.lastInsertId().toInt();


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
 * Function:		getSubAnalysisTotDividendsData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisTotDividendsData(QString stockName,
                           QString stockSymbol,
                           SubAnalysDataST *dataArr,
                           int &nofArrData,
                           bool dbIsHandledExternly)
{


    QSqlRecord rec;
    QString str;
    SubAnalysDataST data;

    nofArrData = 0;


    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

  bool found = false;


    str.sprintf("SELECT TblMainAnalysis.*, TblDateTotDividendsSubAnalysis.*, TblDataTotDividendsSubAnalysis.* "
                " FROM TblMainAnalysis, TblDateTotDividendsSubAnalysis, TblDataTotDividendsSubAnalysis  "
                " WHERE  "
                "       TblMainAnalysis.MainAnalysisId = TblDateTotDividendsSubAnalysis.MainAnalysisId AND "
                "       TblMainAnalysis.MainAnalysisId = TblDataTotDividendsSubAnalysis.MainAnalysisId AND "
                "       TblDateTotDividendsSubAnalysis.DateId = TblDataTotDividendsSubAnalysis.DateId AND "
                "       lower(TblMainAnalysis.stockName) = lower('%s') AND "
                "       lower(TblMainAnalysis.StockSymbol) = lower('%s') "
                " ORDER BY (CAST(TblDateTotDividendsSubAnalysis.Date AS REAL)) ASC;",              // DESC";",
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

            if((rec.value("Date").isNull() == true) ||  // Date
                (rec.value("Data").isNull() == true))   // Data
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
                qDebug() << rec.value("Date").toString() << "\n";
                qDebug() << rec.value("stockSymbol").toString();
                qDebug() << rec.value("stockName").toString();




                // Date
                data.date.clear();
                if(rec.value("Date").isNull() == false)
                {
                    data.date = rec.value("Date").toString();
                }

                // Data
                data.data.clear();
                if(rec.value("Data").isNull() == false)
                {
                    data.data = rec.value("Data").toString();
                }

                dataArr[nofArrData] = data;
                nofArrData++;

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






//=====================================



/****************************************************************
 *
 * Function:    subAnalysisOperatingCashFlowDateExists()
 *
 * Description: Operating Cash Flow
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::subAnalysisOperatingCashFlowDateExists(QString date,
                                     int mainAnalysisId,
                                     int &OperatingCashFlowDateId)
{
    QSqlRecord rec;
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);



    QByteArray ba = date.toLocal8Bit();
    const char *c_date = ba.data();



    str.sprintf("SELECT * "
                " FROM  TblDateOperatingCashFlowSubAnalysis "
                " WHERE DateOperatingCashFlow = '%s' AND MainAnalysisId = %d;",
                c_date,
                mainAnalysisId);

    qDebug() << str;


    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblDateOperatingCashFlowSubAnalysis error 1"), qry.lastError().text().toUtf8().constData());
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



            if(rec.value("DateOperatingCashFlow").isNull() == true || true == rec.value("MainAnalysisId").isNull())
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                OperatingCashFlowDateId = rec.value("DateOperatingCashFlowId").toInt();
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
 * Function:    insertSubAnalysisOperatingCashFlowDate()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisOperatingCashFlowDate(QString date,
                       int mainAnalysisId,
                       int &dateOperatingCashFlowId,
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


    str.sprintf("INSERT OR REPLACE INTO TblDateOperatingCashFlowSubAnalysis "
                "(DateOperatingCashFlow, MainAnalysisId) "
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
            QMessageBox::critical(NULL, QString::fromUtf8("TblDateOperatingCashFlowSubAnalysis"), qry.lastError().text().toUtf8().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    dateOperatingCashFlowId = (int) qry.lastInsertId().toInt();

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
 * Function:		getSubAnalysisOperatingCashFlowId()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisOperatingCashFlowDataId(int mainAnalysisId,
                  int operatingCashFlowDateId,
                  int &operatingCashFlowDataId,
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


    str.sprintf("SELECT TblDataOperatingCashFlowSubAnalysis.DataOperatingCashFlowId, TblDataOperatingCashFlowSubAnalysis.DateOperatingCashFlowId, TblDataOperatingCashFlowSubAnalysis.MainAnalysisId   "
                " FROM TblDataOperatingCashFlowSubAnalysis   "
                " WHERE  "
                "       TblDataOperatingCashFlowSubAnalysis.DateOperatingCashFlowId = %d AND "
                "       TblDataOperatingCashFlowSubAnalysis.MainAnalysisId = %d;",
                                                                           operatingCashFlowDateId,
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




            if(rec.value("DataOperatingCashFlowId").isNull() == true)
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
                qDebug() << rec.value("DataOperatingCashFlowId").toString();
                qDebug() << rec.value("MainAnalysisId").toString();


                if(rec.value("DataOperatingCashFlowId").isNull() == false)
                {
                    operatingCashFlowDataId = rec.value("DataOperatingCashFlowId").toInt();
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
 * Function:    insertSubAnalysisOperatingCashFlow()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisOperatingCashFlowData(int operatingCashFlowDateId,
                          int mainAnalysisId,
                          int inputOperatingCashFlowDataId,
                          bool operatingCashFlowIdIsValid,
                          QString dataOperatingCashFlow,
                          int &operatingCashFlowDataId,
                          bool dbIsHandledExternly)
{
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }


    QSqlQuery qry(m_db);


    if(operatingCashFlowIdIsValid == true)
    {
        str.sprintf("INSERT OR REPLACE INTO TblDataOperatingCashFlowSubAnalysis "
                " (DateOperatingCashFlowId, "
                "  DataOperatingCashFlow, "
                 " DataOperatingCashFlowId,"
                 " MainAnalysisId) "
                 " VALUES( %d,  '%s', %d, %d);",
                        operatingCashFlowDateId,
                        dataOperatingCashFlow.toLocal8Bit().constData(),
                        inputOperatingCashFlowDataId,
                        mainAnalysisId);
    }
    // New data
    else
    {
        str.sprintf("INSERT INTO TblDataOperatingCashFlowSubAnalysis "
                    " (DateOperatingCashFlowId, "
                    " DataOperatingCashFlow, "
                    " MainAnalysisId) "
                    " VALUES( %d, '%s', %d);",
                            operatingCashFlowDateId,
                            dataOperatingCashFlow.toLocal8Bit().constData(),
                            mainAnalysisId);
    }


    qDebug() << str;

    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblDataOperatingCashFlowSubAnalysis"), qry.lastError().text().toLatin1().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    operatingCashFlowDataId = (int) qry.lastInsertId().toInt();


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
 * Function:		getSubAnalysisOperatingCashFlowData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisOperatingCashFlowData(QString stockName,
                           QString stockSymbol,
                           SubAnalysDataST *dataArr,
                           int &nofArrData,
                           bool dbIsHandledExternly)
{


    QSqlRecord rec;
    QString str;
    SubAnalysDataST data;

    nofArrData = 0;


    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

  bool found = false;


    str.sprintf("SELECT TblMainAnalysis.*, TblDateOperatingCashFlowSubAnalysis.*, TblDataOperatingCashFlowSubAnalysis.* "
                " FROM TblMainAnalysis, TblDateOperatingCashFlowSubAnalysis, TblDataOperatingCashFlowSubAnalysis  "
                " WHERE  "
                "       TblMainAnalysis.MainAnalysisId = TblDateOperatingCashFlowSubAnalysis.MainAnalysisId AND "
                "       TblMainAnalysis.MainAnalysisId = TblDataOperatingCashFlowSubAnalysis.MainAnalysisId AND "
                "       TblDateOperatingCashFlowSubAnalysis.DateOperatingCashFlowId = TblDataOperatingCashFlowSubAnalysis.DateOperatingCashFlowId AND "
                "       lower(TblMainAnalysis.stockName) = lower('%s') AND "
                "       lower(TblMainAnalysis.StockSymbol) = lower('%s') "
                " ORDER BY (CAST(TblDateOperatingCashFlowSubAnalysis.DateOperatingCashFlow AS REAL)) ASC;",              // DESC";",
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

            if((rec.value("DateOperatingCashFlow").isNull() == true) ||  // Date
                (rec.value("DataOperatingCashFlow").isNull() == true))   // Data
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
                qDebug() << rec.value("DateOperatingCashFlow").toString() << "\n";
                qDebug() << rec.value("stockSymbol").toString();
                qDebug() << rec.value("stockName").toString();




                // Date
                data.date.clear();
                if(rec.value("DateOperatingCashFlow").isNull() == false)
                {
                    data.date = rec.value("DateOperatingCashFlow").toString();
                }

                // Data
                data.data.clear();
                if(rec.value("DataOperatingCashFlow").isNull() == false)
                {
                    data.data = rec.value("DataOperatingCashFlow").toString();
                }

                dataArr[nofArrData] = data;
                nofArrData++;

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






//=================

/****************************************************************
 *
 * Function:    subAnalysisCashFlowCapexDateExists()
 *
 * Description: Capex (Cash Flow)
 *              Is found in cash flow heading: Investment in property, plant and equipment
 *
 *  (Kassaflöde Capex Investeringar/kapitalutgifter)
 *
 *
 ****************************************************************/
bool CDbHndl::subAnalysisCashFlowCapexDateExists(QString date,
                                     int mainAnalysisId,
                                     int &cashFlowCapexDateId)
{
    QSqlRecord rec;
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);



    QByteArray ba = date.toLocal8Bit();
    const char *c_date = ba.data();



    str.sprintf("SELECT * "
                " FROM  TblDateCashFlowCapexSubAnalysis "
                " WHERE DateCashFlowCapex = '%s' AND MainAnalysisId = %d;",
                c_date,
                mainAnalysisId);

    qDebug() << str;


    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblDateCashFlowCapexSubAnalysis error 1"), qry.lastError().text().toUtf8().constData());
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



            if(rec.value("DateCashFlowCapex").isNull() == true || true == rec.value("MainAnalysisId").isNull())
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                cashFlowCapexDateId = rec.value("DateCashFlowCapexId").toInt();
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
 * Function:    insertSubAnalysisCashFlowCapexDate()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisCashFlowCapexDate(QString date,
                       int mainAnalysisId,
                       int &dateCashFlowCapexId,
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


    str.sprintf("INSERT OR REPLACE INTO TblDateCashFlowCapexSubAnalysis "
                "(DateCashFlowCapex, MainAnalysisId) "
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
            QMessageBox::critical(NULL, QString::fromUtf8("TblDateCashFlowCapexSubAnalysis"), qry.lastError().text().toUtf8().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    dateCashFlowCapexId = (int) qry.lastInsertId().toInt();

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
 * Function:		getSubAnalysisCashFlowCapexId()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisCashFlowCapexDataId(int mainAnalysisId,
                  int cashFlowCapexDateId,
                  int &cashFlowCapexDataId,
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


    str.sprintf("SELECT TblDataCashFlowCapexSubAnalysis.DataCashFlowCapexId, TblDataCashFlowCapexSubAnalysis.DateCashFlowCapexId, TblDataCashFlowCapexSubAnalysis.MainAnalysisId   "
                " FROM TblDataCashFlowCapexSubAnalysis   "
                " WHERE  "
                "       TblDataCashFlowCapexSubAnalysis.DateCashFlowCapexId = %d AND "
                "       TblDataCashFlowCapexSubAnalysis.MainAnalysisId = %d;",
                                                                           cashFlowCapexDateId,
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




            if(rec.value("DataCashFlowCapexId").isNull() == true)
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
                qDebug() << rec.value("DataCashFlowCapexId").toString();
                qDebug() << rec.value("MainAnalysisId").toString();


                if(rec.value("DataCashFlowCapexId").isNull() == false)
                {
                    cashFlowCapexDataId = rec.value("DataCashFlowCapexId").toInt();
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
 * Function:    insertSubAnalysisCashFlowCapex()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisCashFlowCapexData(int cashFlowCapexDateId,
                          int mainAnalysisId,
                          int inputCashFlowCapexDataId,
                          bool cashFlowCapexDataIdIsValid,
                          QString dataCashFlowCapex,
                          int &cashFlowCapexDataId,
                          bool dbIsHandledExternly)
{
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }


    QSqlQuery qry(m_db);


    if(cashFlowCapexDataIdIsValid == true)
    {
        str.sprintf("INSERT OR REPLACE INTO TblDataCashFlowCapexSubAnalysis "
                " (DateCashFlowCapexId, "
                "  DataCashFlowCapex, "
                 " DataCashFlowCapexId,"
                 " MainAnalysisId) "
                 " VALUES( %d,  '%s', %d, %d);",
                        cashFlowCapexDateId,
                        dataCashFlowCapex.toLocal8Bit().constData(),
                        inputCashFlowCapexDataId,
                        mainAnalysisId);
    }
    // New data
    else
    {
        str.sprintf("INSERT INTO TblDataCashFlowCapexSubAnalysis "
                    " (DateCashFlowCapexId, "
                    " DataCashFlowCapex, "
                    " MainAnalysisId) "
                    " VALUES( %d, '%s', %d);",
                            cashFlowCapexDateId,
                            dataCashFlowCapex.toLocal8Bit().constData(),
                            mainAnalysisId);
    }


    qDebug() << str;

    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblDataCashFlowCapexSubAnalysis"), qry.lastError().text().toLatin1().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    cashFlowCapexDataId = (int) qry.lastInsertId().toInt();


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
 * Function:		getSubAnalysisCashFlowCapexData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisCashFlowCapexData(QString stockName,
                           QString stockSymbol,
                           SubAnalysDataST *dataArr,
                           int &nofArrData,
                           bool dbIsHandledExternly)
{


    QSqlRecord rec;
    QString str;
    SubAnalysDataST data;

    nofArrData = 0;


    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

  bool found = false;


    str.sprintf("SELECT TblMainAnalysis.*, TblDateCashFlowCapexSubAnalysis.*, TblDataCashFlowCapexSubAnalysis.* "
                " FROM TblMainAnalysis, TblDateCashFlowCapexSubAnalysis, TblDataCashFlowCapexSubAnalysis  "
                " WHERE  "
                "       TblMainAnalysis.MainAnalysisId = TblDateCashFlowCapexSubAnalysis.MainAnalysisId AND "
                "       TblMainAnalysis.MainAnalysisId = TblDataCashFlowCapexSubAnalysis.MainAnalysisId AND "
                "       TblDateCashFlowCapexSubAnalysis.DateCashFlowCapexId = TblDataCashFlowCapexSubAnalysis.DateCashFlowCapexId AND "
                "       lower(TblMainAnalysis.stockName) = lower('%s') AND "
                "       lower(TblMainAnalysis.StockSymbol) = lower('%s') "
                " ORDER BY (CAST(TblDateCashFlowCapexSubAnalysis.DateCashFlowCapex AS REAL)) ASC;",              // DESC";",
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

            if((rec.value("DateCashFlowCapex").isNull() == true) ||  // Date
                (rec.value("DataCashFlowCapex").isNull() == true))   // Data
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
                qDebug() << rec.value("DateCashFlowCapex").toString() << "\n";
                qDebug() << rec.value("stockSymbol").toString();
                qDebug() << rec.value("stockName").toString();




                // Date
                data.date.clear();
                if(rec.value("DateCashFlowCapex").isNull() == false)
                {
                    data.date = rec.value("DateCashFlowCapex").toString();
                }

                // Data
                data.data.clear();
                if(rec.value("DataCashFlowCapex").isNull() == false)
                {
                    data.data = rec.value("DataCashFlowCapex").toString();
                }

                dataArr[nofArrData] = data;
                nofArrData++;

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







//=============================0

/****************************************************************
 *
 * Function:    subAnalysisCompanyTypeExists()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::subAnalysisCompanyTypeExists(int companyType,
                                     int mainAnalysisId,
                                     int &companyTypeId)
{
    QSqlRecord rec;
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    str.sprintf("SELECT * "
                " FROM  TblSubAnalysisCompanyType "
                " WHERE CompanyType = %d AND MainAnalysisId = %d;",
                companyType,
                mainAnalysisId);

    qDebug() << str;


    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblSubAnalysisCompanyType error 1"), qry.lastError().text().toUtf8().constData());
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



            if(rec.value("CompanyTypeId").isNull() == true || true == rec.value("MainAnalysisId").isNull())
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                companyTypeId = rec.value("CompanyTypeId").toInt();
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
 * Function:    insertSubAnalysisCompanyType()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisCompanyType(int companyType,
                            int mainAnalysisId,
                            int &companyTypeId,
                            bool dbIsHandledExternly)
{
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);



    str.sprintf("INSERT OR REPLACE INTO TblSubAnalysisCompanyType "
                "(CompanyType, MainAnalysisId) "
                " VALUES(%d, %d);",
                companyType,
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


    companyTypeId = (int) qry.lastInsertId().toInt();

    qry.finish();

    if(dbIsHandledExternly==false)
    {
        closeDb();
        m_mutex.unlock();
    }

    return true;
}






/****************************************************************
 *
 * Function:    deleteCompanyType()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
bool CDbHndl::deleteCompanyType(int mainAnalysisId)
{

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);
    QString str;

    str.sprintf("DELETE FROM TblSubAnalysisCompanyType WHERE MainAnalysisId = %d",
                                                             mainAnalysisId);

    qry.prepare(str);


    if( !qry.exec() )
    {
        qry.finish();
        closeDb();
        m_mutex.unlock();

        qDebug() << qry.lastError();
        return false;
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();

    return true;
}







/*****************************************************************
 *
 * Function:		getSubAnalysisCompanyType()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisCompanyType(int mainAnalysisId,
                          int &companyType,
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


    str.sprintf("SELECT *   "
                " FROM TblSubAnalysisCompanyType   "
                " WHERE  "
                "       TblSubAnalysisCompanyType.MainAnalysisId = %d;",
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


            if(rec.value("CompanyType").isNull() == true)
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


                if(rec.value("CompanyType").isNull() == false)
                {
                    companyType = rec.value("CompanyType").toInt();
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

//=========================


/****************************************************************
 *
 * Function:    subAnalysisEarningsPerShareDateExists()
 *
 * Description:
 *
 * EarningsPerShare
 *
 *
 *
 ****************************************************************/
bool CDbHndl::subAnalysisEarningsPerShareDateExists(QString date,
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
 * Function:    insertSubAnalysisEarningsPerShareDate()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisEarningsPerShareDate(QString date,
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
 * Function:		getSubAnalysisEarningsPerShareId()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisEarningsPerShareId(int mainAnalysisId,
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
                qDebug() << rec.value("DataEarningsId").toString();
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
 * Function:    insertSubAnalysisEarningsPerShare()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisEarningsPerShare(int earningsDateId,
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







/*****************************************************************
 *
 * Function:		getSubAnalysisEarningsPerShareData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisEarningsPerShareData(QString stockName,
                           QString stockSymbol,
                           SubAnalysDataST *dataArr,
                           int &nofArrData,
                           bool dbIsHandledExternly)
{


    QSqlRecord rec;
    QString str;
    SubAnalysDataST data;

    nofArrData = 0;


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
                data.data.clear();
                if(rec.value("DataEarnings").isNull() == false)
                {
                    data.data = rec.value("DataEarnings").toString();
                }

                dataArr[nofArrData] = data;
                nofArrData++;

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

//======================================

#if 1
/****************************************************************
 *
 * Function:    subAnalysisRevenueDateExists()
 *
 * Description:
 *
 * EarningsPerShare
 *
 *
 *
 ****************************************************************/
bool CDbHndl::subAnalysisRevenueDateExists(QString date,
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
                " FROM  TblDateRevenueSubAnalysis "
                " WHERE Date = '%s' AND MainAnalysisId = %d;",
                c_date,
                mainAnalysisId);

    qDebug() << str;


    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblDateRevenueSubAnalysis error 1"), qry.lastError().text().toUtf8().constData());
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



            if(rec.value("Date").isNull() == true || true == rec.value("MainAnalysisId").isNull())
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                earningsDateId = rec.value("DateId").toInt();
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
 * Function:    insertSubAnalysisRevenueDate()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisRevenueDate(QString date,
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


    str.sprintf("INSERT OR REPLACE INTO TblDateRevenueSubAnalysis "
                "(Date, MainAnalysisId) "
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
            QMessageBox::critical(NULL, QString::fromUtf8("TblDateRevenueSubAnalysis"), qry.lastError().text().toUtf8().constData());
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
 * Function:		getSubAnalysisRevenueId()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisRevenueDataId(int mainAnalysisId,
                  int dateId,
                  int &dataId,
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


    str.sprintf("SELECT TblDataRevenueSubAnalysis.DataId, TblDataRevenueSubAnalysis.DateId, TblDataRevenueSubAnalysis.MainAnalysisId   "
                " FROM TblDataRevenueSubAnalysis   "
                " WHERE  "
                "       TblDataRevenueSubAnalysis.DateId = %d AND "
                "       TblDataRevenueSubAnalysis.MainAnalysisId = %d;",
                                                                           dateId,
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




            if(rec.value("DataId").isNull() == true)
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
                qDebug() << rec.value("DataId").toString();
                qDebug() << rec.value("MainAnalysisId").toString();


                if(rec.value("DataId").isNull() == false)
                {
                    dataId = rec.value("DataId").toInt();
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
 * Function:    insertSubAnalysisRevenue()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisRevenueData(int dateId,
                          int mainAnalysisId,
                          int inputDataId,
                          bool dataIdIsValid,
                          QString data,
                          int &dataId,
                          bool dbIsHandledExternly)
{
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }


    QSqlQuery qry(m_db);


    if(dataIdIsValid == true)
    {
        str.sprintf("INSERT OR REPLACE INTO TblDataRevenueSubAnalysis "
                " (DateId, "
                "  Data, "
                 " DataId,"
                 " MainAnalysisId) "
                 " VALUES( %d,  '%s', %d, %d);",
                        dateId,
                        data.toLocal8Bit().constData(),
                        inputDataId,
                        mainAnalysisId);
    }
    // New data
    else
    {
        str.sprintf("INSERT INTO TblDataRevenueSubAnalysis "
                    " (DateId, "
                    " Data, "
                    " MainAnalysisId) "
                    " VALUES( %d, '%s', %d);",
                            dateId,
                            data.toLocal8Bit().constData(),
                            mainAnalysisId);
    }


    qDebug() << str;

    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblDataRevenueSubAnalysis"), qry.lastError().text().toLatin1().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    dataId = (int) qry.lastInsertId().toInt();


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
 * Function:		getSubAnalysisRevenueData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisRevenueData(QString stockName,
                           QString stockSymbol,
                           SubAnalysDataST *dataArr,
                           int &nofArrData,
                           bool dbIsHandledExternly)
{


    QSqlRecord rec;
    QString str;
    SubAnalysDataST data;

    nofArrData = 0;


    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

  bool found = false;


    str.sprintf("SELECT TblMainAnalysis.*, TblDateRevenueSubAnalysis.*, TblDataRevenueSubAnalysis.* "
                " FROM TblMainAnalysis, TblDateRevenueSubAnalysis, TblDataRevenueSubAnalysis  "
                " WHERE  "
                "       TblMainAnalysis.MainAnalysisId = TblDateRevenueSubAnalysis.MainAnalysisId AND "
                "       TblMainAnalysis.MainAnalysisId = TblDataRevenueSubAnalysis.MainAnalysisId AND "
                "       TblDateRevenueSubAnalysis.DateId = TblDataRevenueSubAnalysis.DateId AND "
                "       lower(TblMainAnalysis.stockName) = lower('%s') AND "
                "       lower(TblMainAnalysis.StockSymbol) = lower('%s') "
                " ORDER BY (CAST(TblDateRevenueSubAnalysis.Date AS REAL)) ASC;",              // DESC";",
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

            if((rec.value("Date").isNull() == true) ||  // Date
                (rec.value("Data").isNull() == true))   // Data
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
                qDebug() << rec.value("Date").toString() << "\n";
                qDebug() << rec.value("stockSymbol").toString();
                qDebug() << rec.value("stockName").toString();




                // Date
                data.date.clear();
                if(rec.value("Date").isNull() == false)
                {
                    data.date = rec.value("Date").toString();
                }

                // Data
                data.data.clear();
                if(rec.value("Data").isNull() == false)
                {
                    data.data = rec.value("Data").toString();
                }

                dataArr[nofArrData] = data;
                nofArrData++;

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


#endif





//=============================================






/****************************************************************
 *
 * Function:    subAnalysisTotEarningsDateExists()
 *
 * Description:
 *
 * EarningsPerShare
 *
 *
 *
 ****************************************************************/
bool CDbHndl::subAnalysisTotEarningsDateExists(QString date,
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
                " FROM  TblDateTotEarningsSubAnalysis "
                " WHERE DateTotEarnings = '%s' AND MainAnalysisId = %d;",
                c_date,
                mainAnalysisId);

    qDebug() << str;


    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblDateTotEarningsSubAnalysis error 1"), qry.lastError().text().toUtf8().constData());
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



            if(rec.value("DateTotEarnings").isNull() == true || true == rec.value("MainAnalysisId").isNull())
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                earningsDateId = rec.value("DateTotEarningsId").toInt();
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
 * Function:    insertSubAnalysisTotEarningsDate()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisTotEarningsDate(QString date,
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


    str.sprintf("INSERT OR REPLACE INTO TblDateTotEarningsSubAnalysis "
                "(DateTotEarnings, MainAnalysisId) "
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
            QMessageBox::critical(NULL, QString::fromUtf8("TblDateTotEarningsSubAnalysis"), qry.lastError().text().toUtf8().constData());
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
 * Function:		getSubAnalysisTotEarningsDataId()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisTotEarningsDataId(int mainAnalysisId,
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


    str.sprintf("SELECT TblDataTotEarningsSubAnalysis.DataTotEarningsId, TblDataTotEarningsSubAnalysis.DateTotEarningsId, TblDataTotEarningsSubAnalysis.MainAnalysisId   "
                " FROM TblDataTotEarningsSubAnalysis   "
                " WHERE  "
                "       TblDataTotEarningsSubAnalysis.DateTotEarningsId = %d AND "
                "       TblDataTotEarningsSubAnalysis.MainAnalysisId = %d;",
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




            if(rec.value("DataTotEarningsId").isNull() == true)
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
                qDebug() << rec.value("DataTotEarningsId").toString();
                qDebug() << rec.value("MainAnalysisId").toString();


                if(rec.value("DataTotEarningsId").isNull() == false)
                {
                    earningsDataId = rec.value("DataTotEarningsId").toInt();
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
 * Function:    insertSubAnalysisTotEarnings()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisTotEarningsData(int earningsDateId,
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
        str.sprintf("INSERT OR REPLACE INTO TblDataTotEarningsSubAnalysis "
                " (DateTotEarningsId, "
                "  DataTotEarnings, "
                 " DataTotEarningsId,"
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
        str.sprintf("INSERT INTO TblDataTotEarningsSubAnalysis "
                    " (DateTotEarningsId, "
                    " DataTotEarnings, "
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
            QMessageBox::critical(NULL, QString::fromUtf8("TblDataTotEarningsSubAnalysis"), qry.lastError().text().toLatin1().constData());
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







/*****************************************************************
 *
 * Function:		getSubAnalysisTotEarningsData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisTotEarningsData(QString stockName,
                           QString stockSymbol,
                           SubAnalysDataST *dataArr,
                           int &nofArrData,
                           bool dbIsHandledExternly)
{


    QSqlRecord rec;
    QString str;
    SubAnalysDataST data;

    nofArrData = 0;


    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

  bool found = false;


    str.sprintf("SELECT TblMainAnalysis.*, TblDateTotEarningsSubAnalysis.*, TblDataTotEarningsSubAnalysis.* "
                " FROM TblMainAnalysis, TblDateTotEarningsSubAnalysis, TblDataTotEarningsSubAnalysis  "
                " WHERE  "
                "       TblMainAnalysis.MainAnalysisId = TblDateTotEarningsSubAnalysis.MainAnalysisId AND "
                "       TblMainAnalysis.MainAnalysisId = TblDataTotEarningsSubAnalysis.MainAnalysisId AND "
                "       TblDateTotEarningsSubAnalysis.DateTotEarningsId = TblDataTotEarningsSubAnalysis.DateTotEarningsId AND "
                "       lower(TblMainAnalysis.stockName) = lower('%s') AND "
                "       lower(TblMainAnalysis.StockSymbol) = lower('%s') "
                " ORDER BY (CAST(TblDateTotEarningsSubAnalysis.DateTotEarnings AS REAL)) ASC;",              // DESC";",
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

            if((rec.value("DateTotEarnings").isNull() == true) ||  // Date
                (rec.value("DataTotEarnings").isNull() == true))   // Data
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
                qDebug() << rec.value("DateTotEarnings").toString() << "\n";
                qDebug() << rec.value("stockSymbol").toString();
                qDebug() << rec.value("stockName").toString();




                // Date
                data.date.clear();
                if(rec.value("DateTotEarnings").isNull() == false)
                {
                    data.date = rec.value("DateTotEarnings").toString();
                }

                // Data
                data.data.clear();
                if(rec.value("DataTotEarnings").isNull() == false)
                {
                    data.data = rec.value("DataTotEarnings").toString();
                }

                dataArr[nofArrData] = data;
                nofArrData++;

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
                           SubAnalysDataST *dataArr,
                           int &nofArrData,
                           bool dbIsHandledExternly)
{


    QSqlRecord rec;
    QString str;
    SubAnalysDataST data;

    nofArrData = 0;


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
                data.data.clear();
                if(rec.value("DataDividend").isNull() == false)
                {
                    data.data = rec.value("DataDividend").toString();
                }

                dataArr[nofArrData] = data;
                nofArrData++;

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
 * Function:    subAnalysisTotalCurrentAssetsDateExists()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::subAnalysisTotalCurrentAssetsDateExists(QString date,
                                                      int mainAnalysisId,
                                                      int &totalCurrentAssetsDateId)
{
    QSqlRecord rec;
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);



    QByteArray ba = date.toLocal8Bit();
    const char *c_date = ba.data();



    str.sprintf("SELECT * "
                " FROM  TblDateTotalCurrentAssetsSubAnalysis "
                " WHERE DateTotalCurrentAssets = '%s' AND MainAnalysisId = %d;",
                c_date,
                mainAnalysisId);

    qDebug() << str;


    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblDateTotalCurrentAssetsSubAnalysis error 1"), qry.lastError().text().toUtf8().constData());
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



            if(rec.value("DateTotalCurrentAssets").isNull() == true || true == rec.value("MainAnalysisId").isNull())
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                totalCurrentAssetsDateId = rec.value("DateTotalCurrentAssetsId").toInt();
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
 * Function:    insertSubAnalysisTotalCurrentAssetsDate()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisTotalCurrentAssetsDate(QString date,
                       int mainAnalysisId,
                       int &dateTotalCurrentAssetsId,
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


    str.sprintf("INSERT OR REPLACE INTO TblDateTotalCurrentAssetsSubAnalysis "
                "(DateTotalCurrentAssets, MainAnalysisId) "
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
            QMessageBox::critical(NULL, QString::fromUtf8("TblDateTotalCurrentAssetsSubAnalysis"), qry.lastError().text().toUtf8().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    dateTotalCurrentAssetsId = (int) qry.lastInsertId().toInt();

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
 * Function:		getSubAnalysisTotalCurrentAssetsDataId()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisTotalCurrentAssetsDataId(int mainAnalysisId,
                                       int totalCurrentAssetsDateId,
                                       int &totalCurrentAssetsDataId,
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


    str.sprintf("SELECT TblDataTotalCurrentAssetsSubAnalysis.DataTotalCurrentAssetsId, TblDataTotalCurrentAssetsSubAnalysis.DateTotalCurrentAssetsId, TblDataTotalCurrentAssetsSubAnalysis.MainAnalysisId   "
                " FROM TblDataTotalCurrentAssetsSubAnalysis   "
                " WHERE  "
                "       TblDataTotalCurrentAssetsSubAnalysis.DateTotalCurrentAssetsId = %d AND "
                "       TblDataTotalCurrentAssetsSubAnalysis.MainAnalysisId = %d;",
                                                                           totalCurrentAssetsDateId,
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




            if(rec.value("DataTotalCurrentAssetsId").isNull() == true)
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
                qDebug() << rec.value("DataTotalCurrentAssetsId").toString();
                qDebug() << rec.value("MainAnalysisId").toString();


                if(rec.value("DataTotalCurrentAssetsId").isNull() == false)
                {
                    totalCurrentAssetsDataId = rec.value("DataTotalCurrentAssetsId").toInt();
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
insertSubAnalysisTotalCurrentAssets(int totalCurrentAssetsDateId,
                          int mainAnalysisId,
                          int inputTotalCurrentAssetsDataId,
                          bool totalCurrentAssetsDataIdIsValid,
                          QString dataTotalCurrentAssets,
                          int &totalCurrentAssetsDataId,
                          bool dbIsHandledExternly)
{
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }


    QSqlQuery qry(m_db);


    if(totalCurrentAssetsDataIdIsValid == true)
    {
        str.sprintf("INSERT OR REPLACE INTO TblDataTotalCurrentAssetsSubAnalysis "
                " (DateTotalCurrentAssetsId, "
                "  DataTotalCurrentAssets, "
                 " DataTotalCurrentAssetsId,"
                 " MainAnalysisId) "
                 " VALUES( %d,  '%s', %d, %d);",
                        totalCurrentAssetsDateId,
                        dataTotalCurrentAssets.toLocal8Bit().constData(),
                        inputTotalCurrentAssetsDataId,
                        mainAnalysisId);
    }
    // New data
    else
    {
        str.sprintf("INSERT INTO TblDataTotalCurrentAssetsSubAnalysis "
                    " (DateTotalCurrentAssetsId, "
                    " DataTotalCurrentAssets, "
                    " MainAnalysisId) "
                    " VALUES( %d, '%s', %d);",
                            totalCurrentAssetsDateId,
                            dataTotalCurrentAssets.toLocal8Bit().constData(),
                            mainAnalysisId);
    }


    qDebug() << str;

    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblDataTotalCurrentAssetsSubAnalysis"), qry.lastError().text().toLatin1().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    totalCurrentAssetsDataId = (int) qry.lastInsertId().toInt();


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
 * Function:		getSubAnalysisTotalCurrentAssetsData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisTotalCurrentAssetsData(QString stockName,
                                     QString stockSymbol,
                                     SubAnalysDataST *dataArr,
                                     int &nofArrData,
                                     bool dbIsHandledExternly)
{


    QSqlRecord rec;
    QString str;
    SubAnalysDataST data;

    nofArrData = 0;


    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

  bool found = false;


    str.sprintf("SELECT TblMainAnalysis.*, TblDateTotalCurrentAssetsSubAnalysis.*, TblDataTotalCurrentAssetsSubAnalysis.* "
                " FROM TblMainAnalysis, TblDateTotalCurrentAssetsSubAnalysis, TblDataTotalCurrentAssetsSubAnalysis  "
                " WHERE  "
                "       TblMainAnalysis.MainAnalysisId = TblDateTotalCurrentAssetsSubAnalysis.MainAnalysisId AND "
                "       TblMainAnalysis.MainAnalysisId = TblDataTotalCurrentAssetsSubAnalysis.MainAnalysisId AND "
                "       TblDateTotalCurrentAssetsSubAnalysis.DateTotalCurrentAssetsId = TblDataTotalCurrentAssetsSubAnalysis.DateTotalCurrentAssetsId AND "
                "       lower(TblMainAnalysis.stockName) = lower('%s') AND "
                "       lower(TblMainAnalysis.StockSymbol) = lower('%s') "
                " ORDER BY (CAST(TblDateTotalCurrentAssetsSubAnalysis.DateTotalCurrentAssets AS REAL)) ASC;",              // DESC";",
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

            if((rec.value("DateTotalCurrentAssets").isNull() == true) ||  // Date
                (rec.value("DataTotalCurrentAssets").isNull() == true))   // Data
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
                qDebug() << rec.value("DateTotalCurrentAssets").toString() << "\n";
                qDebug() << rec.value("stockSymbol").toString();
                qDebug() << rec.value("stockName").toString();




                // Date
                data.date.clear();
                if(rec.value("DateTotalCurrentAssets").isNull() == false)
                {
                    data.date = rec.value("DateTotalCurrentAssets").toString();
                }

                // Data
                data.data.clear();
                if(rec.value("DataTotalCurrentAssets").isNull() == false)
                {
                    data.data = rec.value("DataTotalCurrentAssets").toString();
                }

                dataArr[nofArrData] = data;
                nofArrData++;

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


//=====================================





/****************************************************************
 *
 * Function:    subAnalysisTotalCurrentLiabilitiesDateExists()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::subAnalysisTotalCurrentLiabilitiesDateExists(QString date,
                                     int mainAnalysisId,
                                     int &totalCurrentLiabilitiesDateId)
{
    QSqlRecord rec;
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);



    QByteArray ba = date.toLocal8Bit();
    const char *c_date = ba.data();



    str.sprintf("SELECT * "
                " FROM  TblDateTotalCurrentLiabilitiesSubAnalysis "
                " WHERE DateTotalCurrentLiabilities = '%s' AND MainAnalysisId = %d;",
                c_date,
                mainAnalysisId);

    qDebug() << str;


    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblDateTotalCurrentLiabilitiesSubAnalysis error 1"), qry.lastError().text().toUtf8().constData());
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



            if((rec.value("DateTotalCurrentLiabilities").isNull() == true) ||
               (true == rec.value("MainAnalysisId").isNull()))
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                totalCurrentLiabilitiesDateId = rec.value("DateTotalCurrentLiabilitiesId").toInt();
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
 * Function:    insertSubAnalysisTotalCurrentLiabilitiesDate()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisTotalCurrentLiabilitiesDate(QString date,
                                             int mainAnalysisId,
                                             int &dateTotalCurrentLiabilitiesId,
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


    str.sprintf("INSERT OR REPLACE INTO TblDateTotalCurrentLiabilitiesSubAnalysis "
                "(DateTotalCurrentLiabilities, MainAnalysisId) "
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
            QMessageBox::critical(NULL, QString::fromUtf8("TblDateTotalCurrentLiabilitiesSubAnalysis"), qry.lastError().text().toUtf8().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    dateTotalCurrentLiabilitiesId = (int) qry.lastInsertId().toInt();

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
 * Function:		getSubAnalysisTotalCurrentLiabilitiesDataId()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisTotalCurrentLiabilitiesDataId(int mainAnalysisId,
                  int totalCurrentLiabilitiesDateId,
                  int &totalCurrentLiabilitiesDataId,
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


    str.sprintf("SELECT TblDataTotalCurrentLiabilitiesSubAnalysis.DataTotalCurrentLiabilitiesId, TblDataTotalCurrentLiabilitiesSubAnalysis.DateTotalCurrentLiabilitiesId, TblDataTotalCurrentLiabilitiesSubAnalysis.MainAnalysisId   "
                " FROM TblDataTotalCurrentLiabilitiesSubAnalysis   "
                " WHERE  "
                "       TblDataTotalCurrentLiabilitiesSubAnalysis.DateTotalCurrentLiabilitiesId = %d AND "
                "       TblDataTotalCurrentLiabilitiesSubAnalysis.MainAnalysisId = %d;",
                                                                           totalCurrentLiabilitiesDateId,
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




            if(rec.value("DataTotalCurrentLiabilitiesId").isNull() == true)
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
                qDebug() << rec.value("DataTotalCurrentLiabilitiesId").toString();
                qDebug() << rec.value("MainAnalysisId").toString();


                if(rec.value("DataTotalCurrentLiabilitiesId").isNull() == false)
                {
                    totalCurrentLiabilitiesDataId = rec.value("DataTotalCurrentLiabilitiesId").toInt();
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
 * Function:    insertSubAnalysisTotalCurrentLiabilities()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisTotalCurrentLiabilities(int dateId,
                                         int mainAnalysisId,
                                         int inputDataId,
                                         bool dataIdIsValid,
                                         QString data,
                                         int &dataId,
                                         bool dbIsHandledExternly)
{
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }


    QSqlQuery qry(m_db);


    if(dataIdIsValid == true)
    {
        str.sprintf("INSERT OR REPLACE INTO TblDataTotalCurrentLiabilitiesSubAnalysis "
                " (DateTotalCurrentLiabilitiesId, "
                "  DataTotalCurrentLiabilities, "
                 " DataTotalCurrentLiabilitiesId,"
                 " MainAnalysisId) "
                 " VALUES( %d,  '%s', %d, %d);",
                        dateId,
                        data.toLocal8Bit().constData(),
                        inputDataId,
                        mainAnalysisId);
    }
    // New data
    else
    {
        str.sprintf("INSERT INTO TblDataTotalCurrentLiabilitiesSubAnalysis "
                    " (DateTotalCurrentLiabilitiesId, "
                    " DataTotalCurrentLiabilities, "
                    " MainAnalysisId) "
                    " VALUES( %d, '%s', %d);",
                            dateId,
                            data.toLocal8Bit().constData(),
                            mainAnalysisId);
    }


    qDebug() << str;

    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblDataTotalCurrentLiabilitiesSubAnalysis"), qry.lastError().text().toLatin1().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    dataId = (int) qry.lastInsertId().toInt();


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
 * Function:		getSubAnalysisTotalCurrentLiabilitiesData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisTotalCurrentLiabilitiesData(QString stockName,
                           QString stockSymbol,
                           SubAnalysDataST *dataArr,
                           int &nofArrData,
                           bool dbIsHandledExternly)
{


    QSqlRecord rec;
    QString str;
    SubAnalysDataST data;

    nofArrData = 0;


    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

  bool found = false;


    str.sprintf("SELECT TblMainAnalysis.*, TblDateTotalCurrentLiabilitiesSubAnalysis.*, TblDataTotalCurrentLiabilitiesSubAnalysis.* "
                " FROM TblMainAnalysis, TblDateTotalCurrentLiabilitiesSubAnalysis, TblDataTotalCurrentLiabilitiesSubAnalysis  "
                " WHERE  "
                "       TblMainAnalysis.MainAnalysisId = TblDateTotalCurrentLiabilitiesSubAnalysis.MainAnalysisId AND "
                "       TblMainAnalysis.MainAnalysisId = TblDataTotalCurrentLiabilitiesSubAnalysis.MainAnalysisId AND "
                "       TblDateTotalCurrentLiabilitiesSubAnalysis.DateTotalCurrentLiabilitiesId = TblDataTotalCurrentLiabilitiesSubAnalysis.DateTotalCurrentLiabilitiesId AND "
                "       lower(TblMainAnalysis.stockName) = lower('%s') AND "
                "       lower(TblMainAnalysis.StockSymbol) = lower('%s') "
                " ORDER BY (CAST(TblDateTotalCurrentLiabilitiesSubAnalysis.DateTotalCurrentLiabilities AS REAL)) ASC;",              // DESC";",
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

            if((rec.value("DateTotalCurrentLiabilities").isNull() == true) ||  // Date
                (rec.value("DataTotalCurrentLiabilities").isNull() == true))   // Data
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
                qDebug() << rec.value("DateTotalCurrentLiabilities").toString() << "\n";
                qDebug() << rec.value("stockSymbol").toString();
                qDebug() << rec.value("stockName").toString();




                // Date
                data.date.clear();
                if(rec.value("DateTotalCurrentLiabilities").isNull() == false)
                {
                    data.date = rec.value("DateTotalCurrentLiabilities").toString();
                }

                // Data
                data.data.clear();
                if(rec.value("DataTotalCurrentLiabilities").isNull() == false)
                {
                    data.data = rec.value("DataTotalCurrentLiabilities").toString();
                }

                dataArr[nofArrData] = data;
                nofArrData++;

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


//===============================================


/****************************************************************
 *
 * Function:    subAnalysisTotalLiabilitiesDateExists()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::subAnalysisTotalLiabilitiesDateExists(QString date,
                                     int mainAnalysisId,
                                     int &dateId)
{
    QSqlRecord rec;
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);



    QByteArray ba = date.toLocal8Bit();
    const char *c_date = ba.data();



    str.sprintf("SELECT * "
                " FROM  TblDateTotalLiabilitiesSubAnalysis "
                " WHERE DateTotalLiabilities = '%s' AND MainAnalysisId = %d;",
                c_date,
                mainAnalysisId);

    qDebug() << str;


    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblDateTotalLiabilitiesSubAnalysis error 1"), qry.lastError().text().toUtf8().constData());
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



            if((rec.value("DateTotalLiabilities").isNull() == true) ||
               (true == rec.value("MainAnalysisId").isNull()))
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                dateId = rec.value("DateTotalLiabilitiesId").toInt();
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
 * Function:    insertSubAnalysisTotalLiabilitiesDate()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisTotalLiabilitiesDate(QString date,
                                      int mainAnalysisId,
                                      int &dateId,
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


    str.sprintf("INSERT OR REPLACE INTO TblDateTotalLiabilitiesSubAnalysis "
                "(DateTotalLiabilities, MainAnalysisId) "
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
            QMessageBox::critical(NULL, QString::fromUtf8("TblDateTotalLiabilitiesSubAnalysis"), qry.lastError().text().toUtf8().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    dateId = (int) qry.lastInsertId().toInt();

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
 * Function:		getSubAnalysisTotalLiabilitiesDataId()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisTotalLiabilitiesDataId(int mainAnalysisId,
                  int dateId,
                  int &dataId,
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


    str.sprintf("SELECT TblDataTotalLiabilitiesSubAnalysis.DataTotalLiabilitiesId, TblDataTotalLiabilitiesSubAnalysis.DateTotalLiabilitiesId, TblDataTotalLiabilitiesSubAnalysis.MainAnalysisId   "
                " FROM TblDataTotalLiabilitiesSubAnalysis   "
                " WHERE  "
                "       TblDataTotalLiabilitiesSubAnalysis.DateTotalLiabilitiesId = %d AND "
                "       TblDataTotalLiabilitiesSubAnalysis.MainAnalysisId = %d;",
                                                                           dateId,
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




            if(rec.value("DataTotalLiabilitiesId").isNull() == true)
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
                qDebug() << rec.value("DataTotalLiabilitiesId").toString();
                qDebug() << rec.value("MainAnalysisId").toString();


                if(rec.value("DataTotalLiabilitiesId").isNull() == false)
                {
                    dataId = rec.value("DataTotalLiabilitiesId").toInt();
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
 * Function:    insertSubAnalysisTotalLiabilities()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisTotalLiabilities(int dateId,
                                         int mainAnalysisId,
                                         int inputDataId,
                                         bool dataIdIsValid,
                                         QString data,
                                         int &dataId,
                                         bool dbIsHandledExternly)
{
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }


    QSqlQuery qry(m_db);


    if(dataIdIsValid == true)
    {
        str.sprintf("INSERT OR REPLACE INTO TblDataTotalLiabilitiesSubAnalysis "
                " (DateTotalLiabilitiesId, "
                "  DataTotalLiabilities, "
                 " DataTotalLiabilitiesId,"
                 " MainAnalysisId) "
                 " VALUES( %d,  '%s', %d, %d);",
                        dateId,
                        data.toLocal8Bit().constData(),
                        inputDataId,
                        mainAnalysisId);
    }
    // New data
    else
    {
        str.sprintf("INSERT INTO TblDataTotalLiabilitiesSubAnalysis "
                    " (DateTotalLiabilitiesId, "
                    " DataTotalLiabilities, "
                    " MainAnalysisId) "
                    " VALUES( %d, '%s', %d);",
                            dateId,
                            data.toLocal8Bit().constData(),
                            mainAnalysisId);
    }


    qDebug() << str;

    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblDataTotalLiabilitiesSubAnalysis"), qry.lastError().text().toLatin1().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    dataId = (int) qry.lastInsertId().toInt();


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
 * Function:		getSubAnalysisTotalLiabilitiesData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisTotalLiabilitiesData(QString stockName,
                           QString stockSymbol,
                           SubAnalysDataST *dataArr,
                           int &nofArrData,
                           bool dbIsHandledExternly)
{


    QSqlRecord rec;
    QString str;
    SubAnalysDataST data;

    nofArrData = 0;


    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

  bool found = false;


    str.sprintf("SELECT TblMainAnalysis.*, TblDateTotalLiabilitiesSubAnalysis.*, TblDataTotalLiabilitiesSubAnalysis.* "
                " FROM TblMainAnalysis, TblDateTotalLiabilitiesSubAnalysis, TblDataTotalLiabilitiesSubAnalysis  "
                " WHERE  "
                "       TblMainAnalysis.MainAnalysisId = TblDateTotalLiabilitiesSubAnalysis.MainAnalysisId AND "
                "       TblMainAnalysis.MainAnalysisId = TblDataTotalLiabilitiesSubAnalysis.MainAnalysisId AND "
                "       TblDateTotalLiabilitiesSubAnalysis.DateTotalLiabilitiesId = TblDataTotalLiabilitiesSubAnalysis.DateTotalLiabilitiesId AND "
                "       lower(TblMainAnalysis.stockName) = lower('%s') AND "
                "       lower(TblMainAnalysis.StockSymbol) = lower('%s') "
                " ORDER BY (CAST(TblDateTotalLiabilitiesSubAnalysis.DateTotalLiabilities AS REAL)) ASC;",              // DESC";",
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

            qDebug() << rec.value("DataTotalLiabilities").toString();
            qDebug() << rec.value("DateTotalLiabilities").toString();
            qDebug() << rec.value("stockSymbol").toString();
            qDebug() << rec.value("stockName").toString();


            if((rec.value("DateTotalLiabilities").isNull() == true) ||  // Date
                (rec.value("DataTotalLiabilities").isNull() == true))   // Data
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
                qDebug() << rec.value("DateTotalLiabilities").toString() << "\n";
                qDebug() << rec.value("stockSymbol").toString();
                qDebug() << rec.value("stockName").toString();




                // Date
                data.date.clear();
                if(rec.value("DateTotalLiabilities").isNull() == false)
                {
                    data.date = rec.value("DateTotalLiabilities").toString();
                }

                // Data
                data.data.clear();
                if(rec.value("DataTotalLiabilities").isNull() == false)
                {
                    data.data = rec.value("DataTotalLiabilities").toString();
                }

                dataArr[nofArrData] = data;
                nofArrData++;

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


//==================================


/****************************************************************
 *
 * Function:    subAnalysisSolidityDateExists()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::subAnalysisSolidityDateExists(QString date,
                                     int mainAnalysisId,
                                     int &dateId)
{
    QSqlRecord rec;
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);



    QByteArray ba = date.toLocal8Bit();
    const char *c_date = ba.data();



    str.sprintf("SELECT * "
                " FROM  TblDateSoliditySubAnalysis "
                " WHERE DateSolidity = '%s' AND MainAnalysisId = %d;",
                c_date,
                mainAnalysisId);

    qDebug() << str;


    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblDateSoliditySubAnalysis error 1"), qry.lastError().text().toUtf8().constData());
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



            if((rec.value("DateSolidity").isNull() == true) ||
               (true == rec.value("MainAnalysisId").isNull()))
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                dateId = rec.value("DateSolidityId").toInt();
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
 * Function:    insertSubAnalysisSolidityDate()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisSolidityDate(QString date,
                              int mainAnalysisId,
                              int &dateId,
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


    str.sprintf("INSERT OR REPLACE INTO TblDateSoliditySubAnalysis "
                "(DateSolidity, MainAnalysisId) "
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
            QMessageBox::critical(NULL, QString::fromUtf8("TblDateSoliditySubAnalysis"), qry.lastError().text().toUtf8().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    dateId = (int) qry.lastInsertId().toInt();

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
 * Function:		getSubAnalysisSolidityDataId()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisSolidityDataId(int mainAnalysisId,
                             int dateId,
                             int &dataId,
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


    str.sprintf("SELECT TblDataSoliditySubAnalysis.*   "
                " FROM TblDataSoliditySubAnalysis   "
                " WHERE  "
                "       DateSolidityId = %d AND "
                "       MainAnalysisId = %d;",
                                                  dateId,
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




            if(rec.value("DataSolidityId").isNull() == true)
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
                qDebug() << rec.value("DataSolidityId").toString();
                qDebug() << rec.value("MainAnalysisId").toString();


                if(rec.value("DataSolidityId").isNull() == false)
                {
                    dataId = rec.value("DataSolidityId").toInt();
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
 * Function:    insertSubAnalysisSolidityData()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisSolidityData(int dateId,
                              int mainAnalysisId,
                              int inputDataId,
                              bool dataIdIsValid,
                              QString data,
                              int &dataId,
                              bool dbIsHandledExternly)
{
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }


    QSqlQuery qry(m_db);


    if(dataIdIsValid == true)
    {
        str.sprintf("INSERT OR REPLACE INTO TblDataSoliditySubAnalysis "
                    " (DateSolidityId, "
                    "  DataSolidity, "
                    "  DataSolidityId,"
                    "  MainAnalysisId) "
                    " VALUES( %d,  '%s', %d, %d);",
                        dateId,
                        data.toLocal8Bit().constData(),
                        inputDataId,
                        mainAnalysisId);
    }
    // New data
    else
    {
        str.sprintf("INSERT INTO TblDataSoliditySubAnalysis "
                    " (DateSolidityId, "
                    "  DataSolidity, "
                    " MainAnalysisId) "
                    " VALUES( %d, '%s', %d);",
                            dateId,
                            data.toLocal8Bit().constData(),
                            mainAnalysisId);
    }


    qDebug() << str;

    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblDataSoliditySubAnalysis"), qry.lastError().text().toLatin1().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    dataId = (int) qry.lastInsertId().toInt();


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
 * Function:		getSubAnalysisSolidityData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisSolidityData(QString stockName,
                           QString stockSymbol,
                           SubAnalysDataST *dataArr,
                           int &nofArrData,
                           bool dbIsHandledExternly)
{


    QSqlRecord rec;
    QString str;
    SubAnalysDataST data;

    nofArrData = 0;


    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

  bool found = false;


    str.sprintf("SELECT TblMainAnalysis.*, TblDateSoliditySubAnalysis.*, TblDataSoliditySubAnalysis.* "
                " FROM TblMainAnalysis, TblDateSoliditySubAnalysis, TblDataSoliditySubAnalysis "
                " WHERE  "
                "       TblMainAnalysis.MainAnalysisId = TblDateSoliditySubAnalysis.MainAnalysisId AND "
                "       TblMainAnalysis.MainAnalysisId = TblDataSoliditySubAnalysis.MainAnalysisId AND "
                "       TblDateSoliditySubAnalysis.DateSolidityId = TblDataSoliditySubAnalysis.DateSolidityId AND "
                "       lower(TblMainAnalysis.stockName) = lower('%s') AND "
                "       lower(TblMainAnalysis.StockSymbol) = lower('%s') "
                " ORDER BY (CAST(TblDateSoliditySubAnalysis.DateSolidity AS REAL)) ASC;",              // DESC";",
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

            //qDebug() << rec.value("DataSolidity").toString();
            //qDebug() << rec.value("DateSolidity").toString();
            //qDebug() << rec.value("stockSymbol").toString();
            //qDebug() << rec.value("stockName").toString();


            if((rec.value("DateSolidity").isNull() == true) ||  // Date
                (rec.value("DataSolidity").isNull() == true))   // Data
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
                qDebug() << rec.value("DateSolidity").toString() << "\n";
                qDebug() << rec.value("stockSymbol").toString();
                qDebug() << rec.value("stockName").toString();




                // Date
                data.date.clear();
                if(rec.value("DateSolidity").isNull() == false)
                {
                    data.date = rec.value("DateSolidity").toString();
                }

                // Data
                data.data.clear();
                if(rec.value("DataSolidity").isNull() == false)
                {
                    data.data = rec.value("DataSolidity").toString();
                }

                dataArr[nofArrData] = data;
                nofArrData++;

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




//=====================

//===================



/****************************************************************
 *
 * Function:    subAnalysisCoverageRatioDateExists()
 *
 * Description:  (räntetäckningsgraden)
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::subAnalysisCoverageRatioDateExists(QString date,
                                                 int mainAnalysisId,
                                                 int &dateId)
{
    QSqlRecord rec;
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    QByteArray ba = date.toLocal8Bit();
    const char *c_date = ba.data();


    str.sprintf("SELECT * "
                " FROM  TblDateCoverageRatioSubAnalysis "
                " WHERE CoverageRatioDate = '%s' AND MainAnalysisId = %d;",
                c_date,
                mainAnalysisId);

    qDebug() << str;


    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblDateCoverageRatioSubAnalysis error 1"), qry.lastError().text().toUtf8().constData());
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


            if((rec.value("CoverageRatioDate").isNull() == true) ||
               (true == rec.value("MainAnalysisId").isNull()))
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                dateId = rec.value("CoverageRatioDateId").toInt();
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
 * Function:    insertSubAnalysisCoverageRatioDate()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisCoverageRatioDate(QString date,
                                   int mainAnalysisId,
                                   int &dateId,
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


    str.sprintf("INSERT OR REPLACE INTO TblDateCoverageRatioSubAnalysis "
                "(CoverageRatioDate, MainAnalysisId) "
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
            QMessageBox::critical(NULL, QString::fromUtf8("TblDateCoverageRatioSubAnalysis"), qry.lastError().text().toUtf8().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    dateId = (int) qry.lastInsertId().toInt();

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
 * Function:		getSubAnalysisCoverageRatioDataId()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisCoverageRatioDataId(int mainAnalysisId,
                                  int dateId,
                                  int &dataId,
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


    str.sprintf("SELECT TblDataCoverageRatioSubAnalysis.*   "
                " FROM TblDataCoverageRatioSubAnalysis   "
                " WHERE  "
                "       CoverageRatioDateId = %d AND "
                "       MainAnalysisId = %d;",
                                                  dateId,
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


            if(rec.value("CoverageRatioDataId").isNull() == true)
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
                // qDebug() << rec.value("CoverageRatioDataId").toString();
                // qDebug() << rec.value("MainAnalysisId").toString();

                if(rec.value("CoverageRatioDataId").isNull() == false)
                {
                    dataId = rec.value("CoverageRatioDataId").toInt();
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
 * Function:    insertSubAnalysisCoverageRatioData()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisCoverageRatioData(int dateId,
                                   int mainAnalysisId,
                                   int inputDataId,
                                   bool dataIdIsValid,
                                   QString data,
                                   int &dataId,
                                   bool dbIsHandledExternly)
{
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }


    QSqlQuery qry(m_db);


    if(dataIdIsValid == true)
    {
        str.sprintf("INSERT OR REPLACE INTO TblDataCoverageRatioSubAnalysis "
                    " (CoverageRatioDateId, "
                    "  CoverageRatioData, "
                    "  CoverageRatioDataId,"
                    "  MainAnalysisId) "
                    " VALUES( %d,  '%s', %d, %d);",
                        dateId,
                        data.toLocal8Bit().constData(),
                        inputDataId,
                        mainAnalysisId);
    }
    // New data
    else
    {
        str.sprintf("INSERT INTO TblDataCoverageRatioSubAnalysis "
                    " (CoverageRatioDateId, "
                    "  CoverageRatioData, "
                    " MainAnalysisId) "
                    " VALUES( %d, '%s', %d);",
                            dateId,
                            data.toLocal8Bit().constData(),
                            mainAnalysisId);
    }


    qDebug() << str;

    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblDataCoverageRatioSubAnalysis"), qry.lastError().text().toLatin1().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    dataId = (int) qry.lastInsertId().toInt();


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
 * Function:		getSubAnalysisCoverageRatioData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisCoverageRatioData(QString stockName,
                                QString stockSymbol,
                                SubAnalysDataST *dataArr,
                                int &nofArrData,
                                bool dbIsHandledExternly)
{
    QSqlRecord rec;
    QString str;
    SubAnalysDataST data;

    nofArrData = 0;


    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

  bool found = false;


    str.sprintf("SELECT TblMainAnalysis.*, TblDateCoverageRatioSubAnalysis.*, TblDataCoverageRatioSubAnalysis.* "
                " FROM TblMainAnalysis, TblDateCoverageRatioSubAnalysis, TblDataCoverageRatioSubAnalysis "
                " WHERE  "
                "       TblMainAnalysis.MainAnalysisId = TblDateCoverageRatioSubAnalysis.MainAnalysisId AND "
                "       TblMainAnalysis.MainAnalysisId = TblDataCoverageRatioSubAnalysis.MainAnalysisId AND "
                "       TblDateCoverageRatioSubAnalysis.CoverageRatioDateId = TblDataCoverageRatioSubAnalysis.CoverageRatioDateId AND "
                "       lower(TblMainAnalysis.stockName) = lower('%s') AND "
                "       lower(TblMainAnalysis.StockSymbol) = lower('%s') "
                " ORDER BY (CAST(TblDateCoverageRatioSubAnalysis.CoverageRatioDate AS REAL)) ASC;",              // DESC";",
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


            if( (rec.value("CoverageRatioDate").isNull() == true) ||  // Date
                (rec.value("CoverageRatioData").isNull() == true))   // Data
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
                // qDebug() << rec.value("CoverageRatioDate").toString() << "\n";
                // qDebug() << rec.value("stockSymbol").toString();
                // qDebug() << rec.value("stockName").toString();


                // Date
                data.date.clear();
                if(rec.value("CoverageRatioDate").isNull() == false)
                {
                    data.date = rec.value("CoverageRatioDate").toString();
                }

                // Data
                data.data.clear();
                if(rec.value("CoverageRatioData").isNull() == false)
                {
                    data.data = rec.value("CoverageRatioData").toString();
                }

                dataArr[nofArrData] = data;
                nofArrData++;
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






//================================================








/****************************************************************
 *
 * Function:    subAnalysisCoreTier1RatioDateExists()
 *
 * Description:  (primärkapitalrelation, Lundaluppen)
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::subAnalysisCoreTier1RatioDateExists(QString date,
                                                 int mainAnalysisId,
                                                 int &dateId)
{
    QSqlRecord rec;
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    QByteArray ba = date.toLocal8Bit();
    const char *c_date = ba.data();


    str.sprintf("SELECT * "
                " FROM  TblDateTier1RatioSubAnalysis "
                " WHERE DateTier1Ratio = '%s' AND MainAnalysisId = %d;",
                c_date,
                mainAnalysisId);

    qDebug() << str;


    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblDateTier1RatioSubAnalysis error 1"), qry.lastError().text().toUtf8().constData());
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


            if((rec.value("DateTier1Ratio").isNull() == true) ||
               (true == rec.value("MainAnalysisId").isNull()))
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                dateId = rec.value("DateTier1RatioId").toInt();
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
 * Function:    insertSubAnalysisCoreTier1RatioDate()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisCoreTier1RatioDate(QString date,
                                    int mainAnalysisId,
                                    int &dateId,
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


    str.sprintf("INSERT OR REPLACE INTO TblDateTier1RatioSubAnalysis "
                "(DateTier1Ratio, MainAnalysisId) "
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
            QMessageBox::critical(NULL, QString::fromUtf8("TblDateTier1RatioSubAnalysis"), qry.lastError().text().toUtf8().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    dateId = (int) qry.lastInsertId().toInt();

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
 * Function:		getSubAnalysisCoreTier1RatioDataId()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisCoreTier1RatioDataId(int mainAnalysisId,
                                  int dateId,
                                  int &dataId,
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


    str.sprintf("SELECT TblDataTier1RatioSubAnalysis.*   "
                " FROM TblDataTier1RatioSubAnalysis   "
                " WHERE  "
                "       DateTier1RatioId = %d AND "
                "       MainAnalysisId = %d;",
                                                  dateId,
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


            if(rec.value("DataTier1RatioId").isNull() == true)
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
                // qDebug() << rec.value("CoverageRatioDataId").toString();
                // qDebug() << rec.value("MainAnalysisId").toString();

                if(rec.value("DataTier1RatioId").isNull() == false)
                {
                    dataId = rec.value("DataTier1RatioId").toInt();
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
 * Function:    insertSubAnalysisCoreTier1RatioData()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisCoreTier1RatioData(int dateId,
                                    int mainAnalysisId,
                                    int inputDataId,
                                    bool dataIdIsValid,
                                    QString data,
                                    int &dataId,
                                    bool dbIsHandledExternly)
{
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }


    QSqlQuery qry(m_db);


    if(dataIdIsValid == true)
    {
        str.sprintf("INSERT OR REPLACE INTO TblDataTier1RatioSubAnalysis "
                    " (DateTier1RatioId, "
                    "  DataTier1Ratio, "
                    "  DataTier1RatioId,"
                    "  MainAnalysisId) "
                    " VALUES( %d,  '%s', %d, %d);",
                        dateId,
                        data.toLocal8Bit().constData(),
                        inputDataId,
                        mainAnalysisId);
    }
    // New data
    else
    {
        str.sprintf("INSERT INTO TblDataTier1RatioSubAnalysis "
                    " (DateTier1RatioId, "
                    "  DataTier1Ratio, "
                    " MainAnalysisId) "
                    " VALUES( %d, '%s', %d);",
                            dateId,
                            data.toLocal8Bit().constData(),
                            mainAnalysisId);
    }


    qDebug() << str;

    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblDataTier1RatioSubAnalysis"), qry.lastError().text().toLatin1().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    dataId = (int) qry.lastInsertId().toInt();


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
 * Function:		getSubAnalysisCoreTier1RatioData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisCoreTier1RatioData(QString stockName,
                                 QString stockSymbol,
                                 SubAnalysDataST *dataArr,
                                 int &nofArrData,
                                 bool dbIsHandledExternly)
{
    QSqlRecord rec;
    QString str;
    SubAnalysDataST data;

    nofArrData = 0;


    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

  bool found = false;


    str.sprintf("SELECT TblMainAnalysis.*, TblDateTier1RatioSubAnalysis.*, TblDataTier1RatioSubAnalysis.* "
                " FROM TblMainAnalysis, TblDateTier1RatioSubAnalysis, TblDataTier1RatioSubAnalysis "
                " WHERE  "
                "       TblMainAnalysis.MainAnalysisId = TblDateTier1RatioSubAnalysis.MainAnalysisId AND "
                "       TblMainAnalysis.MainAnalysisId = TblDataTier1RatioSubAnalysis.MainAnalysisId AND "
                "       TblDateTier1RatioSubAnalysis.DateTier1RatioId = TblDataTier1RatioSubAnalysis.DateTier1RatioId AND "
                "       lower(TblMainAnalysis.stockName) = lower('%s') AND "
                "       lower(TblMainAnalysis.StockSymbol) = lower('%s') "
                " ORDER BY (CAST(TblDateTier1RatioSubAnalysis.DateTier1Ratio AS REAL)) ASC;",              // DESC";",
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


            if( (rec.value("DateTier1Ratio").isNull() == true) ||  // Date
                (rec.value("DataTier1Ratio").isNull() == true))   // Data
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
                // qDebug() << rec.value("CoverageRatioDate").toString() << "\n";
                // qDebug() << rec.value("stockSymbol").toString();
                // qDebug() << rec.value("stockName").toString();


                // Date
                data.date.clear();
                if(rec.value("DateTier1Ratio").isNull() == false)
                {
                    data.date = rec.value("DateTier1Ratio").toString();
                }

                // Data
                data.data.clear();
                if(rec.value("DataTier1Ratio").isNull() == false)
                {
                    data.data = rec.value("DataTier1Ratio").toString();
                }

                dataArr[nofArrData] = data;
                nofArrData++;
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


//=====================================

/****************************************************************
 *
 * Function:    subAnalysisCoreCapitalRatioDateExists()
 *
 * Description:  (kärnprimärkapitalrelation), Använd detta
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::subAnalysisCoreCapitalRatioDateExists(QString date,
                                                    int mainAnalysisId,
                                                    int &dateId)
{
    QSqlRecord rec;
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    QByteArray ba = date.toLocal8Bit();
    const char *c_date = ba.data();


    str.sprintf("SELECT * "
                " FROM  TblDateCoreCapitalRatioSubAnalysis "
                " WHERE DateCoreCapitalRatio = '%s' AND MainAnalysisId = %d;",
                c_date,
                mainAnalysisId);

    qDebug() << str;


    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblDateTier1RatioSubAnalysis error 1"), qry.lastError().text().toUtf8().constData());
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


            if((rec.value("DateCoreCapitalRatio").isNull() == true) ||
               (true == rec.value("MainAnalysisId").isNull()))
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                dateId = rec.value("DateCoreCapitalRatioId").toInt();
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
 * Function:    insertSubAnalysisCoreCapitalRatioDate()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisCoreCapitalRatioDate(QString date,
                                    int mainAnalysisId,
                                    int &dateId,
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


    str.sprintf("INSERT OR REPLACE INTO TblDateCoreCapitalRatioSubAnalysis "
                "(DateCoreCapitalRatio, MainAnalysisId) "
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
            QMessageBox::critical(NULL, QString::fromUtf8("TblDateCoreCapitalRatioSubAnalysis"), qry.lastError().text().toUtf8().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    dateId = (int) qry.lastInsertId().toInt();

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
 * Function:		getSubAnalysisCoreCapitalRatioDataId()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisCoreCapitalRatioDataId(int mainAnalysisId,
                                  int dateId,
                                  int &dataId,
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


    str.sprintf("SELECT TblDataCoreCapitalRatioSubAnalysis.*   "
                " FROM TblDataCoreCapitalRatioSubAnalysis   "
                " WHERE  "
                "       DateCoreCapitalRatioId = %d AND "
                "       MainAnalysisId = %d;",
                                                  dateId,
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


            if(rec.value("DataCoreCapitalRatioId").isNull() == true)
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
                // qDebug() << rec.value("CoverageRatioDataId").toString();
                // qDebug() << rec.value("MainAnalysisId").toString();

                if(rec.value("DataCoreCapitalRatioId").isNull() == false)
                {
                    dataId = rec.value("DataCoreCapitalRatioId").toInt();
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
 * Function:    insertSubAnalysisCoreCapitalRatioData()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisCoreCapitalRatioData(int dateId,
                                      int mainAnalysisId,
                                      int inputDataId,
                                      bool dataIdIsValid,
                                      QString data,
                                      int &dataId,
                                      bool dbIsHandledExternly)
{
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }


    QSqlQuery qry(m_db);


    if(dataIdIsValid == true)
    {
        str.sprintf("INSERT OR REPLACE INTO TblDataCoreCapitalRatioSubAnalysis "
                    " (DateCoreCapitalRatioId, "
                    "  DataCoreCapitalRatio, "
                    "  DataCoreCapitalRatioId,"
                    "  MainAnalysisId) "
                    " VALUES( %d,  '%s', %d, %d);",
                        dateId,
                        data.toLocal8Bit().constData(),
                        inputDataId,
                        mainAnalysisId);
    }
    // New data
    else
    {
        str.sprintf("INSERT INTO TblDataCoreCapitalRatioSubAnalysis "
                    " (DateCoreCapitalRatioId, "
                    "  DataCoreCapitalRatio, "
                    "  MainAnalysisId) "
                    "  VALUES( %d, '%s', %d);",
                            dateId,
                            data.toLocal8Bit().constData(),
                            mainAnalysisId);
    }


    qDebug() << str;

    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblDataCoreCapitalRatioSubAnalysis"), qry.lastError().text().toLatin1().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    dataId = (int) qry.lastInsertId().toInt();


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
 * Function:		getSubAnalysisCoreCapitalRatioData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisCoreCapitalRatioData(QString stockName,
                                   QString stockSymbol,
                                   SubAnalysDataST *dataArr,
                                   int &nofArrData,
                                   bool dbIsHandledExternly)
{
    QSqlRecord rec;
    QString str;
    SubAnalysDataST data;

    nofArrData = 0;


    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

  bool found = false;


    str.sprintf("SELECT TblMainAnalysis.*, TblDateCoreCapitalRatioSubAnalysis.*, TblDataCoreCapitalRatioSubAnalysis.* "
                " FROM TblMainAnalysis, TblDateCoreCapitalRatioSubAnalysis, TblDataCoreCapitalRatioSubAnalysis "
                " WHERE  "
                "       TblMainAnalysis.MainAnalysisId = TblDateCoreCapitalRatioSubAnalysis.MainAnalysisId AND "
                "       TblMainAnalysis.MainAnalysisId = TblDataCoreCapitalRatioSubAnalysis.MainAnalysisId AND "
                "       TblDateCoreCapitalRatioSubAnalysis.DateCoreCapitalRatioId = TblDataCoreCapitalRatioSubAnalysis.DateCoreCapitalRatioId AND "
                "       lower(TblMainAnalysis.stockName) = lower('%s') AND "
                "       lower(TblMainAnalysis.StockSymbol) = lower('%s') "
                " ORDER BY (CAST(TblDateCoreCapitalRatioSubAnalysis.DateCoreCapitalRatio AS REAL)) ASC;",              // DESC";",
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


            if( (rec.value("DateCoreCapitalRatio").isNull() == true) ||  // Date
                (rec.value("DataCoreCapitalRatio").isNull() == true))   // Data
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
                // qDebug() << rec.value("CoverageRatioDate").toString() << "\n";
                // qDebug() << rec.value("stockSymbol").toString();
                // qDebug() << rec.value("stockName").toString();


                // Date
                data.date.clear();
                if(rec.value("DateCoreCapitalRatio").isNull() == false)
                {
                    data.date = rec.value("DateCoreCapitalRatio").toString();
                }

                // Data
                data.data.clear();
                if(rec.value("DataCoreCapitalRatio").isNull() == false)
                {
                    data.data = rec.value("DataCoreCapitalRatio").toString();
                }

                dataArr[nofArrData] = data;
                nofArrData++;
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


//=================================================
// EquityPerShare



/****************************************************************
 *
 * Function:    subAnalysisEquityDateExists()
 *
 * Description:  (Eget kapital)
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::subAnalysisEquityPerShareDateExists(QString date,
                                          int mainAnalysisId,
                                          int &dateId)
{
    QSqlRecord rec;
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    QByteArray ba = date.toLocal8Bit();
    const char *c_date = ba.data();


    str.sprintf("SELECT * "
                " FROM  TblDateEquityPerShareSubAnalysis "
                " WHERE Date = '%s' AND MainAnalysisId = %d;",
                c_date,
                mainAnalysisId);

    qDebug() << str;


    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblDateEquityPerShareSubAnalysis error 1"), qry.lastError().text().toUtf8().constData());
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


            if((rec.value("Date").isNull() == true) ||
               (true == rec.value("MainAnalysisId").isNull()))
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                dateId = rec.value("DateId").toInt();
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
 * Function:    insertSubAnalysisEquityPerShareDate()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisEquityPerShareDate(QString date,
                            int mainAnalysisId,
                            int &dateId,
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


    str.sprintf("INSERT OR REPLACE INTO TblDateEquityPerShareSubAnalysis "
                "(Date, MainAnalysisId) "
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
            QMessageBox::critical(NULL, QString::fromUtf8("TblDateEquityPerShareSubAnalysis"), qry.lastError().text().toUtf8().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    dateId = (int) qry.lastInsertId().toInt();

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
 * Function:		getSubAnalysisEquityPerShareDataId()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisEquityPerShareDataId(int mainAnalysisId,
                                  int dateId,
                                  int &dataId,
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


    str.sprintf("SELECT TblDataEquityPerShareSubAnalysis.*   "
                " FROM TblDataEquityPerShareSubAnalysis   "
                " WHERE  "
                "       DateId = %d AND "
                "       MainAnalysisId = %d;",
                                                  dateId,
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


            if(rec.value("DataId").isNull() == true)
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
                // qDebug() << rec.value("DataId").toString();
                // qDebug() << rec.value("MainAnalysisId").toString();

                if(rec.value("DataId").isNull() == false)
                {
                    dataId = rec.value("DataId").toInt();
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
 * Function:    insertSubAnalysisEquityPerShareData()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisEquityPerShareData(int dateId,
                            int mainAnalysisId,
                            int inputDataId,
                            bool dataIdIsValid,
                            QString data,
                            int &dataId,
                            bool dbIsHandledExternly)
{
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }


    QSqlQuery qry(m_db);


    if(dataIdIsValid == true)
    {
        str.sprintf("INSERT OR REPLACE INTO TblDataEquityPerShareSubAnalysis "
                    " (DateId, "
                    "  Data, "
                    "  DataId,"
                    "  MainAnalysisId) "
                    " VALUES( %d,  '%s', %d, %d);",
                        dateId,
                        data.toLocal8Bit().constData(),
                        inputDataId,
                        mainAnalysisId);
    }
    // New data
    else
    {
        str.sprintf("INSERT INTO TblDataEquityPerShareSubAnalysis "
                    " (DateId, "
                    "  Data, "
                    "  MainAnalysisId) "
                    "  VALUES( %d, '%s', %d);",
                            dateId,
                            data.toLocal8Bit().constData(),
                            mainAnalysisId);
    }


    qDebug() << str;

    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblDataEquityPerShareSubAnalysis"), qry.lastError().text().toLatin1().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    dataId = (int) qry.lastInsertId().toInt();


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
 * Function:		getSubAnalysisEquityPerShareData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisEquityPerShareData(QString stockName,
                                   QString stockSymbol,
                                   SubAnalysDataST *dataArr,
                                   int &nofArrData,
                                   bool dbIsHandledExternly)
{
    QSqlRecord rec;
    QString str;
    SubAnalysDataST data;

    nofArrData = 0;


    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

  bool found = false;


    str.sprintf("SELECT TblMainAnalysis.*, TblDateEquityPerShareSubAnalysis.*, TblDataEquityPerShareSubAnalysis.* "
                " FROM TblMainAnalysis, TblDateEquityPerShareSubAnalysis, TblDataEquityPerShareSubAnalysis "
                " WHERE  "
                "       TblMainAnalysis.MainAnalysisId = TblDateEquityPerShareSubAnalysis.MainAnalysisId AND "
                "       TblMainAnalysis.MainAnalysisId = TblDataEquityPerShareSubAnalysis.MainAnalysisId AND "
                "       TblDateEquityPerShareSubAnalysis.DateId = TblDataEquityPerShareSubAnalysis.DateId AND "
                "       lower(TblMainAnalysis.stockName) = lower('%s') AND "
                "       lower(TblMainAnalysis.StockSymbol) = lower('%s') "
                " ORDER BY (CAST(TblDateEquityPerShareSubAnalysis.Date AS REAL)) ASC;",              // DESC";",
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


            if( (rec.value("Date").isNull() == true) ||  // Date
                (rec.value("Data").isNull() == true))   // Data
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
                // qDebug() << rec.value("CoverageRatioDate").toString() << "\n";
                // qDebug() << rec.value("stockSymbol").toString();
                // qDebug() << rec.value("stockName").toString();


                // Date
                data.date.clear();
                if(rec.value("Date").isNull() == false)
                {
                    data.date = rec.value("Date").toString();
                }

                // Data
                data.data.clear();
                if(rec.value("Data").isNull() == false)
                {
                    data.data = rec.value("Data").toString();
                }

                dataArr[nofArrData] = data;
                nofArrData++;
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





//=======================================================

/****************************************************************
 *
 * Function:    subAnalysisEquityDateExists()
 *
 * Description:  (Eget kapital)
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::subAnalysisEquityDateExists(QString date,
                                          int mainAnalysisId,
                                          int &dateId)
{
    QSqlRecord rec;
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    QByteArray ba = date.toLocal8Bit();
    const char *c_date = ba.data();


    str.sprintf("SELECT * "
                " FROM  TblDateEquitySubAnalysis "
                " WHERE DateEquity = '%s' AND MainAnalysisId = %d;",
                c_date,
                mainAnalysisId);

    qDebug() << str;


    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblDateEquitySubAnalysis error 1"), qry.lastError().text().toUtf8().constData());
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


            if((rec.value("DateEquity").isNull() == true) ||
               (true == rec.value("MainAnalysisId").isNull()))
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                dateId = rec.value("DateEquityId").toInt();
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
 * Function:    insertSubAnalysisEquityDate()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisEquityDate(QString date,
                            int mainAnalysisId,
                            int &dateId,
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


    str.sprintf("INSERT OR REPLACE INTO TblDateEquitySubAnalysis "
                "(DateEquity, MainAnalysisId) "
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
            QMessageBox::critical(NULL, QString::fromUtf8("TblDateEquitySubAnalysis"), qry.lastError().text().toUtf8().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    dateId = (int) qry.lastInsertId().toInt();

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
 * Function:		getSubAnalysisEquityDataId()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisEquityDataId(int mainAnalysisId,
                                  int dateId,
                                  int &dataId,
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


    str.sprintf("SELECT TblDataEquitySubAnalysis.*   "
                " FROM TblDataEquitySubAnalysis   "
                " WHERE  "
                "       DateEquityId = %d AND "
                "       MainAnalysisId = %d;",
                                                  dateId,
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


            if(rec.value("DataEquityId").isNull() == true)
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
                // qDebug() << rec.value("EquityDataId").toString();
                // qDebug() << rec.value("MainAnalysisId").toString();

                if(rec.value("DataEquityId").isNull() == false)
                {
                    dataId = rec.value("DataEquityId").toInt();
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
 * Function:    insertSubAnalysisEquityData()
 *
 * Description:
 *
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::
insertSubAnalysisEquityData(int dateId,
                            int mainAnalysisId,
                            int inputDataId,
                            bool dataIdIsValid,
                            QString data,
                            int &dataId,
                            bool dbIsHandledExternly)
{
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }


    QSqlQuery qry(m_db);


    if(dataIdIsValid == true)
    {
        str.sprintf("INSERT OR REPLACE INTO TblDataEquitySubAnalysis "
                    " (DateEquityId, "
                    "  DataEquity, "
                    "  DataEquityId,"
                    "  MainAnalysisId) "
                    " VALUES( %d,  '%s', %d, %d);",
                        dateId,
                        data.toLocal8Bit().constData(),
                        inputDataId,
                        mainAnalysisId);
    }
    // New data
    else
    {
        str.sprintf("INSERT INTO TblDataEquitySubAnalysis "
                    " (DateEquityId, "
                    "  DataEquity, "
                    "  MainAnalysisId) "
                    "  VALUES( %d, '%s', %d);",
                            dateId,
                            data.toLocal8Bit().constData(),
                            mainAnalysisId);
    }


    qDebug() << str;

    qry.prepare(str);


    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("TblDataEquitySubAnalysis"), qry.lastError().text().toLatin1().constData());
        }

        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    dataId = (int) qry.lastInsertId().toInt();


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
 * Function:		getSubAnalysisEquityData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CDbHndl::
getSubAnalysisEquityData(QString stockName,
                                   QString stockSymbol,
                                   SubAnalysDataST *dataArr,
                                   int &nofArrData,
                                   bool dbIsHandledExternly)
{
    QSqlRecord rec;
    QString str;
    SubAnalysDataST data;

    nofArrData = 0;


    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

  bool found = false;


    str.sprintf("SELECT TblMainAnalysis.*, TblDateEquitySubAnalysis.*, TblDataEquitySubAnalysis.* "
                " FROM TblMainAnalysis, TblDateEquitySubAnalysis, TblDataEquitySubAnalysis "
                " WHERE  "
                "       TblMainAnalysis.MainAnalysisId = TblDateEquitySubAnalysis.MainAnalysisId AND "
                "       TblMainAnalysis.MainAnalysisId = TblDataEquitySubAnalysis.MainAnalysisId AND "
                "       TblDateEquitySubAnalysis.DateEquityId = TblDataEquitySubAnalysis.DateEquityId AND "
                "       lower(TblMainAnalysis.stockName) = lower('%s') AND "
                "       lower(TblMainAnalysis.StockSymbol) = lower('%s') "
                " ORDER BY (CAST(TblDateEquitySubAnalysis.DateEquity AS REAL)) ASC;",              // DESC";",
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


            if( (rec.value("DateEquity").isNull() == true) ||  // Date
                (rec.value("DataEquity").isNull() == true))   // Data
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
                // qDebug() << rec.value("CoverageRatioDate").toString() << "\n";
                // qDebug() << rec.value("stockSymbol").toString();
                // qDebug() << rec.value("stockName").toString();


                // Date
                data.date.clear();
                if(rec.value("DateEquity").isNull() == false)
                {
                    data.date = rec.value("DateEquity").toString();
                }

                // Data
                data.data.clear();
                if(rec.value("DataEquity").isNull() == false)
                {
                    data.data = rec.value("DataEquity").toString();
                }

                dataArr[nofArrData] = data;
                nofArrData++;
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











#endif
