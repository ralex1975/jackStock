#include "leastSquareStaDlg.h"
#include "ui_leastsquarestadlg.h"
#include "dbHndl.h"
#include "extendedTable.h"
#include "util.h"
#include <QPalette>
#include "inc/guiUtil/myLineEdit.h"
#include "inc/guiUtil/guiFinanceCtrls.h"



#define TIME_2_MIN ((int)120000)


const LeastSquaresTaDlg::TimePeriodDays_ST LeastSquaresTaDlg::m_timePeriodDaysArr[LeastSquaresTaDlg::MAX_NOF_TIME_PERIOD_DAYS_ITEMS] =
{
    {QString::fromUtf8("AllaData"),       LeastSquaresTaDlg::TIME_PERIOD_DAYS_ALL_DATA,},
    {QString::fromUtf8("10 År"),          LeastSquaresTaDlg::TIME_PERIOD_DAYS_10_YEAR,},
    {QString::fromUtf8("7 År"),           LeastSquaresTaDlg::TIME_PERIOD_DAYS_7_YEAR,},
    {QString::fromUtf8("5 År"),           LeastSquaresTaDlg::TIME_PERIOD_DAYS_5_YEAR,},
    {QString::fromUtf8("3 År"),           LeastSquaresTaDlg::TIME_PERIOD_DAYS_3_YEAR,},
    {QString::fromUtf8("2 År"),           LeastSquaresTaDlg::TIME_PERIOD_DAYS_2_YEAR,},
    {QString::fromUtf8("1 År"),           LeastSquaresTaDlg::TIME_PERIOD_DAYS_1_YEAR,},
    {QString::fromUtf8("6 månader"),      LeastSquaresTaDlg::TIME_PERIOD_DAYS_6_MONTH,},
    {QString::fromUtf8("3 månader"),      LeastSquaresTaDlg::TIME_PERIOD_DAYS_3_MONTH,},
    {QString::fromUtf8("1 månader"),      LeastSquaresTaDlg::TIME_PERIOD_DAYS_1_MONTH,},
    {QString::fromUtf8("2 veckor"),       LeastSquaresTaDlg::TIME_PERIOD_DAYS_2_WEEK,},
    {QString::fromUtf8("1 vecka"),        LeastSquaresTaDlg::TIME_PERIOD_DAYS_1_WEEK}
};




/*******************************************************************
 *
 * Function:    settimeSlideWindowInc()
 *
 * Description:
 *
 *
 *******************************************************************/
void LeastSquaresTaDlg::settimeSlideWindowInc(int value)
{
    m_timeSlideWindowInc = value;
}



/*******************************************************************
 *
 * Function:    initStockList()
 *
 * Description:
 *
 *
 *******************************************************************/
void LeastSquaresTaDlg::initFa3MinMaxPePrice(void)
{

    QString column0 = QString::fromUtf8("År");
    QString column1 = QString("Min\npris");
    QString column2 = QString::fromUtf8("Medel\npris");
    QString column3 = QString("Max\npris");


    ui->treeWidget_5LSqrt->setColumnCount(4);
    ui->treeWidget_5LSqrt->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeWidget_5LSqrt->setSelectionBehavior(QAbstractItemView::SelectRows);



    if(QTreeWidgetItem* header = ui->treeWidget_5LSqrt->headerItem())
    {
        header->setText(0, column0);
        header->setText(1, column1);
        header->setText(2, column2);
        header->setText(3, column3);

    }

    ui->treeWidget_5LSqrt->setColumnWidth(0, 55);
    ui->treeWidget_5LSqrt->setColumnWidth(1, 55);
    ui->treeWidget_5LSqrt->setColumnWidth(2, 55);
    ui->treeWidget_5LSqrt->setColumnWidth(3, 55);


    QString column4 = QString::fromUtf8("År");
    QString column5 = QString("Min\nP/E");
    QString column6 = QString::fromUtf8("Medel\nP/E");
    QString column7 = QString("Max\nP/E");


    ui->treeWidget_6LSqrt->setColumnCount(4);
    ui->treeWidget_6LSqrt->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeWidget_6LSqrt->setSelectionBehavior(QAbstractItemView::SelectRows);



    if(QTreeWidgetItem* header1 = ui->treeWidget_6LSqrt->headerItem())
    {
        header1->setText(0, column4);
        header1->setText(1, column5);
        header1->setText(2, column6);
        header1->setText(3, column7);
    }

    ui->treeWidget_6LSqrt->setColumnWidth(0, 65);
    ui->treeWidget_6LSqrt->setColumnWidth(1, 65);
    ui->treeWidget_6LSqrt->setColumnWidth(2, 60);
    ui->treeWidget_6LSqrt->setColumnWidth(3, 50);
    ui->treeWidget_6LSqrt->setColumnWidth(4, 50);

}




/******************************************************************
 *
 * Function:        LeastSquaresTaDlg(()
 *
 * Description: Calculate least sqrt data on snapshot data
 *              already stored in database
 *
 *
 * ****************************************************************/
LeastSquaresTaDlg::LeastSquaresTaDlg(QWidget *parent) :
    QDialog(parent),
    m_timeSlideWindowInc(0),
    m_singleStockDataReqStatus(STATUS_REQ_SINGLE_STOCK_IDLE),
    ui(new Ui::LeastSquaresTaDlg)
{
    GuiFinanceCtrls gfc;

    ui->setupUi(this);


    // Just create dummy space so we can delete it
    // later before we create the space we need
    // (Risk returns)
    createQwtPlotArrMemSpace(2);
    m_nofRiskReturPlotData = 2;

    m_plotRiskReturBuy.initPlotPicker(ui->qwtPlotLSqr_3);
    m_plotRiskReturBuy.initPlotZoomer(ui->qwtPlotLSqr_3);
    m_plotRiskReturBuy.enableZoomMode(true);

    m_plotRiskReturSell.initPlotPicker(ui->qwtPlotLSqr_4);
    m_plotRiskReturSell.initPlotZoomer(ui->qwtPlotLSqr_4);
    m_plotRiskReturSell.enableZoomMode(true);


    gfc.addAllStockListsToCombobox(ui->stockListComboBoxSqrt);


    m_macdHist = new QwtPlotHistogram ();

    initFa3MinMaxPePrice();

    //=======================================================
    // Create thread that import data from yahoo
    m_importYahooTaDataThread = new importYahooTaData(this);

    if(m_importYahooTaDataThread == 0)
    {
        QMessageBox::information(NULL, QString::fromUtf8("Create thread"), QString::fromUtf8("Fail to create Ta Import Data thread"));
    }

    // Init time period
    initTimePeriodCtrls(TIME_PERIOD_DAYS_1_YEAR);

    settimeSlideWindowInc(0);

    m_plot.initPlotPicker(ui->qwtPlotLSqr);
    m_plot.initPlotZoomer(ui->qwtPlotLSqr);
    m_plot.initPlotPanner(ui->qwtPlotLSqr);

    m_plot.enablePanningMode(false);
    m_plot.enableZoomMode(false);

}




/**********************************************************************************
 *
 * Function:    tablePlusHeaderClicked()
 *
 *
 * Description:
 *
 *
 *
 ********************************************************************************/
void LeastSquaresTaDlg::tablePlusHeaderClicked(int column)
{
    static bool toggle = false;

    if(true == toggle)
    {
        toggle = false;
        ui->tableViewLeastSquare->sortByColumn(column, Qt::AscendingOrder);
    }
    else
    {
        toggle = true;
        ui->tableViewLeastSquare->sortByColumn(column, Qt::DescendingOrder);
    }

}

/**********************************************************************************
 *
 * Function:    tableMinusHeaderClicked()
 *
 *
 * Description:
 *
 *
 *
 ********************************************************************************/
void LeastSquaresTaDlg::tableMinusHeaderClicked(int column)
{
    static bool toggle = false;

    if(true == toggle)
    {
        toggle = false;
        ui->tableViewLeastSquare_2->sortByColumn(column, Qt::AscendingOrder);
    }
    else
    {
        toggle = true;
        ui->tableViewLeastSquare_2->sortByColumn(column, Qt::DescendingOrder);
    }
}


/**********************************************************************************
 *
 * Function:
 *
 *
 * Description:
 *
 *
 *
 ********************************************************************************/
LeastSquaresTaDlg::~LeastSquaresTaDlg()
{
    delete ui;
    delete m_macdHist;
}



/*******************************************************************
 *
 * Function:    deleteAllPlotData()
 *
 * Description:
 *
 *
 *******************************************************************/
void LeastSquaresTaDlg::deleteAllPlotData(CYahooStockPlotUtil::PlotData_ST &allPlotData, QwtPlot *qwtPlotLSqr)
{
    for(int i = 0; i < PLOT_INDEX_MAX; i++)
    {
        m_plot.removePlotData(allPlotData, i, qwtPlotLSqr);
    }

}


/*******************************************************************
 *
 * Function:    setTimePeriodDaysUpdateStart()
 *
 * Description: This function is used when user changes set
 *              Time Period Slider position.
 *
 *
 *******************************************************************/
void LeastSquaresTaDlg::
setTimePeriodDaysUpdateStartStopDate(QString &startDate, QString &endDate, int value)
{
    CUtil cu;
    int intYear;
    int intMonth;
    int intDays;
    int inc = 0;


    if(false == cu.dateIsValid(endDate))
    {
         QMessageBox::information(this, tr("Datum"), QString::fromUtf8("1. Slut datum felaktigt"));
        return;
    }

    inc = -m_timeSlideWindowInc;
    cu.addDays(endDate, endDate, inc);


    qDebug() << "sw" << m_timePeriodDaysArr[value].timePeriod;

    switch(m_timePeriodDaysArr[value].timePeriod)
    {
    case TIME_PERIOD_DAYS_ALL_DATA:
        // TBD
        break;
    case TIME_PERIOD_DAYS_10_YEAR:
        intYear = -10;
        cu.addYear(endDate, startDate, intYear);
        break;
    case TIME_PERIOD_DAYS_7_YEAR:
        intYear = -7;
        cu.addYear(endDate, startDate, intYear);
        break;
    case TIME_PERIOD_DAYS_5_YEAR:
        intYear = -5;
        cu.addYear(endDate, startDate, intYear);
        break;
    case TIME_PERIOD_DAYS_3_YEAR:
        intYear = -3;
        cu.addYear(endDate, startDate, intYear);
        break;
    case TIME_PERIOD_DAYS_2_YEAR:
        intYear = -2;
        cu.addYear(endDate, startDate, intYear);
        break;
    case TIME_PERIOD_DAYS_1_YEAR:
        intYear = -1;
        cu.addYear(endDate, startDate, intYear);
        break;
    case TIME_PERIOD_DAYS_6_MONTH:
        intMonth = -6;
        cu.addMonth(endDate, startDate, intMonth);
        break;
    case TIME_PERIOD_DAYS_3_MONTH:
        intMonth = -3;
        cu.addMonth(endDate, startDate, intMonth);
        break;
    case TIME_PERIOD_DAYS_1_MONTH:
        intMonth = -1;
        cu.addMonth(endDate, startDate, intMonth);
        break;
    case TIME_PERIOD_DAYS_2_WEEK:
        intDays = -14;
        cu.addDays(endDate, startDate, intDays);
        break;
    case TIME_PERIOD_DAYS_1_WEEK:
        intDays = -7;
        cu.addDays(endDate, startDate, intDays);
        break;
    default:
        break;
    }

    qDebug() << "startTime" << startDate;
    qDebug() << "endTime" << endDate;

    ui->setEndDateLineEditLSqrt->clear();
    ui->setStartDateLineEditLSqrt->clear();
    ui->setEndDateLineEditLSqrt->insert(endDate);
    ui->setStartDateLineEditLSqrt->insert(startDate);

}



/*******************************************************************
 *
 * Function:    initTimePeriodCtrls()
 *
 * Description:
 *
 *
 *******************************************************************/
void LeastSquaresTaDlg::initTimePeriodCtrls(int value)
{
    CUtil cu;

    // Init to valid date
    cu.getCurrentDate(m_startDate);
    cu.getCurrentDate(m_endDate);
    m_endDateRef = m_endDate;


    m_timePeriodDaysInc = value;
    ui->timePeriodLlineEditLSqrt->clear();
    ui->timePeriodLlineEditLSqrt->insert((QString)m_timePeriodDaysArr[value].TxtTimePeriod.toLatin1());
    ui->selTimePeriodSliderLSqrt->setValue(value);
    setTimePeriodDaysUpdateStartStopDate(m_startDate, m_endDate, value);
}



/*******************************************************************
 *
 * Function:    clearStockAndIndicatorTempMem()
 *
 * Description:
 *
 *
 *******************************************************************/
