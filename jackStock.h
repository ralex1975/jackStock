#ifndef JACKSTOCK_H
#define JACKSTOCK_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>


namespace Ui
{
class CJackStock;
}

class CJackStock : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit CJackStock(QWidget *parent = 0);
    ~CJackStock();
    
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButtImportKeyTaBridgeData_clicked();

    void on_pushButtTestKeyData_clicked();

    void on_pushButtonReqCompanyHistory_clicked();

private:
    Ui::CJackStock *ui;
};

#endif // JACKSTOCK_H
