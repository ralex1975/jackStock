#include "TADlg.h"
#include "ui_TADlg.h"
#include "dbHndl.h"
#include "util.h"

extern bool m_crumb_failed;

const CTADlg::TimePeriodDays_ST CTADlg::m_timePeriodDaysArr[CTADlg::MAX_NOF_TIME_PERIOD_DAYS_ITEMS] =
{
    {QString::fromUtf8("AllaData"),       CTADlg::TIME_PERIOD_DAYS_ALL_DATA,},
    {QString::fromUtf8("10 År"),          CTADlg::TIME_PERIOD_DAYS_10_YEAR,},
    {QString::fromUtf8("7 År"),           CTADlg::TIME_PERIOD_DAYS_7_YEAR,},
    {QString::fromUtf8("5 År"),           CTADlg::TIME_PERIOD_DAYS_5_YEAR,},
    {QString::fromUtf8("3 År"),           CTADlg::TIME_PERIOD_DAYS_3_YEAR,},
    {QString::fromUtf8("2 År"),           CTADlg::TIME_PERIOD_DAYS_2_YEAR,},
    {QString::fromUtf8("1 År"),           CTADlg::TIME_PERIOD_DAYS_1_YEAR,},
    {QString::fromUtf8("6 månader"),      CTADlg::TIME_PERIOD_DAYS_6_MONTH,},
    {QString::fromUtf8("3 månader"),      CTADlg::TIME_PERIOD_DAYS_3_MONTH,},
    {QString::fromUtf8("1 månader"),      CTADlg::TIME_PERIOD_DAYS_1_MONTH,},
    {QString::fromUtf8("2 veckor"),       CTADlg::TIME_PERIOD_DAYS_2_WEEK,},
    {QString::fromUtf8("1 vecka"),        CTADlg::TIME_PERIOD_DAYS_1_WEEK}
};



#define TIME_2_MIN ((int)120000)


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
CTADlg::CTADlg(QWidget *parent) :
    QDialog(parent),
    m_singleStockDataReqStatus(STATUS_REQ_SINGLE_STOCK_IDLE),
    ui(new Ui::CTADlg)
{
    CDbHndl db;

    ui->setupUi(this);

    initFa2OperatingIncomeList();
    initFa2netProfitAfterTaxList();
    initFa3ProfitabilityAnalysis();
    initFa3MinMaxPePrice();

    // m_macdHist = new QwtPlotHistogram ();

    initStockList();

    ui->comboBox->clear();
    db.addStockListsToComboBox(ui->comboBox);
    ui->comboBox->setCurrentIndex(0);

    // Display stock list
    on_pushButSelStockList_clicked();

    // Create thread that import data from yahoo
    m_importYahooTaDataThread = new importYahooTaData(this);

    if(m_importYahooTaDataThread == 0)
    {
        QMessageBox::information(NULL, QString::fromUtf8("Create thread"), QString::fromUtf8("Fail to create Ta Import Data thread"));
    }

    // Init time period
    initTimePeriodCtrls(TIME_PERIOD_DAYS_1_YEAR);

    m_plot.initPlotPicker(ui->qwtPlot);
    m_plot.initPlotZoomer(ui->qwtPlot);
    m_plot.initPlotPanner(ui->qwtPlot);

    m_plot.enablePanningMode(false);
    m_plot.enableZoomMode(false);

   m_macdHist = new QwtPlotHistogram ();

   ui->radioButton->setChecked(true);
   ui->radioButSalesGrowthPricePerShare->setCheckable(true);
   ui->radioButSalesGrowthTotal->setCheckable(true);
   ui->radioButSalesGrowthPerYear->setCheckable(true);
   ui->radioButRSI->setCheckable(true);
   ui->radioButRSI->setChecked(true);

   for(int i = 0; i < FA_NOF_DATA; i++ )
   {
       m_faDataPalette[i] = new QPalette();
   }


}

/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
CTADlg::~CTADlg()
{
    delete ui;

    delete m_macdHist;

    for(int i = 0; i < FA_NOF_DATA; i++ )
    {
        delete m_faDataPalette[i];
    }
}


/*******************************************************************
 *
 * Function:    initFa2OperatingIncomeList()
 *
 * Description:
 *
 *
 *******************************************************************/
void CTADlg::initFa2OperatingIncomeList(void)
{

    QString column0 = QString::fromUtf8("År");
    QString column1 = QString("Kr/\nAktie");
    QString column2 = QString("MKr");
    QString column3 = QString::fromUtf8("Tillväxt");

    ui->treeWidgetSalesGrowth->setColumnCount(4);
    ui->treeWidgetSalesGrowth->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeWidgetSalesGrowth->setSelectionBehavior(QAbstractItemView::SelectRows);



    if(QTreeWidgetItem* header = ui->treeWidgetSalesGrowth->headerItem())
    {
        header->setText(0, column0);
        header->setText(1, column1);
        header->setText(2, column2);
        header->setText(3, column3);
    }

    ui->treeWidgetSalesGrowth->setColumnWidth(0, 65);
    ui->treeWidgetSalesGrowth->setColumnWidth(1, 65);
    ui->treeWidgetSalesGrowth->setColumnWidth(2, 60);
    ui->treeWidgetSalesGrowth->setColumnWidth(3, 50);
    ui->treeWidgetSalesGrowth->setColumnWidth(4, 50);

}







/*******************************************************************
 *
 * Function:    initFa2netProfitAfterTaxList()
 *
 * Description:
 *
 *
 *******************************************************************/
void CTADlg::initFa2netProfitAfterTaxList(void)
{

    QString column0 = QString::fromUtf8("År");
    QString column1 = QString("Kr/\nAktie");
    QString column2 = QString("MKr");
    QString column3 = QString::fromUtf8("Tillväxt");

    ui->treeWidgetProfitGrowth->setColumnCount(4);
    ui->treeWidgetProfitGrowth->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeWidgetProfitGrowth->setSelectionBehavior(QAbstractItemView::SelectRows);



    if(QTreeWidgetItem* header = ui->treeWidgetProfitGrowth->headerItem())
    {
        header->setText(0, column0);
        header->setText(1, column1);
        header->setText(2, column2);
        header->setText(3, column3);
    }

    ui->treeWidgetProfitGrowth->setColumnWidth(0, 65);
    ui->treeWidgetProfitGrowth->setColumnWidth(1, 65);
    ui->treeWidgetProfitGrowth->setColumnWidth(2, 60);
    ui->treeWidgetProfitGrowth->setColumnWidth(3, 50);
    ui->treeWidgetProfitGrowth->setColumnWidth(4, 50);

}


/*******************************************************************
 *
 * Function:    initFa3ProfitabilityAnalysis()
 *
 * Description:
 *
 *
 *******************************************************************/
void CTADlg::initFa3ProfitabilityAnalysis(void)
{

    QString column0 = QString::fromUtf8("År");
    QString column1 = QString::fromUtf8("Vinstmarginal");
    QString column2 = QString::fromUtf8("Ränta / \nEget kapital");


    ui->treeWidgetProfit->setColumnCount(3);
    ui->treeWidgetProfit->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeWidgetProfit->setSelectionBehavior(QAbstractItemView::SelectRows);



    if(QTreeWidgetItem* header = ui->treeWidgetProfit->headerItem())
    {
        header->setText(0, column0);
        header->setText(1, column1);
        header->setText(2, column2);

    }

    ui->treeWidgetProfit->setColumnWidth(0, 65);
    ui->treeWidgetProfit->setColumnWidth(1, 85);
    ui->treeWidgetProfit->setColumnWidth(2, 65);

}


/*******************************************************************
 *
 * Function:    initFa3ProfitabilityAnalysis()
 *
 * Description:
 *
 *
 *******************************************************************/
