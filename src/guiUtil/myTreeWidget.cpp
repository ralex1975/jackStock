#include "../../inc/guiUtil/myTreeWidget.h"


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


#if 0
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
    treeWidget->setColumnCount(4);
    treeWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    treeWidget->setSelectionBehavior(QAbstractItemView::SelectRows);


    if(QTreeWidgetItem* header = treeWidget->headerItem())
    {
        header->setText(0, header0);
        header->setText(1, header1);
        header->setText(2, header2);
    }


    treeWidget->setColumnWidth(0, headerWidth0);
    treeWidget->setColumnWidth(1, headerWidth1);
    treeWidget->setColumnWidth(2, headerWidth2);
}
#endif



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