void LeastSquaresTaDlg::clearStockAndIndicatorTempMem(void)
{
    m_stockData.data.x.clear();
    m_stockData.data.y.clear();
    m_stockData.data.xDate.clear();
    m_stockData.data.indicator1.clear();
    m_stockData.data.indicator2.clear();
    m_stockData.data.indicator3.clear();
    m_stockData.data.indicator4.clear();
    m_stockData.data.indicator5.clear();

    m_stockArr.clear();
}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void LeastSquaresTaDlg::initMinMaxAxis(CYahooStockPlotUtil::MinMaxAxisValues_ST inputAxis,
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


/*******************************************************************
 *
 * Function:    setAxisTitles()
 *
 * Description:
 *
 *
 *******************************************************************/
void LeastSquaresTaDlg::
setAxisTitles(QString titleXAxis, QString titleYAxis, QwtPlot *qwtPlotLSqr,bool showXAxis)
{
    if(showXAxis==true)
        m_plot.setXAxisTitle(qwtPlotLSqr, titleXAxis);

    m_plot.setYAxisTitle(qwtPlotLSqr, titleYAxis);

}




/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void LeastSquaresTaDlg::addStockDataToPlot(DataPlotIndex_ET plotIndex,
                                    QString legendLable)
{
    m_yStockArr.insert(0, m_stockData);

    // This is the first added graph
    m_plot.addStockToPlot(m_qwtStockPlotData, m_yStockArr[0], m_qwtStockPlotData.axis, plotIndex, legendLable);

   //  m_plot.setRightLegend(ui->qwtPlotLSqr);

    m_plot.plotData(m_qwtStockPlotData, ui->qwtPlotLSqr, m_qwtStockPlotData.nofStocksToPlot-1);

    m_plot.removeAllStockArrData(m_yStockArr);

}



/*******************************************************************
 *
 * Function:    displayStockData()
 *
 * Description:
 *
 *
 *******************************************************************/
void LeastSquaresTaDlg::displayStockData(bool addLastPrice, double lastPrice)
{
    CDbHndl db;
    CYahooStockPlotUtil cy;
    DataPlotIndex_ET plotIndex;
    SubPlotIndex_ET subplotIndex;


    QString legendLable;
    QString status;
    int indicatorIndex;

    QString titleXAxis;
    CYahooStockPlotUtil::MinMaxAxisValues_ST mainPlotAxis;
    CYahooStockPlotUtil::MinMaxAxisValues_ST subPlotAxis;

    // m_macdHist = new QwtPlotHistogram ();


    deleteAllPlotData(m_qwtStockPlotData, ui->qwtPlotLSqr);
    deleteAllPlotData(m_qwtsubPlotData, ui->qwtPlotLSqr_2);

    m_macdHistData.clear();
    m_macdHist->detach();
    m_macdHist->setData(NULL);


    QString startDate = ui->setStartDateLineEditLSqrt->text();
    QString endDate = ui->setEndDateLineEditLSqrt->text();

    m_qwtStockPlotData.nofStocksToPlot = 0;

    // This variable is automaticly updated when first data is handled by UpdateMinMaxAxis()
    m_qwtStockPlotData.axis.minMaxIsInit = false;
    clearStockAndIndicatorTempMem();

    db.getYahooTaPriceData(m_reqStockSymbol, startDate, endDate, m_qwtStockPlotData, m_stockData);
    initMinMaxAxis(m_qwtStockPlotData.axis, mainPlotAxis);

    if(addLastPrice == true)
    {
        QString date("");
        cy.addLastOnePlotData(m_stockData, m_qwtStockPlotData, lastPrice, date);
    }

    titleXAxis = "Tid (dagar)";
    legendLable = "Pris (SEK)";
    setAxisTitles(titleXAxis, legendLable,ui->qwtPlotLSqr);

    plotIndex = PLOT_INDEX_PRICE;
    addStockDataToPlot(plotIndex, legendLable);


    //=====

    legendLable.clear();
    if(true == ui->checkBoxLinjerRegLSqrt->isChecked())
    {
        clearStockAndIndicatorTempMem();
        QString kValue;
        QString rValue;
        updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);
        db.getYahooTaLeastSqrFit(m_reqStockSymbol, startDate, endDate, m_qwtStockPlotData, m_stockData, kValue, rValue);
        updateMinMaxXAxisScales(m_qwtStockPlotData.axis, mainPlotAxis);

        indicatorIndex = 1;
        plotIndex = PLOT_INDEX_LINJER_REG;
        legendLable = "Linjer Regression";

        status = "K= ";
        status += kValue;
        status += ", R= ";
        status += rValue;

        // ui->StatusInfoLabel->setText(status);

        addStockIndicatorToPlot(indicatorIndex, plotIndex, legendLable, mainPlotAxis, ui->qwtPlotLSqr, m_qwtStockPlotData);
    }



    legendLable.clear();
    // Check the checkbox
    if(true == ui->checkBoxShortMvAvgLSqrt->isChecked())
    {
        clearStockAndIndicatorTempMem();

        qDebug() << "xx size " << m_stockData.data.x.size();
        updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);
        db.getYahooTaMovingAverage(m_reqStockSymbol, startDate, endDate, 20, m_qwtStockPlotData, m_stockData);
        updateMinMaxXAxisScales(m_qwtStockPlotData.axis, mainPlotAxis);

        indicatorIndex = 1;
        plotIndex = PLOT_INDEX_SHORT_MOVING_AVG;
        legendLable = "Short moving average";

        addStockIndicatorToPlot(indicatorIndex, plotIndex, legendLable, mainPlotAxis, ui->qwtPlotLSqr, m_qwtStockPlotData);
    }


    // Check the checkbox
    legendLable.clear();
    if(true == ui->checkBoxMidMvAvgLSqrt->isChecked())
    {
        /// updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);
        clearStockAndIndicatorTempMem();

        qDebug() << "xx size " << m_stockData.data.x.size();
        updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);
        db.getYahooTaMovingAverage(m_reqStockSymbol, startDate, endDate, 50, m_qwtStockPlotData, m_stockData);
        updateMinMaxXAxisScales(m_qwtStockPlotData.axis, mainPlotAxis);

        indicatorIndex = 1;
        plotIndex = PLOT_INDEX_MID_MOVING_AVG;
        legendLable = "Mid moving average";

        addStockIndicatorToPlot(indicatorIndex, plotIndex, legendLable, mainPlotAxis, ui->qwtPlotLSqr, m_qwtStockPlotData);
    }



    // Check the checkbox
    legendLable.clear();
    if(true == ui->checkBoxLongMvAvgLSqrt->isChecked())
    {
        clearStockAndIndicatorTempMem();
        qDebug() << "xx size " << m_stockData.data.x.size();
        updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);
        db.getYahooTaMovingAverage(m_reqStockSymbol, startDate, endDate, 150, m_qwtStockPlotData, m_stockData);
        updateMinMaxXAxisScales(m_qwtStockPlotData.axis, mainPlotAxis);

        indicatorIndex = 1;
        plotIndex = PLOT_INDEX_LONG_MOVING_AVG;
        legendLable = "Long moving average";

        addStockIndicatorToPlot(indicatorIndex, plotIndex, legendLable, mainPlotAxis, ui->qwtPlotLSqr, m_qwtStockPlotData);
    }



    // Check the checkboxes
    legendLable.clear();
    if(true == ui->checkBoxBollingBandLSqrt->isChecked())
    {
        clearStockAndIndicatorTempMem();

        qDebug() << "xx size " << m_stockData.data.x.size();
        updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);
        db.getYahooTaBoillingerBand(m_reqStockSymbol, startDate, endDate, 20, 2, m_qwtStockPlotData, m_stockData);
        updateMinMaxXAxisScales(m_qwtStockPlotData.axis, mainPlotAxis);

        // Lower Bollingerband
        indicatorIndex = 1;
        plotIndex = PLOT_INDEX_BOLLINGERBAND_LOW;
        legendLable = "Lower bollingerband";

        addStockIndicatorToPlot(indicatorIndex, plotIndex, legendLable, mainPlotAxis, ui->qwtPlotLSqr, m_qwtStockPlotData);


        // Mid Bollingerband
        indicatorIndex++;
        plotIndex = PLOT_INDEX_BOLLINGERBAND_MID;
        legendLable = "Mid bollingerband";


        m_stockArr.clear();
        addStockIndicatorToPlot(indicatorIndex, plotIndex, legendLable, mainPlotAxis, ui->qwtPlotLSqr, m_qwtStockPlotData);

        // Upper Bollingerband
        indicatorIndex++;
        plotIndex = PLOT_INDEX_BOLLINGERBAND_UPPER;
        legendLable = "Upper bollingerband";

        m_stockArr.clear();
        addStockIndicatorToPlot(indicatorIndex, plotIndex, legendLable, mainPlotAxis, ui->qwtPlotLSqr, m_qwtStockPlotData);
    }




    //=========
    if(true == ui->checkBoxVolume->isChecked())
    {
        // Get Volume
        legendLable.clear();

        clearStockAndIndicatorTempMem();
        subPlotAxis.minMaxIsInit = false;
        m_qwtsubPlotData.axis.minMaxIsInit = false;
        db.getYahooTaVolume(m_reqStockSymbol, startDate, endDate, m_qwtsubPlotData, m_stockData);

        subPlotAxis.maxX = mainPlotAxis.maxX;
        subPlotAxis.minX = mainPlotAxis.minX;
        m_qwtsubPlotData.axis.maxX = mainPlotAxis.maxX;
        m_qwtsubPlotData.axis.minX = mainPlotAxis.minX;
        m_qwtsubPlotData.axis.minMaxIsInit = true;

        setAxisTitles(titleXAxis, "(%)",ui->qwtPlotLSqr_2, false);
        subplotIndex = SUBPLOT_INDEX_VOLUME;
        legendLable = "";
        indicatorIndex = 1;
        addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET)subplotIndex, legendLable, subPlotAxis, ui->qwtPlotLSqr_2, m_qwtsubPlotData);
        updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);
    }
    else
    {
        legendLable.clear();
        if(ui->radioButMacd->isChecked() == true)
        {
            clearStockAndIndicatorTempMem();
            db.getYahooTaMacd(m_reqStockSymbol, startDate, endDate, 26, 12, 9, m_qwtsubPlotData, m_stockData);
            // db.getYahooTaMacd(m_reqStockSymbol, startDate, endDate, 52, 24, 8, m_qwtsubPlotData, m_stockData);


            setAxisTitles(titleXAxis, " ",ui->qwtPlotLSqr_2, false);
            subplotIndex = SUBPLOT_MACD_LINE;
            legendLable = "MACD";
            indicatorIndex = 1;
            addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET)subplotIndex, legendLable, subPlotAxis, ui->qwtPlotLSqr_2, m_qwtsubPlotData);
            updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);

            subPlotAxis.maxX = mainPlotAxis.maxX;
            subPlotAxis.minX = mainPlotAxis.minX;
            m_qwtsubPlotData.axis.maxX = mainPlotAxis.maxX;
            m_qwtsubPlotData.axis.minX = mainPlotAxis.minX;


            setAxisTitles(titleXAxis, " ",ui->qwtPlotLSqr_2, false);
            subplotIndex = SUBPLOT_MACD_SIGNAL;
            legendLable = "MACD signal";
            indicatorIndex = 2;
            addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET)subplotIndex, legendLable, subPlotAxis, ui->qwtPlotLSqr_2, m_qwtsubPlotData);
            updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);



            setAxisTitles(titleXAxis, " ",ui->qwtPlotLSqr_2, false);
            subplotIndex = SUBPLOT_MACD_HISTOGRAM;
            legendLable = "MACD Hist";
            indicatorIndex = 3;
            addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET)subplotIndex, legendLable, subPlotAxis, ui->qwtPlotLSqr_2, m_qwtsubPlotData);
            updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);


            m_macdHistData.clear();
            m_macdHist->detach();
            m_macdHist->setData(NULL);

            for(int ii = 0; ii < m_stockData.data.indicator3.size(); ii++)
            {
                m_stockData.data.x.at(ii);
                m_macdHistData.append (QwtIntervalSample (m_stockData.data.indicator3.at(ii),m_stockData.data.x.at(ii),m_stockData.data.x.at(ii)+1));
            }

            m_macdHist->setBrush(Qt::red);
            m_macdHist->setPen( QPen( Qt::black) );
            m_macdHist->setSamples(m_macdHistData);
            m_macdHist->attach(ui->qwtPlotLSqr_2);
        }

        legendLable.clear();
        if(true == ui->radioButRoc->isChecked())
        {
            clearStockAndIndicatorTempMem();
            subPlotAxis.minMaxIsInit = false;
            m_qwtsubPlotData.axis.minMaxIsInit = false;
            db.getYahooTaRateOfChange(m_reqStockSymbol, startDate, endDate, 10, m_qwtsubPlotData, m_stockData);

            setAxisTitles(titleXAxis, "(%)",ui->qwtPlotLSqr_2, false);
            subplotIndex = SUBPLOT_RATE_OF_CHANGE;
            legendLable = "Rate of change";
            indicatorIndex = 1;
            addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET)subplotIndex, legendLable, subPlotAxis, ui->qwtPlotLSqr_2, m_qwtsubPlotData);
            updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);

            subplotIndex = SUBPLOT_RATE_OF_CHANGE_LIMIT_100;
            legendLable = " ";
            indicatorIndex = 2;
            addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET)subplotIndex, legendLable, subPlotAxis, ui->qwtPlotLSqr_2, m_qwtsubPlotData);
            updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);
        }


        // RSI = Relative Strength Index
        legendLable.clear();
        if(true == ui->radioButRSI->isChecked())
        {
            clearStockAndIndicatorTempMem();
            subPlotAxis.minMaxIsInit = false;
            m_qwtsubPlotData.axis.minMaxIsInit = false;
            db.getYahooTaRsi(m_reqStockSymbol, startDate, endDate, 14, m_qwtsubPlotData, m_stockData);
            m_qwtsubPlotData.axis.maxY=100;
            m_qwtsubPlotData.axis.minY=0;
            m_qwtsubPlotData.axis.minMaxIsInit = true;

            subplotIndex = SUBPLOT_RSI_LIMIT_30;
            legendLable = "";
            indicatorIndex = 2;
            addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET)subplotIndex, legendLable, subPlotAxis, ui->qwtPlotLSqr_2, m_qwtsubPlotData);
            updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);


            subplotIndex = SUBPLOT_RSI_LIMIT_70;
            legendLable = "";
            indicatorIndex = 3;
            addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET)subplotIndex, legendLable, subPlotAxis, ui->qwtPlotLSqr_2, m_qwtsubPlotData);
            updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);

            setAxisTitles(titleXAxis, "(%)",ui->qwtPlotLSqr_2, false);
            subplotIndex = SUBPLOT_RSI;
            legendLable = "";
            indicatorIndex = 1;
            addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET)subplotIndex, legendLable, subPlotAxis, ui->qwtPlotLSqr_2, m_qwtsubPlotData);
            updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);


        }

        legendLable.clear();
        if(true == ui->radioButStochastics->isChecked())
        {
            clearStockAndIndicatorTempMem();
            db.getYahooTaStochastics(m_reqStockSymbol, startDate, endDate,  9, 3, m_qwtsubPlotData, m_stockData);

            setAxisTitles(titleXAxis, "(%)",ui->qwtPlotLSqr_2, false);
            subplotIndex = SUBPLOT_STOCHASTIC_LIMIT_20;
            legendLable = "";

            indicatorIndex = 3;
            addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET)subplotIndex, legendLable, subPlotAxis, ui->qwtPlotLSqr_2, m_qwtsubPlotData);
            updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);

            legendLable = "";
            indicatorIndex = 4;
            subplotIndex = SUBPLOT_STOCHASTIC_LIMIT_80;
            addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET) subplotIndex, legendLable, subPlotAxis, ui->qwtPlotLSqr_2, m_qwtsubPlotData);
            updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);

            legendLable = "Stochastics D";
            indicatorIndex = 2;
            subplotIndex = SUBPLOT_STOCHASTIC_D;
            addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET) subplotIndex, legendLable, subPlotAxis, ui->qwtPlotLSqr_2, m_qwtsubPlotData);
            updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);

            legendLable = "Stochastics K";
            indicatorIndex = 1;
            subplotIndex = SUBPLOT_STOCHASTIC_K;

            addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET) subplotIndex, legendLable, subPlotAxis, ui->qwtPlotLSqr_2, m_qwtsubPlotData);
            updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);
        }

        legendLable.clear();
        if(true == ui->radioButMomentum->isChecked())
        {
            clearStockAndIndicatorTempMem();
            db.getYahooTaMomentum(m_reqStockSymbol, startDate, endDate, 20, m_qwtsubPlotData, m_stockData);

            indicatorIndex = 2;
            subplotIndex = SUBPLOT_INDEX_MOMENTUM_ZERO_LINE;
            addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET)subplotIndex, legendLable, subPlotAxis, ui->qwtPlotLSqr_2, m_qwtsubPlotData);
            updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);

            indicatorIndex = 1;
            subplotIndex = SUBPLOT_INDEX_MOMENTUM;
            legendLable = "Momentum";

            addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET) subplotIndex, legendLable, subPlotAxis, ui->qwtPlotLSqr_2, m_qwtsubPlotData);
            updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);
        }


    }

    //  boxesUpdate edit
    ui->lineEditAssetNameLSqrt->clear();
    ui->lineEditAssetNameLSqrt->insert(m_reqStockName);
    // QPalette *palette  = new QPalette;
    //MyLineEdit::setTxtColor(ui->lineEditAssetNameLSqrt, palette, Qt::yellow);


    // Shows actually start stop date recevived from database
    ui->lineEditStartDateLSqrt->clear();
    ui->lineEditStartDateLSqrt->insert(m_qwtStockPlotData.axis.minXDate);

    ui->lineEditStopDateLSqrt->clear();
    ui->lineEditStopDateLSqrt->insert(m_qwtStockPlotData.axis.maxXDate);


    // Reset zoomer
    m_plot.initPlotPicker(ui->qwtPlotLSqr);
}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void LeastSquaresTaDlg::addStockIndicatorToPlot(int indicatorIndex,
                                                DataPlotIndex_ET plotIndex,
                                                QString legendLable,
                                                CYahooStockPlotUtil::MinMaxAxisValues_ST &axis,
                                                QwtPlot *qwtPlot,
                                                 CYahooStockPlotUtil::PlotData_ST &qwtPlotData)
{
        m_yStockArr.insert(0, m_stockData);

        m_plot.addIndicatorToPlot(qwtPlotData, m_yStockArr[0], qwtPlotData.axis, indicatorIndex, plotIndex, legendLable);

        // m_plot.setRightLegend(qwtPlot);

        updateMinMaxXAxisScales(axis, qwtPlotData.axis);

        m_plot.plotData(qwtPlotData, qwtPlot, plotIndex);

        m_plot.removeAllStockArrData(m_yStockArr);
}


/*******************************************************************
 *
 * Function:    updateMinMaxXAxisScales()
 *
 * Description:
 *
 *
 *******************************************************************/
void LeastSquaresTaDlg::
updateMinMaxXAxisScales(CYahooStockPlotUtil::MinMaxAxisValues_ST &axis,
                        CYahooStockPlotUtil::MinMaxAxisValues_ST &qwtPlotAxis)
{
    CYahooStockPlotUtil cy;

    // Update min max scale of x and y axis
    cy.updateMinMaxAxis(axis, qwtPlotAxis.maxX, qwtPlotAxis.maxY, qwtPlotAxis.maxXDate);
    cy.updateMinMaxAxis(axis, qwtPlotAxis.minX, qwtPlotAxis.minY, qwtPlotAxis.minXDate);

    cy.updateMinMaxAxis(qwtPlotAxis, axis.maxX, axis.maxY, axis.maxXDate);
    cy.updateMinMaxAxis(qwtPlotAxis, axis.minX, axis.minY, axis.minXDate);

}





/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void LeastSquaresTaDlg::addStatusDateList(QString stockSymbol, bool isUpdate)
{
    NameKey data;
    data.symbolName = stockSymbol;
    data.dataIsUpdated = isUpdate;
    m_dateIsUpdatedList.append(data);

}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void LeastSquaresTaDlg::modifyDateList(QString stockSymbol, bool isUpdate)
{
    NameKey node;
     QMutableListIterator <NameKey> iterator(m_dateIsUpdatedList);
     while(iterator.hasNext())
     {
        node = iterator.next();
        if(node.symbolName.compare(stockSymbol)== 0)
        {
            //iterator.remove();
            addStatusDateList(stockSymbol, isUpdate);

            QString tmp;
            tmp.sprintf("%d",isUpdate);
            qDebug() << tmp;
            qDebug() << stockSymbol;
            node.dataIsUpdated = isUpdate;
            iterator.setValue(node);
            return;
        }

     }
}


/*******************************************************************
 *
 * Function:    startResendTimer()
 *
 * Description: This function activate resend timer that is used when
 *              request data from Yahoo server.
 *
 *              This is a single shot timer.
 *
 *******************************************************************/
void LeastSquaresTaDlg::startReqSingleStockDataTimeoutTimer(int ms)
{
    QTimer::singleShot(ms, this, SLOT(slotReqSingleStockDataTimerExpired()));
    qDebug() << "startReqSingleStockDataTimeoutTimer()";

}




/*******************************************************************
 *
 * Function:    slotImportSingelStockDataIsFinish()
 *
 * Description: This function is invoked when singel stock data is
 *              parsed by: m_importYahooTaDataThread
 *
 *
 *
 *******************************************************************/
void LeastSquaresTaDlg::slotImportSingelStockDataIsFinish(int dummy)
{
    dummy = dummy;
    if(m_singleStockDataReqStatus == STATUS_REQ_SINGLE_STOCK_PROCESSING)
    {
        m_singleStockDataReqStatus = STATUS_REQ_SINGLE_STOCK_IDLE;
        modifyDateList(m_reqStockSymbol, true);
        qDebug() << m_reqStockSymbol;
        displayStockData();
    }

}




/*******************************************************************
 *
 * Function:    slotReqSingleStockDataFromServer()
 *
 * Description: This function is invoked when singel stock data need
 *              to be updated. (price data is old)
 *
 *******************************************************************/
void LeastSquaresTaDlg::slotReqSingleStockDataFromServer()
{
    QString qry;
    CUtil cu;

    QString filename = DWLD_PATH_TA_LIST_FILE;

//    QString startDate;
    QString startYear;
    QString startMonth;
    QString startDay;

  //  QString endDate;
    QString endYear;
    QString endMonth;
    QString endDay;


    if(m_singleStockDataReqStatus == STATUS_REQ_SINGLE_STOCK_PENDING)
    {
        m_singleStockDataReqStatus = STATUS_REQ_SINGLE_STOCK_PROCESSING;

        QByteArray ba1 = m_reqStockSymbol.toLocal8Bit();
        const char *c_reqStockSymbol = ba1.data();

  //      m_reqStartDate = "2013-12-01";

        cu.splitDate(m_reqStartDate, startYear, startMonth, startDay);

        cu.splitDate(m_reqEndDate, endYear, endMonth, endDay);

        qry.sprintf("http://ichart.finance.yahoo.com/table.csv?s=%s&d=%d&e=%d&f=%d&g=d&a=%d&b=%d&c=%d&ignore=.csv",
                    c_reqStockSymbol,
                    (endMonth.toInt()-1),
                    endDay.toInt(),
                    endYear.toInt(),
                    (startMonth.toInt()-1),
                    startDay.toInt(),
                    startYear.toInt());

    //    qDebug() << startDate;
        qDebug() << startYear;
        qDebug() << startMonth;
        qDebug() << startDay;

      //  qDebug() << endDate;
        qDebug() << endYear;
        qDebug() << endMonth;
        qDebug() << endDay;

        qDebug() << qry;


        QObject::connect(&m_hw1, SIGNAL(sendSignalTextToDlg2(int)), this, SLOT(slotReceivedAssetTaDataFromServer(int)));
        QUrl url(qry);

        startReqSingleStockDataTimeoutTimer(TIME_2_MIN);
        m_hw1.startRequest(url, filename, 0x01);
    }
}



/*******************************************************************
 *
 * Function:    on_pushButtonImportAllData_clicked()
 *
 * Description:
 *
 *
 *******************************************************************/
