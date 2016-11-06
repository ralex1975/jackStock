#include "stockAnalysisTab.h"
#include "ui_stockanalysistab.h"
#include "util.h"
#include "inc/guiUtil/guiFinanceColor.h"
#include "dbHndl.h"
#include "extendedLineEdit.h"
#include "yahooStockPlotUtil.h"
#include "createstockanalysishtmlpage.h"
#include <QtWebKit/QWebView>



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
    QFont fnt;
    QStringList labels;
    labels << QString::fromUtf8("År") << QString::fromUtf8("Utdel/Aktie");

    ui->tableWidgetDividend->setRowCount(20);
    ui->tableWidgetDividend->setColumnCount(2);


    fnt.setPointSize(9);
    fnt.setFamily("Sans Serif");
    const int rowCount = ui->tableWidgetDividend->rowCount();
    const int columnCount = ui->tableWidgetDividend->columnCount();

    for(int i = 0; i < rowCount; ++i)
    {
        for(int j = 0; j < columnCount; ++j)
        {
            QTableWidgetItem* selectedItem = ui->tableWidgetDividend->item(i, j);
            if(selectedItem != NULL)
            {
                selectedItem->setFont(fnt);
            }
        }
    }

    ui->tableWidgetDividend->setHorizontalHeaderLabels(labels);
    ui->tableWidgetDividend->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableWidgetDividend->horizontalHeader()->setResizeMode(1, QHeaderView::ResizeToContents);

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


    // Get and display sub data
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

            QTableWidgetItem *itemDate = new QTableWidgetItem( data.Date);
            ui->tableWidgetDividend->setItem( i, 0, itemDate);

            QTableWidgetItem *itemDividend = new QTableWidgetItem(data.Dividend);
            ui->tableWidgetDividend->setItem( i, 1, itemDividend);

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

#if 0
/******************************************************************
 *
 * Function:    on_pushSaveDividend_clicked()
 *
 * Description: This function saves data to db
 *
 *
 *
 *
 *****************************************************************/
void StockAnalysisTab::on_pushSaveDividend_clicked()
{
   // QString itemDate, itemDividend, str;
    int nofData;
    nofData = ui->tableWidgetDividend->rowCount();
    for(int row = 0; row < nofData; row++)
    {
       if(NULL != ui->tableWidgetDividend->item(row, 0))
       {
            qDebug() << ui->tableWidgetDividend->item(row, 0)->text();
            break;
       }
        //ui->tableWidgetDividend->item(row, 0);
        //itemDate = ui->tableWidgetDividend->cellWidget(row, 1);
        //itemDividend = ui->tableWidgetDividend->cellWidget(row, 2);

        //qDebug() << "Date" << itemDate;
        //qDebug() << "Dividend" << itemDividend;
    }

}
#endif



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
            }

            dividendData.toDouble(&isValid);
            if (false == isValid)
            {
               QMessageBox::information(this, QString::fromUtf8("Uppdatera databas"), QString::fromUtf8("Dividend is not a number"));
            }
       }
       else
       {
           break;
       }


        if(true == res)
        {
            res = db.subAnalysisDividendDateExists(dividendDate,
                                                   mainAnalysisId,
                                                   dividendDateId);
        }

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



