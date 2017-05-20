#include "stockAnalysisTab.h"
#include "ui_stockanalysistab.h"
#include "util.h"
#include "inc/guiUtil/guiFinanceColor.h"
#include "dbHndl.h"
#include "extendedLineEdit.h"
#include "yahooStockPlotUtil.h"
#include "createstockanalysishtmlpage.h"
#include <QtWebKit/QWebView>
#include "util.h"
#include "math.h"
#include <qwt_scale_engine.h>
#include "extendedQwtPlot.h"
#include <qwt_plot_histogram.h>

#define INDEX_MY_PORTFOLIO      ((int) 3)


//Sub Analys Tables defines
#define SAT_COLUMN_DATE                  0
#define SAT_COLUMN_DATA                  1
#define SAT_NOF_ROWS                    20
#define SAT_NOF_COLUMNS                  2
#define SAT_FONT_NAME                    QString::fromUtf8("Helvetica")
#define SAT_FONT_SIZE                    9




const QString StockAnalysisTab::m_companyTypesArr[NOF_COMPANY_TYPE_ARR_DATA] =
                            {QString::fromUtf8("Odefinierad"),
                            QString::fromUtf8("Handel"),
                            QString::fromUtf8("Industribolag"),
                            QString::fromUtf8("Bank"),
                            QString::fromUtf8("Fastighetsbolag"),
                            QString::fromUtf8("Försörjningsbolag"),
                            QString::fromUtf8("Investmentbolag"),
                            QString::fromUtf8("Allmännyttiga företag")}; // t.ex energibolag (Skulder/eget kapital exklusive immateriella tillgångar < 2)









/******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 *****************************************************************/
StockAnalysisTab::StockAnalysisTab(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StockAnalysisTab)
{

    QString path;

    m_red_palette = new QPalette();
    m_red_palette->setColor(QPalette::WindowText,Qt::red);

    m_blue_palette = new QPalette();
    m_blue_palette->setColor(QPalette::WindowText,Qt::blue);


    ui->setupUi(this);
    m_qwtcashFlowPlotData.nofStocksToPlot = 0;

    m_stockNameIsInit = false;

    initSubAnalysTables();

    m_gfc.initStockList1(ui->treeWidgetStockListAnalysis);
    m_gfc.addAllStockListsToCombobox(ui->StockListComboBoxAnalysis);
    ui->StockListComboBoxAnalysis->setCurrentIndex(INDEX_MY_PORTFOLIO);


    m_gfc.initStockAnalysisDateList(ui->treeWidgetAnalysisDate);

    on_SelStockListButton_clicked();

    // Show html file that describes investment strategy
    ui->webView_2->load(QUrl("qrc:/database/AnalysDoc/AnalysDoc/Investeringskriterier.html"));

    path = QString::fromUtf8("file:///");
    path += qApp->applicationDirPath();
    path += QString::fromUtf8("/");
    path += QString::fromUtf8("/database/AnalysDoc/AnalysisStockhomLargeMidCap.html");

    qDebug() << path;

    //qDebug() << "App path : " << qApp->applicationDirPath();

//    ui->webViewAllAnalyzedComp->load(QUrl("file:///home/ajn/Documents/OldPC/swProj/MyQtProj/JackStockProj/JackStock/database/AnalysDoc/Investeringskriterier.html"));
    ui->webViewAllAnalyzedComp->load(QUrl(path));


    // New plots
    initAllAnalysisPlots();

    // Old plots
    initSubAnalysisPlots();
}




/******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 *****************************************************************/
StockAnalysisTab::~StockAnalysisTab()
{


    delete ui;
    delete m_barHist;
    delete m_red_palette;
    delete m_blue_palette;
}


/******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 *****************************************************************/
void StockAnalysisTab::initSubAnalysisPlots(void)
{
    CExtendedQwtPlot eqp;


    // Hide axises
    m_qwtPlot[0] = ui->qwtPlot_0;
    m_qwtPlot[0]->enableAxis(QwtPlot::xBottom, false);
    m_qwtPlot[0]->enableAxis(QwtPlot::yLeft, false);
   // m_qwtPlot[0]->hide();

    m_qwtPlot[1] = ui->qwtPlot_1;
    m_qwtPlot[1]->enableAxis(QwtPlot::xBottom, false);
    m_qwtPlot[1]->enableAxis(QwtPlot::yLeft, false);

    m_qwtPlot[2] = ui->qwtPlot_2;
    m_qwtPlot[2]->enableAxis(QwtPlot::xBottom, false);
    m_qwtPlot[2]->enableAxis(QwtPlot::yLeft, false);

    m_qwtPlot[3] = ui->qwtPlot_3;
    m_qwtPlot[3]->enableAxis(QwtPlot::xBottom, false);
    m_qwtPlot[3]->enableAxis(QwtPlot::yLeft, false);

    m_qwtPlot[4] = ui->qwtPlot_4;
    m_qwtPlot[4]->enableAxis(QwtPlot::xBottom, false);
    m_qwtPlot[4]->enableAxis(QwtPlot::yLeft, false);

    m_qwtPlot[5] = ui->qwtPlot_5;
    m_qwtPlot[5]->enableAxis(QwtPlot::xBottom, false);
    m_qwtPlot[5]->enableAxis(QwtPlot::yLeft, false);

    m_qwtPlot[6] = ui->qwtPlot_6;
    m_qwtPlot[6]->enableAxis(QwtPlot::xBottom, false);
    m_qwtPlot[6]->enableAxis(QwtPlot::yLeft, false);

    m_qwtPlot[7] = ui->qwtPlot_7;
    m_qwtPlot[7]->enableAxis(QwtPlot::xBottom, false);
    m_qwtPlot[7]->enableAxis(QwtPlot::yLeft, false);

    m_qwtPlot[8] = ui->qwtPlot_8;
    m_qwtPlot[8]->enableAxis(QwtPlot::xBottom, false);
    m_qwtPlot[8]->enableAxis(QwtPlot::yLeft, false);

    m_qwtPlot[9] = ui->qwtPlot_9;
    m_qwtPlot[9]->enableAxis(QwtPlot::xBottom, false);
    m_qwtPlot[9]->enableAxis(QwtPlot::yLeft, false);

    // Is handle in difftent way ordinary graph
    ui->qwtCashFlowPlot->enableAxis(QwtPlot::xBottom, false);
    ui->qwtCashFlowPlot->enableAxis(QwtPlot::yLeft, false);

    eqp.initPlotPicker(ui->qwtPlot_10);
}



/******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 *****************************************************************/
void StockAnalysisTab::initSubAnalysTables(void)
{
    QString dateHeader = QString::fromUtf8("   År    ");
    QString dataHeader;

    initCompanyTypeComboBox();






    //------------------------------------------------------------------------------
    // Dividend
    dataHeader = QString::fromUtf8("Utdel/Aktie");
    initSubAnalyseTableWidget(ui->tableWidgetDividend, dateHeader, dataHeader);
    m_nofDividendArrData = 0;


    // Dividend (IntrinsicValue tab)
    dataHeader = QString::fromUtf8("Utdel/Aktie");
    initSubAnalyseTableWidget(ui->tableWidgetIntrinsicValueDividendPerShare, dateHeader, dataHeader);
    //------------------------------------------------------------------------------

    // TotDividends ((Total utdelning)
    dataHeader = QString::fromUtf8("Total utdelning");
    initSubAnalyseTableWidget(ui->tableWidgetTotDividends, dateHeader, dataHeader);
    m_nofTotDividensData = 0;

    //------------------------------------------------------------------------------
    // Earnings
    dataHeader = QString::fromUtf8("Vinst/Aktie");
    initSubAnalyseTableWidget(ui->tableWidgetEarnings, dateHeader, dataHeader);
    m_nofEarningsArrData = 0;

    // Earnings (IntrinsicValue tab)
    dataHeader = QString::fromUtf8("Vinst/Aktie");
    initSubAnalyseTableWidget(ui->tableWidgetIntrinsicValueEarningsPerShare, dateHeader, dataHeader);
    //------------------------------------------------------------------------------

    // Total Earnings, Net Income (Vinst)
    dataHeader = QString::fromUtf8("     Vinst      ");
    initSubAnalyseTableWidget(ui->tableWidgetNetIncome, dateHeader, dataHeader);
    m_nofTotEarningsArrData = 0;


    //------------------------------------------------------------------------------
    // TotalCurrentAssets
    dataHeader = QString::fromUtf8("Oms.tillg");
    initSubAnalyseTableWidget(ui->tableWidgetTotalCurrentAsset, dateHeader, dataHeader);
    m_nofTotalCurrentAssetsArrData = 0;

    // totalCurrentLiabilities
    dataHeader = QString::fromUtf8("Kortf.skuld");
    initSubAnalyseTableWidget(ui->tableWidgetTotalCurrentLiabilities, dateHeader, dataHeader);
    m_nofTotalCurrentLiabilitiesData = 0;

    // totalLiabilities
    dataHeader = QString::fromUtf8("Totala Skuld");
    initSubAnalyseTableWidget(ui->tableWidgetTotalLiabilities, dateHeader, dataHeader);
    m_nofTotalLiabilitiesData = 0;


    // TotalCurrentAssets/totalLiabilities (IntrinsicValue tab)
    dataHeader = QString::fromUtf8("O.tillg/T.Skuld");
    initSubAnalyseTableWidget(ui->tableWidgetIntrinsicValueTotalCurrentAssetDivTotalLiabilities, dateHeader, dataHeader);
    //------------------------------------------------------------------------------


    // Solidity
    dataHeader = QString::fromUtf8("Soliditet (%)");
    initSubAnalyseTableWidget(ui->tableWidgetSolidity, dateHeader, dataHeader);
    m_nofSolidityData = 0;


    // Coverage Ratio (Räntetäckningsgrad)
    dataHeader = QString::fromUtf8("RänteTG ggr");
    initSubAnalyseTableWidget(ui->tableWidgetCoverageRatio, dateHeader, dataHeader);
    m_nofCoverageRatioData = 0;


    // Core Tier1 Ratio (Primärkapitalrelation)
    dataHeader = QString::fromUtf8("primärkap.rel");
    initSubAnalyseTableWidget(ui->tableWidgetCoreTier1Ratio, dateHeader, dataHeader);
    m_nofCoreTier1RatioData = 0;


    // CoreCapitalRatio (Kärnprimärkapitalrelation)
    dataHeader = QString::fromUtf8("kärnprimärkap.rel");
    initSubAnalyseTableWidget(ui->tableWidgetCoreCapitalRatio, dateHeader, dataHeader);
    m_nofCoreCapitalRatioData = 0;


    // Equity (Eget kapital)
    dataHeader = QString::fromUtf8("Eget kapital");
    initSubAnalyseTableWidget(ui->tableWidgetEquity, dateHeader, dataHeader);
    m_nofTotEquityData = 0;


    // EquityPerShare (Eget kapital/Aktie)
    dataHeader = QString::fromUtf8("Eget kapital/Aktie");
    initSubAnalyseTableWidget(ui->tableWidgetEquityPerShare, dateHeader, dataHeader);
    m_nofEquityPerShareData = 0;


    // CashFlowCapex ((Kassaflöde Capex Investeringar/kapitalutgifter))
    dataHeader = QString::fromUtf8("    Capex    ");
    initSubAnalyseTableWidget(ui->tableWidgetCashFlowCapex, dateHeader, dataHeader);
    m_nofCashFlowCapexData = 0;


    // CashFlowCapex ((operativt kassaflöde)
    dataHeader = QString::fromUtf8("Operativt kflöde");
    initSubAnalyseTableWidget(ui->tableWidgetOperatingCashFlow, dateHeader, dataHeader);
    m_nofOperatingCashFlowData = 0;


    // Revenue (Försäljning)
    dataHeader = QString::fromUtf8("Försäljning");
    initSubAnalyseTableWidget(ui->tableWidgetRevenue, dateHeader, dataHeader);
    m_nofRevenueArrData = 0;
}


//TotDividendsSubAnalysis (Total utdelning

/******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 *****************************************************************/
void StockAnalysisTab::initCompanyTypeComboBox(void)
{
    ui->comboBoxCompanyType->clear();

    for(int i = 0; i < NOF_COMPANY_TYPE_ARR_DATA; i++)
    {
        ui->comboBoxCompanyType->addItem(m_companyTypesArr[i]);
    }

    ui->comboBoxCompanyType->setCurrentIndex(0);

}




/******************************************************************
 *
 * Function:    on_SelStockListButton_clicked()
 *
 * Description: init Sub Analyse QTableWidget.
 *
 *              This table has 20 rows and 2 columns
 *
 * ****************************************************************/
void StockAnalysisTab::initSubAnalyseTableWidget(QTableWidget *tableWidget,
                                                 QString dateHeader,
                                                 QString dataHeader)
{

    // Set table size
    tableWidget->setRowCount(SAT_NOF_ROWS);
    tableWidget->setColumnCount(SAT_NOF_COLUMNS);

    // Init label
    QStringList labels;
    labels.clear();
    labels << dateHeader << dataHeader;
    tableWidget->setHorizontalHeaderLabels(labels);

    // Init headers
    tableWidget->horizontalHeader()->setResizeMode(SAT_COLUMN_DATE, QHeaderView::ResizeToContents);
    tableWidget->horizontalHeader()->setResizeMode(SAT_COLUMN_DATA, QHeaderView::ResizeToContents);

    // Init font
    QFont font;
    font = tableWidget->horizontalHeader()->font();
    font.setPointSize(SAT_FONT_SIZE);
    font.setFamily(SAT_FONT_NAME);
    tableWidget->horizontalHeader()->setFont( font );
    tableWidget->verticalHeader()->setFont( font );

}



/******************************************************************
 *
 * Function:    on_SelStockListButton_clicked()
 *
 * Description: Invoked when user pressing Select stock list button
 *
 *
 *
 * ****************************************************************/
void StockAnalysisTab::on_SelStockListButton_clicked()
{
    CDbHndl db;

     m_stockListName = ui->StockListComboBoxAnalysis->currentText().toUtf8();

     qDebug() << m_stockListName;

     if(true == db.findTaStockListId(m_stockListName, m_stockListId))
     {
         ui->treeWidgetStockListAnalysis->clear();
         db.addStockSymbolAndNameInTreeWidget1(ui->treeWidgetStockListAnalysis,
                                            m_stockListId,
                                            CDbHndl::SORT_TWIDGET_NAME,
                                            (char *) SQL_STR_ASC);


         ui->treeWidgetStockListAnalysis->setColumnWidth(0, 160);
         ui->treeWidgetStockListAnalysis->scrollToTop();
         ui->treeWidgetStockListAnalysis->setCurrentItem(ui->treeWidgetStockListAnalysis->topLevelItem(0));

     }

}


/******************************************************************
 *
 * Function:    on_treeWidgetStockListAnalysis_doubleClicked()
 *
 * Description: Select stock to analyse and display all date of analysis
 *
 *
 *
 *
 *****************************************************************/
void StockAnalysisTab::on_treeWidgetStockListAnalysis_doubleClicked(const QModelIndex &index)
{
    CDbHndl db;
    CUtil cu;
    QString startDate;
    QString riskStr;
    QString returnStr;
    int intMonth = -12;
    bool res;
    GuiFinanceColor gfc;
    ExtendedLineEdit ele;
    QPalette palette[10];
    QColor gColor;

    Qt::GlobalColor color = Qt::magenta;
    CDbHndl::EfficPortStockData_ST riskReturnData;
    CDbHndl::snapshotStockData_ST faData;
    CDbHndl::YahooNordnetOutputkeyData_ST yahooKeyData;
    NordnetCompanyDescription_ST companyDescripData;


    cu.getCurrentDate(m_analysisDate);
    cu.getCurrentDate(startDate);

    ui->analysisDateLineEdit->setText(m_analysisDate);

    if(false == cu.addMonth(m_analysisDate, startDate, intMonth))
    {
        return;
    }

     resetGuiCtrl();
     clearGUIIntrinsicValue();

    m_gfc.getSelStockListItem(ui->treeWidgetStockListAnalysis, m_stockName, m_stockSymbol, index);
    ui->stockNameLineEdit->setText(m_stockName);
    ui->stockSymbolLineEdit->setText(m_stockSymbol);

    if(m_stockName.size() > 0 && m_stockSymbol.size() > 0)
    {
        m_stockNameIsInit = true;
        db.getStockAnalysisDate(m_stockName, m_stockSymbol, ui->treeWidgetAnalysisDate);
    }

    // Page analysdata 1
    ui->StockNameLabel_16->setText(m_stockName);
    ui->ananlysisDateLabel_32->setText(m_analysisDate);

    // Page analysdata 2
    ui->StockNameLabel_17->setText(m_stockName);
    ui->ananlysisDateLabel_33->setText(m_analysisDate);

    if(startDate.isEmpty() == false && m_analysisDate.isEmpty() == false)
    {
        if(true == db.getKeyDataUseBridge(m_stockSymbol, faData))
        {
            gfc.getColorEarningsDivDividend(faData.earningsDividedByDividend, color);
            ui->lineEditEarningsDivByDividend->setText(faData.earningsDividedByDividend);
            ele.setTxtColor(ui->lineEditEarningsDivByDividend, &palette[0], color);

            gfc.getColorNavDivStockPrice(faData.navDivLastStockPrice, color);
            ui->lineEditNavDivLastStockPrice->setText(faData.navDivLastStockPrice);
            ele.setTxtColor(ui->lineEditNavDivLastStockPrice, &palette[1], color);

            gfc.getColorPe(faData.keyValuePE, color);
            ui->lineEditPE->setText(faData.keyValuePE);
            ele.setTxtColor(ui->lineEditPE, &palette[2], color);

            gfc.getColorPs(faData.keyValuePS, color);
            ui->lineEditPs->setText(faData.keyValuePS);
            ele.setTxtColor(ui->lineEditPs, &palette[3], color);

            gColor = gfc.getColorYield(faData.keyValueYield, faData.earningsDividedByDividend);
            ui->lineEditYield->setText(faData.keyValueYield);
            ele.setTxtColor(ui->lineEditYield, &palette[4], gColor);

        }


        if(true == db.getNordnetCompanyDescription(companyDescripData, faData.companyName))
        {
            //ui->textEditMoreInfobusinessDescription->clear();

            //ui->lineEditMoreInfoCompanyName->setText(companyDescripData.assetName);
            //ui->lineEditMoreInfoexecutiveDirector->setText(companyDescripData.executiveDirector);
            //ui->lineEditMoreInfoBransch->setText(companyDescripData.bransch);
            //ui->lineEditMoreInfoSector->setText(companyDescripData.sector);
            //ui->textEditMoreInfobusinessDescription->insertPlainText(companyDescripData.businessDescription);

        }

    }

    m_riskStdDev.clear();
    m_meanReturns.clear();
    m_returnOnEquity.clear();


    riskReturnData.stockSymbol = m_stockSymbol;
    res = m_gfc.calcRiskAndReturn(startDate, m_analysisDate, riskReturnData);

    if(true == res)
    {
        color = gfc.getColorRiskReturns(riskReturnData.riskStdDev, riskReturnData.meanReturns);

        riskStr.sprintf("%.3f", riskReturnData.riskStdDev);

        returnStr.sprintf("%.3f", riskReturnData.meanReturns);

        ui->lineEditExpectedReturn->setText(returnStr);
        ui->lineEditExpectedRisk->setText(riskStr);

        m_riskStdDev = riskStr;
        m_meanReturns = returnStr;


        ele.setTxtColor(ui->lineEditExpectedReturn, &palette[5], color);
        ele.setTxtColor(ui->lineEditExpectedRisk, &palette[6], color);
    }


    if(true == db.getNordnetYahooSingleKeyData(m_stockSymbol,
                                      m_stockListId,
                                      m_stockListName,
                                      yahooKeyData))
    {

        gfc.getColorCurrentRatio(yahooKeyData.currentRatio, color);
        ui->lineEditCurrentRatio->setText(yahooKeyData.currentRatio);
        ele.setTxtColor(ui->lineEditCurrentRatio, &palette[7], color);


        gfc.getColorDebtToEquityRatio(yahooKeyData.totalDebtToEquityRatio, color);
        ui->lineEditTotDebtEquityRatio->setText(yahooKeyData.totalDebtToEquityRatio);
        ele.setTxtColor(ui->lineEditTotDebtEquityRatio, &palette[8], color);

        ui->lineEditReturnOnEquity->setText(yahooKeyData.returnOnEquity);
        m_returnOnEquity = yahooKeyData.returnOnEquity;
    }


    //====================================================================
    // Get and display sub data
    //====================================================================


    updateTableWithSubAnalysData(ui->tableWidgetNetIncome,
                                 SAD_TOT_EARNINGS,
                                 m_totEarningsDataArr,
                                 m_nofTotEarningsArrData);


    updateTableWithSubAnalysData(ui->tableWidgetRevenue,
                                 SAD_REVENUE,
                                 m_revenueDataArr,
                                 m_nofRevenueArrData);

    //------------------------------------------------------

    updateTableWithSubAnalysData(ui->tableWidgetIntrinsicValueDividendPerShare,
                                 SAD_DIVIDEND,
                                 m_dividendDataArr,
                                 m_nofDividendArrData);

    // Update intrinsic Line edit ctrl
    if(m_nofDividendArrData > 1)
    {
        ui->lineEditEstimateYearlyDividend->setText(m_dividendDataArr[m_nofDividendArrData-1].data);
    }

    updateTableWithSubAnalysData(ui->tableWidgetDividend,
                                 SAD_DIVIDEND,
                                 m_dividendDataArr,
                                 m_nofDividendArrData);
    //------------------------------------------------------


    //------------------------------------------------------

    updateTableWithSubAnalysData(ui->tableWidgetIntrinsicValueEarningsPerShare,
                                 SAD_EARNINGS,
                                 m_earningsDataArr,
                                 m_nofEarningsArrData);

    updateTableWithSubAnalysData(ui->tableWidgetEarnings,
                                 SAD_EARNINGS,
                                 m_earningsDataArr,
                                 m_nofEarningsArrData);


     //------------------------------------------------------

    updateTableWithSubAnalysData(ui->tableWidgetTotalCurrentAsset,
                                 SAD_TOTAL_CURRENT_ASSETS,
                                 m_totalCurrentAssetsArr,
                                 m_nofTotalCurrentAssetsArrData);

    updateTableWithSubAnalysData(ui->tableWidgetTotalCurrentLiabilities,
                                 SAD_TOTAL_CURRENT_LIABILITIES,
                                 m_totalCurrentLiabilitiesArr,
                                 m_nofTotalCurrentLiabilitiesData);

    updateTableWithSubAnalysData(ui->tableWidgetTotalLiabilities,
                                 SAD_TOTAL_LIABILITIES,
                                 m_totalLiabilitiesArr,
                                 m_nofTotalLiabilitiesData);

    insertTableWidgetIntrinsicValueTotCurrAssetDivTotLiabilities(ui->tableWidgetIntrinsicValueTotalCurrentAssetDivTotalLiabilities,
                                                                 m_totalCurrentAssetsArr,
                                                                 m_nofTotalCurrentAssetsArrData,
                                                                 m_totalLiabilitiesArr,
                                                                 m_nofTotalLiabilitiesData);


    //---------------------------------------------------------------------------


    updateTableWithSubAnalysData(ui->tableWidgetSolidity,
                                 SAD_SOLIDITY,
                                 m_solidityArr,
                                 m_nofSolidityData);


    updateTableWithSubAnalysData(ui->tableWidgetCoverageRatio,
                                 SAD_COVERAGE_RATIO,
                                 m_coverageRatioArr,
                                 m_nofCoverageRatioData);

    updateTableWithSubAnalysData(ui->tableWidgetCoreTier1Ratio,
                                 SAD_CORE_TIER_1_RATIO,
                                 m_coreTier1RatioArr,
                                 m_nofCoreTier1RatioData);

    updateTableWithSubAnalysData(ui->tableWidgetCoreCapitalRatio,
                                 SAD_CORE_CAPITAL_RATIO,
                                 m_coreCapitalRatioArr,
                                 m_nofCoreCapitalRatioData);

    updateTableWithSubAnalysData(ui->tableWidgetEquity,
                                 SAD_EQUITY,
                                 m_totEquityArr,
                                 m_nofTotEquityData);

    updateTableWithSubAnalysData(ui->tableWidgetEquityPerShare,
                                 SAD_EQUITY_PER_SHARE,
                                 m_equityPerShareArr,
                                 m_nofEquityPerShareData);

    if(m_nofEquityPerShareData > 1)
    {
        // Update Intrinsic value line edit ctrl
        ui->lineEditCurrEquityPerShare->setText(m_equityPerShareArr[m_nofEquityPerShareData-1].data);
    }



    if(m_nofEquityPerShareData > 1)
    {
        plotEquityPerShareData(m_equityPerShareArr, m_nofEquityPerShareData);
    }




    updateTableWithSubAnalysData(ui->tableWidgetCashFlowCapex,
                                 SAD_CASH_FLOW_CAPEX,
                                 m_cashFlowCapexArr,
                                 m_nofCashFlowCapexData);

    updateTableWithSubAnalysData(ui->tableWidgetOperatingCashFlow,
                                 SAD_OPERATING_CASH_FLOW,
                                 m_operatingCashFlowArr,
                                 m_nofOperatingCashFlowData);

    updateTableWithSubAnalysData(ui->tableWidgetTotDividends,
                                 SAD_TOTAL_DIVIDENT,
                                 m_totDividensArr,
                                 m_nofTotDividensData);


    m_saDisply.subAnalysisClearAllGraphs(m_qwtPlot);
    plotCashflowData();

    calcTotSubdataForIntrinsicValue();
    on_pushButtonCalcYearlyIntrestRateOnEquity_clicked();

    int mainAnalysisId;
    int companyType = 0;

    // Reset Company type
    ui->labelCompanyType->setText(m_companyTypesArr[companyType]);
    ui->labelMainPglabelCompanyType->setText(m_companyTypesArr[companyType]);

    ui->comboBoxCompanyType->setCurrentIndex(companyType);

    if(true == db.mainAnalysisDataExists(m_stockName, m_stockSymbol, mainAnalysisId))
    {
        if(true ==  db.getSubAnalysisCompanyType(mainAnalysisId, companyType))
        {
            if(companyType < NOF_COMPANY_TYPE_ARR_DATA)
            {
                ui->labelCompanyType->setText(m_companyTypesArr[companyType]);
                ui->labelMainPglabelCompanyType->setText(m_companyTypesArr[companyType]);
                ui->comboBoxCompanyType->setCurrentIndex(companyType);
            }
        }
    }

    // Display all analysis graphs
    displayAllAnalysisPlots();


}


