#ifndef LEASTSQUARESTADLG_H
#define LEASTSQUARESTADLG_H

#include <QDialog>
#include <qmutex.h>
#include "QVector"
#include <QList>
#include "stockPlotUtil.h"
#include "importYahooTaData.h"

#include "yahooStockPlotUtil.h"
#include "canvaspicker.h"
#include <qwt_plot_histogram.h>
#include "common.h"
#include "dbHndl.h"
#include "inc/guiUtil/guiFinanceCtrls.h"


namespace Ui {
class LeastSquaresTaDlg;
}

class LeastSquaresTaDlg : public QDialog
{
    Q_OBJECT

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

    enum StatusReqSingleStockData_ET
    {
        STATUS_REQ_SINGLE_STOCK_IDLE,
        STATUS_REQ_SINGLE_STOCK_PENDING,
        STATUS_REQ_SINGLE_STOCK_PROCESSING
    };

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

    struct TimePeriodDays_ST
    {
        QString TxtTimePeriod;
        TimePeriodDays_ET timePeriod;
    };


    GuiFinanceCtrls m_faCtrl;



     HttpWindow m_hw1;
     MyLineEdit m_mle;


     QPalette *m_faDataPalette[FA_NOF_DATA];
    QMutex m_mutex;
    QString m_startDate;
    QString m_endDate;
    QString m_endDateRef;

    int m_timeSlideWindowInc;
    int m_timePeriodDaysInc;
    static const TimePeriodDays_ST m_timePeriodDaysArr[MAX_NOF_TIME_PERIOD_DAYS_ITEMS];

    QString m_reqStartDate;
    QString m_reqEndDate;

    QString m_reqStockSymbol;
    QString m_reqStockName;

    StatusReqSingleStockData_ET m_singleStockDataReqStatus;
    importYahooTaData *m_importYahooTaDataThread;

    QList <NameKey> m_dateIsUpdatedList;


    CYahooStockPlotUtil m_plot;

    // Cannot extract x,y data. Contains all data that is send to graph
    CYahooStockPlotUtil::PlotData_ST m_qwtStockPlotData;

    CYahooStockPlotUtil::PlotData_ST m_qwtsubPlotData;

    QwtPlotHistogram *m_macdHist;
    QVector <QwtIntervalSample> m_macdHistData;

    // Is able to store a number of stocks and all its data. Data can be retrived.
    QVector <CYahooStockPlotUtil::StockData_ST> m_yStockArr;

    // Is able to store one stock with data
    CYahooStockPlotUtil::StockData_ST m_stockData;


    QVector <CStockPlotUtil::StockData_ST> m_stockArr;

    void prepReqTaDataFromServer(QString stockName, QString stockSymbol, QString startDate, QString endDate);
    void displayStockData(bool addLastPrice = false, double lastPrice = 0);
    void deleteAllPlotData(CYahooStockPlotUtil::PlotData_ST &allPlotData, QwtPlot *qwtPlot);
    void clearStockAndIndicatorTempMem(void);
    void initMinMaxAxis(CYahooStockPlotUtil::MinMaxAxisValues_ST inputAxis, CYahooStockPlotUtil::MinMaxAxisValues_ST &outputAxis);
    void setAxisTitles(QString titleXAxis, QString titleYAxis, QwtPlot *qwtPlot,bool showXAxis=true);
    void addStockDataToPlot(DataPlotIndex_ET plotIndex, QString legendLable);
    void modifyDateList(QString stockSymbol, bool isUpdate);
    void addStatusDateList(QString stockSymbol, bool isUpdate);
    void getSelStockListItem(QString &stockName, QString &stockSymbol, const QModelIndex &index);
    void findDateList(QString stockSymbol, bool &isUpdate);
    void startReqSingleStockDataTimeoutTimer(int ms);
    void initTimePeriodCtrls(int value);
    void settimeSlideWindowInc(int value);
    void setTimePeriodDaysUpdateStartStopDate(QString &startDate, QString &endDate, int value);

    void plotStockData(QString stockName, QString StockSymbol);
    void updateMinMaxXAxisScales(CYahooStockPlotUtil::MinMaxAxisValues_ST &axis, CYahooStockPlotUtil::MinMaxAxisValues_ST &qwtPlotAxis);
    void addStockIndicatorToPlot(int indicatorIndex,
                                 DataPlotIndex_ET plotIndex,
                                 QString legendLable,
                                 CYahooStockPlotUtil::MinMaxAxisValues_ST &axis,
                                 QwtPlot *qwtPlot,
                                 CYahooStockPlotUtil::PlotData_ST &qwtPlotData);
    void setFundametalAnalysisCtrlTxtColor(CDbHndl::snapshotStockData_ST keyData,
                                           CDbHndl::EfficPortStockData_ST stockRiskReturnData,
                                           QString assetType);
    void setFaEditControlTxtColor(QLineEdit *lineEdit, QPalette *palette, const QColor &color);
    void initFa3MinMaxPePrice(void);












public:
    explicit LeastSquaresTaDlg(QWidget *parent = 0);
    ~LeastSquaresTaDlg();

signals:
    void emitReqSingleStockDataFromServer();

public slots:
    void tablePlusHeaderClicked(int column);
    void tableMinusHeaderClicked(int column);

    // Thread is finish parsing data
    void slotImportSingelStockDataIsFinish(int);



private slots:
    void on_pushButton_clicked();
    void slotReqSingleStockDataFromServer();
    void slotReceivedAssetTaDataFromServer(int);
    void slotReqSingleStockDataTimerExpired();
    void on_tableViewLeastSquare_clicked(const QModelIndex &index);
    void on_selTimePeriodSliderLSqrt_valueChanged(int value);

    void on_tableViewLeastSquare_2_clicked(const QModelIndex &index);

private:
    Ui::LeastSquaresTaDlg *ui;
};

#endif // LEASTSQUARESTADLG_H
