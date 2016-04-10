#ifndef STOCKANALYSISTAB_H
#define STOCKANALYSISTAB_H

#include <QDialog>

namespace Ui {
class StockAnalysisTab;
}

class StockAnalysisTab : public QDialog
{
    Q_OBJECT

public:
    explicit StockAnalysisTab(QWidget *parent = 0);
    ~StockAnalysisTab();

private:
    Ui::StockAnalysisTab *ui;
};

#endif // STOCKANALYSISTAB_H
