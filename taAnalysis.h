#ifndef TAANALYSIS_H
#define TAANALYSIS_H

#include <QDialog>
#include "dbHndl.h"
#include "importYahooTaData.h"
#include "yahooStockPlotUtil.h"
#include "canvaspicker.h"
#include <qwt_plot_histogram.h>
#include "common.h"

#include <QString>
#include <QtCore>
#include <QtGui>

#include <QList>
#include <QString>


namespace Ui
{
class TaAnalysis;
}





class TaAnalysis : public QDialog
{
    Q_OBJECT


    enum DataPlotIndex_ET
    {
        PLOT_INDEX_PRICE,
        PLOT_INDEX_SHORT_MOVING_AVG,
        PLOT_INDEX_MID_MOVING_AVG,
        PLOT_INDEX_LONG_MOVING_AVG,
        PLOT_INDEX_BOLLINGERBAND_LOW,
        PLOT_INDEX_BOLLINGERBAND_MID,
        PLOT_INDEX_BOLLINGERBAND_UPPER,
        PLOT_INDEX_LINJER_REG,
        PLOT_INDEX_GREEN_MOUSE_LINE,
        PLOT_INDEX_RED_MOUSE_LINE,
        PLOT_INDEX_MAX
    };


    enum SubPlotIndex_ET
    {
        SUBPLOT_INDEX_MOMENTUM_ZERO_LINE,
        SUBPLOT_INDEX_MOMENTUM,
        SUBPLOT_STOCHASTIC_LIMIT_80,
        SUBPLOT_STOCHASTIC_LIMIT_20,
        SUBPLOT_STOCHASTIC_K,
        SUBPLOT_STOCHASTIC_D,
        SUBPLOT_RSI_LIMIT_70 = SUBPLOT_STOCHASTIC_LIMIT_80,
        SUBPLOT_RSI_LIMIT_30 = SUBPLOT_STOCHASTIC_LIMIT_20,
        SUBPLOT_RSI,
        SUBPLOT_RATE_OF_CHANGE,
        SUBPLOT_RATE_OF_CHANGE_LIMIT_100 = SUBPLOT_STOCHASTIC_LIMIT_80,
        SUBPLOT_MACD_LINE,
        SUBPLOT_MACD_SIGNAL,
        SUBPLOT_MACD_HISTOGRAM,
        SUBPLOT_INDEX_MAX
    };


    CYahooStockPlotUtil m_plot;

    // Cannot extract x,y data. Contains all data that is send to graph
    CYahooStockPlotUtil::PlotData_ST m_qwtStockPlotData;

    CYahooStockPlotUtil::PlotData_ST m_qwtsubPlotData;

    QwtPlotHistogram *m_macdHist;
    QVector <QwtIntervalSample> m_macdHistData;


    // Is able to store a number of stocks and all its data. Data can be retrived.
    QVector <CYahooStockPlotUtil::StockData_ST> m_stockArr;

    // Is able to store one stock with data
    CYahooStockPlotUtil::StockData_ST m_stockData;



public:

    enum TimePeriodDays_ET
    {
        TIME_PERIOD_DAYS_ALL_DATA,
        TIME_PERIOD_DAYS_10_YEAR,
        TIME_PERIOD_DAYS_7_YEAR,
        TIME_PERIOD_DAYS_5_YEAR,
        TIME_PERIOD_DAYS_3_YEAR,
        TIME_PERIOD_DAYS_2_YEAR,
        TIME_PERIOD_DAYS_1_YEAR,
        TIME_PERIOD_DAYS_6_MONTH,
        TIME_PERIOD_DAYS_3_MONTH,
        TIME_PERIOD_DAYS_1_MONTH,
        TIME_PERIOD_DAYS_2_WEEK,
        TIME_PERIOD_DAYS_1_WEEK,
        MAX_NOF_TIME_PERIOD_DAYS_ITEMS
    };


    struct TimePeriodDays_ST
    {
        QString TxtTimePeriod;
        TimePeriodDays_ET timePeriod;
    };

    enum FAData_ET
    {
        FA_PE,
        FA_PS,
        FA_ERNING_DIV_DIVIDEN, // Visnt / utdelning
        FA_NAV_DIV_LAST_PRICE, // Substans / Kurs
        FA_YIELD,
        FA_MEAN_RETURN,
        FA_MEAN_RISK,
        FA_NOF_DATA
    };


    QPalette *m_faDataPalette[FA_NOF_DATA];

    void initFa3MinMaxPePrice(void);
    void initFa3ProfitabilityAnalysis(void);
    void initFa2OperatingIncomeList(void);
    void initFa2netProfitAfterTaxList(void);
    static const TimePeriodDays_ST m_timePeriodDaysArr[MAX_NOF_TIME_PERIOD_DAYS_ITEMS];

