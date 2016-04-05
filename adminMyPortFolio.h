#ifndef ADMINMYPORTFOLIO_H
#define ADMINMYPORTFOLIO_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStringList>
#include <QtCore>
#include <QtGui>
#include <QObject>
#include "extendedTable.h"


namespace Ui {
class AdminMyPortfolio;
}

class AdminMyPortfolio : public QDialog
{
    Q_OBJECT
    CExtendedTable m_ynokd;

public:
    explicit AdminMyPortfolio(QWidget *parent = 0);
    ~AdminMyPortfolio();

private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_addDataPushButton_clicked();

    void on_removeDataPushButton_clicked();

private:
    void addTableHead(void);


    Ui::AdminMyPortfolio *ui;
    bool m_okToRemoveData;
    QString m_yearMonth;
    QString m_year;
    QString m_month;
};

#endif // ADMINMYPORTFOLIO_H
