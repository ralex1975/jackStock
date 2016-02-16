/*******************************************************************
 *
 * Filename: tableTab.h
 * Date:     2012-07-08
 *
 * Description: Tab1 page
 *
 *
 *******************************************************************/

#include "tableTab.h"
#include "ui_tableTab.h"
#include <QtNetwork/QSslSocket>
#include <QDebug>
#include <QtCore>
#include <QtGui>
#include "dbHndl.h"
#include "inc/guiUtil/guiUtils.h"
#include <QObject>

#include "parsePriceData.h"
#include "parseYieldData.h"
#include "parseKeyNumberData.h"
#include <QtCore>
#include <QMessageBox>


CTableTab::CTableTab(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CTableTab)
{
    CDbHndl db;

    ui->setupUi(this);


    m_filesReceived = 0;

    m_parserThread = new CParseSnapshotData(this);

    if(!m_parserThread)
        QMessageBox::critical(NULL, QString::fromUtf8("Create thread"), QString::fromUtf8("Fail to create thread"));

    connect(m_parserThread, SIGNAL(sendParserThreadFinish()), this, SLOT(recvParserThreadIsFinish()));



    initComboCountrySel(ui->SelDataCountryComboBox);
    ui->largeCapCheckBox->setChecked(true);
    ui->midCapCheckBox->setChecked(true);
    ui->smallCapCheckBox->setChecked(false);


    m_filterCtrls.compareComboBox[CDbHndl::TTAB_GUI_CTRL1] = ui->compareParam1ComboBox;
    m_filterCtrls.compareComboBox[CDbHndl::TTAB_GUI_CTRL2] = ui->compareParam2ComboBox;
    m_filterCtrls.compareComboBox[CDbHndl::TTAB_GUI_CTRL3] = ui->compareParam3ComboBox;
    m_filterCtrls.compareComboBox[CDbHndl::TTAB_GUI_CTRL4] = ui->compareParam4ComboBox;
    m_filterCtrls.compareComboBox[CDbHndl::TTAB_GUI_CTRL5] = ui->compareParam5ComboBox;
    m_filterCtrls.compareComboBox[CDbHndl::TTAB_GUI_CTRL6] = ui->compareParam6ComboBox;
    m_filterCtrls.compareComboBox[CDbHndl::TTAB_GUI_CTRL7] = ui->compareParam7ComboBox;

    m_filterCtrls.indicatorComboBox[CDbHndl::TTAB_GUI_CTRL1] = ui->indicator1ComboBox;
    m_filterCtrls.indicatorComboBox[CDbHndl::TTAB_GUI_CTRL2] = ui->indicator2ComboBox;
    m_filterCtrls.indicatorComboBox[CDbHndl::TTAB_GUI_CTRL3] = ui->indicator3ComboBox;
    m_filterCtrls.indicatorComboBox[CDbHndl::TTAB_GUI_CTRL4] = ui->indicator4ComboBox;
    m_filterCtrls.indicatorComboBox[CDbHndl::TTAB_GUI_CTRL5] = ui->indicator5ComboBox;
    m_filterCtrls.indicatorComboBox[CDbHndl::TTAB_GUI_CTRL6] = ui->indicator6ComboBox;
    m_filterCtrls.indicatorComboBox[CDbHndl::TTAB_GUI_CTRL7] = ui->indicator7ComboBox;

    m_filterCtrls.IndicatorValueLineEdit[CDbHndl::TTAB_GUI_CTRL1] = ui->indicatorValue1LineEdit;
    m_filterCtrls.IndicatorValueLineEdit[CDbHndl::TTAB_GUI_CTRL2] = ui->indicatorValue2LineEdit;
    m_filterCtrls.IndicatorValueLineEdit[CDbHndl::TTAB_GUI_CTRL3] = ui->indicatorValue3LineEdit;
    m_filterCtrls.IndicatorValueLineEdit[CDbHndl::TTAB_GUI_CTRL4] = ui->indicatorValue4LineEdit;
    m_filterCtrls.IndicatorValueLineEdit[CDbHndl::TTAB_GUI_CTRL5] = ui->indicatorValue5LineEdit;
    m_filterCtrls.IndicatorValueLineEdit[CDbHndl::TTAB_GUI_CTRL6] = ui->indicatorValue6LineEdit;
    m_filterCtrls.IndicatorValueLineEdit[CDbHndl::TTAB_GUI_CTRL7] = ui->indicatorValue7LineEdit;

    m_horizHeader= ui->tableView->horizontalHeader();
    setTableWidth(10);
    connect(m_horizHeader, SIGNAL(sectionClicked(int)), this, SLOT(sectionClicked1(int)));


    // Init filter comboBoxes
    //qDebug("Open db 108 %s, %d", __FILE__, __LINE__);
    //db.openDb(PATH_JACK_STOCK_DB);

    db.initSortCompareToComboBoxes(&m_filterCtrls);
    db.initIndicatorToComboBoxes(&m_filterCtrls);
    (void)db.dbDataSetFilterComboSel(&m_filterCtrls);
    //db.closeDb();
    // qDebug("Close db 108 %s, %d", __FILE__, __LINE__);

    //bool isSSLSupported = QSslSocket::supportsSsl();
    //qDebug() << "isSSLSupported=  " << isSSLSupported << "\n";


    m_sortWithoutfilter = false;
}








