/********************************************************************************
 *
 * Filename: stockPlotUtil.cpp
 *
 * Description: Plots xy data in qwt graph.
 *
 *
 *******************************************************************************/


#include "yahooStockPlotUtil.h"
#include <QtCore>
#include <QMessageBox>
#include <qwt_plot_marker.h>
#include <qwt_symbol.h>
#include <qwt_plot_grid.h>
#include <qwt_scale_engine.h>
#include <qwt_plot_intervalcurve.h>

//#include <qwt_plot_zoomer.h>
//#include <qwt_plot_panner.h>
// #include <qwt_plot_marker.h>
// #include <qwt_plot_grid.h>
// #include <qwt_plot_curve.h>
// #include <qwt_plot_canvas.h>
//#include <qwt_plot_intervalcurve.h>
// #include <qwt_legend.h>
#include <qwt_interval_symbol.h>
//#include <qwt_symbol.h>
// #include <qwt_series_data.h>
//#include <qwt_text.h>
// #include <qwt_scale_draw.h>
#include <qwt_plot_renderer.h>
// #include <qdatetime.h>
// #include <qfiledialog.h>
// #include <qfileinfo.h>




#define INDICATOR_1 1
#define INDICATOR_2 2
#define INDICATOR_3 3
#define INDICATOR_4 4
#define INDICATOR_5 5



QwtSymbol::Style CYahooStockPlotUtil::m_coordSymArr[MAX_NOF_PLOT_COLORS]=
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



Qt::GlobalColor CYahooStockPlotUtil::m_qPenArr[MAX_NOF_PLOT_COLORS] =
{
        Qt::black,
        Qt::red,  // Bra
        Qt::green,
        Qt::blue,  // Bra
        Qt::darkGreen, // bra
        Qt::magenta,
        Qt::darkRed,
        Qt::cyan,  // Bra
        Qt::yellow,
        Qt::darkBlue,
        Qt::darkCyan,
        Qt::darkMagenta,
        Qt::darkYellow,
        // Qt::darkGray,
        // Qt::gray,
    };


/*****************************************************************
 *
 * Function:		CYahooStockPlotUtil()
 *
 * Description:
 *
 *
 *
 *****************************************************************/
