#include "graphtab.h"
#include "ui_graphtab.h"
#include <QDebug>
#include "dbHndl.h"
#include "common.h"
#include <QStringList>



GraphTab::GraphTab(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphTab)
{
    ui->setupUi(this);
    bool res;


    //QPalette qp;
    //qp.setColor(QPalette::Active,QPalette::Background,QColor(55,55,55));
    //ui->listWidget->setPalette(qp);
    //setAutoFillBackground(true);

    // Set list widget background
    QPalette palette = ui->listWidget->palette();
    palette.setBrush(QPalette::Base, QColor(247,246,246)/*Qt::red*/);
    ui->listWidget->setPalette(palette);

    CDbHndl db;
    // qDebug("Open db 104 %s, %d", __FILE__, __LINE__);
    // db.openDb(PATH_JACK_STOCK_DB);
    int i;


    m_qwtStockPlotData.nofStocksToPlot = 0;

    if(true == db.getAllSnapshotPlotStocksData(m_stockArr))
    {
        res = m_spu.initSockNameCombo(m_stockArr, ui->StockComboBox);
        if(res == false)
        {
            // db.closeDb();
            //qDebug("Close db 104a %s, %d", __FILE__, __LINE__);
            return;
        }

        for(i = 0; i < m_stockArr[0].data.x.count(); i++)
        {
            db.updateMinMaxData(m_qwtStockPlotData.axis, m_stockArr[0].data.x[i], m_stockArr[0].data.y[i]);
        }

        // This is first graph that is added
        m_qwtStockPlotData.nofStocksToPlot = 0;
        m_qwtStockPlotData.axis.minMaxIsInit = false;
        m_spu.addStockToListWidget(m_stockArr[0], m_qwtStockPlotData.nofStocksToPlot, ui->listWidget);
        m_spu.addStockToPlot(m_qwtStockPlotData, m_stockArr[0], m_qwtStockPlotData.axis, m_qwtStockPlotData.nofStocksToPlot);
        m_spu.plotData(m_qwtStockPlotData, ui->qwtPlot, m_qwtStockPlotData.nofStocksToPlot-1);
        m_spu.removeAllStockArrData(m_stockArr);
    }

    qDebug("Close db 104b %s, %d", __FILE__, __LINE__);
    // db.closeDb();
}
GraphTab::~GraphTab()
{
    delete ui;
}

void GraphTab::on_SelectButton_clicked()
{

    CDbHndl db;
    CStockPlotUtil::StockData_ST oneStock;

    if(m_qwtStockPlotData.nofStocksToPlot >= CStockPlotUtil::MAX_NOF_PLOT_COLORS)
    {
        QMessageBox::information(0, QString::fromUtf8("Förmånga object.Kan inte lägga till fler"), QString::fromUtf8("Kan inte lägga till fler objekt"));
        return;
    }

    // Get selected data from combo box
    oneStock.stockName = ui->StockComboBox->currentText().toUtf8();

    // Has this object already been selected?
    if(m_spu.findStock(m_qwtStockPlotData, oneStock.stockName)== true)
    {
        QMessageBox::information(0, QString::fromUtf8("Objektet visas redan"), QString::fromUtf8("Objektet visas redan"));
        return;
    }

    // Get data for this object from database
    // qDebug("Open db 105 %s, %d", __FILE__, __LINE__);
    // db.openDb(PATH_JACK_STOCK_DB);
    db.getOneSnapshotStock(oneStock, m_qwtStockPlotData.axis);

    // Update memory used to display graph
    if(true == m_spu.addStockToPlot(m_qwtStockPlotData, oneStock, m_qwtStockPlotData.axis, m_qwtStockPlotData.nofStocksToPlot))
    {
        // Add object name in list widget
        m_spu.addStockToListWidget(oneStock, m_qwtStockPlotData.nofStocksToPlot-1, ui->listWidget);

        // Refresh graph so new object data are displayed
        if(m_qwtStockPlotData.nofStocksToPlot > 0)
        {
            m_spu.plotData(m_qwtStockPlotData, ui->qwtPlot, m_qwtStockPlotData.nofStocksToPlot-1);
        }
        else if(m_qwtStockPlotData.nofStocksToPlot == 0)
        {
            m_spu.plotData(m_qwtStockPlotData, ui->qwtPlot, m_qwtStockPlotData.nofStocksToPlot);
        }
    }
    // qDebug("Close db 105 %s, %d", __FILE__, __LINE__);
    //db.closeDb();

}

