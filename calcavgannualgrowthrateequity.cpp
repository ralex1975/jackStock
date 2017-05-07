#include "calcavgannualgrowthrateequity.h"
#include "ui_calcavgannualgrowthrateequity.h"
#include "util.h"
#include <QMessageBox>

/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *******************************************************************/
calcAvgAnnualGrowthRateEquity::calcAvgAnnualGrowthRateEquity(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::calcAvgAnnualGrowthRateEquity)
{
    m_nofArrData = 0;
    ui->setupUi(this);
}

/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *******************************************************************/
calcAvgAnnualGrowthRateEquity::~calcAvgAnnualGrowthRateEquity()
{
    delete ui;
}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description: This function calcs avg Annual Growth Rate Equity
 *
 *******************************************************************/
void calcAvgAnnualGrowthRateEquity::on_pushButton_clicked()
{
    CUtil cu;
    double minY, maxY;
    double growthRate;
    QString avgAnnualGrowthRateEquity;
    int nofYears;

    if(false == ((ui->lineEditStartValue->text().length() > 0) &&
       (ui->lineEditStopValue->text().length() > 0) &&
       (ui->lineEditNofYears->text().length() > 0)))
    {
        QMessageBox::information(NULL, QString::fromUtf8("Fel"), QString::fromUtf8("Indata till beräkningen saknas"));
        return;
    }

    minY = ui->lineEditStartValue->text().toDouble();
    maxY = ui->lineEditStopValue->text().toDouble();
    nofYears = ui->lineEditNofYears->text().toInt();



    if(nofYears < 2)
    {
        QMessageBox::information(NULL, QString::fromUtf8("Fel"), QString::fromUtf8("För få värden (Eget kap/Aktie)"));
        return;
    }


    if(true == cu.annualGrowthRate(minY, maxY, nofYears, growthRate))
    {
        growthRate = growthRate - 1;
        growthRate = growthRate * 100;
        qDebug() << growthRate;

        avgAnnualGrowthRateEquity.sprintf("%.2f", growthRate);
        ui->lineEditResult->setText(avgAnnualGrowthRateEquity);
    }
}




/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *******************************************************************/
void calcAvgAnnualGrowthRateEquity::initDlg(QString startValue,
                                            QString endValue,
                                            QString nofYears,
                                            SubAnalysDataST *dataArr,
                                            int nofArrData)
{
    ui->lineEditStartValue->setText(startValue);
    ui->lineEditStopValue->setText(endValue);
    ui->lineEditNofYears->setText(nofYears);

    m_nofArrData = nofArrData;

    for(int i = 0; i < nofArrData; i++)
    {
        m_dataArr[i] = dataArr[i];

    }
}
