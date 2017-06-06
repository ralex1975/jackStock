#include "altgcalcintrinsicvaluedlg.h"
#include "ui_altgcalcintrinsicvaluedlg.h"
#include <QMessageBox>


AltGCalcIntrinsicValueDlg::AltGCalcIntrinsicValueDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AltGCalcIntrinsicValueDlg)
{
    ui->setupUi(this);
}

AltGCalcIntrinsicValueDlg::~AltGCalcIntrinsicValueDlg()
{
    delete ui;
}

void AltGCalcIntrinsicValueDlg::on_pushButtonCalc_clicked()
{
    double intrinsicValue;
    double earningPerShare;
    double earningGrowth;
    double tmp;
    QString str;
    bool isNumeric1;
    bool isNumeric2;

    // lineEditIntrinsicValue

    str = ui->lineEditEarningPerShare->text();
    earningPerShare = str.toDouble(&isNumeric1);

    str = ui->lineEditEarningGrowth->text();
    earningGrowth = str.toDouble(&isNumeric2);

    if((isNumeric1 == true) && (isNumeric2 == true))
    {
        tmp = 2.0 * earningGrowth;
        tmp += 8.5;
        intrinsicValue = earningPerShare * tmp;
        str.sprintf("%.2f",intrinsicValue);
        ui->lineEditIntrinsicValue->setText(str);
    }
    else
    {
        QMessageBox::information(NULL, QString::fromUtf8("Error"), QString::fromUtf8("Invalid data"));
    }
}
