#include "leastsquarestadlg.h"
#include "ui_leastsquarestadlg.h"

LeastSquaresTaDlg::LeastSquaresTaDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LeastSquaresTaDlg)
{
    ui->setupUi(this);
}

LeastSquaresTaDlg::~LeastSquaresTaDlg()
{
    delete ui;
}
