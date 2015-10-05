/*******************************************************************
 *
 * File:        extendedQwtPlot.cpp
 *
 * Description: This file contains basic qwt plot functions.
 *              It is used to create xyplots.
 *
 *
 *******************************************************************/


#include "extendedQwtPlot.h"
#include <qwt_plot.h>
#include <qwt_legend.h>


/*******************************************************************
 *
 * Function:
 *
 * Description:
 *
 *
 *
 *******************************************************************/
CExtendedQwtPlot::CExtendedQwtPlot()
{
}


/*******************************************************************
 *
 * Function:
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void CExtendedQwtPlot::setPlotTitle(QwtPlot *qwtPlot, QString title)
{
    qwtPlot->setTitle(title);
}


/*******************************************************************
 *
 * Function:
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void CExtendedQwtPlot::setXAxisTitle(QwtPlot *qwtPlot, QString title, int fontSize)
{
    QwtText title1(title);
    title1.setFont(QFont("Helvetica", fontSize));
    qwtPlot->setAxisTitle(QwtPlot::xBottom, title1);
}


/*******************************************************************
 *
 * Function:
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void CExtendedQwtPlot::setXAxisFontSize(QwtPlot *qwtPlot, int fontSize)
{
    qwtPlot->setAxisFont(QwtPlot::xBottom, QFont("Helvetica", fontSize));
}


/*******************************************************************
 *
 * Function:
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void CExtendedQwtPlot::setYAxisFontSize(QwtPlot *qwtPlot, int fontSize)
{
    qwtPlot->setAxisFont(QwtPlot::yLeft, QFont("Helvetica", fontSize));
}



/*******************************************************************
 *
 * Function:
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void CExtendedQwtPlot::setYAxisTitle(QwtPlot *qwtPlot, QString title)
{
    QwtText title1(title);
    title1.setFont(QFont("Helvetica", 10));
    qwtPlot->setAxisTitle(QwtPlot::yLeft, title1);
}


/*******************************************************************
 *
 * Function:
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void CExtendedQwtPlot::setXAxisScale(QwtPlot *qwtPlot, double min, double max)
{
    qwtPlot->setAxisScale(QwtPlot::xBottom, min, max);
}

/*******************************************************************
 *
 * Function:    setYAxisScale()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void CExtendedQwtPlot::setYAxisScale(QwtPlot *qwtPlot, double min, double max)
{
    qwtPlot->setAxisScale(QwtPlot::yLeft, min, max);
}


/*******************************************************************
 *
 * Function: setRightLegend()
 *
 * Description: OBS Memory leaks need to be fixed here (several places)
 *
 *
 *
 *******************************************************************/
void CExtendedQwtPlot::setRightLegend(QwtPlot *qwtPlot)
{
    QwtLegend *legend = new QwtLegend;
    legend->setItemMode( QwtLegend::ReadOnlyItem );
    legend->setFont(QFont("Helvetica", 10));
    qwtPlot->insertLegend(legend, /*QwtPlot::TopLegend*/ /*QwtPlot::BottomLegend*/ QwtPlot::RightLegend );
}


/*******************************************************************
 *
 * Function: setRightLegend()
 *
 * Description: OBS Memory leaks need to be fixed here (several places)
 *
 * pos = QwtPlot::TopLegend
 *       QwtPlot::BottomLegend
 *       QwtPlot::RightLegend
 *
 *******************************************************************/
void CExtendedQwtPlot::setLegend(QwtPlot *qwtPlot, QwtPlot::LegendPosition pos, QString str)
{
    QwtLegend *legend = new QwtLegend;
    legend->setItemMode( QwtLegend::ReadOnlyItem );
    legend->setFont(QFont("Helvetica", 10));
    qwtPlot->insertLegend(legend, pos);
}





/*******************************************************************
 *
 * Function:        initPlotPicker()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void CExtendedQwtPlot::initPlotPicker(QwtPlot *qwtPlot)
{
    m_picker = new QwtPlotPicker( QwtPlot::xBottom, QwtPlot::yLeft, QwtPlotPicker::CrossRubberBand, QwtPicker::AlwaysOn, qwtPlot->canvas() );
    m_picker->setStateMachine( new QwtPickerDragPointMachine() );
    m_picker->setRubberBandPen( QColor( Qt::green ) );
    m_picker->setRubberBand( QwtPicker::CrossRubberBand );
    m_picker->setTrackerPen( QColor( Qt::red ) );
}


/*******************************************************************
 *
 * Function:        initPlotZoomer()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void CExtendedQwtPlot::initPlotZoomer(QwtPlot *qwtPlot)
{
    m_zoomer = new Zoomer( QwtPlot::xBottom, QwtPlot::yLeft, qwtPlot->canvas() );
    m_zoomer->setRubberBand( QwtPicker::RectRubberBand );
    m_zoomer->setRubberBandPen( QColor( Qt::green ) );
    m_zoomer->setTrackerMode( QwtPicker::ActiveOnly );
    m_zoomer->setTrackerPen( QColor( Qt::black ) );
}




/*******************************************************************
 *
 * Function:    panLeftRight()
 *
 * Description:
 *
 *
 *******************************************************************/
void CExtendedQwtPlot::panLeftRight(int dx)
{
        m_zoomer->moveBy(dx, 0);
}


/*******************************************************************
 *
 * Function:    setZoomStartArea()
 *
 * Description: This function is used to gather starting area of
 *              zommer. This function shall be invoked after data
 *              has been plotted.
 *
 *******************************************************************/
void CExtendedQwtPlot::setZoomStartArea(void)
{
    m_zoomer->setZoomBase();
}



/*******************************************************************
 *
 * Function:        ZoomOut()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void CExtendedQwtPlot::ZoomOut(void)
{
    uint index;

    m_panner->setEnabled(false);
    index = m_zoomer->zoomRectIndex();

    if(index > 1)
    {
        index--;
    }
    else
    {
        index = 0;
    }

    m_zoomer->setZoomStack(m_zoomer->zoomStack(), index);
}


/*******************************************************************
 *
 * Function:    initPlotPanner()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void CExtendedQwtPlot::initPlotPanner(QwtPlot *qwtPlot)
{
  m_panner = new QwtPlotPanner(qwtPlot->canvas());
  m_panner->setMouseButton( Qt::LeftButton );
  m_panner->setCursor(QCursor(Qt::ClosedHandCursor));

}


/*******************************************************************
 *
 * Function:        enableZoomMode()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void CExtendedQwtPlot::enableZoomMode( bool on )
{
    m_zoomer->setEnabled(on);
    m_picker->setEnabled(false);
}


/*******************************************************************
 *
 * Function:    enablePanningMode()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void CExtendedQwtPlot::enablePanningMode(bool on)
{
    m_panner->setEnabled(on);
    m_zoomer->setEnabled(false);
}


