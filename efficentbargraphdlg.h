#ifndef EFFICENTBARGRAPHDLG_H
#define EFFICENTBARGRAPHDLG_H

#include <QDialog>
#include <qwt_plot_histogram.h>
#include "extendedTable.h"

namespace Ui {
class efficentBarGraphDlg;
}

class efficentBarGraphDlg : public QDialog
{
    Q_OBJECT

     CYahooStockPlotUtil m_plot;
    
public:
    explicit efficentBarGraphDlg(QWidget *parent = 0);
    ~efficentBarGraphDlg();

    CExtendedTable m_extTable;
    QwtPlotHistogram *m_efficientFrontHist;
    QVector <QwtIntervalSample> m_efficientFrontHistData;
    void plot();
    void initCorrelationTable(QTreeWidget *treeWidget);
    void mainTraversTreeWidget(QTreeWidget *treeWidget);

    
private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::efficentBarGraphDlg *ui;
};

#endif // EFFICENTBARGRAPHDLG_H