/******************************************************************
 *
 * Function:    on_pushButton_clicked()
 *
 * Description: This function updates table Widget with
 *              Sub Analys Data from the database.
 *
 *
 *
 *
 *****************************************************************/
void StockAnalysisTab::updateTableWithSubAnalysData(QTableWidget *tableWidget,
                                                    SubAnalyseDataTypeET analyseType,
                                                    SubAnalysDataST *subAnalysDataArr,
                                                    int &nofArrData)
{
    CDbHndl db;
    bool res = false;
    SubAnalysDataST data;
    QFont font;

    // Set table widget font
    font.setPointSize(SAT_FONT_SIZE);
    font.setFamily(SAT_FONT_NAME);

    // Clear table widget
    tableWidget->clearContents();



    switch(analyseType)
    {
    case SAD_TOT_EARNINGS:
        res = db.getSubAnalysisTotEarningsData(m_stockName, m_stockSymbol, subAnalysDataArr, nofArrData);
        if(nofArrData > MAX_NOF_TOT_EARNINGS)
        {
            QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Error 1: Too many array data"));
        }
        break;


    case SAD_REVENUE:
        res = db.getSubAnalysisRevenueData(m_stockName, m_stockSymbol, subAnalysDataArr, nofArrData);
        if(nofArrData > MAX_NOF_REVENUE)
        {
            QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Error 1: Too many array data"));
        }
        break;

    case SAD_DIVIDEND:
        res = db.getSubAnalysisDividendData(m_stockName, m_stockSymbol, subAnalysDataArr, nofArrData);
        if(nofArrData > MAX_NOF_DIVIDEND_ARR_DATA)
        {
            QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Error 1: Too many array data"));
        }
        break;

    case SAD_EARNINGS:
        res = db.getSubAnalysisEarningsPerShareData(m_stockName, m_stockSymbol, subAnalysDataArr, nofArrData);
        if(nofArrData > MAX_NOF_EARNINGS_ARR_DATA)
        {
            QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Error 2: Too many array data"));
        }
        break;

    case SAD_TOTAL_CURRENT_ASSETS:
        res = db.getSubAnalysisTotalCurrentAssetsData(m_stockName, m_stockSymbol, subAnalysDataArr, nofArrData);
        if(nofArrData > MAX_NOF_TOTAL_CURRENT_ASSETS_ARR_DATA)
        {
            QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Error 3: Too many array data"));
        }
        break;

    case SAD_TOTAL_CURRENT_LIABILITIES:
        res = db.getSubAnalysisTotalCurrentLiabilitiesData(m_stockName, m_stockSymbol, subAnalysDataArr, nofArrData);
        if(nofArrData > MAX_NOF_TOTAL_CURRENT_LIABILITIES)
        {
            QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Error 4: Too many array data"));
        }
        break;

    case SAD_TOTAL_LIABILITIES:
        res = db.getSubAnalysisTotalLiabilitiesData(m_stockName, m_stockSymbol, subAnalysDataArr, nofArrData);
        if(nofArrData > MAX_NOF_TOTAL_LIABILITIES)
        {
            QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Error 5: Too many array data"));
        }
        break;

   case SAD_SOLIDITY:
        res = db.getSubAnalysisSolidityData(m_stockName, m_stockSymbol, subAnalysDataArr, nofArrData);
        if(nofArrData > MAX_NOF_SOLIDITY)
        {
            QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Error 6: Too many array data"));
        }
        break;

    case SAD_COVERAGE_RATIO:
        res = db.getSubAnalysisCoverageRatioData(m_stockName, m_stockSymbol, subAnalysDataArr, nofArrData);
        if(nofArrData > MAX_NOF_COVERAGE_RATIO)
        {
            QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Error 7: Too many array data"));
        }
        break;

    case SAD_CORE_TIER_1_RATIO:
        res = db.getSubAnalysisCoreTier1RatioData(m_stockName, m_stockSymbol, subAnalysDataArr, nofArrData);
        if(nofArrData > MAX_NOF_CORE_TIER_1_RATIO)
        {
            QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Error 7: Too many array data"));
        }
        break;

    case SAD_CORE_CAPITAL_RATIO:
        res = db.getSubAnalysisCoreCapitalRatioData(m_stockName, m_stockSymbol, subAnalysDataArr, nofArrData);
        if(nofArrData > MAX_NOF_CORE_CAPITAL_RATIO)
        {
            QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Error 7: Too many array data"));
        }
        break;

   case SAD_EQUITY:
        res = db.getSubAnalysisEquityData(m_stockName, m_stockSymbol, subAnalysDataArr, nofArrData);
        if(nofArrData > MAX_NOF_EQUITY)
        {
            QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Error 8: Too many array data"));
        }
        break;

    case SAD_EQUITY_PER_SHARE:
         res = db.getSubAnalysisEquityPerShareData(m_stockName, m_stockSymbol, subAnalysDataArr, nofArrData);
         if(nofArrData > MAX_NOF_EQUITY_PER_SHARE)
         {
             QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Error 8: Too many array data"));
         }
         break;

    case SAD_CASH_FLOW_CAPEX:
         res = db.getSubAnalysisCashFlowCapexData(m_stockName, m_stockSymbol, subAnalysDataArr, nofArrData);
         if(nofArrData > MAX_NOF_CASH_FLOW_CAPEX)
         {
             QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Error 9: Too many array data"));
         }
         break;


    case SAD_OPERATING_CASH_FLOW:
         res = db.getSubAnalysisOperatingCashFlowData(m_stockName, m_stockSymbol, subAnalysDataArr, nofArrData);
         if(nofArrData > MAX_NOF_OPERATING_CASH_FLOW)
         {
             QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Error 10: Too many array data"));
         }
         break;

    case SAD_TOTAL_DIVIDENT:
         res = db.getSubAnalysisTotDividendsData(m_stockName, m_stockSymbol, subAnalysDataArr, nofArrData);
         if(nofArrData > MAX_NOF_TOT_DIVIDENT)
         {
             QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Error 11: Too many array data"));
         }
         break;



    default:
        QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Error: invalid subAnalyseType"));
        return;
    }


    if(true == res)
    {
        for(int i = 0; i < nofArrData; i++)
        {
            data = subAnalysDataArr[i];
            tableWidget->insertRow(i);

            // Insert Year in table widget
            QTableWidgetItem *itemDate = new QTableWidgetItem( data.date);
            itemDate->setFont(font);
            tableWidget->setItem( i, SAT_COLUMN_DATE, itemDate);

            // Insert Data in table widget
            QTableWidgetItem *itemData = new QTableWidgetItem(data.data);
            itemData->setFont(font);
            tableWidget->setItem( i, SAT_COLUMN_DATA, itemData);
        }
    }
}



/******************************************************************
 *
 * Function:    insertTableWidgetIntrinsicValueTotCurrAssetDivTotLiabilities()
 *
 * Description: This function insert data in:
 *
 *              TableWidgetIntrinsicValueTotalCurrentAssetDivTotalLiabilities[]
 *
 *
 *
 *
 *****************************************************************/
bool StockAnalysisTab::
insertTableWidgetIntrinsicValueTotCurrAssetDivTotLiabilities(QTableWidget *tableWidget,
                                                             SubAnalysDataST *totalCurrentAssetArr,
                                                             int nofTotalCurrentAssetArrData,
                                                             SubAnalysDataST *totalLiabilities,
                                                             int nofTotalLiabilitiesArrData)
{
    int i;
    QString dataString;

    QFont font;

    // Set table widget font
    font.setPointSize(SAT_FONT_SIZE);
    font.setFamily(SAT_FONT_NAME);

    // Clear table widget
    tableWidget->clearContents();


    // Do both arrays have equal amount of data?
    if(nofTotalCurrentAssetArrData != nofTotalLiabilitiesArrData)
    {
        return false;
    }


    // Is data from same years?
    for(i = 0; i < nofTotalCurrentAssetArrData; i++)
    {
        if(totalCurrentAssetArr[i].date.toInt() != totalLiabilities[i].date.toInt())
        {
            return false;
        }

    }


    // Is data from same years?
    for(i = 0; i < nofTotalCurrentAssetArrData; i++)
    {
        if(totalLiabilities[i].data.toDouble() != 0)
        {
           dataString.sprintf("%.2f", totalCurrentAssetArr[i].data.toDouble()/totalLiabilities[i].data.toDouble());
        }
        else
        {
            // Do not divide by zero
            dataString.sprintf("%.2f", totalCurrentAssetArr[i].data.toDouble()/0.001);
        }


        // data = subAnalysDataArr[i];
        tableWidget->insertRow(i);

        // Insert Year in table widget
        QTableWidgetItem *itemDate = new QTableWidgetItem(totalCurrentAssetArr[i].date);
        itemDate->setFont(font);
        tableWidget->setItem( i, SAT_COLUMN_DATE, itemDate);

        // Insert Data in table widget
        QTableWidgetItem *itemData = new QTableWidgetItem(dataString);
        itemData->setFont(font);
        tableWidget->setItem( i, SAT_COLUMN_DATA, itemData);
    }

    return true;

}


/******************************************************************
 *
 * Function:    on_pushButton_clicked()
 *
 * Description: This function saves data to db
 *
 *
 *
 *
 *****************************************************************/
void StockAnalysisTab::on_pushButton_clicked()
{

    CDbHndl db;
    bool res;
    int mainAnalysisId;
    int analysisDateId;
    QString str;
    HtmlStockAnalysPageDataST hSAPData;

    str = (QString::fromUtf8("Vill du lägga till data?\n"));
    str = str + m_stockName;
    str = str + ", ";
    str = str + m_stockSymbol;
    str = str + ", ";
    str = str + m_analysisDate;


    if(QMessageBox::No == QMessageBox::question(this, QString::fromUtf8("Uppdatera databas"), str, QMessageBox::Yes|QMessageBox::No))
    {
        return;
    }

    // Check if this stocksymbol and stockname is already added
    // If not add it
    res = db.mainAnalysisDataExists(m_stockName,
                                m_stockSymbol,
                                mainAnalysisId);

    hSAPData.stockName = m_stockName;
    hSAPData.stockSymbol = m_stockSymbol;
    hSAPData.analysisDate = m_analysisDate;


    if(false == res)
    {
        res = db.insertMainAnalysisData(m_stockName,
                                     m_stockSymbol,
                                     mainAnalysisId);
    }


    if(true == res)
    {
        res = db.mainAnalysisDateExists(m_analysisDate,
                                     mainAnalysisId,
                                     analysisDateId);

    }

    // Exist anaysis date?
    if(false == res)
    {
        res = db.insertMainAnalysisDate(m_analysisDate,
                                     mainAnalysisId,
                                     analysisDateId);

    }


    if(true == res)
    {
        hSAPData.companyDescription = ui->textEditCompDescription->toPlainText();

        hSAPData.bigEnoughAnswer = ui->lineEditBigEnoughAnswer->text();
        hSAPData.bigEnoughComment = ui->textEditBigEnoughText->toPlainText();

        hSAPData.strongFinancialPositionAnswer = ui->lineEditFinancialStrongAnswer->text();
        hSAPData.strongFinancialPositionComment = ui->textEditFinancialStrongText->toPlainText();

        hSAPData.earningStabilityAnswer = ui->lineEditErningStabilityAnswer->text();
        hSAPData.earningStabilityComment = ui->textEditErningStabilityText->toPlainText();

        hSAPData.dividendStabilityAnswer = ui->lineEditDividentStabilityAnswer->text();
        hSAPData.dividendStabilityComment =ui->textEditDividentStabilityText->toPlainText();

        hSAPData.earningGrowthAnswer = ui->lineEditErningGrowthAnswer->text();
        hSAPData.earningGrowthComment = ui->textEditErningGrowthText->toPlainText();


        hSAPData.keyValuePe = ui->lineEditPE->text();
        hSAPData.keyValuePs = ui->lineEditPs->text();
        hSAPData.keyValueNavPriceRatio = ui->lineEditNavDivLastStockPrice->text();
        hSAPData.keyValueYield = ui->lineEditYield->text();
        hSAPData.keyValueCurrentRatio = ui->lineEditCurrentRatio->text();
        hSAPData.keyValueTotalDebtEquityRatio = ui->lineEditTotDebtEquityRatio->text();

        // Saknas m_keyValuePriceJEKRatio;
        hSAPData.keyValueErningsDividentRatio = ui->lineEditEarningsDivByDividend->text();

        hSAPData.trustworthyLeadershipAnswer = ui->lineEditTrustworthyManagementAnswer->text();
        hSAPData.trustworthyLeadershipComment =  ui->textEditTrustworthyManagementText->toPlainText();

        hSAPData.goodOwnershipAnswer = ui->lineEditBeneficialOwnershipAnswer->text();
        hSAPData.goodOwnershipComment = ui->textEditBeneficialOwnershipText->toPlainText();

        hSAPData.otherInformation = ui->textEditOtherInfo->toPlainText();

        hSAPData.riskStdDev = m_riskStdDev;
        hSAPData.meanReturns = m_meanReturns;

        // IntrinsicValue
        hSAPData.tenYearNoteYield = ui->lineEditTenYearNoteYield->text();
        hSAPData.currEquityPerShare = ui->lineEditCurrEquityPerShare->text();
        hSAPData.estimateYearlyDividend = ui->lineEditEstimateYearlyDividend->text();
        hSAPData.calcIntrinsicValue = ui->lineEditIntrinsicValueResult->text();
        hSAPData.intrinsicValueYearSpan = m_equityPerShareArr[0].date;
        hSAPData.intrinsicValueYearSpan += " - ";
        hSAPData.intrinsicValueYearSpan += m_equityPerShareArr[m_nofEquityPerShareData-1].date;
        hSAPData.historicalYearlyInterestOnEquity = ui->lineEditHistoricalInterestOnEquity->text();


        int inputAnalysisDataId;
        int analysisDataId;
        bool inputAnalysisDataIdIsValid = false;

        if( true == db.getAnalysisDataId(mainAnalysisId, analysisDateId, inputAnalysisDataId))
        {
           //QString responsStr;
           inputAnalysisDataIdIsValid = true;

           //responsStr.sprintf("Data finns redan analysisDataId = %d", inputAnalysisDataId);
           //QMessageBox::information(NULL, QString::fromUtf8("Uppdatera databas"), responsStr);
           //return;

        }


        res = db.insertAnalysisData(analysisDateId,
                                    mainAnalysisId,
                                    inputAnalysisDataId,
                                    inputAnalysisDataIdIsValid,
                           hSAPData.companyDescription,
                           hSAPData.bigEnoughAnswer,
                           hSAPData.bigEnoughComment,
                           hSAPData.strongFinancialPositionAnswer,
                           hSAPData.strongFinancialPositionComment,
                           hSAPData.earningStabilityAnswer,
                           hSAPData.earningStabilityComment,
                           hSAPData.dividendStabilityAnswer,
                           hSAPData.dividendStabilityComment,
                           hSAPData.earningGrowthAnswer,
                           hSAPData.earningGrowthComment,
                           hSAPData.keyValuePe,
                           hSAPData.keyValuePs,
                           hSAPData.keyValueNavPriceRatio,
                           hSAPData.keyValueYield,
                           hSAPData.keyValuePriceJEKRatio,
                           hSAPData.keyValueErningsDividentRatio,
                           hSAPData.keyValueTotalDebtEquityRatio,
                           hSAPData.keyValueCurrentRatio,
                           hSAPData.trustworthyLeadershipAnswer,
                           hSAPData.trustworthyLeadershipComment,
                           hSAPData.goodOwnershipAnswer,
                           hSAPData.goodOwnershipComment,
                           hSAPData.otherInformation,

                           hSAPData.tenYearNoteYield,
                           hSAPData.currEquityPerShare,
                           hSAPData.estimateYearlyDividend,
                           hSAPData.calcIntrinsicValue,
                           hSAPData.intrinsicValueYearSpan,
                           hSAPData.historicalYearlyInterestOnEquity,

                           analysisDataId);
    }

    // Save excel data to file
    writeHtmlArrDataToTxtFile();

    // Display all analysis graphs
    displayAllAnalysisPlots();


}



/******************************************************************
 *
 * Function:        writeHtmlArrDataToTxtFile()
 *
 * Description:     This function writes table data displayed
 *                  in html report to text file. So it can
 *                  be displayed as graph in Excel.
 *
 *
 *****************************************************************/
