#include "adminMyPortFolio.h"
#include "ui_adminmyportfolio.h"
#include <QMessageBox>
#include <dbHndl.h>
#include "inc/TaBuySellIdicator/taBuySellIdicator.h"
#include "util.h"
#include <QDebug>
#include <qwt_plot_grid.h>

/*******************************************************************
 *
 * Function:
 *
 * Description:
 *
 *
 *
 *******************************************************************/
AdminMyPortfolio::AdminMyPortfolio(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminMyPortfolio)
{
    CExtendedQwtPlot eqp;

    bool enableMinorXGrid = true;
    bool enableMajorXGrid = false;
    bool enableMinorYGrid = false;
    bool enableMajorYGrid = true;

    ui->setupUi(this);

    m_barHist = new QwtPlotHistogram();

    m_okToRemoveData = false;

    TableColumnIndicatorInfo_ST headerList[CDbHndl::YNOI_NOF_ITEMS];
    int nofCols = 0;
    int len = 1000;
    CDbHndl db;
    int nofRows;

    // QHeaderView *horizHeaderTable;

    headerList[nofCols++].name = QString::fromUtf8("År     ");
    headerList[nofCols++].name = QString::fromUtf8("Månad  ");
    headerList[nofCols++].name = QString::fromUtf8("Värde     ");
    headerList[nofCols++].name = QString::fromUtf8("Insättning");
    headerList[nofCols++].name = QString::fromUtf8("Utdelning");
    headerList[nofCols++].name = QString::fromUtf8("Uttag");

    m_ynokd.setHorizontalFont(ui->tableView, "Helvetica", 9);
    m_ynokd.setVerticalFont(ui->tableView, "Helvetica", 9);

     // Init table
    m_ynokd.createTableModel(len, nofCols, this);
    m_ynokd.addHeaders(ui->tableView, headerList, nofCols);

    // Init columm head click event handlers for tableView
    //horizHeaderTable = ui->tableView->horizontalHeader();

    db.getNofRowsProgressMyPortfolioData(nofRows);

    db.getAllProgressMyPortfolioData(m_ynokd, ui->tableView);

    // connect(horizHeaderTable,  SIGNAL(sectionClicked(int)), this, SLOT(tableHeaderClicked(int)));

    m_ynokd.setHorizontalFont(ui->tableView, "Helvetica", 8);
    m_ynokd.setVerticalFont(ui->tableView, "Helvetica", 8);
    ui->tableView->resizeColumnsToContents();


   plotPortfolioMarketValue(m_ynokd, nofRows);

   m_eqp.initPlotPicker(ui->qwtPlot);
   m_eqp.initPlotZoomer(ui->qwtPlot);
   m_eqp.initPlotPanner(ui->qwtPlot);
   m_eqp.enablePanningMode(true);
   // m_eqp.enableZoomMode(true);

   m_eqp1.initPlotPicker(ui->qwtPlot_2);
   m_eqp1.initPlotZoomer(ui->qwtPlot_2);
   m_eqp1.initPlotPanner(ui->qwtPlot_2);
   //m_eqp1.enableZoomMode(true);

    m_eqp1.enablePanningMode(true);
//   m_plot.enableZoomMode(false);

    eqp.turnOnPlotGrid(ui->qwtPlot_2, Qt::darkYellow, enableMinorXGrid, enableMajorXGrid,
                       enableMinorYGrid, enableMajorYGrid);

    QPalette* palette1 = new QPalette();
    palette1->setColor(QPalette::WindowText,Qt::red);
    //label->setPalette(*palette1);

    QPalette* palette2 = new QPalette();
    palette2->setColor(QPalette::WindowText,Qt::blue);


    // ui->marketValueLabel();
    ui->InvestedMoneyLabel->setPalette(*palette1);
    ui->InvestedMoneyLabel_2->setPalette(*palette1);
    ui->InvestedMoneyLabelData->setPalette(*palette1);

    ui->valueIncreaseLabel->setPalette(*palette2);
    ui->valueIncreaseLabel_2->setPalette(*palette2);
    ui->valueIncreaseLabelData->setPalette(*palette2);



}


/*******************************************************************
 *
 * Function:
 *
 * Description:
 *
 *
 *
 *******************************************************************/
AdminMyPortfolio::~AdminMyPortfolio()
{
    delete ui;
    delete m_barHist;
}



#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
/*******************************************************************
 *
 * Function:
 *
 * Description:
 *
 *
 *
 *******************************************************************/
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


    if(ui->tableView->model()->index(rowidx , 5).data().toString().isNull())
    {
        str = " ";
    }
    else
    {
        str = (ui->tableView->model()->index(rowidx , 5).data().toString());
    }

   ui->withdrawingLineEdit->setText(str);


    if(m_year.size() > 0 && m_month.size() > 0)
    {
        m_okToRemoveData = true;
    }

}
#pragma GCC diagnostic pop


