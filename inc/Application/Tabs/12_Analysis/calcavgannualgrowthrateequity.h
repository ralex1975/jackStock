#ifndef CALCAVGANNUALGROWTHRATEEQUITY_H
#define CALCAVGANNUALGROWTHRATEEQUITY_H

#include <QDialog>
#include "dbHndl.h"

namespace Ui {
class calcAvgAnnualGrowthRateEquity;
}

class calcAvgAnnualGrowthRateEquity : public QDialog
{
    Q_OBJECT

private:
    SubAnalysDataST m_dataArr[10000];
    int m_nofArrData;


public:
    explicit calcAvgAnnualGrowthRateEquity(QWidget *parent = 0);
    ~calcAvgAnnualGrowthRateEquity();
    void initDlg(QString startValue, QString endValue, QString nofYears, SubAnalysDataST *dataArr, int nofArrData);


private slots:
    void on_pushButton_clicked();

private:
    Ui::calcAvgAnnualGrowthRateEquity *ui;
};

#endif // CALCAVGANNUALGROWTHRATEEQUITY_H