void CTADlg::initFa3MinMaxPePrice(void)
{

    QString column0 = QString::fromUtf8("År");
    QString column1 = QString("Min\npris");
    QString column2 = QString::fromUtf8("Medel\npris");
    QString column3 = QString("Max\npris");


    ui->treeWidgetHistoricalPrices->setColumnCount(4);
    ui->treeWidgetHistoricalPrices->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeWidgetHistoricalPrices->setSelectionBehavior(QAbstractItemView::SelectRows);



    if(QTreeWidgetItem* header = ui->treeWidgetHistoricalPrices->headerItem())
    {
        header->setText(0, column0);
        header->setText(1, column1);
        header->setText(2, column2);
        header->setText(3, column3);

    }

    ui->treeWidgetHistoricalPrices->setColumnWidth(0, 65);
    ui->treeWidgetHistoricalPrices->setColumnWidth(1, 65);
    ui->treeWidgetHistoricalPrices->setColumnWidth(2, 60);
    ui->treeWidgetHistoricalPrices->setColumnWidth(3, 50);


    QString column4 = QString::fromUtf8("År");
    QString column5 = QString("Min\nP/E");
    QString column6 = QString::fromUtf8("Medel\nP/E");
    QString column7 = QString("Max\nP/E");


    ui->treeWidgetHistoricalPENum->setColumnCount(4);
    ui->treeWidgetHistoricalPENum->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeWidgetHistoricalPENum->setSelectionBehavior(QAbstractItemView::SelectRows);



    if(QTreeWidgetItem* header1 = ui->treeWidgetHistoricalPENum->headerItem())
    {
        header1->setText(0, column4);
        header1->setText(1, column5);
        header1->setText(2, column6);
        header1->setText(3, column7);
    }

    ui->treeWidgetHistoricalPENum->setColumnWidth(0, 65);
    ui->treeWidgetHistoricalPENum->setColumnWidth(1, 65);
    ui->treeWidgetHistoricalPENum->setColumnWidth(2, 60);
    ui->treeWidgetHistoricalPENum->setColumnWidth(3, 50);
    ui->treeWidgetHistoricalPENum->setColumnWidth(4, 50);

}



/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void CTADlg::on_pushButSelStockList_clicked()
{
    CDbHndl db;
    QString stockListName;
    int stockListId;

     stockListName = ui->comboBox->currentText().toUtf8();
     qDebug() << stockListName;

     if(true == db.findTaStockListId(stockListName, stockListId))
     {
         ui->treeWidgetStockList->clear();
         db.addStockSymbolAndNameInTreeWidget1(ui->treeWidgetStockList,
                                            stockListId,
                                            CDbHndl::SORT_TWIDGET_NAME,
                                            (char *) SQL_STR_ASC);


         ui->treeWidgetStockList->setColumnWidth(0, 160);
         ui->treeWidgetStockList->scrollToTop();
         ui->treeWidgetStockList->setCurrentItem(ui->treeWidgetStockList->topLevelItem(0));
     }
}


/*******************************************************************
 *
 * Function:    initStockList()
 *
 * Description:
 *
 *
 *******************************************************************/
void CTADlg::initStockList(void)
{

    QString column0 = QString::fromUtf8("Namn");
    QString column1 = QString::fromUtf8("Symbol");
    QString column2 = QString::fromUtf8("Notera");

    ui->treeWidgetStockList->setColumnCount(3);
    ui->treeWidgetStockList->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeWidgetStockList->setSelectionBehavior(QAbstractItemView::SelectRows);

    if(QTreeWidgetItem* header = ui->treeWidgetStockList->headerItem())
    {
        header->setText(0, column0.toLocal8Bit());
        header->setText(1, column1.toLocal8Bit());
        header->setText(2, column2.toLocal8Bit());
    }

    ui->treeWidgetStockList->setColumnHidden(1, true);

}

/*******************************************************************
 *
 * Function:    on_treeWidgetStockList_doubleClicked()
 *
 * Description:
 *
 *
 *******************************************************************/
void CTADlg::on_treeWidgetStockList_doubleClicked(const QModelIndex &index)
{
    QString stockName;
    QString stockSymbol;
    CDbHndl cd;
    CUtil cu;
    QString assetType;
    QString lastDbDate;
    QString currDate;
    QString tmp;
    int nofDays;

    // Are we already processing data?
    if(m_singleStockDataReqStatus == STATUS_REQ_SINGLE_STOCK_IDLE)
    {
        m_singleStockDataReqStatus = STATUS_REQ_SINGLE_STOCK_PENDING;
        getSelStockListItem(stockName, stockSymbol, index);

        m_reqStockSymbol = stockSymbol;
        m_reqStockName = stockName;

        qDebug() << m_reqStockSymbol;
        qDebug() << m_reqStockName;

        CDbHndl::snapshotStockData_ST keyFaData;

        ui->lineEditPE_5->setDisabled(false);

        ui->textEditMoreInfoBussiness->clear();
        ui->lineEditMoreInfoexecutiveDirector_2->clear();
        ui->lineEditMoreInfoBransch_2->clear();
        ui->lineEditMoreInfoSector_2->clear();
        ui->textEditMoreInfoBussiness->clear();

        ui->lineEditEarningsDivByDividend_5->clear();
        ui->lineEditNavDivLastStockPrice_5->clear();
        ui->lineEditPE_5->clear();
        ui->lineEditPs_5->clear();
        // ui->lineEditRisk->clear();
        ui->lineEditYield_5->clear();
        // ui->lineMeanReturn->clear();

        ui->treeWidgetSalesGrowth->clear();
        ui->treeWidgetProfitGrowth->clear();
        ui->treeWidgetProfit->clear();
        ui->treeWidgetHistoricalPrices->clear();
        ui->treeWidgetHistoricalPENum->clear();
        ui->treeWidgetSalesGrowth->clear();
        deleteAllPlotData(m_qwtHistorcalStockPrices, ui->qwtPlotHistoricalPrices);
        deleteAllPlotData(m_qwtHistoricalPENum, ui->qwtPlotHistoricalPENum);
        deleteAllPlotData(m_qwtSalesGrowth, ui->qwtPlotSalesGrowth);
        deleteAllPlotData(m_qwtProfitGrowth, ui->qwtPlotProfitGrowth);
        deleteAllPlotData(m_qwtProfitAnalysis, ui->qwtPlotProfitAnalysis);


        assetType.clear();
        ui->assetType_3->clear();

        ui->labelLastPrice_3->clear();
        ui->labelOneDayProcentChange_3->clear();

        if(m_startDate.isEmpty() == false && m_endDate.isEmpty() == false)
        {
            if(true == cd.getKeyDataUseBridge(m_reqStockSymbol,keyFaData))
            {
                NordnetCompanyDescription_ST nordnetData;

                if(true == cd.getNordnetCompanyDescription(nordnetData, keyFaData.companyName))
                {
                    // ui->lineEditMoreInfoComapnyName_2->insert(data.assetName);
                    ui->lineEditMoreInfoexecutiveDirector_2->insert(nordnetData.executiveDirector);
                    ui->lineEditMoreInfoBransch_2->insert(nordnetData.bransch);
                    ui->lineEditMoreInfoSector_2->insert(nordnetData.sector);
                    ui->textEditMoreInfoBussiness->insertPlainText(nordnetData.businessDescription);

                }


                qDebug() << keyFaData.companyName;
                cd.addFa2operatingIncomeToTreeWidget(ui->treeWidgetSalesGrowth, keyFaData.companyName);

                if(ui->radioButSalesGrowthPricePerShare->isChecked() == true)
                {
                    displaySalesGrowth(ui->treeWidgetSalesGrowth, 1);
                }
                else if(ui->radioButSalesGrowthTotal->isChecked() == true)
                {
                    displaySalesGrowth(ui->treeWidgetSalesGrowth, 2);
                }
                else if(ui->radioButSalesGrowthPerYear->isChecked() == true)
                {
                    displaySalesGrowth(ui->treeWidgetSalesGrowth, 3);
                }


                cd.addFa2NetProfitAfterTaxToTreeWidget(ui->treeWidgetProfitGrowth, keyFaData.companyName, assetType);

                // Display asset type
                ui->assetType_3->setText(assetType);

                // Plot Profit gowth
                if(ui->radioButProfitGrowthPricePerShare->isChecked() == true)
                {
                    displayProfitGrowth(ui->treeWidgetProfitGrowth, 1);
                }
                else if(ui->radioButProfitGrowthTotGrowth->isChecked() == true)
                {
                    displayProfitGrowth(ui->treeWidgetProfitGrowth, 2);
                }
                else if(ui->radioButProfitGrowthPerYear->isChecked() == true)
                {
                    displayProfitGrowth(ui->treeWidgetProfitGrowth, 3);
                }


                tmp.sprintf("Pris %s",keyFaData.lastPrice.toLocal8Bit().constData());
                ui->labelLastPrice_3->setText(tmp);
                tmp.sprintf("Idag %s \%%",keyFaData.procentChangeOneDay.toLocal8Bit().constData());
                ui->labelOneDayProcentChange_3->setText(tmp);

                cd.addFa3NetProfitAfterTaxToTreeWidget(ui->treeWidgetProfit, keyFaData.companyName);

                // Plot Profit Analysis
                if(ui->radioButProfitMargin->isChecked() == true)
                {
                    displayProfitAnalysis(ui->treeWidgetProfit, 1);
                }
                else if(ui->radioButInterestRateDivEquity->isChecked() == true)
                {
                    displayProfitAnalysis(ui->treeWidgetProfit, 2);
                }


                cd.addFa3MinMaxPEAndPrice(ui->treeWidgetHistoricalPrices, ui->treeWidgetHistoricalPENum, keyFaData.companyName, m_reqStockSymbol);
                displayHistorcalStockPrices(ui->treeWidgetHistoricalPrices, 1, 0);
                displayHistorcalStockPrices(ui->treeWidgetHistoricalPrices, 2, 1);
                displayHistorcalStockPrices(ui->treeWidgetHistoricalPrices, 3, 2);

                displayHistoricalPENum(ui->treeWidgetHistoricalPENum, 1, 0);
                displayHistoricalPENum(ui->treeWidgetHistoricalPENum, 2, 1);
                displayHistoricalPENum(ui->treeWidgetHistoricalPENum, 3, 2);


                ui->lineEditEarningsDivByDividend_5->insert(keyFaData.earningsDividedByDividend);
                ui->lineEditNavDivLastStockPrice_5->insert(keyFaData.navDivLastStockPrice);
                ui->lineEditPE_5->insert(keyFaData.keyValuePE);
                ui->lineEditPs_5->insert(keyFaData.keyValuePS);
                ui->lineEditYield_5->insert(keyFaData.keyValueYield);


                CDbHndl::EfficPortStockData_ST stockRiskReturnData;
                stockRiskReturnData.stockSymbol = m_reqStockSymbol;
                if(true == cd.efficPortfCalcMeanAndStdDev(m_startDate, m_endDate, stockRiskReturnData))
                {
                    QString tmp;
                    tmp.sprintf("%.2f",stockRiskReturnData.riskStdDev * 100);
                    // ui->lineEditRisk->insert(tmp);
                    // tmp.sprintf("%.2f",stockRiskReturnData.meanReturns * 100);
                    // ui->lineMeanReturn->insert(tmp);
                }
                setFundametalAnalysisCtrlTxtColor(keyFaData, stockRiskReturnData, assetType);
            }
        }


        if(true == cd.getLatestDateYahooTaData(lastDbDate, stockSymbol))
        {
            cu.getCurrentDate(currDate);
            qDebug() << currDate;
            qDebug() << lastDbDate;
            cu.nofDaysBeteenDates(currDate, lastDbDate, nofDays);
            if(nofDays < -1)
            {
                bool isUpdate;

                findDateList(stockSymbol, isUpdate);
                QString tmp;
                tmp.sprintf("%d",isUpdate);
                qDebug() << tmp;
                qDebug() << stockSymbol;

                // Prepare request of data from Yahoo
                qDebug() << stockName;
                qDebug() << stockSymbol;
                qDebug() << lastDbDate;
                qDebug() << currDate;

                prepReqTaDataFromServer(stockName, stockSymbol, lastDbDate, currDate);
            }
            else
            {
                m_singleStockDataReqStatus = STATUS_REQ_SINGLE_STOCK_IDLE;
                displayStockData();
            }
        }
        // No latest date was found lets add our own
        else
        {
            // Prepare request of data from Yahoo
            lastDbDate = "1992-01-01";
            cu.getCurrentDate(currDate);
            prepReqTaDataFromServer(stockName, stockSymbol, lastDbDate, currDate);
        }
    }
    else
    {
        m_singleStockDataReqStatus = STATUS_REQ_SINGLE_STOCK_IDLE;
       if(m_crumb_failed == true)
        {
            m_crumb_failed = false;
        }
        else
        {
            QMessageBox::information(this, QString::fromUtf8("Vänta"), QString::fromUtf8("Vänta Processar redan data.."));
        }
    }
}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void CTADlg::getSelStockListItem(QString &stockName, QString &stockSymbol, const QModelIndex &index)
{
    ui->treeWidgetStockList->setCurrentItem(ui->treeWidgetStockList->topLevelItem(index.row()));
    ui->treeWidgetStockList->setFocus();

    stockName.clear();
    stockSymbol.clear();


    stockName = ui->treeWidgetStockList->currentItem()->text(STOCKLIST_NAME);
    stockSymbol = ui->treeWidgetStockList->currentItem()->text(STOCKLIST_SYMBOL);

}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void CTADlg::findDateList(QString stockSymbol, bool &isUpdate)
{
    isUpdate = true;

    NameKey node;
     QMutableListIterator <NameKey> iterator(m_dateIsUpdatedList);
     while(iterator.hasNext())
     {
        node = iterator.next();
        qDebug() << node.symbolName;
        QString tmp;
        tmp.sprintf("%d", node.dataIsUpdated);
        qDebug() << tmp;
        if(node.symbolName.compare(stockSymbol)== 0)
        {
            isUpdate = node.dataIsUpdated;
            return;
        }

     }
}


