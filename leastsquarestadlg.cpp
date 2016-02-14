#include "leastsquarestadlg.h"
#include "ui_leastsquarestadlg.h"
#include "dbHndl.h"
#include "extendedTable.h"
#include "util.h"

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
    // ui->moveTimePeriodSlider_2->setValue(value);
    m_timeSlideWindowInc = value;

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
    int i;
    int rowPlus;
    int rowMinus;
    int j;
    int len;
    int len1;
    int nofCols;
    CDbHndl db;
    QString str;

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

    ui->setupUi(this);

    //=======================================================
    // Create thread that import data from yahoo
    m_importYahooTaDataThread = new importYahooTaData(this);

    if(m_importYahooTaDataThread == 0)
    {
        QMessageBox::information(NULL, QObject::tr("Create thread"), QString::fromUtf8("Fail to create Ta Import Data thread"));
    }

    // Init time period
    initTimePeriodCtrls(TIME_PERIOD_DAYS_1_YEAR);

    settimeSlideWindowInc(0);

    // Ska sättas tillbaka
    m_plot.initPlotPicker(ui->qwtPlotLSqr);
    m_plot.initPlotZoomer(ui->qwtPlotLSqr);
    m_plot.initPlotPanner(ui->qwtPlotLSqr);

    m_plot.enablePanningMode(false);
    m_plot.enableZoomMode(false);

    //=======================================================


    nofCols = 0;

    headerList[nofCols++].name = QString::fromUtf8("Företag");
    headerList[nofCols++].name = QString::fromUtf8("R");
    headerList[nofCols++].name = QString::fromUtf8("K");



    m_mutex.lock();
    db.openDb(PATH_JACK_STOCK_DB);


    // Init least sqrt
    db.init1dLeastSrq(nofData, meanXSum, meanYSum, prodXXSum, prodYYSum, prodXYSum);


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
        // setTableWidth(10);

        connect(horizHeaderPlus,  SIGNAL(sectionClicked(int)), this, SLOT(tablePlusHeaderClicked(int)));
        connect(horizHeaderMinus, SIGNAL(sectionClicked(int)), this, SLOT(tableMinusHeaderClicked(int)));


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


            if(true == db.calc1dLeastSrqFitRParam(nofData,prodXXSum, prodYYSum, prodXYSum, r))
            {
                if(true == db.calc1dLeastSrqFitParams(nofData, meanXSum, meanYSum, prodXXSum, prodYYSum, prodXYSum, m, k))
                {
                    if(r > 0.7)
                    {

                        etPlus.addDataSetTextColor(ui->tableViewLeastSquare, m_stockArr[j].stockName, rowPlus, 0, Qt::black);
                        str.sprintf("%f", r);
                        etPlus.addDataSetTextColor(ui->tableViewLeastSquare, str, rowPlus, 1, Qt::black);
                        str.sprintf("%f", k);
                        if(k >= 0)
                        {
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, str, rowPlus, 2, Qt::blue);
                        }
                        else
                        {
                            etPlus.addDataSetTextColor(ui->tableViewLeastSquare, str, rowPlus, 2, Qt::red);
                        }
                        rowPlus++;
                    }
                    else if(r < -0.7)
                    {

                        etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, m_stockArr[j].stockName, rowMinus, 0, Qt::black);
                        str.sprintf("%f", r);
                        etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, str, rowMinus, 1, Qt::black);
                        str.sprintf("%f", k);
                        if(k >= 0)
                        {
                            etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, str, rowMinus, 2, Qt::blue);
                        }
                        else
                        {
                            etMinus.addDataSetTextColor(ui->tableViewLeastSquare_2, str, rowMinus, 2, Qt::red);
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

    db.closeDb();
    m_mutex.unlock();

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
void LeastSquaresTaDlg::on_pushButton_clicked()
{

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
        intMonth = -6;
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
    int nofDays;

    // Init to valid date
    cu.getCurrentDate(m_startDate);
    cu.getCurrentDate(m_endDate);
    m_endDateRef = m_endDate;


    m_timePeriodDaysInc = value;
    ui->timePeriodLlineEditLSqrt->clear();
    ui->timePeriodLlineEditLSqrt->insert((QString)m_timePeriodDaysArr[value].TxtTimePeriod.toLatin1());
    ui->selTimePeriodSliderLSqrt->setValue(value);
    setTimePeriodDaysUpdateStartStopDate(m_startDate, m_endDate, value);

#if 0
    if(cu.nofDaysBeteenDates(m_startDate, m_endDate, nofDays) == true)
    {
       ui->moveTimePeriodSlider_2->setMaximum(nofDays);
    }
#endif


    // m_startDate.daysTo (m_endDate);

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
    m_stockData.data.indicator1.clear();
    m_stockData.data.indicator2.clear();
    m_stockData.data.indicator3.clear();
    m_stockData.data.indicator4.clear();

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
    // CYahooStockPlotUtil::MinMaxAxisValues_ST subPlotAxis;


    deleteAllPlotData(m_qwtStockPlotData, ui->qwtPlotLSqr);
    // deleteAllPlotData(m_qwtsubPlotData, ui->qwtPlotLSqr_2);

    // ajn 160214 m_macdHistData.clear();
    // ajn 160214 m_macdHist->detach();
    // ajn 160214 m_macdHist->setData(NULL);


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
    #if 0
    legendLable.clear();
    if(true == ui->checkBoxLinjerReg->isChecked())
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

        ui->StatusInfoLabel->setText(status);

        addStockIndicatorToPlot(indicatorIndex, plotIndex, legendLable, mainPlotAxis, ui->qwtPlotLSqr, m_qwtStockPlotData);
        // updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);
    }



    legendLable.clear();
    // Check the checkbox
    if(true == ui->checkBoxShortMvAvg->isChecked())
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
        // updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);

    }


    // Check the checkbox
    legendLable.clear();
    if(true == ui->checkBoxMidMvAvg->isChecked())
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
        // updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);
    }



    // Check the checkbox
    legendLable.clear();
    if(true == ui->checkBoxLongMvAvg->isChecked())
    {
        // updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);
        clearStockAndIndicatorTempMem();
        qDebug() << "xx size " << m_stockData.data.x.size();
        updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);
        db.getYahooTaMovingAverage(m_reqStockSymbol, startDate, endDate, 150, m_qwtStockPlotData, m_stockData);
        updateMinMaxXAxisScales(m_qwtStockPlotData.axis, mainPlotAxis);

        indicatorIndex = 1;
        plotIndex = PLOT_INDEX_LONG_MOVING_AVG;
        legendLable = "Long moving average";

        addStockIndicatorToPlot(indicatorIndex, plotIndex, legendLable, mainPlotAxis, ui->qwtPlotLSqr, m_qwtStockPlotData);
        // updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);
    }



    // Check the checkboxes
    legendLable.clear();
    if(true == ui->checkBoxBollingBand->isChecked())
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
        // updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);


        // Mid Bollingerband
        indicatorIndex++;
        plotIndex = PLOT_INDEX_BOLLINGERBAND_MID;
        legendLable = "Mid bollingerband";


        m_stockArr.clear();
        addStockIndicatorToPlot(indicatorIndex, plotIndex, legendLable, mainPlotAxis, ui->qwtPlotLSqr, m_qwtStockPlotData);
        // updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);

        // Upper Bollingerband
        indicatorIndex++;
        plotIndex = PLOT_INDEX_BOLLINGERBAND_UPPER;
        legendLable = "Upper bollingerband";

        m_stockArr.clear();
        addStockIndicatorToPlot(indicatorIndex, plotIndex, legendLable, mainPlotAxis, ui->qwtPlotLSqr, m_qwtStockPlotData);
        // updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);
    }




    //=========

    legendLable.clear();
    if(ui->radioButMacd->isChecked() == true)
    {
        clearStockAndIndicatorTempMem();
        db.getYahooTaMacd(m_reqStockSymbol, startDate, endDate, 26, 12, 9, m_qwtsubPlotData, m_stockData);

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


        m_macdHist-> setStyle (qwtPlotLSqrHistogram :: Columns);

        m_macdHistData.clear();
        m_macdHist->detach();
        m_macdHist->setData(NULL);

        for(int ii = 0; ii < m_stockData.data.indicator3.size(); ii++)
        {
            m_stockData.data.x.at(ii);
            m_macdHistData.append (QwtIntervalSample (m_stockData.data.indicator3.at(ii),m_stockData.data.x.at(ii),m_stockData.data.x.at(ii)+1));
        }

        //m_macdHist->setBrush(Qt::red);
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


        ui->StatusInfoLabel->setText(status);
        addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET) subplotIndex, legendLable, subPlotAxis, ui->qwtPlotLSqr_2, m_qwtsubPlotData);
        updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);
    }