/*******************************************************************
 *
 * Function:    ~CTableTab()
 *
 * Description: Constructor
 *
 *
 *
 *******************************************************************/
CTableTab::~CTableTab()
{
    delete ui;
}




/*******************************************************************
 *
 * Function:    sectionClicked1()
 *
 * Description: Invoked when table header is clicked
 *
 *
 *
 *******************************************************************/
void CTableTab::sectionClicked1(int logicalIndex)
{
    CDbHndl db;
    QString sortParam;
    static bool toggle = false;

    if(m_sortWithoutfilter == true)
    {
        if(true == toggle)
        {
            toggle = false;
            ui->tableView->sortByColumn(logicalIndex, Qt::AscendingOrder);
        }
        else
        {
            toggle = true;
            ui->tableView->sortByColumn(logicalIndex, Qt::DescendingOrder);
        }
    }
    else
    {
        //logicalIndex = logicalIndex;
        //QMessageBox::information(NULL, QObject::tr("Header clicked"), QObject::tr("Header clicked") );


        if(true == db.filter1DividendGetColumnSortParameter(logicalIndex, sortParam))
        {
            // qDebug("Open db 7 %s, %d", __FILE__, __LINE__);
            // db.openDb(PATH_JACK_STOCK_DB);
            db.createTabelTabFilter(m_filterCtrls);

            //QMessageBox::information(NULL, QObject::tr("sortOrder"), m_tableHeaderList[logicalIndex].sortOrder );


            db.debugPrintFilter();


            if(m_tableHeaderList[logicalIndex].sortOrder.compare(QObject::tr(SQL_STR_DESC)) ==  0)
            {
               m_tableHeaderList[logicalIndex].sortOrder = SQL_STR_ASC;
               //QMessageBox::information(NULL, QObject::tr(SQL_STR_ASC), m_tableHeaderList[logicalIndex].sortOrder);
            }
            else if(m_tableHeaderList[logicalIndex].sortOrder.compare(QObject::tr(SQL_STR_ASC)) ==  0)
            {
                m_tableHeaderList[logicalIndex].sortOrder = SQL_STR_DESC;
                //QMessageBox::information(NULL, QObject::tr(SQL_STR_DESC), m_tableHeaderList[logicalIndex].sortOrder);
            }
            else
            {
                m_tableHeaderList[logicalIndex].sortOrder = SQL_STR_DESC;
                QMessageBox::information(NULL, QObject::tr("SQL_STR_?"), m_tableHeaderList[logicalIndex].sortOrder);
            }


            db.filter1Dividend(this, ui->tableView, &m_filterCtrls, sortParam, m_tableHeaderList[logicalIndex].sortOrder, true);



            // Remove all data from filter combo boxes
            db.delAllFilterStockSnapshotData();

            // Update combo box with fresh data
            (void)db.insertFilterDataInDb();

            // Set Filter Combo Box last Selection
            (void)db.dbDataSetFilterComboSel(&m_filterCtrls);

            // db.closeDb();
            // qDebug("Close db 7 %s, %d", __FILE__, __LINE__);
    }
    else
    {
       QMessageBox::information(NULL, QObject::tr("sortParam"), QObject::tr("Error: fail to sort") );
    }

    }

}







