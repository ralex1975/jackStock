#ifndef YAHOO_STOCKPLOTUTIL_H
#define YAHOO_STOCKPLOTUTIL_H

#include <QVector>
#include <QtCore>
#include <QString>
#include <QComboBox>
#include <QtGui/qcolor.h>

#include <qwt_legend.h>
#include <qwt_scale_draw.h>
#include <qwt_math.h>
#include <qwt_plot_item.h>
#include <qwt_plot_curve.h>
#include <qwt_plot.h>
#include <qwt_symbol.h>

#include <QListWidget>
#include "extendedQwtPlot.h"
#include <qwt_plot_intervalcurve.h>


class QwtPlotCurve;
class QwtPlotIntervalCurve;


class CYahooStockPlotUtil:public CExtendedQwtPlot
{
private:
     //QwtPlotIntervalCurve *d_intervalCurve;


public:
    enum {MAX_NOF_PLOT_COLORS = 13};

    static Qt::GlobalColor m_qPenArr[MAX_NOF_PLOT_COLORS];
    static QwtSymbol::Style m_coordSymArr[MAX_NOF_PLOT_COLORS];


    // This is the x, y coordinats for one stock
    struct XYPlotData_ST
    {
        QVector <double> x;
        QVector <double> y;
        QVector <QString> xDate;
        QVector <double> indicator1;            // Indicator1
        QVector <double> indicator2;            // Indicator2
        QVector <double> indicator3;            // ndicator1
        QVector <double> indicator4;            // ndicator1
        QVector <double> indicator5;
    };

    // This is all data need for one stock
    // This is a copy of the data that is keept in PlotData_ST
    struct StockData_ST
    {
        QString stockName;
        XYPlotData_ST data;
        int nofData;
    };

    struct MinMaxAxisValues_ST
    {
        bool minMaxIsInit;
        double maxY;
        double minY;

        double minX;
        double maxX;

        QString minXDate;
        QString maxXDate;

    };

    struct XYStockPlotData_ST
    {
        QwtPlotIntervalCurve intervalCurve;
        QwtPlotCurve data;
        QString name;
    };



    // This infomation is inserted into the graph
    // There is no methode to extract xy data from QwtPlotCurve
    struct PlotData_ST
    {
        int nofStocksToPlot;
        MinMaxAxisValues_ST axis;
        XYStockPlotData_ST stock[MAX_NOF_PLOT_COLORS];
    };




    void plotIntervalBarData(QwtPlot *qwtPlot,
                             QwtPlotIntervalCurve &intervalCurve,
                             const QString &title,
                             const QVector<QwtIntervalSample>& samples,
                             const QColor &color);


    CYahooStockPlotUtil();
    bool initSockNameCombo(QVector <CYahooStockPlotUtil::StockData_ST> &stockArr, QComboBox *combo);
    void removeAllStockArrData(QVector <CYahooStockPlotUtil::StockData_ST> &stockArr);
    bool addStockToPlot(CYahooStockPlotUtil::PlotData_ST &allPlotData, CYahooStockPlotUtil::StockData_ST &stock, CYahooStockPlotUtil::MinMaxAxisValues_ST &axis, int index, QString LegendLable ="");
    bool addIndicatorToPlot(CYahooStockPlotUtil::PlotData_ST &allPlotData,
                                                 CYahooStockPlotUtil::StockData_ST &stock,
                                                 CYahooStockPlotUtil::MinMaxAxisValues_ST &axis,
                                                 int indicatorIndex,
                                                 int colorIndex,
                                                 QString legendLable);
    bool plotDataSetXScaleStep(CYahooStockPlotUtil::PlotData_ST &allPlotData,
             QwtPlot *qwtPlot,
             int index,
             int xScaleStep = 0);
    bool plotYAxisLogData(CYahooStockPlotUtil::PlotData_ST &allPlotData,
             QwtPlot *qwtPlot,
             int index, int xScaleStep = 2);

    bool plotData(CYahooStockPlotUtil::PlotData_ST &allPlotData, QwtPlot *qwtPlot, int index, bool useAutoScale=false);
    bool addStockToListWidget(CYahooStockPlotUtil::StockData_ST &stock,  int index, QListWidget *listWidget);
    void updateMinMaxAxis(CYahooStockPlotUtil::MinMaxAxisValues_ST &axis, double x, double y, QString date="");
    void updateMinMaxData(CYahooStockPlotUtil::MinMaxAxisValues_ST &oldAxis, CYahooStockPlotUtil::MinMaxAxisValues_ST &newAxis);
    void updateMinMaxXAxisScales(CYahooStockPlotUtil::MinMaxAxisValues_ST &axis,
                                 CYahooStockPlotUtil::MinMaxAxisValues_ST &qwtPlotAxis);
    bool findStock(CYahooStockPlotUtil::PlotData_ST &allPlotData, QString stockName);
    bool emtypPlotData(CYahooStockPlotUtil::PlotData_ST &allPlotData);
    void removePlotData(PlotData_ST &allPlotData , int index, QwtPlot *qwtPlot);
    void panningX1(PlotData_ST &allPlotData ,bool inc, QwtPlot *qwtPlot);
    void addLastOnePlotData(StockData_ST &stockData, CYahooStockPlotUtil::PlotData_ST &qwtStockPlotData, double y, QString date="");
    void initMinMaxAxis(CYahooStockPlotUtil::MinMaxAxisValues_ST inputAxis, CYahooStockPlotUtil::MinMaxAxisValues_ST &outputAxis);
    void addOneIntervalBarData(QVector<QwtIntervalSample> &rangeData, CYahooStockPlotUtil::MinMaxAxisValues_ST &axisMinMax, double x, double minY, double maxY);
    void plotIntervalBarData(QwtPlot *qwtPlot, const QString &title, const QVector<QwtIntervalSample> &samples, const QColor &color);
    void deleteIntervalBarData(QwtPlot *qwtPlot, QwtPlotIntervalCurve &intervalCurve,
                          QVector<QwtIntervalSample>& samples);

    //void deleteAllPlotData(CYahooStockPlotUtil::PlotData_ST &allPlotData, QwtPlot *qwtPlot);



};

#endif // STOCKPLOTUTIL_H