void LeastSquaresTaDlg::
prepReqTaDataFromServer(QString stockName, QString stockSymbol, QString startDate, QString endDate)
{

    if(m_singleStockDataReqStatus == STATUS_REQ_SINGLE_STOCK_PENDING)
    {
        m_reqStartDate = startDate;
        m_reqEndDate = endDate;
        m_reqStockSymbol = stockSymbol;
        m_reqStockName = stockName;

        qDebug() << m_reqStartDate;
        qDebug() << m_reqEndDate;
        qDebug() << m_reqStockSymbol;
        qDebug() << m_reqStockName;


        // Signal used when thread is finish importing data
       connect(m_importYahooTaDataThread, SIGNAL(emitImportSingelStockDataIsFinish(int)), this, SLOT(slotImportSingelStockDataIsFinish(int)));
       connect(this, SIGNAL(emitReqSingleStockDataFromServer()), this, SLOT(slotReqSingleStockDataFromServer()));

       emit emitReqSingleStockDataFromServer();

    }

}

/*******************************************************************
 *
 * Function:    slotReqSingleStockDataTimerExpired()
 *
 * Description: This function is invoked when resend timer has expired
 *
 *
 *******************************************************************/
void LeastSquaresTaDlg::slotReqSingleStockDataTimerExpired()
{
    if(m_singleStockDataReqStatus != STATUS_REQ_SINGLE_STOCK_IDLE)
    {
        m_singleStockDataReqStatus = STATUS_REQ_SINGLE_STOCK_IDLE;
        modifyDateList(m_reqStockSymbol, true);
        qDebug() << m_reqStockSymbol;
        QMessageBox::information(this, QString::fromUtf8("Timeout"), QString::fromUtf8("Timeout: ingen data kunde hämtas"));
    }

}



/*******************************************************************
 *
 * Function:    slotReceivedAssetTaDataFromServer()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void LeastSquaresTaDlg::slotReceivedAssetTaDataFromServer(int)
{
    if(m_singleStockDataReqStatus == STATUS_REQ_SINGLE_STOCK_PROCESSING)
    {
        // Init thread with data before starting it
        m_importYahooTaDataThread->setImportInfoSingleStock(m_reqStockName, m_reqStockSymbol);
        m_importYahooTaDataThread->start(QThread::NormalPriority);
    }
}



#if 0
/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void LeastSquaresTaDlg::
getSelStockListItem(QString &stockName, QString &stockSymbol, const QModelIndex &index)
{
    ui->treeWidget->setCurrentItem(ui->treeWidget->topLevelItem(index.row()));
    ui->treeWidget->setFocus();

    stockName.clear();
    stockSymbol.clear();


    stockName = ui->treeWidget->currentItem()->text(STOCKLIST_NAME);
    stockSymbol = ui->treeWidget->currentItem()->text(STOCKLIST_SYMBOL);

}
#endif


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void LeastSquaresTaDlg::findDateList(QString stockSymbol, bool &isUpdate)
{
    isUpdate = false;

    NameKey node;
     QMutableListIterator <NameKey> iterator(m_dateIsUpdatedList);
     while(iterator.hasNext())
     {
        node = iterator.next();
        qDebug() << node.symbolName;
        QString tmp;
        tmp.sprintf("%d", node.dataIsUpdated);
        qDebug() << tmp;
        if(node.symbolName.compare(stockSymbol)== 0)
        {
            isUpdate = node.dataIsUpdated;
            return;
        }

     }
}




/**********************************************************************************
 *
 * Function:    on_tableViewLeastSquare_clicked
 *
 * Description: Row clicked in table
 *
 *
 *
 * *******************************************************************************/
void LeastSquaresTaDlg::on_tableViewLeastSquare_clicked(const QModelIndex &index)
{
    CDbHndl db;
    QString assetName;
    CDbHndl::snapshotStockData_ST keyData;
    //QModelIndex dummyIndex = index;



    // Get data form table
    ui->tableViewLeastSquare->selectionModel()->currentIndex().row();
    int rowidx = ui->tableViewLeastSquare->currentIndex().row();
    assetName.append(ui->tableViewLeastSquare->model()->index(rowidx , 0).data().toString());

    db.openDb(PATH_JACK_STOCK_DB);
    if(true == db.companynameGetKeyDataUseBridge(assetName, keyData))
    {
        plotStockData(assetName, keyData.assetSymbol);
    }
    else
    {
        QMessageBox::information(NULL, QString::fromUtf8("Tabell"), QString::fromUtf8("Symbolen saknas"));
        return;
    }

}


/*******************************************************************
 *
 * Function:    on_selTimePeriodSlider_valueChanged()
 *
 * Description: This function is invoked when user change set
 *              time period slider.
 *
 *
 *******************************************************************/
void LeastSquaresTaDlg::plotStockData(QString inStockName, QString inStockSymbol)
{
    //==============
    QString stockName;
    QString stockSymbol;
    CDbHndl cd;
    CUtil cu;
    QString lastDbDate;
    QString currDate;
    // ajn 160214 QString tmp;
    int nofDays;
    // ajn 160214 NordnetCompanyDescription_ST data;


    // Are we already processing data?
    if(m_singleStockDataReqStatus == STATUS_REQ_SINGLE_STOCK_IDLE)
    {
        m_singleStockDataReqStatus = STATUS_REQ_SINGLE_STOCK_PENDING;
        // getSelStockListItem(stockName, stockSymbol, index);

        // ajn 160214
        stockSymbol = inStockSymbol;
        stockName = inStockName;

        m_reqStockSymbol = stockSymbol;
        m_reqStockName = stockName;

        qDebug() << m_reqStockSymbol;
        qDebug() << m_reqStockName;

        CDbHndl::snapshotStockData_ST keyFaData;




        #if 1
        // ajn 160215 ui->lineEditPELSqrt->setDisabled(false);
        // ajn 160215 ui->lineEditPE_2->setDisabled(false);


        // ajn 160215 ui->lineEditMoreInfoComapnyName->clear();
        // ajn 160215 ui->lineEditMoreInfoexecutiveDirector->clear();
        // ajn 160215 ui->lineEditMoreInfoBransch->clear();
        // ajn 160215 ui->lineEditMoreInfoSector->clear();
        // ajn 160215 ui->textEditMoreInfobusinessDescription->clear();

        //ui->lineEditMoreInfoComapnyName_2->clear();
        // ajn 160215 ui->lineEditMoreInfoexecutiveDirector_2->clear();
        // ajn 160215 ui->lineEditMoreInfoBransch_2->clear();
        // ajn 160215 ui->lineEditMoreInfoSector_2->clear();
        // ajn 160215 ui->textEditMoreInfobusinessDescription_2->clear();


        m_mle.reset5Ctrl(ui->lineEditEarningsDivByDividendLSqrt,
                               ui->lineEditNavDivLastStockPriceLSqrt,
                               ui->lineEditPELSqrt,
                               ui->lineEditPsLSqrt,
                               ui->lineEditYieldLSqrt);

#if 0
        ui->lineEditEarningsDivByDividendLSqrt->clear();
        ui->lineEditNavDivLastStockPriceLSqrt->clear();
        ui->lineEditPELSqrt->clear();
        ui->lineEditPsLSqrt->clear();
        // ajn 160215 ui->lineEditRisk->clear();
        ui->lineEditYieldLSqrt->clear();
        // ajn 160215 ui->lineMeanReturn->clear();
#endif

        // ajn 160215 ui->lineEditEarningsDivByDividend_2->clear();
        // ajn 160215 ui->lineEditNavDivLastStockPrice_2->clear();
        // ajn 160215 ui->lineEditPE_2->clear();
        // ajn 160215 ui->lineEditPs_2->clear();
        // ajn 160215 ui->lineEditYield_2->clear();

        // ajn 160215 ui->lineEditEarningsDivByDividend_3->clear();
        // ajn 160215 ui->lineEditNavDivLastStockPrice_3->clear();
        // ajn 160215 ui->lineEditPE_3->clear();
        // ajn 160215 ui->lineEditPs_3->clear();
        // ajn 160215 ui->lineEditYield_3->clear();

        // ajn 160215 ui->treeWidget_2->clear();
        // ajn 160215 ui->treeWidget_3->clear();
        // ajn 160215 ui->treeWidget_4->clear();
        ui->treeWidget_5LSqrt->clear();
        ui->treeWidget_6LSqrt->clear();
        // ajn 160215 QString assetType;
        // ajn 160215 assetType.clear();

        // ajn 160215  ui->assetType->clear();

        // ajn 160215 ui->labelLastPrice->clear();
        // ajn 160215 ui->labelOneDayProcentChange->clear();
        #endif

        #if 1
        if(m_startDate.isEmpty() == false && m_endDate.isEmpty() == false)
        {

            if(true == cd.getKeyDataUseBridge(m_reqStockSymbol,keyFaData))
            {
                // ajn 160215 qDebug() << keyFaData.companyName;

                #if 0
                if(true == cd.getNordnetCompanyDescription(data, keyFaData.companyName))
                {
                    ui->lineEditMoreInfoComapnyName->insert(data.assetName);
                    ui->lineEditMoreInfoexecutiveDirector->insert(data.executiveDirector);
                    ui->lineEditMoreInfoBransch->insert(data.bransch);
                    ui->lineEditMoreInfoSector->insert(data.sector);
                    ui->textEditMoreInfobusinessDescription->insertPlainText(data.businessDescription);


                    // ui->lineEditMoreInfoComapnyName_2->insert(data.assetName);
                    ui->lineEditMoreInfoexecutiveDirector_2->insert(data.executiveDirector);
                    ui->lineEditMoreInfoBransch_2->insert(data.bransch);
                    ui->lineEditMoreInfoSector_2->insert(data.sector);
                    ui->textEditMoreInfobusinessDescription_2->insertPlainText(data.businessDescription);

                }


                cd.addFa2operatingIncomeToTreeWidget(ui->treeWidget_2, keyFaData.companyName);
                cd.addFa2NetProfitAfterTaxToTreeWidget(ui->treeWidget_3, keyFaData.companyName, assetType);

                ui->assetType->setText(assetType);

                tmp.sprintf("Pris %s",keyFaData.lastPrice.toLocal8Bit().constData());
                ui->labelLastPrice->setText(tmp);
                tmp.sprintf("Idag %s \%%",keyFaData.procentChangeOneDay.toLocal8Bit().constData());
                ui->labelOneDayProcentChange->setText(tmp);

                cd.addFa3NetProfitAfterTaxToTreeWidget(ui->treeWidget_4, keyFaData.companyName);
                #endif

                cd.addFa3MinMaxPEAndPrice(ui->treeWidget_5LSqrt, ui->treeWidget_6LSqrt, keyFaData.companyName, m_reqStockSymbol);


                #if 0
                ui->lineEditEarningsDivByDividendLSqrt->insert(keyFaData.earningsDividedByDividend);
                ui->lineEditNavDivLastStockPriceLSqrt->insert(keyFaData.navDivLastStockPrice);
                ui->lineEditPELSqrt->insert(keyFaData.keyValuePE);
                ui->lineEditPsLSqrt->insert(keyFaData.keyValuePS);
                ui->lineEditYieldLSqrt->insert(keyFaData.keyValueYield);
                #endif


                // Add data set text color on 5 finance ctrls
                m_faCtrl.fa5AddData(keyFaData.earningsDividedByDividend, ui->lineEditEarningsDivByDividendLSqrt,
                                    keyFaData.navDivLastStockPrice,        ui->lineEditNavDivLastStockPriceLSqrt,
                                    keyFaData.keyValuePE,                  ui->lineEditPELSqrt,
                                    keyFaData.keyValuePS,                  ui->lineEditPsLSqrt,
                                    keyFaData.keyValueYield,               ui->lineEditYieldLSqrt);

                #if 0
                ui->lineEditEarningsDivByDividend_2->insert(keyFaData.earningsDividedByDividend);
                ui->lineEditNavDivLastStockPrice_2->insert(keyFaData.navDivLastStockPrice);
                ui->lineEditPE_2->insert(keyFaData.keyValuePE);
                ui->lineEditPs_2->insert(keyFaData.keyValuePS);
                ui->lineEditYield_2->insert(keyFaData.keyValueYield);

                ui->lineEditEarningsDivByDividend_3->insert(keyFaData.earningsDividedByDividend);
                ui->lineEditNavDivLastStockPrice_3->insert(keyFaData.navDivLastStockPrice);
                ui->lineEditPE_3->insert(keyFaData.keyValuePE);
                ui->lineEditPs_3->insert(keyFaData.keyValuePS);
                ui->lineEditYield_3->insert(keyFaData.keyValueYield);


            #endif
                CDbHndl::EfficPortStockData_ST stockRiskReturnData;
                stockRiskReturnData.stockSymbol = m_reqStockSymbol;
                #if 0
                if(true == cd.efficPortfCalcMeanAndStdDev(m_startDate, m_endDate, stockRiskReturnData))
                {
                    QString tmp;
                    tmp.sprintf("%.2f",stockRiskReturnData.riskStdDev * 100);
                    ui->lineEditRisk->insert(tmp);
                    tmp.sprintf("%.2f",stockRiskReturnData.meanReturns * 100);
                    ui->lineMeanReturn->insert(tmp);
                }
                #endif
            }
        }
        #endif


        if(true == cd.getLatestDateYahooTaData(lastDbDate, inStockSymbol))
        {
            cu.getCurrentDate(currDate);
            qDebug() << currDate;
            qDebug() << lastDbDate;


            // Check how many days we are missing and ned to get from yahoo
            cu.nofDaysBeteenDates(currDate, lastDbDate, nofDays);
            if(nofDays < -1)
            {
                bool isUpdate;

                findDateList(stockSymbol, isUpdate);
                QString tmp;
                tmp.sprintf("%d",isUpdate);
                qDebug() << tmp;
                qDebug() << stockSymbol;


                if(isUpdate == false)
                {
                    // Prepare request of data from Yahoo
                    qDebug() << stockName;
                    qDebug() << stockSymbol;
                    qDebug() << lastDbDate;
                    qDebug() << currDate;

                    prepReqTaDataFromServer(stockName, stockSymbol, lastDbDate, currDate);
                }
#if 0
                else
                {
                    m_singleStockDataReqStatus = STATUS_REQ_SINGLE_STOCK_IDLE;
                    displayStockData(false, keyFaData.lastPrice.toDouble());
                }
#endif
            }
            else
            {
                m_singleStockDataReqStatus = STATUS_REQ_SINGLE_STOCK_IDLE;
                displayStockData(false, keyFaData.lastPrice.toDouble());
            }
        }
        // No latest date was found lets add our own
        else
        {
            // Prepare request of data from Yahoo
            lastDbDate = "1992-01-01";
            cu.getCurrentDate(currDate);
            prepReqTaDataFromServer(stockName, stockSymbol, lastDbDate, currDate);
        }
    }
    else
    {
        m_singleStockDataReqStatus = STATUS_REQ_SINGLE_STOCK_IDLE;
        QMessageBox::information(this, QString::fromUtf8("Vänta"), QString::fromUtf8("Vänta Processar redan data.."));
    }

}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 *******************************************************************/
void LeastSquaresTaDlg::setFaEditControlTxtColor(QLineEdit *lineEdit, QPalette *palette, const QColor &color)
{
    palette->setColor(QPalette::Text, color); //Qt::red
    lineEdit->setPalette(*palette);
}






/*******************************************************************
 *
 * Function:    on_selTimePeriodSlider_valueChanged()
 *
 * Description: This function is invoked when user change set
 *              time period slider.
 *
 *
 *******************************************************************/
void LeastSquaresTaDlg::on_selTimePeriodSliderLSqrt_valueChanged(int value)
{
    m_timePeriodDaysInc = value;

    ui->timePeriodLlineEditLSqrt->clear();
    ui->timePeriodLlineEditLSqrt->insert((QString)m_timePeriodDaysArr[value].TxtTimePeriod.toLatin1());
    setTimePeriodDaysUpdateStartStopDate(m_startDate, m_endDate, value);
}


/*******************************************************************
 *
 * Function:    on_tableViewLeastSquare_2_clicked()
 *
 * Description: Invocked when user click on cell i table
 *
 *
 *
 *******************************************************************/
void LeastSquaresTaDlg::on_tableViewLeastSquare_2_clicked(const QModelIndex &index)
{
    CDbHndl db;
    QString assetName;
    CDbHndl::snapshotStockData_ST keyData;
    QModelIndex dummyIndex = index;



    // Get data form table
    ui->tableViewLeastSquare_2->selectionModel()->currentIndex().row();
    int rowidx = ui->tableViewLeastSquare_2->currentIndex().row();
    assetName.append(ui->tableViewLeastSquare_2->model()->index(rowidx , 0).data().toString());

    db.openDb(PATH_JACK_STOCK_DB);
    if(true == db.companynameGetKeyDataUseBridge(assetName, keyData))
    {
        plotStockData(assetName, keyData.assetSymbol);
    }
    else
    {
        QMessageBox::information(NULL, QString::fromUtf8("Tabell"), QString::fromUtf8("Symbolen saknas"));
        return;
    }

}




/*******************************************************************
 *
 * Function:    on_tableViewLeastSquare_2_clicked()
 *
 * Description: Invocked when user click on cell i table
 *
 *
 *
 *******************************************************************/
