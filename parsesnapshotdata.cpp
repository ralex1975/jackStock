/*******************************************************************
 *
 * Filename: parsesnapshotdata.cpp
 * Date:     2013-02-03
 *
 * Description:
 *
 *
 *******************************************************************/

#include "parsesnapshotdata.h"
#include <QMessageBox>
#include <qwaitcondition.h>
#include "common.h"
#include "dbHndl.h"
#include "parseYieldData.h"
#include "parsePriceData.h"
#include "parseKeyNumberData.h"




/****************************************************************
 *
 * Function:    ()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
CParseSnapshotData::CParseSnapshotData(QObject *parent):
                                                    QThread(parent)
{

}








/****************************************************************
 *
 * Function:    ()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
void CParseSnapshotData::run()
{
    CTaskQueData data;
    bool finish = false;
    TaskQueToDo_ET toDo = PARSER_IS_IDLE;
    CDbHndl db;
    CParsePriceData ppd;
    CParseYieldData pyd;
    CParseKeyNumberData pknd;
    QString filename;


    qDebug("Open db 2 %s, %d", __FILE__, __LINE__);
    db.openDb(PATH_JACK_STOCK_DB, true);

    while(finish == false)
    {
        if(toDo == PARSER_IS_IDLE)
        {
            qDebug() << "Parser thread is running";

            if(true == m_inputQue.removeFirst(data))
            {
                toDo = data.toDo;
                qDebug() << "Thread data has been received";
            }
            else
            {
                qDebug() << "Thread: is idle";
                this->msleep(500);
            }
        }
        else
        {
            switch(toDo)
            {
            case PARSER_IS_IDLE:
                    this->msleep(100);
                break;

            case PARSE_COMPLETE_SNAPSHOT_DATA:
                qDebug() << "Thread: parse snapshot data";



                db.delAllStockSnapshotData(true);
                db.resetSnapshotRamData(db.m_snapshotStockData, true);

                filename = DWLD_PATH_STOCK_PRICE_FILE;
                //qDebug() << filename;
                if(false == ppd.parseStockPrices(filename, db))
                {
                   qDebug() << "Thread: Fail to parse price data";
                   qDebug("Open db 2a %s, %d", __FILE__, __LINE__);
                   db.closeDb(true);
                   return;
                }

                filename = DWLD_PATH_STOCK_YIELD_FILE;
                pyd.parseStockYields(filename, db, CDbHndl::DB_MODE_UPDATE_DATA);

                filename = DWLD_PATH_STOCK_KEY_NO_FILE;
                //qDebug() << filename;
                pknd.parseStockKeyNumberData(filename, db, CDbHndl::DB_MODE_UPDATE_DATA);

                db.addCustomerSnabshotData(true);
                // qDebug("Open db 2b %s, %d", __FILE__, __LINE__);
                db.closeDb();
                emit sendParserThreadFinish();
                toDo = PARSER_IS_IDLE;
                finish = true;
                qDebug() << "Thread: Finish parse snapshot data";
                break;
            default:
                this->msleep(100);
            }
        }
    }
    // db.closeDb(true);



}




/****************************************************************
 *
 * Function:    addQueData()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CParseSnapshotData::addQueData(CTaskQueData data)
{
    return m_inputQue.addDataLast(data);
}

