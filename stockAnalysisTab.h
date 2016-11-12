#ifndef STOCKANALYSISTAB_H
#define STOCKANALYSISTAB_H

#include <QDialog>
#include "inc/guiUtil/guiFinanceCtrls.h"

namespace Ui {
class StockAnalysisTab;
}

#define MAX_NOF_DIVIDEND_ARR_DATA                   100
#define MAX_NOF_EARNINGS_ARR_DATA                   MAX_NOF_DIVIDEND_ARR_DATA
#define MAX_NOF_TOTAL_CURRENT_ASSETS_ARR_DATA       MAX_NOF_DIVIDEND_ARR_DATA
#define MAX_NOF_TOTAL_CURRENT_LIABILITIES           MAX_NOF_DIVIDEND_ARR_DATA

class StockAnalysisTab : public QDialog
{
    Q_OBJECT

     QString m_html;

     DividendDataST             m_dividendDataArr[MAX_NOF_DIVIDEND_ARR_DATA];
     EarningsDataST             m_earningsDataArr[MAX_NOF_EARNINGS_ARR_DATA];
     TotalCurrentAssetsST       m_totalCurrentAssetsArr[MAX_NOF_TOTAL_CURRENT_ASSETS_ARR_DATA];
     TotalCurrentLiabilitiesST  m_totalCurrentLiabilitiesArr[MAX_NOF_TOTAL_CURRENT_LIABILITIES];



    GuiFinanceCtrls m_gfc;
    QString m_stockName;
    QString m_stockSymbol;
    QString m_analysisDate;

    QString m_stockListName;
    int m_stockListId;

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


    void resetGuiCtrl(void);
    void resetStockAnalysisData(void);


public:
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

private:
    Ui::StockAnalysisTab *ui;
};

#endif // STOCKANALYSISTAB_H