/*******************************************************************
 *
 * Function:    on_pushButtonImportAllData_clicked()
 *
 * Description:
 *
 *
 *******************************************************************/
void CTADlg::prepReqTaDataFromServer(QString stockName, QString stockSymbol, QString startDate, QString endDate)
{

    if(m_singleStockDataReqStatus == STATUS_REQ_SINGLE_STOCK_PENDING)
    {
        m_reqStartDate = startDate;
        m_reqEndDate = endDate;
        m_reqStockSymbol = stockSymbol;
        m_reqStockName = stockName;

        qDebug() << m_reqStartDate;
        qDebug() << m_reqEndDate;
        qDebug() << m_reqStockSymbol;
        qDebug() << m_reqStockName;


        // Signal used when thread is finish importing data
       connect(m_importYahooTaDataThread, SIGNAL(emitImportSingelStockDataIsFinish(int)), this, SLOT(slotImportSingelStockDataIsFinish(int)));
       connect(this, SIGNAL(emitReqSingleStockDataFromServer()), this, SLOT(slotReqSingleStockDataFromServer()));

       emit emitReqSingleStockDataFromServer();

    }

}


/*******************************************************************
 *
 * Function:    slotImportSingelStockDataIsFinish()
 *
 * Description: This function is invoked singel stock data is
 *              received from http thread
 *
 *******************************************************************/
void CTADlg::slotReceivedAssetTaDataFromServer(int)
{
    if(m_singleStockDataReqStatus == STATUS_REQ_SINGLE_STOCK_PROCESSING)
    {
        // Init thread with data before starting it
        m_importYahooTaDataThread->setImportInfoSingleStock(m_reqStockName, m_reqStockSymbol);
        m_importYahooTaDataThread->start(QThread::NormalPriority);
    }
}


/*******************************************************************
 *
 * Function:    slotImportSingelStockDataIsFinish()
 *
 * Description: This function is invoked singel stock data is
 *              parsed by: m_importYahooTaDataThread
 *
 *******************************************************************/
void CTADlg::slotImportSingelStockDataIsFinish(int dummy)
{
    dummy = dummy;
    if(m_singleStockDataReqStatus == STATUS_REQ_SINGLE_STOCK_PROCESSING)
    {
        m_singleStockDataReqStatus = STATUS_REQ_SINGLE_STOCK_IDLE;
        modifyDateList(m_reqStockSymbol, true);
        qDebug() << m_reqStockSymbol;
        displayStockData();
    }

}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void CTADlg::modifyDateList(QString stockSymbol, bool isUpdate)
{
    NameKey node;
     QMutableListIterator <NameKey> iterator(m_dateIsUpdatedList);
     while(iterator.hasNext())
     {
        node = iterator.next();
        if(node.symbolName.compare(stockSymbol)== 0)
        {
            //iterator.remove();
            addStatusDateList(stockSymbol, isUpdate);

            QString tmp;
            tmp.sprintf("%d",isUpdate);
            qDebug() << tmp;
            qDebug() << stockSymbol;
            node.dataIsUpdated = isUpdate;
            iterator.setValue(node);
            return;
        }

     }
}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void CTADlg::addStatusDateList(QString stockSymbol, bool isUpdate)
{
    NameKey data;
    data.symbolName = stockSymbol;
    data.dataIsUpdated = isUpdate;
    m_dateIsUpdatedList.append(data);

}



/*******************************************************************
 *
 * Function:    displayStockData()
 *
 * Description:
 *
 *
 *******************************************************************/
