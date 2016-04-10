/*******************************************************************
 *
 * Filename: extendedTable.h
 * Date:     2012-09-08
 *
 * Description: This class contains a number of functions that
 *              makes it easer to use the use the tableView
 *
 *
 *******************************************************************/

#ifndef EXTENDEDTABLE_H
#define EXTENDEDTABLE_H

#include <QStandardItemModel>
#include <QStringList>
#include <QtCore>
#include <QtGui>
#include <QObject>
#include "dbHndl.h"

class CExtendedTable
{
private:
     QStandardItemModel *m_model;


public:
    CExtendedTable();
    void getOneData(QTableView *tableView, int row, int col, QString &data);
    void addHeaders(QTableView *tableView, TableColumnIndicatorInfo_ST *tableHeaderList, int maxNofData);
    void addHorzVertHeaders(QTableView *tableView, QVector <CDbHndl::EfficPortStockData_ST> stockArr1, int maxNofData);
    void addHeaders(QTableView *tableView);
    void addData(QTableView *tableView, QString data, int row, int col);
    void createTableModel(int maxRow, int maxColumn, QObject *parent = 0);
    void deleteAllData(QTableView *tableView);
    // void deleteRowFrom(int row, QTableView *tableView);
    void clearHeaderAndData(QTableView *tableView);
    int  getNofRows(void);
    int  getNofCols(void);
    void getOneData(QTableView *tableView, const QModelIndex &index, QString &data);
    void getOneData(int row, int col, QString &data);
    void getOneRowWithData(QStringList &oneRowWithData, int row);
    void resizeColumnsToContents(QTableView *tableView);
    void getHorizontalHeaderItem(int col, QString &str);
    void getVerticalHeaderItem(int row, QString &str);
    void setHorizontalFont(QTableView *tableView, const QString font, int size);
    void setVerticalFont(QTableView *tableView, const QString font, int size);
    void setBackgroundColor(QTableView *tableView, int row, int col, Qt::GlobalColor color);
    void setTextColor(QTableView *tableView, int row, int col, Qt::GlobalColor color);
    bool addSnappshotData(QTableView *tableView,  QString dbName, QString str, int row, int col, QSqlRecord rec);
    void addDataSetTextColor(QTableView *tableView, QString data, int row, int col, Qt::GlobalColor color);
    void addNumberIndicateSignWithColor(QTableView *tableView, QString data, int row, int col);
    void setTableWidth(QTableView *tableView, int nofColumns);
    void addCheckBoxItem(QTableView *tableView, int row, int col, Qt::CheckState checked, QString str="");
    bool tableRowIsChecked(int row, int col, bool &isChecked);

};

#endif // EXTENDEDTABLE_H
