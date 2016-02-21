#ifndef GUIFINANCECTRLS_H
#define GUIFINANCECTRLS_H

#include <QWidget>
#include <QtGui>
#include <QTreeWidget>

#include "guiFinanceColor.h"
#include "myTreeWidget.h"



class GuiFinanceCtrls : public QWidget
{
    Q_OBJECT
public:
    explicit GuiFinanceCtrls(QWidget *parent = 0);

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