void CTADlg::displayStockData(void)
{
    CDbHndl db;
    DataPlotIndex_ET plotIndex;
    SubPlotIndex_ET subplotIndex;


    QString legendLable;
    QString status;
    int indicatorIndex;

    QString titleXAxis;
    CYahooStockPlotUtil::MinMaxAxisValues_ST mainPlotAxis;
    CYahooStockPlotUtil::MinMaxAxisValues_ST subPlotAxis;


    deleteAllPlotData(m_qwtStockPlotData, ui->qwtPlot);
    deleteAllPlotData(m_qwtsubPlotData, ui->qwtPlot_2);

    m_macdHistData.clear();
    m_macdHist->detach();
    m_macdHist->setData(NULL);


    QString startDate = ui->setStartDateLineEdit_3->text();
    QString endDate = ui->setEndDateLineEdit_3->text();

    m_qwtStockPlotData.nofStocksToPlot = 0;

    // This variable is automaticly updated when first data is handled by UpdateMinMaxAxis()
    m_qwtStockPlotData.axis.minMaxIsInit = false;
    clearStockAndIndicatorTempMem();

    db.getYahooTaPriceData(m_reqStockSymbol, startDate, endDate, m_qwtStockPlotData, m_stockData);
    initMinMaxAxis(m_qwtStockPlotData.axis, mainPlotAxis);



    titleXAxis = "";//"Tid (dagar)";
    legendLable = "Pris (SEK)";
    setAxisTitles(titleXAxis, legendLable,ui->qwtPlot);

    plotIndex = PLOT_INDEX_PRICE;
    addStockDataToPlot(plotIndex, legendLable);


    //=====

    legendLable.clear();
    if(true == ui->checkBoxLinjerReg->isChecked())
    {
        clearStockAndIndicatorTempMem();
        QString kValue;
        QString rValue;
        updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);
        db.getYahooTaLeastSqrFit(m_reqStockSymbol, startDate, endDate, m_qwtStockPlotData, m_stockData, kValue, rValue);
        updateMinMaxXAxisScales(m_qwtStockPlotData.axis, mainPlotAxis);

        indicatorIndex = 1;
        plotIndex = PLOT_INDEX_LINJER_REG;
        legendLable = "Linjer Regression";

        status = "K= ";
        status += kValue;
        status += ", R= ";
        status += rValue;

        ui->StatusInfoLabel_3->setText(status);

        addStockIndicatorToPlot(indicatorIndex, plotIndex, legendLable, mainPlotAxis, ui->qwtPlot, m_qwtStockPlotData);
        // updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);
    }



    legendLable.clear();
    // Check the checkbox
    if(true == ui->checkBoxShortMvAvg->isChecked())
    {
        clearStockAndIndicatorTempMem();

        qDebug() << "xx size " << m_stockData.data.x.size();
        updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);
        db.getYahooTaMovingAverage(m_reqStockSymbol, startDate, endDate, 20, m_qwtStockPlotData, m_stockData);
        updateMinMaxXAxisScales(m_qwtStockPlotData.axis, mainPlotAxis);

        indicatorIndex = 1;
        plotIndex = PLOT_INDEX_SHORT_MOVING_AVG;
        legendLable = "Short moving average";

        addStockIndicatorToPlot(indicatorIndex, plotIndex, legendLable, mainPlotAxis, ui->qwtPlot, m_qwtStockPlotData);
        // updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);

    }


    // Check the checkbox
    legendLable.clear();
    if(true == ui->checkBoxMidMvAvg->isChecked())
    {
        /// updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);
        clearStockAndIndicatorTempMem();

        qDebug() << "xx size " << m_stockData.data.x.size();
        updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);
        db.getYahooTaMovingAverage(m_reqStockSymbol, startDate, endDate, 50, m_qwtStockPlotData, m_stockData);
        updateMinMaxXAxisScales(m_qwtStockPlotData.axis, mainPlotAxis);

        indicatorIndex = 1;
        plotIndex = PLOT_INDEX_MID_MOVING_AVG;
        legendLable = "Mid moving average";

        addStockIndicatorToPlot(indicatorIndex, plotIndex, legendLable, mainPlotAxis, ui->qwtPlot, m_qwtStockPlotData);
        // updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);
    }



    // Check the checkbox
    legendLable.clear();
    if(true == ui->checkBoxLongMvAvg->isChecked())
    {
        // updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);
        clearStockAndIndicatorTempMem();
        qDebug() << "xx size " << m_stockData.data.x.size();
        updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);
        db.getYahooTaMovingAverage(m_reqStockSymbol, startDate, endDate, 150, m_qwtStockPlotData, m_stockData);
        updateMinMaxXAxisScales(m_qwtStockPlotData.axis, mainPlotAxis);

        indicatorIndex = 1;
        plotIndex = PLOT_INDEX_LONG_MOVING_AVG;
        legendLable = "Long moving average";

        addStockIndicatorToPlot(indicatorIndex, plotIndex, legendLable, mainPlotAxis, ui->qwtPlot, m_qwtStockPlotData);
        // updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);
    }



    // Check the checkboxes
    legendLable.clear();
    if(true == ui->checkBoxBollingBand->isChecked())
    {
        clearStockAndIndicatorTempMem();

        qDebug() << "xx size " << m_stockData.data.x.size();
        updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);
        db.getYahooTaBoillingerBand(m_reqStockSymbol, startDate, endDate, 20, 2, m_qwtStockPlotData, m_stockData);
        updateMinMaxXAxisScales(m_qwtStockPlotData.axis, mainPlotAxis);

        // Lower Bollingerband
        indicatorIndex = 1;
        plotIndex = PLOT_INDEX_BOLLINGERBAND_LOW;
        legendLable = "Lower bollingerband";

        addStockIndicatorToPlot(indicatorIndex, plotIndex, legendLable, mainPlotAxis, ui->qwtPlot, m_qwtStockPlotData);
        // updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);


        // Mid Bollingerband
        indicatorIndex++;
        plotIndex = PLOT_INDEX_BOLLINGERBAND_MID;
        legendLable = "Mid bollingerband";


        m_stockArr.clear();
        addStockIndicatorToPlot(indicatorIndex, plotIndex, legendLable, mainPlotAxis, ui->qwtPlot, m_qwtStockPlotData);
        // updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);

        // Upper Bollingerband
        indicatorIndex++;
        plotIndex = PLOT_INDEX_BOLLINGERBAND_UPPER;
        legendLable = "Upper bollingerband";

        m_stockArr.clear();
        addStockIndicatorToPlot(indicatorIndex, plotIndex, legendLable, mainPlotAxis, ui->qwtPlot, m_qwtStockPlotData);
        // updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);
    }




    //=========

    legendLable.clear();
    if(ui->radioButMacd->isChecked() == true)
    {
        clearStockAndIndicatorTempMem();
        db.getYahooTaMacd(m_reqStockSymbol, startDate, endDate, 26, 12, 9, m_qwtsubPlotData, m_stockData);

        setAxisTitles(titleXAxis, " ",ui->qwtPlot_2, false);
        subplotIndex = SUBPLOT_MACD_LINE;
        legendLable = "MACD";
        indicatorIndex = 1;
        addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET)subplotIndex, legendLable, subPlotAxis, ui->qwtPlot_2, m_qwtsubPlotData);
        updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);

        subPlotAxis.maxX = mainPlotAxis.maxX;
        subPlotAxis.minX = mainPlotAxis.minX;
        m_qwtsubPlotData.axis.maxX = mainPlotAxis.maxX;
        m_qwtsubPlotData.axis.minX = mainPlotAxis.minX;


        setAxisTitles(titleXAxis, " ",ui->qwtPlot_2, false);
        subplotIndex = SUBPLOT_MACD_SIGNAL;
        legendLable = "MACD signal";
        indicatorIndex = 2;
        addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET)subplotIndex, legendLable, subPlotAxis, ui->qwtPlot_2, m_qwtsubPlotData);
        updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);



        setAxisTitles(titleXAxis, " ",ui->qwtPlot_2, false);
        subplotIndex = SUBPLOT_MACD_HISTOGRAM;
        legendLable = "MACD Hist";
        indicatorIndex = 3;
        addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET)subplotIndex, legendLable, subPlotAxis, ui->qwtPlot_2, m_qwtsubPlotData);
        updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);


        m_macdHist-> setStyle (QwtPlotHistogram :: Columns);

        m_macdHistData.clear();
        m_macdHist->detach();
        m_macdHist->setData(NULL);

        for(int ii = 0; ii < m_stockData.data.indicator3.size(); ii++)
        {
            m_stockData.data.x.at(ii);
            m_macdHistData.append (QwtIntervalSample (m_stockData.data.indicator3.at(ii),m_stockData.data.x.at(ii),m_stockData.data.x.at(ii)+1));
        }

        //m_macdHist->setBrush(Qt::red);
        m_macdHist->setPen( QPen( Qt::black) );
        m_macdHist->setSamples(m_macdHistData);
        m_macdHist->attach(ui->qwtPlot_2);
    }

    legendLable.clear();
    if(true == ui->radioButRoc->isChecked())
    {
        clearStockAndIndicatorTempMem();
        subPlotAxis.minMaxIsInit = false;
        m_qwtsubPlotData.axis.minMaxIsInit = false;
        db.getYahooTaRateOfChange(m_reqStockSymbol, startDate, endDate, 10, m_qwtsubPlotData, m_stockData);

        setAxisTitles(titleXAxis, "(%)",ui->qwtPlot_2, false);
        subplotIndex = SUBPLOT_RATE_OF_CHANGE;
        legendLable = "Rate of change";
        indicatorIndex = 1;
        addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET)subplotIndex, legendLable, subPlotAxis, ui->qwtPlot_2, m_qwtsubPlotData);
        updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);

        subplotIndex = SUBPLOT_RATE_OF_CHANGE_LIMIT_100;
        legendLable = " ";
        indicatorIndex = 2;
        addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET)subplotIndex, legendLable, subPlotAxis, ui->qwtPlot_2, m_qwtsubPlotData);
        updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);
    }


    // RSI = Relative Strength Index
    legendLable.clear();
    if(true == ui->radioButRSI->isChecked())
    {
        clearStockAndIndicatorTempMem();
        subPlotAxis.minMaxIsInit = false;
        m_qwtsubPlotData.axis.minMaxIsInit = false;
        db.getYahooTaRsi(m_reqStockSymbol, startDate, endDate, 14, m_qwtsubPlotData, m_stockData);
        m_qwtsubPlotData.axis.maxY=100;
        m_qwtsubPlotData.axis.minY=0;
        m_qwtsubPlotData.axis.minMaxIsInit = true;

        subplotIndex = SUBPLOT_RSI_LIMIT_30;
        legendLable = "";
        indicatorIndex = 2;
        addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET)subplotIndex, legendLable, subPlotAxis, ui->qwtPlot_2, m_qwtsubPlotData);
        updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);


        subplotIndex = SUBPLOT_RSI_LIMIT_70;
        legendLable = "";
        indicatorIndex = 3;
        addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET)subplotIndex, legendLable, subPlotAxis, ui->qwtPlot_2, m_qwtsubPlotData);
        updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);

        setAxisTitles(titleXAxis, "(%)",ui->qwtPlot_2, false);
        subplotIndex = SUBPLOT_RSI;
        legendLable = "";
        indicatorIndex = 1;
        addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET)subplotIndex, legendLable, subPlotAxis, ui->qwtPlot_2, m_qwtsubPlotData);
        updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);


    }


    legendLable.clear();
    if(true == ui->radioButStochastics->isChecked())
    {
        clearStockAndIndicatorTempMem();
        db.getYahooTaStochastics(m_reqStockSymbol, startDate, endDate,  9, 3, m_qwtsubPlotData, m_stockData);

        setAxisTitles(titleXAxis, "(%)",ui->qwtPlot_2, false);
        subplotIndex = SUBPLOT_STOCHASTIC_LIMIT_20;
        legendLable = "";

        indicatorIndex = 3;
        addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET)subplotIndex, legendLable, subPlotAxis, ui->qwtPlot_2, m_qwtsubPlotData);
        updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);

        legendLable = "";
        indicatorIndex = 4;
        subplotIndex = SUBPLOT_STOCHASTIC_LIMIT_80;
        addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET) subplotIndex, legendLable, subPlotAxis, ui->qwtPlot_2, m_qwtsubPlotData);
        updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);

        legendLable = "Stochastics D";
        indicatorIndex = 2;
        subplotIndex = SUBPLOT_STOCHASTIC_D;
        addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET) subplotIndex, legendLable, subPlotAxis, ui->qwtPlot_2, m_qwtsubPlotData);
        updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);

        legendLable = "Stochastics K";
        indicatorIndex = 1;
        subplotIndex = SUBPLOT_STOCHASTIC_K;

        addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET) subplotIndex, legendLable, subPlotAxis, ui->qwtPlot_2, m_qwtsubPlotData);
        updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);
    }

    legendLable.clear();
    if(true == ui->radioButMomentum->isChecked())
    {
        clearStockAndIndicatorTempMem();
        db.getYahooTaMomentum(m_reqStockSymbol, startDate, endDate, 20, m_qwtsubPlotData, m_stockData);

        indicatorIndex = 2;
        subplotIndex = SUBPLOT_INDEX_MOMENTUM_ZERO_LINE;
        addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET)subplotIndex, legendLable, subPlotAxis, ui->qwtPlot_2, m_qwtsubPlotData);
        updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);

        indicatorIndex = 1;
        subplotIndex = SUBPLOT_INDEX_MOMENTUM;
        legendLable = "Momentum";


        ui->StatusInfoLabel_3->setText(status);
        addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET) subplotIndex, legendLable, subPlotAxis, ui->qwtPlot_2, m_qwtsubPlotData);
        updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);
    }