void StockAnalysisTab::writeHtmlArrDataToTxtFile(void)
{
    CUtil cu;
    QString str;
    QString filename;
    double tmpRes;


    m_fildata.clear();

    // Create header in file and part of filename
    str = str + m_stockName;
    str = str + "_";
    str = str + m_stockSymbol;
    str = str + "_";
    str = str + m_analysisDate;

    m_fildata = str;
    m_fildata = "|\n";

    // Create file extention
    filename = str + ".txt";



    //-------------------------------------------------------------------
    // Stark finansiell ställning:
    //-------------------------------------------------------------------


    //=======================================================================
    // Omsättningstillgångarna / Kortfristiga skulder > 2
    //=======================================================================
    if((m_nofTotalCurrentAssetsArrData == m_nofTotalCurrentLiabilitiesData) &&
       (m_nofTotalCurrentAssetsArrData > 0))
    {
        m_fildata += QString::fromUtf8("Omsättningstillgångarna / Kortfristiga skulder > 2||'O/KS'|\n");

        for(int ii = 0; ii < m_nofTotalCurrentAssetsArrData; ii++)
        {
            if(m_totalCurrentAssetsArr[ii].date.toInt() == m_totalCurrentLiabilitiesArr[ii].date.toInt())
            {
                // Add Year
                m_fildata +=  "|";
                m_fildata += m_totalCurrentAssetsArr[ii].date;
                m_fildata +=  "|";

                if(m_totalCurrentLiabilitiesArr[ii].data.toDouble() != 0)
                {
                    //tmpRes = (m_totalCurrentAssetsArr[ii].data.toDouble() - m_totalCurrentLiabilitiesArr[ii].data.toDouble())/m_totalCurrentLiabilitiesArr[ii].data.toDouble();
                    tmpRes = (m_totalCurrentAssetsArr[ii].data.toDouble() / m_totalCurrentLiabilitiesArr[ii].data.toDouble());
                }
                else
                {
                    tmpRes = m_totalCurrentAssetsArr[ii].data.toDouble() / 0.001;
                }

                // Add data
                //tmpRes = 1.0 + tmpRes;
                str = str.sprintf("%.2f", tmpRes);
                m_fildata +=  str;
                m_fildata +=  "|\n";
           }
        }
        // Make space for graph in excel file
        for(int kk = 0; kk < 15; kk++)
        {
            m_fildata +=  "|\n";
        }
    }


    //=======================================================================
    // Omsättningstillgångarna	/ Totala skulder >= 1
    //=======================================================================
    if((m_nofTotalCurrentAssetsArrData == m_nofTotalLiabilitiesData) &&
       (m_nofTotalCurrentAssetsArrData > 0))
    {
        m_fildata += QString::fromUtf8("Omsättningstillgångarna / Totala skulder >= 1||'O/TS'|\n");

        for(int ii = 0; ii < m_nofTotalCurrentAssetsArrData; ii++)
        {
            if(m_totalCurrentAssetsArr[ii].date.toInt() == m_totalLiabilitiesArr[ii].date.toInt())
            {
                // Add Year
                m_fildata +=  "|";
                m_fildata += m_totalCurrentAssetsArr[ii].date;
                m_fildata +=  "|";

                if(m_totalLiabilitiesArr[ii].data.toDouble() != 0)
                {
                    // tmpRes = (m_totalCurrentAssetsArr[ii].data.toDouble() - m_totalLiabilitiesArr[ii].data.toDouble()) / m_totalCurrentLiabilitiesArr[ii].data.toDouble();
                    tmpRes = (m_totalCurrentAssetsArr[ii].data.toDouble() / m_totalLiabilitiesArr[ii].data.toDouble());
                }
                else
                {
                    tmpRes = m_totalCurrentAssetsArr[ii].data.toDouble() / 0.001;
                }

                // tmpRes = 1.0 + tmpRes;

                QString tmpDb;
                tmpDb = tmpDb.sprintf("%.2f", tmpRes);

                // Add data to file
                m_fildata += tmpDb;
                m_fildata +=  "|\n";

            }
        }
        // Make space for graph in excel file
        for(int kk = 0; kk < 15; kk++)
        {
            m_fildata +=  "|\n";
        }
    }




    //====================================================================
    // Totala skulder / Eget kapital < 2
    //====================================================================
    if(m_nofTotalLiabilitiesData > 0 && m_nofTotalLiabilitiesData ==  m_nofTotEquityData)
    {
        m_fildata += QString::fromUtf8("Totala skulder / Eget kapital< 2||'TS/EK'|\n");
        for(int ii = 0; ii < m_nofTotalLiabilitiesData; ii++)
        {
            if((m_totEquityArr[ii].date.toInt() == m_totalLiabilitiesArr[ii].date.toInt()))
            {
                // Add Year
                m_fildata +=  "|";
                m_fildata += m_totEquityArr[ii].date;
                m_fildata +=  "|";


                QString tmpStr;
                double realEquity = m_totEquityArr[ii].data.toDouble();
                double realTotalLiabilities = m_totalLiabilitiesArr[ii].data.toDouble();
                double ratio;
                if(realEquity == 0)
                {
                    realEquity = 0.001;
                    ratio = realTotalLiabilities / realEquity;
                }
                if(realEquity < 0)
                {
                    ratio = realTotalLiabilities / realEquity;
                    if(ratio > 0)
                    {
                        ratio = -ratio;
                    }
                }
                else
                {
                    ratio = realTotalLiabilities / realEquity;
                }

                tmpStr = tmpStr.sprintf("%.2f", ratio);

                // Add data
                m_fildata += tmpStr;
                m_fildata +=  "|\n";

            }
        }

        // Make space for graph in excel file
        for(int kk = 0; kk < 15; kk++)
        {
            m_fildata +=  "|\n";
        }
    }



#if 0
    //====================================================================
    // Soliditet >= 40%
    //====================================================================
    if(m_nofSolidityData > 0)
    {
        m_fildata += QString::fromUtf8("Soliditet >= 40%||S|\n");


        for(int ii = 0; ii < m_nofSolidityData; ii++)
        {
            // Add Year
            m_fildata +=  "|";
            m_fildata += m_solidityArr[ii].date;
            m_fildata +=  "|";

            // Add data
            QString tmpDb;
            tmpDb.sprintf("%.2f", (m_solidityArr[ii].data.toDouble() * 100.0));
            m_fildata += tmpDb;
            m_fildata +=  "|\n";
        }

        // Make space for graph in excel file
        for(int kk = 0; kk < 15; kk++)
        {
            m_fildata +=  "|\n";
        }

    }
#endif




    //====================================================================
    // Soliditet >= 40%
    //====================================================================
    if(m_nofSolidityData > 9)
    {
        m_fildata += QString::fromUtf8("Soliditet >= 40%||S|\n");

        for(int ii = 0; ii < m_nofSolidityData; ii++)
        {
            // Add Year
            m_fildata +=  "|";
            m_fildata += m_solidityArr[ii].date;
            m_fildata +=  "|";

            // Add data
            QString tmpDb;
            tmpDb.sprintf("%.2f", (m_solidityArr[ii].data.toDouble() * 100.0));
            m_fildata += tmpDb;
            m_fildata +=  "|\n";
        }

        // Make space for graph in excel file
        for(int kk = 0; kk < 15; kk++)
        {
            m_fildata +=  "|\n";
        }

    }


    //====================================================================
    // Räntetäckningsgraden >= 3
    //====================================================================
    if(m_nofCoverageRatioData > 0)
    {
        m_fildata += QString::fromUtf8("Räntetäckningsgraden >= 3||R|\n");

        for(int ii = 0; ii < m_nofCoverageRatioData; ii++)
        {
            // Add Year
            m_fildata +=  "|";
            m_fildata += m_coverageRatioArr[ii].date;
            m_fildata +=  "|";

            // Add data
            m_fildata += m_coverageRatioArr[ii].data;
            m_fildata +=  "|\n";
        }

        // Make space for graph in excel file
        for(int kk = 0; kk < 15; kk++)
        {
            m_fildata +=  "|\n";
        }

    }


#if 0
    //====================================================================
    // Totala skulder / Eget kapital< 2
    //====================================================================
    if(m_nofTotalLiabilitiesData > 0 && m_nofTotalLiabilitiesData ==  m_nofTotEquityData)
    {
        m_fildata += QString::fromUtf8("Totala skulder / Eget kapital< 2||'TS/EK'|\n");
        for(int ii = 0; ii < m_nofTotalLiabilitiesData; ii++)
        {
            if((m_totEquityArr[ii].date.toInt() == m_totalLiabilitiesArr[ii].date.toInt()))
            {
                // Add Year
                m_fildata +=  "|";
                m_fildata += m_totEquityArr[ii].date;
                m_fildata +=  "|";


                QString tmpStr;
                double realEquity = m_totEquityArr[ii].data.toDouble();
                double realTotalLiabilities = m_totalLiabilitiesArr[ii].data.toDouble();
                double ratio;
                if(realEquity == 0)
                {
                    realEquity = 0.001;
                    ratio = realTotalLiabilities / realEquity;
                }
                if(realEquity < 0)
                {
                    ratio = realTotalLiabilities / realEquity;
                    if(ratio > 0)
                    {
                        ratio = -ratio;
                    }
                }
                else
                {
                    ratio = realTotalLiabilities / realEquity;
                }

                tmpStr = tmpStr.sprintf("%.2f", ratio);

                // Add data
                m_fildata += tmpStr;
                m_fildata +=  "|\n";

            }
        }

        // Make space for graph in excel file
        for(int kk = 0; kk < 15; kk++)
        {
            m_fildata +=  "|\n";
        }

    }
#endif

    //====================================================================
    // Kärnprimärkapitalrelation > 15%
    //====================================================================
    if(m_nofCoreCapitalRatioData > 0)
    {
        m_fildata += QString::fromUtf8("Kärnprimärkapitalrelation > 15%||K|\n");

        for(int ii = 0; ii < m_nofCoreCapitalRatioData; ii++)
        {
            // Add Year
            m_fildata +=  "|";
            m_fildata += m_coreCapitalRatioArr[ii].date;
            m_fildata +=  "|";

            // Add data to file
            m_fildata += m_coreCapitalRatioArr[ii].data;
            m_fildata +=  "|\n";
        }

        // Make space for graph in excel file
        for(int kk = 0; kk < 15; kk++)
        {
            m_fildata +=  "|\n";
        }
    }



    //-------------------------------------------------------------------
    // Intjäningsstabilitet:
    //-------------------------------------------------------------------

    //====================================================================
    // Vinst/Aktie,
    //====================================================================
    if(m_nofEarningsArrData > 0)
    {
        m_fildata += QString::fromUtf8("Vinst/Aktie||V|\n");
        for(int ii = 0; ii < m_nofEarningsArrData; ii++)
        {

            // Add Year
            m_fildata +=  "|";
            m_fildata += m_earningsDataArr[ii].date;
            m_fildata +=  "|";

            // Add data
            m_fildata += m_earningsDataArr[ii].data;
            m_fildata +=  "|\n";
        }

        // Make space for graph in excel file
        for(int kk = 0; kk < 15; kk++)
        {
            m_fildata +=  "|\n";
        }
    }


    //====================================================================
    //  Omsättning, Vinst efter skatt
    //====================================================================
    if(m_nofTotEarningsArrData > 0 && (m_nofRevenueArrData == m_nofTotEarningsArrData))
    {
        m_fildata += QString::fromUtf8("Omsättning, Vinst efter skatt|År|O|V|\n");
        for(int ii = 0; ii < m_nofTotEarningsArrData; ii++)
        {
            if((m_revenueDataArr[ii].date.toInt() == m_totEarningsDataArr[ii].date.toInt()))
            {
                // Add Year
                m_fildata +=  "|";
                m_fildata += m_revenueDataArr[ii].date;
                m_fildata +=  "|";

                // Add data
                QString tmpStr;
                tmpStr = tmpStr.sprintf("%.2f", m_revenueDataArr[ii].data.toDouble());
                m_fildata += tmpStr;
                m_fildata +=  "|";

                tmpStr = tmpStr.sprintf("%.2f", m_totEarningsDataArr[ii].data.toDouble());
                m_fildata += tmpStr;
                m_fildata +=  "|\n";
            }
        }

        // Make space for graph in excel file
        for(int kk = 0; kk < 15; kk++)
        {
            m_fildata +=  "|\n";
        }

    }


    //-------------------------------------------------------------------
    // Utdelningsstabilitet:
    //-------------------------------------------------------------------


    //====================================================================
    // Utdelning
    //====================================================================
    if((m_nofDividendArrData > 0))
    {
        m_fildata += QString::fromUtf8("Utdelning||U|\n");

        for(int ii = 0; ii < m_nofDividendArrData; ii++)
        {
            // Add date
            m_fildata +=  "|";
            m_fildata += m_dividendDataArr[ii].date;
            m_fildata +=  "|";

            // Add data
            QString div;
            div.sprintf("%.2f", m_dividendDataArr[ii].data.toDouble());

            m_fildata += div;
            m_fildata +=  "|\n";
        }
        // Make space for graph in excel file
        for(int kk = 0; kk < 15; kk++)
        {
            m_fildata +=  "|\n";
        }
    }



    //====================================================================
    // Vinst/Utdelning
    //====================================================================
    if((m_nofEarningsArrData > 0) && (m_nofDividendArrData > 0))
    {
        m_fildata += QString::fromUtf8("Vinst/Utdelning||'V/U'|\n");

        for(int ii = 0; ii < m_nofDividendArrData; ii++)
        {
	        for(int jj = 0; jj < m_nofEarningsArrData; jj++)
	        {
	            if(m_dividendDataArr[ii].date.toInt() == m_earningsDataArr[jj].date.toInt())
	            {
	                // Add date
	                m_fildata +=  "|";
	                m_fildata += m_dividendDataArr[ii].date;
	                m_fildata +=  "|";

	                // Add data
	                QString res;
	                res.sprintf("%.2f", (m_earningsDataArr[jj].data.toDouble() / m_dividendDataArr[ii].data.toDouble()));

	                m_fildata += res;
	                m_fildata +=  "|\n";
	                break;
	            }
	        }
        }

        // Make space for graph in excel file
        for(int kk = 0; kk < 15; kk++)
        {
            m_fildata +=  "|\n";
        }
    }

    //==================================================================
    //  Operativt kassaflöde - CAPEX - Utdelning > 0
    //==================================================================
    if((m_nofTotDividensData > 0) &&
       (m_nofTotDividensData == m_nofCashFlowCapexData) &&
       (m_nofTotDividensData == m_nofCashFlowCapexData))
    {
        double res;
        QString resStr;

        m_fildata += QString::fromUtf8("Operativt kassaflöde - CAPEX - Utdelning > 0||OK-CA-UT|\n");


        for(int i = 0; i < m_nofTotDividensData; i++)
        {
            // Add Year
            m_fildata +=  "|";
            m_fildata += m_cashFlowCapexArr[i].date;;
            m_fildata +=  "|";

            // cashFlowCapexArr totDividensArr is stored as negative numbers
            res = m_operatingCashFlowArr[i].data.toDouble() +
                  m_cashFlowCapexArr[i].data.toDouble() +
                  m_totDividensArr[i].data.toDouble();

            resStr.sprintf("%.2f", res);

            // Add data to file
            m_fildata += resStr;
            m_fildata +=  "|\n";
        }

        // Make space for graph in excel file
        for(int kk = 0; kk < 15; kk++)
        {
            m_fildata +=  "|\n";
        }
    }



    //==================================================================
    //  Operativt kassaflöde| CAPEX| Utdelning|
    //==================================================================
    if((m_nofTotDividensData > 0) &&
       (m_nofTotDividensData == m_nofCashFlowCapexData) &&
       (m_nofTotDividensData == m_nofCashFlowCapexData))
    {

        m_fildata += QString::fromUtf8("Operativt kassaflöde, CAPEX, Utdelning||OK|CA|UT|\n");


        for(int i = 0; i < m_nofTotDividensData; i++)
        {
            // Add Year
            m_fildata +=  "| ";
            m_fildata += m_cashFlowCapexArr[i].date;;
            m_fildata +=  "|";

            // Add data
            str = str.sprintf("%.0f", m_operatingCashFlowArr[i].data.toDouble());
            m_fildata += str;
            m_fildata +=  "|";

            // Add data
            if(m_cashFlowCapexArr[i].data.toDouble() < 0)
            {
                str = str.sprintf("%.0f", -m_cashFlowCapexArr[i].data.toDouble());
            }
            else
            {
                str =  m_cashFlowCapexArr[i].data;
            }

            m_fildata += str;
            m_fildata +=  "|";

            // Add data
            if(m_totDividensArr[i].data.toDouble() < 0)
            {
                str = str.sprintf("%.0f", -m_totDividensArr[i].data.toDouble());
            }
            else
            {
                str = m_totDividensArr[i].data;
            }

            m_fildata += str;
            m_fildata +=  "|\n";
        }

        // Make space for graph in excel file
        for(int kk = 0; kk < 15; kk++)
        {
            m_fildata +=  "|\n";
        }
    }





    //-------------------------------------------------------------------
    // Intjäningstillväxt:
    //-------------------------------------------------------------------

    //====================================================================
    // Vinsttillväxt
    //====================================================================
    QString earningsGrowthStr;
    double earningsGrowth = 0;
    bool negativeSign = false;
    double currEarnings;
    double lastEarnings;

    if(m_nofEarningsArrData > 0)
    {
        m_fildata += QString::fromUtf8("Vinsttillväxt (%)||Vt|\n");

        for(int ii = 0; ii < m_nofEarningsArrData; ii++)
        {

            // Add Year
            m_fildata +=  "|";
            m_fildata += m_earningsDataArr[ii].date;
            m_fildata +=  "|";

            negativeSign = false;
            if(ii > 0)
            {
                currEarnings = m_earningsDataArr[ii].data.toDouble();
                lastEarnings = m_earningsDataArr[ii-1].data.toDouble();

                if(currEarnings < 0)
                {
                    negativeSign = true;
                }


                // Handle divide by zero
                if(lastEarnings == 0)
                {
                    earningsGrowth = currEarnings / 0.001;
                }
                else
                {
                    earningsGrowth = (currEarnings - lastEarnings) / lastEarnings;
                }


                // Change sign if negative growth
                if( ((negativeSign == true) && (earningsGrowth > 0)) || ((currEarnings > 0) && (lastEarnings < 0)) )
                {
                    earningsGrowth = -earningsGrowth;
                }

                earningsGrowth = earningsGrowth * 100;
                earningsGrowthStr.sprintf("%.2f",  earningsGrowth);
            }
            else
            {
                earningsGrowthStr.clear();
            }

            // Add data
            m_fildata += earningsGrowthStr;
            m_fildata +=  "|\n";
        }

        // Make space for graph in excel file
        for(int kk = 0; kk < 15; kk++)
        {
            m_fildata +=  "|\n";
        }
    }



    //====================================================================
    // Vinstmarginal (Vinst efter skatt / Omsättning)
    //====================================================================
    if(m_nofTotEarningsArrData > 0 && (m_nofRevenueArrData == m_nofTotEarningsArrData))
    {
        m_fildata += QString::fromUtf8("Vinstmarginal (%) (Vinst efter skatt / Omsättning)||'V/O'|\n");
        for(int ii = 0; ii < m_nofTotEarningsArrData; ii++)
        {
            if((m_revenueDataArr[ii].date.toInt() == m_totEarningsDataArr[ii].date.toInt()))
            {
                // Add Year
                m_fildata +=  "|";
                m_fildata += m_revenueDataArr[ii].date;
                m_fildata +=  "|";


                QString tmpStr;
                double realRevenue = m_revenueDataArr[ii].data.toDouble();
                double realEarnings = m_totEarningsDataArr[ii].data.toDouble();
                double ratio;

                if(realRevenue == 0)
                {
                    realRevenue = 0.001;
                    ratio = realEarnings / realRevenue * 100.0;
                }

                if(realRevenue < 0 || (realEarnings < 0))
                {
                    ratio = realEarnings / realRevenue * 100.0;
                    if(ratio > 0)
                    {
                        ratio = -ratio;
                    }
                }
                else
                {
                    ratio = realEarnings/ realRevenue * 100.0;
                }

                tmpStr = tmpStr.sprintf("%.2f", ratio);

                // Add data
                m_fildata += tmpStr;
                m_fildata +=  "|\n";
            }
        }

        // Make space for graph in excel file
        for(int kk = 0; kk < 15; kk++)
        {
            m_fildata +=  "|\n";
        }
    }




    //====================================================================
    // Avkastning på det egna kapitalet (Vinst efter skatt / Eget kapital)
    //====================================================================
    if(m_nofTotEarningsArrData > 0 && (m_nofTotEarningsArrData == m_nofTotEquityData))
    {
        m_fildata += QString::fromUtf8("Avkastning på det egna kapitalet (%) [Vinst/Eget kapital]||'V/EK'|\n");
        for(int ii = 0; ii < m_nofTotEarningsArrData; ii++)
        {
            if((m_totEquityArr[ii].date.toInt() == m_totEarningsDataArr[ii].date.toInt()))
            {
                // Add Year
                m_fildata +=  "|";
                m_fildata += m_totEquityArr[ii].date;
                m_fildata +=  "|";


                QString tmpStr;
                double realEquity = m_totEquityArr[ii].data.toDouble();
                double realEarnings = m_totEarningsDataArr[ii].data.toDouble();
                double ratio;

                if(realEquity == 0)
                {
                    realEquity = 0.001;
                    ratio = realEarnings / realEquity * 100.0;
                }

                if(realEquity < 0 || (realEarnings < 0))
                {
                    ratio = realEarnings / realEquity * 100.0;
                    if(ratio > 0)
                    {
                        ratio = -ratio;
                    }
                }
                else
                {
                    ratio = realEarnings/ realEquity * 100.0;
                }


                tmpStr = tmpStr.sprintf("%.2f", ratio);

                // Add data
                m_fildata += tmpStr;
                m_fildata +=  "|\n";
            }
        }

        // Make space for graph in excel file
        for(int kk = 0; kk < 15; kk++)
        {
            m_fildata +=  "|\n";
        }
    }


    //====================================================================
    // Totala skulder Eget kapital, Vinst, Utdelning
    //====================================================================
    if((m_nofEarningsArrData > 0) && (m_nofTotDividensData > 0))
    {
        bool earningsIsFound;
        bool totalLiabilitiesIsFound;
        bool equityIsFound;

        int jj;
        int kk;
        int nn;

        qDebug() << QString::fromUtf8("Totala skulder Eget kapital, Vinst, Utdelning|");;
        m_fildata += QString::fromUtf8("Totala Skulder, Eget kapital, Vinst, Utdelning|År|TS|EK|V|UT|\n");

        // Loop through all dividend
        for(int ii = 0; ii < m_nofTotDividensData; ii++)
        {
            earningsIsFound = false;
            totalLiabilitiesIsFound = false;
            equityIsFound = false;


            // Check if we have earnings from same year
            for(jj = 0; jj < m_nofTotEarningsArrData; jj++)
            {
                if(m_totDividensArr[ii].date.toInt() == m_totEarningsDataArr[jj].date.toInt())
                {
                    qDebug() << "divYear" << m_totDividensArr[ii].date;
                    qDebug() << "earYear" << m_totEarningsDataArr[jj].date;

                    qDebug() << ii << "div" << m_totDividensArr[ii].data;
                    qDebug() << jj << "ear" << m_totEarningsDataArr[jj].data;

                    earningsIsFound = true;
                    break;
                }
            }


            // Check if we have Equity from same year
            for(kk = 0; kk < m_nofTotEquityData; kk++)
            {
                if(m_totDividensArr[ii].date.toInt() == m_totEquityArr[kk].date.toInt())
                {
                    qDebug() << "divYear" << m_totDividensArr[ii].date;
                    qDebug() << "euqYear" << m_totEquityArr[kk].date;


                    qDebug() << kk << "equ" << m_totEquityArr[kk].data;
                    equityIsFound = true;
                    break;
                }
            }

            // Check if we have Total Liabilities from same year
            for(nn = 0; nn < m_nofTotalLiabilitiesData; nn++)
            {
                if(m_totDividensArr[ii].date.toInt() == m_totalLiabilitiesArr[nn].date.toInt())
                {
                    qDebug() << "divYear" << m_totDividensArr[ii].date;
                    qDebug() << "totalLiaYear" << m_totalLiabilitiesArr[kk].date;


                    qDebug() << nn << "totLia" << m_totalLiabilitiesArr[nn].data;
                    totalLiabilitiesIsFound = true;
                    break;
                }
            }


            // If all data are found for this year then wite to file.
            if((earningsIsFound == true) && (equityIsFound == true) && (totalLiabilitiesIsFound == true))
            {
                QString res;

                qDebug() << ii << "divYear" << m_totDividensArr[ii].date;
                qDebug() << nn << "totalLia" << m_totalLiabilitiesArr[nn].data;
                qDebug() << kk << "equYear" << m_totEquityArr[kk].data;
                qDebug() << jj << "totEar" << m_totEarningsDataArr[jj].data;
                qDebug() << ii << "totDiv" << m_totDividensArr[ii].data;

                // Add Year
                m_fildata +=  "|";
                m_fildata += m_totDividensArr[ii].date;
                m_fildata +=  "|";

                // Add total liabilities
                res = res.sprintf("%.2f", m_totalLiabilitiesArr[nn].data.toDouble());
                m_fildata += res;
                m_fildata +=  "|";

                // Add equity
                res = res.sprintf("%.2f", m_totEquityArr[kk].data.toDouble());
                m_fildata += res;
                m_fildata +=  "|";


                // Add earnings
                res = res.sprintf("%.2f", m_totEarningsDataArr[jj].data.toDouble());
                m_fildata += res;
                m_fildata +=  "|";

                // Add divident
                res = res.sprintf("%.2f", -m_totDividensArr[ii].data.toDouble());
                m_fildata += res;
                m_fildata +=  "|\n";
            }
        }
    }





    // Save data
    if(m_fildata.length() > 0)
    {
        QString path = qApp->applicationDirPath();
        path += "/database/AnalysDoc/htmlReports/Excel";

        cu.saveTextFile(m_fildata, filename, path);
        // m_fildata.clear();
    }
}


