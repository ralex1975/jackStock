#include "nordnetparsecompanyid.h"

#include <QMessageBox>
#include <QtGui>
#include <QtCore>
#include <QString>
#include <QObject>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlError>
#include "dbHndl.h"

//#define PRICE_MAX_NOF_STOCK_DATA 10
#define TAG_START_COMPANY_NAME QObject::tr("&marketplace=11\" class=\"underline\">")
#define TAG_START_COMPANY_NAME1 QObject::tr("&marketplace=14\" class=\"underline\">")
#define TAG_START_COMPANY_NAME2 QObject::tr("&marketplace=15\" class=\"underline\">")
#define TAG_START_COMPANY_NAME3 QObject::tr("&marketplace=24\" class=\"underline\">")


#define TAG_START_COMPANY_NAME_LEN strlen("&marketplace=11\" class=\"underline\">")
#define TAG_END_COMPANY_NAME QObject::tr("</a></div></td>")


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
nordnetParseCompanyId::
nordnetParseCompanyId(QObject *parent) : QObject(parent),
                                        m_waitOnServerResp(false)
{

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
 ****************************************************************/
void nordnetParseCompanyId::reqHtmlFile(QString path, QUrl url)
{
    if(m_waitOnServerResp == true)
    {
        QMessageBox::information(NULL, QString::fromUtf8("Hämta data"), QString::fromUtf8("Vänta: Upptagen med hämta data"));
        return;
    }

    QObject::connect(&m_hw1, SIGNAL(sendSignalTextToDlg2(int)), this, SLOT(slotHtmlPageIsRecv(int)));

    m_hw1.startRequest(url, path, 0x01);
    m_waitOnServerResp = true;
}



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
bool nordnetParseCompanyId::readFile(QString filename)
{
    QFile file(filename);
    QString errStr = QString("Fail to open file: %1").arg(filename);


    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::critical(NULL, QObject::tr("Fail to open file"), errStr);
        return false;
    }

    QTextStream inStream(&file);
    inStream.setCodec("ISO 8859-1");


    while(!inStream.atEnd())
    {
        (void)parseCompanyIdAndName(inStream);
    }

    file.close();
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
bool nordnetParseCompanyId::
parseCompanyIdAndName(QTextStream &inStream)
{
    QString str;
    int index;
    QString assetId;
    QString assetName;
    CDbHndl db;

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
        assetName = str.right(str.length()- index - TAG_START_COMPANY_NAME_LEN);

        assetId = str.left(str.length()- TAG_START_COMPANY_NAME_LEN);

        QString substring("			<td class=\"text\"><div class=\"truncate\"><a href=\"/mux/web/marknaden/aktiehemsidan/index.html?identifier=");

        assetId = str.right(str.length() - substring.length());

        assetId = assetId.left(assetId.indexOf("&"));

        // qDebug() << "assetId =" << assetId;

        index = assetName.indexOf(TAG_END_COMPANY_NAME);
        if(index > -1)
        {
            assetName = assetName.left(index);
            assetName = assetName.trimmed();

            // qDebug() << "assetName =" << assetName << "\n\n";
            db.insertNordnetStockIdData(assetId, assetName);
            return true;
        }
    }
    return false;
}





/*******************************************************************
 *
 * Function:    slotHtmlPageIsRecv()
 *
 * Description: This function is invoked when a html page is
 *              completely received.
 *
 *
 *******************************************************************/
void nordnetParseCompanyId::slotHtmlPageIsRecv(int number)
{
    number = number;
    m_waitOnServerResp = false;

    QMessageBox::information(NULL, QObject::tr("Hämta data"), QString::fromUtf8("Filen hämtad"));
}