/*******************************************************************
 *
 * Function:    on_pushButton_clicked()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void CTableTab::on_pushButton_clicked()
{
    QString filename1 = DWLD_PATH_STOCK_PRICE_FILE;
    QString filename2 = DWLD_PATH_STOCK_KEY_NO_FILE;
    QString filename3 = DWLD_PATH_STOCK_YIELD_FILE;
    QUrl url;
    QUrl ur2;
    QUrl ur3;
    int result = 0;

    if(m_filesReceived != 0)
    {
        QMessageBox::information(NULL, QString::fromUtf8("Hämta data"), QString::fromUtf8("Vänta: Upptagen med hämta data"));
        return;
    }

    QMessageBox::information(NULL, QString::fromUtf8("Hämta data"), (QString) QString::fromUtf8("Hämtar data via thread"));


    getSelectionDataToParse(ui->SelDataCountryComboBox, ui->largeCapCheckBox, ui->midCapCheckBox, ui->smallCapCheckBox, result);

    if(true == getUrlToSnapshopParseData(result, url, ur2, ur3))
    {
        QObject::connect(&m_hw1, SIGNAL(sendSignalTextToDlg2(int)), this, SLOT(recvHtmlPageIsDownloaded(int)));
        QObject::connect(&m_hw2, SIGNAL(sendSignalTextToDlg2(int)), this, SLOT(recvHtmlPageIsDownloaded(int)));
        QObject::connect(&m_hw3, SIGNAL(sendSignalTextToDlg2(int)), this, SLOT(recvHtmlPageIsDownloaded(int)));

        //QMessageBox::information(this, tr("HTTP"), QString::fromUtf8("Börjar Hämta data"));
        m_hw1.startRequest(url, filename1, 0x01);
        m_hw2.startRequest(ur2, filename2, 0x02);
        m_hw3.startRequest(ur3, filename3, 0x04);
        m_filesReceived = 0;
    }

}


/*******************************************************************
 *
 * Function:    recvHtmlPageIsDownloaded()
 *
 * Description: This function is invoked when a html page is
 *              completely received.
 *
 *
 *******************************************************************/
void CTableTab::recvHtmlPageIsDownloaded(int number)
{
    CTaskQueData data;

    m_filesReceived |= number;

    if(m_filesReceived == (0x01 | 0x02 | 0x04))
    {
        data.toDo = PARSE_COMPLETE_SNAPSHOT_DATA;
        data.priority = QUE_PRIO_3;

         m_parserThread->addQueData(data);
         m_parserThread->start(QThread::HighestPriority);
         m_filesReceived = 0xff;
   }

}


/*******************************************************************
 *
 * Function:    recvParserThreadIsFinish()
 *
 * Description: This function is invoked when parser thread is finished
 *
 *
 *
 *******************************************************************/
void CTableTab::recvParserThreadIsFinish(void)
{
    CDbHndl db;
    db.initFilter1Dividend(this, ui->tableView, &m_filterCtrls);
    m_filesReceived = 0;
}

/*******************************************************************
 *
 * Function:    on_FilterData_clicked()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void CTableTab::on_FilterData_clicked()
{
    CDbHndl db;

    db.createTabelTabFilter(m_filterCtrls);

    if(false == db.filter1Dividend(this, ui->tableView, &m_filterCtrls))
    {
        QMessageBox::information(NULL, QObject::tr("Filter fel"), "Inget filter kunde skapas");
        return;
    }


    // Remove all data from filter combo boxes
    db.delAllFilterStockSnapshotData();

    // Update combo box with fresh data
    (void)db.insertFilterDataInDb();

    // Set Filter Combo Box last Selection
    (void)db.dbDataSetFilterComboSel(&m_filterCtrls);

    m_sortWithoutfilter = true;

}






/*******************************************************************
 *
 * Function:    on_GetDbDataButton_clicked()
 *
 * Description: Get data from file. This is the last received data.
 *
 *
 *
 *******************************************************************/
