#ifndef TREEWIDGETFINANCE_H
#define TREEWIDGETFINANCE_H
#include <QTreeWidget>
#include "dbHndl.h"

class TreeWidgetFinance
{
public:
    TreeWidgetFinance();
    bool calcLeastSqrtFit(QTreeWidget *treeWidget, double &k, double &m, double &minX, double &maxX, int colToUse);
    bool addTreeWidgetData(QTreeWidget *treeWidget,QString inData1, QString inData2, QString inData3, bool setColor = false);
    bool addTreeWidgetData(QTreeWidget *treeWidget, QString inData1, QString inData2, QString inData3, QString inData4, bool setColor = false);

    bool addLeastSqrtFitAndGrowthRateDataToTreeWidget(QTreeWidget *treeWidget, double k, double m, double maxX, int nofDataToAdd);
    void getTreeWidgetData(QTreeWidget *treeWidget, SubAnalysDataST *outDataArr, int &nofOutArrData, int colx, int coly);
    bool treeWidgetCalcAverage(QTreeWidget *treeWidget, double &average, int coly);

};

#endif // TREEWIDGETFINANCE_H
