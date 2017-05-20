#ifndef STOCKPLOTUTIL_H
#define STOCKPLOTUTIL_H

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



class CStockPlotUtil
{
public:
    enum {MAX_NOF_PLOT_COLORS = 13};

    static Qt::GlobalColor m_qPenArr[MAX_NOF_PLOT_COLORS];
    static QwtSymbol::Style m_coordSymArr[MAX_NOF_PLOT_COLORS];


    // This is the x, y coordinats for one stock
    struct XYPlotData_ST
    {
        QVector <double> x;
        QVector <double> y;
    };

    // This is all data need for one stock
    // This is a copy of the data that is keept in PlotData_ST
    struct StockData_ST
    {
        QString stockName;
        QString stockSymbol;
        XYPlotData_ST data;
        //int nofData;
    };

    struct MinMaxAxisValues_ST
    {
        bool minMaxIsInit;
        double minY;
        double minX;
        double maxY;
        double maxX;
    };

    struct XYStockPlotData_ST
    {
        QwtPlotCurve data;
        QString name;
    };



    // This infomation is inseted in to graph
    // There is no methode to extrax xy data from QwtPlotCurve
    struct PlotData_ST
    {
        int nofStocksToPlot;
        MinMaxAxisValues_ST axis;
        XYStockPlotData_ST stock[MAX_NOF_PLOT_COLORS];
    };



    CStockPlotUtil();
    bool initSockNameCombo(QVector <CStockPlotUtil::StockData_ST> &stockArr, QComboBox *combo);
    void removeAllStockArrData(QVector <CStockPlotUtil::StockData_ST> &stockArr);
    bool addStockToPlot(PlotData_ST &allPlotData, CStockPlotUtil::StockData_ST &stock, CStockPlotUtil::MinMaxAxisValues_ST &axis, int index);
    bool plotData(PlotData_ST &allPlotData, QwtPlot *qwtPlot, int index);
    bool addStockToListWidget(CStockPlotUtil::StockData_ST &stock,  int index, QListWidget *listWidget);
    void updateMinMaxData(CStockPlotUtil::MinMaxAxisValues_ST &oldAxis, CStockPlotUtil::MinMaxAxisValues_ST &newAxis);
    //bool removePlotFromStock(PlotData_ST &allPlotData, QString stockName, int &removedIndex);
    bool findStock(PlotData_ST &allPlotData, QString stockName);
    bool emtypPlotData(CStockPlotUtil::PlotData_ST &allPlotData);


};

#endif // STOCKPLOTUTIL_H
