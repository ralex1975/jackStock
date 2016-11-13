#include "stockAnalysisTab.h"
#include "ui_stockanalysistab.h"
#include "util.h"
#include "inc/guiUtil/guiFinanceColor.h"
#include "dbHndl.h"
#include "extendedLineEdit.h"
#include "yahooStockPlotUtil.h"
#include "createstockanalysishtmlpage.h"
#include <QtWebKit/QWebView>

//Sub Analys Tables defines
#define SAT_COLUMN_DATE                  0
#define SAT_COLUMN_DATA                  1
#define SAT_NOF_ROWS                    20
#define SAT_NOF_COLUMNS                  2
#define SAT_FONT_NAME                    QString::fromUtf8("Helvetica")
#define SAT_FONT_SIZE                    9


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
    QString dateHeader = QString::fromUtf8("   År   ");
    QString dataHeader;

    // Dividend
    dataHeader = QString::fromUtf8("Utdel/Aktie");
    initSubAnalyseTableWidget(ui->tableWidgetDividend, dateHeader,dataHeader);

    // Earnings
    dataHeader = QString::fromUtf8("Vinst/Aktie");
    initSubAnalyseTableWidget(ui->tableWidgetEarnings, dateHeader,dataHeader);

    // TotalCurrentAssets
    dataHeader = QString::fromUtf8("Oms.tillg");
    initSubAnalyseTableWidget(ui->tableWidgetTotalCurrentAsset, dateHeader,dataHeader);

    // totalCurrentLiabilities
    dataHeader = QString::fromUtf8("Kortf.skuld");
    initSubAnalyseTableWidget(ui->tableWidgetTotalCurrentLiabilities, dateHeader,dataHeader);

    // totalLiabilities
    dataHeader = QString::fromUtf8("Totala Skuld");
    initSubAnalyseTableWidget(ui->tableWidgetTotalLiabilities, dateHeader,dataHeader);
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
        db.getStockAnalysisDate(m_stockName, m_stockSymbol, ui->treeWidgetAnalysisDate);
    }

    ui->StockNameLabel_16->setText(m_stockName);
    ui->ananlysisDateLabel_32->setText(m_analysisDate);


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
            ui->textEditMoreInfobusinessDescription->clear();

            ui->lineEditMoreInfoCompanyName->setText(companyDescripData.assetName);
            ui->lineEditMoreInfoexecutiveDirector->setText(companyDescripData.executiveDirector);
            ui->lineEditMoreInfoBransch->setText(companyDescripData.bransch);
            ui->lineEditMoreInfoSector->setText(companyDescripData.sector);
            ui->textEditMoreInfobusinessDescription->insertPlainText(companyDescripData.businessDescription);

        }

    }

    riskReturnData.stockSymbol = m_stockSymbol;
    res = m_gfc.calcRiskAndReturn(startDate, m_analysisDate, riskReturnData);

    if(true == res)
    {
        color = gfc.getColorRiskReturns(riskReturnData.riskStdDev, riskReturnData.meanReturns);

        riskStr.sprintf("%.3f", riskReturnData.riskStdDev);

        returnStr.sprintf("%.3f", riskReturnData.meanReturns);

        ui->lineEditExpectedReturn->setText(returnStr);
        ui->lineEditExpectedRisk->setText(riskStr);

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






#if 0
    QFont font;
    font.setPointSize(9);
    font.setFamily("Helvetica");


    ui->tableWidgetDividend->clearContents();
    ui->tableWidgetEarnings->clearContents();
    ui->tableWidgetTotalCurrentAsset->clearContents();
    ui->tableWidgetTotalCurrentLiabilities->clearContents();

    // Dividend
    int nofDividendArrData;
    DividendDataST data;

    if(true == db.getSubAnalysisDividendData(m_stockName,
                                             m_stockSymbol,
                                             m_dividendDataArr,
                                             nofDividendArrData))
    {

        for(int i = 0; i < nofDividendArrData; i++)
        {
            data = m_dividendDataArr[i];
            ui->tableWidgetDividend->insertRow(i);

            QTableWidgetItem *itemDate = new QTableWidgetItem( data.date);
            itemDate->setFont(font);
            ui->tableWidgetDividend->setItem( i, 0, itemDate);


            QTableWidgetItem *itemDividend = new QTableWidgetItem(data.dividend);
            itemDividend->setFont(font);
            ui->tableWidgetDividend->setItem( i, 1, itemDividend);

        }
    }

    // Earnings
    int nofEarningsArrData;
    EarningsDataST eData;

    if(true == db.getSubAnalysisEarningsData(m_stockName,
                                             m_stockSymbol,
                                             m_earningsDataArr,
                                             nofEarningsArrData))
    {

        for(int i = 0; i < nofEarningsArrData; i++)
        {
            eData = m_earningsDataArr[i];
            ui->tableWidgetEarnings->insertRow(i);

            QTableWidgetItem *itemDate = new QTableWidgetItem(eData.date);
            itemDate->setFont(font);
            ui->tableWidgetEarnings->setItem( i, 0, itemDate);


            QTableWidgetItem *itemEarnings = new QTableWidgetItem(eData.earnings);
            itemEarnings->setFont(font);
            ui->tableWidgetEarnings->setItem( i, 1, itemEarnings);

        }
    }



    // TotalCurrentAssets
    int nofTotalCurrentAssetsArrData;
    TotalCurrentAssetsST tcaData;


    if(true == db.getSubAnalysisTotalCurrentAssetsData(m_stockName,
                                                       m_stockSymbol,
                                                       m_totalCurrentAssetsArr,
                                                       nofTotalCurrentAssetsArrData))
    {

        for(int i = 0; i < nofTotalCurrentAssetsArrData; i++)
        {
            tcaData = m_totalCurrentAssetsArr[i];
            ui->tableWidgetTotalCurrentAsset->insertRow(i);

            QTableWidgetItem *itemDate = new QTableWidgetItem(tcaData.date);
            itemDate->setFont(font);
            ui->tableWidgetTotalCurrentAsset->setItem( i, 0, itemDate);

            QTableWidgetItem *itemData = new QTableWidgetItem(tcaData.totalCurrentAssets);
            itemData->setFont(font);
            ui->tableWidgetTotalCurrentAsset->setItem( i, 1, itemData);
        }
    }



    // TotalCurrentLiabilities
    int nofTotalCurrentLiabilitiesArrData;
    TotalCurrentLiabilitiesST tclData;


    if(true == db.getSubAnalysisTotalCurrentLiabilitiesData(m_stockName,
                                                       m_stockSymbol,
                                                       m_totalCurrentLiabilitiesArr,
                                                       nofTotalCurrentLiabilitiesArrData))
    {

        for(int i = 0; i < nofTotalCurrentLiabilitiesArrData; i++)
        {
            tclData = m_totalCurrentLiabilitiesArr[i];
            ui->tableWidgetTotalCurrentLiabilities->insertRow(i);

            QTableWidgetItem *itemDate = new QTableWidgetItem(tclData.date);
            itemDate->setFont(font);
            ui->tableWidgetTotalCurrentLiabilities->setItem( i, 0, itemDate);

            QTableWidgetItem *itemData = new QTableWidgetItem(tclData.totalCurrentLiabilities);
            itemData->setFont(font);
            ui->tableWidgetTotalCurrentLiabilities->setItem( i, 1, itemData);
        }
    }
#endif
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
            QMessageBox::critical(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Error 4: Too many array data"));
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




#if 0
//====================================================================
// Get and display sub data
//====================================================================
QFont font;
font.setPointSize(9);
font.setFamily("Helvetica");


ui->tableWidgetDividend->clearContents();
ui->tableWidgetEarnings->clearContents();
ui->tableWidgetTotalCurrentAsset->clearContents();
ui->tableWidgetTotalCurrentLiabilities->clearContents();

// Dividend
int nofDividendArrData;
DividendDataST data;

if(true == db.getSubAnalysisDividendData(m_stockName,
                                         m_stockSymbol,
                                         m_dividendDataArr,
                                         nofDividendArrData))
{

    for(int i = 0; i < nofDividendArrData; i++)
    {
        data = m_dividendDataArr[i];
        ui->tableWidgetDividend->insertRow(i);

        QTableWidgetItem *itemDate = new QTableWidgetItem( data.date);
        itemDate->setFont(font);
        ui->tableWidgetDividend->setItem( i, 0, itemDate);


        QTableWidgetItem *itemDividend = new QTableWidgetItem(data.dividend);
        itemDividend->setFont(font);
        ui->tableWidgetDividend->setItem( i, 1, itemDividend);

    }
}

#endif


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
        m_BigCompDescription = ui->textEditCompDescription->toPlainText();

        m_bigEnoughAnswer = ui->lineEditBigEnoughAnswer->text();
        m_bigEnoughComment = ui->textEditBigEnoughText->toPlainText();

        m_strongFinancialPositionAnswer = ui->lineEditFinancialStrongAnswer->text();
        m_strongFinancialPositionComment = ui->textEditFinancialStrongText->toPlainText();

        m_earningStabilityAnswer = ui->lineEditErningStabilityAnswer->text();
        m_earningStabilityComment = ui->textEditErningStabilityText->toPlainText();

        m_dividendStabilityAnswer = ui->lineEditDividentStabilityAnswer->text();
        m_dividendStabilityComment =ui->textEditDividentStabilityText->toPlainText();

        m_earningGrowthAnswer = ui->lineEditErningGrowthAnswer->text();
        m_earningGrowthComment = ui->textEditErningGrowthText->toPlainText();


        m_keyValuePe = ui->lineEditPE->text();
        m_keyValuePs = ui->lineEditPs->text();
        m_keyValueNavPriceRatio = ui->lineEditNavDivLastStockPrice->text();
        m_keyValueYield = ui->lineEditYield->text();
        m_keyValueCurrentRatio = ui->lineEditCurrentRatio->text();
        m_keyValueTotalDebtEquityRatio = ui->lineEditTotDebtEquityRatio->text();

        // Saknas m_keyValuePriceJEKRatio;
        m_keyValueErningsDividentRatio = ui->lineEditEarningsDivByDividend->text();

        m_trustworthyLeadershipAnswer = ui->lineEditTrustworthyManagementAnswer->text();
        m_trustworthyLeadershipComment =  ui->textEditTrustworthyManagementText->toPlainText();

        m_goodOwnershipAnswer = ui->lineEditBeneficialOwnershipAnswer->text();
        m_goodOwnershipComment = ui->textEditBeneficialOwnershipText->toPlainText();

        m_otherInformation = ui->textEditOtherInfo->toPlainText();

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
                           m_BigCompDescription,
                           m_bigEnoughAnswer,
                           m_bigEnoughComment,
                           m_strongFinancialPositionAnswer,
                           m_strongFinancialPositionComment,
                           m_earningStabilityAnswer,
                           m_earningStabilityComment,
                           m_dividendStabilityAnswer,
                           m_dividendStabilityComment,
                           m_earningGrowthAnswer,
                           m_earningGrowthComment,
                           m_keyValuePe,
                           m_keyValuePs,
                           m_keyValueNavPriceRatio,
                           m_keyValueYield,
                           m_keyValuePriceJEKRatio,
                           m_keyValueErningsDividentRatio,
                           m_keyValueTotalDebtEquityRatio,
                           m_keyValueCurrentRatio,
                           m_trustworthyLeadershipAnswer,
                           m_trustworthyLeadershipComment,
                           m_goodOwnershipAnswer,
                           m_goodOwnershipComment,
                           m_otherInformation,
                           analysisDataId);
    }

}