void LeastSquaresTaDlg::on_GetDbDataButton_clicked()
{
    int i;
    int rowPlus;
    int rowMinus;
    int j;
    int len;
    int len1;
    int nofCols;
    CDbHndl db;
    QString str;
    GuiFinanceColor gfc;
    CDbHndl::snapshotStockData_ST keyData;
    TaBuySellIdicator::SellSignalMovingAvgST sellSignals;
    TaBuySellIdicator::BuySignalMovingAvgST buySignals;
    TaBuySellIdicator::SellSignalMomentumST sellMomentumSignals;
    TaBuySellIdicator::BuySignalMomentumST buyMomentumSignals;

    int nofData = 0;
    double meanXSum = 0;
    double meanYSum = 0;
    double prodXXSum = 0;
    double prodYYSum = 0;
    double prodXYSum = 0;
    double k;
    double m;
    double r;

    QHeaderView *horizHeaderPlus;
    QHeaderView *horizHeaderMinus;

    CExtendedTable etPlus;
    CExtendedTable etMinus;
    TableColumnIndicatorInfo_ST headerList[CDbHndl::MAX_NOF_GPSEL_INDEX];

    CUtil cu;
    QString startDate;
    QString endDate;
    int intMonth = -12;


    etPlus.setHorizontalFont(ui->tableViewLeastSquare, "Helvetica", 8);
    etPlus.setVerticalFont(ui->tableViewLeastSquare, "Helvetica", 8);

    etMinus.setHorizontalFont(ui->tableViewLeastSquare_2, "Helvetica", 8);
    etMinus.setVerticalFont(ui->tableViewLeastSquare_2, "Helvetica", 8);



    nofCols = 0;

    headerList[nofCols++].name = QString::fromUtf8("Företag     ");
    headerList[nofCols++].name = QString::fromUtf8("Symbol      ");
    headerList[nofCols++].name = QString::fromUtf8("R");
    headerList[nofCols++].name = QString::fromUtf8("K");
    headerList[nofCols++].name = QString::fromUtf8("T Köp /\nSälj");
    headerList[nofCols++].name = QString::fromUtf8("M Köp /\nSälj");


    headerList[nofCols++].name = QString::fromUtf8("PE     ");
    headerList[nofCols++].name = QString::fromUtf8("PS     ");
    headerList[nofCols++].name = QString::fromUtf8("DirAvk");
    headerList[nofCols++].name = QString::fromUtf8("Vinst/\nDAvk");
    headerList[nofCols++].name = QString::fromUtf8("Sub/\nKurs");

    headerList[nofCols++].name = QString::fromUtf8("Förv.\nRisk");
    headerList[nofCols++].name = QString::fromUtf8("Förv.\nAvkst");


    cu.getCurrentDate(endDate);

    if(false == cu.addMonth(endDate, startDate, intMonth))
    {
        return;
    }



    m_mutex.lock();
    //db.openDb(PATH_JACK_STOCK_DB);


    // Init least sqrt
    db.init1dLeastSrq(nofData, meanXSum, meanYSum, prodXXSum, prodYYSum, prodXYSum);

    int len10 = m_stockArr.size();
    for( int i = 0; i < len10; i++)
    {
        m_stockArr[i].data.x.clear();
        m_stockArr[i].data.y.clear();
    }

    m_stockArr.clear();


    // Request snapshot data from database
    if(true == db.getAllSnapshotPlotStocksData(m_stockArr))
    {
        len = m_stockArr.size();

        // Init table
        etPlus.createTableModel(len, nofCols, this);
        etPlus.addHeaders(ui->tableViewLeastSquare, headerList, nofCols);


        etMinus.createTableModel(len, nofCols, this);
        etMinus.addHeaders(ui->tableViewLeastSquare_2, headerList, nofCols);

        // Init columm head click event handlers for tableView
        horizHeaderPlus= ui->tableViewLeastSquare->horizontalHeader();
        horizHeaderMinus= ui->tableViewLeastSquare_2->horizontalHeader();

        connect(horizHeaderPlus,  SIGNAL(sectionClicked(int)), this, SLOT(tablePlusHeaderClicked(int)));
        connect(horizHeaderMinus, SIGNAL(sectionClicked(int)), this, SLOT(tableMinusHeaderClicked(int)));

        for(i = 0; i < FA_NOF_DATA; i++ )
        {
            m_faDataPalette[i] = new QPalette();
        }

        // Used by risk & returns plot
        removeQwtPlotArrMemSpace();
        createQwtPlotArrMemSpace(len);


        // One day is added first
        for(rowPlus = 0, rowMinus = 0, j = 0; j < len; j++)
        {
            len1 = m_stockArr[j].data.x.count();
            // Is there enough data to calc least square on?
            if(len1 < 2)
            {
                break;
            }

            if(len1 > 6)
            {
                len1 = 6;
            }

            for(i = len1 -1; i >= 0; i--)
            {
                // m_stockArr[j].data.x[i];
                // m_stockArr[j].data.y[i];
                db.gather1dLeastSrqData(m_stockArr[j].data.x[i],
                                     m_stockArr[j].data.y[i],
                                     nofData,
                                     meanXSum,
                                     meanYSum,
                                     prodXXSum,
                                     prodYYSum,
                                     prodXYSum);
            }




            if(true == db.calc1dLeastSrqFitRParam(nofData,prodXXSum, prodYYSum, prodXYSum, r) || (true == ui->showAllCheckBox->isChecked()))
            {
                if(true == db.calc1dLeastSrqFitParams(nofData, meanXSum, meanYSum, prodXXSum, prodYYSum, prodXYSum, m, k) || (true == ui->showAllCheckBox->isChecked()))
                {

                    if(r > 0.7 || (true == ui->showAllCheckBox->isChecked()))
                    {
                        // Add stock symbol Column 1
                        if(true == db.companynameGetKeyDataUseBridge(m_stockArr[j].stockName, keyData))
                        {
                            Qt::GlobalColor color1 = Qt::black;
                            Qt::GlobalColor riskReturnColor = Qt::magenta;

                            CDbHndl::EfficPortStockData_ST riskReturnData;
                            riskReturnData.stockSymbol = keyData.assetSymbol;

                            calcRiskAndReturn(startDate, endDate, riskReturnData);

                            riskReturnColor = gfc.getColorRiskReturns(riskReturnData.riskStdDev, riskReturnData.meanReturns);

                            QString riskStr;
                            riskStr.sprintf("%.3f", riskReturnData.riskStdDev);

                            QString returnStr;
                            returnStr.sprintf("%.3f", riskReturnData.meanReturns);

                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, riskStr, rowPlus, 11, riskReturnColor);
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, returnStr, rowPlus, 12, riskReturnColor);

                            gfc.getColorPe(keyData.keyValuePE, color1);
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.keyValuePE, rowPlus, 6, color1);

                            color1 = Qt::black;
                            gfc.getColorPs(keyData.keyValuePS, color1);
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.keyValuePS, rowPlus, 7, color1);

                            gfc.getColorYield(keyData.keyValueYield, keyData.earningsDividedByDividend, color1);
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.keyValueYield, rowPlus, 8, color1);

                            gfc.getColorEarningsDivDividend(keyData.earningsDividedByDividend, color1);
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.earningsDividedByDividend, rowPlus, 9, color1);

                            color1 = Qt::black;
                            gfc.getColorNavDivStockPrice(keyData.navDivLastStockPrice, color1);
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.navDivLastStockPrice, rowPlus, 10, color1);

                            // Asset symbol
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.assetSymbol, rowPlus, 1, Qt::black);


                            if( true == m_taBuy.getAvgBuySellSignals(keyData.assetSymbol,
                                                                     sellSignals,
                                                                     buySignals,
                                                                     sellMomentumSignals,
                                                                     buyMomentumSignals))
                            {
                                QString buySignal;
                                buySignal = m_taBuy.convAvgBuySignalToNumber(buySignals);
                                if(buySignal.compare(QString::fromUtf8("0")) != 0)
                                {
                                    etPlus.addDataSetTextColor(ui->tableViewLeastSquare, buySignal, rowPlus, 4, Qt::blue);
                                }

                                QString sellSignal;
                                sellSignal = m_taBuy.convAvgSellSignalToNumber(sellSignals);
                                if(sellSignal.compare(QString::fromUtf8("0")) != 0)
                                {
                                    etPlus.addDataSetTextColor(ui->tableViewLeastSquare, sellSignal, rowPlus, 4, Qt::red);
                                }

                                //===============================================================
                                // Momentum sell buy signal
                                //===============================================================
                                buySignal = m_taBuy.convMomentumBuySignalToNumber(buyMomentumSignals);
                                if(buySignal.compare(QString::fromUtf8("0")) != 0)
                                {
                                    etPlus.addDataSetTextColor(ui->tableViewLeastSquare, buySignal, rowPlus, 5, Qt::blue);
                                }

                                sellSignal = m_taBuy.convMomentumSellSignalToNumber(sellMomentumSignals);
                                if(sellSignal.compare(QString::fromUtf8("0")) != 0)
                                {
                                    etPlus.addDataSetTextColor(ui->tableViewLeastSquare, sellSignal, rowPlus, 5, Qt::red);
                                }

                            }
                        }
                        else
                        {
                            keyData.assetSymbol = "";
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.assetSymbol, rowPlus, 1, Qt::black);
                        }

                        // Add stockname column 0
                        etPlus.addDataSetTextColor(ui->tableViewLeastSquare, m_stockArr[j].stockName, rowPlus, 0, Qt::black);

                        // Add R on column 2
                        str.sprintf("%.2f", r);
                        etPlus.addDataSetTextColor(ui->tableViewLeastSquare, str, rowPlus, 2, Qt::black);

                        // Add K on column 3
                        str.sprintf("%.2f", k);
                        if(k >= 0)
                        {
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, str, rowPlus, 3, Qt::blue);
                        }
                        else
                        {
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, str, rowPlus, 3, Qt::red);
                        }
                        rowPlus++;
                    }
                    else if(r < -0.7)
                    {
                        // Add stock symbol Column 1
                        if(true == db.companynameGetKeyDataUseBridge(m_stockArr[j].stockName, keyData))
                        {

                            Qt::GlobalColor color1 = Qt::black;
                        Qt::GlobalColor riskReturnColor = Qt::magenta;

                        CDbHndl::EfficPortStockData_ST riskReturnData;
                        riskReturnData.stockSymbol = keyData.assetSymbol;

                        calcRiskAndReturn(startDate, endDate, riskReturnData);

                        riskReturnColor = gfc.getColorRiskReturns(riskReturnData.riskStdDev, riskReturnData.meanReturns);

                        QString riskStr;
                        riskStr.sprintf("%.3f", riskReturnData.riskStdDev);

                        QString returnStr;
                        returnStr.sprintf("%.3f", riskReturnData.meanReturns);

                        etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, riskStr, rowMinus, 11, riskReturnColor);
                        etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, returnStr, rowMinus, 12, riskReturnColor);
                            gfc.getColorPe(keyData.keyValuePE, color1);
                            etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.keyValuePE, rowMinus, 6, color1);

                            gfc.getColorPs(keyData.keyValuePS, color1);
                            etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.keyValuePS, rowMinus, 7, color1);

                            gfc.getColorYield(keyData.keyValueYield, keyData.earningsDividedByDividend, color1);
                            etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.keyValueYield, rowMinus, 8, color1);

                            gfc.getColorEarningsDivDividend(keyData.earningsDividedByDividend, color1);
                            etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.earningsDividedByDividend, rowMinus, 9, color1);

                            color1 = Qt::black;
                            gfc.getColorNavDivStockPrice(keyData.navDivLastStockPrice, color1);
                            etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.navDivLastStockPrice, rowMinus, 10, color1);

                            // Asset symbol
                            if( true == m_taBuy.getAvgBuySellSignals(keyData.assetSymbol,
                                                                             sellSignals,
                                                                             buySignals,
                                                                             sellMomentumSignals,
                                                                             buyMomentumSignals))
                            {

                                //============================================================
                                // Trend sell buy signal
                                //============================================================
                                QString buySignal;
                                buySignal = m_taBuy.convAvgBuySignalToNumber(buySignals);
                                if(buySignal.compare(QString::fromUtf8("0")) != 0)
                                {
                                    etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, buySignal, rowMinus, 4, Qt::blue);
                                }

                                #if 1
                                QString sellSignal;
                                sellSignal = m_taBuy.convAvgSellSignalToNumber(sellSignals);
                                if(sellSignal.compare(QString::fromUtf8("0")) != 0)
                                {
                                    etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, sellSignal, rowMinus, 4, Qt::red);
                                }
                                #endif

                                //===============================================================
                                // Momentum sell buy signal
                                //===============================================================
                                buySignal = m_taBuy.convMomentumBuySignalToNumber(buyMomentumSignals);
                                if(buySignal.compare(QString::fromUtf8("0")) != 0)
                                {
                                    etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, buySignal, rowMinus, 5, Qt::blue);
                                }

                                sellSignal = m_taBuy.convMomentumSellSignalToNumber(sellMomentumSignals);
                                if(sellSignal.compare(QString::fromUtf8("0")) != 0)
                                {
                                    etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, sellSignal, rowMinus, 5, Qt::red);
                                }

                            }

                            etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.assetSymbol, rowMinus, 1, Qt::black);
                        }
                        else
                        {
                            keyData.assetSymbol = "";
                            etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.assetSymbol, rowMinus, 1, Qt::black);
                        }

                        // Add stock name Column 0
                        etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, m_stockArr[j].stockName, rowMinus, 0, Qt::black);

                        // Add R on Column 2
                        str.sprintf("%.2f", r);
                        etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, str, rowMinus, 2, Qt::black);

                        // Add K on Column 3
                        str.sprintf("%.2f", k);
                        if(k >= 0)
                        {
                            etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, str, rowMinus, 3, Qt::blue);
                        }
                        else
                        {
                            etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, str, rowMinus, 3, Qt::red);
                        }
                        rowMinus++;
                    }
                }
             }
        }

        ui->tableViewLeastSquare->resizeColumnsToContents();
        ui->tableViewLeastSquare->sortByColumn(2, Qt::DescendingOrder);

        ui->tableViewLeastSquare_2->resizeColumnsToContents();
        ui->tableViewLeastSquare_2->sortByColumn(2, Qt::DescendingOrder);

    }

    //db.closeDb();
    m_mutex.unlock();
}



/*******************************************************************
 *
 * Function:    on_pushButton_clicked()
 *
 * Description: Invocked when user click on cell i table
 *
 *
 *
 *******************************************************************/