//    updateMinMaxXAxisScales(subPlotAxis);

#endif


    //  boxesUpdate edit
    ui->lineEditAssetNameLSqrt->clear();
    ui->lineEditAssetNameLSqrt->insert(m_reqStockName);

    #if 0
    ui->lineEditStartDate->clear();
    ui->lineEditStartDate->insert(m_qwtStockPlotData.axis.minXDate);

    ui->lineEditStopDate->clear();
    ui->lineEditStopDate->insert(m_qwtStockPlotData.axis.maxXDate);
    #endif

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
 * Description: This function is invoked singel stock data is
 *              parsed by: m_importYahooTaDataThread
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
 * Function:    slotImportSingelStockDataIsFinish()
 *
 * Description: This function is invoked singel stock data is
 *              received from http thread
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
    isUpdate = true;

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



#if 0
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
    QString assetSymbol;
    CDbHndl::snapshotStockData_ST keyData;
    QModelIndex dummyIndex = index;


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



    // Get data form table
    ui->tableViewLeastSquare->selectionModel()->currentIndex().row();
    int rowidx = ui->tableViewLeastSquare->currentIndex().row();
    assetSymbol.append(ui->tableViewLeastSquare->model()->index(rowidx , 0).data().toString());

    db.openDb(PATH_JACK_STOCK_DB);
    if(true == db.companynameGetKeyDataUseBridge(assetSymbol, keyData))
    {
        // QMessageBox::information(NULL, QObject::tr("Tabell"), keyData.assetSymbol);
    }
    else
    {
        QMessageBox::information(NULL, QObject::tr("Tabell"), QString::fromUtf8("Symbolen saknas"));
        return;
    }




    // Are we already processing data?
    if(m_singleStockDataReqStatus == STATUS_REQ_SINGLE_STOCK_IDLE)
    {
        m_singleStockDataReqStatus = STATUS_REQ_SINGLE_STOCK_PENDING;
        // getSelStockListItem(stockName, stockSymbol, index);

        // ajn 160214
        stockSymbol = keyData.assetSymbol;
        stockName = keyData.companyName;

        m_reqStockSymbol = stockSymbol;
        m_reqStockName = stockName;

        qDebug() << m_reqStockSymbol;
        qDebug() << m_reqStockName;

        CDbHndl::snapshotStockData_ST keyFaData;

        #if 0
        ui->lineEditPE->setDisabled(false);
        ui->lineEditPE_2->setDisabled(false);


        ui->lineEditMoreInfoComapnyName->clear();
        ui->lineEditMoreInfoexecutiveDirector->clear();
        ui->lineEditMoreInfoBransch->clear();
        ui->lineEditMoreInfoSector->clear();
        ui->textEditMoreInfobusinessDescription->clear();

        //ui->lineEditMoreInfoComapnyName_2->clear();
        ui->lineEditMoreInfoexecutiveDirector_2->clear();
        ui->lineEditMoreInfoBransch_2->clear();
        ui->lineEditMoreInfoSector_2->clear();
        ui->textEditMoreInfobusinessDescription_2->clear();


        ui->lineEditEarningsDivByDividend->clear();
        ui->lineEditNavDivLastStockPrice->clear();
        ui->lineEditPE->clear();
        ui->lineEditPs->clear();
        ui->lineEditRisk->clear();
        ui->lineEditYield->clear();
        ui->lineMeanReturn->clear();

        ui->lineEditEarningsDivByDividend_2->clear();
        ui->lineEditNavDivLastStockPrice_2->clear();
        ui->lineEditPE_2->clear();
        ui->lineEditPs_2->clear();
        ui->lineEditYield_2->clear();

        ui->lineEditEarningsDivByDividend_3->clear();
        ui->lineEditNavDivLastStockPrice_3->clear();
        ui->lineEditPE_3->clear();
        ui->lineEditPs_3->clear();
        ui->lineEditYield_3->clear();

        ui->treeWidget_2->clear();
        ui->treeWidget_3->clear();
        ui->treeWidget_4->clear();
        ui->treeWidget_5->clear();
        ui->treeWidget_6->clear();
        QString assetType;
        assetType.clear();

        ui->assetType->clear();

        ui->labelLastPrice->clear();
        ui->labelOneDayProcentChange->clear();
        #endif

        #if 0
        if(m_startDate.isEmpty() == false && m_endDate.isEmpty() == false)
        {

            if(true == cd.getKeyDataUseBridge(m_reqStockSymbol,keyFaData))
            {
                qDebug() << keyFaData.companyName;

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

                cd.addFa3MinMaxPEAndPrice(ui->treeWidget_5, ui->treeWidget_6, keyFaData.companyName, m_reqStockSymbol);

                ui->lineEditEarningsDivByDividend->insert(keyFaData.earningsDividedByDividend);
                ui->lineEditNavDivLastStockPrice->insert(keyFaData.navDivLastStockPrice);
                ui->lineEditPE->insert(keyFaData.keyValuePE);
                ui->lineEditPs->insert(keyFaData.keyValuePS);
                ui->lineEditYield->insert(keyFaData.keyValueYield);

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



                CDbHndl::EfficPortStockData_ST stockRiskReturnData;
                stockRiskReturnData.stockSymbol = m_reqStockSymbol;
                if(true == cd.efficPortfCalcMeanAndStdDev(m_startDate, m_endDate, stockRiskReturnData))
                {
                    QString tmp;
                    tmp.sprintf("%.2f",stockRiskReturnData.riskStdDev * 100);
                    ui->lineEditRisk->insert(tmp);
                    tmp.sprintf("%.2f",stockRiskReturnData.meanReturns * 100);
                    ui->lineMeanReturn->insert(tmp);
                }
                setFundametalAnalysisCtrlTxtColor(keyFaData, stockRiskReturnData, assetType);
            }
        }
        #endif


        if(true == cd.getLatestDateYahooTaData(lastDbDate, keyData.assetSymbol))
        {
            cu.getCurrentDate(currDate);
            qDebug() << currDate;
            qDebug() << lastDbDate;


            cu.nofDaysBeteenDates(currDate, lastDbDate, nofDays);
            if(nofDays < -1)
            {
                bool isUpdate;

                findDateList(stockSymbol, isUpdate);
                QString tmp;
                tmp.sprintf("%d",isUpdate);
                qDebug() << tmp;
                qDebug() << stockSymbol;

                #if 0
                if(/*isUpdate == false &&*/ ui->checkBoxReqestData->isChecked() == true)
                {
                    // Prepare request of data from Yahoo
                    qDebug() << stockName;
                    qDebug() << stockSymbol;
                    qDebug() << lastDbDate;
                    qDebug() << currDate;

                    prepReqTaDataFromServer(stockName, stockSymbol, lastDbDate, currDate);
                }

                else
                #endif
                {
                    m_singleStockDataReqStatus = STATUS_REQ_SINGLE_STOCK_IDLE;
                    // ajn 160118 displayStockData(true, keyFaData.lastPrice.toDouble());
                    displayStockData(false, keyFaData.lastPrice.toDouble());
                }

            }
            else
            {
                m_singleStockDataReqStatus = STATUS_REQ_SINGLE_STOCK_IDLE;
                // ajn 160118 displayStockData(true, keyFaData.lastPrice.toDouble());
                displayStockData(false, keyFaData.lastPrice.toDouble());
            }
       }
        #if 0
        // No latest date was found lets add our own
        else
        {
            // Prepare request of data from Yahoo
            lastDbDate = "1992-01-01";
            cu.getCurrentDate(currDate);
            prepReqTaDataFromServer(stockName, stockSymbol, lastDbDate, currDate);
        }
        #endif
    }
    else
    {
        m_singleStockDataReqStatus = STATUS_REQ_SINGLE_STOCK_IDLE;
        QMessageBox::information(this, QString::fromUtf8("Vänta"), QString::fromUtf8("Vänta Processar redan data.."));
    }
}
#endif

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
    QModelIndex dummyIndex = index;



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
        QMessageBox::information(NULL, QObject::tr("Tabell"), QString::fromUtf8("Symbolen saknas"));
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

        #if 0
        ui->lineEditPE->setDisabled(false);
        ui->lineEditPE_2->setDisabled(false);


        ui->lineEditMoreInfoComapnyName->clear();
        ui->lineEditMoreInfoexecutiveDirector->clear();
        ui->lineEditMoreInfoBransch->clear();
        ui->lineEditMoreInfoSector->clear();
        ui->textEditMoreInfobusinessDescription->clear();

        //ui->lineEditMoreInfoComapnyName_2->clear();
        ui->lineEditMoreInfoexecutiveDirector_2->clear();
        ui->lineEditMoreInfoBransch_2->clear();
        ui->lineEditMoreInfoSector_2->clear();
        ui->textEditMoreInfobusinessDescription_2->clear();


        ui->lineEditEarningsDivByDividend->clear();
        ui->lineEditNavDivLastStockPrice->clear();
        ui->lineEditPE->clear();
        ui->lineEditPs->clear();
        ui->lineEditRisk->clear();
        ui->lineEditYield->clear();
        ui->lineMeanReturn->clear();

        ui->lineEditEarningsDivByDividend_2->clear();
        ui->lineEditNavDivLastStockPrice_2->clear();
        ui->lineEditPE_2->clear();
        ui->lineEditPs_2->clear();
        ui->lineEditYield_2->clear();

        ui->lineEditEarningsDivByDividend_3->clear();
        ui->lineEditNavDivLastStockPrice_3->clear();
        ui->lineEditPE_3->clear();
        ui->lineEditPs_3->clear();
        ui->lineEditYield_3->clear();

        ui->treeWidget_2->clear();
        ui->treeWidget_3->clear();
        ui->treeWidget_4->clear();
        ui->treeWidget_5->clear();
        ui->treeWidget_6->clear();
        QString assetType;
        assetType.clear();

        ui->assetType->clear();

        ui->labelLastPrice->clear();
        ui->labelOneDayProcentChange->clear();
        #endif

        #if 0
        if(m_startDate.isEmpty() == false && m_endDate.isEmpty() == false)
        {

            if(true == cd.getKeyDataUseBridge(m_reqStockSymbol,keyFaData))
            {
                qDebug() << keyFaData.companyName;

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

                cd.addFa3MinMaxPEAndPrice(ui->treeWidget_5, ui->treeWidget_6, keyFaData.companyName, m_reqStockSymbol);

                ui->lineEditEarningsDivByDividend->insert(keyFaData.earningsDividedByDividend);
                ui->lineEditNavDivLastStockPrice->insert(keyFaData.navDivLastStockPrice);
                ui->lineEditPE->insert(keyFaData.keyValuePE);
                ui->lineEditPs->insert(keyFaData.keyValuePS);
                ui->lineEditYield->insert(keyFaData.keyValueYield);

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



                CDbHndl::EfficPortStockData_ST stockRiskReturnData;
                stockRiskReturnData.stockSymbol = m_reqStockSymbol;
                if(true == cd.efficPortfCalcMeanAndStdDev(m_startDate, m_endDate, stockRiskReturnData))
                {
                    QString tmp;
                    tmp.sprintf("%.2f",stockRiskReturnData.riskStdDev * 100);
                    ui->lineEditRisk->insert(tmp);
                    tmp.sprintf("%.2f",stockRiskReturnData.meanReturns * 100);
                    ui->lineMeanReturn->insert(tmp);
                }
                setFundametalAnalysisCtrlTxtColor(keyFaData, stockRiskReturnData, assetType);
            }
        }
        #endif


        if(true == cd.getLatestDateYahooTaData(lastDbDate, inStockSymbol))
        {
            cu.getCurrentDate(currDate);
            qDebug() << currDate;
            qDebug() << lastDbDate;


            cu.nofDaysBeteenDates(currDate, lastDbDate, nofDays);
            if(nofDays < -1)
            {
                bool isUpdate;

                findDateList(stockSymbol, isUpdate);
                QString tmp;
                tmp.sprintf("%d",isUpdate);
                qDebug() << tmp;
                qDebug() << stockSymbol;

                #if 0
                if(/*isUpdate == false &&*/ ui->checkBoxReqestData->isChecked() == true)
                {
                    // Prepare request of data from Yahoo
                    qDebug() << stockName;
                    qDebug() << stockSymbol;
                    qDebug() << lastDbDate;
                    qDebug() << currDate;

                    prepReqTaDataFromServer(stockName, stockSymbol, lastDbDate, currDate);
                }

                else
                #endif
                {
                    m_singleStockDataReqStatus = STATUS_REQ_SINGLE_STOCK_IDLE;
                    // ajn 160118 displayStockData(true, keyFaData.lastPrice.toDouble());
                    displayStockData(false, keyFaData.lastPrice.toDouble());
                }

            }
            else
            {
                m_singleStockDataReqStatus = STATUS_REQ_SINGLE_STOCK_IDLE;
                // ajn 160118 displayStockData(true, keyFaData.lastPrice.toDouble());
                displayStockData(false, keyFaData.lastPrice.toDouble());
            }
       }
        #if 0
        // No latest date was found lets add our own
        else
        {
            // Prepare request of data from Yahoo
            lastDbDate = "1992-01-01";
            cu.getCurrentDate(currDate);
            prepReqTaDataFromServer(stockName, stockSymbol, lastDbDate, currDate);
        }
        #endif
    }
    else
    {
        m_singleStockDataReqStatus = STATUS_REQ_SINGLE_STOCK_IDLE;
        QMessageBox::information(this, QString::fromUtf8("Vänta"), QString::fromUtf8("Vänta Processar redan data.."));
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
        QMessageBox::information(NULL, QObject::tr("Tabell"), QString::fromUtf8("Symbolen saknas"));
        return;
    }

}
