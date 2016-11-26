#ifndef STOCKANALYSISTAB_H
#define STOCKANALYSISTAB_H

#include <QDialog>
#include "inc/guiUtil/guiFinanceCtrls.h"
#include "dbHndl.h"

namespace Ui {
class StockAnalysisTab;
}

#define MAX_NOF_DIVIDEND_ARR_DATA                   100
#define MAX_NOF_EARNINGS_ARR_DATA                   MAX_NOF_DIVIDEND_ARR_DATA
#define MAX_NOF_TOTAL_CURRENT_ASSETS_ARR_DATA       MAX_NOF_DIVIDEND_ARR_DATA
#define MAX_NOF_TOTAL_CURRENT_LIABILITIES           MAX_NOF_DIVIDEND_ARR_DATA
#define MAX_NOF_TOTAL_LIABILITIES                   MAX_NOF_DIVIDEND_ARR_DATA
#define MAX_NOF_SOLIDITY                            MAX_NOF_DIVIDEND_ARR_DATA
#define MAX_NOF_COVERAGE_RATIO                      MAX_NOF_DIVIDEND_ARR_DATA
#define MAX_NOF_CORE_TIER_1_RATIO                   MAX_NOF_DIVIDEND_ARR_DATA
#define MAX_NOF_CORE_CAPITAL_RATIO                  MAX_NOF_DIVIDEND_ARR_DATA



#define NOF_COMPANY_TYPE_ARR_DATA 8


class StockAnalysisTab : public QDialog
{
    Q_OBJECT

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

    SubAnalysDataST       m_coreCapitalRatioArr[MAX_NOF_CORE_TIER_1_RATIO];
    int                   m_nofCoreCapitalRatioData;





    //QString m_html;

    GuiFinanceCtrls m_gfc;
    QString m_stockName;
    bool m_stockNameIsInit;
    QString m_stockSymbol;
    QString m_analysisDate;

    QString m_stockListName;
    int m_stockListId;

#if 0
    QString m_companyDescription;
    QString m_BigCompDescription;
    QString m_bigEnoughAnswer;
    QString m_bigEnoughComment;
    QString m_strongFinancialPositionAnswer;
    QString m_strongFinancialPositionComment;
    QString m_earningStabilityAnswer;
    QString m_earningStabilityComment;
    QString m_dividendStabilityAnswer;
    QString m_dividendStabilityComment;
    QString m_earningGrowthAnswer;
    QString m_earningGrowthComment;
    QString m_keyValuePe;
    QString m_keyValuePs;
    QString m_keyValueNavPriceRatio;
    QString m_keyValueYield;
    QString m_keyValuePriceJEKRatio;
    QString m_keyValueErningsDividentRatio;
    QString m_keyValueTotalDebtEquityRatio;
    QString m_keyValueCurrentRatio;
    QString m_trustworthyLeadershipAnswer;
    QString m_trustworthyLeadershipComment;
    QString m_goodOwnershipAnswer;
    QString m_goodOwnershipComment;
    QString m_otherInformation;
#endif

    void resetGuiCtrl(void);
    void resetStockAnalysisData(HtmlStockAnalysPageDataST &hSAPData);

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

private:
    Ui::StockAnalysisTab *ui;
};

#endif // STOCKANALYSISTAB_H
