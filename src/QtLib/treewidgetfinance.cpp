/*******************************************************************
 *
 * filename:    treewidgetfinance.cpp
 *
 * Description: This file contains treewidget functions which used
 *              together with for finance specific tasks
 *
 *******************************************************************/
#include "treewidgetfinance.h"
#include "dbHndl.h"
#include "util.h"
#include "financemath.h"


/*******************************************************************
 *
 * Function:    TreeWidgetFinance()
 *
 * Description:
 *
 *
 *
 *
 *******************************************************************/
TreeWidgetFinance::TreeWidgetFinance()
{

}



/*******************************************************************
 *
 * Function:    TreeWidgetFinance()
 *
 * Description: In TreeWidget, add 3 data.
 *              Note: this is a top level tree widget without branches.
 *
 *
 *******************************************************************/
bool TreeWidgetFinance::addTreeWidgetData(QTreeWidget *treeWidget,
                                          QString inData1,
                                          QString inData2,
                                          bool setColor)
{
    QTreeWidgetItem *item = new QTreeWidgetItem;
    bool isNumeric;
    double value;

    if(item == NULL)
    {
        return false;
    }

    item->setText(0, inData1);
    item->setText(1, inData2);

    value = inData2.toDouble(&isNumeric);

    if((isNumeric == true) && (setColor == true))
    {
        if(value >= 0)
        {
            item->setTextColor(2, Qt::darkGreen);
        }
        else
        {
            item->setTextColor(2, Qt::red);
        }
    }

    treeWidget->addTopLevelItem(item);

    return true;
}




/*******************************************************************
 *
 * Function:    TreeWidgetFinance()
 *
 * Description: In TreeWidget, add 3 data.
 *              Note: this is a top level tree widget without branches.
 *
 *
 *******************************************************************/
bool TreeWidgetFinance::addTreeWidgetData(QTreeWidget *treeWidget,
                                          QString inData1,
                                          QString inData2,
                                          QString inData3,
                                          bool setColor)
{
    QTreeWidgetItem *item = new QTreeWidgetItem;
    bool isNumeric;
    double value;

    if(item == NULL)
    {
        return false;
    }

    item->setText(0, inData1);
    item->setText(1, inData2);
    item->setText(2, inData3);

    value = inData3.toDouble(&isNumeric);

    if((isNumeric == true) && (setColor == true))
    {
        if(value >= 0)
        {
            item->setTextColor(2, Qt::darkGreen);
        }
        else
        {
            item->setTextColor(2, Qt::red);
        }
    }

    treeWidget->addTopLevelItem(item);

    return true;
}


/*******************************************************************
 *
 * Function:    TreeWidgetFinance()
 *
 * Description: In TreeWidget, add 3 data.
 *              Note: this is a top level tree widget without branches.
 *
 *
 *******************************************************************/
bool TreeWidgetFinance::addTreeWidgetData(QTreeWidget *treeWidget,
                                          QString inData1,
                                          QString inData2,
                                          QString inData3,
                                          QString inData4,
                                          bool setColor)
{
    QTreeWidgetItem *item = new QTreeWidgetItem;
    bool isNumeric;
    double value3;
    double value4;

    if(item == NULL)
    {
        return false;
    }

    item->setText(0, inData1);
    item->setText(1, inData2);
    item->setText(2, inData3);
    item->setText(3, inData4);

    value3 = inData3.toDouble(&isNumeric);

    if((isNumeric == true) && (setColor == true))
    {
        if(value3 >= 0)
        {
            item->setTextColor(2, Qt::darkGreen);
        }
        else
        {
            item->setTextColor(2, Qt::red);
        }
    }

    value4 = inData4.toDouble(&isNumeric);

    if((isNumeric == true) && (setColor == true))
    {
        if(value4 >= 0)
        {
            item->setTextColor(3, Qt::darkGreen);
        }
        else
        {
            item->setTextColor(3, Qt::red);
        }
    }

    treeWidget->addTopLevelItem(item);

    return true;
}



/*******************************************************************
 *
 * Function:    calcLeastSqrtFit()
 *
 * Description: This function traverse treewidget and calculate
 *              least square fit on the column that is selected
 *              by the parameter colToUse.
 *
 *******************************************************************/