//    updateMinMaxXAxisScales(subPlotAxis);




    //  boxesUpdate edit
    ui->lineEditAssetName_3->clear();
    ui->lineEditAssetName_3->insert(m_reqStockName);

    // ui->lineEditStartDate->clear();
    //ui->lineEditStartDate->insert(m_qwtStockPlotData.axis.minXDate);

    // ui->lineEditStopDate->clear();
    // ui->lineEditStopDate->insert(m_qwtStockPlotData.axis.maxXDate);



    // Reset zoomer
    m_plot.initPlotPicker(ui->qwtPlot);
}


/*******************************************************************
 *
 * Function:    deleteAllPlotData()
 *
 * Description:
 *
 *
 *******************************************************************/
void CTADlg::deleteAllPlotData(CYahooStockPlotUtil::PlotData_ST &allPlotData, QwtPlot *qwtPlot)
{
    for(int i = 0; i < PLOT_INDEX_MAX; i++)
    {
        m_plot.removePlotData(allPlotData, i, qwtPlot);
    }

}


/*******************************************************************
 *
 * Function:    clearStockAndIndicatorTempMem()
 *
 * Description:
 *
 *
 *******************************************************************/
void CTADlg::clearStockAndIndicatorTempMem(void)
{
    m_stockData.data.x.clear();
    m_stockData.data.y.clear();
    m_stockData.data.xDate.clear();
    m_stockData.data.indicator1.clear();
    m_stockData.data.indicator2.clear();
    m_stockData.data.indicator3.clear();
    m_stockData.data.indicator4.clear();

    m_stockArr.clear();
}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void CTADlg::initMinMaxAxis(CYahooStockPlotUtil::MinMaxAxisValues_ST inputAxis,
               CYahooStockPlotUtil::MinMaxAxisValues_ST &outputAxis)
{
    outputAxis.minMaxIsInit = inputAxis.minMaxIsInit;
    outputAxis.maxX = inputAxis.maxX;
    outputAxis.maxY = inputAxis.maxY;
    outputAxis.minX  = inputAxis.minX;
    outputAxis.maxXDate  = inputAxis.maxXDate;
    outputAxis.minXDate  = inputAxis.minXDate;
    outputAxis.minY  = inputAxis.minY;
}


/*******************************************************************
 *
 * Function:    setAxisTitles()
 *
 * Description:
 *
 *
 *******************************************************************/
void CTADlg::setAxisTitles(QString titleXAxis, QString titleYAxis, QwtPlot *qwtPlot,bool showXAxis)
{
    if(showXAxis==true)
        m_plot.setXAxisTitle(qwtPlot, titleXAxis);

    m_plot.setYAxisTitle(qwtPlot, titleYAxis);

}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void CTADlg::addStockDataToPlot(DataPlotIndex_ET plotIndex, QString legendLable)
{
    m_stockArr.insert(0, m_stockData);

    // This is the first added graph
    m_plot.addStockToPlot(m_qwtStockPlotData, m_stockArr[0], m_qwtStockPlotData.axis, plotIndex, legendLable);

   //  m_plot.setRightLegend(ui->qwtPlot);

    m_plot.plotData(m_qwtStockPlotData, ui->qwtPlot, m_qwtStockPlotData.nofStocksToPlot-1);

    m_plot.removeAllStockArrData(m_stockArr);

}


