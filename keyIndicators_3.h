#ifndef KEYINDICATORS_3_H
#define KEYINDICATORS_3_H

#include <QDialog>
#include <dbHndl.h>
#include "extendedTable.h"

namespace Ui {
class keyIndicators_3;
}

class keyIndicators_3 : public QDialog
{
    Q_OBJECT

    CExtendedTable m_ynokd;
    QVector <CDbHndl::YahooNordnetOutputkeyData_ST> m_stockArr;

public:
    explicit keyIndicators_3(QWidget *parent = 0);
    ~keyIndicators_3();

private slots:
    void on_loadDataPushButtonKey3_clicked();

    void on_turnOffFilterpushButtonKey3_clicked();

public slots:
    void tableHeaderClicked(int column);

private:
    Ui::keyIndicators_3 *ui;
};

#endif // KEYINDICATORS_3_H
