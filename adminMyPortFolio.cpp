#include "adminMyPortFolio.h"
#include "ui_adminmyportfolio.h"
#include <QMessageBox>
#include <dbHndl.h>

AdminMyPortfolio::AdminMyPortfolio(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminMyPortfolio)
{
    ui->setupUi(this);

    m_okToRemoveData = false;

    TableColumnIndicatorInfo_ST headerList[CDbHndl::YNOI_NOF_ITEMS];
    int nofCols = 0;
    int len = 1000;
    CDbHndl db;

    QHeaderView *horizHeaderTable;


    // db.delAllTblProgressMyPortfolio();

    headerList[nofCols++].name = QString::fromUtf8("År     ");
    headerList[nofCols++].name = QString::fromUtf8("Månad  ");
    headerList[nofCols++].name = QString::fromUtf8("Värde     ");
    headerList[nofCols++].name = QString::fromUtf8("Insättning");
    headerList[nofCols++].name = QString::fromUtf8("Utdelning");

    m_ynokd.setHorizontalFont(ui->tableView, "Helvetica", 9);
    m_ynokd.setVerticalFont(ui->tableView, "Helvetica", 9);

     // Init table
    m_ynokd.createTableModel(len, nofCols, this);
    m_ynokd.addHeaders(ui->tableView, headerList, nofCols);

    // Init columm head click event handlers for tableView
    horizHeaderTable = ui->tableView->horizontalHeader();

    db.getAllProgressMyPortfolioData(m_ynokd, ui->tableView);

    // connect(horizHeaderTable,  SIGNAL(sectionClicked(int)), this, SLOT(tableHeaderClicked(int)));

}

AdminMyPortfolio::~AdminMyPortfolio()
{
    delete ui;
}



void AdminMyPortfolio::on_tableView_doubleClicked(const QModelIndex &index)
{
    QString str;

    // Get data form table
    ui->tableView->selectionModel()->currentIndex().row();
    int rowidx = ui->tableView->currentIndex().row();

    m_year = (ui->tableView->model()->index(rowidx , 0).data().toString());
    ui->yearLineEditAdminPortfolio->setText(m_year);

    m_month = (ui->tableView->model()->index(rowidx , 1).data().toString());
    ui->monthLineEditAdminPortfolio->setText(m_month);


     if(ui->tableView->model()->index(rowidx , 2).data().toString().isNull())
     {
         str = " ";
     }
     else
     {
         str = (ui->tableView->model()->index(rowidx , 2).data().toString());
     }
     ui->marketValueLineEditAdminPortfolio->setText(str);


     if(ui->tableView->model()->index(rowidx , 3).data().toString().isNull())
     {
         str = " ";
     }
     else
     {
         str = (ui->tableView->model()->index(rowidx , 3).data().toString());
     }
     ui->addedMoneyLineEditAdminPortfolio->setText(str);



     if(ui->tableView->model()->index(rowidx , 4).data().toString().isNull())
     {
         str = " ";
     }
     else
     {
         str = (ui->tableView->model()->index(rowidx , 4).data().toString());
     }

    ui->ReceivedDividentLineEditAdminPortfolio->setText(str);


    if(m_year.size() > 0 && m_month.size() > 0)
    {
        m_okToRemoveData = true;
    }

}

void AdminMyPortfolio::on_addDataPushButton_clicked()
{
    CDbHndl db;

   // TableColumnIndicatorInfo_ST headerList[CDbHndl::YNOI_NOF_ITEMS];
   // static int row = 0;

    // db.delAllTblProgressMyPortfolio();


    if(ui->yearLineEditAdminPortfolio->text().toInt() < 1900 && ui->yearLineEditAdminPortfolio->text().toInt() > 2200)
    {
        QMessageBox::information(NULL, QString::fromUtf8("Fel"), QString::fromUtf8("Ange år YYYY"));
    }


    if(ui->yearLineEditAdminPortfolio->text().toInt() < 01 && ui->monthLineEditAdminPortfolio->text().toInt() > 12)
    {
        QMessageBox::information(NULL, QString::fromUtf8("Fel"), QString::fromUtf8("Månad år YYYY"));
    }

    if(ui->marketValueLineEditAdminPortfolio->text().isEmpty())
    {
        QMessageBox::information(NULL, QString::fromUtf8("Fel"), QString::fromUtf8("Ange marknadsvärde"));
    }

    if(true == db.insertProgressMyPortfolioData(ui->yearLineEditAdminPortfolio->text(),
                                                ui->monthLineEditAdminPortfolio->text(),
                                                ui->marketValueLineEditAdminPortfolio->text(),
                                                ui->addedMoneyLineEditAdminPortfolio->text(),
                                                ui->ReceivedDividentLineEditAdminPortfolio->text()))
    {






       // m_ynokd.addDataSetTextColor(ui->tableView, ui->yearLineEditAdminPortfolio->text(), row, 0, Qt::black);
       // m_ynokd.addDataSetTextColor(ui->tableView, ui->monthLineEditAdminPortfolio->text(), row, 1, Qt::black);
       // m_ynokd.addDataSetTextColor(ui->tableView, ui->marketValueLineEditAdminPortfolio->text(),  row, 2, Qt::black);
       // m_ynokd.addDataSetTextColor(ui->tableView, ui->addedMoneyLineEditAdminPortfolio->text(), row, 3, Qt::black);
       // m_ynokd.addDataSetTextColor(ui->tableView, ui->ReceivedDividentLineEditAdminPortfolio->text(), row, 4, Qt::black);

    }

    //m_ynokd.m_model->clear();
    db.getAllProgressMyPortfolioData(m_ynokd, ui->tableView);



}

void AdminMyPortfolio::on_removeDataPushButton_clicked()
{
    CDbHndl db;

    if(m_okToRemoveData == true)
    {
        m_okToRemoveData = false;

        if(false == db.deleteDataFromProgressMyPortfolio(m_year, m_month))
        {
            QMessageBox::information(NULL, QString::fromUtf8("Fel"), QString::fromUtf8("Kunde inte radera"));
        }
        else
        {
            m_ynokd.clearHeaderAndData(ui->tableView);
            // db.getAllProgressMyPortfolioData(m_ynokd, ui->tableView);
            addTableHead();
        }
    }
}


void AdminMyPortfolio::addTableHead(void)
{
    TableColumnIndicatorInfo_ST headerList[CDbHndl::YNOI_NOF_ITEMS];
    int nofCols = 0;
    int len = 1000;
    CDbHndl db;

    QHeaderView *horizHeaderTable;


    // db.delAllTblProgressMyPortfolio();

    headerList[nofCols++].name = QString::fromUtf8("År     ");
    headerList[nofCols++].name = QString::fromUtf8("Månad  ");
    headerList[nofCols++].name = QString::fromUtf8("Värde     ");
    headerList[nofCols++].name = QString::fromUtf8("Insättning");
    headerList[nofCols++].name = QString::fromUtf8("Utdelning");

    m_ynokd.setHorizontalFont(ui->tableView, "Helvetica", 9);
    m_ynokd.setVerticalFont(ui->tableView, "Helvetica", 9);

     // Init table
    m_ynokd.createTableModel(len, nofCols, this);
    m_ynokd.addHeaders(ui->tableView, headerList, nofCols);

    // Init columm head click event handlers for tableView
    horizHeaderTable = ui->tableView->horizontalHeader();

    db.getAllProgressMyPortfolioData(m_ynokd, ui->tableView);

}