void LeastSquaresTaDlg::on_pushButton_clicked()
{


    int i;
    int rowPlus;
    int rowMinus;
    int j;
    int len;
    int len1;
    int nofCols;
    bool res;
    CDbHndl db;
    QString str;
    GuiFinanceColor gfc;
    GuiFinanceCtrls gfic;
    CDbHndl::snapshotStockData_ST keyData;
    TaBuySellIdicator::SellSignalMovingAvgST sellSignals;
    TaBuySellIdicator::BuySignalMovingAvgST buySignals;
    TaBuySellIdicator::SellSignalMomentumST sellMomentumSignals;
    TaBuySellIdicator::BuySignalMomentumST buyMomentumSignals;

    int nofData = 0;
    double meanXSum = 0;
    double meanYSum = 0;
    double prodXXSum = 0;
    double prodYYSum = 0;
    double prodXYSum = 0;
    double k;
    double m;
    double r;

    QHeaderView *horizHeaderPlus;
    QHeaderView *horizHeaderMinus;

    CExtendedTable etPlus;
    CExtendedTable etMinus;
    TableColumnIndicatorInfo_ST headerList[CDbHndl::MAX_NOF_GPSEL_INDEX];


    nofCols = 0;

    etPlus.setHorizontalFont(ui->tableViewLeastSquare, "Helvetica", 9);
    etPlus.setVerticalFont(ui->tableViewLeastSquare, "Helvetica", 9);

    etMinus.setHorizontalFont(ui->tableViewLeastSquare_2, "Helvetica", 9);
    etMinus.setVerticalFont(ui->tableViewLeastSquare_2, "Helvetica", 9);

    headerList[nofCols++].name = QString::fromUtf8("Företag     ");
    headerList[nofCols++].name = QString::fromUtf8("Symbol      ");
    headerList[nofCols++].name = QString::fromUtf8("R");
    headerList[nofCols++].name = QString::fromUtf8("K");
    headerList[nofCols++].name = QString::fromUtf8("T Köp /\nSälj");
    headerList[nofCols++].name = QString::fromUtf8("M Köp /\nSälj");


    headerList[nofCols++].name = QString::fromUtf8("PE     ");
    headerList[nofCols++].name = QString::fromUtf8("PS     ");
    headerList[nofCols++].name = QString::fromUtf8("DirAvk");
    headerList[nofCols++].name = QString::fromUtf8("Vinst/\nDAvk");
    headerList[nofCols++].name = QString::fromUtf8("Sub/\nKurs");

    headerList[nofCols++].name = QString::fromUtf8("Förv.\nRisk");
    headerList[nofCols++].name = QString::fromUtf8("Förv.\nAvkst");


    CUtil cu;
    QString startDate;
    QString endDate;
    int intMonth = -12;


    QString stockListName;
    int stockListId;

    cu.getCurrentDate(endDate);

    if(false == cu.addMonth(endDate, startDate, intMonth))
    {
        return;
    }



    res = gfic.getStockListNameAndId(ui->stockListComboBoxSqrt, stockListName, stockListId);


    if(false == res)
    {
        QMessageBox::information(NULL, QString::fromUtf8("Aktielistor"), QString::fromUtf8("Fel ingen data hittad"));
        return;
    }




    if(true == db.findTaStockListId(stockListName, stockListId))
    {
        int len = m_stockArr.size();

        // Used by risk & returns plot
        removeQwtPlotArrMemSpace();
        createQwtPlotArrMemSpace(len);


        for( int i = 0; i < len; i++)
        {
            m_stockArr[i].data.x.clear();
            m_stockArr[i].data.y.clear();
        }

        m_stockArr.clear();

        if(false == db.getAllSnapshotPlotStocksData(stockListName, m_stockArr))
        {
            QMessageBox::information(NULL, QString::fromUtf8("Min Portfölj"), QString::fromUtf8("Fel ingen data hittad"));
            return;
        }

    }
    else
    {
        QMessageBox::information(NULL, QString::fromUtf8("Min Portfölj"), QString::fromUtf8("Fel ingen data hittad"));
        return;
    }

    m_mutex.lock();

    // Init least sqrt
    db.init1dLeastSrq(nofData, meanXSum, meanYSum, prodXXSum, prodYYSum, prodXYSum);


    len = m_stockArr.size();

    // Init table
    etPlus.createTableModel(len, nofCols, this);
    etPlus.addHeaders(ui->tableViewLeastSquare, headerList, nofCols);


    etMinus.createTableModel(len, nofCols, this);
    etMinus.addHeaders(ui->tableViewLeastSquare_2, headerList, nofCols);

    // Init columm head click event handlers for tableView
    horizHeaderPlus= ui->tableViewLeastSquare->horizontalHeader();
    horizHeaderMinus= ui->tableViewLeastSquare_2->horizontalHeader();

    connect(horizHeaderPlus,  SIGNAL(sectionClicked(int)), this, SLOT(tablePlusHeaderClicked(int)));
    connect(horizHeaderMinus, SIGNAL(sectionClicked(int)), this, SLOT(tableMinusHeaderClicked(int)));

    for(i = 0; i < FA_NOF_DATA; i++ )
    {
        m_faDataPalette[i] = new QPalette();
    }


    // One day is added first
    for(rowPlus = 0, rowMinus = 0, j = 0; j < len; j++)
    {
        len1 = m_stockArr[j].data.x.count();

        // Is there enough data to calc least square on?
        if(len1 < 2)
        {
            break;
        }

        if(len1 > 6)
        {
            len1 = 6;
        }

        for(i = len1 -1; i >= 0; i--)
        {
            db.gather1dLeastSrqData(m_stockArr[j].data.x[i],
                                 m_stockArr[j].data.y[i],
                                 nofData,
                                 meanXSum,
                                 meanYSum,
                                 prodXXSum,
                                 prodYYSum,
                                 prodXYSum);
        }




        if((true == db.calc1dLeastSrqFitRParam(nofData,prodXXSum, prodYYSum, prodXYSum, r)) || (true == ui->showAllCheckBox->isChecked()) )
        {
            if((true == db.calc1dLeastSrqFitParams(nofData, meanXSum, meanYSum, prodXXSum, prodYYSum, prodXYSum, m, k)) || (true == ui->showAllCheckBox->isChecked()))
            {

                if((r > 0.7)|| (true == ui->showAllCheckBox->isChecked()))
                {
                    // Add stock symbol Column 1
                    if(true == db.companynameGetKeyDataUseBridge(m_stockArr[j].stockName, keyData))
                    {
                        Qt::GlobalColor color1 = Qt::black;
                        Qt::GlobalColor riskReturnColor = Qt::magenta;

                        CDbHndl::EfficPortStockData_ST riskReturnData;
                        riskReturnData.stockSymbol = keyData.assetSymbol;

                        calcRiskAndReturn(startDate, endDate, riskReturnData);

                        riskReturnColor = gfc.getColorRiskReturns(riskReturnData.riskStdDev, riskReturnData.meanReturns);

                        QString riskStr;
                        riskStr.sprintf("%.3f", riskReturnData.riskStdDev);

                        QString returnStr;
                        returnStr.sprintf("%.3f", riskReturnData.meanReturns);

                        etPlus.addDataSetTextColor(ui->tableViewLeastSquare, riskStr, rowPlus, 11, riskReturnColor);
                        etPlus.addDataSetTextColor(ui->tableViewLeastSquare, returnStr, rowPlus, 12, riskReturnColor);

                        gfc.getColorPe(keyData.keyValuePE, color1);
                        etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.keyValuePE, rowPlus, 6, color1);

                        color1 = Qt::black;
                        gfc.getColorPs(keyData.keyValuePS, color1);
                        etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.keyValuePS, rowPlus, 7, color1);

                        gfc.getColorYield(keyData.keyValueYield, keyData.earningsDividedByDividend, color1);
                        etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.keyValueYield, rowPlus, 8, color1);

                        gfc.getColorEarningsDivDividend(keyData.earningsDividedByDividend, color1);
                        etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.earningsDividedByDividend, rowPlus, 9, color1);

                        color1 = Qt::black;
                        gfc.getColorNavDivStockPrice(keyData.navDivLastStockPrice, color1);
                        etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.navDivLastStockPrice, rowPlus, 10, color1);

                        // Asset symbol
                        etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.assetSymbol, rowPlus, 1, Qt::black);


                        if( true == m_taBuy.getAvgBuySellSignals(keyData.assetSymbol,
                                                                 sellSignals,
                                                                 buySignals,
                                                                 sellMomentumSignals,
                                                                 buyMomentumSignals))
                        {
                            QString buySignal;
                            buySignal = m_taBuy.convAvgBuySignalToNumber(buySignals);
                            if(buySignal.compare(QString::fromUtf8("0")) != 0)
                            {
                                etPlus.addDataSetTextColor(ui->tableViewLeastSquare, buySignal, rowPlus, 4, Qt::blue);
                            }

                            QString sellSignal;
                            sellSignal = m_taBuy.convAvgSellSignalToNumber(sellSignals);
                            if(sellSignal.compare(QString::fromUtf8("0")) != 0)
                            {
                                etPlus.addDataSetTextColor(ui->tableViewLeastSquare, sellSignal, rowPlus, 4, Qt::red);
                            }

                            //===============================================================
                            // Momentum sell buy signal
                            //===============================================================
                            buySignal = m_taBuy.convMomentumBuySignalToNumber(buyMomentumSignals);
                            if(buySignal.compare(QString::fromUtf8("0")) != 0)
                            {
                                etPlus.addDataSetTextColor(ui->tableViewLeastSquare, buySignal, rowPlus, 5, Qt::blue);
                            }

                            sellSignal = m_taBuy.convMomentumSellSignalToNumber(sellMomentumSignals);
                            if(sellSignal.compare(QString::fromUtf8("0")) != 0)
                            {
                                etPlus.addDataSetTextColor(ui->tableViewLeastSquare, sellSignal, rowPlus, 5, Qt::red);
                            }

                        }
                    }
                    else
                    {
                        keyData.assetSymbol = "";
                        etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.assetSymbol, rowPlus, 1, Qt::black);
                    }

                    // Add stockname column 0
                    etPlus.addDataSetTextColor(ui->tableViewLeastSquare, m_stockArr[j].stockName, rowPlus, 0, Qt::black);

                    // Add R on column 2
                    str.sprintf("%.2f", r);
                    etPlus.addDataSetTextColor(ui->tableViewLeastSquare, str, rowPlus, 2, Qt::black);

                    // Add K on column 3
                    str.sprintf("%.2f", k);
                    if(k >= 0)
                    {
                        etPlus.addDataSetTextColor(ui->tableViewLeastSquare, str, rowPlus, 3, Qt::blue);
                    }
                    else
                    {
                        etPlus.addDataSetTextColor(ui->tableViewLeastSquare, str, rowPlus, 3, Qt::red);
                    }
                    rowPlus++;
                }
                else if(r < -0.7)
                {
                    // Add stock symbol Column 1
                    if(true == db.companynameGetKeyDataUseBridge(m_stockArr[j].stockName, keyData))
                    {

                        Qt::GlobalColor color1 = Qt::black;

                        Qt::GlobalColor riskReturnColor = Qt::magenta;

                        CDbHndl::EfficPortStockData_ST riskReturnData;
                        riskReturnData.stockSymbol = keyData.assetSymbol;

                        calcRiskAndReturn(startDate, endDate, riskReturnData);

                        riskReturnColor = gfc.getColorRiskReturns(riskReturnData.riskStdDev, riskReturnData.meanReturns);

                        QString riskStr;
                        riskStr.sprintf("%.3f", riskReturnData.riskStdDev);

                        QString returnStr;
                        returnStr.sprintf("%.3f", riskReturnData.meanReturns);

                        etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, riskStr, rowMinus, 11, riskReturnColor);
                        etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, returnStr, rowMinus, 12, riskReturnColor);


                        gfc.getColorPe(keyData.keyValuePE, color1);
                        etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.keyValuePE, rowMinus, 6, color1);

                        gfc.getColorPs(keyData.keyValuePS, color1);
                        etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.keyValuePS, rowMinus, 7, color1);

                        gfc.getColorYield(keyData.keyValueYield, keyData.earningsDividedByDividend, color1);
                        etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.keyValueYield, rowMinus, 8, color1);

                        gfc.getColorEarningsDivDividend(keyData.earningsDividedByDividend, color1);
                        etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.earningsDividedByDividend, rowMinus, 9, color1);

                        color1 = Qt::black;
                        gfc.getColorNavDivStockPrice(keyData.navDivLastStockPrice, color1);
                        etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.navDivLastStockPrice, rowMinus, 10, color1);

                        // Asset symbol
                        if( true == m_taBuy.getAvgBuySellSignals(keyData.assetSymbol,
                                                                         sellSignals,
                                                                         buySignals,
                                                                         sellMomentumSignals,
                                                                         buyMomentumSignals))
                        {

                            //============================================================
                            // Trend sell buy signal
                            //============================================================
                            QString buySignal;
                            buySignal = m_taBuy.convAvgBuySignalToNumber(buySignals);
                            if(buySignal.compare(QString::fromUtf8("0")) != 0)
                            {
                                etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, buySignal, rowMinus, 4, Qt::blue);
                            }

                            #if 1
                            QString sellSignal;
                            sellSignal = m_taBuy.convAvgSellSignalToNumber(sellSignals);
                            if(sellSignal.compare(QString::fromUtf8("0")) != 0)
                            {
                                etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, sellSignal, rowMinus, 4, Qt::red);
                            }
                            #endif

                            //===============================================================
                            // Momentum sell buy signal
                            //===============================================================
                            buySignal = m_taBuy.convMomentumBuySignalToNumber(buyMomentumSignals);
                            if(buySignal.compare(QString::fromUtf8("0")) != 0)
                            {
                                etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, buySignal, rowMinus, 5, Qt::blue);
                            }

                            sellSignal = m_taBuy.convMomentumSellSignalToNumber(sellMomentumSignals);
                            if(sellSignal.compare(QString::fromUtf8("0")) != 0)
                            {
                                etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, sellSignal, rowMinus, 5, Qt::red);
                            }

                        }

                        etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.assetSymbol, rowMinus, 1, Qt::black);
                    }
                    else
                    {
                        keyData.assetSymbol = "";
                        etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.assetSymbol, rowMinus, 1, Qt::black);
                    }

                    // Add stock name Column 0
                    etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, m_stockArr[j].stockName, rowMinus, 0, Qt::black);

                    // Add R on Column 2
                    str.sprintf("%.2f", r);
                    etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, str, rowMinus, 2, Qt::black);

                    // Add K on Column 3
                    str.sprintf("%.2f", k);
                    if(k >= 0)
                    {
                        etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, str, rowMinus, 3, Qt::blue);
                    }
                    else
                    {
                        etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, str, rowMinus, 3, Qt::red);
                    }
                    rowMinus++;
                }
            }
         }
    }

    ui->tableViewLeastSquare->resizeColumnsToContents();
    ui->tableViewLeastSquare->sortByColumn(2, Qt::DescendingOrder);

    ui->tableViewLeastSquare_2->resizeColumnsToContents();
    ui->tableViewLeastSquare_2->sortByColumn(2, Qt::DescendingOrder);

    m_mutex.unlock();

}


/*********************************************************************
 * Function: ()
 *
 * Description: .
 *
 *********************************************************************/
void LeastSquaresTaDlg::removeQwtPlotArrMemSpace(void)
{
    delete [] m_riskReturPlotArrBuy;
    delete [] m_riskReturPlotArrSell;
    delete [] m_riskReturTxtArrBuy;
    delete [] m_riskReturTxtArrSell;
    delete m_xAxisPlotBuy;
    delete m_xAxisPlotSell;
    delete m_plotGridBuy;
    delete m_plotGridSell;
}



/*********************************************************************
 * Function: ()
 *
 * Description: .
 *
 *********************************************************************/
bool LeastSquaresTaDlg::createQwtPlotArrMemSpace(int nofStocks)
{

    // Create variances covariances matrix space
    m_riskReturPlotArrBuy = new QwtPlotCurve [nofStocks + 5];
    m_riskReturPlotArrSell = new QwtPlotCurve [nofStocks + 5];
    m_nofRiskReturPlotData = nofStocks;

    if((m_riskReturPlotArrBuy) == 0)
    {
        return false;
    }

    if((m_riskReturPlotArrSell) == 0)
    {
        return false;
    }


    m_riskReturTxtArrBuy = new QwtPlotMarker [nofStocks + 5];

    if((m_riskReturTxtArrBuy) == 0)
    {
        return false;
    }

    m_riskReturTxtArrSell = new QwtPlotMarker [nofStocks + 5];

    if((m_riskReturTxtArrSell) == 0)
    {
        return false;
    }


    m_xAxisPlotBuy = new QwtPlotCurve;

    if((m_xAxisPlotBuy) == 0)
    {
        return false;
    }


    m_xAxisPlotSell = new QwtPlotCurve;

    if((m_xAxisPlotSell) == 0)
    {
        return false;
    }



    m_plotGridBuy = new QwtPlotGrid;

    if((m_plotGridBuy) == 0)
    {
        return false;
    }

    m_plotGridSell = new QwtPlotGrid;

    if((m_plotGridSell) == 0)
    {
        return false;
    }


    return true;
}



/*******************************************************************
 *
 * Function:    ()
 *
 * Description: Risk & return plot array
 *
 *
 *******************************************************************/
void LeastSquaresTaDlg::
updateMinMax(double x,
             double y,
             CYahooStockPlotUtil::MinMaxAxisValues_ST &minMaxReturPlotArr)
{
    if(minMaxReturPlotArr.minMaxIsInit == false)
    {
        minMaxReturPlotArr.minMaxIsInit = true;
        minMaxReturPlotArr.maxX = x;
        minMaxReturPlotArr.minX = x;

        minMaxReturPlotArr.maxY = y;
        minMaxReturPlotArr.minY = y;
    }
    else
    {
        if(x > minMaxReturPlotArr.maxX)
            minMaxReturPlotArr.maxX = x;

        if(x < minMaxReturPlotArr.minX)
            minMaxReturPlotArr.minX = x;

        if(y > minMaxReturPlotArr.maxY)
            minMaxReturPlotArr.maxY = y;

        if(y < minMaxReturPlotArr.minY)
            minMaxReturPlotArr.minY = y;
     }
}


/*********************************************************************
 *
 * Function: ()
 *
 * Description: Risk & returns
 *
 *
 *
 *********************************************************************/
void LeastSquaresTaDlg::
enterPlotLabel(int index, double x, double y, QwtPlot *qwtPlot, QwtPlotMarker *riskReturTxtArr)
{
    QString label;
    label.sprintf("%d.", index);

    QwtText text(label);
    text.setFont( QFont( "Helvetica", 14, QFont::Bold) );
    text.setColor( QColor(Qt::black));
    text.setBackgroundBrush(QColor(Qt::white));


    riskReturTxtArr[index].setLabel( text );
    riskReturTxtArr[index].setValue((x), y);

    riskReturTxtArr[index].attach(qwtPlot);

}



/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 *******************************************************************/
void LeastSquaresTaDlg::addRiskReturnsPlotData(int row, double x, double y, int plotNo)
{
    qDebug() << "valid risk return";


    if(plotNo == 0)
    {
        updateMinMax(x, y, m_minMaxReturPlotArrBuy);
        m_riskReturPlotArrBuy[row].setSamples(&x, &y, 1);
        enterPlotLabel(row, x, y, ui->qwtPlotLSqr_3, m_riskReturTxtArrBuy);
    }
    else
    {
        updateMinMax(x, y, m_minMaxReturPlotArrSell);
        m_riskReturPlotArrSell[row].setSamples(&x, &y, 1);
        enterPlotLabel(row, x, y, ui->qwtPlotLSqr_4, m_riskReturTxtArrSell);
    }

}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 *******************************************************************/
bool LeastSquaresTaDlg::
calcRiskAndReturn(QString startDate,
                  QString endDate,
                  CDbHndl::EfficPortStockData_ST &data)
{
    // CUtil cu;
    CDbHndl db;

    // int row;

    // removeQwtPlotArrMemSpace();
    // createQwtPlotArrMemSpace(m_stockArr1.size());
    // m_minMaxReturPlotArr.minMaxIsInit = false;



   //  QString tmp;
   // double x;
   // double y;
   // int colorNumber = 0;

    data.isValid = false;

    if(true == db.efficPortfCalcMeanAndStdDev(startDate, endDate, data))
    {
        data.meanReturns = data.meanReturns *100;
        data.riskStdDev = data.riskStdDev * 100;

        if(false == data.isValid)
        {
            qDebug() << "invalid risk return calc" << data.stockName;
        }
    }

    return data.isValid;
}


/*******************************************************************
 *
 * Function:    on_SellSigPushButton_clicked()
 *
 * Description:
 *
 *
 *******************************************************************/