/******************************************************************
 *
 * Function:    resetStockAnalysisData()
 *
 * Description:
 *
 *
 *
 *
 *****************************************************************/
void StockAnalysisTab::resetStockAnalysisData(HtmlStockAnalysPageDataST &hSAPData)
{
    hSAPData.companyDescription.clear();
    //m_BigCompDescription.clear();
    hSAPData.bigEnoughAnswer.clear();
    hSAPData.bigEnoughComment.clear();
    hSAPData.strongFinancialPositionAnswer.clear();
    hSAPData.strongFinancialPositionComment.clear();
    hSAPData.earningStabilityAnswer.clear();
    hSAPData.earningStabilityComment.clear();
    hSAPData.dividendStabilityAnswer.clear();
    hSAPData.dividendStabilityComment.clear();
    hSAPData.earningGrowthAnswer.clear();
    hSAPData.earningGrowthComment.clear();
    hSAPData.keyValuePe.clear();
    hSAPData.keyValuePs.clear();
    hSAPData.keyValueNavPriceRatio.clear();
    hSAPData.keyValueYield.clear();
    hSAPData.keyValuePriceJEKRatio.clear();
    hSAPData.keyValueErningsDividentRatio.clear();
    hSAPData.keyValueTotalDebtEquityRatio.clear();
    hSAPData.keyValueCurrentRatio.clear();
    hSAPData.trustworthyLeadershipAnswer.clear();
    hSAPData.trustworthyLeadershipComment.clear();
    hSAPData.goodOwnershipAnswer.clear();
    hSAPData.goodOwnershipComment.clear();
    hSAPData.otherInformation.clear();
    hSAPData.riskStdDev.clear();
    hSAPData.meanReturns.clear();
    hSAPData.meanReturns.clear();

    hSAPData.nofCoreCapitalRatioData = 0;
    hSAPData.nofCoreTier1RatioData = 0;
    hSAPData.nofCoverageRatioData = 0;
    hSAPData.nofDividendArrData = 0;
    hSAPData.nofEarningsArrData = 0;
    hSAPData.nofEquityData = 0;
    hSAPData.nofSolidityData = 0;
    hSAPData.nofTotalCurrentAssetsArrData = 0;
    hSAPData.nofTotalCurrentLiabilitiesData = 0;
    hSAPData.nofTotalLiabilitiesData = 0;

    // Intrinsic value
    hSAPData.tenYearNoteYield.clear();
    hSAPData.currEquityPerShare.clear();
    hSAPData.estimateYearlyDividend.clear();
    hSAPData.calcIntrinsicValue.clear();
    hSAPData.intrinsicValueYearSpan.clear();
    hSAPData.historicalYearlyInterestOnEquity.clear();

}


/******************************************************************
 *
 * Function:    on_treeWidgetAnalysisDate_doubleClicked()
 *
 * Description: This function use date to get stock analysis data
 *              from the database.
 *
 *
 *
 *
 *****************************************************************/
void StockAnalysisTab::on_treeWidgetAnalysisDate_doubleClicked(const QModelIndex &index)
{

    createStockAnalysisHtmlPage csaHtmlPg;
    QString analysisDate;
    CDbHndl db;
    HtmlStockAnalysPageDataST hSAPData;



    m_gfc.getSelStockAnalysisDateItem(ui->treeWidgetAnalysisDate,
                                      analysisDate,
                                      index);

    if(analysisDate.size() < 1)
    {
        return;
    }

    // Main analys page
    m_analysisDate = analysisDate;
    hSAPData.analysisDate = analysisDate;
    ui->analysisDateLineEdit->setText(m_analysisDate);

    // Page analysdata 1
    ui->ananlysisDateLabel_32->setText(m_analysisDate);

    // Page analysdata 2
    ui->ananlysisDateLabel_33->setText(m_analysisDate);

    resetStockAnalysisData(hSAPData);
    resetGuiCtrl();
    clearGUIIntrinsicValue();


    hSAPData.stockName = m_stockName;
    hSAPData.stockSymbol = m_stockSymbol;
    hSAPData.analysisDate = m_analysisDate;


    db.getStockAnalysisData(hSAPData.stockName,
                         hSAPData.stockSymbol,
                         hSAPData.analysisDate,
                         hSAPData.companyDescription,
                         hSAPData.bigEnoughAnswer,
                         hSAPData.bigEnoughComment,
                         hSAPData.strongFinancialPositionAnswer,
                         hSAPData.strongFinancialPositionComment,
                         hSAPData.earningStabilityAnswer,
                         hSAPData.earningStabilityComment,
                         hSAPData.dividendStabilityAnswer,
                         hSAPData.dividendStabilityComment,
                         hSAPData.earningGrowthAnswer,
                         hSAPData.earningGrowthComment,
                         hSAPData.keyValuePe,
                         hSAPData.keyValuePs,
                         hSAPData.keyValueNavPriceRatio,
                         hSAPData.keyValueYield,
                         hSAPData.keyValuePriceJEKRatio,
                         hSAPData.keyValueErningsDividentRatio,
                         hSAPData.keyValueTotalDebtEquityRatio,
                         hSAPData.keyValueCurrentRatio,
                         hSAPData.trustworthyLeadershipAnswer,
                         hSAPData.trustworthyLeadershipComment,
                         hSAPData.goodOwnershipAnswer,
                         hSAPData.goodOwnershipComment,
                         hSAPData.otherInformation,

                         hSAPData.tenYearNoteYield,
                         hSAPData.currEquityPerShare,
                         hSAPData.estimateYearlyDividend,
                         hSAPData.calcIntrinsicValue,
                         hSAPData.intrinsicValueYearSpan,
                         hSAPData.historicalYearlyInterestOnEquity);


    //m_companyDescription = hSAPData.companyDescription;


    hSAPData.dividendDataArr = m_dividendDataArr;
    hSAPData.nofDividendArrData = m_nofDividendArrData;

    hSAPData.earningsDataArr = &m_earningsDataArr[0];
    hSAPData.nofEarningsArrData = m_nofEarningsArrData;

    hSAPData.totalCurrentAssetsArr = m_totalCurrentAssetsArr;
    hSAPData.nofTotalCurrentAssetsArrData = m_nofTotalCurrentAssetsArrData;
    hSAPData.totalCurrentLiabilitiesArr = m_totalCurrentLiabilitiesArr;
    hSAPData.nofTotalCurrentLiabilitiesData = m_nofTotalCurrentLiabilitiesData;
    hSAPData.totalLiabilitiesArr = m_totalLiabilitiesArr;
    hSAPData.nofTotalLiabilitiesData = m_nofTotalLiabilitiesData;
    hSAPData.solidityArr = m_solidityArr;
    hSAPData.nofSolidityData = m_nofSolidityData;
    hSAPData.coverageRatioArr = m_coverageRatioArr;
    hSAPData.nofCoverageRatioData = m_nofCoverageRatioData;

    hSAPData.equityArr = m_totEquityArr;
    hSAPData.nofEquityData = m_nofTotEquityData;

    hSAPData.coreTier1RatioArr = m_coreTier1RatioArr;
    hSAPData.nofCoreTier1RatioData = m_nofCoreTier1RatioData;

    hSAPData.coreCapitalRatioArr = m_coreCapitalRatioArr;
    hSAPData.nofCoreCapitalRatioData = m_nofCoreCapitalRatioData;

    hSAPData.cashFlowCapexArr = m_cashFlowCapexArr;
    hSAPData.nofCashFlowCapexData = m_nofCashFlowCapexData;
    hSAPData.operatingCashFlowArr = m_operatingCashFlowArr;
    hSAPData.nofOperatingCashFlowData = m_nofOperatingCashFlowData;

    hSAPData.totDividensArr = m_totDividensArr;
    hSAPData.nofTotDividensData = m_nofTotDividensData;

    hSAPData.riskStdDev = m_riskStdDev;
    hSAPData.meanReturns = m_meanReturns;
    hSAPData.returnOnEquity = m_returnOnEquity;


    csaHtmlPg.createHtmlPage(hSAPData);
     m_saDisply.subAnalysisShowDataInGraphs(hSAPData, m_qwtPlot);

    ui->webView->setHtml(hSAPData.htmlStr);


    // Write html report to disk
    ///home/ajn/Documents/OldPC/swProj/MyQtProj/JackStockProj/JackStock/database/AnalysDoc/htmlReports

    qDebug() << "App path : " << qApp->applicationDirPath();
    // path =  QCoreApplication::applicationDirPath();
    QString path = qApp->applicationDirPath();
    path += "/database/AnalysDoc/htmlReports";
    QString filname;

    filname = m_stockName;
    filname.trimmed();
    filname += "_";
    filname += m_analysisDate;
    filname.trimmed();
    filname += ".html";

    CUtil cu;
    cu.saveTextFile(hSAPData.htmlStr, filname, path);

    ui->textEditCompDescription->insertPlainText(hSAPData.companyDescription);

    // Big enought
    ui->lineEditBigEnoughAnswer->setText(hSAPData.bigEnoughAnswer);
    ui->textEditBigEnoughText->insertPlainText(hSAPData.bigEnoughComment);

    // Financial strong enought
    ui->lineEditFinancialStrongAnswer->setText(hSAPData.strongFinancialPositionAnswer);
    ui->textEditFinancialStrongText->insertPlainText(hSAPData.strongFinancialPositionComment);

    // Earning Stability
    ui->lineEditErningStabilityAnswer->setText(hSAPData.earningStabilityAnswer);
    ui->textEditErningStabilityText->insertPlainText(hSAPData.earningStabilityComment);

    // Dividend Stability
    ui->lineEditDividentStabilityAnswer->setText(hSAPData.dividendStabilityAnswer);
    ui->textEditDividentStabilityText->insertPlainText(hSAPData.dividendStabilityComment);

    // Erning Growth
    ui->lineEditErningGrowthAnswer->setText(hSAPData.earningGrowthAnswer);
    ui->textEditErningGrowthText->insertPlainText(hSAPData.earningGrowthComment);

    // Pe-value
    ui->lineEditPE->setText(hSAPData.keyValuePe);
    ui->lineEditPs->setText(hSAPData.keyValuePs);
    ui->lineEditNavDivLastStockPrice->setText(hSAPData.keyValueNavPriceRatio);
    ui->lineEditYield->setText(hSAPData.keyValueYield);

    // m_keyValuePriceJEKRatio, saknas
    ui->lineEditTotDebtEquityRatio->setText(hSAPData.keyValueTotalDebtEquityRatio);
    ui->lineEditEarningsDivByDividend->setText(hSAPData.keyValueErningsDividentRatio);
    ui->lineEditCurrentRatio->setText(hSAPData.keyValueCurrentRatio);

    // Trustworthy Leadership
    ui->lineEditTrustworthyManagementAnswer->setText(hSAPData.trustworthyLeadershipAnswer);
    ui->textEditTrustworthyManagementText->setText(hSAPData.trustworthyLeadershipComment);

    // Good Ownership
    ui->lineEditBeneficialOwnershipAnswer->setText(hSAPData.goodOwnershipAnswer);
    ui->textEditBeneficialOwnershipText->insertPlainText(hSAPData.goodOwnershipComment);

    ui->textEditOtherInfo->insertPlainText(hSAPData.otherInformation);

    // Intrinsic value
    ui->lineEditTenYearNoteYield->setText(hSAPData.tenYearNoteYield);
    ui->lineEditCurrEquityPerShare->setText(hSAPData.currEquityPerShare);
    ui->lineEditEstimateYearlyDividend->setText(hSAPData.estimateYearlyDividend);
    ui->lineEditIntrinsicValueResult->setText(hSAPData.calcIntrinsicValue);
    ui->lineEditHistoricalInterestOnEquity->setText(hSAPData.historicalYearlyInterestOnEquity);



    if(m_nofEquityPerShareData > 1)
    {
        plotEquityPerShareData(m_equityPerShareArr, m_nofEquityPerShareData);
    }

    calcTotSubdataForIntrinsicValue();
    on_pushButtonCalcYearlyIntrestRateOnEquity_clicked();

    // Display all analysis graphs
    displayAllAnalysisPlots();

}



/******************************************************************
 *
 * Function:    resetGuiCtrl()
 *
 * Description:
 *
 *
 *
 *
 *****************************************************************/
void StockAnalysisTab::resetGuiCtrl(void)
{
    ui->textEditCompDescription->clear();

    // Big enought
    ui->lineEditBigEnoughAnswer->clear();
    ui->textEditBigEnoughText->clear();

    // Financial strong enought
    ui->lineEditFinancialStrongAnswer->clear();
    ui->textEditFinancialStrongText->clear();

    // Earning Stability
    ui->lineEditErningStabilityAnswer->clear();
    ui->textEditErningStabilityText->clear();

    // Dividend Stability
    ui->lineEditDividentStabilityAnswer->clear();
    ui->textEditDividentStabilityText->clear();

    // Erning Growth
    ui->lineEditErningGrowthAnswer->clear();
    ui->textEditErningGrowthText->clear();

    // Pe-value
    ui->lineEditPE->clear();
    ui->lineEditPs->clear();
    ui->lineEditNavDivLastStockPrice->clear();
    ui->lineEditYield->clear();

    // m_keyValuePriceJEKRatio, saknas
    ui->lineEditTotDebtEquityRatio->clear();
    ui->lineEditEarningsDivByDividend->clear();
    ui->lineEditCurrentRatio->clear();

    // Trustworthy Leadership
    ui->lineEditTrustworthyManagementAnswer->clear();
    ui->textEditTrustworthyManagementText->clear();

    // Good Ownership
    ui->lineEditBeneficialOwnershipAnswer->clear();
    ui->textEditBeneficialOwnershipText->clear();

    ui->textEditOtherInfo->clear();

}


/******************************************************************
 *
 * Function:    on_pushButtonRemoveRecord_clicked()
 *
 * Description:
 *
 *
 *
 *
 *****************************************************************/
void StockAnalysisTab::on_pushButtonRemoveRecord_clicked()
{
    QString str;
    CDbHndl db;


    str = QString::fromUtf8("Vill du ta bort data?\n");
    str = str + m_stockName;
    str = str + ", ";
    str = str + m_stockSymbol;
    str = str + ", ";
    str = str + m_analysisDate;


    if( false == QMessageBox::information(NULL, QString::fromUtf8("Uppdatera databas"), str))
    {
        return;
    }


    // Remove all data from data table in db
    if( false == db.deleteStockAnalysisDataRecord(m_stockName, m_stockSymbol, m_analysisDate))
    {
        str = (QString::fromUtf8("Fel: Data gick ej att ta bort\n"));
        QMessageBox::information(NULL, QString::fromUtf8("Uppdatera databas"), str);
        return;
    }

    // Delete data from date table in db
    if( false == db.deleteStockAnalysisDateRecord(m_stockName, m_stockSymbol, m_analysisDate))
    {
        str = (QString::fromUtf8("Fel: Data gick ej att ta bort\n"));
        QMessageBox::information(NULL, QString::fromUtf8("Uppdatera databas"), str);
        return;
    }


    // Find date in treeWidget and remove it
    QTreeWidgetItem *item;
    int index = -1;
    int numberOfTopLevelItems = ui->treeWidgetAnalysisDate->topLevelItemCount();
    for (int topLevelindex = 0 ; topLevelindex < numberOfTopLevelItems ; topLevelindex++)
    {
        item = ui->treeWidgetAnalysisDate->topLevelItem(topLevelindex);
        if(item != NULL)
        {
            if(m_analysisDate.compare(item->text(0)) == 0)
            {
                index = topLevelindex;
                break;
            }
        }
    }




    if(index > -1)
    {
        item  = ui->treeWidgetAnalysisDate->takeTopLevelItem(index);
        delete item; // do not forget to delete the item if it is not owned by any other widget.
    }

}




/******************************************************************
 *
 * Function:    on_pushSaveDividend_clicked()
 *
 * Description: This function saves dividend data to db
 *
 *
 *
 *****************************************************************/
void StockAnalysisTab::on_pushSaveDividend_clicked()
{
    CDbHndl db;
    bool res;
    int mainAnalysisId;
    QString str;

    bool isValid;
    int dividendDateId;
    int inputDividendDataId;
    int dividendDataId;
    bool dividendDataIdIsValid = false;

    int nofData;
    QString dividendDate;
    QString dividendData;

    if((m_stockName.length() < 1) || m_stockSymbol.length() < 1)
    {
        QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Välj aktie"));
        return;
    }


    str = (QString::fromUtf8("Vill du lägga till sub data?\n"));
    str = str + m_stockName;
    str = str + ", ";
    str = str + m_stockSymbol;
    str = str + ", ";
    str = str + m_analysisDate;





    if(QMessageBox::No == QMessageBox::question(this, QString::fromUtf8("Uppdatera databas"), str, QMessageBox::Yes|QMessageBox::No))
    {
        return;
    }

    // Check if this stocksymbol and stockname is already added, if not add it
    res = db.mainAnalysisDataExists(m_stockName,
                                m_stockSymbol,
                                mainAnalysisId);
    if(false == res)
    {
        res = db.insertMainAnalysisData(m_stockName,
                                     m_stockSymbol,
                                     mainAnalysisId);
    }



    nofData = ui->tableWidgetDividend->rowCount();

    for(int row = 0; row < nofData; row++)
    {
       if(NULL != ui->tableWidgetDividend->item(row, 0))
       {
            dividendDate = ui->tableWidgetDividend->item(row, 0)->text();
            dividendData = ui->tableWidgetDividend->item(row, 1)->text();

            dividendDate.toInt(&isValid);
            if (false == isValid)
            {
                QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Year is not a number"));
                continue;
            }

            CUtil cu;
            cu.number2double(dividendData, dividendData);
            dividendData.toDouble(&isValid);
            if (false == isValid)
            {
                QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Dividend is not a number"));
                continue;
            }
       }
       else
       {
           break;
       }


        res = db.subAnalysisDividendDateExists(dividendDate,
                                                   mainAnalysisId,
                                                   dividendDateId);

        // Exist anaysis date?
        if(false == res)
        {
            res = db.insertSubAnalysisDividendDate(dividendDate,
                                                   mainAnalysisId,
                                                   dividendDateId);
        }


        if(true == res)
        {
           dividendDataIdIsValid = false;

           if( true == db.getSubAnalysisDividendId(mainAnalysisId, dividendDateId, inputDividendDataId))
           {
               dividendDataIdIsValid = true;
           }

            res = db.insertSubAnalysisDividend(dividendDateId,
                                               mainAnalysisId,
                                               inputDividendDataId,
                                               dividendDataIdIsValid,
                                               dividendData,
                                               dividendDataId);
        }
    }

}



/******************************************************************
 *
 * Function:    on_pushSaveEarnings_clicked()
 *
 * Description: This function saves earnings data to db
 *
 *
 *
 *****************************************************************/