/******************************************************************
 *
 * Function:    resetStockAnalysisData()
 *
 * Description: This function use date to get stock analysis data
 *              from the database.
 *
 *
 *
 *
 *****************************************************************/
void StockAnalysisTab::resetStockAnalysisData(void)
{
    m_companyDescription.clear();
    m_BigCompDescription.clear();
    m_bigEnoughAnswer.clear();
    m_bigEnoughComment.clear();
    m_strongFinancialPositionAnswer.clear();
    m_strongFinancialPositionComment.clear();
    m_earningStabilityAnswer.clear();
    m_earningStabilityComment.clear();
    m_dividendStabilityAnswer.clear();
    m_dividendStabilityComment.clear();
    m_earningGrowthAnswer.clear();
    m_earningGrowthComment.clear();
    m_keyValuePe.clear();
    m_keyValuePs.clear();
    m_keyValueNavPriceRatio.clear();
    m_keyValueYield.clear();
    m_keyValuePriceJEKRatio.clear();
    m_keyValueErningsDividentRatio.clear();
    m_keyValueTotalDebtEquityRatio.clear();
    m_keyValueCurrentRatio.clear();
    m_trustworthyLeadershipAnswer.clear();
    m_trustworthyLeadershipComment.clear();
    m_goodOwnershipAnswer.clear();
    m_goodOwnershipComment.clear();
    m_otherInformation.clear();
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


    m_gfc.getSelStockAnalysisDateItem(ui->treeWidgetAnalysisDate,
                                      analysisDate,
                                      index);

    if(analysisDate.size() < 1)
    {
        return;
    }

    m_analysisDate = analysisDate;
    ui->analysisDateLineEdit->setText(analysisDate);
    ui->ananlysisDateLabel_32->setText(m_analysisDate);

    resetStockAnalysisData();
    resetGuiCtrl();

    db.getStockAnalysisData(m_stockName,
                         m_stockSymbol,
                         m_analysisDate,
                         m_companyDescription,
                         m_bigEnoughAnswer,
                         m_bigEnoughComment,
                         m_strongFinancialPositionAnswer,
                         m_strongFinancialPositionComment,
                         m_earningStabilityAnswer,
                         m_earningStabilityComment,
                         m_dividendStabilityAnswer,
                         m_dividendStabilityComment,
                         m_earningGrowthAnswer,
                         m_earningGrowthComment,
                         m_keyValuePe,
                         m_keyValuePs,
                         m_keyValueNavPriceRatio,
                         m_keyValueYield,
                         m_keyValuePriceJEKRatio,
                         m_keyValueErningsDividentRatio,
                         m_keyValueTotalDebtEquityRatio,
                         m_keyValueCurrentRatio,
                         m_trustworthyLeadershipAnswer,
                         m_trustworthyLeadershipComment,
                         m_goodOwnershipAnswer,
                         m_goodOwnershipComment,
                         m_otherInformation);


    csaHtmlPg.createHtmlPage(m_html,
                                          m_stockName,
                                          m_stockSymbol,
                                          m_analysisDate,
                                          m_companyDescription,
                                          m_bigEnoughAnswer,
                                          m_bigEnoughComment,
                                          m_strongFinancialPositionAnswer,
                                          m_strongFinancialPositionComment,
                                          m_earningStabilityAnswer,
                                          m_earningStabilityComment,
                                          m_dividendStabilityAnswer,
                                          m_dividendStabilityComment,
                                          m_earningGrowthAnswer,
                                          m_earningGrowthComment,
                                          m_keyValuePe,
                                          m_keyValuePs,
                                          m_keyValueNavPriceRatio,
                                          m_keyValueYield,
                                          m_keyValuePriceJEKRatio,
                                          m_keyValueErningsDividentRatio,
                                          m_keyValueTotalDebtEquityRatio,
                                          m_keyValueCurrentRatio,
                                          m_trustworthyLeadershipAnswer,
                                          m_trustworthyLeadershipComment,
                                          m_goodOwnershipAnswer,
                                          m_goodOwnershipComment,
                                          m_otherInformation);


    //QUrl url(m_html);
    //qDebug() << m_html;
    //ui->webView->setUrl(url);
    ui->webView->setHtml(m_html);


    ui->textEditCompDescription->insertPlainText(m_companyDescription);

    // Big enought
    ui->lineEditBigEnoughAnswer->setText(m_bigEnoughAnswer);
    ui->textEditBigEnoughText->insertPlainText(m_bigEnoughComment);

    // Financial strong enought
    ui->lineEditFinancialStrongAnswer->setText(m_strongFinancialPositionAnswer);
    ui->textEditFinancialStrongText->insertPlainText(m_strongFinancialPositionComment);

    // Earning Stability
    ui->lineEditErningStabilityAnswer->setText(m_earningStabilityAnswer);
    ui->textEditErningStabilityText->insertPlainText(m_earningStabilityComment);

    // Dividend Stability
    ui->lineEditDividentStabilityAnswer->setText(m_dividendStabilityAnswer);
    ui->textEditDividentStabilityText->insertPlainText(m_dividendStabilityComment);

    // Erning Growth
    ui->lineEditErningGrowthAnswer->setText(m_earningGrowthAnswer);
    ui->textEditErningGrowthText->insertPlainText(m_earningGrowthComment);

    // Pe-value
    ui->lineEditPE->setText(m_keyValuePe);
    ui->lineEditPs->setText(m_keyValuePs);
    ui->lineEditNavDivLastStockPrice->setText(m_keyValueNavPriceRatio);
    ui->lineEditYield->setText(m_keyValueYield);
    // m_keyValuePriceJEKRatio, saknas
    ui->lineEditTotDebtEquityRatio->setText(m_keyValueTotalDebtEquityRatio);
    ui->lineEditEarningsDivByDividend->setText(m_keyValueErningsDividentRatio);
    ui->lineEditCurrentRatio->setText(m_keyValueCurrentRatio);

    // Trustworthy Leadership
    ui->lineEditTrustworthyManagementAnswer->setText(m_trustworthyLeadershipAnswer);
    ui->textEditTrustworthyManagementText->setText(m_trustworthyLeadershipComment);

    // Good Ownership
    ui->lineEditBeneficialOwnershipAnswer->setText(m_goodOwnershipAnswer);
    ui->textEditBeneficialOwnershipText->insertPlainText(m_goodOwnershipComment);

    ui->textEditOtherInfo->insertPlainText(m_otherInformation);

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