/*******************************************************************
 *
 * Function:    updateMinMaxXAxisScales()
 *
 * Description:
 *
 *
 *******************************************************************/
void CTADlg::updateMinMaxXAxisScales(CYahooStockPlotUtil::MinMaxAxisValues_ST &axis, CYahooStockPlotUtil::MinMaxAxisValues_ST &qwtPlotAxis)
{
    CYahooStockPlotUtil cy;

    // Update min max scale of x and y axis
    cy.updateMinMaxAxis(axis, qwtPlotAxis.maxX, qwtPlotAxis.maxY, qwtPlotAxis.maxXDate);
    cy.updateMinMaxAxis(axis, qwtPlotAxis.minX, qwtPlotAxis.minY, qwtPlotAxis.minXDate);

    cy.updateMinMaxAxis(qwtPlotAxis, axis.maxX, axis.maxY, axis.maxXDate);
    cy.updateMinMaxAxis(qwtPlotAxis, axis.minX, axis.minY, axis.minXDate);

}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void CTADlg::addStockIndicatorToPlot(int indicatorIndex,
                                         DataPlotIndex_ET plotIndex,
                                         QString legendLable,
                                         CYahooStockPlotUtil::MinMaxAxisValues_ST &axis,
                                         QwtPlot *qwtPlot,
                                         CYahooStockPlotUtil::PlotData_ST &qwtPlotData)
{
        m_stockArr.insert(0, m_stockData);

        m_plot.addIndicatorToPlot(qwtPlotData, m_stockArr[0], qwtPlotData.axis, indicatorIndex, plotIndex, legendLable);

        // m_plot.setRightLegend(qwtPlot);

        updateMinMaxXAxisScales(axis, qwtPlotData.axis);

        m_plot.plotData(qwtPlotData, qwtPlot, plotIndex);

        m_plot.removeAllStockArrData(m_stockArr);
}


/*******************************************************************
 *
 * Function:    slotReqSingleStockDataFromServer()
 *
 * Description: This function is invoked when singel stock data need
 *              to be updated. (price data is old)
 *
 *******************************************************************/
void CTADlg::slotReqSingleStockDataFromServer()
{
    QString qry;
    CUtil cu;

    QString filename = DWLD_PATH_TA_LIST_FILE;

    QString startYear;
    QString startMonth;
    QString startDay;

    QString endYear;
    QString endMonth;
    QString endDay;


    if(m_singleStockDataReqStatus == STATUS_REQ_SINGLE_STOCK_PENDING)
    {
        m_singleStockDataReqStatus = STATUS_REQ_SINGLE_STOCK_PROCESSING;

        QByteArray ba1 = m_reqStockSymbol.toLocal8Bit();
        const char *c_reqStockSymbol = ba1.data();

        cu.splitDate(m_reqStartDate, startYear, startMonth, startDay);

        cu.splitDate(m_reqEndDate, endYear, endMonth, endDay);

        qry.sprintf("http://ichart.finance.yahoo.com/table.csv?s=%s&d=%d&e=%d&f=%d&g=d&a=%d&b=%d&c=%d&ignore=.csv",
                    c_reqStockSymbol,
                    (endMonth.toInt()-1),
                    endDay.toInt(),
                    endYear.toInt(),
                    (startMonth.toInt()-1),
                    startDay.toInt(),
                    startYear.toInt());

        qDebug() << startYear;
        qDebug() << startMonth;
        qDebug() << startDay;

        qDebug() << endYear;
        qDebug() << endMonth;
        qDebug() << endDay;

        qDebug() << qry;


        QObject::connect(&m_hw1, SIGNAL(sendSignalTextToDlg2(int)), this, SLOT(slotReceivedAssetTaDataFromServer(int)));
        QUrl url(qry);

        startReqSingleStockDataTimeoutTimer(TIME_2_MIN);
        m_hw1.startRequest(url, filename, 0x01);
    }
}


/*******************************************************************
 *
 * Function:    startResendTimer()
 *
 * Description: This function activate resend timer that is used when
 *              request data from Yahoo server.
 *
 *              This is a single shot timer.
 *
 *******************************************************************/
void CTADlg::startReqSingleStockDataTimeoutTimer(int ms)
{
    QTimer::singleShot(ms, this, SLOT(slotReqSingleStockDataTimerExpired()));
    qDebug() << "startReqSingleStockDataTimeoutTimer()";

}


/*******************************************************************
 *
 * Function:    initTimePeriodCtrls()
 *
 * Description:
 *
 *
 *******************************************************************/
void CTADlg::initTimePeriodCtrls(int value)
{
    CUtil cu;
    int nofDays;

    // Init to valid date
    cu.getCurrentDate(m_startDate);
    cu.getCurrentDate(m_endDate);
    m_endDateRef = m_endDate;

    qDebug() << m_startDate;
    qDebug() << m_endDate;

    m_timePeriodDaysInc = value;
    ui->timePeriodLlineEdit_3->clear();
    ui->timePeriodLlineEdit_3->insert((QString)m_timePeriodDaysArr[value].TxtTimePeriod.toLatin1());
    ui->selTimePeriodSlider_3->setValue(value);
    setTimePeriodDaysUpdateStartStopDate(m_startDate, m_endDate, value);

    if(cu.nofDaysBeteenDates(m_startDate, m_endDate, nofDays) == true)
    {
      // ui->moveTimePeriodSlider_2->setMaximum(nofDays);
    }


    // m_startDate.daysTo (m_endDate);

}


/*******************************************************************
 *
 * Function:    setTimePeriodDaysUpdateStart()
 *
 * Description: This function is used when user changes set
 *              Time Period Slider position.
 *
 *
 *******************************************************************/
void CTADlg::
setTimePeriodDaysUpdateStartStopDate(QString &startDate, QString &endDate, int value)
{
    CUtil cu;
    int intYear;
    int intMonth;
    int intDays;
    // int inc = 0;


    if(false == cu.dateIsValid(endDate))
    {
         QMessageBox::information(this, QString::fromUtf8("Datum"), QString::fromUtf8("1. Slut datum felaktigt"));
        return;
    }

    // inc = -m_timeSlideWindowInc;
    // cu.addDays(endDate, endDate, inc);


    qDebug() << "sw" << m_timePeriodDaysArr[value].timePeriod;

    switch(m_timePeriodDaysArr[value].timePeriod)
    {
    case TIME_PERIOD_DAYS_ALL_DATA:
        // TBD
        break;
    case TIME_PERIOD_DAYS_10_YEAR:
        intYear = -10;
        cu.addYear(endDate, startDate, intYear);
        break;
    case TIME_PERIOD_DAYS_7_YEAR:
        intYear = -7;
        cu.addYear(endDate, startDate, intYear);
        break;
    case TIME_PERIOD_DAYS_5_YEAR:
        intYear = -5;
        cu.addYear(endDate, startDate, intYear);
        break;
    case TIME_PERIOD_DAYS_3_YEAR:
        intYear = -3;
        cu.addYear(endDate, startDate, intYear);
        break;
    case TIME_PERIOD_DAYS_2_YEAR:
        intYear = -2;
        cu.addYear(endDate, startDate, intYear);
        break;
    case TIME_PERIOD_DAYS_1_YEAR:
        intYear = -1;
        cu.addYear(endDate, startDate, intYear);
        break;
    case TIME_PERIOD_DAYS_6_MONTH:
        intMonth = -6;
        cu.addMonth(endDate, startDate, intMonth);
        break;
    case TIME_PERIOD_DAYS_3_MONTH:
        intMonth = -3;
        cu.addMonth(endDate, startDate, intMonth);
        break;
    case TIME_PERIOD_DAYS_1_MONTH:
        intMonth = -1;
        cu.addMonth(endDate, startDate, intMonth);
        break;
    case TIME_PERIOD_DAYS_2_WEEK:
        intDays = -14;
        cu.addDays(endDate, startDate, intDays);
        break;
    case TIME_PERIOD_DAYS_1_WEEK:
        intDays = -7;
        cu.addDays(endDate, startDate, intDays);
        break;
    default:
        break;
    }

    qDebug() << "startTime" << startDate;
    qDebug() << "endTime" << endDate;

    ui->setEndDateLineEdit_3->clear();
    ui->setStartDateLineEdit_3->clear();
    ui->setEndDateLineEdit_3->insert(endDate);
    ui->setStartDateLineEdit_3->insert(startDate);

}



