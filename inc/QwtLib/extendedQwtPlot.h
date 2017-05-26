#ifndef CEXTENDEDQWTPLOT_H
#define CEXTENDEDQWTPLOT_H


#include <QtCore>
#include <QtGui>


#include <qwt_plot.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_renderer.h>
#include <qwt_picker_machine.h>
#include <qwt_symbol.h>
#include <qwt_plot_curve.h>


//=============================================================================
// Class declaration
//=============================================================================


class Zoomer: public QwtPlotZoomer
{
public:
    Zoomer( int xAxis, int yAxis, QwtPlotCanvas *canvas ):
                                                QwtPlotZoomer( xAxis, yAxis, canvas )
    {
        setTrackerMode(QwtPicker::AlwaysOff);
        setRubberBand(QwtPicker::NoRubberBand);

        // RightButton: zoom out by 1 Ctrl+RightButton: zoom out to full size
        setMousePattern( QwtEventPattern::MouseSelect2, Qt::RightButton, Qt::ControlModifier );
        setMousePattern( QwtEventPattern::MouseSelect3, Qt::RightButton );
    }
};



//=============================================================================
// Class declaration
//=============================================================================
class CExtendedQwtPlot
{
private:


protected:
    //QwtPlotZoomer *m_zoomer;
    Zoomer *m_zoomer;
    QwtPlotPicker *m_picker;
    QwtPlotPanner *m_panner;


public:
    CExtendedQwtPlot();
    void setPlotTitle(QwtPlot *qwtPlot, QString title);
    void setXAxisTitle(QwtPlot *qwtPlot, QString title, int fontSize=10);
    void setYAxisTitle(QwtPlot *qwtPlot, QString title);
    void setXAxisFontSize(QwtPlot *qwtPlot, int fontSize);
    void setYAxisFontSize(QwtPlot *qwtPlot, int fontSize);
    void saveQwtPlotAsImage(QString filename, QwtPlot *qwtPlot);
    void removeLegend( QwtPlotCurve *data);
    void setXAxisScale(QwtPlot *qwtPlot, double min, double max);
    void setYAxisScale(QwtPlot *qwtPlot, double min, double max);
    void setLegendSymbol(QwtPlot *qwtPlot, QString legendText, QwtSymbol legendSymbol, QColor legendColor, int legendSize = 10);
    void setRightLegend(QwtPlot *qwtPlot);
    void setLegend(QwtPlot *qwtPlot, QwtPlot::LegendPosition pos);
    void turnOnPlotMarker(QwtPlot *qwtPlot, QColor color = Qt::green);
    void initPlotPicker(QwtPlot *qwtPlot);
    void initPlotZoomer(QwtPlot *qwtPlot);
    void initPlotPanner(QwtPlot *qwtPlot);
    void enableZoomMode(bool on);
    void setZoomStartArea(void);
    void ZoomOut(void);
    void enablePanningMode(bool on);
    void panLeftRight(int dx);
    void setCanvasBackground(QwtPlot *qwtPlot, QColor color);
    void turnOnPlotGrid(QwtPlot *qwtPlot, QColor color, bool enableMinorXGrid,
                        bool enableMajorXGrid, bool enableMinorYGrid, bool enableMajorYGrid);

};

#endif // CEXTENDEDQWTPLOT_H
