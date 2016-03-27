#include "importYahooTaData.h"
#include <QMessageBox>
#include "common.h"
#include "parseTaData.h"

/*******************************************************************
 *
 * Function:    ()
 *
 * Description: Constructor
 *
 *
 *
 *******************************************************************/
importYahooTaData::importYahooTaData(QObject *parent) :
    QThread(parent),
    m_importInfoIsSet(false),
    m_stateImportTaData(STATE_IDLE)
{

}

/*******************************************************************
 *
 * Function:    setImportInfo()
 *
 * Description: This function is used to request and parse data from
 *              a number of stocks. All stock symbols are stored
 *              in the file with the path: filename.
 *
 *******************************************************************/
void importYahooTaData::setImportInfo(QString filename)
{
    m_importInfoIsSet = true;
    m_processSingleStock = false;

    m_filename = filename;
}


/*******************************************************************
 *
 * Function:    setImportInfoSingleStock()
 *
 * Description: This function is used to request and parse data from
 *              only one stock.
 *
 *
 *******************************************************************/
void importYahooTaData::
setImportInfoSingleStock(QString stockName, QString stockSymbol)
{
    m_importInfoIsSet = true;
    m_processSingleStock = true;

    m_stockName = stockName;
    m_stockSymbol = stockSymbol;
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
void importYahooTaData::run()
{
    QFile *file;
    QTextStream *inStream;
    bool finish = false;


    // Have we got all data we need from user?
    if(m_importInfoIsSet == false)
    {
        emit emitImportYahooTaDataIsFinish(ERR_FAIL_USER_INPUT_DATA_MISSING);
        return;
    }


    if(m_processSingleStock == true)
    {
        m_stateImportTaData = STATE_PARSE_DATA;

        while(finish == false)
        {
            switch(m_stateImportTaData)
            {
            default:
            case STATE_IDLE:
                break;

            case STATE_PARSE_DATA:
                parseTaDataOnFile(m_stockName, m_stockSymbol);
                emit emitImportSingelStockDataIsFinish(1);
                m_stateImportTaData = STATE_IDLE;
                finish = true;
                break;
            }
        }
    }
    else
    {
        m_stateImportTaData = STATE_OPEN_ASSET_FILE;

        while(finish == false)
        {

            switch(m_stateImportTaData)
            {
            case STATE_OPEN_ASSET_FILE:
                if(false == openAssetFile(m_filename, &file, &inStream))
                {
                    emit emitImportYahooTaDataIsFinish(ERR_FAIL_OPEN_SYM_FILE);
                    return;
                }
                m_stateImportTaData = STATE_RETRIEVE_ASSET_NAME;
                break;

            case STATE_RETRIEVE_ASSET_NAME:
                 if(false == retreiveAssetName(m_stockSymbol, m_stockName, inStream))
                 {
                     closeAssetFile(&file, &inStream);
                     m_stateImportTaData = STATE_IDLE;
                     emit emitImportYahooTaDataIsFinish(ERR_OK);
                     return;
                 }

                 qDebug() << m_stockSymbol << m_stockName << "\n";
                 emit emitRequestYahooTaDataFromServer();
                 m_stateImportTaData = STATE_WAIT_ON_SERVER_DATA;
                break;

            case STATE_WAIT_ON_SERVER_DATA:
                qDebug() <<  "waiting \n";
                this->msleep(100);
                break;

            case STATE_PARSE_DATA:
                parseTaDataOnFile(m_stockName, m_stockSymbol);
                m_stateImportTaData = STATE_RETRIEVE_ASSET_NAME;
                break;

            case STATE_IDLE:
            default:
                break;
            }
        }
    }
}





/*******************************************************************
 *
 * Function:    openAssetFile()
 *
 * Description: This function open the file that contains the
 *              assets that shall be imported from yahoo:
 *              abb.st
 *
 *
 *******************************************************************/
 bool importYahooTaData::openAssetFile(QString filename, QFile **file, QTextStream **inStream)
{
    (*file) = new QFile(filename);

    if(!(*file)->open(QFile::ReadOnly | QFile::Text))
    {
        QString errStr = QString("Fail to open file: %1").arg(filename);
        QMessageBox::critical(NULL, QString::fromUtf8("Fail to open file"), errStr);
        return false;
    }

    (*inStream) = new QTextStream((*file));

    return true;
}


 /*******************************************************************
  *
  * Function:    importYahooTaData()
  *
  * Description:
  *
  *
  *
  *******************************************************************/
 bool importYahooTaData::
 retreiveAssetName(QString &stockSymbol, QString &stockName, QTextStream *inStream)
 {
      QString str;

      if(inStream->atEnd())
      {
          return false;
      }


      str = inStream->readLine(); //.toUtf8();
      qDebug() << str;

      if(str.length() < 1)
          return false;


      // Get stock symbol
      if(str.indexOf("\t") != -1)
      {
          stockSymbol = str.left(str.indexOf("\t"));
          qDebug() << stockSymbol;
      }
      else
      {
          return false;
      }

      // Stockname
      if(str.indexOf("\t") != -1)
      {
          stockName = str.right(str.length() - str.indexOf("\t")-1);
          qDebug() << stockName;
      }
      else
      {
          return false;
      }

    return true;
 }



/*******************************************************************
 *
 * Function:    openAssetFile()
 *
 * Description: This function open the file that contains the
 *              assets that shall be imported from yahoo:
 *              abb.st
 *
 *
 *******************************************************************/
void importYahooTaData::closeAssetFile(QFile **file, QTextStream **inStream)
{

    (*file)->close();
    delete (*file);
    delete (*inStream);
}




/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void importYahooTaData::parseTaDataOnFile(QString stockName, QString stockSymbol)
{
    CParseTaData ptd;
    QString filename = DWLD_PATH_TA_LIST_FILE;

    ptd.parseData(filename, stockName, stockSymbol);
}

