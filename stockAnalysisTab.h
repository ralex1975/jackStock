#ifndef STOCKANALYSISTAB_H
#define STOCKANALYSISTAB_H

#include <qwt_plot_histogram.h>
#include <qwt_plot_marker.h>
#include <QDialog>
#include "inc/guiUtil/guiFinanceCtrls.h"
#include "dbHndl.h"
#include "subanalysisdisplaygraphdata.h"
#include "stockPlotUtil.h"
#include "calcavgannualgrowthrateequity.h"



namespace Ui
{
    class StockAnalysisTab;
}


#define NOF_QWT_PLOTS 10
#define MAX_NOF_QWT_MARKERS_IN_EACH_PLOT 100





class StockAnalysisTab : public QDialog
{
    Q_OBJECT

    QString m_fildata;
    QPalette *m_red_palette;
    QPalette *m_blue_palette;

    //QwtPlotHistogram *m_barHistArr[NOF_QWT_PLOTS];
    //QwtPlotMarker *m_mark[NOF_QWT_PLOTS][MAX_NOF_QWT_MARKERS_IN_EACH_PLOT];
   // QVector <QwtIntervalSample> m_barHistDataArr[NOF_QWT_PLOTS];



    QwtPlot *m_qwtPlot[NOF_QWT_PLOTS];
    subAnalysisDisplayGraphData m_saDisply;


    QwtPlotHistogram *m_barHist;
    QVector <QwtIntervalSample> m_barHistData;



    SubAnalysDataST       m_totEarningsDataArr[MAX_NOF_TOT_EARNINGS];
    int                   m_nofTotEarningsArrData;

    SubAnalysDataST       m_revenueDataArr[MAX_NOF_REVENUE];
    int                   m_nofRevenueArrData;

    SubAnalysDataST       m_dividendDataArr[MAX_NOF_DIVIDEND_ARR_DATA];
    int                   m_nofDividendArrData;

    SubAnalysDataST       m_earningsDataArr[MAX_NOF_EARNINGS_ARR_DATA];
    int                   m_nofEarningsArrData;

    SubAnalysDataST       m_totalCurrentAssetsArr[MAX_NOF_TOTAL_CURRENT_ASSETS_ARR_DATA];
    int                   m_nofTotalCurrentAssetsArrData;

    SubAnalysDataST       m_totalCurrentLiabilitiesArr[MAX_NOF_TOTAL_CURRENT_LIABILITIES];
    int                   m_nofTotalCurrentLiabilitiesData;

    SubAnalysDataST       m_totalLiabilitiesArr[MAX_NOF_TOTAL_CURRENT_LIABILITIES];
    int                   m_nofTotalLiabilitiesData;

    SubAnalysDataST       m_solidityArr[MAX_NOF_SOLIDITY];
    int                   m_nofSolidityData;

    SubAnalysDataST       m_coverageRatioArr[MAX_NOF_COVERAGE_RATIO];
    int                   m_nofCoverageRatioData;

    SubAnalysDataST       m_coreTier1RatioArr[MAX_NOF_CORE_TIER_1_RATIO];
    int                   m_nofCoreTier1RatioData;

    SubAnalysDataST       m_coreCapitalRatioArr[MAX_NOF_CORE_CAPITAL_RATIO];
    int                   m_nofCoreCapitalRatioData;

    SubAnalysDataST       m_totEquityArr[MAX_NOF_EQUITY];
    int                   m_nofTotEquityData;

    SubAnalysDataST       m_equityPerShareArr[MAX_NOF_EQUITY_PER_SHARE];
    int                   m_nofEquityPerShareData;

    SubAnalysDataST       m_cashFlowCapexArr[MAX_NOF_CASH_FLOW_CAPEX];
    int                   m_nofCashFlowCapexData;

    SubAnalysDataST       m_operatingCashFlowArr[MAX_NOF_CASH_FLOW_CAPEX];
    int                   m_nofOperatingCashFlowData;

    SubAnalysDataST       m_totDividensArr[MAX_NOF_TOT_DIVIDENT];
    int                   m_nofTotDividensData;


    // We using stock plot functions here even when data is cash flow
    CStockPlotUtil::PlotData_ST m_qwtcashFlowPlotData;

    void plotCashflowData(void);



    GuiFinanceCtrls m_gfc;
    QString m_stockName;
    bool m_stockNameIsInit;
    QString m_stockSymbol;
    QString m_analysisDate;

    QString  m_riskStdDev;
    QString  m_meanReturns;
    QString m_returnOnEquity;

    QString m_stockListName;
    int m_stockListId;


    void resetGuiCtrl(void);
    void resetStockAnalysisData(HtmlStockAnalysPageDataST &hSAPData);
    void initSubAnalysisPlots(void);


    void initSubAnalyseTableWidget(QTableWidget *tableWidget,
                                   QString dateHeader,
                                   QString dataHeader);



    void updateTableWithSubAnalysData(QTableWidget *tableWidget,
                                      SubAnalyseDataTypeET analyseType,
                                      SubAnalysDataST *subAnalysDataArr,
                                      int &nofArrData);

