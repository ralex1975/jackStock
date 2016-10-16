#ifndef STOCKANALYSISTAB_H
#define STOCKANALYSISTAB_H

#include <QDialog>
#include "inc/guiUtil/guiFinanceCtrls.h"

namespace Ui {
class StockAnalysisTab;
}

class StockAnalysisTab : public QDialog
{
    Q_OBJECT

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


    void resetGuiCtrl(void);


public:
    explicit StockAnalysisTab(QWidget *parent = 0);
    ~StockAnalysisTab();

private slots:
    void on_SelStockListButton_clicked();

    void on_treeWidgetStockListAnalysis_doubleClicked(const QModelIndex &index);

    void on_pushButton_clicked();

    void on_treeWidgetAnalysisDate_doubleClicked(const QModelIndex &index);

private:
    Ui::StockAnalysisTab *ui;
};

#endif // STOCKANALYSISTAB_H
