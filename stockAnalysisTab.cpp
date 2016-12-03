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
    ui->setupUi(this);

    m_stockNameIsInit = false;

    initSubAnalysTables();

    m_gfc.initStockList1(ui->treeWidgetStockListAnalysis);
    m_gfc.addAllStockListsToCombobox(ui->StockListComboBoxAnalysis);

    m_gfc.initStockAnalysisDateList(ui->treeWidgetAnalysisDate);

    on_SelStockListButton_clicked();

    // Show html file that describes investment strategy
    ui->webView_2->load(QUrl("qrc:/database/AnalysDoc/AnalysDoc/Investeringskriterier.html"));

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

    // Dividend
    dataHeader = QString::fromUtf8("Utdel/Aktie");
    initSubAnalyseTableWidget(ui->tableWidgetDividend, dateHeader, dataHeader);

    // Earnings
    dataHeader = QString::fromUtf8("Vinst/Aktie");
    initSubAnalyseTableWidget(ui->tableWidgetEarnings, dateHeader, dataHeader);

    // TotalCurrentAssets
    dataHeader = QString::fromUtf8("Oms.tillg");
    initSubAnalyseTableWidget(ui->tableWidgetTotalCurrentAsset, dateHeader, dataHeader);

    // totalCurrentLiabilities
    dataHeader = QString::fromUtf8("Kortf.skuld");
    initSubAnalyseTableWidget(ui->tableWidgetTotalCurrentLiabilities, dateHeader, dataHeader);

    // totalLiabilities
    dataHeader = QString::fromUtf8("Totala Skuld");
    initSubAnalyseTableWidget(ui->tableWidgetTotalLiabilities, dateHeader, dataHeader);

    // Solidity
    dataHeader = QString::fromUtf8("Soliditet (%)");
    initSubAnalyseTableWidget(ui->tableWidgetSolidity, dateHeader, dataHeader);

    // Coverage Ratio (Räntetäckningsgrad)
    dataHeader = QString::fromUtf8("RänteTG ggr");
    initSubAnalyseTableWidget(ui->tableWidgetCoverageRatio, dateHeader, dataHeader);

    // Core Tier1 Ratio (Primärkapitalrelation)
    dataHeader = QString::fromUtf8("primärkap.rel");
    initSubAnalyseTableWidget(ui->tableWidgetCoreTier1Ratio, dateHeader, dataHeader);

    // CoreCapitalRatio (Kärnprimärkapitalrelation)
    dataHeader = QString::fromUtf8("kärnprimärkap.rel");
    initSubAnalyseTableWidget(ui->tableWidgetCoreCapitalRatio, dateHeader, dataHeader);

    // CoreCapitalRatio (Kärnprimärkapitalrelation)
    dataHeader = QString::fromUtf8("Eget kapital");
    initSubAnalyseTableWidget(ui->tableWidgetEquity, dateHeader, dataHeader);

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

    updateTableWithSubAnalysData(ui->tableWidgetDividend,
                                 SAD_DIVIDEND,
                                 m_dividendDataArr,
                                 m_nofDividendArrData);

    updateTableWithSubAnalysData(ui->tableWidgetEarnings,
                                 SAD_EARNINGS,
                                 m_earningsDataArr,
                                 m_nofEarningsArrData);

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
                                 SAD_CORE_EQUITY,
                                 m_equityArr,
                                 m_nofEquityData);





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
    case SAD_DIVIDEND:
        res = db.getSubAnalysisDividendData(m_stockName, m_stockSymbol, subAnalysDataArr, nofArrData);
        if(nofArrData > MAX_NOF_DIVIDEND_ARR_DATA)
        {
            QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Error 1: Too many array data"));
        }
        break;

    case SAD_EARNINGS:
        res = db.getSubAnalysisEarningsData(m_stockName, m_stockSymbol, subAnalysDataArr, nofArrData);
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

   case SAD_CORE_EQUITY:
        res = db.getSubAnalysisEquityData(m_stockName, m_stockSymbol, subAnalysDataArr, nofArrData);
        if(nofArrData > MAX_NOF_CORE_CAPITAL_RATIO)
        {
            QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Error 8: Too many array data"));
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
                           analysisDataId);
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
                         hSAPData.otherInformation);


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

    hSAPData.equityArr = m_equityArr;
    hSAPData.nofEquityData = m_nofEquityData;

    hSAPData.coreTier1RatioArr = m_coreTier1RatioArr;
    hSAPData.nofCoreTier1RatioData = m_nofCoreTier1RatioData;

    hSAPData.coreCapitalRatioArr = m_coreCapitalRatioArr;
    hSAPData.nofCoreCapitalRatioData = m_nofCoreCapitalRatioData;
    hSAPData.riskStdDev = m_riskStdDev;
    hSAPData.meanReturns = m_meanReturns;
    hSAPData.returnOnEquity = m_returnOnEquity;





    csaHtmlPg.createHtmlPage(hSAPData);


    ui->webView->setHtml(hSAPData.htmlStr);


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


        res = db.subAnalysisEarningsDateExists(earningsDate,
                                               mainAnalysisId,
                                               earningsDateId);

        // Exist anaysis date?
        if(false == res)
        {
            res = db.insertSubAnalysisEarningsDate(earningsDate,
                                                   mainAnalysisId,
                                                   earningsDateId);
        }


        if(true == res)
        {
           earningsDataIdIsValid = false;

           if( true == db.getSubAnalysisEarningsId(mainAnalysisId, earningsDateId, inputEarningsDataId))
           {
               earningsDataIdIsValid = true;
           }

            res = db.insertSubAnalysisEarnings(earningsDateId,
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
