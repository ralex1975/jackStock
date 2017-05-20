/********************************************************************************
 *
 * Filename: stockPlotUtil.cpp
 *
 * Description: Plots xy data in qwt graph.
 *
 *
 *******************************************************************************/


#include "stockPlotUtil.h"
#include <QtCore>
#include <QMessageBox>
#include <qwt_plot_marker.h>
#include <qwt_symbol.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_picker.h>
#include <qwt_scale_engine.h>


QwtSymbol::Style CStockPlotUtil::m_coordSymArr[MAX_NOF_PLOT_COLORS]=
{
    //! Ellipse or circle
    QwtSymbol::Ellipse,

    //! Rectangle
    QwtSymbol::Rect,

    //!  Diamond
    QwtSymbol::Diamond,

    //! Triangle pointing upwards
    QwtSymbol::Triangle,

    //! Triangle pointing downwards
    QwtSymbol::DTriangle,

    //! Triangle pointing upwards
    QwtSymbol::UTriangle,

    //! Triangle pointing left
    QwtSymbol::LTriangle,

    //! Triangle pointing right
    QwtSymbol::RTriangle,

    //! Cross (+)
    QwtSymbol::Cross,

    //! Diagonal cross (X)
    QwtSymbol::XCross,

    //! Horizontal line
    QwtSymbol::HLine,

    //! Vertical line
    //QwtSymbol::VLine,

    //! X combined with +
   // QwtSymbol::Star1,

    //! Six-pointed star
    QwtSymbol::Star2,

    //! Hexagon
    QwtSymbol::Hexagon,
};



Qt::GlobalColor CStockPlotUtil::m_qPenArr[MAX_NOF_PLOT_COLORS] =
{
        Qt::black,
       // Qt::darkGray,
       // Qt::gray,
        Qt::red,  // Bra
        Qt::green,
        Qt::blue,  // Bra
        Qt::cyan,  // Bra
        Qt::magenta,
        Qt::yellow,
        Qt::darkRed,
        Qt::darkGreen, // bra
        Qt::darkBlue,
        Qt::darkCyan,
        Qt::darkMagenta,
        Qt::darkYellow
    };


/*****************************************************************
 *
 * Function:		CStockPlotUtil()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
CStockPlotUtil::CStockPlotUtil()
{

}




/*****************************************************************
 *
 * Function:		initSockNameCombo()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
bool CStockPlotUtil::initSockNameCombo(QVector <CStockPlotUtil::StockData_ST> &stockArr, QComboBox *combo)
{
    if(stockArr.count() < 1)
        return false;

    for(int i = 0; i <  stockArr.count(); i++)
    {
        combo->addItem(stockArr[i].stockName);
    }

    return true;

}



/*****************************************************************
 *
 * Function:		removeAllStockArrData()
 *
 * Description:		This function
 *
 *
 *
 *****************************************************************/
void CStockPlotUtil::removeAllStockArrData(QVector <CStockPlotUtil::StockData_ST> &stockArr)
{
        for( int i = 0; i < stockArr.count(); i++)
        {
            stockArr[i].data.x.clear();
            stockArr[i].data.y.clear();
        }
        stockArr.clear();
}






/*****************************************************************
 *
 * Function:		addStockToPlot()
 *
 * Description:		This function
 *
 *
 *
 *****************************************************************/