#if 0
void GraphTab::on_removePlotButton_clicked()
{
    int removedIndex;

    // Check that any item in list wighet is selected && and be sure that least one graph can be removed
    if(ui->listWidget->selectedItems().count() == 1 && m_qwtStockPlotData.nofStocksToPlot > 0)
    {
        // Get select item in list widget
        ui->listWidget->currentItem()->text();

        // Remove data from memory
        m_spu.removePlotFromStock(m_qwtStockPlotData, ui->listWidget->currentItem()->text(), removedIndex);

        // Update graph so object disappears
        ui->qwtPlot->replot();
        delete ui->listWidget->currentItem();
    }
    else
    {
        QMessageBox::information(0, QString::fromUtf8("Kan inte ta bort Aktie"), QString::fromUtf8("Minst en aktie måste väljas i listan"));
    }
}
#endif

void GraphTab::on_removePlotButton_clicked()
{

    // Check that any item in list wighet is selected && and be sure that least one graph can be removed
    if(ui->listWidget->selectedItems().count() == 1 && m_qwtStockPlotData.nofStocksToPlot > 0)
    {
        // Get select item in list widget
        //ui->listWidget->currentItem()->text();
        delete ui->listWidget->currentItem();

        m_spu.emtypPlotData(m_qwtStockPlotData);
        ui->qwtPlot->replot();
        copyListBoxDataToPlotList(m_qwtStockPlotData,  ui->listWidget);

        // Update graph so object disappears
        //ui->qwtPlot->replot();

    }
    else
    {
        QMessageBox::information(0, QString::fromUtf8("Kan inte ta bort Aktie"), QString::fromUtf8("Minst en aktie måste väljas i listan"));
    }
}






void GraphTab::copyListBoxDataToPlotList(CStockPlotUtil::PlotData_ST &allPlotData,  QListWidget *listWidget)
{
    CDbHndl db;
    CStockPlotUtil::StockData_ST oneStock;
    QStringList listWidgetData;

    allPlotData.nofStocksToPlot = 0;
    int count = listWidget->count();

    for(int index = 0; index < count; index++)
    {
        oneStock.data.x.clear();
        oneStock.data.y.clear();


        QListWidgetItem *item = listWidget->item(index);
        oneStock.stockName = item->text().toUtf8();
        listWidgetData.append(oneStock.stockName);


        // A wild item has appeared
        // qDebug("Open db 106 %s, %d", __FILE__, __LINE__);
        // db.openDb(PATH_JACK_STOCK_DB);
        db.getOneSnapshotStock(oneStock, allPlotData.axis);

        // Update memory used to display graph
        if(true == m_spu.addStockToPlot(allPlotData, oneStock, allPlotData.axis, allPlotData.nofStocksToPlot))
        {


            // Refresh graph so new object data are displayed
            if(allPlotData.nofStocksToPlot > 0)
            {
                m_spu.plotData(allPlotData, ui->qwtPlot, allPlotData.nofStocksToPlot-1);
            }
            else if(allPlotData.nofStocksToPlot == 0)
            {
                m_spu.plotData(allPlotData, ui->qwtPlot, allPlotData.nofStocksToPlot);
            }
        }
        // qDebug("Close db 106 %s, %d", __FILE__, __LINE__);
        // db.closeDb();
    }
    listWidget->clear();
    int j = 0;
    foreach(QString item, listWidgetData)
    {
        QString s = QString::fromUtf8(item.toStdString().c_str());
        listWidget->addItem(s);
        //listWidget->addItem(item);
        listWidget->item(j)->setForeground(m_spu.m_qPenArr[j]);
        j++;
    }

    listWidgetData.clear();
}