bool TreeWidgetFinance::
calcLeastSqrtFit(QTreeWidget *treeWidget,
                 double &k,
                 double &m,
                 double &minX,
                 double &maxX,
                 int colToUse)
{
    CUtil cu;
    CDbHndl db;
    int nofData = 0;
    double meanXSum = 0;
    double meanYSum = 0;
    double prodXXSum = 0;
    double prodYYSum = 0;
    double prodXYSum = 0;
    double x;
    double y;
    bool minMaxXIsInit = false;


    db.init1dLeastSrq(nofData, meanXSum, meanYSum, prodXXSum, prodYYSum, prodXYSum);


    int nofTopItems = treeWidget->topLevelItemCount();

    if(nofTopItems < 2)
    {
        return false;
    }

    for (int i = 0 ; i < nofTopItems ; i++)
    {
        QTreeWidgetItem *item = treeWidget->topLevelItem(i);


        if(true == cu.number2double(item->text(0), x) && true == cu.number2double(item->text(colToUse), y))
        {
            //x = (double) (nofTopItems - i);
            qDebug() << "xx" << x << "yy" << y;
            db.gather1dLeastSrqData(x,
                                    y,
                                    nofData,
                                    meanXSum,
                                    meanYSum,
                                    prodXXSum,
                                    prodYYSum,
                                    prodXYSum);

            if(minMaxXIsInit == false)
            {
                minMaxXIsInit = true;
                maxX = x;
                minX = x;
            }
            else
            {
                if(minX > x)
                    minX = x;

                if(maxX < x)
                    maxX = x;
            }
        }
    }

    if(nofData < 2)
    {
        return false;
    }

    if(false == db.calc1dLeastSrqFitParams(nofData, meanXSum, meanYSum, prodXXSum, prodYYSum, prodXYSum, m, k))
    {
       return false;
    }

    qDebug() << "k" << k << "m" << m;

    return true;
}




/*******************************************************************
 *
 * Function:    addLeastSqrtFitAndGrowthRateDataToTreeWidget()
 *
 * Description: This function adds:
 *                  - year in column 0
 *                  - xxxx/shares in column 1
 *                  - growth rate of xxxx/share in column 2
 *
 *******************************************************************/
bool TreeWidgetFinance::
addLeastSqrtFitAndGrowthRateDataToTreeWidget(QTreeWidget *treeWidget,
                                             double k,
                                             double m,
                                             double maxX,
                                             int nofDataToAdd)
{
    bool lastYIsInit = false;
    int i;

    double x;
    double y;
    double lastY = 0.0;

    QString xStr;
    QString yStr;
    QTreeWidgetItem *item1;


    if(nofDataToAdd < 1)
    {
        return false;
    }

    int nofTopItems = treeWidget->topLevelItemCount();

    if(nofTopItems > 0)
    {
        item1 = treeWidget->topLevelItem(treeWidget->topLevelItemCount() - 1);
        lastY = item1->text(1).toDouble();
        lastYIsInit = true;
    }


    x = maxX + 1;
    for(i = 0; i < nofDataToAdd; i++)
    {
        y = k * x + m;

        xStr = xStr.sprintf("%.0f", x);
        yStr = yStr.sprintf("%.1f", y);

        QTreeWidgetItem *item = new QTreeWidgetItem;

        if(item == NULL)
        {
            return false;
        }

        item->setText(0, xStr);
        item->setTextColor(0, Qt::darkRed);
        item->setText(1, yStr);
        item->setTextColor(1, Qt::darkRed);

        if(lastYIsInit==true && lastY != 0)
        {
            FinanceMath fm;
            double growth;

            fm.singleEventGrowth(y, lastY,  growth);

            yStr = yStr.sprintf("%.2f", (growth * 100));
            item->setText(2, yStr);
            item->setTextColor(2, Qt::darkRed);

            qDebug() << yStr;

            if(growth >= 0)
            {
                item->setTextColor(1, Qt::darkGreen);
            }
            else
            {
                item->setTextColor(2, Qt::red);
            }
        }

        treeWidget->addTopLevelItem(item);
        lastY = y;
        lastYIsInit = true;
        x++;
    }
    return true;
}


/*******************************************************************
 *
 * Function:    getTreeWidgetData()
 *
 * Description: This function traverse treewidget
 *
 *
 *******************************************************************/
void TreeWidgetFinance::
getTreeWidgetData(QTreeWidget *treeWidget,
                  SubAnalysDataST *outDataArr,
                  int &nofOutArrData,
                  int colx,
                  int coly)
{
    CUtil cu;
    double x;
    double y;

    nofOutArrData = treeWidget->topLevelItemCount();

    qDebug() << nofOutArrData;

    for(int i = 0; i < nofOutArrData; i++)
    {
        QTreeWidgetItem *item = treeWidget->topLevelItem(i);
        if(true == cu.number2double(item->text(colx), x) && true == cu.number2double(item->text(coly), y))
        {
            qDebug() << item->text(colx);
            qDebug() << item->text(coly);

            outDataArr[i].date = item->text(colx);
            outDataArr[i].data = item->text(coly);
        }

    }
}




/*******************************************************************
 *
 * Function:    treeWidgetCalcAverage()
 *
 * Description: This function traverse treewidget and calc average
 *              of the selected column data.
 *
 *
 *******************************************************************/
bool TreeWidgetFinance::
treeWidgetCalcAverage(QTreeWidget *treeWidget,
                      double &average,
                      int coly)
{
    CUtil cu;
    double y = 0;
    int nofData = 0;

    average = 0;

    for(int i = 0; i < treeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *item = treeWidget->topLevelItem(i);
        if(true == cu.number2double(item->text(coly), y))
        {
            qDebug() << item->text(coly);
            average += y;
            nofData++;
        }
    }

    if(nofData > 0)
    {
        average = average / (double) nofData;
        return true;
    }

    return false;
}