bool CStockPlotUtil::addStockToPlot(PlotData_ST &allPlotData,
                                    CStockPlotUtil::StockData_ST &stock,
                                    CStockPlotUtil::MinMaxAxisValues_ST &axis,
                                    int index)
{
    int i;
    QString str;
    double *x = new double [stock.data.x.count() + 5];
    double *y = new double [stock.data.x.count() + 5];


    if(index >= MAX_NOF_PLOT_COLORS)
    {
        str.sprintf("Kan inte visa fler grafer (Max = %d)", MAX_NOF_PLOT_COLORS);
        QMessageBox::information(NULL, QString::fromUtf8("L?gga till graf"), str);
        return false;
    }



   // Set color
    for(i = 0; i < stock.data.x.count(); i++)
    {
        x[i] = stock.data.x[i];
        y[i] = stock.data.y[i];
        qDebug() << x[i];
        qDebug() << y[i];
    }

    allPlotData.stock[index].name = stock.stockName;
    allPlotData.stock[index].data.setSamples(x, y, stock.data.x.count());

    allPlotData.stock[index].data.setSymbol( new QwtSymbol( m_coordSymArr[index]/*QwtSymbol::Ellipse*/, Qt::black, QPen( Qt::black ), QSize( 7, 7 ) ) );
    allPlotData.stock[index].data.setPen(QPen(m_qPenArr[index], 2));

    allPlotData.stock[index].data.setRenderHint(QwtPlotItem::RenderAntialiased);
    allPlotData.stock[index].data.setLegendAttribute(QwtPlotCurve::LegendShowLine, true);

    updateMinMaxData(allPlotData.axis, axis);
    allPlotData.nofStocksToPlot++;
    delete x;
    delete y;
    return true;

}


/*****************************************************************
 *
 * Function:		findStock()
 *
 * Description:		This function
 *
 *
 *
 *****************************************************************/
bool CStockPlotUtil::findStock(PlotData_ST &allPlotData, QString stockName)
{
    int i;

    if(stockName.isEmpty()== true)
        return false;


    for(i = 0; i < allPlotData.nofStocksToPlot; i++)
    {
        if(allPlotData.stock[i].name.compare(stockName) == 0)
        {
            return true;
        }
    }
      return false;
}



/*****************************************************************
 *
 * Function:		()
 *
 * Description:		This function
 *
 *
 *
 *****************************************************************/
bool CStockPlotUtil::addStockToListWidget(CStockPlotUtil::StockData_ST &stock,  int index, QListWidget *listWidget)
{
    QString str;

    if(index >= MAX_NOF_PLOT_COLORS)
    {
        str.sprintf("Kan inte l?gga till aktie (Max = %d)", MAX_NOF_PLOT_COLORS);
        QMessageBox::information(NULL, QString::fromUtf8("L?gga till graf"), str);
        return false;
    }

    if(index < 0)
    {
        str.sprintf("Kan inte aktie index f?rlite (Max = %d)", index);
        QMessageBox::information(NULL, QString::fromUtf8("L?gga till graf"), str);
        return false;
    }
    QString s = QString::fromUtf8(stock.stockName.toStdString().c_str());
    listWidget->addItem(s);
    listWidget->item(index)->setForeground(m_qPenArr[index]);
    return true;
}


#if 0
/*****************************************************************
 *
 * Function:		()
 *
 * Description:		This function
 *
 *
 *
 *****************************************************************/
bool CStockPlotUtil::removePlotFromStock(PlotData_ST &allPlotData, QString stockName, int &removedIndex)
{
    QString str;

    str.sprintf("allPlotData.nofStocksToPlot =%d", allPlotData.nofStocksToPlot);
    QMessageBox::information(NULL, QString::fromUtf8("L?gga till graf"), str);


    for(int i = 0; i < allPlotData.nofStocksToPlot; i++)
    {
        if(allPlotData.stock[i].name.compare(stockName) == 0)
        {
            allPlotData.stock[i].data.detach();
            allPlotData.stock[i].data.setData(NULL);
            removedIndex = i;

            if(allPlotData.nofStocksToPlot <= 0)
            {
                allPlotData.axis.minMaxIsInit = false;
                allPlotData.nofStocksToPlot = 0;
            }
            else
            {
                allPlotData.nofStocksToPlot--;
                if(allPlotData.nofStocksToPlot <= 0)
                {
                    allPlotData.axis.minMaxIsInit = false;
                }
            }
            return true;
        }
    }

    return false;
}
#endif



/*****************************************************************
 *
 * Function:		()
 *
 * Description:		This function
 *
 *
 *
 *****************************************************************/