void LeastSquaresTaDlg::on_SellSigPushButton_clicked()
{
    int i;
    int rowPlus;
    int rowMinus;
    int j;
    int len;
    int len1;
    int nofCols;
    bool res;
    CDbHndl db;
    QString str;
    GuiFinanceColor gfc;
    GuiFinanceCtrls gfic;
    CDbHndl::snapshotStockData_ST keyData;
    TaBuySellIdicator::SellSignalMovingAvgST sellSignals;
    TaBuySellIdicator::BuySignalMovingAvgST buySignals;
    TaBuySellIdicator::SellSignalMomentumST sellMomentumSignals;
    TaBuySellIdicator::BuySignalMomentumST buyMomentumSignals;
    TaBuySellIdicator tbsi;


    Qt::GlobalColor riskReturnColor = Qt::magenta;
    CDbHndl::EfficPortStockData_ST riskReturnData;

    int nofData = 0;
    double meanXSum = 0;
    double meanYSum = 0;
    double prodXXSum = 0;
    double prodYYSum = 0;
    double prodXYSum = 0;
    double k;
    double m;
    double r;

    QHeaderView *horizHeaderPlus;
    QHeaderView *horizHeaderMinus;

    CExtendedTable etPlus;
    CExtendedTable etMinus;
    TableColumnIndicatorInfo_ST headerList[CDbHndl::MAX_NOF_GPSEL_INDEX];

    bool sellIndicator = false;
    TaBuySellIdicator::FaBuyIndicatorStateET buyIndicator = TaBuySellIdicator::FA_IND_BUY_NOT_SET;
    bool riskReturnsIsValid = false;
    QString riskStr;
    QString returnStr;
    // CDbHndl::EfficPortStockData_ST riskReturnData;

    // Qt::GlobalColor riskReturnColor = Qt::magenta;
    Qt::GlobalColor color1;
    // bool sellIndicator = false;
    // TaBuySellIdicator::FaBuyIndicatorStateET buyIndicator = TaBuySellIdicator::FA_IND_BUY_NOT_SET;
    // TaBuySellIdicator tbsi;
    nofCols = 0;

    etPlus.setHorizontalFont(ui->tableViewLeastSquare, "Helvetica", 9);
    etPlus.setVerticalFont(ui->tableViewLeastSquare, "Helvetica", 9);

    etMinus.setHorizontalFont(ui->tableViewLeastSquare_2, "Helvetica", 9);
    etMinus.setVerticalFont(ui->tableViewLeastSquare_2, "Helvetica", 9);

    headerList[nofCols++].name = QString::fromUtf8("Företag     ");
    headerList[nofCols++].name = QString::fromUtf8("Symbol      ");
    headerList[nofCols++].name = QString::fromUtf8("Skuld/\nEget Kap");
    headerList[nofCols++].name = QString::fromUtf8("K");
    headerList[nofCols++].name = QString::fromUtf8("T Köp /\nSälj");
    headerList[nofCols++].name = QString::fromUtf8("M Köp /\nSälj");


    headerList[nofCols++].name = QString::fromUtf8("PE     ");
    headerList[nofCols++].name = QString::fromUtf8("PS     ");
    headerList[nofCols++].name = QString::fromUtf8("DirAvk");
    headerList[nofCols++].name = QString::fromUtf8("Vinst/\nDAvk");
    headerList[nofCols++].name = QString::fromUtf8("Sub/\nKurs");

    headerList[nofCols++].name = QString::fromUtf8("Förv.\nRisk");
    headerList[nofCols++].name = QString::fromUtf8("Förv.\nAvkst");
    headerList[nofCols++].name = QString::fromUtf8("index");
    headerList[nofCols++].name = QString::fromUtf8("balans-\nlikvd.");



    CUtil cu;
    QString startDate;
    QString endDate;
    int intMonth = -12;

    QString stockListName;
    int stockListId;


    cu.getCurrentDate(endDate);

    if(false == cu.addMonth(endDate, startDate, intMonth))
    {
        return;
    }



    res = gfic.getStockListNameAndId(ui->stockListComboBoxSqrt, stockListName, stockListId);


    if(false == res)
    {
        QMessageBox::information(NULL, QString::fromUtf8("Aktielistor"), QString::fromUtf8("Fel ingen data hittad"));
        return;
    }

    if(true == db.findTaStockListId(stockListName, stockListId))
    {
        int len = m_stockArr.size();
        for( int i = 0; i < len; i++)
        {
            m_stockArr[i].data.x.clear();
            m_stockArr[i].data.y.clear();
        }

        m_stockArr.clear();

        if(false == db.getAllSnapshotPlotStocksData(stockListName, m_stockArr))
        {
            QMessageBox::information(NULL, QString::fromUtf8("Min Portfölj"), QString::fromUtf8("Fel ingen data hittad"));
            return;
        }

    }
    else
    {
        QMessageBox::information(NULL, QString::fromUtf8("Min Portfölj"), QString::fromUtf8("Fel ingen data hittad"));
        return;
    }

    m_mutex.lock();

    // Init least sqrt
    db.init1dLeastSrq(nofData, meanXSum, meanYSum, prodXXSum, prodYYSum, prodXYSum);


    len = m_stockArr.size();
    // Used by risk & returns plot
    removeQwtPlotArrMemSpace();
    createQwtPlotArrMemSpace(len);
    m_nofRiskReturPlotData = len;
    m_minMaxReturPlotArrBuy.minMaxIsInit = false;
    m_minMaxReturPlotArrSell.minMaxIsInit = false;

    // Init table
    etPlus.createTableModel(len, nofCols, this);
    etPlus.addHeaders(ui->tableViewLeastSquare, headerList, nofCols);


    etMinus.createTableModel(len, nofCols, this);
    etMinus.addHeaders(ui->tableViewLeastSquare_2, headerList, nofCols);

    // Init columm head click event handlers for tableView
    horizHeaderPlus= ui->tableViewLeastSquare->horizontalHeader();
    horizHeaderMinus= ui->tableViewLeastSquare_2->horizontalHeader();

    connect(horizHeaderPlus,  SIGNAL(sectionClicked(int)), this, SLOT(tablePlusHeaderClicked(int)));
    connect(horizHeaderMinus, SIGNAL(sectionClicked(int)), this, SLOT(tableMinusHeaderClicked(int)));

    for(i = 0; i < FA_NOF_DATA; i++ )
    {
        m_faDataPalette[i] = new QPalette();
    }


    // One day is added first
    for(rowPlus = 0, rowMinus = 0, j = 0; j < len; j++)
    {
        len1 = m_stockArr[j].data.x.count();

        // Is there enough data to calc least square on?
        if(len1 < 2)
        {
            break;
        }

        if(len1 > 6)
        {
            len1 = 6;
        }

        for(i = len1 -1; i >= 0; i--)
        {
            db.gather1dLeastSrqData(m_stockArr[j].data.x[i],
                                 m_stockArr[j].data.y[i],
                                 nofData,
                                 meanXSum,
                                 meanYSum,
                                 prodXXSum,
                                 prodYYSum,
                                 prodXYSum);
        }



        if((true == db.calc1dLeastSrqFitRParam(nofData,prodXXSum, prodYYSum, prodXYSum, r)) || (true == ui->showAllCheckBox->isChecked()) )
        {
            if((true == db.calc1dLeastSrqFitParams(nofData, meanXSum, meanYSum, prodXXSum, prodYYSum, prodXYSum, m, k)) || (true == ui->showAllCheckBox->isChecked()))
            {

                bool res = db.companynameGetKeyDataUseBridge(m_stockArr[j].stockName, keyData);

                if( true == res)
                {


                    riskReturnColor = Qt::magenta;

                    riskReturnData.stockSymbol = keyData.assetSymbol;
                    riskReturnsIsValid = false;
                    if(true == calcRiskAndReturn(startDate, endDate, riskReturnData))
                    {
                        riskReturnsIsValid = true;

                        tbsi.faSellBuyIndicator(keyData, riskReturnData.riskStdDev, buyIndicator, sellIndicator);
                    }

                    if(buyIndicator == TaBuySellIdicator::FA_IND_BUY_TRUE && riskReturnsIsValid == true)
                    {
                        addRiskReturnsPlotData(j, riskReturnData.riskStdDev, riskReturnData.meanReturns, 0);
                    }
                    else if(true == sellIndicator && riskReturnsIsValid == true)
                    {
                        addRiskReturnsPlotData(j, riskReturnData.riskStdDev, riskReturnData.meanReturns, 1);
                    }

                }


                if(buyIndicator == TaBuySellIdicator::FA_IND_BUY_TRUE)
                {
                    // Add stock symbol Column 1
                    if(true == db.companynameGetKeyDataUseBridge(m_stockArr[j].stockName, keyData))
                    {
                        color1 = Qt::black;

                        if(true == riskReturnsIsValid)
                        {
                            riskReturnColor = gfc.getColorRiskReturns(riskReturnData.riskStdDev, riskReturnData.meanReturns);
                            riskStr.sprintf("%.3f", riskReturnData.riskStdDev);
                            returnStr.sprintf("%.3f", riskReturnData.meanReturns);

                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, riskStr, rowPlus, 11, riskReturnColor);
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, returnStr, rowPlus, 12, riskReturnColor);

                            // Add stockname column 0
                            QString str1;
                            str1.sprintf("%d, ", j);
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, str1, rowPlus, 13, Qt::black);

                        }

                        gfc.getColorPe(keyData.keyValuePE, color1);
                        etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.keyValuePE, rowPlus, 6, color1);

                        color1 = Qt::black;
                        gfc.getColorPs(keyData.keyValuePS, color1);
                        etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.keyValuePS, rowPlus, 7, color1);

                        gfc.getColorYield(keyData.keyValueYield, keyData.earningsDividedByDividend, color1);
                        etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.keyValueYield, rowPlus, 8, color1);

                        gfc.getColorEarningsDivDividend(keyData.earningsDividedByDividend, color1);
                        etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.earningsDividedByDividend, rowPlus, 9, color1);

                        color1 = Qt::black;
                        gfc.getColorNavDivStockPrice(keyData.navDivLastStockPrice, color1);
                        etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.navDivLastStockPrice, rowPlus, 10, color1);

                        // Asset symbol
                        etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.assetSymbol, rowPlus, 1, Qt::black);


                        if( true == m_taBuy.getAvgBuySellSignals(keyData.assetSymbol,
                                                                 sellSignals,
                                                                 buySignals,
                                                                 sellMomentumSignals,
                                                                 buyMomentumSignals))
                        {
                            QString buySignal;
                            buySignal = m_taBuy.convAvgBuySignalToNumber(buySignals);
                            if(buySignal.compare(QString::fromUtf8("0")) != 0)
                            {
                                etPlus.addDataSetTextColor(ui->tableViewLeastSquare, buySignal, rowPlus, 4, Qt::blue);
                            }

                            QString sellSignal;
                            sellSignal = m_taBuy.convAvgSellSignalToNumber(sellSignals);
                            if(sellSignal.compare(QString::fromUtf8("0")) != 0)
                            {
                                etPlus.addDataSetTextColor(ui->tableViewLeastSquare, sellSignal, rowPlus, 4, Qt::red);
                            }

                            //===============================================================
                            // Momentum sell buy signal
                            //===============================================================
                            buySignal = m_taBuy.convMomentumBuySignalToNumber(buyMomentumSignals);
                            if(buySignal.compare(QString::fromUtf8("0")) != 0)
                            {
                                etPlus.addDataSetTextColor(ui->tableViewLeastSquare, buySignal, rowPlus, 5, Qt::blue);
                            }

                            sellSignal = m_taBuy.convMomentumSellSignalToNumber(sellMomentumSignals);
                            if(sellSignal.compare(QString::fromUtf8("0")) != 0)
                            {
                                etPlus.addDataSetTextColor(ui->tableViewLeastSquare, sellSignal, rowPlus, 5, Qt::red);
                            }

                        }
                    }
                    else
                    {
                        keyData.assetSymbol = "";
                        etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.assetSymbol, rowPlus, 1, Qt::black);
                    }

                    // Add stockname column 0
                    etPlus.addDataSetTextColor(ui->tableViewLeastSquare, m_stockArr[j].stockName, rowPlus, 0, Qt::black);



                    // Add R on column 2
                    // ajn 160321cstr.sprintf("%.2f", r);
                    // ajn 160321cetPlus.addDataSetTextColor(ui->tableViewLeastSquare, str, rowPlus, 2, Q   t::black);
                     double debtToEquityRatio = 0;
                     double currentRatio = 0;
                     if(true == db.getYahooKeyData(keyData.assetSymbol, debtToEquityRatio, currentRatio))

                     //if(true == db.getDebtToEquityRatio(keyData.assetSymbol, debtToEquityRatio))
                     {
                         str.sprintf("%.2f", debtToEquityRatio);

                         // This is totaly dept so we set 75 instead of 50
                         if(debtToEquityRatio > 150.0)
                         {
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, str, rowPlus, 2, Qt::red);
                         }
                         else if(debtToEquityRatio > 100.0 && debtToEquityRatio < 150.0)
                         {
                             etPlus.addDataSetTextColor(ui->tableViewLeastSquare, str, rowPlus, 2, Qt::magenta);
                         }
                         else
                         {
                             etPlus.addDataSetTextColor(ui->tableViewLeastSquare, str, rowPlus, 2, Qt::darkGreen);
                         }


                         str.sprintf("%.2f", currentRatio);

                         // This is totaly dept so we set 75 instead of 50
                         if(currentRatio > 1.5)
                         {
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, str, rowPlus, 14, Qt::darkGreen);
                         }
                         else if(currentRatio > 1.0 && currentRatio < 1.5)
                         {
                             etPlus.addDataSetTextColor(ui->tableViewLeastSquare, str, rowPlus, 14, Qt::magenta);
                         }
                         else
                         {
                             etPlus.addDataSetTextColor(ui->tableViewLeastSquare, str, rowPlus, 14, Qt::red);
                         }


                     }
                     else
                     {
                         str = " ";
                         etPlus.addDataSetTextColor(ui->tableViewLeastSquare, str, rowPlus, 2, Qt::black);
                     }

                    // Add K on column 3
                    str.sprintf("%.2f", k);
                    if(k >= 0)
                    {
                        etPlus.addDataSetTextColor(ui->tableViewLeastSquare, str, rowPlus, 3, Qt::blue);
                    }
                    else
                    {
                        etPlus.addDataSetTextColor(ui->tableViewLeastSquare, str, rowPlus, 3, Qt::red);
                    }
                    rowPlus++;
                }
                else if(sellIndicator == true)
                {

                    // if(true == db.companynameGetKeyDataUseBridge(m_stockArr[j].stockName, keyData))
                    {

                        color1 = Qt::black;

                        if(true == riskReturnsIsValid)
                        {
                            riskReturnColor = Qt::magenta;

                            // Add risk & returns index
                            QString str1;
                            str1.sprintf("%d, ", j);
                            etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, str1, rowMinus, 13, Qt::black);


                            riskReturnColor = gfc.getColorRiskReturns(riskReturnData.riskStdDev, riskReturnData.meanReturns);

                            QString riskStr;
                            riskStr.sprintf("%.3f", riskReturnData.riskStdDev);

                            QString returnStr;
                            returnStr.sprintf("%.3f", riskReturnData.meanReturns);

                            etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, riskStr, rowMinus, 11, riskReturnColor);

                            etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, returnStr, rowMinus, 12, riskReturnColor);
                        }


                        gfc.getColorPe(keyData.keyValuePE, color1);
                        etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.keyValuePE, rowMinus, 6, color1);

                        gfc.getColorPs(keyData.keyValuePS, color1);
                        etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.keyValuePS, rowMinus, 7, color1);

                        gfc.getColorYield(keyData.keyValueYield, keyData.earningsDividedByDividend, color1);
                        etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.keyValueYield, rowMinus, 8, color1);

                        gfc.getColorEarningsDivDividend(keyData.earningsDividedByDividend, color1);
                        etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.earningsDividedByDividend, rowMinus, 9, color1);

                        color1 = Qt::black;
                        gfc.getColorNavDivStockPrice(keyData.navDivLastStockPrice, color1);
                        etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.navDivLastStockPrice, rowMinus, 10, color1);

                        // Asset symbol
                        if( true == m_taBuy.getAvgBuySellSignals(keyData.assetSymbol,
                                                                         sellSignals,
                                                                         buySignals,
                                                                         sellMomentumSignals,
                                                                         buyMomentumSignals))
                        {

                            //============================================================
                            // Trend sell buy signal
                            //============================================================
                            QString buySignal;
                            buySignal = m_taBuy.convAvgBuySignalToNumber(buySignals);
                            if(buySignal.compare(QString::fromUtf8("0")) != 0)
                            {
                                etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, buySignal, rowMinus, 4, Qt::blue);
                            }

                                QString sellSignal;
                                sellSignal = m_taBuy.convAvgSellSignalToNumber(sellSignals);
                                if(sellSignal.compare(QString::fromUtf8("0")) != 0)
                                {
                                    etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, sellSignal, rowMinus, 4, Qt::red);
                                }

                            //===============================================================
                            // Momentum sell buy signal
                            //===============================================================
                            buySignal = m_taBuy.convMomentumBuySignalToNumber(buyMomentumSignals);
                            if(buySignal.compare(QString::fromUtf8("0")) != 0)
                            {
                                etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, buySignal, rowMinus, 5, Qt::blue);
                            }

                            sellSignal = m_taBuy.convMomentumSellSignalToNumber(sellMomentumSignals);
                            if(sellSignal.compare(QString::fromUtf8("0")) != 0)
                            {
                                etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, sellSignal, rowMinus, 5, Qt::red);
                            }

                        }

                        etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.assetSymbol, rowMinus, 1, Qt::black);
                    }
                        //else
                        //{
                        //    keyData.assetSymbol = "";
                        //    etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.assetSymbol, rowMinus, 1, Qt::black);
                        //}

                    // Add stock name Column 0
                    etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, m_stockArr[j].stockName, rowMinus, 0, Qt::black);

                    // Add R on Column 2
                        // ajn 160321 str.sprintf("%.2f", r);
                        // ajn 160321 etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, str, rowMinus, 2, Qt::black);

                        double debtToEquityRatio = 0;
                        double currentRatio = 0;
                        if(true == db.getYahooKeyData(keyData.assetSymbol, debtToEquityRatio, currentRatio))
                        // if(true == db.getDebtToEquityRatio(keyData.assetSymbol, debtToEquityRatio))
                        {
                            str.sprintf("%.2f", debtToEquityRatio);
                            if(debtToEquityRatio > 150.0)
                            {
                                etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, str, rowMinus, 2, Qt::red);
                            }
                            else if(debtToEquityRatio > 100.0 && debtToEquityRatio < 150.0)
                            {
                                etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, str, rowMinus, 2, Qt::magenta);
                            }
                            else
                            {
                                etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, str, rowMinus, 2, Qt::darkGreen);
                            }


                            str.sprintf("%.2f", currentRatio);

                            // This is totaly dept so we set 75 instead of 50
                            if(currentRatio > 1.5)
                            {
                               etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, str, rowMinus, 14, Qt::darkGreen);
                            }
                            else if(currentRatio > 1.0 && currentRatio < 1.5)
                            {
                                etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, str, rowMinus, 14, Qt::magenta);
                            }
                            else
                            {
                                etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, str, rowMinus, 14, Qt::red);
                            }
                        }
                        else
                        {
                            str = " ";
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, str, rowPlus, 2, Qt::black);
                        }


                        // Add K on Column 3
                        str.sprintf("%.2f", k);
                        if(k >= 0)
                        {
                            etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, str, rowMinus, 3, Qt::blue);
                        }
                        else
                        {
                            etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, str, rowMinus, 3, Qt::red);
                        }
                        rowMinus++;
                    }
                }
             }
        }

        ui->tableViewLeastSquare->resizeColumnsToContents();
        ui->tableViewLeastSquare->sortByColumn(2, Qt::DescendingOrder);

        ui->tableViewLeastSquare_2->resizeColumnsToContents();
        ui->tableViewLeastSquare_2->sortByColumn(2, Qt::DescendingOrder);


    // Risk & return
    plotXAxis(ui->qwtPlotLSqr_3, m_plotGridBuy, m_minMaxReturPlotArrBuy, m_xAxisPlotBuy);
    plotXAxis(ui->qwtPlotLSqr_4, m_plotGridSell, m_minMaxReturPlotArrSell, m_xAxisPlotSell);
    plotQwtData(m_nofRiskReturPlotData, ui->qwtPlotLSqr_3, m_riskReturPlotArrBuy);
    plotQwtData(m_nofRiskReturPlotData, ui->qwtPlotLSqr_4, m_riskReturPlotArrSell);
    m_mutex.unlock();

}







#if 0
/*******************************************************************
 *
 * Function:    on_SellSigPushButton_clicked()
 *
 * Description:
 *
 *
 *******************************************************************/
