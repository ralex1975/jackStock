/********************************************************************************
** Form generated from reading UI file 'fatab.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FATAB_H
#define UI_FATAB_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QWidget>
#include "qwt_plot.h"

QT_BEGIN_NAMESPACE

class Ui_FaTab
{
public:
    QPushButton *SelStockListButton;
    QComboBox *StockListComboBox;
    QTreeWidget *treeWidgetStockList;
    QTabWidget *tabWidget;
    QWidget *tab;
    QTreeWidget *treeWidgetSalesGrowth;
    QwtPlot *qwtPlotSalesGrowth;
    QPushButton *pushButNext1;
    QLabel *label;
    QLabel *assetType;
    QLabel *labelOneDayProcentChange;
    QLabel *StatusInfoLabel;
    QLabel *label_25;
    QLineEdit *lineEditMoreInfoSector_2;
    QLabel *labelLastPrice;
    QLineEdit *lineEditAssetName;
    QLineEdit *lineEditAssetName_2;
    QLabel *label_7;
    QLabel *label_23;
    QLineEdit *lineEditMoreInfoBransch_2;
    QCheckBox *checkBoxShowForecast;
    QLabel *labelReportAvg;
    QLabel *labelCalcTrendline;
    QWidget *tab_2;
    QwtPlot *qwtPlotProfitAnalysis;
    QPushButton *pushButNextTab_2;
    QTreeWidget *treeWidgetProfitGrowth;
    QLabel *label_2;
    QCheckBox *checkBoxShowForecast_2;
    QLabel *labelCalcTrendline_2;
    QLabel *labelReportAvg_2;
    QWidget *tab_3;
    QwtPlot *qwtPlotProfitMargin;
    QwtPlot *qwtPlotReturnOnEquity;
    QwtPlot *qwtPlotSalesAndProfitGrowth;
    QTreeWidget *treeWidgetProfit;
    QLabel *label_3;
    QLabel *labelSalesPg3;
    QLabel *labelProfitPg3;
    QLabel *labelProfitMarginPg3;
    QLabel *labelReturnOnEquityPg3;
    QPushButton *pushButNextTabPg3;
    QWidget *tab_4;
    QwtPlot *qwtPlotPePrice;
    QPushButton *pushButton_3;
    QTreeWidget *treeWidgetHistoricalPENum;
    QLabel *label_4;
    QTreeWidget *treeWidgetHistoricalPrices;
    QLabel *label_16;
    QWidget *tab_5;
    QPushButton *pushButton_4;
    QwtPlot *qwtPlot_7;
    QLabel *label_5;
    QWidget *tab_6;
    QPushButton *pushButton_5;
    QwtPlot *qwtPlot_8;
    QLabel *label_6;
    QWidget *tab_7;
    QLineEdit *lineEditEarningsDivByDividend_3;
    QLabel *label_31;
    QLabel *label_29;
    QLabel *label_26;
    QLineEdit *lineEditPE_3;
    QLineEdit *lineEditYield_3;
    QLabel *label_28;
    QLabel *label_30;
    QLineEdit *lineEditNavDivLastStockPrice_3;
    QLineEdit *lineEditPs_3;
    QLabel *assetType_2;
    QLabel *labelLastPrice_3;
    QLabel *labelOneDayProcentChange_3;

    void setupUi(QDialog *FaTab)
    {
        if (FaTab->objectName().isEmpty())
            FaTab->setObjectName(QString::fromUtf8("FaTab"));
        FaTab->resize(1336, 603);
        SelStockListButton = new QPushButton(FaTab);
        SelStockListButton->setObjectName(QString::fromUtf8("SelStockListButton"));
        SelStockListButton->setGeometry(QRect(200, 20, 51, 27));
        QFont font;
        font.setPointSize(9);
        SelStockListButton->setFont(font);
        StockListComboBox = new QComboBox(FaTab);
        StockListComboBox->setObjectName(QString::fromUtf8("StockListComboBox"));
        StockListComboBox->setGeometry(QRect(10, 20, 191, 27));
        StockListComboBox->setFont(font);
        treeWidgetStockList = new QTreeWidget(FaTab);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidgetStockList->setHeaderItem(__qtreewidgetitem);
        treeWidgetStockList->setObjectName(QString::fromUtf8("treeWidgetStockList"));
        treeWidgetStockList->setGeometry(QRect(10, 50, 241, 441));
        QFont font1;
        font1.setPointSize(9);
        font1.setBold(false);
        font1.setWeight(50);
        treeWidgetStockList->setFont(font1);
        treeWidgetStockList->header()->setDefaultSectionSize(90);
        tabWidget = new QTabWidget(FaTab);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(260, 10, 1061, 581));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        treeWidgetSalesGrowth = new QTreeWidget(tab);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem();
        __qtreewidgetitem1->setText(0, QString::fromUtf8("1"));
        treeWidgetSalesGrowth->setHeaderItem(__qtreewidgetitem1);
        treeWidgetSalesGrowth->setObjectName(QString::fromUtf8("treeWidgetSalesGrowth"));
        treeWidgetSalesGrowth->setGeometry(QRect(10, 30, 341, 211));
        treeWidgetSalesGrowth->setFont(font);
        qwtPlotSalesGrowth = new QwtPlot(tab);
        qwtPlotSalesGrowth->setObjectName(QString::fromUtf8("qwtPlotSalesGrowth"));
        qwtPlotSalesGrowth->setGeometry(QRect(380, 50, 671, 481));
        pushButNext1 = new QPushButton(tab);
        pushButNext1->setObjectName(QString::fromUtf8("pushButNext1"));
        pushButNext1->setGeometry(QRect(280, 510, 98, 27));
        label = new QLabel(tab);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 221, 17));
        assetType = new QLabel(tab);
        assetType->setObjectName(QString::fromUtf8("assetType"));
        assetType->setGeometry(QRect(1080, 380, 171, 17));
        assetType->setFont(font);
        labelOneDayProcentChange = new QLabel(tab);
        labelOneDayProcentChange->setObjectName(QString::fromUtf8("labelOneDayProcentChange"));
        labelOneDayProcentChange->setGeometry(QRect(1260, 380, 71, 17));
        labelOneDayProcentChange->setFont(font);
        StatusInfoLabel = new QLabel(tab);
        StatusInfoLabel->setObjectName(QString::fromUtf8("StatusInfoLabel"));
        StatusInfoLabel->setGeometry(QRect(1080, 400, 171, 21));
        StatusInfoLabel->setFont(font);
        label_25 = new QLabel(tab);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        label_25->setGeometry(QRect(640, 0, 41, 17));
        label_25->setFont(font);
        lineEditMoreInfoSector_2 = new QLineEdit(tab);
        lineEditMoreInfoSector_2->setObjectName(QString::fromUtf8("lineEditMoreInfoSector_2"));
        lineEditMoreInfoSector_2->setGeometry(QRect(640, 20, 211, 21));
        lineEditMoreInfoSector_2->setFont(font);
        labelLastPrice = new QLabel(tab);
        labelLastPrice->setObjectName(QString::fromUtf8("labelLastPrice"));
        labelLastPrice->setGeometry(QRect(1260, 400, 61, 17));
        labelLastPrice->setFont(font);
        lineEditAssetName = new QLineEdit(tab);
        lineEditAssetName->setObjectName(QString::fromUtf8("lineEditAssetName"));
        lineEditAssetName->setGeometry(QRect(1080, 350, 201, 21));
        QFont font2;
        font2.setPointSize(11);
        lineEditAssetName->setFont(font2);
        lineEditAssetName_2 = new QLineEdit(tab);
        lineEditAssetName_2->setObjectName(QString::fromUtf8("lineEditAssetName_2"));
        lineEditAssetName_2->setGeometry(QRect(430, 20, 201, 21));
        lineEditAssetName_2->setFont(font2);
        label_7 = new QLabel(tab);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(430, 0, 151, 17));
        label_7->setFont(font);
        label_23 = new QLabel(tab);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setGeometry(QRect(860, 0, 51, 17));
        label_23->setFont(font);
        lineEditMoreInfoBransch_2 = new QLineEdit(tab);
        lineEditMoreInfoBransch_2->setObjectName(QString::fromUtf8("lineEditMoreInfoBransch_2"));
        lineEditMoreInfoBransch_2->setGeometry(QRect(860, 20, 191, 21));
        lineEditMoreInfoBransch_2->setFont(font);
        checkBoxShowForecast = new QCheckBox(tab);
        checkBoxShowForecast->setObjectName(QString::fromUtf8("checkBoxShowForecast"));
        checkBoxShowForecast->setGeometry(QRect(250, 10, 131, 22));
        checkBoxShowForecast->setFont(font);
        checkBoxShowForecast->setChecked(true);
        labelReportAvg = new QLabel(tab);
        labelReportAvg->setObjectName(QString::fromUtf8("labelReportAvg"));
        labelReportAvg->setGeometry(QRect(10, 250, 361, 17));
        labelReportAvg->setFont(font);
        labelCalcTrendline = new QLabel(tab);
        labelCalcTrendline->setObjectName(QString::fromUtf8("labelCalcTrendline"));
        labelCalcTrendline->setGeometry(QRect(10, 270, 361, 17));
        labelCalcTrendline->setFont(font);
        tabWidget->addTab(tab, QString());
        qwtPlotSalesGrowth->raise();
        treeWidgetSalesGrowth->raise();
        pushButNext1->raise();
        label->raise();
        assetType->raise();
        labelOneDayProcentChange->raise();
        StatusInfoLabel->raise();
        label_25->raise();
        lineEditMoreInfoSector_2->raise();
        labelLastPrice->raise();
        lineEditAssetName->raise();
        lineEditAssetName_2->raise();
        label_7->raise();
        label_23->raise();
        lineEditMoreInfoBransch_2->raise();
        checkBoxShowForecast->raise();
        labelReportAvg->raise();
        labelCalcTrendline->raise();
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        qwtPlotProfitAnalysis = new QwtPlot(tab_2);
        qwtPlotProfitAnalysis->setObjectName(QString::fromUtf8("qwtPlotProfitAnalysis"));
        qwtPlotProfitAnalysis->setGeometry(QRect(390, 0, 611, 521));
        pushButNextTab_2 = new QPushButton(tab_2);
        pushButNextTab_2->setObjectName(QString::fromUtf8("pushButNextTab_2"));
        pushButNextTab_2->setGeometry(QRect(290, 510, 98, 27));
        treeWidgetProfitGrowth = new QTreeWidget(tab_2);
        QTreeWidgetItem *__qtreewidgetitem2 = new QTreeWidgetItem();
        __qtreewidgetitem2->setText(0, QString::fromUtf8("1"));
        treeWidgetProfitGrowth->setHeaderItem(__qtreewidgetitem2);
        treeWidgetProfitGrowth->setObjectName(QString::fromUtf8("treeWidgetProfitGrowth"));
        treeWidgetProfitGrowth->setGeometry(QRect(10, 40, 351, 201));
        label_2 = new QLabel(tab_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 10, 181, 17));
        checkBoxShowForecast_2 = new QCheckBox(tab_2);
        checkBoxShowForecast_2->setObjectName(QString::fromUtf8("checkBoxShowForecast_2"));
        checkBoxShowForecast_2->setGeometry(QRect(250, 10, 131, 22));
        checkBoxShowForecast_2->setFont(font);
        checkBoxShowForecast_2->setChecked(true);
        labelCalcTrendline_2 = new QLabel(tab_2);
        labelCalcTrendline_2->setObjectName(QString::fromUtf8("labelCalcTrendline_2"));
        labelCalcTrendline_2->setGeometry(QRect(10, 270, 361, 17));
        labelCalcTrendline_2->setFont(font);
        labelReportAvg_2 = new QLabel(tab_2);
        labelReportAvg_2->setObjectName(QString::fromUtf8("labelReportAvg_2"));
        labelReportAvg_2->setGeometry(QRect(10, 250, 361, 17));
        labelReportAvg_2->setFont(font);
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        qwtPlotProfitMargin = new QwtPlot(tab_3);
        qwtPlotProfitMargin->setObjectName(QString::fromUtf8("qwtPlotProfitMargin"));
        qwtPlotProfitMargin->setGeometry(QRect(390, 250, 651, 141));
        qwtPlotReturnOnEquity = new QwtPlot(tab_3);
        qwtPlotReturnOnEquity->setObjectName(QString::fromUtf8("qwtPlotReturnOnEquity"));
        qwtPlotReturnOnEquity->setGeometry(QRect(390, 400, 651, 131));
        qwtPlotSalesAndProfitGrowth = new QwtPlot(tab_3);
        qwtPlotSalesAndProfitGrowth->setObjectName(QString::fromUtf8("qwtPlotSalesAndProfitGrowth"));
        qwtPlotSalesAndProfitGrowth->setGeometry(QRect(390, 20, 651, 221));
        treeWidgetProfit = new QTreeWidget(tab_3);
        QTreeWidgetItem *__qtreewidgetitem3 = new QTreeWidgetItem();
        __qtreewidgetitem3->setText(0, QString::fromUtf8("1"));
        treeWidgetProfit->setHeaderItem(__qtreewidgetitem3);
        treeWidgetProfit->setObjectName(QString::fromUtf8("treeWidgetProfit"));
        treeWidgetProfit->setGeometry(QRect(10, 30, 371, 141));
        treeWidgetProfit->setFont(font);
        label_3 = new QLabel(tab_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 10, 181, 17));
        labelSalesPg3 = new QLabel(tab_3);
        labelSalesPg3->setObjectName(QString::fromUtf8("labelSalesPg3"));
        labelSalesPg3->setGeometry(QRect(510, 0, 61, 17));
        labelSalesPg3->setFont(font);
        labelProfitPg3 = new QLabel(tab_3);
        labelProfitPg3->setObjectName(QString::fromUtf8("labelProfitPg3"));
        labelProfitPg3->setGeometry(QRect(440, 0, 61, 17));
        labelProfitPg3->setFont(font);
        labelProfitMarginPg3 = new QLabel(tab_3);
        labelProfitMarginPg3->setObjectName(QString::fromUtf8("labelProfitMarginPg3"));
        labelProfitMarginPg3->setGeometry(QRect(550, 0, 81, 20));
        labelProfitMarginPg3->setFont(font);
        labelReturnOnEquityPg3 = new QLabel(tab_3);
        labelReturnOnEquityPg3->setObjectName(QString::fromUtf8("labelReturnOnEquityPg3"));
        labelReturnOnEquityPg3->setGeometry(QRect(640, 0, 81, 20));
        labelReturnOnEquityPg3->setFont(font);
        pushButNextTabPg3 = new QPushButton(tab_3);
        pushButNextTabPg3->setObjectName(QString::fromUtf8("pushButNextTabPg3"));
        pushButNextTabPg3->setGeometry(QRect(280, 490, 98, 27));
        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        qwtPlotPePrice = new QwtPlot(tab_4);
        qwtPlotPePrice->setObjectName(QString::fromUtf8("qwtPlotPePrice"));
        qwtPlotPePrice->setGeometry(QRect(390, 10, 611, 511));
        pushButton_3 = new QPushButton(tab_4);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(290, 510, 98, 27));
        treeWidgetHistoricalPENum = new QTreeWidget(tab_4);
        QTreeWidgetItem *__qtreewidgetitem4 = new QTreeWidgetItem();
        __qtreewidgetitem4->setText(0, QString::fromUtf8("1"));
        treeWidgetHistoricalPENum->setHeaderItem(__qtreewidgetitem4);
        treeWidgetHistoricalPENum->setObjectName(QString::fromUtf8("treeWidgetHistoricalPENum"));
        treeWidgetHistoricalPENum->setGeometry(QRect(10, 30, 271, 121));
        treeWidgetHistoricalPENum->setFont(font);
        label_4 = new QLabel(tab_4);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 10, 181, 17));
        QFont font3;
        font3.setPointSize(9);
        font3.setBold(false);
        font3.setItalic(false);
        font3.setWeight(50);
        label_4->setFont(font3);
        treeWidgetHistoricalPrices = new QTreeWidget(tab_4);
        QTreeWidgetItem *__qtreewidgetitem5 = new QTreeWidgetItem();
        __qtreewidgetitem5->setText(0, QString::fromUtf8("1"));
        treeWidgetHistoricalPrices->setHeaderItem(__qtreewidgetitem5);
        treeWidgetHistoricalPrices->setObjectName(QString::fromUtf8("treeWidgetHistoricalPrices"));
        treeWidgetHistoricalPrices->setGeometry(QRect(10, 170, 271, 131));
        treeWidgetHistoricalPrices->setFont(font3);
        label_16 = new QLabel(tab_4);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(10, 150, 91, 17));
        label_16->setFont(font);
        tabWidget->addTab(tab_4, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QString::fromUtf8("tab_5"));
        pushButton_4 = new QPushButton(tab_5);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(300, 500, 98, 27));
        qwtPlot_7 = new QwtPlot(tab_5);
        qwtPlot_7->setObjectName(QString::fromUtf8("qwtPlot_7"));
        qwtPlot_7->setGeometry(QRect(390, 20, 611, 511));
        label_5 = new QLabel(tab_5);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 20, 231, 17));
        tabWidget->addTab(tab_5, QString());
        tab_6 = new QWidget();
        tab_6->setObjectName(QString::fromUtf8("tab_6"));
        pushButton_5 = new QPushButton(tab_6);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setGeometry(QRect(290, 500, 98, 27));
        qwtPlot_8 = new QwtPlot(tab_6);
        qwtPlot_8->setObjectName(QString::fromUtf8("qwtPlot_8"));
        qwtPlot_8->setGeometry(QRect(380, 20, 611, 511));
        label_6 = new QLabel(tab_6);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(10, 10, 291, 17));
        tabWidget->addTab(tab_6, QString());
        tab_7 = new QWidget();
        tab_7->setObjectName(QString::fromUtf8("tab_7"));
        tabWidget->addTab(tab_7, QString());
        lineEditEarningsDivByDividend_3 = new QLineEdit(FaTab);
        lineEditEarningsDivByDividend_3->setObjectName(QString::fromUtf8("lineEditEarningsDivByDividend_3"));
        lineEditEarningsDivByDividend_3->setGeometry(QRect(200, 520, 51, 21));
        lineEditEarningsDivByDividend_3->setFont(font);
        label_31 = new QLabel(FaTab);
        label_31->setObjectName(QString::fromUtf8("label_31"));
        label_31->setGeometry(QRect(110, 500, 51, 17));
        label_31->setFont(font);
        label_29 = new QLabel(FaTab);
        label_29->setObjectName(QString::fromUtf8("label_29"));
        label_29->setGeometry(QRect(170, 500, 16, 17));
        label_29->setFont(font);
        label_26 = new QLabel(FaTab);
        label_26->setObjectName(QString::fromUtf8("label_26"));
        label_26->setGeometry(QRect(20, 500, 21, 20));
        label_26->setFont(font);
        lineEditPE_3 = new QLineEdit(FaTab);
        lineEditPE_3->setObjectName(QString::fromUtf8("lineEditPE_3"));
        lineEditPE_3->setGeometry(QRect(10, 520, 51, 21));
        lineEditPE_3->setFont(font);
        lineEditYield_3 = new QLineEdit(FaTab);
        lineEditYield_3->setObjectName(QString::fromUtf8("lineEditYield_3"));
        lineEditYield_3->setGeometry(QRect(60, 520, 51, 21));
        lineEditYield_3->setFont(font);
        label_28 = new QLabel(FaTab);
        label_28->setObjectName(QString::fromUtf8("label_28"));
        label_28->setGeometry(QRect(60, 500, 41, 17));
        label_28->setFont(font);
        label_30 = new QLabel(FaTab);
        label_30->setObjectName(QString::fromUtf8("label_30"));
        label_30->setGeometry(QRect(190, 500, 71, 17));
        label_30->setFont(font);
        lineEditNavDivLastStockPrice_3 = new QLineEdit(FaTab);
        lineEditNavDivLastStockPrice_3->setObjectName(QString::fromUtf8("lineEditNavDivLastStockPrice_3"));
        lineEditNavDivLastStockPrice_3->setGeometry(QRect(110, 520, 51, 21));
        lineEditNavDivLastStockPrice_3->setFont(font);
        lineEditPs_3 = new QLineEdit(FaTab);
        lineEditPs_3->setObjectName(QString::fromUtf8("lineEditPs_3"));
        lineEditPs_3->setGeometry(QRect(160, 520, 41, 21));
        lineEditPs_3->setFont(font);
        assetType_2 = new QLabel(FaTab);
        assetType_2->setObjectName(QString::fromUtf8("assetType_2"));
        assetType_2->setGeometry(QRect(10, 550, 161, 20));
        assetType_2->setFont(font);
        labelLastPrice_3 = new QLabel(FaTab);
        labelLastPrice_3->setObjectName(QString::fromUtf8("labelLastPrice_3"));
        labelLastPrice_3->setGeometry(QRect(180, 570, 71, 17));
        labelLastPrice_3->setFont(font);
        labelOneDayProcentChange_3 = new QLabel(FaTab);
        labelOneDayProcentChange_3->setObjectName(QString::fromUtf8("labelOneDayProcentChange_3"));
        labelOneDayProcentChange_3->setGeometry(QRect(180, 550, 71, 17));
        labelOneDayProcentChange_3->setFont(font);

        retranslateUi(FaTab);

        tabWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(FaTab);
    } // setupUi

    void retranslateUi(QDialog *FaTab)
    {
        FaTab->setWindowTitle(QApplication::translate("FaTab", "Dialog", 0, QApplication::UnicodeUTF8));
        SelStockListButton->setText(QApplication::translate("FaTab", "V\303\244lj", 0, QApplication::UnicodeUTF8));
        pushButNext1->setText(QApplication::translate("FaTab", "N\303\244sta >", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("FaTab", "Historisk f\303\266rs\303\244ljningsutveckling", 0, QApplication::UnicodeUTF8));
        assetType->setText(QString());
        labelOneDayProcentChange->setText(QString());
        StatusInfoLabel->setText(QString());
        label_25->setText(QApplication::translate("FaTab", "Sektor", 0, QApplication::UnicodeUTF8));
        labelLastPrice->setText(QString());
        label_7->setText(QApplication::translate("FaTab", "F\303\266retag", 0, QApplication::UnicodeUTF8));
        label_23->setText(QApplication::translate("FaTab", "Bransch", 0, QApplication::UnicodeUTF8));
        checkBoxShowForecast->setText(QApplication::translate("FaTab", "Visa prognosdata", 0, QApplication::UnicodeUTF8));
        labelReportAvg->setText(QString());
        labelCalcTrendline->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("FaTab", "1 ->", 0, QApplication::UnicodeUTF8));
        pushButNextTab_2->setText(QApplication::translate("FaTab", "N\303\244sta >", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("FaTab", "Historisk vinstutveckling", 0, QApplication::UnicodeUTF8));
        checkBoxShowForecast_2->setText(QApplication::translate("FaTab", "Visa prognosdata", 0, QApplication::UnicodeUTF8));
        labelCalcTrendline_2->setText(QString());
        labelReportAvg_2->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("FaTab", "2 ->", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("FaTab", "L\303\266nsamhetsanalys", 0, QApplication::UnicodeUTF8));
        labelSalesPg3->setText(QString());
        labelProfitPg3->setText(QString());
        labelProfitMarginPg3->setText(QString());
        labelReturnOnEquityPg3->setText(QString());
        pushButNextTabPg3->setText(QApplication::translate("FaTab", "N\303\244sta >", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("FaTab", "3 ->", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("FaTab", "PushButton", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("FaTab", "P/E-talsprognos", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("FaTab", "Historiska priser", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("FaTab", "4 ->", 0, QApplication::UnicodeUTF8));
        pushButton_4->setText(QApplication::translate("FaTab", "PushButton", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("FaTab", "Kursprognoser", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_5), QApplication::translate("FaTab", "5 ->", 0, QApplication::UnicodeUTF8));
        pushButton_5->setText(QApplication::translate("FaTab", "PushButton", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("FaTab", "Kursintervall, riskbed\303\266mning, avkastning", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_6), QApplication::translate("FaTab", "6 ->", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_7), QApplication::translate("FaTab", "7   ", 0, QApplication::UnicodeUTF8));
        label_31->setText(QApplication::translate("FaTab", "Sub/Kurs", 0, QApplication::UnicodeUTF8));
        label_29->setText(QApplication::translate("FaTab", "PS", 0, QApplication::UnicodeUTF8));
        label_26->setText(QApplication::translate("FaTab", "PE", 0, QApplication::UnicodeUTF8));
        label_28->setText(QApplication::translate("FaTab", "DirAvk", 0, QApplication::UnicodeUTF8));
        label_30->setText(QApplication::translate("FaTab", "Vinst/Utdel", 0, QApplication::UnicodeUTF8));
        assetType_2->setText(QString());
        labelLastPrice_3->setText(QString());
        labelOneDayProcentChange_3->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class FaTab: public Ui_FaTab {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FATAB_H
