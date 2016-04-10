#ifndef ADMINMYPORTFOLIO_H
#define ADMINMYPORTFOLIO_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStringList>
#include <QtCore>
#include <QtGui>
#include <QObject>
#include "extendedTable.h"
#include "yahooStockPlotUtil.h"
#include <qwt_plot_histogram.h>
#include "extendedQwtPlot.h"


namespace Ui {
class AdminMyPortfolio;
}

class AdminMyPortfolio : public QDialog
{
    Q_OBJECT
    CExtendedTable m_ynokd;

public:
    explicit AdminMyPortfolio(QWidget *parent = 0);
    ~AdminMyPortfolio();

private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_addDataPushButton_clicked();
    void on_removeDataPushButton_clicked();

private:
    Ui::AdminMyPortfolio *ui;
    bool m_okToRemoveData;
    QString m_yearMonth;
    QString m_year;
    QString m_month;
    double m_x[1000];
    double m_y[1000];
    double m_z[1000];
    double m_n[1000];
    double m_valueIncreaseArr[1000];

    CExtendedQwtPlot m_eqp;

    CExtendedQwtPlot m_eqp1;

    // Is able to store a number of plot data. Data can be retrived.
    QVector <CYahooStockPlotUtil::StockData_ST> m_plotDataArr;

    // Cannot extract x,y data. Contains all data that is send to graph
    CYahooStockPlotUtil::PlotData_ST m_qwtPlotData;

    QwtPlotHistogram *m_barHist;
    QVector <QwtIntervalSample> m_barHistData;

    void addTableHead(void);
    void plotPortfolioMarketValue(CExtendedTable &table, int maxRow);

};

#endif // ADMINMYPORTFOLIO_H
