#ifndef GUIFINANCECTRLS_H
#define GUIFINANCECTRLS_H

#include <QWidget>
#include <QtGui>
#include <QTreeWidget>

#include "guiFinanceColor.h"
#include "myTreeWidget.h"
#include "../../dbHndl.h"


class GuiFinanceCtrls : public QWidget
{
    Q_OBJECT
    int m_comboBoxIndex;
    CDbHndl m_db;

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

    enum StockListColName_ET
    {
        STOCKLIST_NAME = 0,
        STOCKLIST_SYMBOL = 1
    };

    struct TimePeriodDays_ST
    {
        QString TxtTimePeriod;
        TimePeriodDays_ET timePeriod;
    };


    int m_timeSlideWindowInc;
    int m_timePeriodDaysInc;
    static const TimePeriodDays_ST m_timePeriodDaysArr[MAX_NOF_TIME_PERIOD_DAYS_ITEMS];

    QString m_startDate;
    QString m_endDate;
    QString m_endDateRef;




public:
    explicit GuiFinanceCtrls(QWidget *parent = 0);

    void getSelStockListItem(QTreeWidget *treeWidgetStockList,
                             QString &stockName,
                             QString &stockSymbol,
                             const QModelIndex &index);


    void initStockList1(QTreeWidget *treeWidgetStockList, bool hideSymbol=true);


    void initTimePeriodCtrls(int value,
                             QLineEdit *timePeriodLineEdit,
                             QLineEdit *setEndDateLineEdit,
                             QLineEdit *setStartDateLineEdit,
                             QSlider *selTimePeriodSlider);

    void setTimePeriodDaysUpdateStartStopDate(QString &startDate,
                                         QString &endDate,
                                         int value,
                                         QLineEdit *setEndDateLineEdit,
                                         QLineEdit *setStartDateLineEdit);


    void addAllStockListsToCombobox(QComboBox *comboBox);
    bool getStockListNameAndId(QComboBox *comboBox, QString &stockListName, int &stockListId);


    bool calcRiskAndReturn(QString startDate,
                           QString endDate,
                           CDbHndl::EfficPortStockData_ST &data);



    //===============================================================================================
    // LineEdits
    //===============================================================================================
    void fa5AddData(QString inEarningsDivDividend,  QLineEdit *earningsDivDividend,
                    QString inBavDivStockPrice,     QLineEdit *navDivStockPrice,
                    QString inPe,                   QLineEdit *pe,
                    QString inPs,                   QLineEdit *ps,
                    QString inYield,                QLineEdit *yield,
                    QString assetType = "");



    //===============================================================================================
    // TreeWidgets
    //===============================================================================================
    void initHistoricalPe(QTreeWidget *treeWidget,
                          QString header0 = QString::fromUtf8("År"),           int headerWidth0 = 65,
                          QString header1 = QString::fromUtf8("Min\nP/E"),     int headerWidth1 = 65,
                          QString header2 = QString::fromUtf8("Medel\nP/E"),   int headerWidth2 = 60,
                          QString header3 = QString::fromUtf8("Max\nP/E"),     int headerWidth3 = 50);



    void initHistoricalStockPrice(QTreeWidget *treeWidget,
                                  QString header0 = QString::fromUtf8("År"),            int headerWidth0 = 65,
                                  QString header1 = QString::fromUtf8("Min\npris"),     int headerWidth1 = 65,
                                  QString header2 = QString::fromUtf8("Medel\npris"),   int headerWidth2 = 60,
                                  QString header3 = QString::fromUtf8("Max\npris"),     int headerWidth3 = 50);



    void initSalesGrowth(QTreeWidget *treeWidget,
                         QString header0 = QString::fromUtf8("År"),             int headerWidth0 = 65,
                         QString header1 = QString::fromUtf8("Kr/\nAktie"),     int headerWidth1 = 65,
                         QString header2 = QString::fromUtf8("MKr"),            int headerWidth2 = 60,
                         QString header3 = QString::fromUtf8("Tillväxt"),       int headerWidth3 = 50);


    void initProfitGrowth(QTreeWidget *treeWidget,
                         QString header0 = QString::fromUtf8("År"),             int headerWidth0 = 65,
                         QString header1 = QString::fromUtf8("Kr/\nAktie"),     int headerWidth1 = 65,
                         QString header2 = QString::fromUtf8("MKr"),            int headerWidth2 = 60,
                         QString header3 = QString::fromUtf8("Tillväxt"),       int headerWidth3 = 50);


    void initProfitabilityAnalysis(QTreeWidget *treeWidget,
                      QString header0 = QString::fromUtf8("År"),                        int headerWidth0 = 65,
                      QString header1 = QString::fromUtf8("Vinstmarginal"),             int headerWidth1 = 85,
                      QString header2 = QString::fromUtf8("Ränta / \nEget kapital"),    int headerWidth2 = 65);


    void initStockList(QTreeWidget *treeWidget,
                       QString header0 = QString::fromUtf8("Namn"),   int headerWidth0 = 65, bool hideCol0 = false,
                       QString header1 = QString::fromUtf8("Symbol"), int headerWidth1 = 65, bool hideCol1 = true,
                       QString header2 = QString::fromUtf8("Notera"), int headerWidth2 = 65, bool hideCol2 = false);






signals:

public slots:

};

#endif // GUIFINANCECTRLS_H
