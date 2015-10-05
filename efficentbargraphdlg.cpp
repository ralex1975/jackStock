#include "efficentbargraphdlg.h"
#include "ui_efficentbargraphdlg.h"
#include "util.h"

efficentBarGraphDlg::efficentBarGraphDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::efficentBarGraphDlg)
{
    ui->setupUi(this);

    m_plot.initPlotPicker(ui->qwtPlot);

     m_efficientFrontHist = new QwtPlotHistogram ();
}


efficentBarGraphDlg::~efficentBarGraphDlg()
{
    delete ui;
    delete m_efficientFrontHist;
}



void efficentBarGraphDlg::plot()
{
    TableColumnIndicatorInfo_ST tableHeaderList[2];

    // m_efficientFrontHistData.clear();
    m_efficientFrontHist->detach();
    m_efficientFrontHist->setData(NULL);
    m_efficientFrontHist-> setStyle (QwtPlotHistogram :: Columns);


   // CUtil cu;
   // QColor::fromRgb(cu.m_colorRgbArr[color].r, cu.m_colorRgbArr[color].g, cu.m_colorRgbArr[color].b);

    m_efficientFrontHist->setBrush(Qt::blue);
    m_efficientFrontHist->setPen(QPen(Qt::black));
    m_efficientFrontHist->setSamples(m_efficientFrontHistData);
    m_efficientFrontHist->attach(ui->qwtPlot);
    ui->qwtPlot->replot();


    //m_extTable.createTableModel(m_efficientFrontHistData.size(), 2, this);
    tableHeaderList[0].name = tr("Namn");
    tableHeaderList[1].name = tr("Serie");
    m_extTable.addHeaders(ui->tableView, tableHeaderList, 2);
    // ui->tableView->resizeColumnsToContents();

}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void efficentBarGraphDlg::initCorrelationTable(QTreeWidget *treeWidget)
{

    m_extTable.createTableModel(treeWidget->topLevelItemCount(), 2, this);
    m_extTable.setHorizontalFont(ui->tableView, "Helvetica", 9);
    m_extTable.setVerticalFont(ui->tableView, "Helvetica", 9);
    mainTraversTreeWidget(treeWidget);

}



/*******************************************************************
 *
 * Function:    ()
 *
 * Description: This function traverse treewidget and check which
 *              of the nodes that are checked in column 1
 *
 *******************************************************************/
void efficentBarGraphDlg::mainTraversTreeWidget(QTreeWidget *treeWidget)
{
    int numberOfTopLevelItems = treeWidget->topLevelItemCount();
    for ( int topLevelindex=0 ; topLevelindex < numberOfTopLevelItems ; topLevelindex++)
    {
        QTreeWidgetItem *item = treeWidget->topLevelItem(topLevelindex);
        // qDebug() << "top_0" << 0 << "#" << item->text(CDbHndl::TWColum_Name);
        // qDebug() << "top_1" << 1 << "#" << item->text(CDbHndl::TWColum_SYMBOL);


        QString strIndex;
        strIndex = strIndex.sprintf("%d",topLevelindex);
        m_extTable.addData(ui->tableView, item->text(CDbHndl::TWColum_Name), topLevelindex, 0);
        m_extTable.addData(ui->tableView, strIndex, topLevelindex, 1);

         #if 0
         if(Qt::Checked == ui->treeWidget->topLevelItem(topLevelindex)->checkState(2))
             {
                 stockArr1[topLevelindex].isSelected = true;
                 qDebug() << "top" << 2 << "state" << "Qt::checked";
                 // item->setCheckState(3, Qt::Checked);
             }
             else
             {
                 m_stockArr1[topLevelindex].isSelected = false;
                 qDebug() << "top" << 1 << "state" << "Qt::Unchecked";
                 // item->setCheckState(3, Qt::Unchecked);
             }
         }
        #endif


        // Not needed we have no children
        // processItem(item, 1);
    }
}


void efficentBarGraphDlg::on_tableView_doubleClicked(const QModelIndex &index)
{

}
