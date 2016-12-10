#ifndef STOCKANALYSISTAB_H
#define STOCKANALYSISTAB_H

#include <QDialog>
#include "inc/guiUtil/guiFinanceCtrls.h"
#include "dbHndl.h"
#include "subanalysisdisplaygraphdata.h"

namespace Ui {
class StockAnalysisTab;
}






class StockAnalysisTab : public QDialog
{
    Q_OBJECT

    QwtPlot *m_qwtPlot[10];
    subAnalysisDisplayGraphData m_saDisply;


    QwtPlotHistogram *m_barHist;
    QVector <QwtIntervalSample> m_barHistData;



    SubAnalysDataST       m_dividendDataArr[MAX_NOF_DIVIDEND_ARR_DATA];
    int                   m_nofDividendArrData;

    SubAnalysDataST       m_earningsDataArr[MAX_NOF_EARNINGS_ARR_DATA];
    int                   m_nofEarningsArrData;

    SubAnalysDataST       m_totalCurrentAssetsArr[MAX_NOF_TOTAL_CURRENT_ASSETS_ARR_DATA];
    int                   m_nofTotalCurrentAssetsArrData;

    SubAnalysDataST       m_totalCurrentLiabilitiesArr[MAX_NOF_TOTAL_CURRENT_LIABILITIES];
    int                   m_nofTotalCurrentLiabilitiesData;

    SubAnalysDataST       m_totalLiabilitiesArr[MAX_NOF_TOTAL_CURRENT_LIABILITIES];
    int                   m_nofTotalLiabilitiesData;

    SubAnalysDataST       m_solidityArr[MAX_NOF_SOLIDITY];
    int                   m_nofSolidityData;

    SubAnalysDataST       m_coverageRatioArr[MAX_NOF_COVERAGE_RATIO];
    int                   m_nofCoverageRatioData;

    SubAnalysDataST       m_coreTier1RatioArr[MAX_NOF_CORE_TIER_1_RATIO];
    int                   m_nofCoreTier1RatioData;

    SubAnalysDataST       m_coreCapitalRatioArr[MAX_NOF_CORE_CAPITAL_RATIO];
    int                   m_nofCoreCapitalRatioData;

    SubAnalysDataST       m_equityArr[MAX_NOF_EQUITY];
    int                   m_nofEquityData;

    SubAnalysDataST       m_cashFlowCapexArr[MAX_NOF_CASH_FLOW_CAPEX];
    int                   m_nofCashFlowCapexData;

    SubAnalysDataST       m_operatingCashFlowArr[MAX_NOF_CASH_FLOW_CAPEX];
    int                   m_nofOperatingCashFlowData;








    //QString m_html;

    GuiFinanceCtrls m_gfc;
    QString m_stockName;
    bool m_stockNameIsInit;
    QString m_stockSymbol;
    QString m_analysisDate;

    QString  m_riskStdDev;
    QString  m_meanReturns;
    QString m_returnOnEquity;

    QString m_stockListName;
    int m_stockListId;


    void resetGuiCtrl(void);
    void resetStockAnalysisData(HtmlStockAnalysPageDataST &hSAPData);
    void initSubAnalysisPlots(void);


    void initSubAnalyseTableWidget(QTableWidget *tableWidget,
                                   QString dateHeader,
                                   QString dataHeader);



    void updateTableWithSubAnalysData(QTableWidget *tableWidget,
                                      SubAnalyseDataTypeET analyseType,
                                      SubAnalysDataST *subAnalysDataArr,
                                      int &nofArrData);


    void initCompanyTypeComboBox(void);



public:
    static const QString m_companyTypesArr[NOF_COMPANY_TYPE_ARR_DATA];


    explicit StockAnalysisTab(QWidget *parent = 0);
    ~StockAnalysisTab();

    void initSubAnalysTables(void);


private slots:
    void on_SelStockListButton_clicked();

    void on_treeWidgetStockListAnalysis_doubleClicked(const QModelIndex &index);

    void on_pushButton_clicked();

    void on_treeWidgetAnalysisDate_doubleClicked(const QModelIndex &index);

    void on_pushButtonRemoveRecord_clicked();

    void on_pushSaveDividend_clicked();

    void on_pushSaveEarnings_clicked();

    void on_pushButtonSaveTotalCurrentAsset_clicked();

    void on_pushButtonSaveTotalCurrentLiabilities_clicked();

    void on_pushButtonSaveTotalLiabilities_clicked();

    void on_pushButtonSelectSaveCompanyType_clicked();

    void on_pushButtonSaveSolidity_clicked();

    void on_pushButtonSaveCoverageRatio_clicked();

    void on_pushButtonSaveCoreTier1Ratio_clicked();

    void on_pushButtonSaveCoreCapitalRatio_clicked();

    void on_pushButtonSaveEquity_clicked();

    void on_pushButtonSaveCashFlowCapex_clicked();

    void on_pushButtonSaveOperatingCashFlow_clicked();

private:
    Ui::StockAnalysisTab *ui;
};

#endif // STOCKANALYSISTAB_H