void CStockPlotUtil::updateMinMaxData(CStockPlotUtil::MinMaxAxisValues_ST &oldAxis, CStockPlotUtil::MinMaxAxisValues_ST &newAxis)
{

    if(newAxis.minMaxIsInit == false)
    {
        oldAxis.minY = newAxis.minY;
        oldAxis.maxY = newAxis.maxY;
        oldAxis.minX = newAxis.minX;
        oldAxis.maxX = newAxis.maxX;
        oldAxis.minMaxIsInit = true;
    }
    else
    {
        if(oldAxis.minY > newAxis.minY)
            oldAxis.minY = newAxis.minY;

        if(oldAxis.maxY < newAxis.maxY)
            oldAxis.maxY = newAxis.maxY;

        if(oldAxis.minX > newAxis.minX)
            oldAxis.minX = newAxis.minX;

        if (oldAxis.maxX < newAxis.maxX)
            oldAxis.maxX = newAxis.maxX;
    }

}

/*****************************************************************
 *
 * Function:		()
 *
 * Description:		This function
 *
 *
 *
 *****************************************************************/
bool CStockPlotUtil::plotData(PlotData_ST &allPlotData, QwtPlot *qwtPlot, int index)
{
    QString str;

    if(index >= MAX_NOF_PLOT_COLORS)
    {
        str.sprintf("Felaktigt index f?r inte vara st?rre (Max = %d)", MAX_NOF_PLOT_COLORS);
        QMessageBox::information(NULL, QString::fromUtf8("F?rstort index"), str);
        return false;
    }

    if(index < 0)
    {
        str.sprintf("Felaktigt index. (Max = %d)", index);
        QMessageBox::information(NULL, QString::fromUtf8("Index f?rlite"), str);
        return false;
    }

    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->enableXMin(true);
    grid->setMajPen(QPen(Qt::darkYellow, 0, Qt::DotLine));
    grid->setMinPen(QPen(Qt::darkYellow, 0 , Qt::DotLine));
    grid->attach(qwtPlot);


    #if 0
    QwtPlotPicker *d_picker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
    QwtPlotPicker::CrossRubberBand, QwtPicker::AlwaysOn,
    qwtPlot->canvas());

    d_picker->setRubberBandPen(QColor(Qt::green));
    d_picker->setRubberBand(QwtPicker::CrossRubberBand);
    d_picker->setTrackerPen(QColor(Qt::yellow));
    #endif

    //x-axis ( QwtScaleEngine::setAttribute( QwtScaleEngine::Inverted ) )

    //qwtPlot->setAxisScale(QwtPlot::xBottom, allPlotData.axis.minX, allPlotData.axis.maxX);
    qwtPlot->setAxisScale(QwtPlot::xBottom, allPlotData.axis.maxX, allPlotData.axis.minX);
    qwtPlot->setAxisScale(QwtPlot::yLeft, allPlotData.axis.minY, allPlotData.axis.maxY); // Max av % satser

    // qwtPlot->setAxisScaleEngine(QwtPlot::yLeft, new QwtLog10ScaleEngine);
    // qwtPlot->setAxisAutoScale(QwtPlot::yLeft, true);


    allPlotData.stock[index].data.attach(qwtPlot);
    qDebug() << "index" << index;
    qwtPlot->replot();
    return true;
}


/*****************************************************************
 *
 * Function:		()
 *
 * Description:		This function
 *
 *
 *
 *****************************************************************/
bool CStockPlotUtil::emtypPlotData(PlotData_ST &allPlotData)
{
    for(int i = 0; i < allPlotData.nofStocksToPlot; i++)
    {
        allPlotData.stock[i].data.detach();
        allPlotData.stock[i].data.setData(NULL);
        allPlotData.stock[i].name.clear();
    }

    allPlotData.axis.minMaxIsInit = false;
    allPlotData.nofStocksToPlot = 0;

    return true;
}
