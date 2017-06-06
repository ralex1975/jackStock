#ifndef ALTGCALCINTRINSICVALUEDLG_H
#define ALTGCALCINTRINSICVALUEDLG_H

#include <QDialog>

namespace Ui {
class AltGCalcIntrinsicValueDlg;
}

class AltGCalcIntrinsicValueDlg : public QDialog
{
    Q_OBJECT

public:
    explicit AltGCalcIntrinsicValueDlg(QWidget *parent = 0);
    ~AltGCalcIntrinsicValueDlg();

private slots:
    void on_pushButtonCalc_clicked();

private:
    Ui::AltGCalcIntrinsicValueDlg *ui;
};

#endif // ALTGCALCINTRINSICVALUEDLG_H
