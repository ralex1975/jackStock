#include "../../inc/guiUtil/myTreeWidget.h"
#include <QList>
#include <QtCore>
#include <QColor>
#include <QMessageBox>
#include <QDebug>

/*******************************************************************
 *
 * Function:        MyTreeWidget()
 *
 * Description:
 *
 *
 *******************************************************************/
MyTreeWidget::MyTreeWidget(QWidget *parent) :
    QTreeWidget(parent)
{
}



/*******************************************************************
 *
 * Function:        setHeader4ColumnTreeWidget()
 *
 * Description:
 *
 *
 *******************************************************************/
void MyTreeWidget::
setHeader4ColumnTreeWidget(QTreeWidget *treeWidget,
                           QString header0, int headerWidth0,
                           QString header1, int headerWidth1,
                           QString header2, int headerWidth2,
                           QString header3, int headerWidth3)
{
    treeWidget->setColumnCount(4);
    treeWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    treeWidget->setSelectionBehavior(QAbstractItemView::SelectRows);


    if(QTreeWidgetItem* header = treeWidget->headerItem())
    {
        header->setText(0, header0);
        header->setText(1, header1);
        header->setText(2, header2);
        header->setText(3, header3);
    }


    treeWidget->setColumnWidth(0, headerWidth0);
    treeWidget->setColumnWidth(1, headerWidth1);
    treeWidget->setColumnWidth(2, headerWidth2);
    treeWidget->setColumnWidth(3, headerWidth3);

}



/*******************************************************************
 *
 * Function:        setHeader3ColumnTreeWidget()
 *
 * Description:
 *
 *
 *******************************************************************/
void MyTreeWidget::
setHeader3ColumnTreeWidget(QTreeWidget *treeWidget,
                           QString header0, int headerWidth0,
                           QString header1, int headerWidth1,
                           QString header2, int headerWidth2)
{

    setHeader3ColumnTreeWidget(treeWidget,
                               header0, headerWidth0, false,
                               header1, headerWidth1, false,
                               header2, headerWidth2, false);
}



/*******************************************************************
 *
 * Function:        setHeader3ColumnTreeWidget()
 *
 * Description:
 *
 *
 *******************************************************************/
void MyTreeWidget::
setHeader3ColumnTreeWidget(QTreeWidget *treeWidget,
                           QString header0, int headerWidth0, bool hideCol0,
                           QString header1, int headerWidth1, bool hideCol1,
                           QString header2, int headerWidth2, bool hideCol2)
{
    treeWidget->setColumnCount(3);
    treeWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    treeWidget->setSelectionBehavior(QAbstractItemView::SelectRows);


    if(QTreeWidgetItem* header = treeWidget->headerItem())
    {
        header->setText(0, header0);
        header->setText(1, header1);
        header->setText(2, header2);
    }

    // Set column with
    treeWidget->setColumnWidth(0, headerWidth0);
    treeWidget->setColumnWidth(1, headerWidth1);
    treeWidget->setColumnWidth(2, headerWidth2);

    // Hide column
    treeWidget->setColumnHidden(0, hideCol0);
    treeWidget->setColumnHidden(1, hideCol1);
    treeWidget->setColumnHidden(2, hideCol2);
}





/*******************************************************************
 *
 * Function:        setHeader3ColumnTreeWidget()
 *
 * Description:
 *
 *
 *******************************************************************/
void MyTreeWidget::addOneLineTxtColorData(QTreeWidget *treeWidget, OneLineTxtColorDataST &data)
{
    int i;

    if(data.lineData.size() != data.lineColor.size())
    {
        QMessageBox::critical(NULL, QString::fromUtf8("MyTreeWidget"), QString::fromUtf8("lineData.size() != lineColor.size()"));
        return;
    }

    if(data.lineColor.size() < 1)
    {
        QMessageBox::critical(NULL, QString::fromUtf8("MyTreeWidget"), QString::fromUtf8("lineColor.size() < 1"));
        return;
    }

    QTreeWidgetItem *item = new QTreeWidgetItem;

    if(item == 0)
    {
        QMessageBox::critical(NULL, QString::fromUtf8("myTreeWidget"),
                                 QString::fromUtf8("Error create QTreeWidgetItem"));
    }

    qDebug() << "Start";
    qDebug() << "nof Line data "<< data.lineData.size();
    qDebug() << "nof Line color "<< data.lineColor.size();



    for(i = 0; i < data.lineData.size(); i++)
    {
        item->setText(i, data.lineData.at(i).toLocal8Bit().constData());
        qDebug() << data.lineData.at(i).toLatin1().constData();
        item->setTextColor(i, data.lineColor.at(i));
        qDebug() << data.lineColor.at(i).toRgb();
    }

    treeWidget->addTopLevelItem(item);
}



#if 0
QTreeWidgetItem *item = new QTreeWidgetItem;

assetGroup = (QString)rec.value("assetGroup").toString().toLatin1();


item->setText(0, (QString)rec.value("year").toString().toUtf8());

profitDivNofShares = rec.value("netProfitAfterTax").toDouble() / rec.value("numberOfShares").toDouble();
tmp.sprintf("%.1f", (profitDivNofShares * 1000.0));
item->setText(1, tmp);

profitIncomeDiv1000 = rec.value("netProfitAfterTax").toDouble() / 1000.0;
tmp.sprintf("%.0f", profitIncomeDiv1000);
item->setText(2, tmp);


if(profitRefIsInit == true)
{
    if(refProfit > 0)
    {
        growth = (profitDivNofShares - refProfit) / refProfit;
    }
    else
    {
        double tmp = refProfit;
        tmp = -tmp;
        growth = (profitDivNofShares + refProfit) / refProfit;
    }

    tmp.sprintf("%.2f", growth*100);
    item->setText(3, tmp);

    if(growth*100 >= 10)
        item->setTextColor(3, Qt::green);
    else if(growth*100 < 10 && growth*100 >= 0)
        item->setTextColor(3, cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3));
    else
        item->setTextColor(3, Qt::red);
}
else
{
    item->setText(3, " ");
}

#endif