    bool insertTableWidgetIntrinsicValueTotCurrAssetDivTotLiabilities(QTableWidget *tableWidget,
                                                                      SubAnalysDataST *totalCurrentAssetArr,
                                                                      int nofTotalCurrentAssetArrData,
                                                                      SubAnalysDataST *totalLiabilities,
                                                                      int nofTotalLiabilitiesArrData);


    bool calcLeastSqrtFit(SubAnalysDataST *dataArr,
                          int nofArrData,
                          double &k,
                          double &m,
                          double &minX,
                          double &maxX);


    void initCompanyTypeComboBox(void);

    void writeHtmlArrDataToTxtFile(void);




public:
    static const QString m_companyTypesArr[NOF_COMPANY_TYPE_ARR_DATA];


    explicit StockAnalysisTab(QWidget *parent = 0);
    ~StockAnalysisTab();

    void initSubAnalysTables(void);


private slots:
    void on_SelStockListButton_clicked();

    void on_treeWidgetStockListAnalysis_doubleClicked(const QModelIndex &index);

    void on_pushButton_clicked();

    void on_treeWidgetAnalysisDate_doubleClicked(const QModelIndex &index);

    void on_pushButtonRemoveRecord_clicked();

    void on_pushSaveDividend_clicked();

    void on_pushSaveEarnings_clicked();

    void on_pushButtonSaveTotalCurrentAsset_clicked();

    void on_pushButtonSaveTotalCurrentLiabilities_clicked();

    void on_pushButtonSaveTotalLiabilities_clicked();

    void on_pushButtonSelectSaveCompanyType_clicked();

    void on_pushButtonSaveSolidity_clicked();

    void on_pushButtonSaveCoverageRatio_clicked();

    void on_pushButtonSaveCoreTier1Ratio_clicked();

    void on_pushButtonSaveCoreCapitalRatio_clicked();

    void on_pushButtonSaveEquity_clicked();

    void on_pushButtonSaveCashFlowCapex_clicked();

    void on_pushButtonSaveOperatingCashFlow_clicked();

    void on_pushButtonSaveTotDividends_clicked();

    void on_pushButtonSaveRevenue_clicked();

    void on_pushButtonSaveNetIncome_clicked();

    void on_pushButtonSaveEquityPerShare_clicked();

    void on_pushButtonCalcIntrinsicValue_clicked();

    void on_pushButtonCalcYearlyIntrestRateOnEquity_clicked();

    void plotEquityPerShareData(SubAnalysDataST *dataArr, int nofData, bool plotPrediction = false);

    void on_pushSaveIValueDividend_clicked();

    void on_pushButtonAltCalcAvgAnnualGrowthRateEquity_clicked();

    void on_pushButtonAltcalcAvgAnnualGrowthRateEquity_clicked();

    void on_pushButtonSaveImg_2_clicked();


private:
    Ui::StockAnalysisTab *ui;
    calcAvgAnnualGrowthRateEquity calcAvgAnnualGrowthRateEquity_dlg;

    // Cannot extract x,y data. Contains all data that is send to graph
    CYahooStockPlotUtil::PlotData_ST m_qwtEquityPlotData;

    // Cannot extract x,y data. Contains all data that is send to graph
    CYahooStockPlotUtil::PlotData_ST m_qwtAllAnalysisPlotData;

    double m_x[1000];
    double m_y[1000];

    void getminMaxLogScale(double minIn, double &minOut, double maxIn, double &maxOut);
    void clearGUIIntrinsicValue(void);
    void calcTotSubdataForIntrinsicValue(void);


    void initPlotLinearReportData(QwtPlot *qwtPlot,
                                  QString plotHeader,
                                  QColor canvasColor,
                                  QString legendText,
                                  QwtSymbol legendSymbol,
                                  QColor legendColor,
                                  QwtPlot::LegendPosition location,
                                  int legendSize = 10);

    void plotLinearReportData(QwtPlot *qwtPlot,
                              bool useAutoScale,
                              SubAnalysDataST *dataArr,
                              int nofData,
                              int indexToPlot,
                              int nofPlotToClear,
                              QColor LineColor);

    void plotBarGraphReportData(QwtPlot *qwtPlot,
                                bool useAutoScale,
                                SubAnalysDataST *dataArr,
                                int nofData,
                                int indexToPlot,
                                int nofPlotToClear,
                                QColor lineColor);

#if 0
    void clearBarGraph(void);

    void plotBarGraph(int graphIndex,
                      int nofArrData,
                      SubAnalysDataST *subAnalysDataArr,
                      QwtPlot *qwtPlot[NOF_QWT_PLOTS]);
#endif

    void savePlotLinearReportData(QwtPlot *qwtPlot,
                                  QString stockName,
                                  QString date);


    void initAllAnalysisPlots(void);
    void displayAllAnalysisPlots(void);
    void subAnalysisCalcQuotient(SubAnalysDataST *resultArr, int &nofDataResultArr,
                                 SubAnalysDataST *numeratorArr, int nofDataNumeratorArr,
                                 SubAnalysDataST *denominatorArr, int nofDataDenominatorArr);







};

#endif // STOCKANALYSISTAB_H
