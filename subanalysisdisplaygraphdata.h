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



public:
    subAnalysisDisplayGraphData();
    ~subAnalysisDisplayGraphData();
    void subAnalysisShowDataInGraphs(HtmlStockAnalysPageDataST data, QwtPlot *qwtPlot[NOF_QWT_PLOTS]);
};

#endif // SUBANALYSISDISPLAYGRAPHDATA_H
