#include "fatab.h"
#include "ui_fatab.h"
#include "util.h"


#define GROWTH_TYPE_SALES 0
#define GROWTH_TYPE_PROFIT 1


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
FaTab::FaTab(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FaTab)
{
    ui->setupUi(this);

    ui->labelProfitPg3->setText(QString::fromUtf8("Försäljning"));
    ui->labelProfitPg3->setStyleSheet("QLabel { color : black; }");

    ui->labelSalesPg3->setText(QString::fromUtf8("Vinst"));
    ui->labelSalesPg3->setStyleSheet("QLabel { color : red; }");

    ui->labelProfitMarginPg3->setText(QString::fromUtf8("Vinstmarginal"));
    ui->labelProfitMarginPg3->setStyleSheet("QLabel { color : lightgreen; }");

    ui->labelReturnOnEquityPg3->setText(QString::fromUtf8("Ränta på eget kapital"));
    ui->labelReturnOnEquityPg3->setStyleSheet("QLabel { color : magenta; }");

    CDbHndl db;

    #if 0
    CYahooStockPlotUtil ya;

    histPricesAxisMinMaxVal.minMaxIsInit = false;
    ya.addOneIntervalBarData(m_histMinMaxPrices, histPricesAxisMinMaxVal, 2009.0, 25.0, 20.0);
    ya.addOneIntervalBarData(m_histMinMaxPrices, histPricesAxisMinMaxVal, 2010.0, 30.0, 25.0);
    ya.addOneIntervalBarData(m_histMinMaxPrices, histPricesAxisMinMaxVal, 2011.0, 35.0, 30.0);
    ya.addOneIntervalBarData(m_histMinMaxPrices, histPricesAxisMinMaxVal, 2012.0, 40.0, 35.0);
    ya.addOneIntervalBarData(m_histMinMaxPrices, histPricesAxisMinMaxVal, 2013.0, 45.0, 40.0);

    QString title("");
    ya.plotIntervalBarData(ui->qwtPlotPePrice, title, m_histMinMaxPrices, Qt::red);
    #endif

    initOperatingIncomeList();
    initNetProfitAfterTaxList();
    initProfitabilityAnalysis();
    initMinMaxPePrice();


    initStockList();

    ui->StockListComboBox->clear();
    db.addStockListsToComboBox(ui->StockListComboBox);
    ui->StockListComboBox->setCurrentIndex(0);

    // Display stock list
    on_SelStockListButton_clicked();



  //  m_plot.initPlotPicker(ui->qwtPlot);
  //  m_plot.initPlotZoomer(ui->qwtPlot);
  //  m_plot.initPlotPanner(ui->qwtPlot);

  //  m_plot.enablePanningMode(false);
  //  m_plot.enableZoomMode(false);


  // ui->radioButton->setChecked(true);
  // ui->radioButSalesGrowthPricePerShare->setCheckable(true);
  //  ui->radioButSalesGrowthTotal->setCheckable(true);
  //  ui->radioButSalesGrowthPerYear->setCheckable(true);
  //  ui->radioButRSI->setCheckable(true);
  //  ui->radioButRSI->setChecked(true);

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
FaTab::~FaTab()
{
    delete ui;

    for(int i = 0; i < FA_NOF_DATA; i++ )
    {
        delete m_faDataPalette[i];
    }
}







/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 * Note:    when add data to new qwtPlot widget. You need to create a
 *          new set of following variables:
 *
 *          m_stockData3
 *          m_qwtPePrice
 *          ui->qwtPlotPePrice
 *
 *          Do not forget to set m_qwtSalesGrowth.axis.minMaxIsInit = false;
 *
 *******************************************************************/
void FaTab::on_treeWidgetStockList_doubleClicked(const QModelIndex &index)
{
    QString stockName;
    QString stockSymbol;
    CDbHndl db;
    QString assetType;
    QString tmp;
    CYahooStockPlotUtil cy;
    double minY;
    double maxY;
    double minY1;
    double maxY1;
    double growthRate;
    int nofYears;
    DataPlotIndex_ET memSlotAndPlotIndex = (DataPlotIndex_ET) 1;

    CUtil cu;

    double k;
    double m;
    double k1;
    double m1;
    double k2;
    double m2;
    double minX;
    double maxX;

    getSelStockListItem(stockName, stockSymbol, index);

    m_reqStockSymbol = stockSymbol;
    m_reqStockName = stockName;

    qDebug() << m_reqStockSymbol;
    qDebug() << m_reqStockName;

    CDbHndl::snapshotStockData_ST keyFaData;

    ui->lineEditPE_3->setDisabled(false);

    ui->lineEditEarningsDivByDividend_3->clear();
    ui->lineEditNavDivLastStockPrice_3->clear();
    ui->lineEditPE_3->clear();
    ui->lineEditPs_3->clear();
    ui->lineEditYield_3->clear();

    assetType.clear();
    ui->assetType->clear();

    // Reported and calc Sales growth
    ui->labelReportAvg->clear();
    ui->labelReportAvg_2->clear();
    ui->labelCalcTrendline->clear();
    ui->labelCalcTrendline_2->clear();


    // Reset treeWidget
    ui->treeWidgetSalesGrowth->clear();
    ui->treeWidgetProfitGrowth->clear();
    ui->treeWidgetProfit->clear();
    ui->treeWidgetHistoricalPrices->clear();
    ui->treeWidgetHistoricalPENum->clear();
    ui->treeWidgetProfit->clear();

    // m_histMinMaxPrices.clear();

    cy.deleteIntervalBarData(ui->qwtPlotPePrice, m_intervalCurvePE, m_histMinMaxPE);
    cy.deleteIntervalBarData(ui->qwtPlotPePrice, m_intervalCurvePrice, m_histMinMaxPrices);
    deleteAllPlotData(m_qwtPePrice, ui->qwtPlotPePrice);
    deleteAllPlotData(m_qwtHistoricalPENum, ui->qwtPlotReturnOnEquity);
    deleteAllPlotData(m_qwtSalesGrowth, ui->qwtPlotSalesGrowth);
    deleteAllPlotData(m_qwtProfitGrowth, ui->qwtPlotProfitAnalysis);
    deleteAllPlotData(m_ProfitMargin, ui->qwtPlotProfitMargin);
    deleteAllPlotData(m_qwtSalesAndProfitGrowth, ui->qwtPlotSalesAndProfitGrowth);
    deleteAllPlotData(m_qwtProfitAnalysis, ui->qwtPlotPePrice);


    ui->labelLastPrice_3->clear();
    ui->labelOneDayProcentChange_3->clear();


    if(true == db.getKeyDataUseBridge(m_reqStockSymbol, keyFaData))
    {
        qDebug() << keyFaData.companyName;

        // Display asset type
        ui->assetType->setText(assetType);

        // Display last price and price change
        tmp.sprintf("Pris %s",keyFaData.lastPrice.toLocal8Bit().constData());
        ui->labelLastPrice_3->setText(tmp);
        tmp.sprintf("Idag %s \%%",keyFaData.procentChangeOneDay.toLocal8Bit().constData());
        ui->labelOneDayProcentChange_3->setText(tmp);


        //=====================================
        //
        //=====================================
        clearStockAndIndicatorTempMem();
        CYahooStockPlotUtil::MinMaxAxisValues_ST mainPlotAxis;
        int treeWidgetYCol = 1;
        int indicator = 0;
        m_qwtSalesGrowth.axis.minMaxIsInit = false;
        m_qwtSalesGrowth.nofStocksToPlot = 1;

        m_qwtProfitGrowth.axis.minMaxIsInit = false;
        m_qwtSalesGrowth.nofStocksToPlot = 1;

        m_qwtSalesAndProfitGrowth.axis.minMaxIsInit = false;
        m_qwtSalesAndProfitGrowth.nofStocksToPlot = 1;

        m_histPricesAxisMinMaxVal.minMaxIsInit = false;
        m_qwtPePrice.axis.minMaxIsInit = false;
        m_qwtPePrice.nofStocksToPlot = 0;


        // Sales growth & Profit growth
        m_stockData.data.x.clear();
        m_stockData.data.y.clear();
        db.addFa2operatingIncomeToTreeWidget(ui->treeWidgetSalesGrowth, keyFaData.companyName);
        db.addFa2NetProfitAfterTaxToTreeWidget(ui->treeWidgetProfitGrowth, keyFaData.companyName, assetType);



        db.addFa3MinMaxPEAndPrice(ui->treeWidgetHistoricalPrices, ui->treeWidgetHistoricalPENum, keyFaData.companyName, m_reqStockSymbol);


        QString title("");
        getHistoricalMinMaxPrices(ui->treeWidgetHistoricalPENum,
                                  m_histMinMaxPE,
                                  m_histPEAxisMinMaxVal);

        cy.plotIntervalBarData(ui->qwtPlotPePrice, m_intervalCurvePE, title, m_histMinMaxPE, Qt::red);


        updateMinMaxXAxisScales(m_histPricesAxisMinMaxVal, m_histPEAxisMinMaxVal);


        getHistoricalMinMaxPrices(ui->treeWidgetHistoricalPrices,
                                  m_histMinMaxPrices,
                                  m_histPricesAxisMinMaxVal);
       cy.plotIntervalBarData(ui->qwtPlotPePrice, m_intervalCurvePrice, title, m_histMinMaxPrices, Qt::black);



        // Calc least sqrt fit on profit and sales growth
        calcLeastSqrtFit(ui->treeWidgetSalesGrowth, k, m, minX, maxX, 1);
        minY = k * minX + m;
        maxY = k * maxX + m;
        nofYears = (int) (maxX - minX);

        // Display trend line sales growth
        if(true == cu.annualGrowthRate(minY, maxY, (nofYears), growthRate))
        {
            growthRate = growthRate - 1;
            growthRate = growthRate * 100;
            qDebug() << growthRate;
            QString trendLineGrowth;
            trendLineGrowth = trendLineGrowth.sprintf("                                                                %.1f\%%     ", growthRate);
            trendLineGrowth += QString::fromUtf8("(Enligt trendlinje)");
            ui->labelCalcTrendline->setText(trendLineGrowth);
        }

        QString avgResult[10];
        if(true == SalesOrProfitGrowthCalcAverage(ui->treeWidgetSalesGrowth, avgResult, 4))
        {
            QString reportAvg;
            reportAvg = QString::fromUtf8("Medel:       ");
            for(int i = 0; i < 3; i++)
            {
                if( i > 0)
                    reportAvg += ",        ";
                reportAvg += avgResult[i];
            }

            reportAvg += QString::fromUtf8("    (Rapporterad)");
            ui->labelReportAvg->clear();
            ui->labelReportAvg->setText(reportAvg);
        }

        calcLeastSqrtFit(ui->treeWidgetProfitGrowth, k1, m1, minX, maxX, 1);
        minY1 = k1 * minX + m1;
        maxY1 = k1 * maxX + m1;

        if(true == cu.annualGrowthRate(minY1, maxY1, (nofYears), growthRate))
        {
            growthRate = growthRate - 1;
            growthRate = growthRate * 100;
            qDebug() << growthRate;
            QString trendLineGrowth;
            trendLineGrowth = trendLineGrowth.sprintf("                                                                %.1f\%%     ", growthRate);
            trendLineGrowth += QString::fromUtf8("(Enligt trendlinje)");
            ui->labelCalcTrendline_2->setText(trendLineGrowth);
        }


        if(true == SalesOrProfitGrowthCalcAverage(ui->treeWidgetProfitGrowth, avgResult, 4))
        {
            QString reportAvg;
            reportAvg = QString::fromUtf8("Medel:       ");
            for(int i = 0; i < 3; i++)
            {
                if( i > 0)
                    reportAvg += ",        ";
                reportAvg += avgResult[i];
            }

            reportAvg += QString::fromUtf8("    (Rapporterad)");
            ui->labelReportAvg_2->clear();
            ui->labelReportAvg_2->setText(reportAvg);
        }


        // Page 1. Sales growth
        indicator = 0;
        displayTreeWidgetDataInQwtPlot(ui->treeWidgetSalesGrowth,
                           m_stockData,
                           m_qwtSalesGrowth,
                           ui->qwtPlotSalesGrowth,
                           treeWidgetYCol,
                           indicator);


        // Page 2. Profit growth
        indicator = 0;
        m_stockData2.data.x.clear();
        m_stockData2.data.y.clear();
        displayTreeWidgetDataInQwtPlot(ui->treeWidgetProfitGrowth,
                           m_stockData2,
                           m_qwtProfitGrowth,
                           ui->qwtPlotProfitAnalysis,
                           treeWidgetYCol,
                           indicator);


        // Page 3. Sales growth
        indicator = 0;
        memSlotAndPlotIndex = (DataPlotIndex_ET) 0;
        m_stockData1.data.x.clear();
        m_stockData1.data.y.clear();
        displayTreeWidgetDataInQwtPlot(ui->treeWidgetSalesGrowth,
                           m_stockData1,
                           m_qwtSalesAndProfitGrowth,
                           ui->qwtPlotSalesAndProfitGrowth,
                           treeWidgetYCol,
                           indicator,
                           memSlotAndPlotIndex);



        updateMinMaxXAxisScales(m_qwtPePrice.axis, m_histPricesAxisMinMaxVal);
        // Page 4. Sales growth
        indicator = 0;
        memSlotAndPlotIndex = (DataPlotIndex_ET) 0;
        m_stockData3.data.x.clear();
        m_stockData3.data.y.clear();
        displayTreeWidgetDataInQwtPlot(ui->treeWidgetSalesGrowth,
                           m_stockData3,
                           m_qwtPePrice,
                           ui->qwtPlotPePrice,
                           treeWidgetYCol,
                           indicator,
                           memSlotAndPlotIndex);


        mainPlotAxis.minMaxIsInit = false;
        updateMinMaxXAxisScales(mainPlotAxis, m_qwtSalesAndProfitGrowth.axis);



        // Page 4. Profit growth
        indicator = 4;
        memSlotAndPlotIndex = (DataPlotIndex_ET) 4;
        m_stockData3.data.x.clear();
        m_stockData3.data.indicator4.clear();
        displayTreeWidgetDataInQwtPlot(ui->treeWidgetProfitGrowth,
                           m_stockData3,
                           m_qwtPePrice,
                           ui->qwtPlotPePrice,
                           treeWidgetYCol,
                           indicator,
                           memSlotAndPlotIndex);





        // Page 3. Profit growth
        indicator = 1;
        memSlotAndPlotIndex = (DataPlotIndex_ET) 1;
        m_stockData1.data.x.clear();
        m_stockData1.data.indicator1.clear();
        displayTreeWidgetDataInQwtPlot(ui->treeWidgetProfitGrowth,
                           m_stockData1,
                           m_qwtSalesAndProfitGrowth,
                           ui->qwtPlotSalesAndProfitGrowth,
                           treeWidgetYCol,
                           indicator,
                           memSlotAndPlotIndex);






         // Page 1. Trend line Sales growth
         indicator = 1;
         m_stockData.data.x.clear();
         m_stockData.data.indicator1.clear();
         memSlotAndPlotIndex = (DataPlotIndex_ET) 2;
         createLeastSqrtFitPlotData(k, m, minX, maxX, m_qwtSalesGrowth.axis, m_stockData, indicator);
         displayLeastSqrtFitData(m_stockData,  m_qwtSalesGrowth, ui->qwtPlotSalesGrowth, indicator, (DataPlotIndex_ET) memSlotAndPlotIndex);


         // Page 2. Trend line profit growth
         indicator = 1;
         m_stockData2.data.x.clear();
         m_stockData2.data.indicator1.clear();
         memSlotAndPlotIndex = (DataPlotIndex_ET) 2;
         createLeastSqrtFitPlotData(k1, m1, minX, maxX, m_qwtProfitGrowth.axis, m_stockData2, indicator);
         displayLeastSqrtFitData(m_stockData2,  m_qwtProfitGrowth, ui->qwtPlotProfitAnalysis, indicator, (DataPlotIndex_ET) memSlotAndPlotIndex);
         updateMinMaxXAxisScales(m_qwtSalesAndProfitGrowth.axis, mainPlotAxis);


         // Page 3. Trend line sales growth
         indicator = 3;
         memSlotAndPlotIndex = (DataPlotIndex_ET) 3;
         m_stockData1.data.x.clear();
         m_stockData1.data.indicator3.clear();
         createLeastSqrtFitPlotData(k, m, minX, maxX, m_qwtSalesAndProfitGrowth.axis, m_stockData1, indicator);
         displayLeastSqrtFitData(m_stockData1,  m_qwtSalesAndProfitGrowth, ui->qwtPlotSalesAndProfitGrowth, indicator, (DataPlotIndex_ET) memSlotAndPlotIndex);


         // Page 4. Trend line sales growth
         indicator = 3;
         memSlotAndPlotIndex = (DataPlotIndex_ET) 3;
         m_stockData3.data.x.clear();
         m_stockData3.data.indicator3.clear();
         createLeastSqrtFitPlotData(k, m, minX, maxX, m_qwtPePrice.axis, m_stockData3, indicator);
         displayLeastSqrtFitData(m_stockData3,  m_qwtPePrice, ui->qwtPlotPePrice, indicator, (DataPlotIndex_ET) memSlotAndPlotIndex);



         // Page 3. Trend line profit growth
         indicator = 2;
         memSlotAndPlotIndex = (DataPlotIndex_ET) 2;
         m_stockData1.data.x.clear();
         m_stockData1.data.indicator2.clear();
         createLeastSqrtFitPlotData(k1, m1, minX, maxX, m_qwtSalesAndProfitGrowth.axis, m_stockData1, indicator);
         displayLeastSqrtFitData(m_stockData1,  m_qwtSalesAndProfitGrowth, ui->qwtPlotSalesAndProfitGrowth, indicator, (DataPlotIndex_ET) memSlotAndPlotIndex);


         // Page 4. Trend line profit growth
         indicator = 2;
         memSlotAndPlotIndex = (DataPlotIndex_ET) 2;
         m_stockData3.data.x.clear();
         m_stockData3.data.indicator2.clear();
         createLeastSqrtFitPlotData(k1, m1, minX, maxX, m_qwtPePrice.axis, m_stockData3, indicator);
         displayLeastSqrtFitData(m_stockData3,  m_qwtPePrice, ui->qwtPlotPePrice, indicator, (DataPlotIndex_ET) memSlotAndPlotIndex);




         // Page 1 Add prognose data to treeWidget (based on tredline)
         calcLeastSqrtFit(ui->treeWidgetSalesGrowth, k2, m2, minX, maxX, 2);
         if(ui->checkBoxShowForecast->isChecked() == true)
         {
             addSalesOrProfitGrowthPrognosData(ui->treeWidgetSalesGrowth, k, m, k2, m2, maxX);
         }


         // Page 2 Add prognose data to treeWidget (based on tredline)
         calcLeastSqrtFit(ui->treeWidgetProfitGrowth, k2, m2, minX, maxX, 2);
         if(ui->checkBoxShowForecast_2->isChecked() == true)
         {
             addSalesOrProfitGrowthPrognosData(ui->treeWidgetProfitGrowth, k1, m1, k2, m2, maxX);
         }


        // Page 3. In treeWidget, display profit margin and return on equity
        db.addFa3NetProfitAfterTaxToTreeWidget(ui->treeWidgetProfit, keyFaData.companyName);


        // Page 3. Plot Profit margin & Return on equity
        displayProfitAnalysis(ui->treeWidgetProfit, ui->qwtPlotProfitMargin, 1);
        displayProfitAnalysis(ui->treeWidgetProfit, ui->qwtPlotReturnOnEquity, 2);






#if 0
        displayHistoricalPENum(ui->treeWidgetHistoricalPENum, 1, 0);
        displayHistoricalPENum(ui->treeWidgetHistoricalPENum, 2, 1);
        displayHistoricalPENum(ui->treeWidgetHistoricalPENum, 3, 2);
#endif
        ui->lineEditEarningsDivByDividend_3->insert(keyFaData.earningsDividedByDividend);
        ui->lineEditNavDivLastStockPrice_3->insert(keyFaData.navDivLastStockPrice);
        ui->lineEditPE_3->insert(keyFaData.keyValuePE);
        ui->lineEditPs_3->insert(keyFaData.keyValuePS);
        ui->lineEditYield_3->insert(keyFaData.keyValueYield);

        setFundametalAnalysisCtrlTxtColor(keyFaData, assetType);

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
void FaTab::initStockList(void)
{

    QString column0 = QObject::tr("Namn");
    QString column1 = QObject::tr("Symbol");
    QString column2 = QObject::tr("Notera");

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
 * Function:    initFa2OperatingIncomeList()
 *
 * Description:
 *
 *
 *******************************************************************/
void FaTab::initOperatingIncomeList(void)
{

    QString column0 = QString::fromUtf8("År");
    QString column1 = QString("Kr/\nAktie");
    QString column2 = QString("MKr");
    QString column3 = QString::fromUtf8("Tillväxt (%)");

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
 * Function:    on_SelStockListButton_clicked()
 *
 * Description:
 *
 *
 *******************************************************************/
void FaTab::on_SelStockListButton_clicked()
{
    CDbHndl db;
    QString stockListName;
    int stockListId;

     stockListName = ui->StockListComboBox->currentText().toUtf8();
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
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void FaTab::getSelStockListItem(QString &stockName, QString &stockSymbol, const QModelIndex &index)
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
 * Function:    deleteAllPlotData()
 *
 * Description:
 *
 *
 *******************************************************************/
void FaTab::deleteAllPlotData(CYahooStockPlotUtil::PlotData_ST &allPlotData, QwtPlot *qwtPlot)
{
    for(int i = 0; i < PLOT_INDEX_MAX; i++)
    {
        m_plot.removePlotData(allPlotData, i, qwtPlot);
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
void FaTab::displayTreeWidgetDataInQwtPlot(QTreeWidget *treeWidget,
                               CYahooStockPlotUtil::StockData_ST &stockData,
                               CYahooStockPlotUtil::PlotData_ST &plotData,
                               QwtPlot *qwtPlot,
                               int treeWidgetYCol,
                               int indicator,
                               DataPlotIndex_ET plotIndex)
{
    QString legendLable;
    QString titleXAxis;

    getTreeWidgetData(plotData.axis, treeWidget, stockData, 0, treeWidgetYCol, indicator);

    m_stockArr.insert(0, stockData);
    if(indicator == 0)
    {
        m_plot.addStockToPlot(plotData, m_stockArr[0], plotData.axis, plotIndex, legendLable);
    }
    else
    {
        m_plot.addIndicatorToPlot(plotData, m_stockArr[0], plotData.axis, indicator, plotIndex, legendLable);
    }


    // Profit per share
    if(treeWidgetYCol == 1 && plotData.axis.maxY > 0 && plotData.axis.minY > 0)
    {
        m_plot.plotYAxisLogData(plotData, qwtPlot, plotIndex);
    }
    else
    {
        m_plot.plotDataSetXScaleStep(plotData, qwtPlot, plotIndex, 1);
    }


    legendLable = "";
    setAxisTitles(titleXAxis, legendLable, qwtPlot);

    // Reset zoomer
    m_plot.initPlotPicker(qwtPlot);
}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description: This function traverse treewidget
 *
 *******************************************************************/
void FaTab::getTreeWidgetData(CYahooStockPlotUtil::MinMaxAxisValues_ST &axis,
                              QTreeWidget *treeWidget,
                              CYahooStockPlotUtil::StockData_ST &data,
                              int colx,
                              int coly,
                              int indicator)
{
    CUtil cu;
    CYahooStockPlotUtil ypu;
    double x;
    double y;
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
 * Function:    setAxisTitles()
 *
 * Description:
 *
 *
 *******************************************************************/
void FaTab::setAxisTitles(QString titleXAxis, QString titleYAxis, QwtPlot *qwtPlot,bool showXAxis)
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
 *
 *
 *******************************************************************/
void FaTab::setFundametalAnalysisCtrlTxtColor(CDbHndl::snapshotStockData_ST keyData,
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

    setFaEditControlTxtColor(ui->lineEditEarningsDivByDividend_3, m_faDataPalette[FA_ERNING_DIV_DIVIDEN], color);


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
        color = Qt::black;
    }

    setFaEditControlTxtColor(ui->lineEditNavDivLastStockPrice_3, m_faDataPalette[FA_NAV_DIV_LAST_PRICE], color);



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
        ui->lineEditPE_3->setDisabled(true);
    }
    else
    {
        ui->lineEditPE_3->setDisabled(false);
    }

    setFaEditControlTxtColor(ui->lineEditPE_3, m_faDataPalette[FA_PE], color);




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


    setFaEditControlTxtColor(ui->lineEditYield_3, m_faDataPalette[FA_YIELD], color);


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

    setFaEditControlTxtColor(ui->lineEditPs_3, m_faDataPalette[FA_YIELD], color);
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
void FaTab::setFaEditControlTxtColor(QLineEdit *lineEdit, QPalette *palette, const QColor &color)
{
    palette->setColor(QPalette::Text, color); //Qt::red
    lineEdit->setPalette(*palette);
}



/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void FaTab::displayLeastSqrtFitData(CYahooStockPlotUtil::StockData_ST stockData,
                                    CYahooStockPlotUtil::PlotData_ST &outPlotData,
                                    QwtPlot *qwtPlot,
                                    int indicator,
                                    DataPlotIndex_ET plotIndex)
{
    QString legendLable;
    QString titleXAxis;


    m_stockArr.clear();
    outPlotData.axis.minMaxIsInit = false;
    outPlotData.nofStocksToPlot = 1;


    m_stockArr.insert(0, stockData);
    m_plot.addIndicatorToPlot(outPlotData, m_stockArr[0], outPlotData.axis, indicator, plotIndex, legendLable);


    // Profit per share
    if(outPlotData.axis.maxY > 0 && outPlotData.axis.minY > 0)
    {
        m_plot.plotYAxisLogData(outPlotData, qwtPlot, plotIndex);
    }
    else
    {
        m_plot.plotDataSetXScaleStep(outPlotData, qwtPlot, plotIndex, 1);
    }

    // m_plot.removeAllStockArrData(m_stockArr);

    legendLable = "";
    setAxisTitles(titleXAxis, legendLable, qwtPlot);

    // Reset zoomer
    m_plot.initPlotPicker(qwtPlot);
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
bool FaTab::createLeastSqrtFitPlotData(double k,
                                       double m,
                                       double minX,
                                       double maxX,
                                       CYahooStockPlotUtil::MinMaxAxisValues_ST &axis,
                                       CYahooStockPlotUtil::StockData_ST &data,
                                       int indicator)
{
    double y;
    double x;
    CYahooStockPlotUtil ypu;


    // x = minX;
    //for(int i = 0; i < 10; i++)
    for(int i = 0; i < 3; i++)
    {
        #if 1
        if(i == 0)
        {
            x = minX;
        }
        else if(i == 1)
        {
            x = maxX;
        }
        else if(i == 2)
        {
            x = (maxX + 5);
        }
        #endif


        y = k * x + m;
        qDebug() << "y" << y;
        qDebug() << "x" << x;

        data.data.x.insert(i, x);
        switch(indicator)
        {
        case 0:
            data.data.y.insert(i, y);
            break;
        case 1:
            data.data.indicator1.insert(i, y);
            break;
        case 2:
            data.data.indicator2.insert(i, y);
            break;
        case 3:
            data.data.indicator3.insert(i, y);
            break;
        case 4:
            data.data.indicator4.insert(i, y);
            break;
        default:
            return false;
        }

        ypu.updateMinMaxAxis(axis, x, y);
        //x++;
    }

    return true;
}



/*******************************************************************
 *
 * Function:    ()
 *
 * Description: This function traverse treewidget and check which
 *              of the nodes that are checked in column 1
 *
 *******************************************************************/
bool FaTab::getHistoricalMinMaxPrices(QTreeWidget *treeWidget,
                                      QVector<QwtIntervalSample> &rangeData,
                                      CYahooStockPlotUtil::MinMaxAxisValues_ST &axisMinMax)
{
    CUtil cu;
    double x;
    double minY;
    double maxY;
    CYahooStockPlotUtil cy;


    int nofTopItems = treeWidget->topLevelItemCount();

    if(nofTopItems < 1)
        return false;

    for (int i = 0 ; i < nofTopItems ; i++)
    {
        QTreeWidgetItem *item = treeWidget->topLevelItem(i);

        if(i== 0)
        {
            axisMinMax.minMaxIsInit = false;
        }

        if(true == cu.number2double(item->text(0), x) &&
           true == cu.number2double(item->text(1), minY) &&
           true == cu.number2double(item->text(3), maxY))
        {
            cy.addOneIntervalBarData(rangeData, axisMinMax, x, minY, maxY);
            qDebug() << "x" << x << "minY" << minY << "MaxY" << maxY;
        }
    }

    return true;
}






/*******************************************************************
 *
 * Function:    ()
 *
 * Description: This function traverse treewidget and check which
 *              of the nodes that are checked in column 1
 *
 *******************************************************************/
bool FaTab::calcLeastSqrtFit(QTreeWidget *treeWidget, double &k, double &m, double &minX, double &maxX, int colToUse)
{
    CUtil cu;
    CDbHndl db;
    int nofData = 0;
    double meanXSum = 0;
    double meanYSum = 0;
    double prodXXSum = 0;
    double prodYYSum = 0;
    double prodXYSum = 0;
    double x;
    double y;
    bool minMaxXIsInit = false;


    db.init1dLeastSrq(nofData, meanXSum, meanYSum, prodXXSum, prodYYSum, prodXYSum);


    int nofTopItems = treeWidget->topLevelItemCount();

    if(nofTopItems < 2)
        return false;

    for (int i = 0 ; i < nofTopItems ; i++)
    {
        QTreeWidgetItem *item = treeWidget->topLevelItem(i);


        if(true == cu.number2double(item->text(0), x) && true == cu.number2double(item->text(colToUse), y))
        {
            //x = (double) (nofTopItems - i);
            qDebug() << "xx" << x << "yy" << y;
            db.gather1dLeastSrqData(x,
                                    y,
                                    nofData,
                                    meanXSum,
                                    meanYSum,
                                    prodXXSum,
                                    prodYYSum,
                                    prodXYSum);

            if(minMaxXIsInit == false)
            {
                minMaxXIsInit = true;
                maxX = x;
                minX = x;
            }
            else
            {
                if(minX > x)
                    minX = x;

                if(maxX < x)
                    maxX = x;
            }
        }
    }

    if(nofData < 2)
        return false;


    if(false == db.calc1dLeastSrqFitParams(nofData, meanXSum, meanYSum, prodXXSum, prodYYSum, prodXYSum, m, k))
    {
       return false;
    }

    qDebug() << "k" << k << "m" << m;

    return true;
}



/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *******************************************************************/
void FaTab::clearStockAndIndicatorTempMem(void)
{
    m_stockData.data.x.clear();
    m_stockData.data.y.clear();
    m_stockData.data.xDate.clear();
    m_stockData.data.indicator1.clear();
    m_stockData.data.indicator2.clear();
    m_stockData.data.indicator3.clear();
    m_stockData.data.indicator4.clear();

    m_stockData1.data.x.clear();
    m_stockData1.data.y.clear();
    m_stockData1.data.xDate.clear();
    m_stockData1.data.indicator1.clear();
    m_stockData1.data.indicator2.clear();
    m_stockData1.data.indicator3.clear();
    m_stockData1.data.indicator4.clear();

    m_stockData2.data.x.clear();
    m_stockData2.data.y.clear();
    m_stockData2.data.xDate.clear();
    m_stockData2.data.indicator1.clear();
    m_stockData2.data.indicator2.clear();
    m_stockData2.data.indicator3.clear();
    m_stockData2.data.indicator4.clear();


    m_stockData3.data.x.clear();
    m_stockData3.data.y.clear();
    m_stockData3.data.xDate.clear();
    m_stockData3.data.indicator1.clear();
    m_stockData3.data.indicator2.clear();
    m_stockData3.data.indicator3.clear();
    m_stockData3.data.indicator4.clear();


    m_stockArr.clear();
}

/*******************************************************************
 *
 * Function:    updateMinMaxXAxisScales()
 *
 * Description:
 *
 *
 *******************************************************************/
void FaTab::updateMinMaxXAxisScales(CYahooStockPlotUtil::MinMaxAxisValues_ST &axis, CYahooStockPlotUtil::MinMaxAxisValues_ST &qwtPlotAxis)
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
void FaTab::
addSalesOrProfitGrowthPrognosData(QTreeWidget *treeWidget,
                                  double k,
                                  double m,
                                  double k1,
                                  double m1,
                                  double maxX)
{
    bool lastYIsInit = false;
    int i;
    int max;

    // Kr/shares
    double x;
    double y;

    // Milj Kr
    double y1;

    double lastY;

    QString xStr;
    QString yStr;
    QTreeWidgetItem *item1;

    max = 5;

    int nofTopItems = treeWidget->topLevelItemCount();
    if(nofTopItems > 0)
    {
        item1 = treeWidget->topLevelItem(0);
        lastY = item1->text(1).toDouble();
        lastYIsInit = true;
    }


    x = maxX + 1;
    for(i = 0; i < max; i++)
    {
        y = k * x + m;
        y1 = k1 * x + m1;

        xStr = xStr.sprintf("%.0f", x);
        yStr = yStr.sprintf("%.1f", y);
        QTreeWidgetItem *item = new QTreeWidgetItem;
        item->setText(0, xStr);
        item->setTextColor(0, Qt::darkRed);
        item->setText(1, yStr);
        item->setTextColor(1, Qt::darkRed);

        yStr = yStr.sprintf("%.0f", y1);
        item->setText(2, yStr);
        item->setTextColor(2, Qt::darkRed);

        if(lastYIsInit==true && lastY != 0)
        {
            yStr = yStr.sprintf("%.2f", ((y - lastY)/lastY)*100);
            item->setText(3, yStr);
            item->setTextColor(3, Qt::darkRed);
        }

        treeWidget->addTopLevelItem(item);
        lastY = y;
        //lastY1 = y1;
        lastYIsInit = true;
        x++;
    }
}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description: This function traverse treewidget and check which
 *              of the nodes that are checked in column 1
 *
 *******************************************************************/
bool FaTab::SalesOrProfitGrowthCalcAverage(QTreeWidget *treeWidget, QString result[], int maxCol)
{
    CUtil cu;
    int j;
    bool reIsInit = false;
    double res[10];
    double y;
    double minY;
    double maxY;

    if(maxCol >= 10)
    {
        return false;
    }




    int nofTopItems = treeWidget->topLevelItemCount();

    if(nofTopItems < 1)
        return false;

    for (int i = 0 ; i < nofTopItems ; i++)
    {
        QTreeWidgetItem *item = treeWidget->topLevelItem(i);

        for(j = 1; j < maxCol; j++)
        {
            if(true == cu.number2double(item->text(j), y))
            {
                if(reIsInit== false)
                {
                    res[j] = y;
                }
                else
                {
                    res[j] += y;
                }
            }
        }
        reIsInit = true;
    }

    for(j = 0; j < maxCol; j++)
    {
        if(j == 1)
            result[j] = result[j].sprintf("%.0f", res[j+1]/nofTopItems);
        else
            result[j] = result[j].sprintf("%.2f", res[j+1]/nofTopItems);

        qDebug() << result[j];
    }

    double growthRate;
    QTreeWidgetItem *item3 = treeWidget->topLevelItem(nofTopItems-1);
    QTreeWidgetItem *item4 = treeWidget->topLevelItem(0);
    if(true == cu.number2double(item3->text(1), minY) && true == cu.number2double(item4->text(1), maxY))
    {
        if(true == cu.annualGrowthRate(minY, maxY, (nofTopItems-1), growthRate))
        {
            growthRate = growthRate - 1;
            growthRate = growthRate * 100;
            result[maxCol-2] = result[maxCol-2].sprintf("%.1f\%%", growthRate);
        }
    }


    return true;
}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description: FA. Go from page 1 to page 2
 *
 *******************************************************************/
void FaTab::on_pushButNext1_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void FaTab::displayProfitGrowth(QTreeWidget *treeWidget, int coly)
{
    DataPlotIndex_ET plotIndex;
    QString legendLable;
    QString titleXAxis;
    int indicator;


    indicator = 0;
    m_profitStockData.data.x.clear();
    m_profitStockData.data.y.clear();
    m_stockArr.clear();

    m_qwtProfitGrowth.axis.minMaxIsInit = false;
    m_qwtProfitGrowth.nofStocksToPlot = 1;

    getTreeWidgetData(m_qwtProfitGrowth.axis, treeWidget, m_profitStockData, 0, coly, indicator);

    plotIndex = (DataPlotIndex_ET) coly;
    m_stockArr.insert(0, m_profitStockData);
    m_plot.addStockToPlot(m_qwtProfitGrowth, m_stockArr[0], m_qwtProfitGrowth.axis, plotIndex, legendLable);
    // updateMinMaxXAxisScales(m_qwtSalesAndProfitGrowth.axis, m_qwtProfitGrowth.axis);



    // Profit per share
    if(coly == 1 && m_qwtProfitGrowth.axis.maxY > 0 && m_qwtProfitGrowth.axis.minY > 0)
    {
        m_plot.plotYAxisLogData(m_qwtProfitGrowth, ui->qwtPlotProfitAnalysis, plotIndex);
    }
    else
    {
        m_plot.plotDataSetXScaleStep(m_qwtProfitGrowth, ui->qwtPlotProfitAnalysis, plotIndex, 1);
    }

    // m_plot.removeAllStockArrData(m_stockArr);

    legendLable = "";
    setAxisTitles(titleXAxis, legendLable, ui->qwtPlotProfitAnalysis);

    // Reset zoomer
    m_plot.initPlotPicker(ui->qwtPlotProfitAnalysis);
}


/*******************************************************************
 *
 * Function:
 *
 * Description:
 *
 *
 *******************************************************************/
void FaTab::initNetProfitAfterTaxList(void)
{

    QString column0 = QString::fromUtf8("År");
    QString column1 = QString("Kr/\nAktie");
    QString column2 = QString("MKr");
    QString column3 = QString::fromUtf8("Tillväxt (%)");

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
 * Function:
 *
 * Description:
 *
 *
 *******************************************************************/
void FaTab::on_pushButNextTab_2_clicked()
{
     ui->tabWidget->setCurrentIndex(2);
}


/*******************************************************************
 *
 * Function:    initStockList()
 *
 * Description:
 *
 *
 *******************************************************************/
void FaTab::initProfitabilityAnalysis(void)
{

    QString column0 = QString::fromUtf8("År");
    QString column1 = QString::fromUtf8("Vinst-\nmarginal(%)");
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
    ui->treeWidgetProfit->setColumnWidth(1, 100);
    ui->treeWidgetProfit->setColumnWidth(2, 65);
}



//======================== Page 3 =====================================
#if 0
/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void FaTab::displaySalesAndProfitGrowth(void)
{
    CYahooStockPlotUtil::MinMaxAxisValues_ST plotAxis;

    DataPlotIndex_ET plotIndex;
    QString legendLable;
    int indicator;
    int coly = 1;

    indicator = 0;
    m_stockArr.clear();

    m_qwtSalesAndProfitGrowth.axis.minMaxIsInit = false;
    m_qwtSalesAndProfitGrowth.nofStocksToPlot = 1;


    //===================================================================
    // Sales
    //===================================================================
    plotIndex = (DataPlotIndex_ET) 0;
    m_stockArr.insert(0, m_salesStockData);
    qDebug() << m_salesStockData.data.x.size();
    qDebug() << m_salesStockData.data.y.size();
    m_plot.addStockToPlot(m_qwtSalesAndProfitGrowth, m_stockArr[0], m_qwtSalesAndProfitGrowth.axis, plotIndex, legendLable);



    // Profit per share
    if(coly == 1 && m_qwtSalesAndProfitGrowth.axis.maxY > 0 && m_qwtSalesAndProfitGrowth.axis.minY > 0)
    {
        m_plot.plotYAxisLogData(m_qwtSalesAndProfitGrowth, ui->qwtPlotSalesAndProfitGrowth, plotIndex, 1);
    }
    else
    {
        m_plot.plotDataSetXScaleStep(m_qwtSalesAndProfitGrowth, ui->qwtPlotSalesAndProfitGrowth, plotIndex, 1);
    }


    //===================================================================
    // Profit
    //===================================================================
    plotIndex = (DataPlotIndex_ET) 1;
    m_stockArr.clear();
    m_stockArr.insert(0, m_profitStockData);
    m_plot.addStockToPlot(m_qwtSalesAndProfitGrowth, m_stockArr[0], m_qwtSalesAndProfitGrowth.axis, plotIndex, legendLable);


    // Profit per share
    if(coly == 1 && m_qwtSalesGrowth.axis.maxY > 0 && m_qwtSalesGrowth.axis.minY > 0)
    {
        m_plot.plotYAxisLogData(m_qwtSalesAndProfitGrowth, ui->qwtPlotSalesAndProfitGrowth, plotIndex, 1);
    }
    else
    {
        m_plot.plotDataSetXScaleStep(m_qwtSalesAndProfitGrowth, ui->qwtPlotSalesAndProfitGrowth, plotIndex, 1);
    }



    // Reset zoomer
    m_plot.initPlotPicker(ui->qwtPlotSalesAndProfitGrowth);
}
#endif


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void FaTab::displayProfitAnalysis(QTreeWidget *treeWidget, QwtPlot *qwtPlot, int coly)
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

    if(coly == 1)
        plotIndex = (DataPlotIndex_ET) (2);
    else if(coly == 2)
        plotIndex = (DataPlotIndex_ET) (5);
    else
        plotIndex = (DataPlotIndex_ET) (8);
    m_stockArr.insert(0, m_stockData);
    m_plot.addStockToPlot(m_qwtProfitAnalysis, m_stockArr[0], m_qwtProfitAnalysis.axis, plotIndex, legendLable);
    m_plot.plotDataSetXScaleStep(m_qwtProfitAnalysis, qwtPlot, plotIndex, 1);
    m_plot.removeAllStockArrData(m_stockArr);

    legendLable = "";
    setAxisTitles(titleXAxis, legendLable, qwtPlot);

    // Reset zoomer
    m_plot.initPlotPicker(qwtPlot);
}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void FaTab::on_pushButNextTabPg3_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
}


/*******************************************************************
 *
 * Function:    initStockList()
 *
 * Description:
 *
 *
 *******************************************************************/
void FaTab::initMinMaxPePrice(void)
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
void FaTab::displayHistorcalStockPrices(QTreeWidget *treeWidget, int coly, int indicator)
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
    m_plot.plotDataSetXScaleStep(m_qwtHistorcalStockPrices, ui->qwtPlotPePrice, m_qwtHistorcalStockPrices.nofStocksToPlot-1, 1);
    m_plot.removeAllStockArrData(m_stockArr);

    legendLable = "";
    setAxisTitles(titleXAxis, legendLable,ui->qwtPlotPePrice);

    // Reset zoomer
    m_plot.initPlotPicker(ui->qwtPlotPePrice);
}
