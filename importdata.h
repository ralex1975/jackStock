#ifndef IMPORTDATA_H
#define IMPORTDATA_H

#include <QDialog>

#include <QFile>
#include <QString>
#include <QDebug>
#include <QTextStream>
#include <QObject>

#include "common.h"
#include <QtCore>
#include <QtGui>
#include "httpwindow.h"
#include "common.h"
#include <QTimer>
#include <QList>
#include "dbHndl.h"


#include "importYahooTaData.h"
#include <QString>

#define YAHOO_KEY_STATISTICS_ARR_SIZE 58



namespace Ui {
class ImportData;
}

class ImportData : public QDialog
{
    Q_OBJECT

    enum StockListColName_ET
    {
        STOCKLIST_NAME = 0,
        STOCKLIST_SYMBOL = 1
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


    struct TimePeriodDays_ST
    {
        QString TxtTimePeriod;
        TimePeriodDays_ET timePeriod;
    };

    enum YahooKeyStatEt
    {
     YKS_STATE_READ_FILE,
     YKS_STATE_FIND_TOKEN,
     YKS_STATE_FIND_DATA_TOKEN,
    };


    YahooKeyStatEt m_yksState;


    bool readYahooKeyStatistics(QString filename);


    HttpWindow m_hw1;
    bool m_waitOnServerResp;
    QString m_startDate;
    QString m_endDate;
    QString m_endDateRef;
    int m_timePeriodDaysInc;





    CDbHndl m_db;
    CYahooStockPlotUtil::PlotData_ST m_qwtStockPlotData;
    importYahooTaData *m_importYahooTaDataThread;
    CYahooStockPlotUtil m_plot;
    QList <NameKey> m_dateIsUpdatedList;
    StatusReqSingleStockData_ET m_singleStockDataReqStatus;
    QList <CompanyNameAndId_ST> m_companyList;
    int m_companyListIndex;
    int m_oldCompanyListIndex;
    Ui::ImportData *ui;
    int m_timerId;
    QTimer *m_timeoutTimer;
    bool m_sendNextReq;
    bool m_displayFinish;

    QString m_reqStartDate;
    QString m_reqEndDate;
    QString m_reqStockSymbol;
    QString m_reqStockName;
    CYahooStockPlotUtil::StockData_ST m_stockData;
    QVector <CYahooStockPlotUtil::StockData_ST> m_stockArr;




    
public:

    static const QString yahooKeyStatisticsArr[YAHOO_KEY_STATISTICS_ARR_SIZE];

    static const TimePeriodDays_ST m_timePeriodDaysArr[MAX_NOF_TIME_PERIOD_DAYS_ITEMS];
    explicit ImportData(QWidget *parent = 0);
    ~ImportData();
    
private:
    void getSelStockListItem(QString &stockName, QString &stockSymbol, const QModelIndex &index);

    void initStockList(void);
    void openUserStockListsFile(void);
    void initTimePeriodCtrls(int value);
    bool openStockListFile(QString filename);
    void addDataToStockListAndStockListCombo(void);
    void setTimePeriodDaysUpdateStartStopDate(QString &startDate, QString &endDate, int value);
    bool openStockNameFile(QString filename, QString stockListName);
    void addStatusDateList(QString stockSymbol, bool isUpdate);
    void findDateList(QString stockSymbol, bool &isUpdate);
    void prepReqTaDataFromServer(QString stockName, QString stockSymbol, QString startDate, QString endDate);
    void displayStockData(void);
    void deleteAllPlotData(CYahooStockPlotUtil::PlotData_ST &allPlotData, QwtPlot *qwtPlot);
    void clearStockAndIndicatorTempMem(void);
    void initMinMaxAxis(CYahooStockPlotUtil::MinMaxAxisValues_ST inputAxis,
                        CYahooStockPlotUtil::MinMaxAxisValues_ST &outputAxis);
    void setAxisTitles(QString titleXAxis, QString titleYAxis, QwtPlot *qwtPlot,bool showXAxis=false);
    void addStockDataToPlot(DataPlotIndex_ET plotIndex, QString legendLable);
    void startReqSingleStockDataTimeoutTimer(int ms);
    void modifyDateList(QString stockSymbol, bool isUpdate);








public:
    void reqHtmlFile(QString path, QUrl url);
    bool readFile(QString filename);

private:
    void reqCompanyDescriptionHtmlFile(QString path, QUrl url);


    bool parseCompanyName(QTextStream &inStream, QString &result);

signals:
     void emitReqSingleStockDataFromServer();

public slots:

    void slotCompanyDescriptionHtmlPageIsRecv(int number);
    void slotHtmlPageIsRecv(int number);
    void slotImportSingelStockDataIsFinish(int);
   // void slotImportSingelStockDataIsFinish(int);
   // void slotReceivedAssetTaDataFromServer(int);


private slots:
    void on_pushButtonReqCompInfo_clicked();
    void slotReqSingleStockDataFromServer();
    void slotReqSingleStockDataTimerExpired();
    void slotReceivedAssetTaDataFromServer(int);


    void on_pushButtonParseCompanyIds_clicked();
    void slotReqNextCompanyDescriptionData();
    void slotReqNextCompanyData();
    void on_SelStockListButton_clicked();
    void on_pushButInit_clicked();

    void on_treeWidget_doubleClicked(const QModelIndex &index);
    void on_selTimePeriodSlider_valueChanged(int value);
    void on_pushButtImportKeyTaBridgeData_clicked();
    void on_pushButDescription_clicked();
    void on_pushButParseCompanyInfo_clicked();
    // void on_pushButtImportKeyTaBridgeData_toggled(bool checked);
    void on_pushButton_clicked();



};

#endif // IMPORTDATA_H