/*******************************************************************
 *
 * Function:    ()
 *
 * Description: This function traverse treewidget and check which
 *              of the nodes that are checked in column 1
 *
 *******************************************************************/
void CTADlg::
getTreeWidgetData(CYahooStockPlotUtil::MinMaxAxisValues_ST &axis, QTreeWidget *treeWidget, CYahooStockPlotUtil::StockData_ST &data, int colx, int coly, int indicator)
{
    CUtil cu;
    CYahooStockPlotUtil ypu;
    double x;
    double y;
    //int cnt;
    int numberOfTopLevelItems = treeWidget->topLevelItemCount();

    qDebug() << numberOfTopLevelItems;

    for (int topLevelindex = 0, cnt = 0; topLevelindex < numberOfTopLevelItems; topLevelindex++)
    {
        QTreeWidgetItem *item = treeWidget->topLevelItem(topLevelindex);
        if(true == cu.number2double(item->text(colx), x) && true == cu.number2double(item->text(coly), y))
        {
            qDebug() << item->text(colx) << item->text(colx);
            qDebug() << item->text(coly) << item->text(coly);
            qDebug() << x << "x";
            qDebug() << cnt << "cnt";

            data.data.x.insert(cnt, x);
            switch(indicator)
            {
            case 0:
                data.data.y.insert(cnt, y);
                break;
            case 1:
                data.data.indicator1.insert(cnt, y);
                break;
            case 2:
                data.data.indicator2.insert(cnt, y);
                break;
            case 3:
                data.data.indicator3.insert(cnt, y);
                break;
            case 4:
                data.data.indicator4.insert(cnt, y);
                break;
            }
            ypu.updateMinMaxAxis(axis, x, y, item->text(colx));
            cnt++;
        }

    }
}





/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void CTADlg::displayHistoricalPENum(QTreeWidget *treeWidget, int coly, int indicator)
{
    DataPlotIndex_ET plotIndex;
    QString legendLable;
    QString titleXAxis;



    m_stockData.data.x.clear();
    m_stockData.data.y.clear();
    m_stockArr.clear();


    if(indicator == 0)
    {
        m_qwtHistoricalPENum.axis.minMaxIsInit = false;

    }
    m_qwtHistoricalPENum.nofStocksToPlot = indicator+1;

    getTreeWidgetData(m_qwtHistoricalPENum.axis, treeWidget, m_stockData, 0, coly, indicator);


    plotIndex = (DataPlotIndex_ET) coly;
    m_stockArr.insert(0, m_stockData);
    if(indicator == 0)
    {
        m_plot.addStockToPlot(m_qwtHistoricalPENum, m_stockArr[0], m_qwtHistoricalPENum.axis, plotIndex, legendLable);
    }
    else
    {
        m_plot.addIndicatorToPlot(m_qwtHistoricalPENum, m_stockArr[0], m_qwtHistoricalPENum.axis, indicator, plotIndex, legendLable);
    }
    m_plot.plotDataSetXScaleStep(m_qwtHistoricalPENum, ui->qwtPlotHistoricalPENum, m_qwtHistoricalPENum.nofStocksToPlot-1, 1);
    m_plot.removeAllStockArrData(m_stockArr);

    legendLable = "";
    setAxisTitles(titleXAxis, legendLable,ui->qwtPlotHistoricalPENum);

    // Reset zoomer
    m_plot.initPlotPicker(ui->qwtPlotHistoricalPENum);
}



/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void CTADlg::displayProfitGrowth(QTreeWidget *treeWidget, int coly)
{
    DataPlotIndex_ET plotIndex;
    QString legendLable;
    QString titleXAxis;
    int indicator;


    indicator = 0;
    m_stockData.data.x.clear();
    m_stockData.data.y.clear();
    m_stockArr.clear();

    m_qwtProfitGrowth.axis.minMaxIsInit = false;
    m_qwtProfitGrowth.nofStocksToPlot = 1;

    getTreeWidgetData(m_qwtProfitGrowth.axis, treeWidget, m_stockData, 0, coly, indicator);

    plotIndex = (DataPlotIndex_ET) coly;
    m_stockArr.insert(0, m_stockData);
    m_plot.addStockToPlot(m_qwtProfitGrowth, m_stockArr[0], m_qwtProfitGrowth.axis, plotIndex, legendLable);

    // Profit per share
    if(coly == 1 && m_qwtProfitGrowth.axis.maxY > 0 && m_qwtProfitGrowth.axis.minY > 0)
    {
        m_plot.plotYAxisLogData(m_qwtProfitGrowth, ui->qwtPlotProfitGrowth, plotIndex);
    }
    else
    {
        m_plot.plotDataSetXScaleStep(m_qwtProfitGrowth, ui->qwtPlotProfitGrowth, plotIndex, 1);
    }
    m_plot.removeAllStockArrData(m_stockArr);

    legendLable = "";
    setAxisTitles(titleXAxis, legendLable, ui->qwtPlotProfitGrowth);

    // Reset zoomer
    m_plot.initPlotPicker(ui->qwtPlotProfitGrowth);
}



/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void CTADlg::displayProfitAnalysis(QTreeWidget *treeWidget, int coly)
{
    DataPlotIndex_ET plotIndex;
    QString legendLable;
    QString titleXAxis;
    int indicator;


    indicator = 0;
    m_stockData.data.x.clear();
    m_stockData.data.y.clear();
    m_stockArr.clear();

    m_qwtProfitAnalysis.axis.minMaxIsInit = false;
    m_qwtProfitAnalysis.nofStocksToPlot = 1;

    getTreeWidgetData(m_qwtProfitAnalysis.axis, treeWidget, m_stockData, 0, coly, indicator);

    plotIndex = (DataPlotIndex_ET) coly;
    m_stockArr.insert(0, m_stockData);
    m_plot.addStockToPlot(m_qwtProfitAnalysis, m_stockArr[0], m_qwtProfitAnalysis.axis, plotIndex, legendLable);
    m_plot.plotDataSetXScaleStep(m_qwtProfitAnalysis, ui->qwtPlotProfitAnalysis, plotIndex, 1);
    m_plot.removeAllStockArrData(m_stockArr);

    legendLable = "";
    setAxisTitles(titleXAxis, legendLable, ui->qwtPlotProfitAnalysis);

    // Reset zoomer
    m_plot.initPlotPicker(ui->qwtPlotProfitAnalysis);
}



/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void CTADlg::displaySalesGrowth(QTreeWidget *treeWidget, int coly)
{
    DataPlotIndex_ET plotIndex;
    QString legendLable;
    QString titleXAxis;
    int indicator;


    indicator = 0;
    m_stockData.data.x.clear();
    m_stockData.data.y.clear();
    m_stockArr.clear();

    m_qwtSalesGrowth.axis.minMaxIsInit = false;
    m_qwtSalesGrowth.nofStocksToPlot = 1;

    getTreeWidgetData(m_qwtSalesGrowth.axis, treeWidget, m_stockData, 0, coly, indicator);

    plotIndex = (DataPlotIndex_ET) coly;
    m_stockArr.insert(0, m_stockData);
    m_plot.addStockToPlot(m_qwtSalesGrowth, m_stockArr[0], m_qwtSalesGrowth.axis, plotIndex, legendLable);


    // Profit per share
    if(coly == 1 && m_qwtSalesGrowth.axis.maxY > 0 && m_qwtSalesGrowth.axis.minY > 0)
    {
        m_plot.plotYAxisLogData(m_qwtSalesGrowth, ui->qwtPlotSalesGrowth, plotIndex);
    }
    else
    {
        m_plot.plotDataSetXScaleStep(m_qwtSalesGrowth, ui->qwtPlotSalesGrowth, plotIndex, 1);
    }


    // m_plot.plotDataSetXScaleStep(m_qwtSalesGrowth, ui->qwtPlotSalesGrowth, plotIndex/*m_qwtSalesGrowth.nofStocksToPlot-1*/, 1);
    m_plot.removeAllStockArrData(m_stockArr);

    legendLable = "";
    setAxisTitles(titleXAxis, legendLable, ui->qwtPlotSalesGrowth);

    // Reset zoomer
    m_plot.initPlotPicker(ui->qwtPlotSalesGrowth);
}