void LeastSquaresTaDlg::on_SellSigPushButton_clicked()
{
    int i;
    int rowPlus;
    int rowMinus;
    int j;
    int len;
    int len1;
    int nofCols;
    bool res;
    CDbHndl db;
    QString str;
    GuiFinanceColor gfc;
    GuiFinanceCtrls gfic;
    CDbHndl::snapshotStockData_ST keyData;
    TaBuySellIdicator::SellSignalMovingAvgST sellSignals;
    TaBuySellIdicator::BuySignalMovingAvgST buySignals;
    TaBuySellIdicator::SellSignalMomentumST sellMomentumSignals;
    TaBuySellIdicator::BuySignalMomentumST buyMomentumSignals;
    TaBuySellIdicator tbsi;


    Qt::GlobalColor riskReturnColor = Qt::magenta;
    CDbHndl::EfficPortStockData_ST riskReturnData;

    int nofData = 0;
    double meanXSum = 0;
    double meanYSum = 0;
    double prodXXSum = 0;
    double prodYYSum = 0;
    double prodXYSum = 0;
    double k;
    double m;
    double r;

    QHeaderView *horizHeaderPlus;
    QHeaderView *horizHeaderMinus;

    CExtendedTable etPlus;
    CExtendedTable etMinus;
    TableColumnIndicatorInfo_ST headerList[CDbHndl::MAX_NOF_GPSEL_INDEX];

    bool sellIndicator = false;
    TaBuySellIdicator::FaBuyIndicatorStateET buyIndicator = TaBuySellIdicator::FA_IND_BUY_NOT_SET;

    QString riskStr;
    QString returnStr;




    nofCols = 0;

    etPlus.setHorizontalFont(ui->tableViewLeastSquare, "Helvetica", 9);
    etPlus.setVerticalFont(ui->tableViewLeastSquare, "Helvetica", 9);

    etMinus.setHorizontalFont(ui->tableViewLeastSquare_2, "Helvetica", 9);
    etMinus.setVerticalFont(ui->tableViewLeastSquare_2, "Helvetica", 9);

    headerList[nofCols++].name = QString::fromUtf8("Företag     ");
    headerList[nofCols++].name = QString::fromUtf8("Symbol      ");
    headerList[nofCols++].name = QString::fromUtf8("Skuld/\nEget Kap");
    headerList[nofCols++].name = QString::fromUtf8("K");
    headerList[nofCols++].name = QString::fromUtf8("T Köp /\nSälj");
    headerList[nofCols++].name = QString::fromUtf8("M Köp /\nSälj");


    headerList[nofCols++].name = QString::fromUtf8("PE     ");
    headerList[nofCols++].name = QString::fromUtf8("PS     ");
    headerList[nofCols++].name = QString::fromUtf8("DirAvk");
    headerList[nofCols++].name = QString::fromUtf8("Vinst/\nDAvk");
    headerList[nofCols++].name = QString::fromUtf8("Sub/\nKurs");

    headerList[nofCols++].name = QString::fromUtf8("Förv.\nRisk");
    headerList[nofCols++].name = QString::fromUtf8("Förv.\nAvkst");


    CUtil cu;
    QString startDate;
    QString endDate;
    int intMonth = -12;

    QString stockListName;
    int stockListId;


    cu.getCurrentDate(endDate);

    if(false == cu.addMonth(endDate, startDate, intMonth))
    {
        return;
    }



    res = gfic.getStockListNameAndId(ui->stockListComboBoxSqrt, stockListName, stockListId);


    if(false == res)
    {
        QMessageBox::information(NULL, QString::fromUtf8("Aktielistor"), QString::fromUtf8("Fel ingen data hittad"));
        return;
    }

    if(true == db.findTaStockListId(stockListName, stockListId))
    {
        int len = m_stockArr.size();
        for( int i = 0; i < len; i++)
        {
            m_stockArr[i].data.x.clear();
            m_stockArr[i].data.y.clear();
        }

        m_stockArr.clear();

        if(false == db.getAllSnapshotPlotStocksData(stockListName, m_stockArr))
        {
            QMessageBox::information(NULL, QString::fromUtf8("Min Portfölj"), QString::fromUtf8("Fel ingen data hittad"));
            return;
        }

    }
    else
    {
        QMessageBox::information(NULL, QString::fromUtf8("Min Portfölj"), QString::fromUtf8("Fel ingen data hittad"));
        return;
    }

    m_mutex.lock();

    // Init least sqrt
    db.init1dLeastSrq(nofData, meanXSum, meanYSum, prodXXSum, prodYYSum, prodXYSum);


    len = m_stockArr.size();

    // Init table
    etPlus.createTableModel(len, nofCols, this);
    etPlus.addHeaders(ui->tableViewLeastSquare, headerList, nofCols);


    etMinus.createTableModel(len, nofCols, this);
    etMinus.addHeaders(ui->tableViewLeastSquare_2, headerList, nofCols);

    // Init columm head click event handlers for tableView
    horizHeaderPlus= ui->tableViewLeastSquare->horizontalHeader();
    horizHeaderMinus= ui->tableViewLeastSquare_2->horizontalHeader();

    connect(horizHeaderPlus,  SIGNAL(sectionClicked(int)), this, SLOT(tablePlusHeaderClicked(int)));
    connect(horizHeaderMinus, SIGNAL(sectionClicked(int)), this, SLOT(tableMinusHeaderClicked(int)));

    for(i = 0; i < FA_NOF_DATA; i++ )
    {
        m_faDataPalette[i] = new QPalette();
    }


    // One day is added first
    for(rowPlus = 0, rowMinus = 0, j = 0; j < len; j++)
    {
        len1 = m_stockArr[j].data.x.count();

        // Is there enough data to calc least square on?
        if(len1 < 2)
        {
            break;
        }

        if(len1 > 6)
        {
            len1 = 6;
        }

        for(i = len1 -1; i >= 0; i--)
        {
            db.gather1dLeastSrqData(m_stockArr[j].data.x[i],
                                 m_stockArr[j].data.y[i],
                                 nofData,
                                 meanXSum,
                                 meanYSum,
                                 prodXXSum,
                                 prodYYSum,
                                 prodXYSum);
        }



        if((true == db.calc1dLeastSrqFitRParam(nofData,prodXXSum, prodYYSum, prodXYSum, r)) || (true == ui->showAllCheckBox->isChecked()) )
        {
            if((true == db.calc1dLeastSrqFitParams(nofData, meanXSum, meanYSum, prodXXSum, prodYYSum, prodXYSum, m, k)) || (true == ui->showAllCheckBox->isChecked()))
            {

                bool res = db.companynameGetKeyDataUseBridge(m_stockArr[j].stockName, keyData);

                if(m_stockArr[j].stockName.compare(QString::fromUtf8("Lundbergföretagen AB, L E ser. B"))== 0)
                {
                    j++;
                    j--;
                }

                if(true == res)
                {
                    riskReturnColor = Qt::magenta;
                    riskReturnData.stockSymbol = keyData.assetSymbol;
                    calcRiskAndReturn(startDate, endDate, riskReturnData);

                    riskReturnColor = gfc.getColorRiskReturns(riskReturnData.riskStdDev, riskReturnData.meanReturns);
                    riskStr.sprintf("%.3f", riskReturnData.riskStdDev);
                    returnStr.sprintf("%.3f", riskReturnData.meanReturns);

                    tbsi.faSellBuyIndicator(keyData,
                                       riskReturnData.riskStdDev,
                                       buyIndicator,
                                       sellIndicator);

                    if(buyIndicator == TaBuySellIdicator::FA_IND_BUY_TRUE)
                    {
                        if(true == res)
                        {
                            Qt::GlobalColor color1 = Qt::black;

                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, riskStr, rowPlus, 11, riskReturnColor);
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, returnStr, rowPlus, 12, riskReturnColor);

                            gfc.getColorPe(keyData.keyValuePE, color1);
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.keyValuePE, rowPlus, 6, color1);

                            color1 = Qt::black;
                            gfc.getColorPs(keyData.keyValuePS, color1);
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.keyValuePS, rowPlus, 7, color1);

                            gfc.getColorYield(keyData.keyValueYield, keyData.earningsDividedByDividend, color1);
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.keyValueYield, rowPlus, 8, color1);

                            gfc.getColorEarningsDivDividend(keyData.earningsDividedByDividend, color1);
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.earningsDividedByDividend, rowPlus, 9, color1);

                            color1 = Qt::black;
                            gfc.getColorNavDivStockPrice(keyData.navDivLastStockPrice, color1);
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.navDivLastStockPrice, rowPlus, 10, color1);

                            // Asset symbol
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.assetSymbol, rowPlus, 1, Qt::black);


                            if( true == m_taBuy.getAvgBuySellSignals(keyData.assetSymbol,
                                                                     sellSignals,
                                                                     buySignals,
                                                                     sellMomentumSignals,
                                                                     buyMomentumSignals))
                            {
                                QString buySignal;
                                buySignal = m_taBuy.convAvgBuySignalToNumber(buySignals);
                                if(buySignal.compare(QString::fromUtf8("0")) != 0)
                                {
                                    etPlus.addDataSetTextColor(ui->tableViewLeastSquare, buySignal, rowPlus, 4, Qt::blue);
                                }

                                QString sellSignal;
                                sellSignal = m_taBuy.convAvgSellSignalToNumber(sellSignals);
                                if(sellSignal.compare(QString::fromUtf8("0")) != 0)
                                {
                                    etPlus.addDataSetTextColor(ui->tableViewLeastSquare, sellSignal, rowPlus, 4, Qt::red);
                                }

                                //===============================================================
                                // Momentum sell buy signal
                                //===============================================================
                                buySignal = m_taBuy.convMomentumBuySignalToNumber(buyMomentumSignals);
                                if(buySignal.compare(QString::fromUtf8("0")) != 0)
                                {
                                    etPlus.addDataSetTextColor(ui->tableViewLeastSquare, buySignal, rowPlus, 5, Qt::blue);
                                }

                                sellSignal = m_taBuy.convMomentumSellSignalToNumber(sellMomentumSignals);
                                if(sellSignal.compare(QString::fromUtf8("0")) != 0)
                                {
                                    etPlus.addDataSetTextColor(ui->tableViewLeastSquare, sellSignal, rowPlus, 5, Qt::red);
                                }

                            }
                        }
                        else
                        {
                            keyData.assetSymbol = "";
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.assetSymbol, rowPlus, 1, Qt::black);
                        }

                        // Add stockname column 0
                        etPlus.addDataSetTextColor(ui->tableViewLeastSquare, m_stockArr[j].stockName, rowPlus, 0, Qt::black);

                        // Add R on column 2
                        str.sprintf("%.2f", r);
                        etPlus.addDataSetTextColor(ui->tableViewLeastSquare, str, rowPlus, 2, Qt::black);

                        // Add K on column 3
                        str.sprintf("%.2f", k);
                        if(k >= 0)
                        {
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, str, rowPlus, 3, Qt::blue);
                        }
                        else
                        {
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, str, rowPlus, 3, Qt::red);
                        }
                        rowPlus++;
                    }
                    else if(sellIndicator == true)
                    {
                        // Add stock symbol Column 1
                        if(true == db.companynameGetKeyDataUseBridge(m_stockArr[j].stockName, keyData))
                        {

                            Qt::GlobalColor color1 = Qt::black;

                            Qt::GlobalColor riskReturnColor = Qt::magenta;

                            CDbHndl::EfficPortStockData_ST riskReturnData;
                            riskReturnData.stockSymbol = keyData.assetSymbol;

                            calcRiskAndReturn(startDate, endDate, riskReturnData);

                            riskReturnColor = gfc.getColorRiskReturns(riskReturnData.riskStdDev, riskReturnData.meanReturns);

                            riskStr.sprintf("%.3f", riskReturnData.riskStdDev);

                            returnStr.sprintf("%.3f", riskReturnData.meanReturns);

                            etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, riskStr, rowMinus, 11, riskReturnColor);
                            etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, returnStr, rowMinus, 12, riskReturnColor);


                            gfc.getColorPe(keyData.keyValuePE, color1);
                            etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.keyValuePE, rowMinus, 6, color1);

                            gfc.getColorPs(keyData.keyValuePS, color1);
                            etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.keyValuePS, rowMinus, 7, color1);

                            gfc.getColorYield(keyData.keyValueYield, keyData.earningsDividedByDividend, color1);
                            etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.keyValueYield, rowMinus, 8, color1);

                            gfc.getColorEarningsDivDividend(keyData.earningsDividedByDividend, color1);
                            etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.earningsDividedByDividend, rowMinus, 9, color1);

                            color1 = Qt::black;
                            gfc.getColorNavDivStockPrice(keyData.navDivLastStockPrice, color1);
                            etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.navDivLastStockPrice, rowMinus, 10, color1);

                            // Asset symbol
                            if( true == m_taBuy.getAvgBuySellSignals(keyData.assetSymbol,
                                                                             sellSignals,
                                                                             buySignals,
                                                                             sellMomentumSignals,
                                                                             buyMomentumSignals))
                            {

                                //============================================================
                                // Trend sell buy signal
                                //============================================================
                                QString buySignal;
                                buySignal = m_taBuy.convAvgBuySignalToNumber(buySignals);
                                if(buySignal.compare(QString::fromUtf8("0")) != 0)
                                {
                                    etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, buySignal, rowMinus, 4, Qt::blue);
                                }

                                #if 1
                                QString sellSignal;
                                sellSignal = m_taBuy.convAvgSellSignalToNumber(sellSignals);
                                if(sellSignal.compare(QString::fromUtf8("0")) != 0)
                                {
                                    etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, sellSignal, rowMinus, 4, Qt::red);
                                }
                                #endif

                                //===============================================================
                                // Momentum sell buy signal
                                //===============================================================
                                buySignal = m_taBuy.convMomentumBuySignalToNumber(buyMomentumSignals);
                                if(buySignal.compare(QString::fromUtf8("0")) != 0)
                                {
                                    etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, buySignal, rowMinus, 5, Qt::blue);
                                }

                                sellSignal = m_taBuy.convMomentumSellSignalToNumber(sellMomentumSignals);
                                if(sellSignal.compare(QString::fromUtf8("0")) != 0)
                                {
                                    etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, sellSignal, rowMinus, 5, Qt::red);
                                }

                            }

                            etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.assetSymbol, rowMinus, 1, Qt::black);
                        }
                        else
                        {
                            keyData.assetSymbol = "";
                            etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.assetSymbol, rowMinus, 1, Qt::black);
                        }

                        // Add stock name Column 0
                        etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, m_stockArr[j].stockName, rowMinus, 0, Qt::black);

                        // Add R on Column 2
                        str.sprintf("%.2f", r);
                        etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, str, rowMinus, 2, Qt::black);

                        // Add K on Column 3
                        str.sprintf("%.2f", k);
                        if(k >= 0)
                        {
                            etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, str, rowMinus, 3, Qt::blue);
                        }
                        else
                        {
                            etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, str, rowMinus, 3, Qt::red);
                        }
                        rowMinus++;
                    }
                }
            }
         }
    }

    ui->tableViewLeastSquare->resizeColumnsToContents();
    ui->tableViewLeastSquare->sortByColumn(2, Qt::DescendingOrder);

    ui->tableViewLeastSquare_2->resizeColumnsToContents();
    ui->tableViewLeastSquare_2->sortByColumn(2, Qt::DescendingOrder);

    m_mutex.unlock();

}
#endif






