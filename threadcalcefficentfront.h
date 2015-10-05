#ifndef THREADCALCEFFICENTFRONT_H
#define THREADCALCEFFICENTFRONT_H

#include <QtNetwork/QSslSocket>
#include <QThread>
#include <QtCore>
#include <QFile>
#include "httpwindow.h"
#include "dbHndl.h"
#include "common.h"
#include "extendedTable.h"
#include <QTableView>


class threadCalcEfficentFront : public QThread
{
public:
    explicit threadCalcEfficentFront(QObject *parent = 0);
    void run();
    void initCalcEfficPortData(QString startDate,
                               QString endDate,
                               QTableView **tableView,
                               CExtendedTable &extendedtable,
                               double **varCovMatrix,
                               double **correlationMatrix,
                               QVector <CDbHndl::EfficPortStockData_ST> &stockArr1);
private:
    bool m_isIdle;
    QTableView *m_tableView;
    QString m_startDate;
    QString m_endDate;
    CExtendedTable *m_extendedtable;

    double **m_varCovMatrix;
    double **m_correlationMatrix;
    QVector <CDbHndl::EfficPortStockData_ST> *m_stockArr1;

    void calcEfficPortData(void);

};

#endif // THREADCALCEFFICENTFRONT_H
