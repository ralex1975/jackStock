#include "stockAnalysisTab.h"
#include "ui_stockanalysistab.h"

StockAnalysisTab::StockAnalysisTab(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StockAnalysisTab)
{
    ui->setupUi(this);
}

StockAnalysisTab::~StockAnalysisTab()
{
    delete ui;
}