void StockAnalysisTab::on_pushSaveEarnings_clicked()
{
    CDbHndl db;
    bool res;
    int mainAnalysisId;
    QString str;

    bool isValid;
    int earningsDateId;
    int inputEarningsDataId;
    int earningsDataId;
    bool earningsDataIdIsValid = false;

    int nofData;
    QString earningsDate;
    QString earningsData;

    if((m_stockName.length() < 1) || m_stockSymbol.length() < 1)
    {
        QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Välj aktie"));
        return;
    }

    str = (QString::fromUtf8("Vill du lägga till sub data?\n"));
    str = str + m_stockName;
    str = str + ", ";
    str = str + m_stockSymbol;
    str = str + ", ";
    str = str + m_analysisDate;





    if(QMessageBox::No == QMessageBox::question(this, QString::fromUtf8("Uppdatera databas"), str, QMessageBox::Yes|QMessageBox::No))
    {
        return;
    }

    // Check if this stocksymbol and stockname is already added, if not add it
    res = db.mainAnalysisDataExists(m_stockName,
                                m_stockSymbol,
                                mainAnalysisId);
    if(false == res)
    {
        res = db.insertMainAnalysisData(m_stockName,
                                     m_stockSymbol,
                                     mainAnalysisId);
    }



    nofData = ui->tableWidgetEarnings->rowCount();

    for(int row = 0; row < nofData; row++)
    {
       if(NULL != ui->tableWidgetEarnings->item(row, 0))
       {
            earningsDate = ui->tableWidgetEarnings->item(row, 0)->text();
            earningsData = ui->tableWidgetEarnings->item(row, 1)->text();

            earningsDate.toInt(&isValid);
            if (false == isValid)
            {
                QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Year is not a number"));
                continue;
            }

            CUtil cu;
            cu.number2double(earningsData, earningsData);
            earningsData.toDouble(&isValid);
            if (false == isValid)
            {
                QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Earnings is not a number"));
                continue;
            }
       }
       else
       {
           break;
       }


        res = db.subAnalysisEarningsPerShareDateExists(earningsDate,
                                               mainAnalysisId,
                                               earningsDateId);

        // Exist anaysis date?
        if(false == res)
        {
            res = db.insertSubAnalysisEarningsPerShareDate(earningsDate,
                                                   mainAnalysisId,
                                                   earningsDateId);
        }


        if(true == res)
        {
           earningsDataIdIsValid = false;

           if( true == db.getSubAnalysisEarningsPerShareId(mainAnalysisId, earningsDateId, inputEarningsDataId))
           {
               earningsDataIdIsValid = true;
           }

            res = db.insertSubAnalysisEarningsPerShare(earningsDateId,
                                               mainAnalysisId,
                                               inputEarningsDataId,
                                               earningsDataIdIsValid,
                                               earningsData,
                                               earningsDataId);
        }
    }

}


/******************************************************************
 *
 * Function:    on_pushSaveEarnings_clicked()
 *
 * Description: This function saves earnings data to db
 *
 *
 *
 *****************************************************************/
void StockAnalysisTab::on_pushButtonSaveTotalCurrentAsset_clicked()
{
    CDbHndl db;
    bool res;
    int mainAnalysisId;
    QString str;

    bool isValid;
    int totalCurrentAssetsDateId;
    int inputTotalCurrentAssetsDataId;
    int totalCurrentAssetsDataId;
    bool totalCurrentAssetsDataIdIsValid = false;

    int nofData;
    QString totalCurrentAssetsDate;
    QString totalCurrentAssetsData;

    if((m_stockName.length() < 1) || m_stockSymbol.length() < 1)
    {
        QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Välj aktie"));
        return;
    }


    str = (QString::fromUtf8("Vill du lägga till sub data?\n"));
    str = str + m_stockName;
    str = str + ", ";
    str = str + m_stockSymbol;
    str = str + ", ";
    str = str + m_analysisDate;





    if(QMessageBox::No == QMessageBox::question(this, QString::fromUtf8("Uppdatera databas"), str, QMessageBox::Yes|QMessageBox::No))
    {
        return;
    }

    // Check if this stocksymbol and stockname is already added, if not add it
    res = db.mainAnalysisDataExists(m_stockName,
                                m_stockSymbol,
                                mainAnalysisId);
    if(false == res)
    {
        res = db.insertMainAnalysisData(m_stockName,
                                     m_stockSymbol,
                                     mainAnalysisId);
    }



    nofData = ui->tableWidgetTotalCurrentAsset->rowCount();

    for(int row = 0; row < nofData; row++)
    {
       if(NULL != ui->tableWidgetTotalCurrentAsset->item(row, 0))
       {
            totalCurrentAssetsDate = ui->tableWidgetTotalCurrentAsset->item(row, 0)->text();
            totalCurrentAssetsData = ui->tableWidgetTotalCurrentAsset->item(row, 1)->text();

            totalCurrentAssetsDate.toInt(&isValid);
            if (false == isValid)
            {
               QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Year is not a number"));
               continue;
            }

            CUtil cu;
            cu.number2double(totalCurrentAssetsData, totalCurrentAssetsData);
            totalCurrentAssetsData.toDouble(&isValid);
            if (false == isValid)
            {
               QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Earnings is not a number"));
               continue;
            }
       }
       else
       {
           break;
       }


        res = db.subAnalysisTotalCurrentAssetsDateExists(totalCurrentAssetsDate,
                                                        mainAnalysisId,
                                                        totalCurrentAssetsDateId);

        // Exist anaysis date?
        if(false == res)
        {
            res = db.insertSubAnalysisTotalCurrentAssetsDate(totalCurrentAssetsDate,
                                                            mainAnalysisId,
                                                            totalCurrentAssetsDateId);
        }


        if(true == res)
        {
           totalCurrentAssetsDataIdIsValid = false;

           if( true == db.getSubAnalysisTotalCurrentAssetsDataId(mainAnalysisId, totalCurrentAssetsDateId, inputTotalCurrentAssetsDataId))
           {
               totalCurrentAssetsDataIdIsValid = true;
           }

            res = db.insertSubAnalysisTotalCurrentAssets(totalCurrentAssetsDateId,
                                                        mainAnalysisId,
                                                        inputTotalCurrentAssetsDataId,
                                                        totalCurrentAssetsDataIdIsValid,
                                                        totalCurrentAssetsData,
                                                        totalCurrentAssetsDataId);
        }
    }
}


/******************************************************************
 *
 * Function:    on_pushButtonSaveTotalCurrentLiabilities_clicked()
 *
 * Description: This function saves Total Current Liabilities data to db
 *
 *
 *
 *****************************************************************/
void StockAnalysisTab::on_pushButtonSaveTotalCurrentLiabilities_clicked()
{
    CDbHndl db;
    bool res;
    int mainAnalysisId;
    QString str;

    bool isValid;
    int dateId;
    int inputDataId;
    int dataId;
    bool dataIdIsValid = false;

    int nofData;
    QString date;
    QString data;

    if((m_stockName.length() < 1) || m_stockSymbol.length() < 1)
    {
        QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Välj aktie"));
        return;
    }


    str = (QString::fromUtf8("Vill du lägga till sub data?\n"));
    str = str + m_stockName;
    str = str + ", ";
    str = str + m_stockSymbol;
    str = str + ", ";
    str = str + m_analysisDate;





    if(QMessageBox::No == QMessageBox::question(this, QString::fromUtf8("Uppdatera databas"), str, QMessageBox::Yes|QMessageBox::No))
    {
        return;
    }

    // Check if this stocksymbol and stockname is already added, if not add it
    res = db.mainAnalysisDataExists(m_stockName,
                                m_stockSymbol,
                                mainAnalysisId);
    if(false == res)
    {
        res = db.insertMainAnalysisData(m_stockName,
                                     m_stockSymbol,
                                     mainAnalysisId);
    }



    nofData = ui->tableWidgetTotalCurrentLiabilities->rowCount();

    for(int row = 0; row < nofData; row++)
    {
       if(NULL != ui->tableWidgetTotalCurrentLiabilities->item(row, 0))
       {
            date = ui->tableWidgetTotalCurrentLiabilities->item(row, 0)->text();
            data = ui->tableWidgetTotalCurrentLiabilities->item(row, 1)->text();

            date.toInt(&isValid);
            if (false == isValid)
            {
               QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Year is not a number"));
               continue;
            }

            CUtil cu;
            cu.number2double(data, data);
            data.toDouble(&isValid);
            if (false == isValid)
            {
               QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Earnings is not a number"));
               continue;
            }
       }
       else
       {
           break;
       }


        res = db.subAnalysisTotalCurrentLiabilitiesDateExists(date,
                                                        mainAnalysisId,
                                                        dateId);

        // Exist anaysis date?
        if(false == res)
        {
            res = db.insertSubAnalysisTotalCurrentLiabilitiesDate(date,
                                                            mainAnalysisId,
                                                            dateId);
        }


        if(true == res)
        {
           dataIdIsValid = false;

           if( true == db.getSubAnalysisTotalCurrentLiabilitiesDataId(mainAnalysisId, dateId, inputDataId))
           {
               dataIdIsValid = true;
           }

            res = db.insertSubAnalysisTotalCurrentLiabilities(dateId,
                                                              mainAnalysisId,
                                                              inputDataId,
                                                              dataIdIsValid,
                                                              data,
                                                              dataId);
        }
    }

}



/******************************************************************
 *
 * Function:    on_pushButtonSaveTotalLiabilities_clicked()
 *
 * Description: This function saves Total Liabilities data to db
 *
 *
 *
 *****************************************************************/
void StockAnalysisTab::on_pushButtonSaveTotalLiabilities_clicked()
{
    CDbHndl db;
    bool res;
    int mainAnalysisId;
    QString str;

    bool isValid;
    int dateId;
    int inputDataId;
    int dataId;
    bool dataIdIsValid = false;

    int nofData;
    QString date;
    QString data;


    if((m_stockName.length() < 1) || m_stockSymbol.length() < 1)
    {
        QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Välj aktie"));
        return;
    }


    str = (QString::fromUtf8("Vill du lägga till sub data?\n"));
    str = str + m_stockName;
    str = str + ", ";
    str = str + m_stockSymbol;
    str = str + ", ";
    str = str + m_analysisDate;




    if(QMessageBox::No == QMessageBox::question(this, QString::fromUtf8("Uppdatera databas"), str, QMessageBox::Yes|QMessageBox::No))
    {
        return;
    }

    // Check if this stocksymbol and stockname is already added, if not add it
    res = db.mainAnalysisDataExists(m_stockName,
                                m_stockSymbol,
                                mainAnalysisId);
    if(false == res)
    {
        res = db.insertMainAnalysisData(m_stockName,
                                     m_stockSymbol,
                                     mainAnalysisId);
    }



    nofData = ui->tableWidgetTotalLiabilities->rowCount();

    for(int row = 0; row < nofData; row++)
    {
       if(NULL != ui->tableWidgetTotalLiabilities->item(row, 0))
       {
            date = ui->tableWidgetTotalLiabilities->item(row, 0)->text();
            data = ui->tableWidgetTotalLiabilities->item(row, 1)->text();

            date.toInt(&isValid);
            if (false == isValid)
            {
               QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Year is not a number"));
               continue;
            }

            CUtil cu;
            cu.number2double(data, data);
            data.toDouble(&isValid);
            if (false == isValid)
            {
               QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Earnings is not a number"));
               continue;
            }
       }
       else
       {
           break;
       }


        res = db.subAnalysisTotalLiabilitiesDateExists(date,
                                                       mainAnalysisId,
                                                       dateId);

        // Exist anaysis date?
        if(false == res)
        {
            res = db.insertSubAnalysisTotalLiabilitiesDate(date,
                                                           mainAnalysisId,
                                                           dateId);
        }


        if(true == res)
        {
           dataIdIsValid = false;

           if( true == db.getSubAnalysisTotalLiabilitiesDataId(mainAnalysisId, dateId, inputDataId))
           {
               dataIdIsValid = true;
           }

            res = db.insertSubAnalysisTotalLiabilities(dateId,
                                                       mainAnalysisId,
                                                       inputDataId,
                                                       dataIdIsValid,
                                                       data,
                                                       dataId);
        }
    }

}



/******************************************************************
 *
 * Function:    on_pushButtonSaveTotalLiabilities_clicked()
 *
 * Description: This function set company type and save it in db
 *
 *
 *
 *****************************************************************/
void StockAnalysisTab::on_pushButtonSelectSaveCompanyType_clicked()
{
    CDbHndl db;
    bool res;
    int mainAnalysisId;
    int companyType;
    int companyTypeId;


    if((m_stockName.length() < 1) || m_stockSymbol.length() < 1)
    {
        QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Välj aktie"));
        return;
    }

    QString str;

    str = (QString::fromUtf8("Vill du lägga till bolagstyp?\n"));
    str = str + m_stockName;
    str = str + ", ";
    str = str + m_stockSymbol;


    if(m_stockNameIsInit == false)
    {
        return;
    }

    if(QMessageBox::No == QMessageBox::question(this, QString::fromUtf8("Uppdatera databas"), str, QMessageBox::Yes|QMessageBox::No))
    {
        return;
    }

    // Check if this stocksymbol and stockname is already added, if not add it
    res = db.mainAnalysisDataExists(m_stockName,
                                m_stockSymbol,
                                mainAnalysisId);
    if(false == res)
    {
        res = db.insertMainAnalysisData(m_stockName,
                                     m_stockSymbol,
                                     mainAnalysisId);
    }


    companyType = ui->comboBoxCompanyType->currentIndex();

    db.deleteCompanyType(mainAnalysisId);

    res = db.insertSubAnalysisCompanyType(companyType,
                                          mainAnalysisId,
                                          companyTypeId);

    if(companyType < NOF_COMPANY_TYPE_ARR_DATA)
    {
        ui->labelCompanyType->setText(m_companyTypesArr[companyType]);
        ui->labelMainPglabelCompanyType->setText(m_companyTypesArr[companyType]);

        ui->comboBoxCompanyType->setCurrentIndex(companyType);
    }

}


/******************************************************************
 *
 * Function:    on_pushButtonSaveSolidity_clicked()
 *
 * Description: This function saves Solidity data to db
 *
 *
 *
 *****************************************************************/
void StockAnalysisTab::on_pushButtonSaveSolidity_clicked()
{
    CDbHndl db;
    bool res;
    int mainAnalysisId;
    QString str;

    bool isValid;
    int dateId;
    int inputDataId;
    int dataId;
    bool dataIdIsValid = false;

    int nofData;
    QString date;
    QString data;

    if((m_stockName.length() < 1) || m_stockSymbol.length() < 1)
    {
        QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Välj aktie"));
        return;
    }


    str = (QString::fromUtf8("Vill du lägga till sub data?\n"));
    str = str + m_stockName;
    str = str + ", ";
    str = str + m_stockSymbol;
    str = str + ", ";
    str = str + m_analysisDate;





    if(QMessageBox::No == QMessageBox::question(this, QString::fromUtf8("Uppdatera databas"), str, QMessageBox::Yes|QMessageBox::No))
    {
        return;
    }

    // Check if this stocksymbol and stockname is already added, if not add it
    res = db.mainAnalysisDataExists(m_stockName,
                                m_stockSymbol,
                                mainAnalysisId);
    if(false == res)
    {
        res = db.insertMainAnalysisData(m_stockName,
                                     m_stockSymbol,
                                     mainAnalysisId);
    }



    nofData = ui->tableWidgetSolidity->rowCount();

    for(int row = 0; row < nofData; row++)
    {
       if(NULL != ui->tableWidgetSolidity->item(row, 0))
       {
            date = ui->tableWidgetSolidity->item(row, 0)->text();
            data = ui->tableWidgetSolidity->item(row, 1)->text();

            date.toInt(&isValid);
            if (false == isValid)
            {
               QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Year is not a number"));
               continue;
            }

            CUtil cu;
            cu.number2double(data, data);
            data.toDouble(&isValid);
            if (false == isValid)
            {
               QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Solidity is not a number"));
               continue;
            }
       }
       else
       {
           break;
       }


        res = db.subAnalysisSolidityDateExists(date,
                                               mainAnalysisId,
                                               dateId);

        // Exist anaysis date?
        if(false == res)
        {
            res = db.insertSubAnalysisSolidityDate(date,
                                                   mainAnalysisId,
                                                   dateId);
        }


        if(true == res)
        {
           dataIdIsValid = false;

           if( true == db.getSubAnalysisSolidityDataId(mainAnalysisId, dateId, inputDataId))
           {
               dataIdIsValid = true;
           }

            res = db.insertSubAnalysisSolidityData(dateId,
                                               mainAnalysisId,
                                               inputDataId,
                                               dataIdIsValid,
                                               data,
                                               dataId);
        }
    }

}

/******************************************************************
 *
 * Function:    on_pushButtonSaveCoverageRatio_clicked()
 *
 * Description: This function saves Coverage Ratio data to db
 *
 *
 *
 *****************************************************************/
void StockAnalysisTab::on_pushButtonSaveCoverageRatio_clicked()
{
    CDbHndl db;
    bool res;
    int mainAnalysisId;
    QString str;

    bool isValid;
    int dateId;
    int inputDataId;
    int dataId;
    bool dataIdIsValid = false;

    int nofData;
    QString date;
    QString data;


    if((m_stockName.length() < 1) || m_stockSymbol.length() < 1)
    {
        QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Välj aktie"));
        return;
    }


    str = (QString::fromUtf8("Vill du lägga till sub data?\n"));
    str = str + m_stockName;
    str = str + ", ";
    str = str + m_stockSymbol;
    str = str + ", ";
    str = str + m_analysisDate;


    if(QMessageBox::No == QMessageBox::question(this, QString::fromUtf8("Uppdatera databas"), str, QMessageBox::Yes|QMessageBox::No))
    {
        return;
    }

    // Check if this stocksymbol and stockname is already added, if not add it
    res = db.mainAnalysisDataExists(m_stockName, m_stockSymbol, mainAnalysisId);

    if(false == res)
    {
        res = db.insertMainAnalysisData(m_stockName, m_stockSymbol, mainAnalysisId);
    }


    nofData = ui->tableWidgetCoverageRatio->rowCount();

    for(int row = 0; row < nofData; row++)
    {
        if(NULL != ui->tableWidgetCoverageRatio->item(row, 0))
        {
            date = ui->tableWidgetCoverageRatio->item(row, 0)->text();
            data = ui->tableWidgetCoverageRatio->item(row, 1)->text();

            date.toInt(&isValid);
            if (false == isValid)
            {
                QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Year is not a number"));
                continue;
            }

            CUtil cu;
            cu.number2double(data, data);
            data.toDouble(&isValid);
            if (false == isValid)
            {
                QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("CoverageRatio is not a number"));
                continue;
            }
        }
        else
        {
           break;
        }


        res = db.subAnalysisCoverageRatioDateExists(date, mainAnalysisId, dateId);

        // Exist anaysis date?
        if(false == res)
        {
            res = db.insertSubAnalysisCoverageRatioDate(date, mainAnalysisId, dateId);
        }


        if(true == res)
        {
           dataIdIsValid = false;

           if( true == db.getSubAnalysisCoverageRatioDataId(mainAnalysisId, dateId, inputDataId))
           {
               dataIdIsValid = true;
           }

           res = db.insertSubAnalysisCoverageRatioData(dateId, mainAnalysisId, inputDataId, dataIdIsValid,
                                                       data, dataId);
        }
    }
}


/******************************************************************
 *
 * Function:    on_pushButtonSaveCoreTier1Ratio_clicked()
 *
 * Description: This function saves CoreTier1Ratio data to db
 *
 *
 *
 *****************************************************************/
void StockAnalysisTab::on_pushButtonSaveCoreTier1Ratio_clicked()
{
    CDbHndl db;
    bool res;
    int mainAnalysisId;
    QString str;

    bool isValid;
    int dateId;
    int inputDataId;
    int dataId;
    bool dataIdIsValid = false;

    int nofData;
    QString date;
    QString data;


    if((m_stockName.length() < 1) || m_stockSymbol.length() < 1)
    {
        QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Välj aktie"));
        return;
    }


    str = (QString::fromUtf8("Vill du lägga till sub data?\n"));
    str = str + m_stockName;
    str = str + ", ";
    str = str + m_stockSymbol;
    str = str + ", ";
    str = str + m_analysisDate;


    if(QMessageBox::No == QMessageBox::question(this, QString::fromUtf8("Uppdatera databas"), str, QMessageBox::Yes|QMessageBox::No))
    {
        return;
    }

    // Check if this stocksymbol and stockname is already added, if not add it
    res = db.mainAnalysisDataExists(m_stockName, m_stockSymbol, mainAnalysisId);

    if(false == res)
    {
        res = db.insertMainAnalysisData(m_stockName, m_stockSymbol, mainAnalysisId);
    }


    nofData = ui->tableWidgetCoreTier1Ratio->rowCount();

    for(int row = 0; row < nofData; row++)
    {
        if(NULL != ui->tableWidgetCoreTier1Ratio->item(row, 0))
        {
            date = ui->tableWidgetCoreTier1Ratio->item(row, 0)->text();
            data = ui->tableWidgetCoreTier1Ratio->item(row, 1)->text();

            date.toInt(&isValid);
            if (false == isValid)
            {
                QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Year is not a number"));
                continue;
            }

            CUtil cu;
            cu.number2double(data, data);
            data.toDouble(&isValid);
            if (false == isValid)
            {
                QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("CoreTier1Ratio is not a number"));
                continue;
            }
        }
        else
        {
           break;
        }


        res = db.subAnalysisCoreTier1RatioDateExists(date, mainAnalysisId, dateId);

        // Exist anaysis date?
        if(false == res)
        {
            res = db.insertSubAnalysisCoreTier1RatioDate(date, mainAnalysisId, dateId);
        }


        if(true == res)
        {
           dataIdIsValid = false;

           if( true == db.getSubAnalysisCoreTier1RatioDataId(mainAnalysisId, dateId, inputDataId))
           {
               dataIdIsValid = true;
           }

           res = db.insertSubAnalysisCoreTier1RatioData(dateId, mainAnalysisId, inputDataId, dataIdIsValid,
                                                       data, dataId);
        }
    }
}



/******************************************************************
 *
 * Function:    on_pushButtonSaveCoreCapitalRatio_clicked()
 *
 * Description: This function saves CoreCapitalRatio data to db
 *
 *
 *
 *****************************************************************/
