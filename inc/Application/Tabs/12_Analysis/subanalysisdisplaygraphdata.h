#ifndef SUBANALYSISDISPLAYGRAPHDATA_H
#define SUBANALYSISDISPLAYGRAPHDATA_H

#include <qwt_plot_histogram.h>
#include <qwt_plot_marker.h>
#include <dbHndl.h>


#define NOF_QWT_PLOTS 10
#define MAX_NOF_QWT_MARKERS_IN_EACH_PLOT 100

class subAnalysisDisplayGraphData
{

    QwtPlotHistogram *m_barHist[NOF_QWT_PLOTS];
    QwtPlotMarker *m_mark[NOF_QWT_PLOTS][MAX_NOF_QWT_MARKERS_IN_EACH_PLOT];
    QVector <QwtIntervalSample> m_barHistData[NOF_QWT_PLOTS];

    void plotBarGraph(int graphIndex, int nofArrData, SubAnalysDataST *coverageRatioArr, QwtPlot *qwtPlot[NOF_QWT_PLOTS]);
    void subAnalysisCalcRation(SubAnalysDataST *numeratorArr,   // Top of ratio
                               int nofNumeratorArrData,
                               SubAnalysDataST *denominatorArr, // Bottom of ratio
                               int nofDenominatorData,
                               SubAnalysDataST *resultArr,
                               int &nofResultArrData);


    void subAnalysisGetCompanyType(HtmlStockAnalysPageDataST data, int &companyType);
    void subAnalysisPlotGraphicDataIndustrialComp(HtmlStockAnalysPageDataST data, QwtPlot *qwtPlot[NOF_QWT_PLOTS]);
    void subAnalysisPlotGraphicDataTradingComp(HtmlStockAnalysPageDataST data, QwtPlot *qwtPlot[NOF_QWT_PLOTS]);
    void subAnalysisPlotGraphicDataUtilitisComp(HtmlStockAnalysPageDataST data, QwtPlot *qwtPlot[NOF_QWT_PLOTS]);
    void subAnalysisPlotGraphicDataRealEstateComp(HtmlStockAnalysPageDataST data, QwtPlot *qwtPlot[NOF_QWT_PLOTS]);
    void subAnalysisPlotGraphicDataBankComp(HtmlStockAnalysPageDataST data, QwtPlot *qwtPlot[NOF_QWT_PLOTS]);








public:
    subAnalysisDisplayGraphData();
    ~subAnalysisDisplayGraphData();
    void subAnalysisShowDataInGraphs(HtmlStockAnalysPageDataST data, QwtPlot *qwtPlot[NOF_QWT_PLOTS]);
    void subAnalysisClearAllGraphs(QwtPlot *qwtPlot[NOF_QWT_PLOTS]);
    void subAnalysisOneArrCalcRationPrevCurrSlot(SubAnalysDataST *inputArr,
                                                 int nofInputArrData,
                                                 SubAnalysDataST *resultArr,
                                                 int &nofResultArrData,
                                                 bool skipWhenDenominatorEqZero = true);

    void subAnalysisOneArrCalcProcentRationPrevCurrSlot(SubAnalysDataST *inputArr,
                                            int nofInputArrData,
                                            SubAnalysDataST *resultArr,
                                            int &nofResultArrData,
                                            bool skipWhenDenominatorEqZero = true);


};

#endif // SUBANALYSISDISPLAYGRAPHDATA_H