/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void CTADlg::displayHistorcalStockPrices(QTreeWidget *treeWidget, int coly, int indicator)
{
    DataPlotIndex_ET plotIndex;
    QString legendLable;
    QString titleXAxis;



    m_stockData.data.x.clear();
    m_stockData.data.y.clear();
    m_stockArr.clear();


    if(indicator == 0)
    {
        m_qwtHistorcalStockPrices.axis.minMaxIsInit = false;

    }
    m_qwtHistorcalStockPrices.nofStocksToPlot = indicator+1;

    getTreeWidgetData(m_qwtHistorcalStockPrices.axis, treeWidget, m_stockData, 0, coly, indicator);


    plotIndex = (DataPlotIndex_ET) coly;
    m_stockArr.insert(0, m_stockData);
    if(indicator == 0)
    {
        m_plot.addStockToPlot(m_qwtHistorcalStockPrices, m_stockArr[0], m_qwtHistorcalStockPrices.axis, plotIndex, legendLable);
    }
    else
    {
        m_plot.addIndicatorToPlot(m_qwtHistorcalStockPrices, m_stockArr[0], m_qwtHistorcalStockPrices.axis, indicator, plotIndex, legendLable);
    }
    m_plot.plotDataSetXScaleStep(m_qwtHistorcalStockPrices, ui->qwtPlotHistoricalPrices, m_qwtHistorcalStockPrices.nofStocksToPlot-1, 1);
    m_plot.removeAllStockArrData(m_stockArr);

    legendLable = "";
    setAxisTitles(titleXAxis, legendLable,ui->qwtPlotHistoricalPrices);

    // Reset zoomer
    m_plot.initPlotPicker(ui->qwtPlotHistoricalPrices);
}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 *******************************************************************/

void CTADlg::setFundametalAnalysisCtrlTxtColor(CDbHndl::snapshotStockData_ST keyData,
                                               CDbHndl::EfficPortStockData_ST stockRiskReturnData,
                                               QString assetType)
{
    CUtil cu;
    QColor color;

    if(keyData.earningsDividedByDividend.toDouble() >= 1 && keyData.earningsDividedByDividend.toDouble() < 2)
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
    }
    else if(keyData.earningsDividedByDividend.toDouble() >= 2)
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::GREEN);
    }
    else
    {
        color = Qt::red;
    }


    setFaEditControlTxtColor(ui->lineEditEarningsDivByDividend_5, m_faDataPalette[FA_ERNING_DIV_DIVIDEN], color);
    //setFaEditControlTxtColor(ui->lineEditEarningsDivByDividend_2, m_faDataPalette[FA_ERNING_DIV_DIVIDEN], color);

    if(keyData.navDivLastStockPrice.toDouble() >= 0.7 && keyData.navDivLastStockPrice.toDouble() < 1)
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3);
    }
    else if(keyData.navDivLastStockPrice.toDouble() >= 1 && keyData.navDivLastStockPrice.toDouble() < 2)
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
    }
    else if(keyData.navDivLastStockPrice.toDouble() >= 2)
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::GREEN);
    }
    else
    {
        // color = Qt::black;
        color = Qt::red;
    }

    setFaEditControlTxtColor(ui->lineEditNavDivLastStockPrice_5, m_faDataPalette[FA_NAV_DIV_LAST_PRICE], color);
    // setFaEditControlTxtColor(ui->lineEditNavDivLastStockPrice_2, m_faDataPalette[FA_NAV_DIV_LAST_PRICE], color);



    if(keyData.keyValuePE.toDouble() < 5)
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
    }
    else if(keyData.keyValuePE.toDouble() >= 5 && keyData.keyValuePE.toDouble() <= 15)
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::GREEN);
    }
    else if(keyData.keyValuePE.toDouble() > 15 && keyData.keyValuePE.toDouble() <= 18)
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3);
    }
    else if(keyData.keyValuePE.toDouble() > 18 && keyData.keyValuePE.toDouble() <= 25)
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
    }
    else
    {
        color = Qt::red;
    }

    // Disable P/E when company type is finance and real estate
    if(assetType.compare("Finans och fastighet")== 0)
    {
        ui->lineEditPE_5->setDisabled(true);
    }
    else
    {
        ui->lineEditPE_5->setDisabled(false);
    }

    setFaEditControlTxtColor(ui->lineEditPE_5, m_faDataPalette[FA_PE], color);




    double doubleNumber;
    if(false == cu.number2double(keyData.earningsDividedByDividend, doubleNumber))
    {
        if(keyData.keyValueYield.toDouble() < 2)
        {
            color = Qt::red;
        }
        else if(keyData.keyValueYield.toDouble() >= 3 && keyData.keyValueYield.toDouble() < 4)
        {
            color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
        }
        else if(keyData.keyValueYield.toDouble() >= 4 && keyData.keyValueYield.toDouble() <= 6)
        {
            color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::GREEN);
        }
        else if(keyData.keyValueYield.toDouble() > 6 && keyData.keyValueYield.toDouble() < 7)
        {
            color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
        }
        else if(keyData.keyValueYield.toDouble() > 7 && keyData.keyValueYield.toDouble() < 10)
        {
            color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3);
        }
        else if(keyData.keyValueYield.toDouble() > 10)
        {
            color = Qt::red;
        }
        else
        {
            color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3);
        }
    }
    else
    {
        //  Grön Om vinst/Utdelning är >= 2 && direktavkastningn = 4 - 6 %
         if(keyData.earningsDividedByDividend.toDouble() > 1 && keyData.earningsDividedByDividend.toDouble() < 2)
        {
            if(keyData.keyValueYield.toDouble() < 2)
            {
                color = Qt::red;
            }
            else if(keyData.keyValueYield.toDouble() >= 3 && keyData.keyValueYield.toDouble() < 4)
            {
                color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3);
            }
            else if(keyData.keyValueYield.toDouble() >= 4 )
            {
                color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
            }
            else
            {
                color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3);
            }
        }
        else if(keyData.earningsDividedByDividend.toDouble() >= 2)
        {
            if(keyData.keyValueYield.toDouble() < 2)
            {
                color = Qt::red;
            }
            else if(keyData.keyValueYield.toDouble() >= 3 && keyData.keyValueYield.toDouble() < 4)
            {
                color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
            }
            else if(keyData.keyValueYield.toDouble() >= 4)
            {
                color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::GREEN);
            }
            else
            {
                color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3);
            }
        }
        else
        {
            color = Qt::red;
        }
    }


    setFaEditControlTxtColor(ui->lineEditYield_5, m_faDataPalette[FA_YIELD], color);
    //setFaEditControlTxtColor(ui->lineEditYield_2, m_faDataPalette[FA_YIELD], color);


    if(stockRiskReturnData.meanReturns <= 0)
    {
        color = Qt::red;
    }
    else
    {
        color = Qt::black;
    }

    //setFaEditControlTxtColor(ui->lineMeanReturn, m_faDataPalette[FA_MEAN_RETURN], color);

    if(keyData.keyValuePS.toDouble() <= 0.75)
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::GREEN);
    }
    else if(keyData.keyValuePS.toDouble() > 0.75 && keyData.keyValuePS.toDouble() <= 1.5)
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3);
    }
    else if(keyData.keyValuePS.toDouble() > 1.5 && keyData.keyValuePS.toDouble() <= 3.0)
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
    }
    else
    {
        color = Qt::red;
    }

    setFaEditControlTxtColor(ui->lineEditPs_5, m_faDataPalette[FA_YIELD], color);
}



/*******************************************************************
 *
 * Function:    setFaEditControlTxtColor()
 *
 * Description:
 *
 *
 *
 *
 *******************************************************************/
void CTADlg::setFaEditControlTxtColor(QLineEdit *lineEdit, QPalette *palette, const QColor &color)
{
    palette->setColor(QPalette::Text, color); //Qt::red
    lineEdit->setPalette(*palette);
}



/*******************************************************************
 *
 * Function:    on_selTimePeriodSlider_3_valueChanged()
 *
 * Description:
 *
 *
 *
 *
 *******************************************************************/
void CTADlg::on_selTimePeriodSlider_3_valueChanged(int value)
{
    m_timePeriodDaysInc = value;

    ui->timePeriodLlineEdit_3->clear();
    ui->timePeriodLlineEdit_3->insert((QString)m_timePeriodDaysArr[value].TxtTimePeriod.toLatin1());
    setTimePeriodDaysUpdateStartStopDate(m_startDate, m_endDate, value);
}
