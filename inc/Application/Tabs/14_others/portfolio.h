#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <QDialog>
#include <QStandardItemModel>
#include <QTableView>

namespace Ui
{
class Portfolio;
}

class Portfolio : public QDialog
{
    Q_OBJECT
private:
     QStandardItemModel *m_model;
    
public:

     enum StockIndicatorSortType
     {
         SISORT_TYPE_FLOAT,
         SISORT_TYPE_TEXT
     };

     struct TableColumnIndicatorInfo_ST
     {
         QString name;
         qint64 indicatorlMask;
         QString nameInDb;
         QString sortOrder;
         StockIndicatorSortType sortType;

     };

    explicit Portfolio(QWidget *parent = 0);
    ~Portfolio();
     void addHeaders(QTableView *tableView, Portfolio::TableColumnIndicatorInfo_ST *tableHeaderList, int maxNofData);
     void createTableModel(int maxRow, int maxColumn, QObject *parent);


private slots:
    void on_pushButton_clicked();

    void on_ResetDatabaseButton_clicked();

    void on_ReadDbButton_clicked();

private:
    Ui::Portfolio *ui;
};

#endif // PORTFOLIO_H
