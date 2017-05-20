#ifndef GRAPHTAB_H
#define GRAPHTAB_H

#include <QDialog>
#include <QVector>
#include "stockPlotUtil.h"



namespace Ui {
class GraphTab;
}




class GraphTab : public QDialog
{
    Q_OBJECT
    
public:
    CStockPlotUtil m_spu;
    explicit GraphTab(QWidget *parent = 0);
    ~GraphTab();
    
private slots:

    void on_SelectButton_clicked();

    void on_removePlotButton_clicked();

private:
    // Cannot extract x,y data. Contains all data that is send to graph
    CStockPlotUtil::PlotData_ST m_qwtStockPlotData;

    // Is able to store a nuber of stocks and all its data. Data can be retrived.
    QVector <CStockPlotUtil::StockData_ST> m_stockArr;

    // Is able to store one stock with data
    CStockPlotUtil::StockData_ST m_stockData;



    void copyListBoxDataToPlotList(CStockPlotUtil::PlotData_ST &allPlotData, QListWidget *listWidget);


    Ui::GraphTab *ui;

public:


     //QVector <StockData_ST> m_stockArr;

};

#endif // GRAPHTAB_H