void StockAnalysisTab::on_pushButtonSaveCoreCapitalRatio_clicked()
{
    CDbHndl db;
    bool res;
    int mainAnalysisId;
    QString str;

    bool isValid;
    int dateId;
    int inputDataId;
    int dataId;
    bool dataIdIsValid = false;

    int nofData;
    QString date;
    QString data;


    if((m_stockName.length() < 1) || m_stockSymbol.length() < 1)
    {
        QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Välj aktie"));
        return;
    }


    str = (QString::fromUtf8("Vill du lägga till sub data?\n"));
    str = str + m_stockName;
    str = str + ", ";
    str = str + m_stockSymbol;
    str = str + ", ";
    str = str + m_analysisDate;


    if(QMessageBox::No == QMessageBox::question(this, QString::fromUtf8("Uppdatera databas"), str, QMessageBox::Yes|QMessageBox::No))
    {
        return;
    }

    // Check if this stocksymbol and stockname is already added, if not add it
    res = db.mainAnalysisDataExists(m_stockName, m_stockSymbol, mainAnalysisId);

    if(false == res)
    {
        res = db.insertMainAnalysisData(m_stockName, m_stockSymbol, mainAnalysisId);
    }


    nofData = ui->tableWidgetCoreCapitalRatio->rowCount();

    for(int row = 0; row < nofData; row++)
    {
        if(NULL != ui->tableWidgetCoreCapitalRatio->item(row, 0))
        {
            date = ui->tableWidgetCoreCapitalRatio->item(row, 0)->text();
            data = ui->tableWidgetCoreCapitalRatio->item(row, 1)->text();

            date.toInt(&isValid);
            if (false == isValid)
            {
                QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Year is not a number"));
                continue;
            }

            CUtil cu;
            cu.number2double(data, data);
            data.toDouble(&isValid);
            if (false == isValid)
            {
                QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("CoreCapitalRatio is not a number"));
                continue;
            }
        }
        else
        {
           break;
        }


        res = db.subAnalysisCoreCapitalRatioDateExists(date, mainAnalysisId, dateId);

        // Exist anaysis date?
        if(false == res)
        {
            res = db.insertSubAnalysisCoreCapitalRatioDate(date, mainAnalysisId, dateId);
        }


        if(true == res)
        {
           dataIdIsValid = false;

           if( true == db.getSubAnalysisCoreCapitalRatioDataId(mainAnalysisId, dateId, inputDataId))
           {
               dataIdIsValid = true;
           }

           res = db.insertSubAnalysisCoreCapitalRatioData(dateId, mainAnalysisId, inputDataId, dataIdIsValid,
                                                       data, dataId);
        }
    }

}




/******************************************************************
 *
 * Function:    on_pushButtonSaveEquity_clicked()
 *
 * Description: This function saves Equity data to db
 *
 *
 *
 *****************************************************************/
void StockAnalysisTab::on_pushButtonSaveEquity_clicked()
{
    CDbHndl db;
    bool res;
    int mainAnalysisId;
    QString str;

    bool isValid;
    int dateId;
    int inputDataId;
    int dataId;
    bool dataIdIsValid = false;

    int nofData;
    QString date;
    QString data;


    if((m_stockName.length() < 1) || m_stockSymbol.length() < 1)
    {
        QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Välj aktie"));
        return;
    }


    str = (QString::fromUtf8("Vill du lägga till sub data?\n"));
    str = str + m_stockName;
    str = str + ", ";
    str = str + m_stockSymbol;
    str = str + ", ";
    str = str + m_analysisDate;


    if(QMessageBox::No == QMessageBox::question(this, QString::fromUtf8("Uppdatera databas"), str, QMessageBox::Yes|QMessageBox::No))
    {
        return;
    }

    // Check if this stocksymbol and stockname is already added, if not add it
    res = db.mainAnalysisDataExists(m_stockName, m_stockSymbol, mainAnalysisId);

    if(false == res)
    {
        res = db.insertMainAnalysisData(m_stockName, m_stockSymbol, mainAnalysisId);
    }


    nofData = ui->tableWidgetEquity->rowCount();

    for(int row = 0; row < nofData; row++)
    {
        if(NULL != ui->tableWidgetEquity->item(row, 0))
        {
            date = ui->tableWidgetEquity->item(row, 0)->text();
            data = ui->tableWidgetEquity->item(row, 1)->text();

            date.toInt(&isValid);
            if (false == isValid)
            {
                QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Year is not a number"));
                continue;
            }

            CUtil cu;
            cu.number2double(data, data);
            data.toDouble(&isValid);
            if (false == isValid)
            {
                QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Equity is not a number"));
                continue;
            }
        }
        else
        {
           break;
        }


        res = db.subAnalysisEquityDateExists(date, mainAnalysisId, dateId);

        // Exist anaysis date?
        if(false == res)
        {
            res = db.insertSubAnalysisEquityDate(date, mainAnalysisId, dateId);
        }


        if(true == res)
        {
           dataIdIsValid = false;

           if( true == db.getSubAnalysisEquityDataId(mainAnalysisId, dateId, inputDataId))
           {
               dataIdIsValid = true;
           }

           res = db.insertSubAnalysisEquityData(dateId, mainAnalysisId, inputDataId, dataIdIsValid,
                                                       data, dataId);
        }
    }
}


/******************************************************************
 *
 * Function:    on_pushButtonSaveEquity_clicked()
 *
 * Description: This function saves CashFlowCapex data to db
 *
 *
 *
 *****************************************************************/
void StockAnalysisTab::on_pushButtonSaveCashFlowCapex_clicked()
{
    CDbHndl db;
    bool res;
    int mainAnalysisId;
    QString str;

    bool isValid;
    int dateId;
    int inputDataId;
    int dataId;
    bool dataIdIsValid = false;

    int nofData;
    QString date;
    QString data;


    if((m_stockName.length() < 1) || m_stockSymbol.length() < 1)
    {
        QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Välj aktie"));
        return;
    }


    str = (QString::fromUtf8("Vill du lägga till sub data?\n"));
    str = str + m_stockName;
    str = str + ", ";
    str = str + m_stockSymbol;
    str = str + ", ";
    str = str + m_analysisDate;


    if(QMessageBox::No == QMessageBox::question(this, QString::fromUtf8("Uppdatera databas"), str, QMessageBox::Yes|QMessageBox::No))
    {
        return;
    }

    // Check if this stocksymbol and stockname is already added, if not add it
    res = db.mainAnalysisDataExists(m_stockName, m_stockSymbol, mainAnalysisId);

    if(false == res)
    {
        res = db.insertMainAnalysisData(m_stockName, m_stockSymbol, mainAnalysisId);
    }


    nofData = ui->tableWidgetCashFlowCapex->rowCount();

    for(int row = 0; row < nofData; row++)
    {
        if(NULL != ui->tableWidgetCashFlowCapex->item(row, 0))
        {
            date = ui->tableWidgetCashFlowCapex->item(row, 0)->text();
            data = ui->tableWidgetCashFlowCapex->item(row, 1)->text();

            date.toInt(&isValid);
            if (false == isValid)
            {
                QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Year is not a number"));
                continue;
            }

            CUtil cu;
            cu.number2double(data, data);
            data.toDouble(&isValid);
            if (false == isValid)
            {
                QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("CashFlowCapex is not a number"));
                continue;
            }
        }
        else
        {
           break;
        }


        res = db.subAnalysisCashFlowCapexDateExists(date, mainAnalysisId, dateId);

        // Exist anaysis date?
        if(false == res)
        {
            res = db.insertSubAnalysisCashFlowCapexDate(date, mainAnalysisId, dateId);
        }


        if(true == res)
        {
           dataIdIsValid = false;

           if( true == db.getSubAnalysisCashFlowCapexDataId(mainAnalysisId, dateId, inputDataId))
           {
               dataIdIsValid = true;
           }

           res = db.insertSubAnalysisCashFlowCapexData(dateId, mainAnalysisId, inputDataId, dataIdIsValid,
                                                       data, dataId);
        }
    }

}


/******************************************************************
 *
 * Function:    on_pushButtonSaveEquity_clicked()
 *
 * Description: This function saves Operating Cash Flow data to db
 *
 *
 *
 *****************************************************************/
void StockAnalysisTab::on_pushButtonSaveOperatingCashFlow_clicked()
{

    CDbHndl db;
    bool res;
    int mainAnalysisId;
    QString str;

    bool isValid;
    int dateId;
    int inputDataId;
    int dataId;
    bool dataIdIsValid = false;

    int nofData;
    QString date;
    QString data;


    if((m_stockName.length() < 1) || m_stockSymbol.length() < 1)
    {
        QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Välj aktie"));
        return;
    }


    str = (QString::fromUtf8("Vill du lägga till sub data?\n"));
    str = str + m_stockName;
    str = str + ", ";
    str = str + m_stockSymbol;
    str = str + ", ";
    str = str + m_analysisDate;


    if(QMessageBox::No == QMessageBox::question(this, QString::fromUtf8("Uppdatera databas"), str, QMessageBox::Yes|QMessageBox::No))
    {
        return;
    }

    // Check if this stocksymbol and stockname is already added, if not add it
    res = db.mainAnalysisDataExists(m_stockName, m_stockSymbol, mainAnalysisId);

    if(false == res)
    {
        res = db.insertMainAnalysisData(m_stockName, m_stockSymbol, mainAnalysisId);
    }


    nofData = ui->tableWidgetOperatingCashFlow->rowCount();

    for(int row = 0; row < nofData; row++)
    {
        if(NULL != ui->tableWidgetOperatingCashFlow->item(row, 0))
        {
            date = ui->tableWidgetOperatingCashFlow->item(row, 0)->text();
            data = ui->tableWidgetOperatingCashFlow->item(row, 1)->text();

            date.toInt(&isValid);
            if (false == isValid)
            {
                QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Year is not a number"));
                continue;
            }

            CUtil cu;
            cu.number2double(data, data);
            data.toDouble(&isValid);
            if (false == isValid)
            {
                QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("OperatingCashFlow is not a number"));
                continue;
            }
        }
        else
        {
           break;
        }


        res = db.subAnalysisOperatingCashFlowDateExists(date, mainAnalysisId, dateId);

        // Exist anaysis date?
        if(false == res)
        {
            res = db.insertSubAnalysisOperatingCashFlowDate(date, mainAnalysisId, dateId);
        }


        if(true == res)
        {
           dataIdIsValid = false;

           if( true == db.getSubAnalysisOperatingCashFlowDataId(mainAnalysisId, dateId, inputDataId))
           {
               dataIdIsValid = true;
           }

           res = db.insertSubAnalysisOperatingCashFlowData(dateId, mainAnalysisId, inputDataId, dataIdIsValid,
                                                       data, dataId);
        }
    }
}






/******************************************************************
 *
 * Function:    plotCashflowData()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
void StockAnalysisTab::plotCashflowData(void)
{
    int i;
    CDbHndl db;
    std::vector<double> data_y;
    std::vector<double> data_x;
    double minX;
    double maxX;
    double minY;
    double maxY;


    if((m_nofOperatingCashFlowData < 2) && (m_nofOperatingCashFlowData != m_nofCashFlowCapexData))
    {
        return;
    }



    minX = m_operatingCashFlowArr[0].date.toDouble();
    maxX = m_operatingCashFlowArr[0].date.toDouble();
    minY = m_operatingCashFlowArr[0].data.toDouble();
    maxY = m_operatingCashFlowArr[0].data.toDouble();


    for(i = 0; i < m_nofOperatingCashFlowData; i++)
    {

        data_x.push_back(m_operatingCashFlowArr[i].date.toDouble());
        data_y.push_back(m_operatingCashFlowArr[i].data.toDouble());

        // Check x
        if(minX > m_operatingCashFlowArr[i].date.toDouble())
        {
            minX = m_operatingCashFlowArr[i].date.toDouble();
        }

        if(maxX < m_operatingCashFlowArr[i].date.toDouble())
        {
            maxX = m_operatingCashFlowArr[i].date.toDouble();
        }

        // Check y
        if(minY > m_operatingCashFlowArr[i].data.toDouble())
        {
            minY = m_operatingCashFlowArr[i].data.toDouble();
        }

        if(maxY < m_operatingCashFlowArr[i].data.toDouble())
        {
            maxY = m_operatingCashFlowArr[i].data.toDouble();
        }
    }


    m_qwtcashFlowPlotData.stock[0].data.detach();
    m_qwtcashFlowPlotData.stock[0].data.setData(NULL);


    m_qwtcashFlowPlotData.stock[0].data.setSamples(data_x.data(),data_y.data(),data_y.size());
    m_qwtcashFlowPlotData.stock[0].data.setPen(QPen(Qt::blue, 2));
    m_qwtcashFlowPlotData.stock[0].data.setSymbol( new QwtSymbol(QwtSymbol::Ellipse,                                                            Qt::blue,
                                                                 QPen( Qt::blue ),
                                                                  QSize( 7, 7 ) ) );


    data_x.clear();
    data_y.clear();


    for(i = 0; i < m_nofCashFlowCapexData; i++)
    {

        data_x.push_back(m_cashFlowCapexArr[i].date.toDouble());
        data_y.push_back(-m_cashFlowCapexArr[i].data.toDouble());

        // Check x
        if(minX > m_cashFlowCapexArr[i].date.toDouble())
        {
            minX = m_cashFlowCapexArr[i].date.toDouble();
        }

        if(maxX < m_cashFlowCapexArr[i].date.toDouble())
        {
            maxX = m_cashFlowCapexArr[i].date.toDouble();
        }

        // Check y
        if(minY > -m_cashFlowCapexArr[i].data.toDouble())
        {
            minY = -m_cashFlowCapexArr[i].data.toDouble();
        }

        if(maxY < -m_cashFlowCapexArr[i].data.toDouble())
        {
            maxY = -m_cashFlowCapexArr[i].data.toDouble();
        }
    }

    // ui->qwtCashFlowPlot->setAxisScale(QwtPlot::xBottom, minX, maxX);
    // ui->qwtCashFlowPlot->setAxisScale(QwtPlot::yLeft, minY, maxY); // Max av % satser

    m_qwtcashFlowPlotData.stock[1].data.detach();
    m_qwtcashFlowPlotData.stock[1].data.setData(NULL);


    m_qwtcashFlowPlotData.stock[1].data.setSamples(data_x.data(),data_y.data(),data_y.size());
    m_qwtcashFlowPlotData.stock[1].data.setPen(QPen(Qt::red, 2));
    m_qwtcashFlowPlotData.stock[1].data.setSymbol( new QwtSymbol(QwtSymbol::Ellipse,
                                                                 Qt::red,
                                                                 QPen( Qt::red ),
                                                                  QSize( 7, 7 ) ) );


    //================
    data_x.clear();
    data_y.clear();


    for(i = 0; i < m_nofTotDividensData; i++)
    {

        data_x.push_back(m_totDividensArr[i].date.toDouble());
        data_y.push_back(-m_totDividensArr[i].data.toDouble());

        // Check x
        if(minX > m_totDividensArr[i].date.toDouble())
        {
            minX = m_totDividensArr[i].date.toDouble();
        }

        if(maxX < m_totDividensArr[i].date.toDouble())
        {
            maxX = m_totDividensArr[i].date.toDouble();
        }

        // Check y
        if(minY > -m_totDividensArr[i].data.toDouble())
        {
            minY = -m_totDividensArr[i].data.toDouble();
        }

        if(maxY < -m_totDividensArr[i].data.toDouble())
        {
            maxY = -m_totDividensArr[i].data.toDouble();
        }
    }

    ui->qwtCashFlowPlot->setAxisScale(QwtPlot::xBottom, minX, maxX);
    ui->qwtCashFlowPlot->setAxisScale(QwtPlot::yLeft, minY, maxY); // Max av % satser

    m_qwtcashFlowPlotData.stock[2].data.detach();
    m_qwtcashFlowPlotData.stock[2].data.setData(NULL);


    m_qwtcashFlowPlotData.stock[2].data.setSamples(data_x.data(),data_y.data(),data_y.size());
    m_qwtcashFlowPlotData.stock[2].data.setPen(QPen(Qt::green, 2));
    m_qwtcashFlowPlotData.stock[2].data.setSymbol( new QwtSymbol(QwtSymbol::Ellipse,
                                                                 Qt::green,
                                                                 QPen( Qt::red ),
                                                                  QSize( 7, 7 ) ) );


    //================

    // 1:st curve
    m_qwtcashFlowPlotData.stock[0].data.attach(ui->qwtCashFlowPlot);
    ui->qwtCashFlowPlot->replot();

    // 2:st curve
    m_qwtcashFlowPlotData.stock[1].data.attach(ui->qwtCashFlowPlot);
    ui->qwtCashFlowPlot->replot();

    // 3:th curve
    m_qwtcashFlowPlotData.stock[2].data.attach(ui->qwtCashFlowPlot);
    ui->qwtCashFlowPlot->replot();

    data_x.clear();
    data_y.clear();

}



/******************************************************************
 *
 * Function:    on_pushButtonSaveTotDividends_clicked()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
void StockAnalysisTab::on_pushButtonSaveTotDividends_clicked()
{

    CDbHndl db;
    bool res;
    int mainAnalysisId;
    QString str;

    bool isValid;
    int dateId;
    int inputDataId;
    int dataId;
    bool dataIdIsValid = false;

    int nofData;
    QString date;
    QString data;


    if((m_stockName.length() < 1) || m_stockSymbol.length() < 1)
    {
        QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Välj aktie"));
        return;
    }


    str = (QString::fromUtf8("Vill du lägga till sub data?\n"));
    str = str + m_stockName;
    str = str + ", ";
    str = str + m_stockSymbol;
    str = str + ", ";
    str = str + m_analysisDate;


    if(QMessageBox::No == QMessageBox::question(this, QString::fromUtf8("Uppdatera databas"), str, QMessageBox::Yes|QMessageBox::No))
    {
        return;
    }

    // Check if this stocksymbol and stockname is already added, if not add it
    res = db.mainAnalysisDataExists(m_stockName, m_stockSymbol, mainAnalysisId);

    if(false == res)
    {
        res = db.insertMainAnalysisData(m_stockName, m_stockSymbol, mainAnalysisId);
    }


    nofData = ui->tableWidgetTotDividends->rowCount();

    for(int row = 0; row < nofData; row++)
    {
        if(NULL != ui->tableWidgetTotDividends->item(row, 0))
        {
            date = ui->tableWidgetTotDividends->item(row, 0)->text();
            data = ui->tableWidgetTotDividends->item(row, 1)->text();

            date.toInt(&isValid);
            if (false == isValid)
            {
                QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Year is not a number"));
                continue;
            }

            CUtil cu;
            cu.number2double(data, data);
            data.toDouble(&isValid);
            if (false == isValid)
            {
                QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("TotDividends is not a number"));
                continue;
            }
        }
        else
        {
           break;
        }


        res = db.subAnalysisTotDividendsDateExists(date, mainAnalysisId, dateId);

        // Exist anaysis date?
        if(false == res)
        {
            res = db.insertSubAnalysisTotDividendsDate(date, mainAnalysisId, dateId);
        }


        if(true == res)
        {
           dataIdIsValid = false;

           if( true == db.getSubAnalysisTotDividendsDataId(mainAnalysisId, dateId, inputDataId))
           {
               dataIdIsValid = true;
           }

           res = db.insertSubAnalysisTotDividendsData(dateId, mainAnalysisId, inputDataId, dataIdIsValid,
                                                       data, dataId);
        }
    }

}


/******************************************************************
 *
 * Function:    on_pushButtonSaveRevenue_clicked()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
void StockAnalysisTab::on_pushButtonSaveRevenue_clicked()
{

    CDbHndl db;
    bool res;
    int mainAnalysisId;
    QString str;

    bool isValid;
    int dateId;
    int inputDataId;
    int dataId;
    bool dataIdIsValid = false;

    int nofData;
    QString date;
    QString data;


    if((m_stockName.length() < 1) || m_stockSymbol.length() < 1)
    {
        QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Välj aktie"));
        return;
    }


    str = (QString::fromUtf8("Vill du lägga till sub data?\n"));
    str = str + m_stockName;
    str = str + ", ";
    str = str + m_stockSymbol;
    str = str + ", ";
    str = str + m_analysisDate;


    if(QMessageBox::No == QMessageBox::question(this, QString::fromUtf8("Uppdatera databas"), str, QMessageBox::Yes|QMessageBox::No))
    {
        return;
    }

    // Check if this stocksymbol and stockname is already added, if not add it
    res = db.mainAnalysisDataExists(m_stockName, m_stockSymbol, mainAnalysisId);

    if(false == res)
    {
        res = db.insertMainAnalysisData(m_stockName, m_stockSymbol, mainAnalysisId);
    }


    nofData = ui->tableWidgetRevenue->rowCount();

    for(int row = 0; row < nofData; row++)
    {
        if(NULL != ui->tableWidgetRevenue->item(row, 0))
        {
            date = ui->tableWidgetRevenue->item(row, 0)->text();
            data = ui->tableWidgetRevenue->item(row, 1)->text();

            date.toInt(&isValid);
            if (false == isValid)
            {
                QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Year is not a number"));
                continue;
            }

            CUtil cu;
            cu.number2double(data, data);
            data.toDouble(&isValid);
            if (false == isValid)
            {
                QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("TotDividends is not a number"));
                continue;
            }
        }
        else
        {
           break;
        }


        res = db.subAnalysisRevenueDateExists(date, mainAnalysisId, dateId);

        // Exist anaysis date?
        if(false == res)
        {
            res = db.insertSubAnalysisRevenueDate(date, mainAnalysisId, dateId);
        }


        if(true == res)
        {
           dataIdIsValid = false;

           if( true == db.getSubAnalysisRevenueDataId(mainAnalysisId, dateId, inputDataId))
           {
               dataIdIsValid = true;
           }

           res = db.insertSubAnalysisRevenueData(dateId, mainAnalysisId, inputDataId, dataIdIsValid,
                                                       data, dataId);
        }
    }
}



/******************************************************************
 *
 * Function:    on_pushButtonSaveNetIncome_clicked()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
void StockAnalysisTab::on_pushButtonSaveNetIncome_clicked()
{

    CDbHndl db;
    bool res;
    int mainAnalysisId;
    QString str;

    bool isValid;
    int dateId;
    int inputDataId;
    int dataId;
    bool dataIdIsValid = false;

    int nofData;
    QString date;
    QString data;


    if((m_stockName.length() < 1) || m_stockSymbol.length() < 1)
    {
        QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Välj aktie"));
        return;
    }


    str = (QString::fromUtf8("Vill du lägga till sub data?\n"));
    str = str + m_stockName;
    str = str + ", ";
    str = str + m_stockSymbol;
    str = str + ", ";
    str = str + m_analysisDate;


    if(QMessageBox::No == QMessageBox::question(this, QString::fromUtf8("Uppdatera databas"), str, QMessageBox::Yes|QMessageBox::No))
    {
        return;
    }

    // Check if this stocksymbol and stockname is already added, if not add it
    res = db.mainAnalysisDataExists(m_stockName, m_stockSymbol, mainAnalysisId);

    if(false == res)
    {
        res = db.insertMainAnalysisData(m_stockName, m_stockSymbol, mainAnalysisId);
    }


    nofData = ui->tableWidgetNetIncome->rowCount();

    for(int row = 0; row < nofData; row++)
    {
        if(NULL != ui->tableWidgetNetIncome->item(row, 0))
        {
            date = ui->tableWidgetNetIncome->item(row, 0)->text();
            data = ui->tableWidgetNetIncome->item(row, 1)->text();

            date.toInt(&isValid);
            if (false == isValid)
            {
                QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Year is not a number"));
                continue;
            }

            CUtil cu;
            cu.number2double(data, data);
            data.toDouble(&isValid);
            if (false == isValid)
            {
                QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("TotDividends is not a number"));
                continue;
            }
        }
        else
        {
           break;
        }


        res = db.subAnalysisTotEarningsDateExists(date, mainAnalysisId, dateId);

        // Exist anaysis date?
        if(false == res)
        {
            res = db.insertSubAnalysisTotEarningsDate(date, mainAnalysisId, dateId);
        }


        if(true == res)
        {
           dataIdIsValid = false;

           if( true == db.getSubAnalysisTotEarningsDataId(mainAnalysisId, dateId, inputDataId))
           {
               dataIdIsValid = true;
           }

           res = db.insertSubAnalysisTotEarningsData(dateId, mainAnalysisId, inputDataId, dataIdIsValid,
                                                       data, dataId);
        }
    }
}



/******************************************************************
 *
 * Function:    on_pushButtonSaveEquityPerShare_clicked()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
void StockAnalysisTab::on_pushButtonSaveEquityPerShare_clicked()
{
    CDbHndl db;
    bool res;
    int mainAnalysisId;
    QString str;

    bool isValid;
    int dateId;
    int inputDataId;
    int dataId;
    bool dataIdIsValid = false;

    int nofData;
    QString date;
    QString data;


    if((m_stockName.length() < 1) || m_stockSymbol.length() < 1)
    {
        QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Välj aktie"));
        return;
    }


    str = (QString::fromUtf8("Vill du lägga till sub data?\n"));
    str = str + m_stockName;
    str = str + ", ";
    str = str + m_stockSymbol;
    str = str + ", ";
    str = str + m_analysisDate;


    if(QMessageBox::No == QMessageBox::question(this, QString::fromUtf8("Uppdatera databas"), str, QMessageBox::Yes|QMessageBox::No))
    {
        return;
    }

    // Check if this stocksymbol and stockname is already added, if not add it
    res = db.mainAnalysisDataExists(m_stockName, m_stockSymbol, mainAnalysisId);

    if(false == res)
    {
        res = db.insertMainAnalysisData(m_stockName, m_stockSymbol, mainAnalysisId);
    }


    nofData = ui->tableWidgetEquityPerShare->rowCount();

    for(int row = 0; row < nofData; row++)
    {
        if(NULL != ui->tableWidgetEquityPerShare->item(row, 0))
        {
            date = ui->tableWidgetEquityPerShare->item(row, 0)->text();
            data = ui->tableWidgetEquityPerShare->item(row, 1)->text();

            date.toInt(&isValid);
            if (false == isValid)
            {
                QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Year is not a number"));
                continue;
            }

            CUtil cu;
            cu.number2double(data, data);
            data.toDouble(&isValid);
            if (false == isValid)
            {
                QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Equity is not a number"));
                continue;
            }
        }
        else
        {
           break;
        }


        res = db.subAnalysisEquityPerShareDateExists(date, mainAnalysisId, dateId);

        // Exist anaysis date?
        if(false == res)
        {
            res = db.insertSubAnalysisEquityPerShareDate(date, mainAnalysisId, dateId);
        }


        if(true == res)
        {
           dataIdIsValid = false;

           if( true == db.getSubAnalysisEquityPerShareDataId(mainAnalysisId, dateId, inputDataId))
           {
               dataIdIsValid = true;
           }

           res = db.insertSubAnalysisEquityPerShareData(dateId, mainAnalysisId, inputDataId, dataIdIsValid,
                                                       data, dataId);
        }
    }
}


/******************************************************************
 *
 * Function:    on_pushButtonSaveEquityPerShare_clicked()
 *
 * Description:
 *
 *  i   = is the discount rate.
    FBV = Future Book Value = Present Book Value * (1+g)^n
    The future book value is
        then discounted at rate i (e.g. 10 year t-bill return)
        to find the present value of what the book value will grow to.

    If you input that into the formula you get:
    IV = DIV * [ (1 - (1/( (1+i)^n) ) )/i] + PBV*((1+g)/(1+i))^n

 *
 *
 *****************************************************************/
