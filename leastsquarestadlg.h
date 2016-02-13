#ifndef LEASTSQUARESTADLG_H
#define LEASTSQUARESTADLG_H

#include <QDialog>

namespace Ui {
class LeastSquaresTaDlg;
}

class LeastSquaresTaDlg : public QDialog
{
    Q_OBJECT

public:
    explicit LeastSquaresTaDlg(QWidget *parent = 0);
    ~LeastSquaresTaDlg();

private:
    Ui::LeastSquaresTaDlg *ui;
};

#endif // LEASTSQUARESTADLG_H
