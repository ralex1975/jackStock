#include "portfolio.h"
#include "ui_portfolio.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QtCore>
#include "parseavanzadata.h"
#include "dbHndl.h"
#include "common.h"


/*******************************************************************
 *
 * Function:    Portfolio()
 *
 * Description:
 *
 *
 *
 *
 *******************************************************************/
Portfolio::Portfolio(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Portfolio)
{
    Portfolio::TableColumnIndicatorInfo_ST tableHeaderList[8];

    m_model = 0;

    tableHeaderList[0].name = QString::fromUtf8("Datum");
    tableHeaderList[1].name = QString::fromUtf8("Konto");
    tableHeaderList[2].name = QString::fromUtf8("Typ av transaktion");
    tableHeaderList[3].name = QString::fromUtf8("Värdepapper/beskrivning");
    tableHeaderList[4].name = QString::fromUtf8("Antal");
    tableHeaderList[5].name = QString::fromUtf8("Kurs");
    tableHeaderList[6].name = QString::fromUtf8("Belopp");
    tableHeaderList[7].name = QString::fromUtf8("Valuta");

    int maxRow = 10;
    int maxColumn = 8;

    ui->setupUi(this);

    createTableModel(maxRow, maxColumn, parent);
    addHeaders(ui->tableView, tableHeaderList, maxColumn);


}

/*******************************************************************
 *
 * Function:    ~Portfolio()
 *
 * Description:
 *
 *
 *
 *
 *******************************************************************/
Portfolio::~Portfolio()
{
    delete ui;
}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 *******************************************************************/
void Portfolio::on_pushButton_clicked()
{
    CParseAvanzaData pad;
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), "database/inputData/avanza/", tr("CSV Files (*.csv)"));

    if(path.isNull() == false)
    {
        // QMessageBox::information(NULL, QString::fromUtf8("Path"), path);
        pad.parseData(path);

    }
}


/*******************************************************************
 *
 * Function:    createTableModel()
 *
 * Description: Create memory space for the data model variable
 *              used for portfolio data.
 *
 *
 *
 *******************************************************************/
void Portfolio::createTableModel(int maxRow, int maxColumn, QObject *parent)
{
    if(m_model)
        delete m_model;

    m_model = new QStandardItemModel(maxRow, maxColumn, parent);
}


/*******************************************************************
 *
 * Function:    addHeaders()
 *
 * Description: add header to the table view
 *
 *
 *
 *******************************************************************/
void Portfolio::
addHeaders(QTableView *tableView, Portfolio::TableColumnIndicatorInfo_ST *tableHeaderList, int maxNofData)
{
    int i;
    QString debugStr;


    if(maxNofData > m_model->columnCount())
    {
        debugStr.sprintf("Invalid number of columns maxNofData=%d > %d=m_model->columnCount()", maxNofData, m_model->columnCount());
        QMessageBox::critical(NULL, QString::fromUtf8("Error: Create Table"), debugStr );
        return;
    }

    for(i = 0; i < maxNofData; i++)
    {
        m_model->setHeaderData(i, Qt::Horizontal, tableHeaderList[i].name);
    }

    tableView->setModel(m_model);
    tableView->resizeColumnsToContents();
}

void Portfolio::on_ResetDatabaseButton_clicked()
{
    CDbHndl db;

    QMessageBox::StandardButton reply;
      reply = QMessageBox::question(this, QString::fromUtf8("Remove database data"), QString::fromUtf8("Remove database data?"),
                                    QMessageBox::Yes|QMessageBox::No);

    if(reply == QMessageBox::Yes)
    {
        // db.openDb(PATH_JACK_STOCK_DB);
        db.delAllTblAvanzaAccount();
        db.delAllTblAvanzaAssetData();
        db.delAllTblAvanzaAssetName();
        // db.closeDb();
    }

}

void Portfolio::on_ReadDbButton_clicked()
{
    CDbHndl db;
    int nofRow;


    int maxColumn = 8;


    // db.openDb(PATH_JACK_STOCK_DB);


    if( true == db.getTotNofAvanzaPortfolioRecords(nofRow))
    {
        #if 0
        Portfolio::TableColumnIndicatorInfo_ST tableHeaderList[8];

        tableHeaderList[0].name = QString::fromUtf8("Datum");
        tableHeaderList[1].name = QString::fromUtf8("Konto");
        tableHeaderList[2].name = QString::fromUtf8("Typ av transaktion");
        tableHeaderList[3].name = QString::fromUtf8("Värdepapper/beskrivning");
        tableHeaderList[4].name = QString::fromUtf8("Antal");
        tableHeaderList[5].name = QString::fromUtf8("Kurs");
        tableHeaderList[6].name = QString::fromUtf8("Belopp");
        tableHeaderList[7].name = QString::fromUtf8("Valuta");
        addHeaders(tableView, tableHeaderList, cols);
        #endif

        db.listAllAvanzaPortfolioRecords(this, nofRow, maxColumn, ui->tableView);
    }

    // db.closeDb();

}