void StockAnalysisTab::on_pushButtonCalcIntrinsicValue_clicked()
{
    double intrinsicValue, tenYearNoteYield, yearlyInterestOnEquity, presentEquityPerShare, dividendPerYear;
    double nofYears = 10.0;
    double tmpDivRes;
    double tmpEquityRes;

    double powXData;
    double tmpPow1Res;
    double oneDivPow1;
    double oneMinusoneDivPow1;

    double powYData1;
    double powYData2;
    double powYData3;
    double tmpPow2Res;


    //intrinsicValue = pow(1.05, nofYears);
    //QString str1;
    //str1.sprintf("%.2f", intrinsicValue);
    //ui->lineEditIntrinsicValueResult->setText(str1);

    if(ui->lineEditTenYearNoteYield->text().isEmpty() == true)
    {
        QMessageBox::information(this, QString::fromUtf8("Data saknas"), QString::fromUtf8("Ange räntan för 10 års stadsobligation"));
        return;
    }

    if(ui->lineEditHistoricalInterestOnEquity->text().isEmpty() == true)
    {
        QMessageBox::information(this, QString::fromUtf8("Data saknas"), QString::fromUtf8("Ange förväntad ånge räntan på eget kapital"));
        return;
    }

    if(ui->lineEditCurrEquityPerShare->text().isEmpty() == true)
    {
        QMessageBox::information(this, QString::fromUtf8("Data saknas"), QString::fromUtf8("Ange senast värde på eget kapital/Aktie"));
        return;
    }

    if(ui->lineEditEstimateYearlyDividend->text().isEmpty() == true)
    {
        QMessageBox::information(this, QString::fromUtf8("Data saknas"), QString::fromUtf8("Ange uppskattad årlig utdelning/Aktie"));
        return;
    }

    tenYearNoteYield = ui->lineEditTenYearNoteYield->text().toDouble();
    tenYearNoteYield = tenYearNoteYield / 100.0;
    yearlyInterestOnEquity = ui->lineEditHistoricalInterestOnEquity->text().toDouble();
    yearlyInterestOnEquity = yearlyInterestOnEquity / 100.0;
    presentEquityPerShare = ui->lineEditCurrEquityPerShare->text().toDouble();
    dividendPerYear = ui->lineEditEstimateYearlyDividend->text().toDouble();

    // IV = C * [1-[1/(1+I)^n]]/I + M/(1+I)^n

    // IV = DIV * [ (1 - (1/( (1+i)^n) ) )/i] + PBV*((1+g)/(1+i))^n

    //              DIV         * [ (1   - (1   / (    (1  + i              )^n       ) ) )/ i]
    powXData = (1.0 + tenYearNoteYield);
    tmpPow1Res = pow(powXData,nofYears);
    oneDivPow1 = (1.0 / tmpPow1Res );
    oneMinusoneDivPow1 = (1.0 - oneDivPow1);
    tmpDivRes = oneMinusoneDivPow1 / tenYearNoteYield;
    tmpDivRes = dividendPerYear * tmpDivRes;


    powYData1 = (1.0 + yearlyInterestOnEquity);
    powYData2 = (1.0 + tenYearNoteYield);
    powYData3 = powYData1 / powYData2;

    QString str1;
    str1.sprintf("%.2f", powYData3);
    ui->lineEditIntrinsicValueResult->setText(str1);


    tmpPow2Res = pow(powYData3, nofYears);
    tmpEquityRes = presentEquityPerShare * tmpPow2Res;


    //                PBV                *((1   + g                     ) /    (1   + i               ))^n
    intrinsicValue = tmpDivRes + tmpEquityRes;
    QString str;
    str.sprintf("%.2f", intrinsicValue);
    ui->lineEditIntrinsicValueResult->setText(str);

}