void CTableTab::on_GetDbDataButton_clicked()
{
    CDbHndl db;

    db.openDb(PATH_JACK_STOCK_DB);
    bool dbIsHandledExternly = true;
    db.delAllStockSnapshotData(dbIsHandledExternly);
    db.resetSnapshotRamData(db.m_snapshotStockData, dbIsHandledExternly);

    CParsePriceData ppd;
    CParseYieldData pyd;
    CParseKeyNumberData pknd;
    QString filename;


    filename = DWLD_PATH_STOCK_PRICE_FILE;
    ppd.parseStockPrices(filename, db);

    filename = DWLD_PATH_STOCK_YIELD_FILE;
    pyd.parseStockYields(filename, db, CDbHndl::DB_MODE_UPDATE_DATA);

    filename = DWLD_PATH_STOCK_KEY_NO_FILE;
    pknd.parseStockKeyNumberData(filename, db, CDbHndl::DB_MODE_UPDATE_DATA);
    db.addCustomerSnabshotData(true);
    db.initFilter1Dividend(this, ui->tableView, &m_filterCtrls);

    db.closeDb(true);
}


/*******************************************************************
 *
 * Function:    on_GetDbDataButton_2_clicked()
 *
 * Description: Get latest stored stock info from db
 *
 *
 *
 *******************************************************************/
void CTableTab::on_GetDbDataButton_2_clicked()
{
    CDbHndl db;

    m_sortWithoutfilter = true;

    db.createTabelTabFilter(m_filterCtrls);

    db.addCustomerSnabshotData();
    db.initFilter1Dividend(this, ui->tableView, &m_filterCtrls);
    (void)db.dbDataSetFilterComboSel(&m_filterCtrls);

}