/*******************************************************************
 *
 * Function:
 *
 * Description:
 *
 *
 *
 *******************************************************************/
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
                                                ui->ReceivedDividentLineEditAdminPortfolio->text(),
                                                ui->withdrawingLineEdit->text()))
    {






       // m_ynokd.addDataSetTextColor(ui->tableView, ui->yearLineEditAdminPortfolio->text(), row, 0, Qt::black);
       // m_ynokd.addDataSetTextColor(ui->tableView, ui->monthLineEditAdminPortfolio->text(), row, 1, Qt::black);
       // m_ynokd.addDataSetTextColor(ui->tableView, ui->marketValueLineEditAdminPortfolio->text(),  row, 2, Qt::black);
       // m_ynokd.addDataSetTextColor(ui->tableView, ui->addedMoneyLineEditAdminPortfolio->text(), row, 3, Qt::black);
       // m_ynokd.addDataSetTextColor(ui->tableView, ui->ReceivedDividentLineEditAdminPortfolio->text(), row, 4, Qt::black);

    }

    int nofRows;
    db.getNofRowsProgressMyPortfolioData(nofRows);
    db.getAllProgressMyPortfolioData(m_ynokd, ui->tableView);
    plotPortfolioMarketValue(m_ynokd, nofRows);

}



/*******************************************************************
 *
 * Function:
 *
 * Description:
 *
 *
 *
 *******************************************************************/
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