/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 *******************************************************************/
void LeastSquaresTaDlg::on_SellBuyBridgeListpushButton_2_clicked()
{
    int i;
    int rowPlus;
    int rowMinus;
    int j;
    int len;
    int len1;
    int nofCols;
    CDbHndl db;
    QString str;
    GuiFinanceColor gfc;
    CDbHndl::snapshotStockData_ST keyData;
    TaBuySellIdicator::SellSignalMovingAvgST sellSignals;
    TaBuySellIdicator::BuySignalMovingAvgST buySignals;
    TaBuySellIdicator::SellSignalMomentumST sellMomentumSignals;
    TaBuySellIdicator::BuySignalMomentumST buyMomentumSignals;

    int nofData = 0;
    double meanXSum = 0;
    double meanYSum = 0;
    double prodXXSum = 0;
    double prodYYSum = 0;
    double prodXYSum = 0;
    double k;
    double m;
    double r;

    QHeaderView *horizHeaderPlus;
    QHeaderView *horizHeaderMinus;

    CExtendedTable etPlus;
    CExtendedTable etMinus;
    TableColumnIndicatorInfo_ST headerList[CDbHndl::MAX_NOF_GPSEL_INDEX];

    CUtil cu;
    QString startDate;
    QString endDate;
    int intMonth = -12;

    bool riskReturnsIsValid = false;
    QString riskStr;
    QString returnStr;
    CDbHndl::EfficPortStockData_ST riskReturnData;

    Qt::GlobalColor riskReturnColor = Qt::magenta;
    Qt::GlobalColor color1;
    bool sellIndicator = false;
    TaBuySellIdicator::FaBuyIndicatorStateET buyIndicator = TaBuySellIdicator::FA_IND_BUY_NOT_SET;
    TaBuySellIdicator tbsi;


    etPlus.setHorizontalFont(ui->tableViewLeastSquare, "Helvetica", 9);
    etPlus.setVerticalFont(ui->tableViewLeastSquare, "Helvetica", 9);

    etMinus.setHorizontalFont(ui->tableViewLeastSquare_2, "Helvetica", 9);
    etMinus.setVerticalFont(ui->tableViewLeastSquare_2, "Helvetica", 9);

    nofCols = 0;

    headerList[nofCols++].name = QString::fromUtf8("Företag     ");
    headerList[nofCols++].name = QString::fromUtf8("Symbol      ");
    headerList[nofCols++].name = QString::fromUtf8("Skuld/\nEget Kap");
    headerList[nofCols++].name = QString::fromUtf8("K");
    headerList[nofCols++].name = QString::fromUtf8("T Köp /\nSälj");
    headerList[nofCols++].name = QString::fromUtf8("M Köp /\nSälj");


    headerList[nofCols++].name = QString::fromUtf8("PE     ");
    headerList[nofCols++].name = QString::fromUtf8("PS     ");
    headerList[nofCols++].name = QString::fromUtf8("DirAvk");
    headerList[nofCols++].name = QString::fromUtf8("Vinst/\nDAvk");
    headerList[nofCols++].name = QString::fromUtf8("Sub/\nKurs");

    headerList[nofCols++].name = QString::fromUtf8("Förv.\nRisk");
    headerList[nofCols++].name = QString::fromUtf8("Förv.\nAvkst");
    headerList[nofCols++].name = QString::fromUtf8("index");
    headerList[nofCols++].name = QString::fromUtf8("balans-\nlikvd.");


    cu.getCurrentDate(endDate);

    if(false == cu.addMonth(endDate, startDate, intMonth))
    {
        return;
    }



    m_mutex.lock();

    // Init least sqrt
    db.init1dLeastSrq(nofData, meanXSum, meanYSum, prodXXSum, prodYYSum, prodXYSum);

    int len10 = m_stockArr.size();
    for( int i = 0; i < len10; i++)
    {
        m_stockArr[i].data.x.clear();
        m_stockArr[i].data.y.clear();
    }

    m_stockArr.clear();


    // Request snapshot data from database
    if(true == db.getAllSnapshotPlotStocksData(m_stockArr))
    {
        len = m_stockArr.size();
        // Used by risk & returns plot
        removeQwtPlotArrMemSpace();
        createQwtPlotArrMemSpace(len);
        m_nofRiskReturPlotData = len;
        m_minMaxReturPlotArrBuy.minMaxIsInit = false;
        m_minMaxReturPlotArrSell.minMaxIsInit = false;



        // Init table
        etPlus.createTableModel(len, nofCols, this);
        etPlus.addHeaders(ui->tableViewLeastSquare, headerList, nofCols);


        etMinus.createTableModel(len, nofCols, this);
        etMinus.addHeaders(ui->tableViewLeastSquare_2, headerList, nofCols);

        // Init columm head click event handlers for tableView
        horizHeaderPlus= ui->tableViewLeastSquare->horizontalHeader();
        horizHeaderMinus= ui->tableViewLeastSquare_2->horizontalHeader();

        connect(horizHeaderPlus,  SIGNAL(sectionClicked(int)), this, SLOT(tablePlusHeaderClicked(int)));
        connect(horizHeaderMinus, SIGNAL(sectionClicked(int)), this, SLOT(tableMinusHeaderClicked(int)));

        for(i = 0; i < FA_NOF_DATA; i++ )
        {
            m_faDataPalette[i] = new QPalette();
        }


        // One day is added first
        for(rowPlus = 0, rowMinus = 0, j = 0; j < len; j++)
        {
            len1 = m_stockArr[j].data.x.count();
            // Is there enough data to calc least square on?
            if(len1 < 2)
            {
                break;
            }

            if(len1 > 6)
            {
                len1 = 6;
            }

            for(i = len1 -1; i >= 0; i--)
            {
                db.gather1dLeastSrqData(m_stockArr[j].data.x[i],
                                     m_stockArr[j].data.y[i],
                                     nofData,
                                     meanXSum,
                                     meanYSum,
                                     prodXXSum,
                                     prodYYSum,
                                     prodXYSum);
            }




            if(true == db.calc1dLeastSrqFitRParam(nofData,prodXXSum, prodYYSum, prodXYSum, r) || (true == ui->showAllCheckBox->isChecked()))
            {
                if(true == db.calc1dLeastSrqFitParams(nofData, meanXSum, meanYSum, prodXXSum, prodYYSum, prodXYSum, m, k) || (true == ui->showAllCheckBox->isChecked()))
                {
                    bool res = db.companynameGetKeyDataUseBridge(m_stockArr[j].stockName, keyData);
                    if( true == res)
                    {
                        if(j == 109 || j == 86)
                        {
                            j++;
                            j--;
                        }

                        riskReturnColor = Qt::magenta;

                        riskReturnData.stockSymbol = keyData.assetSymbol;
                        riskReturnsIsValid = false;
                        if(true == calcRiskAndReturn(startDate, endDate, riskReturnData))
                        {
                            riskReturnsIsValid = true;

                            tbsi.faSellBuyIndicator(keyData, riskReturnData.riskStdDev, buyIndicator, sellIndicator);
                        }

                        if(buyIndicator == TaBuySellIdicator::FA_IND_BUY_TRUE && riskReturnsIsValid == true)
                        {
                            addRiskReturnsPlotData(j, riskReturnData.riskStdDev, riskReturnData.meanReturns, 0);
                        }
                        else if(true == sellIndicator && riskReturnsIsValid == true)
                        {
                            addRiskReturnsPlotData(j, riskReturnData.riskStdDev, riskReturnData.meanReturns, 1);
                        }

                    }


                    if(buyIndicator == TaBuySellIdicator::FA_IND_BUY_TRUE)
                    {
                        // Add stock symbol Column 1
                        if(true == db.companynameGetKeyDataUseBridge(m_stockArr[j].stockName, keyData))
                        {
                            color1 = Qt::black;

                            if(true == riskReturnsIsValid)
                            {
                                riskReturnColor = gfc.getColorRiskReturns(riskReturnData.riskStdDev, riskReturnData.meanReturns);
                                riskStr.sprintf("%.3f", riskReturnData.riskStdDev);
                                returnStr.sprintf("%.3f", riskReturnData.meanReturns);

                                etPlus.addDataSetTextColor(ui->tableViewLeastSquare, riskStr, rowPlus, 11, riskReturnColor);
                                etPlus.addDataSetTextColor(ui->tableViewLeastSquare, returnStr, rowPlus, 12, riskReturnColor);

                                // Add stockname column 0
                                QString str1;
                                str1.sprintf("%d, ", j);
                                etPlus.addDataSetTextColor(ui->tableViewLeastSquare, str1, rowPlus, 13, Qt::black);

                            }

                            gfc.getColorPe(keyData.keyValuePE, color1);
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.keyValuePE, rowPlus, 6, color1);

                            color1 = Qt::black;
                            gfc.getColorPs(keyData.keyValuePS, color1);
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.keyValuePS, rowPlus, 7, color1);

                            gfc.getColorYield(keyData.keyValueYield, keyData.earningsDividedByDividend, color1);
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.keyValueYield, rowPlus, 8, color1);

                            gfc.getColorEarningsDivDividend(keyData.earningsDividedByDividend, color1);
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.earningsDividedByDividend, rowPlus, 9, color1);

                            color1 = Qt::black;
                            gfc.getColorNavDivStockPrice(keyData.navDivLastStockPrice, color1);
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.navDivLastStockPrice, rowPlus, 10, color1);

                            // Asset symbol
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.assetSymbol, rowPlus, 1, Qt::black);


                            if( true == m_taBuy.getAvgBuySellSignals(keyData.assetSymbol,
                                                                     sellSignals,
                                                                     buySignals,
                                                                     sellMomentumSignals,
                                                                     buyMomentumSignals))
                            {
                                QString buySignal;
                                buySignal = m_taBuy.convAvgBuySignalToNumber(buySignals);
                                if(buySignal.compare(QString::fromUtf8("0")) != 0)
                                {
                                    etPlus.addDataSetTextColor(ui->tableViewLeastSquare, buySignal, rowPlus, 4, Qt::blue);
                                }

                                QString sellSignal;
                                sellSignal = m_taBuy.convAvgSellSignalToNumber(sellSignals);
                                if(sellSignal.compare(QString::fromUtf8("0")) != 0)
                                {
                                    etPlus.addDataSetTextColor(ui->tableViewLeastSquare, sellSignal, rowPlus, 4, Qt::red);
                                }

                                //===============================================================
                                // Momentum sell buy signal
                                //===============================================================
                                buySignal = m_taBuy.convMomentumBuySignalToNumber(buyMomentumSignals);
                                if(buySignal.compare(QString::fromUtf8("0")) != 0)
                                {
                                    etPlus.addDataSetTextColor(ui->tableViewLeastSquare, buySignal, rowPlus, 5, Qt::blue);
                                }

                                sellSignal = m_taBuy.convMomentumSellSignalToNumber(sellMomentumSignals);
                                if(sellSignal.compare(QString::fromUtf8("0")) != 0)
                                {
                                    etPlus.addDataSetTextColor(ui->tableViewLeastSquare, sellSignal, rowPlus, 5, Qt::red);
                                }

                            }
                        }
                        else
                        {
                            keyData.assetSymbol = "";
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, keyData.assetSymbol, rowPlus, 1, Qt::black);
                        }

                        // Add stockname column 0
                        etPlus.addDataSetTextColor(ui->tableViewLeastSquare, m_stockArr[j].stockName, rowPlus, 0, Qt::black);

                        if(keyData.assetSymbol.compare(QString::fromUtf8("Öresund, Investment AB")) == 0)
                        {
                            j++;
                            j--;
                        }


                        // Add R on column 2
                        // ajn 160321cstr.sprintf("%.2f", r);
                        // ajn 160321cetPlus.addDataSetTextColor(ui->tableViewLeastSquare, str, rowPlus, 2, Q   t::black);
                         double debtToEquityRatio = 0;
                         double currentRatio = 0;
                         if(true == db.getYahooKeyData(keyData.assetSymbol, debtToEquityRatio, currentRatio))
                         //if(true == db.getDebtToEquityRatio(keyData.assetSymbol, debtToEquityRatio))
                         {
                             str.sprintf("%.2f", debtToEquityRatio);

                             // This is totaly dept so we set 75 instead of 50
                             if(debtToEquityRatio > 150.0)
                             {
                                etPlus.addDataSetTextColor(ui->tableViewLeastSquare, str, rowPlus, 2, Qt::red);
                             }
                             else if(debtToEquityRatio > 100.0 && debtToEquityRatio < 150.0)
                             {
                                 etPlus.addDataSetTextColor(ui->tableViewLeastSquare, str, rowPlus, 2, Qt::magenta);
                             }
                             else
                             {
                                 etPlus.addDataSetTextColor(ui->tableViewLeastSquare, str, rowPlus, 2, Qt::darkGreen);
                             }

                             str.sprintf("%.2f", currentRatio);

                             // This is totaly dept so we set 75 instead of 50
                             if(currentRatio > 1.5)
                             {
                                etPlus.addDataSetTextColor(ui->tableViewLeastSquare, str, rowPlus, 14, Qt::darkGreen);
                             }
                             else if(currentRatio > 1.0 && currentRatio < 1.5)
                             {
                                 etPlus.addDataSetTextColor(ui->tableViewLeastSquare, str, rowPlus, 14, Qt::magenta);
                             }
                             else
                             {
                                 etPlus.addDataSetTextColor(ui->tableViewLeastSquare, str, rowPlus, 14, Qt::red);
                             }


                         }
                         else
                         {
                             str = " ";
                             etPlus.addDataSetTextColor(ui->tableViewLeastSquare, str, rowPlus, 2, Qt::black);
                         }


                        // Add K on column 3
                        str.sprintf("%.2f", k);
                        if(k >= 0)
                        {
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, str, rowPlus, 3, Qt::blue);
                        }
                        else
                        {
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, str, rowPlus, 3, Qt::red);
                        }
                        rowPlus++;
                    }
                    else if(sellIndicator == true)
                    {

                        // if(true == db.companynameGetKeyDataUseBridge(m_stockArr[j].stockName, keyData))
                        {

                            color1 = Qt::black;

                            if(true == riskReturnsIsValid)
                            {
                                riskReturnColor = Qt::magenta;

                                // Add risk & returns index
                                QString str1;
                                str1.sprintf("%d, ", j);
                                etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, str1, rowMinus, 13, Qt::black);


                                riskReturnColor = gfc.getColorRiskReturns(riskReturnData.riskStdDev, riskReturnData.meanReturns);

                                QString riskStr;
                                riskStr.sprintf("%.3f", riskReturnData.riskStdDev);

                                QString returnStr;
                                returnStr.sprintf("%.3f", riskReturnData.meanReturns);

                                etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, riskStr, rowMinus, 11, riskReturnColor);

                                etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, returnStr, rowMinus, 12, riskReturnColor);
                            }


                            gfc.getColorPe(keyData.keyValuePE, color1);
                            etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.keyValuePE, rowMinus, 6, color1);

                            gfc.getColorPs(keyData.keyValuePS, color1);
                            etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.keyValuePS, rowMinus, 7, color1);

                            gfc.getColorYield(keyData.keyValueYield, keyData.earningsDividedByDividend, color1);
                            etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.keyValueYield, rowMinus, 8, color1);

                            gfc.getColorEarningsDivDividend(keyData.earningsDividedByDividend, color1);
                            etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.earningsDividedByDividend, rowMinus, 9, color1);

                            color1 = Qt::black;
                            gfc.getColorNavDivStockPrice(keyData.navDivLastStockPrice, color1);
                            etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.navDivLastStockPrice, rowMinus, 10, color1);

                                // Asset symbol
                                if( true == m_taBuy.getAvgBuySellSignals(keyData.assetSymbol,
                                                                                 sellSignals,
                                                                                 buySignals,
                                                                                 sellMomentumSignals,
                                                                                 buyMomentumSignals))
                                {

                                    //============================================================
                                    // Trend sell buy signal
                                    //============================================================
                                    QString buySignal;
                                    buySignal = m_taBuy.convAvgBuySignalToNumber(buySignals);
                                    if(buySignal.compare(QString::fromUtf8("0")) != 0)
                                    {
                                        etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, buySignal, rowMinus, 4, Qt::blue);
                                    }

                                    QString sellSignal;
                                    sellSignal = m_taBuy.convAvgSellSignalToNumber(sellSignals);
                                    if(sellSignal.compare(QString::fromUtf8("0")) != 0)
                                    {
                                        etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, sellSignal, rowMinus, 4, Qt::red);
                                    }

                                    //===============================================================
                                    // Momentum sell buy signal
                                    //===============================================================
                                    buySignal = m_taBuy.convMomentumBuySignalToNumber(buyMomentumSignals);
                                    if(buySignal.compare(QString::fromUtf8("0")) != 0)
                                    {
                                        etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, buySignal, rowMinus, 5, Qt::blue);
                                    }

                                    sellSignal = m_taBuy.convMomentumSellSignalToNumber(sellMomentumSignals);
                                    if(sellSignal.compare(QString::fromUtf8("0")) != 0)
                                    {
                                        etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, sellSignal, rowMinus, 5, Qt::red);
                                    }

                                }

                                etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.assetSymbol, rowMinus, 1, Qt::black);
                            }
                            //else
                            //{
                            //    keyData.assetSymbol = "";
                            //    etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, keyData.assetSymbol, rowMinus, 1, Qt::black);
                            //}

                            // Add stock name Column 0
                            etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, m_stockArr[j].stockName, rowMinus, 0, Qt::black);

                            // Add R on Column 2
                            // ajn 160321 str.sprintf("%.2f", r);
                            // ajn 160321 etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, str, rowMinus, 2, Qt::black);

                            double debtToEquityRatio = 0;
                            double currentRatio = 0;
                            if(true == db.getYahooKeyData(keyData.assetSymbol, debtToEquityRatio, currentRatio))
                            //if(true == db.getDebtToEquityRatio(keyData.assetSymbol, debtToEquityRatio))
                            {
                                str.sprintf("%.2f", debtToEquityRatio);
                                if(debtToEquityRatio > 150.0)
                                {
                                    etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, str, rowMinus, 2, Qt::red);
                                }
                                else if(debtToEquityRatio > 100.0 && debtToEquityRatio < 150.0)
                                {
                                    etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, str, rowMinus, 2, Qt::magenta);
                                }
                                else
                                {
                                    etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, str, rowMinus, 2, Qt::darkGreen);
                                }


                                str.sprintf("%.2f", currentRatio);

                                // This is totaly dept so we set 75 instead of 50
                                if(currentRatio > 1.5)
                                {
                                   etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, str, rowMinus, 14, Qt::darkGreen);
                                }
                                else if(currentRatio > 1.0 && currentRatio < 1.5)
                                {
                                    etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, str, rowMinus, 14, Qt::magenta);
                                }
                                else
                                {
                                    etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, str, rowMinus, 14, Qt::red);
                                }

                            }
                            else
                            {
                                str = " ";
                                etPlus.addDataSetTextColor(ui->tableViewLeastSquare, str, rowPlus, 2, Qt::black);
                            }


                            // Add K on Column 3
                            str.sprintf("%.2f", k);
                            if(k >= 0)
                            {
                                etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, str, rowMinus, 3, Qt::blue);
                            }
                            else
                            {
                                etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, str, rowMinus, 3, Qt::red);
                            }
                            rowMinus++;
                        }
                    }
             }
        }

        ui->tableViewLeastSquare->resizeColumnsToContents();
        ui->tableViewLeastSquare->sortByColumn(2, Qt::DescendingOrder);

        ui->tableViewLeastSquare_2->resizeColumnsToContents();
        ui->tableViewLeastSquare_2->sortByColumn(2, Qt::DescendingOrder);

    }

    // Risk & return
    plotXAxis(ui->qwtPlotLSqr_3, m_plotGridBuy, m_minMaxReturPlotArrBuy, m_xAxisPlotBuy);
    plotXAxis(ui->qwtPlotLSqr_4, m_plotGridSell, m_minMaxReturPlotArrSell, m_xAxisPlotSell);
    plotQwtData(m_nofRiskReturPlotData, ui->qwtPlotLSqr_3, m_riskReturPlotArrBuy);
    plotQwtData(m_nofRiskReturPlotData, ui->qwtPlotLSqr_4, m_riskReturPlotArrSell);

    m_mutex.unlock();
}


/*********************************************************************
 * Function: ()
 *
 * Description: .
 *
 *********************************************************************/
void LeastSquaresTaDlg::plotQwtData(int nofCurves, QwtPlot *qwtPlot, QwtPlotCurve *riskReturPlotArr)
{
    CUtil cu;

    for(int i = 0; i < nofCurves; i++)
    {
        riskReturPlotArr[i].setSymbol( new QwtSymbol( QwtSymbol::Ellipse, cu.getQColor((CUtil::ColorRgb_ET)i)
                                                        /*Qt::black*/, QPen(cu.getQColor((CUtil::ColorRgb_ET)i)
                                                        /*Qt::red*/ ), QSize( 15, 15 ) ) );

        riskReturPlotArr[i].setPen( QPen( Qt::black) );
        riskReturPlotArr[i].attach(qwtPlot);
    }
    qwtPlot->replot();
}




/*********************************************************************
 * Function: ()
 *
 * Description: Risk & return
 *
 *********************************************************************/
void LeastSquaresTaDlg::
plotXAxis(QwtPlot *qwtPlot,
          QwtPlotGrid *plotGrid,
          CYahooStockPlotUtil::MinMaxAxisValues_ST &minMaxReturPlotArr,
          QwtPlotCurve *xAxisPlot)
{
    plotGrid->enableXMin(true);
    plotGrid->setMajPen(QPen(Qt::darkYellow, 0, Qt::DotLine));
    plotGrid->setMinPen(QPen(Qt::darkYellow, 0 , Qt::DotLine));
    plotGrid->attach(qwtPlot);

    double x[2];
    double y[2];

    double minX, minY, maxX, maxY;

    if(minMaxReturPlotArr.minX >= 0)
        minX = minMaxReturPlotArr.minX*0.9;
    else
        minX = minMaxReturPlotArr.minX*1.1;


    if(minMaxReturPlotArr.minY >= 0)
        minY = minMaxReturPlotArr.minY*0.9;
    else
        minY = minMaxReturPlotArr.minY*1.1;

    if(minMaxReturPlotArr.maxX >= 0)
        maxX = minMaxReturPlotArr.maxX*1.1;
    else
        maxX = minMaxReturPlotArr.maxX*0.9;


    if(minMaxReturPlotArr.maxY >= 0)
        maxY = minMaxReturPlotArr.maxY*1.1;
    else
        maxY = minMaxReturPlotArr.maxY*0.9;



    x[0] = minX;
    x[1] = maxX;
    y[0] = 0;
    y[1] = 0;

    xAxisPlot->setSamples(x,y, 2);
    xAxisPlot->setPen(QPen( Qt::black, 2));


    qwtPlot->setAxisScale(QwtPlot::xBottom,
                          minX,
                          (maxX));
    qwtPlot->setAxisScale(QwtPlot::yLeft,
                          minY,
                          (maxY)); // Max av % satser

    xAxisPlot->attach(qwtPlot);

}