    void setFaEditControlTxtColor(QLineEdit *lineEdit, QPalette *palette, const QColor &color);
    void setFundametalAnalysisCtrlTxtColor(CDbHndl::snapshotStockData_ST keyData,
                                           CDbHndl::EfficPortStockData_ST stockRiskReturnData,
                                           QString assetType);


    void initMinMaxAxis(CYahooStockPlotUtil::MinMaxAxisValues_ST inputAxis,
                        CYahooStockPlotUtil::MinMaxAxisValues_ST &outputAxis);


    explicit TaAnalysis(QWidget *parent = 0);
    ~TaAnalysis();

signals:
    void emitReqSingleStockDataFromServer();

public slots:
    // Thread is finish parsing data
    void slotImportSingelStockDataIsFinish(int);
    // http Thread is finish download file from server



private slots:
    void slotReceivedAssetTaDataFromServer(int);

    void slotReqSingleStockDataTimerExpired();

    void slotReqSingleStockDataFromServer();

    void on_SelStockListButton_clicked();

    void on_treeWidget_doubleClicked(const QModelIndex &index);

    void on_selTimePeriodSlider_valueChanged(int value);

    void on_toggleZoomButton_clicked();

    void on_moveTimePeriodSlider_2_valueChanged(int value);


    void on_ZoomOutButton_clicked();

    void on_togglePanButton_clicked();

private:

    enum StockListColName_ET
    {
        STOCKLIST_NAME = 0,
        STOCKLIST_SYMBOL = 1
    };

    enum StatusReqSingleStockData_ET
    {
        STATUS_REQ_SINGLE_STOCK_IDLE,
        STATUS_REQ_SINGLE_STOCK_PENDING,
        STATUS_REQ_SINGLE_STOCK_PROCESSING
    };


    importYahooTaData *m_importYahooTaDataThread;
    Ui::TaAnalysis *ui;
    HttpWindow m_hw1;
    CDbHndl m_db;
    int m_timeSlideWindowInc;
    int m_timePeriodDaysInc;
    QString m_startDate;
    QString m_endDate;
    QString m_endDateRef;

    QString m_reqStartDate;
    QString m_reqEndDate;
    QString m_reqStockSymbol;
    QString m_reqStockName;
    StatusReqSingleStockData_ET m_singleStockDataReqStatus;
    QList <NameKey> m_dateIsUpdatedList;


    void initStockList(void);
    bool openStockNameFile(QString filename, QString stockListName);
    void getSelStockListItem(QString &stockName, QString &stockSymbol, const QModelIndex &index);
    void initTimePeriodCtrls(int value);
    void setTimePeriodDaysUpdateStartStopDate(QString &startDate, QString &endDate, int value);
    void settimeSlideWindowInc(int value);
    void setTimeSlideWindowUpdateStartStopDate(QString &startDate, QString &endDate, int value);
    void prepReqTaDataFromServer(QString stockname, QString stockSymbol, QString startDate, QString endDate);
    void startReqSingleStockDataTimeoutTimer(int ms);
    void displayStockData(bool addLastPrice = false, double lastPrice = 0);
    void addStockDataToPlot(DataPlotIndex_ET plotIndex,
                            QString legendLable);
    void addStockIndicatorToPlot(int indicatorIndex,
                                 DataPlotIndex_ET plotIndex,
                                 QString legendLable,
                                 CYahooStockPlotUtil::MinMaxAxisValues_ST &axis,
                                 QwtPlot *qwtPlot,
                                 CYahooStockPlotUtil::PlotData_ST &qwtPlotData);
    void deleteAllPlotData(CYahooStockPlotUtil::PlotData_ST &allPlotData, QwtPlot *qwtPlot);
    void setAxisTitles(QString titleXAxis, QString titleYAxis, QwtPlot *qwtPlot,bool showXAxis=true);
    void initMinMaxScale(CYahooStockPlotUtil::MinMaxAxisValues_ST inputAxis, CYahooStockPlotUtil::MinMaxAxisValues_ST &outputAxis);
    void updateMinMaxXAxisScales(CYahooStockPlotUtil::MinMaxAxisValues_ST &axis, CYahooStockPlotUtil::MinMaxAxisValues_ST &qwtPlotAxis);
    void clearStockAndIndicatorTempMem(void);
    void modifyDateList(QString stockSymbol, bool isUpdate);
    void findDateList(QString stockSymbol, bool &isUpdate);
    void addStatusDateList(QString stockSymbol, bool isUpdate);


};

#endif
