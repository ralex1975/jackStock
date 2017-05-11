#ifndef FATAB_H
#define FATAB_H

#include <QDialog>
#include <QString>
#include <QtCore>
#include <QtGui>
#include "common.h"
#include "yahooStockPlotUtil.h"
#include "dbHndl.h"

namespace Ui
{
    class FaTab;
}

class FaTab : public QDialog
{
    Q_OBJECT

    enum FAData_ET
    {
        FA_PE,
        FA_PS,
        FA_ERNING_DIV_DIVIDEN, // Vinst / utdelning
        FA_NAV_DIV_LAST_PRICE, // Substans / Kurs
        FA_YIELD,
        FA_MEAN_RETURN,
        FA_MEAN_RISK,
        FA_NOF_DATA
    };

    enum DataPlotIndex_ET
    {
        PLOT_INDEX_PRICE,
        PLOT_INDEX_SHORT_MOVING_AVG,
        PLOT_INDEX_MID_MOVING_AVG,
        PLOT_INDEX_LONG_MOVING_AVG,
        PLOT_INDEX_BOLLINGERBAND_LOW,
        PLOT_INDEX_BOLLINGERBAND_MID,
        PLOT_INDEX_BOLLINGERBAND_UPPER,
        PLOT_INDEX_LINJER_REG,
        PLOT_INDEX_GREEN_MOUSE_LINE,
        PLOT_INDEX_RED_MOUSE_LINE,
        PLOT_INDEX_MAX
    };


    enum StockListColName_ET
    {
        STOCKLIST_NAME = 0,
        STOCKLIST_SYMBOL = 1
    };

    QPalette *m_faDataPalette[FA_NOF_DATA];
    QString m_reqStockSymbol;
    QString m_reqStockName;

    // Is able to store a number of stocks and all its data. Data can be retrived.
    QVector <CYahooStockPlotUtil::StockData_ST> m_stockArr;

    // Is able to store one stock with data
    CYahooStockPlotUtil::StockData_ST m_stockData;
    CYahooStockPlotUtil::StockData_ST m_stockData1;
    CYahooStockPlotUtil::StockData_ST m_stockData2;
    CYahooStockPlotUtil::StockData_ST m_stockData3;
    CYahooStockPlotUtil::StockData_ST m_tmpData;

    CYahooStockPlotUtil::StockData_ST m_salesStockData;
    CYahooStockPlotUtil::StockData_ST m_profitStockData;

    QwtPlotIntervalCurve m_intervalCurvePrice;
    QwtPlotIntervalCurve m_intervalCurvePE;


    CYahooStockPlotUtil m_plot;
    CYahooStockPlotUtil::PlotData_ST m_qwtHistorcalStockPrices;
    CYahooStockPlotUtil::PlotData_ST m_qwtHistoricalPENum;
    CYahooStockPlotUtil::PlotData_ST m_qwtSalesGrowth;
    CYahooStockPlotUtil::PlotData_ST m_qwtProfitGrowth;
    CYahooStockPlotUtil::PlotData_ST m_ProfitMargin;
    CYahooStockPlotUtil::PlotData_ST m_qwtSalesAndProfitGrowth;
    CYahooStockPlotUtil::PlotData_ST m_qwtPePrice;
    CYahooStockPlotUtil::PlotData_ST m_qwtProfitAnalysis;

    QVector<QwtIntervalSample> m_histMinMaxPE;
    QVector<QwtIntervalSample> m_histMinMaxPrices;

    CYahooStockPlotUtil::MinMaxAxisValues_ST m_histPricesAxisMinMaxVal;
    CYahooStockPlotUtil::MinMaxAxisValues_ST m_histPEAxisMinMaxVal;


    void initStockList(void);
    void initOperatingIncomeList(void);
    void deleteAllPlotData(CYahooStockPlotUtil::PlotData_ST &allPlotData, QwtPlot *qwtPlot);
    void getSelStockListItem(QString &stockName, QString &stockSymbol, const QModelIndex &index);
    void getTreeWidgetData(CYahooStockPlotUtil::MinMaxAxisValues_ST &axis, QTreeWidget *treeWidget, CYahooStockPlotUtil::StockData_ST &data, int colx, int coly, int indicator);
    void setAxisTitles(QString titleXAxis, QString titleYAxis, QwtPlot *qwtPlot,bool showXAxis=false);
    void setFundametalAnalysisCtrlTxtColor(CDbHndl::snapshotStockData_ST keyData,QString assetType);
    void setFaEditControlTxtColor(QLineEdit *lineEdit, QPalette *palette, const QColor &color);
    bool calcLeastSqrtFit(QTreeWidget *treeWidget, double &k, double &m, double &minX, double &maxX, int colToUse);
    bool createLeastSqrtFitPlotData(double k, double m, double minX, double maxX,
                                    CYahooStockPlotUtil::MinMaxAxisValues_ST &axis,
                                    CYahooStockPlotUtil::StockData_ST &data,
                                    int indicator);
    void displayLeastSqrtFitData(CYahooStockPlotUtil::StockData_ST stockData,
                                 CYahooStockPlotUtil::PlotData_ST &qwtPlotData,
                                 QwtPlot *qwtPlot,
                                 int indicator,
                                 DataPlotIndex_ET plotIndex);
    void clearStockAndIndicatorTempMem(void);
    void updateMinMaxXAxisScales(CYahooStockPlotUtil::MinMaxAxisValues_ST &axis, CYahooStockPlotUtil::MinMaxAxisValues_ST &qwtPlotAxis);
    void addSalesOrProfitGrowthPrognosData(QTreeWidget *treeWidget,
                                           double k,
                                           double m,
                                           double k1,
                                           double m1,
                                           double maxX);
    bool SalesOrProfitGrowthCalcAverage(QTreeWidget *treeWidget, QString result[], int maxCol, double &growthRate);
    void displayProfitGrowth(QTreeWidget *treeWidget, int coly);
    void initNetProfitAfterTaxList(void);
    void initProfitabilityAnalysis(void);
    void displayProfitAnalysis(QTreeWidget *treeWidget, QwtPlot *qwtPlot, int coly);
    void initMinMaxPePrice(void);
    void displayHistorcalStockPrices(QTreeWidget *treeWidget, int coly, int indicator);
    bool getHistoricalMinMaxPrices(QTreeWidget *treeWidget,
                                   QVector<QwtIntervalSample> &rangeData,
                                   CYahooStockPlotUtil::MinMaxAxisValues_ST &axisMinMax);
    void displayTreeWidgetDataInQwtPlot(QTreeWidget *treeWidget,
                            CYahooStockPlotUtil::StockData_ST &stockData,
                            CYahooStockPlotUtil::PlotData_ST &plotData,
                            QwtPlot *qwtPlot,
                            int treeWidgetYCol,
                            int indicator=0,
                            DataPlotIndex_ET plotIndex = (DataPlotIndex_ET) 0);

    
public:
    explicit FaTab(QWidget *parent = 0);
    ~FaTab();
    
private slots:
    void on_SelStockListButton_clicked();
    void on_treeWidgetStockList_doubleClicked(const QModelIndex &index);
    void on_pushButNext1_clicked();
    void on_pushButNextTab_2_clicked();
    void on_pushButNextTabPg3_clicked();

    void on_pushButtonSaveImage4_clicked();

private:
    Ui::FaTab *ui;
};

#endif // FATAB_H
