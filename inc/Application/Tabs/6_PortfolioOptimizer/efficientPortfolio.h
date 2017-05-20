#ifndef EFFICIENTPORTFOLIO_H
#define EFFICIENTPORTFOLIO_H

#include <QDialog>
#include "dbHndl.h"
#include "common.h"
#include "extendedTable.h"
#include "threadcalcefficentfront.h"
#include "efficentbargraphdlg.h"
#include "yahooStockPlotUtil.h"
#include <qwt_plot_marker.h>
#include <qwt_plot_grid.h>

namespace Ui {
class EfficientPortfolio;
}

class EfficientPortfolio : public QDialog
{
    Q_OBJECT

public:

private:
    Ui::EfficientPortfolio *ui;

private:



    struct MinMaxAxisValues_ST
    {
        bool minMaxIsInit;
        double maxY;
        double minY;

        double minX;
        double maxX;
    };

    CYahooStockPlotUtil m_plot;
    MinMaxAxisValues_ST m_minMaxReturPlotArr;
    QwtPlotCurve *m_riskReturPlotArr;
    QwtPlotMarker *m_riskReturTxtArr;
    QwtPlotCurve *m_xAxisPlot;
    QwtPlotGrid *m_plotGrid;
    int m_nofRiskReturPlotData;

    QwtPlotMarker *m_plotflashLabel;





    int m_comboBoxIndex;
    CDbHndl m_db;
    QList <NameKey> m_dateIsUpdatedList;
    QHeaderView *m_CorrelTableHorizHeader;
    efficentBarGraphDlg m_efficentBarGraphDlg;

    threadCalcEfficentFront *m_threadCalcEfficentFront;
    CExtendedTable m_extendedtable;
    double **m_varCovMatrix;
    double **m_correlationMatrix;
    double **m_weightCorrResultMatrix;
    double *m_expectedReturnArr;
    double *m_expectedRiskArr;
    double *m_weightArr;
    CDbHndl::EfficPortStockData_ST *m_stockArr;
    QVector <CDbHndl::EfficPortStockData_ST> m_stockArr1;
    double m_riskFreeRate;
    double m_portfolioReturns;
    double m_portfolioRiskStdDev;

    bool createArrMemSpace(int nofStocks, CDbHndl::EfficPortStockData_ST **arr);
    void initDateCtrls(void);
    void initStockList(void);

    void openUserStockListsFile(void);
    bool openStockListFile(QString filename);
    bool openStockNameFile(QString filename, QString stockListName);
    void addDataToStockListAndStockListCombo(void);
    void addStatusDateList(QString stockSymbol, bool isUpdate);
    void uppdateStockArrWithRiskAndReturn(void);
    void plotQwtData(int nofCurves, QwtPlot *qwtPlot);
    void initCorrelationTable(void);
    void setTableWidth(int nofColumns);
    void addCorrelationDataToTable(void);

    bool createQwtPlotArrMemSpace(int nofStocks);
    void deleteQwtPlotData(int nofStocks);
    void removeQwtPlotArrMemSpace(void);
    void updateMinMax(double x, double y);
    void enterPlotLabel(int index, double x, double y, QwtPlot *qwtPlot);
    void plotXAxis(QwtPlot *qwtPlot);
    void flashLabelOnPlot(int index, double x, double y, QwtPlot *qwtPlot);
    void getRiskReturFromTreeWidget(double &risk, double &meanReturn, const QModelIndex &index);
    void copyTreeWidgetDataToTmpDb(CDbHndl &db, bool dbIsHandledExternly=false);







public:
    explicit EfficientPortfolio(QWidget *parent = 0);
    ~EfficientPortfolio();

    bool createMemSpace(int nofStocks);



private slots:
    void on_pushButtSel_clicked();
    void slotHorHeaderClicked(int index);
    void slotVertHeaderClicked(int index);

    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButtCalc_clicked();
    void mainTraversTreeWidget(void);
    void processItem(QTreeWidgetItem * parent, int level);

   // void updateitem ( QTreeWidgetItem* item ,int col);

    void on_StockListComboBox_currentIndexChanged(int index);

    void on_treeWidget_doubleClicked(const QModelIndex &index);
    void slotTreeHeaderDoubleClick(int);


};

#endif // EFFICIENTPORTFOLIO_H