/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *******************************************************************/
bool StockAnalysisTab::
calcLeastSqrtFit(SubAnalysDataST *dataArr,
                 int nofArrData,
                 double &k,
                 double &m,
                 double &minX,
                 double &maxX)
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




    if(nofArrData < 2)
    {
        return false;
    }

    for(int i = 0 ; i < nofArrData ; i++)
    {
        if(true == cu.number2double(dataArr[i].date, x) && true == cu.number2double(dataArr[i].data, y))
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
    {
        return false;
    }


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
void StockAnalysisTab::calcTotSubdataForIntrinsicValue(void)
{
    int i = 0;
    double diff = 0.0;
    double totDividend = 0.0;
    double totEarning = 0.0;
    bool tooMuchDebt = false;



    if(m_nofEquityPerShareData > 1)
    {
        for(i = 1; i < m_nofEquityPerShareData; i++)
        {
            diff += (m_equityPerShareArr[i].data.toDouble() - m_equityPerShareArr[i-1].data.toDouble());
        }
    }

    if(m_nofDividendArrData > 1)
    {
        for(i = 1; i < m_nofDividendArrData; i++)
        {
            totDividend += m_dividendDataArr[i].data.toDouble();
        }
    }

    if(m_nofEarningsArrData > 1)
    {
        for(i = 1; i < m_nofEarningsArrData; i++)
        {
            totEarning += m_earningsDataArr[i].data.toDouble();
        }
    }

    QString str;
    str.sprintf("%.2f", totEarning);
    ui->lineEditTotErningsPerShare->setText(str);

    str.sprintf("%.2f", diff);
    ui->lineEditTotEquityIncPerShare->setText(str);

    str.sprintf("%.2f", totDividend);
    ui->lineEditTotDividendPerShare->setText(str);


    if(m_nofEquityPerShareData > 1 && m_nofDividendArrData > 1 && m_nofEarningsArrData > 1)
    {
        QString str;
        if(totEarning > (diff + totDividend))
        {
            ui->labelWarningEps->setPalette(*m_blue_palette);
            str = str.sprintf("OK uttag av vinst OK %.2f > %.2f + %2.f", totEarning, diff, totDividend);
            ui->labelWarningEps->setText(str);
        }
        else
        {
            QString str1;
            ui->labelWarningEps->setPalette(*m_red_palette);
            str = QString::fromUtf8("Högt uttag av vinst: ");
            str1.sprintf("%.2f < %.2f + %.2f", totEarning, diff, totDividend);
            str += str1;
            ui->labelWarningEps->setText(str);
        }
    }


    if(m_nofTotalCurrentAssetsArrData == m_nofTotalLiabilitiesData && m_nofTotalLiabilitiesData > 1)
    {
        for(int i = 1; i < m_nofTotalLiabilitiesData; i++)
        {
            if((m_totalCurrentAssetsArr[i].data.toDouble() / m_totalLiabilitiesArr[i].data.toDouble()) < 1.0 )
            {
                tooMuchDebt = true;
                break;
            }
        }


        if(tooMuchDebt == true)
        {
            ui->labelWarningDebt->setPalette(*m_red_palette);
            str = QString::fromUtf8("Höga Skulder: Kortfrisktiga tillgångar/Tot skulder < 1");
            ui->labelWarningDebt->setText(str);
        }
        else
        {
            ui->labelWarningDebt->setPalette(*m_blue_palette);
            str = QString::fromUtf8("Skulder OK: Kortfrisktiga tillgångar/skulder >= 1");
            ui->labelWarningDebt->setText(str);
        }
    }
 }



/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *******************************************************************/
void StockAnalysisTab::on_pushButtonCalcYearlyIntrestRateOnEquity_clicked()
{
    CUtil cu;
    double k, m, minX, maxX, minY, maxY, growthRate = 0;
    double nofYears;
    QString trendLineGrowth;

    if(m_nofEquityPerShareData <  1)
    {
        return;
    }

    nofYears = (double) (m_nofEquityPerShareData -1);

    calcLeastSqrtFit(m_equityPerShareArr, m_nofEquityPerShareData, k, m, minX, maxX);


    minY = k * minX + m;
    maxY = k * maxX + m;

    double x;
    double y;
    SubAnalysDataST pdataArr[2000];

    x = minX;
    for(int cnt = 0; cnt < m_nofEquityPerShareData + 10; cnt++)
    {
        y = k * x + m;
        pdataArr[cnt].date.sprintf("%.2f", x);
        pdataArr[cnt].data.sprintf("%.2f", y);
        x = x + 1;
    }

   plotEquityPerShareData(pdataArr, (m_nofEquityPerShareData + 5), true);


    if(true == cu.annualGrowthRate(minY, maxY, (nofYears), growthRate))
    {
        growthRate = growthRate - 1;
        growthRate = growthRate * 100;
        qDebug() << growthRate;


        trendLineGrowth.sprintf("%.2f", growthRate);
        ui->lineEditHistoricalInterestOnEquity->setText(trendLineGrowth);
    }
    else
    {
        trendLineGrowth.sprintf("  ");
        ui->lineEditHistoricalInterestOnEquity->setText(trendLineGrowth);
    }
}





/*******************************************************************
 *
 * Function:
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void StockAnalysisTab::plotEquityPerShareData(SubAnalysDataST *dataArr, int nofData, bool plotPrediction)
{
    CYahooStockPlotUtil cyspu;
    int index = 0;

    if(plotPrediction == true)
    {
        index = 1;
    }
    else
    {
        m_qwtEquityPlotData.stock[1].data.detach();
        m_qwtEquityPlotData.stock[1].data.setData(NULL);
    }

    m_qwtEquityPlotData.stock[index].data.detach();
    m_qwtEquityPlotData.stock[index].data.setData(NULL);

    cyspu.removePlotData(m_qwtEquityPlotData, index, ui->qwtPlot_10);

    m_qwtEquityPlotData.axis.minMaxIsInit = false;

    for(int i = 0; i < nofData; i++)
    {
        m_x[i] = dataArr[i].date.toDouble();
        m_y[i] = dataArr[i].data.toDouble();
        cyspu.updateMinMaxAxis(m_qwtEquityPlotData.axis, m_x[i], m_y[i]);
    }

    m_qwtEquityPlotData.stock[index].data.setSamples(m_x, m_y, nofData);
    ui->qwtPlot_10->setAxisMaxMinor(QwtPlot::xBottom, 2);
    ui->qwtPlot_10->setAxisMaxMajor(QwtPlot::xBottom, nofData - 1);

    if(index == 1)
    {
        m_qwtEquityPlotData.stock[index].data.setPen(QPen(Qt::blue, 2));
        m_qwtEquityPlotData.stock[index].data.setSymbol( new QwtSymbol(QwtSymbol::Ellipse,                                                            Qt::blue,
                                                                     QPen( Qt::blue ),
                                                                      QSize( 7, 7 ) ) );
    }
    else
    {
        m_qwtEquityPlotData.stock[index].data.setPen(QPen(Qt::black, 2));
        m_qwtEquityPlotData.stock[index].data.setSymbol( new QwtSymbol(QwtSymbol::Ellipse,                                                            Qt::blue,
                                                                     QPen( Qt::black ),
                                                                      QSize( 7, 7 ) ) );
    }

    cyspu.plotData(m_qwtEquityPlotData, ui->qwtPlot_10, index, true);
    m_qwtEquityPlotData.stock[index].data.attach(ui->qwtPlot_10);
    ui->qwtPlot_10->replot();
}



/*******************************************************************
 *
 * Function:        clearGUIIntrinsicValue()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void StockAnalysisTab::initAllAnalysisPlots(void)
{
    QString plotHeader;
    QString legendText;
    QColor canvasColor = Qt::white;
    QwtSymbol legendSymbol = QwtSymbol::Rect;
    QColor legendColor = Qt::blue;
    QwtPlot::LegendPosition location = QwtPlot::BottomLegend; //QwtPlot::TopLegend;
    int legendSize = 10;


    //===================================================================================
    // Trading company industrial companies
    //===================================================================================

    //-----------------------------------------------------------------------------------
    // Omsättningstillgångarna / Kortfristiga skulder > 2
    //-----------------------------------------------------------------------------------

    plotHeader = QString::fromUtf8("Omsättningstillgångarna / Kortfristiga skulder > 2");
    legendText = QString::fromUtf8("OT/KS");

    initPlotLinearReportData(ui->qwtPlotCurrAssLiab, plotHeader, canvasColor, legendText, legendSymbol, legendColor,
                             location, legendSize);

    //-----------------------------------------------------------------------------------
    // Omsättningstillgångarna / Totala skulder >= 1
    //-----------------------------------------------------------------------------------
    plotHeader = QString::fromUtf8("Omsättningstillgångarna / Totala skulder >= 1");
    legendText = QString::fromUtf8("OT/TS");
    legendSymbol = QwtSymbol::Ellipse;

    initPlotLinearReportData(ui->qwtPlotCurrAssTotLiab_12, plotHeader, canvasColor, legendText, legendSymbol, legendColor,
                             location, legendSize);


    //-----------------------------------------------------------------------------------
    // Vinst/Aktie
    //-----------------------------------------------------------------------------------
    plotHeader = QString::fromUtf8("Vinst/Aktie");
    legendText = QString::fromUtf8("V/A");

    initPlotLinearReportData(ui->qwtPlotEarningsPerShare_13, plotHeader, canvasColor, legendText, legendSymbol, legendColor,
                             location, legendSize);

    //-----------------------------------------------------------------------------------
    // Utdelning
    //-----------------------------------------------------------------------------------
    plotHeader = QString::fromUtf8("Utdelning");
    legendText = QString::fromUtf8("UT");

    initPlotLinearReportData(ui->qwtPlotDiv_14, plotHeader, canvasColor, legendText, legendSymbol, legendColor,
                             location, legendSize);




    //-----------------------------------------------------------------------------------
    // Vinst/Utdelning
    //-----------------------------------------------------------------------------------
    plotHeader = QString::fromUtf8("Vinst/Utdelning");
    legendText = QString::fromUtf8("V/UT");

    initPlotLinearReportData(ui->qwtPlotEarningDiv_16, plotHeader, canvasColor, legendText, legendSymbol, legendColor,
                             location, legendSize);



    //-----------------------------------------------------------------------------------
    // Vinsttillväxt (%)
    //-----------------------------------------------------------------------------------

    plotHeader = QString::fromUtf8("Vinsttillväxt (%)");
    legendText = QString::fromUtf8("Vt");

    initPlotLinearReportData(ui->qwtPlot_19, plotHeader, canvasColor, legendText, legendSymbol, legendColor,
                             location, legendSize);



    //-----------------------------------------------------------------------------------
    // Vinstmarginal (%) (Vinst efter skatt / Omsättning)
    //-----------------------------------------------------------------------------------

    plotHeader = QString::fromUtf8("Vinstmarginal (%) (Vinst efter skatt / Omsättning)");
    legendText = QString::fromUtf8("Vm");

    initPlotLinearReportData(ui->qwtPlotEarningMargin_20, plotHeader, canvasColor, legendText, legendSymbol, legendColor,
                             location, legendSize);



    //-----------------------------------------------------------------------------------
    // Avkastning på det egna kapitalet (%) [Vinst/Eget kapital]
    //-----------------------------------------------------------------------------------
    plotHeader = QString::fromUtf8("Avkastning på det egna kapitalet (%) [Vinst/Eget kapital]");
    legendText = QString::fromUtf8("Ak/Ek");

    initPlotLinearReportData(ui->qwtPlotEquityMargin_22, plotHeader, canvasColor, legendText, legendSymbol, legendColor,
                             location, legendSize);
}






/*******************************************************************
 *
 * Function:        clearGUIIntrinsicValue()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void StockAnalysisTab::displayAllAnalysisPlots(void)
{
    //SubAnalysDataST tmpArr[MAX_NOF_TOTAL_CURRENT_ASSETS_ARR_DATA];
    SubAnalysDataST resultArr[MAX_NOF_TOTAL_CURRENT_ASSETS_ARR_DATA];
    int nofDataResultArr;
    //int nofTmpArrData;
    int indexToPlot;
    int nofPlotToClear;
    QColor lineColor;
    bool useAutoScale;
    bool skipDenominatorEqZero = true;
    bool convToProcent;

    CYahooStockPlotUtil cyspu;
    m_qwtAllAnalysisPlotData.nofStocksToPlot = CYahooStockPlotUtil::MAX_NOF_PLOT_COLORS;
    cyspu.emtypPlotData(m_qwtAllAnalysisPlotData, false);



    //===================================================================================
    // Trading company industrial companies
    //===================================================================================

    //-----------------------------------------------------------------------------------
    // Omsättningstillgångarna / Kortfristiga skulder > 2
    //-----------------------------------------------------------------------------------
    // Beräkna kvoten
    subAnalysisCalcQuotient(resultArr,
                            nofDataResultArr,
                            m_totalCurrentAssetsArr,
                            m_nofTotalCurrentAssetsArrData,
                            m_totalCurrentLiabilitiesArr,
                            m_nofTotalCurrentLiabilitiesData);


    indexToPlot = 0;
    nofPlotToClear = 0;
    lineColor = Qt::blue;
    useAutoScale = false;
    plotLinearReportData(ui->qwtPlotCurrAssLiab,
                          useAutoScale,
                          resultArr,
                          nofDataResultArr,
                          indexToPlot,
                          nofPlotToClear,
                          lineColor);

    //-----------------------------------------------------------------------------------
    // Omsättningstillgångarna / Totala skulder >= 1
    //-----------------------------------------------------------------------------------
    // Beräkna kvoten
    subAnalysisCalcQuotient(resultArr,
                            nofDataResultArr,
                            m_totalCurrentAssetsArr,
                            m_nofTotalCurrentAssetsArrData,
                            m_totalLiabilitiesArr,
                            m_nofTotalLiabilitiesData);


    indexToPlot = 1;
    nofPlotToClear = 0;
    lineColor = Qt::blue;
    useAutoScale = false;
    plotLinearReportData(ui->qwtPlotCurrAssTotLiab_12,
                          useAutoScale,
                          resultArr,
                          nofDataResultArr,
                          indexToPlot,
                          nofPlotToClear,
                          lineColor);


    //-----------------------------------------------------------------------------------
    // Vinst/Aktie
    //-----------------------------------------------------------------------------------
    indexToPlot = 2;
    nofPlotToClear = 0;
    lineColor = Qt::blue;
    useAutoScale = false;
    plotLinearReportData(ui->qwtPlotEarningsPerShare_13,
                          useAutoScale,
                          m_earningsDataArr,
                          m_nofEarningsArrData,
                          indexToPlot,
                          nofPlotToClear,
                          lineColor);



    //-----------------------------------------------------------------------------------
    // Utdelning
    //-----------------------------------------------------------------------------------
    indexToPlot = 3;
    nofPlotToClear = 0;
    lineColor = Qt::blue;
    useAutoScale = false;

    plotBarGraphReportData(ui->qwtPlotDiv_14,
                                useAutoScale,
                                m_dividendDataArr,
                                m_nofDividendArrData,
                                indexToPlot,
                                nofPlotToClear,
                                lineColor);


    //-----------------------------------------------------------------------------------
    // Vinst/Utdelning
    //-----------------------------------------------------------------------------------

    // Beräkna kvoten
    subAnalysisCalcQuotient(resultArr,
                            nofDataResultArr,
                            m_earningsDataArr,
                            m_nofEarningsArrData,
                            m_dividendDataArr,
                            m_nofDividendArrData);

    indexToPlot = 4;
    nofPlotToClear = 0;
    lineColor = Qt::blue;
    useAutoScale = false;
    skipDenominatorEqZero = true;
    plotLinearReportData(ui->qwtPlotEarningDiv_16,
                          useAutoScale,
                          resultArr,
                          nofDataResultArr,
                          indexToPlot,
                          nofPlotToClear,
                          lineColor);


    //-----------------------------------------------------------------------------------
    // Vinsttillväxt (%)
    //-----------------------------------------------------------------------------------
    subAnalysisDisplayGraphData sadgd;
    sadgd.subAnalysisOneArrCalcProcentRationPrevCurrSlot(m_earningsDataArr,
                                                  m_nofEarningsArrData,
                                                  resultArr,
                                                  nofDataResultArr,
                                                  skipDenominatorEqZero);

    indexToPlot = 5;
    nofPlotToClear = 0;
    lineColor = Qt::blue;
    useAutoScale = false;

    plotBarGraphReportData(ui->qwtPlot_19,
                                useAutoScale,
                                resultArr,
                                nofDataResultArr,
                                indexToPlot,
                                nofPlotToClear,
                                lineColor);


    //-----------------------------------------------------------------------------------
    // Vinstmarginal (%) (Vinst efter skatt / Omsättning)
    //-----------------------------------------------------------------------------------
    // Beräkna kvoten
    skipDenominatorEqZero = true;
    convToProcent = true;

    subAnalysisCalcQuotient(resultArr,
                            nofDataResultArr,
                            m_totEarningsDataArr,
                            m_nofTotEarningsArrData,
                            m_revenueDataArr,
                            m_nofRevenueArrData,
                            skipDenominatorEqZero,
                            convToProcent);



    indexToPlot = 7;
    nofPlotToClear = 0;
    lineColor = Qt::blue;
    useAutoScale = false;
    plotLinearReportData(ui->qwtPlotEarningMargin_20,
                          useAutoScale,
                          resultArr,
                          nofDataResultArr,
                          indexToPlot,
                          nofPlotToClear,
                          lineColor);

    //-----------------------------------------------------------------------------------
    // Avkastning på det egna kapitalet (%) [Vinst/Eget kapital]
    //-----------------------------------------------------------------------------------
    // Beräkna kvoten

    skipDenominatorEqZero = true;
    convToProcent = true;

    subAnalysisCalcQuotient(resultArr,
                            nofDataResultArr,
                            m_totEarningsDataArr,
                            m_nofTotEarningsArrData,
                            m_totEquityArr,
                            m_nofTotEquityData,
                            skipDenominatorEqZero,
                            convToProcent);

    indexToPlot = 8;
    nofPlotToClear = 0;
    lineColor = Qt::blue;
    useAutoScale = false;
    plotLinearReportData(ui->qwtPlotEquityMargin_22,
                          useAutoScale,
                          resultArr,
                          nofDataResultArr,
                          indexToPlot,
                          nofPlotToClear,
                          lineColor);

}




/*******************************************************************
 *
 * Function:        clearGUIIntrinsicValue()
 *
 * Description:
 *              Numerator =     täljare
 *              Denominator =   nämnare
 *
 *******************************************************************/
void StockAnalysisTab::subAnalysisCalcQuotient(SubAnalysDataST *resultArr,     int &nofDataResultArr,
                                                SubAnalysDataST *numeratorArr, int nofDataNumeratorArr,
                                               SubAnalysDataST *denominatorArr, int nofDataDenominatorArr,
                                               bool skipDenominatorEqZero,
                                               bool convToProcent)
{
    double tmpRes;
    double procent = 100.0;
    double result;

    nofDataResultArr = 0;
    for(int i = 0; i < nofDataNumeratorArr; i++)
    {
        for(int j = 0; j < nofDataDenominatorArr; j++)
        {
            if((skipDenominatorEqZero == true) && (denominatorArr[j].data.toDouble() == 0))
            {
                continue;
            }

            if(numeratorArr[i].date.toInt() == denominatorArr[j].date.toInt())
            {
                resultArr[nofDataResultArr].date = numeratorArr[i].date;

                // Do not divide by zero
                if((denominatorArr[j].data.toDouble() >= 0) &&
                   (denominatorArr[j].data.toDouble() <  0.0001))
                {
                    tmpRes = numeratorArr[i].data.toDouble() / 0.0001;
                }
                else
                {
                    tmpRes = (numeratorArr[i].data.toDouble() / denominatorArr[j].data.toDouble());

                }

                result =tmpRes;
                if(convToProcent == true)
                {
                    result = tmpRes * procent;
                }
                resultArr[nofDataResultArr].data.sprintf("%f", result);
                nofDataResultArr++;
            }
        }
    }
}





/*******************************************************************
 *
 * Function:        clearGUIIntrinsicValue()
 *
 * Description:
 *
 * pos = QwtPlot::TopLegend
 *       QwtPlot::BottomLegend
 *       QwtPlot::RightLegend
 *
 *
 *******************************************************************/
void StockAnalysisTab::initPlotLinearReportData(QwtPlot *qwtPlot,
                                                QString plotHeader,
                                                QColor canvasColor,
                                                QString legendText,
                                                QwtSymbol legendSymbol,
                                                QColor legendColor,
                                                QwtPlot::LegendPosition location,
                                                int legendSize)
{
    CExtendedQwtPlot eqp;

    eqp.setPlotTitle(qwtPlot, plotHeader);
    eqp.setCanvasBackground(qwtPlot, canvasColor);

    eqp.setLegendSymbol(qwtPlot, legendText, legendSymbol, legendColor, legendSize);
    eqp.setLegend(qwtPlot, location);
}


/*******************************************************************
 *
 * Function:        plotLinearReportData()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void StockAnalysisTab::plotBarGraphReportData(QwtPlot *qwtPlot,
                                            bool useAutoScale,
                                            SubAnalysDataST *dataArr,
                                            int nofData,
                                            int indexToPlot,
                                            int nofPlotToClear,
                                            QColor lineColor)
{
    CYahooStockPlotUtil cyspu;

    // Do not excide array bondary
    if((indexToPlot >= CYahooStockPlotUtil::MAX_NOF_PLOT_COLORS) ||
       (nofPlotToClear >= CYahooStockPlotUtil::MAX_NOF_PLOT_COLORS))
    {
        return;
    }


    for(int i = 0; i < nofPlotToClear; i++)
    {
        cyspu.removePlotData(m_qwtAllAnalysisPlotData, i, qwtPlot);
    }


    m_qwtAllAnalysisPlotData.nofStocksToPlot =
    m_qwtAllAnalysisPlotData.axis.minMaxIsInit = false;

    for(int i = 0; i < nofData; i++)
    {
        m_x[i] = dataArr[i].date.toDouble();
        m_y[i] = dataArr[i].data.toDouble();
        cyspu.updateMinMaxAxis(m_qwtAllAnalysisPlotData.axis, m_x[i], m_y[i]);
    }

    // Make bars fit on plot
    m_qwtAllAnalysisPlotData.axis.minX -= 0.5;
    m_qwtAllAnalysisPlotData.axis.maxX += 0.5;


    if((useAutoScale == false) && (m_qwtAllAnalysisPlotData.axis.minY > 0))
    {
        m_qwtAllAnalysisPlotData.axis.minY = 0;
    }

    // Data to be plotted
    m_qwtAllAnalysisPlotData.stock[indexToPlot].data.setSamples(m_x, m_y, nofData);

    // set plot grid
    qwtPlot->setAxisMaxMinor(QwtPlot::xBottom, 1);
    qwtPlot->setAxisMaxMajor(QwtPlot::xBottom, nofData);


    // Set plot to bar graph
    m_qwtAllAnalysisPlotData.stock[indexToPlot].data.setStyle(QwtPlotCurve::Sticks);


    // Importent to use Qt::FlatCap together with QwtPlotCurve::Sticks if width
    // is larger then 4 otherwise you get wrong y - value
    QPen pen;
    pen.setWidth(20);
    pen.setBrush(lineColor);
    pen.setCapStyle(Qt::FlatCap);
    pen.setJoinStyle(Qt::MiterJoin);
    pen.setStyle(Qt::SolidLine);

    m_qwtAllAnalysisPlotData.stock[indexToPlot].data.setPen(pen);
    m_qwtAllAnalysisPlotData.stock[indexToPlot].data.setBaseline(0);


    cyspu.plotData(m_qwtAllAnalysisPlotData, qwtPlot, indexToPlot, useAutoScale);

    // Disable legend when we plot to not get two
    m_qwtAllAnalysisPlotData.stock[indexToPlot].data.setItemAttribute(QwtPlotItem::Legend, false);
    m_qwtAllAnalysisPlotData.stock[indexToPlot].data.attach(qwtPlot);
    qwtPlot->replot();
}




/*******************************************************************
 *
 * Function:        plotLinearReportData()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void StockAnalysisTab::plotLinearReportData(QwtPlot *qwtPlot,
                                            bool useAutoScale,
                                            SubAnalysDataST *dataArr,
                                            int nofData,
                                            int indexToPlot,
                                            int nofPlotToClear,
                                            QColor lineColor)
{
    CYahooStockPlotUtil cyspu;

    // Do not excide array bondary
    if((indexToPlot >= CYahooStockPlotUtil::MAX_NOF_PLOT_COLORS) ||
       (nofPlotToClear >= CYahooStockPlotUtil::MAX_NOF_PLOT_COLORS))
    {
        return;
    }


    for(int i = 0; i < nofPlotToClear; i++)
    {
        cyspu.removePlotData(m_qwtAllAnalysisPlotData, i, qwtPlot);
    }


    m_qwtAllAnalysisPlotData.nofStocksToPlot =
    m_qwtAllAnalysisPlotData.axis.minMaxIsInit = false;

    for(int i = 0; i < nofData; i++)
    {
        m_x[i] = dataArr[i].date.toDouble();
        m_y[i] = dataArr[i].data.toDouble();
        cyspu.updateMinMaxAxis(m_qwtAllAnalysisPlotData.axis, m_x[i], m_y[i]);
    }


    if((useAutoScale == false) && (m_qwtAllAnalysisPlotData.axis.minY > 0))
    {
        m_qwtAllAnalysisPlotData.axis.minY = 0;
    }

    m_qwtAllAnalysisPlotData.stock[indexToPlot].data.setSamples(m_x, m_y, nofData);
    qwtPlot->setAxisMaxMinor(QwtPlot::xBottom, 2);
    qwtPlot->setAxisMaxMajor(QwtPlot::xBottom, nofData);

    qwtPlot->setAxisMaxMinor(QwtPlot::yLeft, 10);


    m_qwtAllAnalysisPlotData.stock[indexToPlot].data.setPen(QPen(lineColor, 2));
    m_qwtAllAnalysisPlotData.stock[indexToPlot].data.setSymbol(new QwtSymbol(QwtSymbol::Ellipse,                                                            Qt::blue,
                                                               QPen(lineColor),
                                                               QSize(7, 7) ) );


    cyspu.plotData(m_qwtAllAnalysisPlotData, qwtPlot, indexToPlot, useAutoScale);

    // Dirty way to acutoscale y axis
    //qwtPlot->setAxisAutoScale(QwtPlot::yLeft, true);

    // Disable legend when we plot to not get two
    m_qwtAllAnalysisPlotData.stock[indexToPlot].data.setItemAttribute(QwtPlotItem::Legend, false);
    m_qwtAllAnalysisPlotData.stock[indexToPlot].data.attach(qwtPlot);
    qwtPlot->replot();
}



#if 0

/*!
  \brief build a logarithmic scale
*/
00300 void QwtAutoScale::buildLogScale ()
{
    if (!d_autoScale)
        return;

    double minval = d_minValue; // the calculation of scale divisions
    double maxval = d_maxValue; // is based on the input data.

    if (d_loMargin > 0.0)
        minval /= pow(10.0, d_loMargin);
    if (d_hiMargin > 0.0)
        maxval *= pow(10.0, d_hiMargin);

    if (d_scaleOpt & Symmetric)
    {
        const double delta = qwtMax(maxval / d_lref,  d_lref / minval);
        maxval = d_lref * delta;
        minval = d_lref / delta;
    }
    else if (d_scaleOpt & IncludeRef)
    {
        if (maxval < d_lref)
            maxval = d_lref;
        else if (minval > d_lref)
            minval = d_lref;
    }

    const double ticks = (d_maxMajor > 0) ? double(d_maxMajor) : 1;

    setRange(minval, maxval);

    // decades included in the interval
    const double decades = qwtAbs(log10 (d_scaleMax / d_scaleMin));

    // calculate step size in decades

    double step;
    if ((decades > 1.0) && (decades > ticks))
    {
        double ipart;
        // One interval contains more than one decade.
        // The number of decades in an interval is adjusted
        // to be a multiple of 2,3,5, or 10.
        double fpart = modf (log10 (ceil (decades * 0.999999 / ticks)), &ipart);
        if (fpart < MinEps)
           fpart = 1.0;
        else if ((fpart - LOG10_2) < MinEps)
           fpart = 2.0;
        else if ((fpart - LOG10_3) < MinEps)
           fpart = 3.0;
        else if ((fpart - LOG10_5) < MinEps)
           fpart = 5.0;
        else
           fpart = 10.0;

        step = pow (10.0, ipart) * fpart;

    }
    else                // The minimal step size is one decade.
    {
        step = 1.0;
    }

    if (!(d_scaleOpt & Floating))
    {
        d_scaleMin = pow (10.0, step *
            floor ((log10(d_scaleMin) + MinEps * step) / step));
        d_scaleMax = pow (10.0, step *
            ceil ((log10(d_scaleMax) - MinEps * step) / step));
    }

    if (d_scaleOpt & Inverted)
    {
        step = -step;
        d_scldiv.rebuild(d_scaleMax, d_scaleMin, d_maxMajor, d_maxMinor, TRUE,
             step, FALSE);
    }
    else
    {
        d_scldiv.rebuild(d_scaleMin, d_scaleMax, d_maxMajor, d_maxMinor,
             TRUE, step, TRUE);
    }
}



#endif




/*******************************************************************
 *
 * Function:        clearGUIIntrinsicValue()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void StockAnalysisTab::savePlotLinearReportData(QwtPlot *qwtPlot,
                                                QString stockName,
                                                QString date)
{

#if 0
    CExtendedQwtPlot();
    void setPlotTitle(QwtPlot *qwtPlot, QString title);
    void setXAxisTitle(QwtPlot *qwtPlot, QString title, int fontSize=10);
    void setYAxisTitle(QwtPlot *qwtPlot, QString title);
    void setXAxisFontSize(QwtPlot *qwtPlot, int fontSize);
    void setYAxisFontSize(QwtPlot *qwtPlot, int fontSize);
    void setXAxisScale(QwtPlot *qwtPlot, double min, double max);
    void setYAxisScale(QwtPlot *qwtPlot, double min, double max);
#endif

}



#if 0
/*******************************************************************
 *
 * Function:        clearGUIIntrinsicValue()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void StockAnalysisTab::plotLogarithmicReportData()
{

}
#endif

/*******************************************************************
 *
 * Function:        clearGUIIntrinsicValue()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void StockAnalysisTab::clearGUIIntrinsicValue(void)
{
    CYahooStockPlotUtil cyspu;


    ui->labelWarningEps->clear();
    ui->labelWarningDebt->clear();

    ui->lineEditTotErningsPerShare->clear();
    ui->lineEditTotEquityIncPerShare->clear();
    ui->lineEditTotDividendPerShare->clear();


    ui->lineEditHistoricalInterestOnEquity->clear();
    ui->lineEditCurrEquityPerShare->clear();
    ui->lineEditEstimateYearlyDividend->clear();
    ui->lineEditIntrinsicValueResult->clear();

    ui->lineEditIntrinsicValueResult->clear();
    for(int i = 0; i < 2; i++)
        cyspu.removePlotData(m_qwtEquityPlotData, i, ui->qwtPlot_10);
}


/*******************************************************************
 *
 * Function:
 *
 * Description: Not used
 *
 *
 *
 *******************************************************************/
void StockAnalysisTab::getminMaxLogScale(double minIn, double &minOut, double maxIn, double &maxOut)
{

    if(minIn < 0)
    {
        minOut = 1;
    }

    if(maxIn < 0)
    {
        maxOut = 1;
    }


    double maxTenPotenseData = 0.00002;
    double minTenPotenseData = 1000000000000;

    for(int i = 0; i < 20; i++)
    {
        if(minTenPotenseData > minIn)
        {
            minTenPotenseData = minTenPotenseData / 10;
        }
        else
        {
            minOut = minTenPotenseData;
        }


        if(maxTenPotenseData < maxIn)
        {
            maxTenPotenseData = maxTenPotenseData * 10;
        }
        else
        {
            maxOut = maxTenPotenseData;
        }
    }
}


/*******************************************************************
 *
 * Function:    on_pushSaveIValueDividend_clicked()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void StockAnalysisTab::on_pushSaveIValueDividend_clicked()
{
    CDbHndl db;
    bool res;
    int mainAnalysisId;
    QString str;

    bool isValid;
    int dividendDateId;
    int inputDividendDataId;
    int dividendDataId;
    bool dividendDataIdIsValid = false;

    int nofData;
    QString dividendDate;
    QString dividendData;

    if((m_stockName.length() < 1) || m_stockSymbol.length() < 1)
    {
        QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Välj aktie"));
        return;
    }


    str = (QString::fromUtf8("Vill du lägga till sub data?\n"));
    str = str + m_stockName;
    str = str + ", ";
    str = str + m_stockSymbol;
    str = str + ", ";
    str = str + m_analysisDate;





    if(QMessageBox::No == QMessageBox::question(this, QString::fromUtf8("Uppdatera databas"), str, QMessageBox::Yes|QMessageBox::No))
    {
        return;
    }

    // Check if this stocksymbol and stockname is already added, if not add it
    res = db.mainAnalysisDataExists(m_stockName,
                                m_stockSymbol,
                                mainAnalysisId);
    if(false == res)
    {
        res = db.insertMainAnalysisData(m_stockName,
                                     m_stockSymbol,
                                     mainAnalysisId);
    }



    nofData = ui->tableWidgetIntrinsicValueDividendPerShare->rowCount();

    for(int row = 0; row < nofData; row++)
    {
       if(NULL != ui->tableWidgetIntrinsicValueDividendPerShare->item(row, 0))
       {
            dividendDate = ui->tableWidgetIntrinsicValueDividendPerShare->item(row, 0)->text();
            dividendData = ui->tableWidgetIntrinsicValueDividendPerShare->item(row, 1)->text();

            dividendDate.toInt(&isValid);
            if (false == isValid)
            {
                QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Year is not a number"));
                continue;
            }

            CUtil cu;
            cu.number2double(dividendData, dividendData);
            dividendData.toDouble(&isValid);
            if (false == isValid)
            {
                QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Dividend is not a number"));
                continue;
            }
       }
       else
       {
           break;
       }


        res = db.subAnalysisDividendDateExists(dividendDate,
                                                   mainAnalysisId,
                                                   dividendDateId);

        // Exist anaysis date?
        if(false == res)
        {
            res = db.insertSubAnalysisDividendDate(dividendDate,
                                                   mainAnalysisId,
                                                   dividendDateId);
        }


        if(true == res)
        {
           dividendDataIdIsValid = false;

           if( true == db.getSubAnalysisDividendId(mainAnalysisId, dividendDateId, inputDividendDataId))
           {
               dividendDataIdIsValid = true;
           }

            res = db.insertSubAnalysisDividend(dividendDateId,
                                               mainAnalysisId,
                                               inputDividendDataId,
                                               dividendDataIdIsValid,
                                               dividendData,
                                               dividendDataId);
        }
    }
}


/*******************************************************************
 *
 * Function:    on_pushButtonAltCalcAvgAnnualGrowthRateEquity_clicked()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void StockAnalysisTab::on_pushButtonAltCalcAvgAnnualGrowthRateEquity_clicked()
{

}

/*******************************************************************
 *
 * Function:    on_pushButtonAltcalcAvgAnnualGrowthRateEquity_clicked()
 *
 * Description: Open a dialog so user is able to adjust and
 *              calc Avg Annual Growth Rate of Equity
 *
 *
 *******************************************************************/
void StockAnalysisTab::on_pushButtonAltcalcAvgAnnualGrowthRateEquity_clicked()
{

    QString nofYears;






    if(m_nofEquityPerShareData >  2)
    {
        nofYears.sprintf("%d", (m_nofEquityPerShareData -1));
        calcAvgAnnualGrowthRateEquity_dlg.initDlg(m_equityPerShareArr[0].data,
                                                  m_equityPerShareArr[(m_nofEquityPerShareData -1)].data,
                                                  nofYears,
                                                  m_equityPerShareArr,
                                                  m_nofEquityPerShareData);
    }

    calcAvgAnnualGrowthRateEquity_dlg.show();
}




void StockAnalysisTab::on_pushButtonSaveImg_2_clicked()
{

    QString filename = "img_test1.png";

    QPixmap qPix = QPixmap::grabWidget(ui->qwtPlotCurrAssLiab);

    if(qPix.isNull())
    {
        qDebug("Failed to capture the plot for saving");
        return;
    }

   qPix.save(filename, "png");
}