/*******************************************************************
 *
 * Function:
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void AdminMyPortfolio::addTableHead(void)
{
    TableColumnIndicatorInfo_ST headerList[CDbHndl::YNOI_NOF_ITEMS];
    int nofCols = 0;
    int len = 1000;
    CDbHndl db;


    //QHeaderView *horizHeaderTable;


    // db.delAllTblProgressMyPortfolio();

    headerList[nofCols++].name = QString::fromUtf8("År     ");
    headerList[nofCols++].name = QString::fromUtf8("Månad  ");
    headerList[nofCols++].name = QString::fromUtf8("Värde     ");
    headerList[nofCols++].name = QString::fromUtf8("Insättning");
    headerList[nofCols++].name = QString::fromUtf8("Utdelning");
    headerList[nofCols++].name = QString::fromUtf8("Uttag");


    m_ynokd.setHorizontalFont(ui->tableView, "Helvetica", 9);
    m_ynokd.setVerticalFont(ui->tableView, "Helvetica", 9);

     // Init table
    m_ynokd.createTableModel(len, nofCols, this);
    m_ynokd.addHeaders(ui->tableView, headerList, nofCols);

    // Init columm head click event handlers for tableView
    //horizHeaderTable = ui->tableView->horizontalHeader();

    int nofRows;

    db.getNofRowsProgressMyPortfolioData(nofRows);
    db.getAllProgressMyPortfolioData(m_ynokd, ui->tableView);
    ui->tableView->resizeColumnsToContents();

}



/*******************************************************************
 *
 * Function:
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void AdminMyPortfolio::plotPortfolioMarketValue(CExtendedTable &table, int maxRow)
{
    CYahooStockPlotUtil cyspu;
    CUtil cu;

    //int row = 0;
    int year;
    int month;
    int minYear;
    int minMonth;
    double investedMoney = 0;
    double withdraw;
    //double addedMoney;
    double valueIncrease = 0;
    QString data;
    CDbHndl db;
    QString doubleNumber;


    m_barHistData.clear();
    m_barHist->detach();
    m_barHist->setData(NULL);
    m_barHist-> setStyle (QwtPlotHistogram :: Columns);


    m_qwtPlotData.stock[0].data.detach();
    m_qwtPlotData.stock[0].data.setData(NULL);

    m_qwtPlotData.stock[1].data.detach();
    m_qwtPlotData.stock[1].data.setData(NULL);



    if(maxRow > 999)
    {
        // m_x[], m_y[] is too small
        QMessageBox::critical(NULL, QString::fromUtf8("Error"), QString::fromUtf8("Errot: maxRow > 999"));
        return;
    }



    cyspu.removePlotData(m_qwtPlotData, 0, ui->qwtPlot);
    cyspu.removePlotData(m_qwtPlotData, 1, ui->qwtPlot);
    cyspu.removePlotData(m_qwtPlotData, 2, ui->qwtPlot);
    db.getMinDateProgressMyPortfolio(minYear, minMonth);

    if(maxRow > 0)
    {
        m_qwtPlotData.axis.minMaxIsInit = false;

        // Get month count since min date
        table.getOneData(ui->tableView, maxRow - 1, 0, data);
        year = data.toInt();
        table.getOneData(ui->tableView, maxRow - 1, 1, data);
        month = data.toInt();
        m_x[maxRow-1] = (double) cu.calcDeltaMonth(year, month, minYear, minMonth);

        // Get market value
        table.getOneData(ui->tableView, maxRow - 1, 2, data);
        m_y[maxRow-1] = (double) data.toDouble();

        table.getOneData(ui->tableView, maxRow - 1, 3, data);
        investedMoney = (double) data.toDouble();
        m_z[maxRow-1] = investedMoney;

        QwtInterval interval(0, 1);
        interval.setBorderFlags( QwtInterval::ExcludeMaximum | QwtInterval::ExcludeMinimum);
        m_barHistData.append (QwtIntervalSample((m_y[maxRow]-m_y[maxRow]), interval));

        cyspu.updateMinMaxAxis(m_qwtPlotData.axis, m_x[maxRow-1], m_y[maxRow-1]);
        m_valueIncreaseArr[maxRow-1] = 0;
    }
    else
    {
        // No plot data
        return;
    }


    investedMoney = 0;
    for(int i = 1, row = maxRow - 2; row >= 0; row--)
    {
        // Get month count since min date
        table.getOneData(ui->tableView, row, 0, data);
        year = data.toInt();
        table.getOneData(ui->tableView, row, 1, data);
        month = data.toInt();
        m_x[row] = (double) cu.calcDeltaMonth(year, month, minYear, minMonth);


        // Get market value
        table.getOneData(ui->tableView, row, 2, data);
        m_y[row] = (double) data.toInt();
        cyspu.updateMinMaxAxis(m_qwtPlotData.axis, m_x[row], m_y[row]);


        // Calc increased marked value
        double diff = (m_y[row]-m_y[row+1]);

        //double investedMoney = 0;


        withdraw = 0;
        // Remove withdraw
        table.getOneData(ui->tableView, row, 5, data);
        if(true == cu.number2double(data, doubleNumber))
        {
            withdraw = doubleNumber.toDouble();
            diff += doubleNumber.toDouble();
        }


        // Invested money
        table.getOneData(ui->tableView, row, 3, data);

        // addedMoney = 0;
        if(true == cu.number2double(data, doubleNumber))
        {
            // addedMoney = (double) data.toDouble();
            investedMoney += (double) data.toDouble();
            diff -= doubleNumber.toDouble();
        }
        investedMoney = investedMoney - withdraw;
        m_z[row] = investedMoney;

        valueIncrease += diff;
        m_valueIncreaseArr[row] = valueIncrease;

        qDebug() << "sum value" << m_valueIncreaseArr[row];


        cyspu.updateMinMaxAxis(m_qwtPlotData.axis, m_x[row], m_valueIncreaseArr[row]);
        cyspu.updateMinMaxAxis(m_qwtPlotData.axis, m_x[row], m_z[row]);

        // market value change
        QwtInterval interval(i, i + 1);
        interval.setBorderFlags( QwtInterval::ExcludeMaximum | QwtInterval::ExcludeMinimum);
        m_barHistData.append (QwtIntervalSample(diff, interval));
        qDebug() << m_y[row] << m_y[row+1] << diff;
        i++;
    }

    QString str;
    str =  str.sprintf("%.0f\n", m_y[0]);
    ui->marketValueLabelData->setText(str);

    str =  str.sprintf("%.0f\n", m_z[0]);
    ui->InvestedMoneyLabelData->setText(str);


    str =  str.sprintf("%.0f\n", m_valueIncreaseArr[0]);
    ui->valueIncreaseLabelData->setText(str);


    if(m_qwtPlotData.axis.minY > 0)
    {
        m_qwtPlotData.axis.minY = 0;
    }

    m_qwtPlotData.stock[0].data.setSamples(m_x, m_y, maxRow);

    m_qwtPlotData.stock[1].data.setSamples(m_x, m_z, maxRow);
    m_qwtPlotData.stock[1].data.setPen(QPen(Qt::red, 2));

    m_qwtPlotData.stock[2].data.setSamples(m_x, m_valueIncreaseArr, maxRow);
    m_qwtPlotData.stock[2].data.setPen(QPen(Qt::blue, 2));

    cyspu.plotData(m_qwtPlotData, ui->qwtPlot, 0);
    cyspu.plotData(m_qwtPlotData, ui->qwtPlot, 1);
    cyspu.plotData(m_qwtPlotData, ui->qwtPlot, 2);

    m_barHist->setBrush(Qt::blue);
    m_barHist->setPen(QPen(Qt::black));
    m_barHist->setSamples(m_barHistData);
    m_barHist->attach(ui->qwtPlot_2);
    ui->qwtPlot_2->replot();

}