CYahooStockPlotUtil::CYahooStockPlotUtil()
{
    //d_intervalCurve = NULL;
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
bool CYahooStockPlotUtil::initSockNameCombo(QVector <CYahooStockPlotUtil::StockData_ST> &stockArr, QComboBox *combo)
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
void CYahooStockPlotUtil::removeAllStockArrData(QVector <CYahooStockPlotUtil::StockData_ST> &stockArr)
{
        for( int i = 0; i < stockArr.count(); i++)
        {
            stockArr[i].data.x.clear();
            stockArr[i].data.y.clear();
        }
        stockArr.clear();
}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void CYahooStockPlotUtil::initMinMaxAxis(CYahooStockPlotUtil::MinMaxAxisValues_ST inputAxis,
                                         CYahooStockPlotUtil::MinMaxAxisValues_ST &outputAxis)
{
    outputAxis.minMaxIsInit = inputAxis.minMaxIsInit;
    outputAxis.maxX = inputAxis.maxX;
    outputAxis.maxY = inputAxis.maxY;
    outputAxis.minX  = inputAxis.minX;
    outputAxis.maxXDate  = inputAxis.maxXDate;
    outputAxis.minXDate  = inputAxis.minXDate;
    outputAxis.minY  = inputAxis.minY;
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
bool CYahooStockPlotUtil::
addStockToPlot(CYahooStockPlotUtil::PlotData_ST &allPlotData,
                                    CYahooStockPlotUtil::StockData_ST &stock,
                                    CYahooStockPlotUtil::MinMaxAxisValues_ST &axis,
                                    int index,
                                    QString legendLable)
{
    int i;
    QString str;
    double *x = new double [stock.data.x.count()+5];
    double *y = new double [stock.data.y.count()+5];


    if(index >= MAX_NOF_PLOT_COLORS)
    {
        str.sprintf("Kan inte visa fler grafer (Max = %d)", MAX_NOF_PLOT_COLORS);
        QMessageBox::information(NULL, QString::fromUtf8("Lägga till graf"), str);
        delete x;
        delete y;
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

    allPlotData.stock[index].data.setTitle(legendLable);
    allPlotData.stock[index].name = stock.stockName;
    allPlotData.stock[index].data.setSamples(x, y, stock.data.x.count());

    // allPlotData.stock[index].intervalCurve.setSamples(y);

    qDebug() << "index" << index;
    qDebug() << stock.data.x.size();
    if(stock.data.x.size() < 100)
    {
       allPlotData.stock[index].data.setSymbol( new QwtSymbol( m_coordSymArr[index]/*QwtSymbol::Ellipse*/, Qt::black, QPen( Qt::red ), QSize( 5, 5 ) ) );
       allPlotData.stock[index].data.setPen(QPen(m_qPenArr[index], 2));

       //allPlotData.stock[index].intervalCurve.setSymbol( new QwtSymbol( m_coordSymArr[index]/*QwtSymbol::Ellipse*/, Qt::black, QPen( Qt::red ), QSize( 5, 5 ) ) );
       // allPlotData.stock[index].intervalCurve.setPen(QPen(m_qPenArr[index], 2));
    }
    else
    {
        allPlotData.stock[index].data.setSymbol( new QwtSymbol( QwtSymbol::NoSymbol, Qt::black, QPen( Qt::red ), QSize( 5, 5 ) ) );
        allPlotData.stock[index].data.setPen(QPen(m_qPenArr[index], 2));

    }


    allPlotData.stock[index].data.setRenderHint(QwtPlotItem::RenderAntialiased);
    allPlotData.stock[index].data.setLegendAttribute(QwtPlotCurve::LegendShowLine, true);

    // funkar inte allPlotData.stock[index].data.setStyle(QwtPlotIntervalCurve::NoCurve);

    updateMinMaxData(allPlotData.axis, axis);
    allPlotData.nofStocksToPlot++;
    delete x;
    delete y;
    return true;

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
bool CYahooStockPlotUtil::
addIndicatorToPlot(CYahooStockPlotUtil::PlotData_ST &allPlotData,
                                             CYahooStockPlotUtil::StockData_ST &stock,
                                             CYahooStockPlotUtil::MinMaxAxisValues_ST &axis,
                                             int indicatorIndex,
                                             int colorIndex,
                                             QString legendLable)
{
    int i;

    QString str;
    qDebug() << stock.data.x.count();
    double *x = new double [stock.data.x.count()+5];
    double *y = new double [stock.data.x.count()+5];


    if(colorIndex >= MAX_NOF_PLOT_COLORS)
    {
        str.sprintf("Kan inte visa fler grafer (Max = %d)", MAX_NOF_PLOT_COLORS);
        QMessageBox::information(NULL, QString::fromUtf8("Lägga till graf"), str);
        delete x;
        delete y;
        return false;
    }


   // Set color
    for(i = 0; i < stock.data.x.count(); i++)
    {
        switch(indicatorIndex)
        {
        case INDICATOR_1:
            x[i] = stock.data.x[i];
            y[i] = stock.data.indicator1[i];
            break;
        case INDICATOR_2:
            x[i] = stock.data.x[i];
            y[i] = stock.data.indicator2[i];
            break;
        case INDICATOR_3:
            x[i] = stock.data.x[i];
            y[i] = stock.data.indicator3[i];
            break;
        case INDICATOR_4:
            x[i] = stock.data.x[i];
            y[i] = stock.data.indicator4[i];
            break;
        case INDICATOR_5:
            x[i] = stock.data.x[i];
            y[i] = stock.data.indicator4[i];
            break;
        default:
            str.sprintf("Invalid indicator selected");
            QMessageBox::information(NULL, QString::fromUtf8("Plot indicator"), str);
            delete x;
            delete y;
            return false;
        }

        qDebug() << x[i];
        qDebug() << y[i];
    }

    allPlotData.stock[colorIndex].data.setTitle(legendLable);
    allPlotData.stock[colorIndex].name = stock.stockName;
    allPlotData.stock[colorIndex].data.setSamples(x, y, stock.data.x.count());

    // allPlotData.stock[index].data.setSymbol( new QwtSymbol( m_coordSymArr[index]/*QwtSymbol::Ellipse*/, Qt::black, QPen( Qt::black ), QSize( 7, 7 ) ) );
    allPlotData.stock[colorIndex].data.setPen(QPen(m_qPenArr[colorIndex], 1));

    allPlotData.stock[colorIndex].data.setRenderHint(QwtPlotItem::RenderAntialiased);
    allPlotData.stock[colorIndex].data.setLegendAttribute(QwtPlotCurve::LegendShowLine, true);

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
bool CYahooStockPlotUtil::
findStock(CYahooStockPlotUtil::PlotData_ST &allPlotData, QString stockName)
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
bool CYahooStockPlotUtil::
addStockToListWidget(CYahooStockPlotUtil::StockData_ST &stock,  int index, QListWidget *listWidget)
{
    QString str;

    if(index >= MAX_NOF_PLOT_COLORS)
    {
        str.sprintf("Kan inte lägga till aktie (Max = %d)", MAX_NOF_PLOT_COLORS);
        QMessageBox::information(NULL, QString::fromUtf8("Lägga till graf"), str);
        return false;
    }

    if(index < 0)
    {
        str.sprintf("Kan inte aktie index förlite (Max = %d)", index);
        QMessageBox::information(NULL, QString::fromUtf8("Lägga till graf"), str);
        return false;
    }
    QString s = QString::fromUtf8(stock.stockName.toStdString().c_str());
    listWidget->addItem(s);
    listWidget->item(index)->setForeground(m_qPenArr[index]);
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
void CYahooStockPlotUtil::
updateMinMaxData(CYahooStockPlotUtil::MinMaxAxisValues_ST &oldAxis,
                 CYahooStockPlotUtil::MinMaxAxisValues_ST &newAxis)
{

    if(newAxis.minMaxIsInit == false)
    {
        oldAxis.minY = newAxis.minY;
        oldAxis.maxY = newAxis.maxY;
        oldAxis.minX = newAxis.minX;
        oldAxis.maxX = newAxis.maxX;

        if(newAxis.maxXDate.size() > 0)
        {
            oldAxis.maxXDate = newAxis.maxXDate;
            qDebug() << newAxis.maxXDate;
        }

        if(newAxis.minXDate.size() > 0)
        {
            oldAxis.minXDate = newAxis.minXDate;
            qDebug() << newAxis.minXDate;
        }

        oldAxis.minMaxIsInit = true;
    }
    else
    {
        if(oldAxis.minY > newAxis.minY)
            oldAxis.minY = newAxis.minY;

        if(oldAxis.maxY < newAxis.maxY)
            oldAxis.maxY = newAxis.maxY;

        if(oldAxis.minX > newAxis.minX)
        {
            if(newAxis.minXDate.size() > 0)
            {
                oldAxis.minXDate = newAxis.minXDate;
                qDebug() << newAxis.minXDate;
            }

            oldAxis.minX = newAxis.minX;
        }

        if (oldAxis.maxX < newAxis.maxX)
        {
            if(newAxis.maxXDate.size() > 0)
            {
                oldAxis.maxXDate = newAxis.maxXDate;
                qDebug() << newAxis.minXDate;
            }

            oldAxis.maxX = newAxis.maxX;
        }
    }

}

/*******************************************************************
 *
 * Function:    updateMinMaxXAxisScales()
 *
 * Description:
 *
 *
 *******************************************************************/
void CYahooStockPlotUtil::
updateMinMaxXAxisScales(CYahooStockPlotUtil::MinMaxAxisValues_ST &oldAxis,
                        CYahooStockPlotUtil::MinMaxAxisValues_ST &newAxis)
{
    // Update min max scale of x and y axis
    updateMinMaxAxis(oldAxis, newAxis.maxX, newAxis.maxY, newAxis.maxXDate);
    updateMinMaxAxis(oldAxis, newAxis.minX, newAxis.minY, newAxis.minXDate);

    updateMinMaxAxis(newAxis, oldAxis.maxX, oldAxis.maxY, oldAxis.maxXDate);
    updateMinMaxAxis(newAxis, oldAxis.minX, oldAxis.minY, oldAxis.minXDate);
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
void CYahooStockPlotUtil::
updateMinMaxAxis(CYahooStockPlotUtil::MinMaxAxisValues_ST &axis, double x, double y, QString date)
{

    if(axis.minMaxIsInit == false)
    {
        axis.minY = y;
        axis.maxY = y;
        axis.minX = x;
        axis.maxX = x;
        if(date.size() > 0 )
        {
            axis.maxXDate = date;
            axis.minXDate = date;
            qDebug() << "date1" << date;
        }
        axis.minMaxIsInit = true;
    }
    else
    {
        if(axis.minY > y)
            axis.minY = y;

        if(axis.maxY < y)
            axis.maxY = y;

        if(axis.minX > x)
        {
            axis.minX = x;
            if(date.size() > 0 )
            {
                axis.minXDate = date;
                qDebug() << "date2" << date;
            }
        }

        if (axis.maxX < x)
        {
            axis.maxX = x;
            if(date.size() > 0 )
            {
                axis.maxXDate = date;
                qDebug() << "date3" << date;
            }
        }
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
bool CYahooStockPlotUtil::
plotYAxisLogData(CYahooStockPlotUtil::PlotData_ST &allPlotData,
         QwtPlot *qwtPlot,
         int index, int xScaleStep)
{
    QString str;
    // QString tmp;

    if(index >= MAX_NOF_PLOT_COLORS)
    {
        str.sprintf("Felaktigt index för inte vara större (Max = %d)", MAX_NOF_PLOT_COLORS);
        QMessageBox::information(NULL, QString::fromUtf8("Förstort index"), str);
        return false;
    }

    if(index < 0)
    {
        str.sprintf("Felaktigt index. (Max = %d)", index);
        QMessageBox::information(NULL, QString::fromUtf8("Index förlite"), str);
        return false;
    }

    //QwtScaleDiv *scaleDiv = new QwtPlotGrid;

    //scaleDiv->setTicks();

    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->enableXMin(true);
    grid->setMajPen(QPen(Qt::darkYellow, 0, Qt::DotLine));
    grid->setMinPen(QPen(Qt::darkYellow, 0, Qt::DotLine));
    grid->attach(qwtPlot);

    qwtPlot->setAxisScale(QwtPlot::xBottom, allPlotData.axis.minX, allPlotData.axis.maxX, xScaleStep);
    // qwtPlot->setAxisScale(QwtPlot::yLeft, (allPlotData.axis.minY), (allPlotData.axis.maxY)); // Max av % satser
    // qDebug() << "minY" << allPlotData.axis.minY;
    // qDebug() << "maxY" << allPlotData.axis.maxY;


    qwtPlot->setAxisScaleEngine(QwtPlot::yLeft, new QwtLog10ScaleEngine);
    //qwtPlot->setAxisAutoScale(QwtPlot::yLeft, true);
    double minY;
    if(allPlotData.axis.minY < 0)
        minY = 1;
    else
        minY = allPlotData.axis.minY / 10;

    qwtPlot->setAxisMaxMajor(QwtPlot::yLeft, 6);
    qwtPlot->setAxisMaxMinor(QwtPlot::yLeft, 10);
    qwtPlot->setAxisScale(QwtPlot::yLeft, minY, (allPlotData.axis.maxY*5), 1);


    //qwtPlot->setAxisScaleDiv();
    //qwtPlot->setAxisScale()
    //qwtPlot->setAxisScale(QwtPlot::yLeft, 5, 250);
    //qwtPlot->setAxisScale(QwtPlot::yLeft, (allPlotData.axis.minY), (allPlotData.axis.maxY)/*1, 100000000*/);



    allPlotData.stock[index].data.attach(qwtPlot);
    qDebug() << "index" << index;
    qwtPlot->replot();

     #if 0
    // qwtPlot->axisScaleDiv(QwtPlot::xBottom)->upperBound(); //  and ->hBound()
    tmp.sprintf("%g", qwtPlot->axisScaleDiv(QwtPlot::xBottom)->upperBound());
    qDebug() << "x axis upper" << tmp << "\n";

    tmp.sprintf("%g", qwtPlot->axisScaleDiv(QwtPlot::xBottom)->lowerBound());
    qDebug() << "x axis lower" << tmp << "\n";

    tmp.sprintf("%g", qwtPlot->axisScaleDiv(QwtPlot::xBottom)->range());
    qDebug() << "x range " << tmp << "\n";

    qDebug() << "x axisInterval" << tmp << "\n";
    #endif

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
bool CYahooStockPlotUtil::
plotDataSetXScaleStep(CYahooStockPlotUtil::PlotData_ST &allPlotData,
         QwtPlot *qwtPlot,
         int index,
         int xScaleStep)
{
    QString str;
    QString tmp;

    if(index >= MAX_NOF_PLOT_COLORS)
    {
        str.sprintf("Felaktigt index för inte vara större (Max = %d)", MAX_NOF_PLOT_COLORS);
        QMessageBox::information(NULL, QString::fromUtf8("Förstort index"), str);
        return false;
    }

    if(index < 0)
    {
        str.sprintf("Felaktigt index. (Max = %d)", index);
        QMessageBox::information(NULL, QString::fromUtf8("Index förlite"), str);
        return false;
    }

    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->enableXMin(true);
    grid->setMajPen(QPen(Qt::darkYellow, 0, Qt::DotLine));
    grid->setMinPen(QPen(Qt::darkYellow, 0, Qt::DotLine));
    grid->attach(qwtPlot);


    qwtPlot->setAxisScaleEngine(QwtPlot::yLeft, new QwtLinearScaleEngine);

#if 0
    if(useAutoScale == true)
    {
        // QString title("hej");
        // setXAxisTitle(qwtPlot, title, 6);
        qwtPlot->setAxisScale(QwtPlot::xBottom, 2009, 2013, 1);
        qwtPlot->setAxisFont(QwtPlot::xBottom,QFont("Helvetica", 7));
        // qwtPlot->setAxisAutoScale(QwtPlot::xBottom, true);
        qwtPlot->setAxisAutoScale(QwtPlot::yLeft, true);

    }
    else
#endif
    {
        if((allPlotData.axis.minY) == (allPlotData.axis.maxY))
        {
            if(allPlotData.axis.minY == 0)
            {
                qwtPlot->setAxisScale(QwtPlot::yLeft, (-1), (1));
            }
            else
            {
                qwtPlot->setAxisScale(QwtPlot::yLeft, (allPlotData.axis.minY*0.9), (allPlotData.axis.maxY*1.0)); // Max av % satser
            }
        }
        else
        {
            qwtPlot->setAxisScale(QwtPlot::yLeft, (allPlotData.axis.minY), (allPlotData.axis.maxY)); // Max av % satser
        }
        qwtPlot->setAxisScale(QwtPlot::xBottom, allPlotData.axis.minX, allPlotData.axis.maxX, xScaleStep);
        qDebug() << "minY" << allPlotData.axis.minY;
        qDebug() << "maxY" << allPlotData.axis.maxY;

    }



    allPlotData.stock[index].data.attach(qwtPlot);
    qDebug() << "index" << index;
    qwtPlot->replot();


    #if 0
    // qwtPlot->axisScaleDiv(QwtPlot::xBottom)->upperBound(); //  and ->hBound()
    tmp.sprintf("%g", qwtPlot->axisScaleDiv(QwtPlot::xBottom)->upperBound());
    qDebug() << "x axis upper" << tmp << "\n";

    tmp.sprintf("%g", qwtPlot->axisScaleDiv(QwtPlot::xBottom)->lowerBound());
    qDebug() << "x axis lower" << tmp << "\n";

    tmp.sprintf("%g", qwtPlot->axisScaleDiv(QwtPlot::xBottom)->range());
    qDebug() << "x range " << tmp << "\n";

    qDebug() << "x axisInterval" << tmp << "\n";
    #endif

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
void CYahooStockPlotUtil::
addLastOnePlotData(StockData_ST &stockData,
               CYahooStockPlotUtil::PlotData_ST &qwtStockPlotData,
               double y, QString date)
{

    int size = stockData.data.x.size();
    qDebug() << stockData.data.x.size();
    qDebug() << stockData.data.y.size();

    stockData.data.x.insert(size, size);
    stockData.data.y.insert(size, y);
    stockData.data.xDate.insert(size, date);
    updateMinMaxAxis(qwtStockPlotData.axis, (double) size, y, date);
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
bool CYahooStockPlotUtil::
plotData(CYahooStockPlotUtil::PlotData_ST &allPlotData,
         QwtPlot *qwtPlot,
         int index,
         bool useAutoScale)
{
    QString str;
    // QString tmp;

    if(index >= MAX_NOF_PLOT_COLORS)
    {
        str.sprintf("Felaktigt index för inte vara större (Max = %d)", MAX_NOF_PLOT_COLORS);
        QMessageBox::information(NULL, QString::fromUtf8("Förstort index"), str);
        return false;
    }

    if(index < 0)
    {
        str.sprintf("Felaktigt index. (Max = %d)", index);
        QMessageBox::information(NULL, QString::fromUtf8("Index förlite"), str);
        return false;
    }

    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->enableXMin(true);
    grid->setMajPen(QPen(Qt::darkYellow, 0, Qt::DotLine));
    grid->setMinPen(QPen(Qt::darkYellow, 0, Qt::DotLine));
    grid->attach(qwtPlot);

    qwtPlot->setAxisScaleEngine(QwtPlot::yLeft, new QwtLinearScaleEngine);

    if(useAutoScale == true)
    {
        qwtPlot->setAxisAutoScale(QwtPlot::xBottom, true);
        qwtPlot->setAxisAutoScale(QwtPlot::yLeft, true);
    }
    else
    {
        qwtPlot->setAxisScale(QwtPlot::xBottom, allPlotData.axis.minX, allPlotData.axis.maxX);
        qwtPlot->setAxisScale(QwtPlot::yLeft, (allPlotData.axis.minY), (allPlotData.axis.maxY)); // Max av % satser
        qDebug() << "minY" << allPlotData.axis.minY;
        qDebug() << "maxY" << allPlotData.axis.maxY;
    }

    allPlotData.stock[index].data.attach(qwtPlot);
    qDebug() << "index" << index;
    qwtPlot->replot();


    #if 0
    // qwtPlot->axisScaleDiv(QwtPlot::xBottom)->upperBound(); //  and ->hBound()
    tmp.sprintf("%g", qwtPlot->axisScaleDiv(QwtPlot::xBottom)->upperBound());
    qDebug() << "x axis upper" << tmp << "\n";

    tmp.sprintf("%g", qwtPlot->axisScaleDiv(QwtPlot::xBottom)->lowerBound());
    qDebug() << "x axis lower" << tmp << "\n";

    tmp.sprintf("%g", qwtPlot->axisScaleDiv(QwtPlot::xBottom)->range());
    qDebug() << "x range " << tmp << "\n";

    qDebug() << "x axisInterval" << tmp << "\n";
    #endif

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
bool CYahooStockPlotUtil::emtypPlotData(PlotData_ST &allPlotData)
{
    for(int i = 0; i < allPlotData.nofStocksToPlot; i++)
    {
        allPlotData.stock[i].data.detach();
        allPlotData.stock[i].data.setData(NULL);
        allPlotData.stock[i].name.clear();
        allPlotData.stock[i].data.setLegendAttribute(QwtPlotCurve::LegendShowLine, false);
        allPlotData.stock[i].data.setLegendAttribute(QwtPlotCurve::LegendShowSymbol, false);

    }
    allPlotData.axis.minMaxIsInit = false;
    allPlotData.nofStocksToPlot = 0;

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
void CYahooStockPlotUtil::
removePlotData(PlotData_ST &allPlotData , int index, QwtPlot *qwtPlot)
{
    /* detatch all data from grap */
    allPlotData.stock[index].data.detach();
    allPlotData.stock[index].data.setData(NULL);
    allPlotData.stock[index].name.clear();
    allPlotData.stock[index].data.setLegendAttribute(QwtPlotCurve::LegendShowLine, false);
    allPlotData.stock[index].data.setLegendAttribute(QwtPlotCurve::LegendShowSymbol, false);


    allPlotData.axis.minMaxIsInit = false;
    allPlotData.nofStocksToPlot = 0;

    qwtPlot->replot();

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
void CYahooStockPlotUtil::
panningX1(PlotData_ST &allPlotData, bool inc, QwtPlot *qwtPlot)
{
    if( inc == true)
    {
        allPlotData.axis.minX++;
        allPlotData.axis.maxX++;

    }
    else
    {
        allPlotData.axis.minX--;
        allPlotData.axis.maxX--;
    }

    qwtPlot->setAxisScale(QwtPlot::xBottom, allPlotData.axis.minX, allPlotData.axis.maxX);
    qwtPlot->replot();

}


//=================

/*****************************************************************
 *
 * Function:
 *
 * Description:
 *
 *
 *
 *****************************************************************/
void CYahooStockPlotUtil::
addOneIntervalBarData(QVector<QwtIntervalSample> &rangeData,
                      CYahooStockPlotUtil::MinMaxAxisValues_ST &axisMinMax,
                      double x,
                      double minY,
                      double maxY)
{
    if(maxY > minY)
    {
        updateMinMaxAxis(axisMinMax, x, maxY);
    }
    else
    {
        // Something is wrong
        updateMinMaxAxis(axisMinMax, x, minY);
    }

    updateMinMaxAxis(axisMinMax, x, minY);
    rangeData.append(QwtIntervalSample(x, QwtInterval(minY, maxY)));
}



#if 0
/*******************************************************************
 *
 * Function:    deleteAllPlotData()
 *
 * Description:
 *
 *
 *******************************************************************/
void CYahooStockPlotUtil::
deleteAllPlotData(CYahooStockPlotUtil::PlotData_ST &allPlotData, QwtPlot *qwtPlot)
{
    for(int i = 0; i < PLOT_INDEX_MAX; i++)
    {
        m_plot.removePlotData(allPlotData, i, qwtPlot);
    }

}
#endif


/*****************************************************************
 *
 * Function:
 *
 * Description:
 *
 *
 *
 *****************************************************************/
void CYahooStockPlotUtil::
plotIntervalBarData(QwtPlot *qwtPlot, QwtPlotIntervalCurve &intervalCurve,
                    const QString &title, const QVector<QwtIntervalSample>& samples, const QColor &color)
{
    //qDebug() << d_intervalCurve;
    // Draw bars with spaces between them
    // intervalCurve = new QwtPlotIntervalCurve( title );
    // qDebug() << d_intervalCurve;
    intervalCurve.setTitle(title);
    intervalCurve.setRenderHint( QwtPlotItem::RenderAntialiased );
    intervalCurve.setStyle( QwtPlotIntervalCurve::NoCurve );

    // Set symbol style and color
    QwtIntervalSymbol *errorBar = new QwtIntervalSymbol( QwtIntervalSymbol::Bar );

    // The with of the horizontel top/bottom lines
    errorBar->setWidth( 8 ); // should be something even

    // Symbol line color and with
    QPen pen;
    pen.setWidth(2);
    pen.setColor(color);
    errorBar->setPen(pen);

    intervalCurve.setSymbol( errorBar );
    intervalCurve.setRenderHint( QwtPlotItem::RenderAntialiased, false );

    intervalCurve.setSamples( samples );
    intervalCurve.attach( qwtPlot );

    qwtPlot->replot();
}

#if 0
/*****************************************************************
 *
 * Function:
 *
 * Description:
 *
 *
 *
 *****************************************************************/
void CYahooStockPlotUtil::
plotIntervalBarData(QwtPlot *qwtPlot,
                    const QString &title, const QVector<QwtIntervalSample>& samples, const QColor &color)
{
    qDebug() << d_intervalCurve;
    // Draw bars with spaces between them
    d_intervalCurve = new QwtPlotIntervalCurve( title );
    qDebug() << d_intervalCurve;
    d_intervalCurve->setRenderHint( QwtPlotItem::RenderAntialiased );
    d_intervalCurve->setStyle( QwtPlotIntervalCurve::NoCurve );

    // Set symbol style and color
    QwtIntervalSymbol *errorBar = new QwtIntervalSymbol( QwtIntervalSymbol::Bar );

    // The with of the horizontel top/bottom lines
    errorBar->setWidth( 8 ); // should be something even

    // Symbol line color and with
    QPen pen;
    pen.setWidth(2);
    pen.setColor(color);
    errorBar->setPen(pen);

    d_intervalCurve->setSymbol( errorBar );
    d_intervalCurve->setRenderHint( QwtPlotItem::RenderAntialiased, false );

    d_intervalCurve->setSamples( samples );
    d_intervalCurve->attach( qwtPlot );

    qwtPlot->replot();
}
#endif


#if 0
void CYahooStockPlotUtil::
deleteIntervalBarData(QwtPlot *qwtPlot, QVector<QwtIntervalSample>& samples)
{

    //qDebug() << d_intervalCurve;
    samples.clear();
    if(d_intervalCurve != NULL)
    {
        d_intervalCurve->setSamples( samples );
       // d_intervalCurve->detach();
        qwtPlot->replot();
    }
}
#endif

void CYahooStockPlotUtil::
deleteIntervalBarData(QwtPlot *qwtPlot,
                      QwtPlotIntervalCurve &intervalCurve,
                      QVector<QwtIntervalSample>& samples)
{

    //qDebug() << d_intervalCurve;
    samples.clear();
    // if(d_intervalCurve != NULL)
    {
        intervalCurve.detach();
        intervalCurve.setData(NULL);
        // intervalCurve.setSamples( samples );
       // d_intervalCurve->detach();
        qwtPlot->replot();
    }
}