/*******************************************************************
 *
 * Function:    initComboCountrySel()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void CTableTab::initComboCountrySel(QComboBox *combo)
{
    combo->addItem("Sverige");
    combo->addItem("Finland");
    combo->addItem("Danmark");
    combo->addItem("Norge");
    combo->addItem("Norden");
    combo->setCurrentIndex(0);

}


/*******************************************************************
 *
 * Function:    getUrlToSnapshopParseData()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
bool CTableTab::
getUrlToSnapshopParseData(int result, QUrl &url, QUrl &ur2, QUrl &ur3)
{

    QString page1;
    QString page2;
    QString page3;


    switch((result & 0x00FF))
    {
    case PDATASEL_SWEDEN:

        switch(result & 0x0F00)
        {
        case (PDATASEL_LARGE_CAP | PDATASEL_MID_CAP):
            page1 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Sverige&lista=1_1&large=on&mid=on&sektor=0&subtyp=price&sortera=&sorteringsordning=";
            page2 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Sverige&lista=1_1&large=on&mid=on&sektor=0&subtyp=key_ratios&sortera=&sorteringsordning=";
            page3 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Sverige&lista=1_1&large=on&mid=on&sektor=0&subtyp=historic_return&sortera=&sorteringsordning=";
            break;

        case PDATASEL_LARGE_CAP:
            page1 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Sverige&lista=1_1&large=on&sektor=0&subtyp=price&sortera=&sorteringsordning=";
            page2 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Sverige&lista=1_1&large=on&sektor=0&subtyp=key_ratios&sortera=&sorteringsordning=";
            page3 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Sverige&lista=1_1&large=on&sektor=0&subtyp=historic_return&sortera=&sorteringsordning=";
            break;

        case PDATASEL_MID_CAP:
            page1 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Sverige&lista=1_1&mid=on&sektor=0&subtyp=price&sortera=&sorteringsordning=";
            page2 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Sverige&lista=1_1&mid=on&sektor=0&subtyp=key_ratios&sortera=&sorteringsordning=";
            page3 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Sverige&lista=1_1&mid=on&sektor=0&subtyp=historic_return&sortera=&sorteringsordning=";
            break;

        case PDATASEL_SMALL_CAP:
            QMessageBox::information(NULL, QObject::tr("Selection"), QObject::tr("Not implemented"));
            break;
        }

        break;

    case PDATASEL_FINLAND:
        switch(result & 0x0F00)
        {
        case (PDATASEL_LARGE_CAP | PDATASEL_MID_CAP):
            page1 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Finland&lista=1_1&large=on&mid=on&sektor=0&subtyp=price&sortera=aktie&sorteringsordning=";
            page2 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Finland&lista=1_1&large=on&mid=on&sektor=0&subtyp=key_ratios&sortera=&sorteringsordning=";
            page3 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Finland&lista=1_1&large=on&mid=on&sektor=0&subtyp=historic_return&sortera=&sorteringsordning=";
            break;

        case PDATASEL_LARGE_CAP:
            page1 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Finland&lista=1_1&large=on&sektor=0&subtyp=price&sortera=&sorteringsordning=";
            page2 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Finland&lista=1_1&large=on&sektor=0&subtyp=key_ratios&sortera=&sorteringsordning=";
            page3 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Finland&lista=1_1&large=on&sektor=0&subtyp=historic_return&sortera=&sorteringsordning=";
            break;

       case PDATASEL_MID_CAP:
            page1 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Finland&lista=1_1&mid=on&sektor=0&subtyp=price&sortera=aktie&sorteringsordning=";
            page2 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Finland&lista=1_1&mid=on&sektor=0&subtyp=key_ratios&sortera=&sorteringsordning=";
            page3 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Finland&lista=1_1&mid=on&sektor=0&subtyp=historic_return&sortera=&sorteringsordning=";
            break;

        case PDATASEL_SMALL_CAP:
            QMessageBox::information(NULL, QObject::tr("Selection"), QObject::tr("Not implemented"));
            break;

        }
        break;



    case PDATASEL_NORWAY:
         switch(result & 0x0F00)
        {
        case (PDATASEL_LARGE_CAP | PDATASEL_MID_CAP):
             page1 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Norge&lista=1_1&large=on&mid=on&sektor=0&subtyp=price&sortera=&sorteringsordning=";
             page2 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Norge&lista=1_1&large=on&mid=on&sektor=0&subtyp=key_ratios&sortera=&sorteringsordning=";
             page3 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Norge&lista=1_1&large=on&mid=on&sektor=0&subtyp=historic_return&sortera=&sorteringsordning=";
            break;

        case PDATASEL_LARGE_CAP:
            page1 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Norge&lista=1_1&large=on&sektor=0&subtyp=price&sortera=&sorteringsordning=";
            page2 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Norge&lista=1_1&large=on&sektor=0&subtyp=key_ratios&sortera=&sorteringsordning=";
            page3 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Norge&lista=1_1&large=on&sektor=0&subtyp=historic_return&sortera=&sorteringsordning=";
            break;

       case PDATASEL_MID_CAP:
            page1 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Norge&lista=1_1&mid=on&sektor=0&subtyp=price&sortera=&sorteringsordning=";
            page2 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Norge&lista=1_1&mid=on&sektor=0&subtyp=key_ratios&sortera=&sorteringsordning=";
            page3 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Norge&lista=1_1&mid=on&sektor=0&subtyp=historic_return&sortera=&sorteringsordning=";
            break;

        case PDATASEL_SMALL_CAP:
            QMessageBox::information(NULL, QObject::tr("Selection"), QObject::tr("Not implemented"));
            break;
        }
        break;

    case PDATASEL_DENMARK:
        switch(result & 0x0F00)
       {
       case (PDATASEL_LARGE_CAP | PDATASEL_MID_CAP):
           page1 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Danmark&lista=1_1&large=on&mid=on&sektor=0&subtyp=price&sortera=aktie&sorteringsordning=stigande";
           page2 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Danmark&lista=1_1&large=on&mid=on&sektor=0&subtyp=key_ratios&sortera=&sorteringsordning=";
           page3 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Danmark&lista=1_1&large=on&mid=on&sektor=0&subtyp=historic_return&sortera=&sorteringsordning=";
           break;

       case PDATASEL_LARGE_CAP:
           page1 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Danmark&lista=1_1&large=on&sektor=0&subtyp=price&sortera=&sorteringsordning=";
           page2 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Danmark&lista=1_1&large=on&sektor=0&subtyp=key_ratios&sortera=&sorteringsordning=";
           page3 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Danmark&lista=1_1&large=on&sektor=0&subtyp=historic_return&sortera=&sorteringsordning=";
           break;

      case PDATASEL_MID_CAP:
           page1 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Danmark&lista=1_1&mid=on&sektor=0&subtyp=price&sortera=&sorteringsordning=";
           page2 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Danmark&lista=1_1&mid=on&sektor=0&subtyp=key_ratios&sortera=&sorteringsordning=";
           page3 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Danmark&lista=1_1&mid=on&sektor=0&subtyp=historic_return&sortera=&sorteringsordning=";
           break;

       case PDATASEL_SMALL_CAP:
           QMessageBox::information(NULL, QObject::tr("Selection"), QObject::tr("Not implemented"));
           break;
       }
       break;

    case PDATASEL_NORDIC:
        switch(result & 0x0F00)
        {
        case (PDATASEL_LARGE_CAP | PDATASEL_MID_CAP):
            page1 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Norden&Sverige=on&Finland=on&Norge=on&Danmark=on&large=on&mid=on&sektor=0&subtyp=price&sortera=aktie&sorteringsordning=";
            page2 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Norden&Sverige=on&Finland=on&Norge=on&Danmark=on&large=on&mid=on&sektor=0&subtyp=key_ratios&sortera=&sorteringsordning=";
            page3 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Norden&Sverige=on&Finland=on&Norge=on&Danmark=on&large=on&mid=on&sektor=0&subtyp=historic_return&sortera=&sorteringsordning=";
            break;

        case PDATASEL_LARGE_CAP:
            page1 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Norden&Sverige=on&Finland=on&Norge=on&Danmark=on&large=on&sektor=0&subtyp=price&sortera=aktie&sorteringsordning=stigande";
            page2 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Norden&Sverige=on&Finland=on&Norge=on&Danmark=on&large=on&sektor=0&subtyp=key_ratios&sortera=&sorteringsordning=";
            page3 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Norden&Sverige=on&Finland=on&Norge=on&Danmark=on&large=on&sektor=0&subtyp=historic_return&sortera=&sorteringsordning=";
            break;

       case PDATASEL_MID_CAP:
            page1 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Norden&Sverige=on&Finland=on&Norge=on&Danmark=on&mid=on&sektor=0&subtyp=price&sortera=&sorteringsordning=";
            page2 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Norden&Sverige=on&Finland=on&Norge=on&Danmark=on&mid=on&sektor=0&subtyp=key_ratios&sortera=&sorteringsordning=";
            page3 = "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Norden&Sverige=on&Finland=on&Norge=on&Danmark=on&mid=on&sektor=0&subtyp=historic_return&sortera=&sorteringsordning=";
            break;

        case PDATASEL_SMALL_CAP:
            QMessageBox::information(NULL, QObject::tr("Selection"), QObject::tr("Not implemented"));
            break;
        }

        break;

    default:
        QMessageBox::information(NULL, QObject::tr("Selection"), QObject::tr("Invalid selection"));
        return false;
    }

    url = page1;
    ur2 = page2;
    ur3 = page3;

    return true;
}



/*******************************************************************
 *
 * Function:    getSelectionDataToParse()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void CTableTab::
getSelectionDataToParse(QComboBox *combo, QCheckBox *largeCap, QCheckBox *midCap, QCheckBox *smallCap, int &result)
{
    QString countrySel;
    int chkBoxSel = PDATESEL_NOT_INIT;


    result = PDATESEL_NOT_INIT;


    countrySel = combo->currentText();

    if(countrySel.compare("Sverige")== 0)
    {
        result |= PDATASEL_SWEDEN;
    }

    if(countrySel.compare("Finland")== 0)
    {
        result |= PDATASEL_FINLAND;
    }

    if(countrySel.compare("Norge")== 0)
    {
        result |= PDATASEL_NORWAY;
    }

    if(countrySel.compare("Danmark")== 0)
    {
        result |= PDATASEL_DENMARK;
    }

    if(countrySel.compare("Norden")== 0)
    {
        result |= PDATASEL_NORDIC;
    }

    if(result == PDATASEL_INVALID_SEL)
    {
        result = PDATASEL_INVALID_SEL;
    }


    chkBoxSel = PDATESEL_NOT_INIT;

    if(largeCap->isChecked() == true)
    {
        chkBoxSel |= PDATASEL_LARGE_CAP;
    }

    if(midCap->isChecked() == true)
    {
        chkBoxSel |= PDATASEL_MID_CAP;
    }

    if(smallCap->isChecked() == true)
    {
        chkBoxSel |= PDATASEL_SMALL_CAP;
    }


    if(chkBoxSel == PDATASEL_INVALID_SEL)
    {
        chkBoxSel = PDATASEL_INVALID_SEL;
    }

    result |= chkBoxSel;
}








/*******************************************************************
 *
 * Function:    setTableWidth()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void CTableTab::setTableWidth(int nofColumns)
{
    int totColumnWith = 0;
    int i;

    for(i = 0; i < nofColumns; i++)
    {
       totColumnWith += ui->tableView->columnWidth(i);
    }

    QRect rect = ui->tableView->geometry();
    rect.setWidth(2 + ui->tableView->verticalHeader()->width() + totColumnWith);
    ui->tableView->setGeometry(rect);
}

