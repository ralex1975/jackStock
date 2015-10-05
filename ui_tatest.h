/********************************************************************************
** Form generated from reading UI file 'tatest.ui'
**
** Created: Fri Feb 28 16:27:47 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TATEST_H
#define UI_TATEST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QTabWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QWidget>
#include "qwt_plot.h"

QT_BEGIN_NAMESPACE

class Ui_TaTest
{
public:
    QPushButton *UpdateButton;
    QPushButton *InsertDataButton;
    QPushButton *OpenFileButton;
    QTabWidget *tabWidget;
    QWidget *tab;
    QTreeWidget *treeWidget;
    QComboBox *StockListComboBox;
    QPushButton *SelStockListButton;
    QWidget *tab_2;
    QPushButton *AddToTreeButton;
    QwtPlot *qwtPlot;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QProgressBar *progressBar;
    QPushButton *toggleZoomButton_3;
    QPushButton *ZoomOutButton_3;
    QPushButton *PanningOnButton_3;
    QPushButton *panLeftButton_3;
    QSlider *horizontalSlider;
    QPushButton *panRightButton_3;
    QPushButton *parseDataButton_3;
    QLabel *label;
    QLineEdit *lineEditAssetName;
    QLabel *label_2;
    QLineEdit *lineEditStartDate;
    QLineEdit *lineEditStartXAxis;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLineEdit *lineEditStopDate;
    QLineEdit *lineEditStopXaxis;
    QPushButton *pushButtonImportAllData;
    QLineEdit *lineEditImportStartDate;
    QLineEdit *lineEditImportEndDate;
    QLabel *label_6;
    QLabel *label_7;
    QGroupBox *Import;
    QPushButton *pushButtonTestSendRecvServerData;
    QLineEdit *lineEditShowPlotStartDate;
    QLineEdit *lineEditShowPlotEndDate;
    QLabel *label_8;
    QLabel *label_9;
    QCheckBox *checkBoxBollingBand;
    QPushButton *pushButtonS;
    QCheckBox *checkBoxShortMvAvg;
    QCheckBox *checkBoxMidMvAvg;
    QCheckBox *checkBoxLongMvAvg;
    QPushButton *pushButtoGetLatetestDate;

    void setupUi(QDialog *TaTest)
    {
        if (TaTest->objectName().isEmpty())
            TaTest->setObjectName(QString::fromUtf8("TaTest"));
        TaTest->resize(1296, 622);
        UpdateButton = new QPushButton(TaTest);
        UpdateButton->setObjectName(QString::fromUtf8("UpdateButton"));
        UpdateButton->setGeometry(QRect(10, 10, 131, 27));
        InsertDataButton = new QPushButton(TaTest);
        InsertDataButton->setObjectName(QString::fromUtf8("InsertDataButton"));
        InsertDataButton->setGeometry(QRect(160, 10, 98, 27));
        OpenFileButton = new QPushButton(TaTest);
        OpenFileButton->setObjectName(QString::fromUtf8("OpenFileButton"));
        OpenFileButton->setGeometry(QRect(270, 10, 98, 27));
        tabWidget = new QTabWidget(TaTest);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(20, 60, 301, 561));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        treeWidget = new QTreeWidget(tab);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        treeWidget->setGeometry(QRect(10, 50, 271, 461));
        StockListComboBox = new QComboBox(tab);
        StockListComboBox->setObjectName(QString::fromUtf8("StockListComboBox"));
        StockListComboBox->setGeometry(QRect(10, 10, 191, 27));
        SelStockListButton = new QPushButton(tab);
        SelStockListButton->setObjectName(QString::fromUtf8("SelStockListButton"));
        SelStockListButton->setGeometry(QRect(210, 10, 71, 27));
        tabWidget->addTab(tab, QString());
        StockListComboBox->raise();
        treeWidget->raise();
        SelStockListButton->raise();
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tabWidget->addTab(tab_2, QString());
        AddToTreeButton = new QPushButton(TaTest);
        AddToTreeButton->setObjectName(QString::fromUtf8("AddToTreeButton"));
        AddToTreeButton->setGeometry(QRect(380, 10, 98, 27));
        qwtPlot = new QwtPlot(TaTest);
        qwtPlot->setObjectName(QString::fromUtf8("qwtPlot"));
        qwtPlot->setGeometry(QRect(330, 130, 691, 471));
        pushButton = new QPushButton(TaTest);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(490, 10, 99, 27));
        pushButton_2 = new QPushButton(TaTest);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(600, 10, 99, 27));
        progressBar = new QProgressBar(TaTest);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(1040, 550, 211, 23));
        progressBar->setValue(24);
        toggleZoomButton_3 = new QPushButton(TaTest);
        toggleZoomButton_3->setObjectName(QString::fromUtf8("toggleZoomButton_3"));
        toggleZoomButton_3->setGeometry(QRect(710, 10, 99, 27));
        ZoomOutButton_3 = new QPushButton(TaTest);
        ZoomOutButton_3->setObjectName(QString::fromUtf8("ZoomOutButton_3"));
        ZoomOutButton_3->setGeometry(QRect(820, 10, 99, 27));
        PanningOnButton_3 = new QPushButton(TaTest);
        PanningOnButton_3->setObjectName(QString::fromUtf8("PanningOnButton_3"));
        PanningOnButton_3->setGeometry(QRect(930, 10, 99, 27));
        panLeftButton_3 = new QPushButton(TaTest);
        panLeftButton_3->setObjectName(QString::fromUtf8("panLeftButton_3"));
        panLeftButton_3->setGeometry(QRect(1040, 10, 99, 27));
        horizontalSlider = new QSlider(TaTest);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(1050, 490, 160, 29));
        horizontalSlider->setOrientation(Qt::Horizontal);
        panRightButton_3 = new QPushButton(TaTest);
        panRightButton_3->setObjectName(QString::fromUtf8("panRightButton_3"));
        panRightButton_3->setGeometry(QRect(1150, 10, 99, 27));
        parseDataButton_3 = new QPushButton(TaTest);
        parseDataButton_3->setObjectName(QString::fromUtf8("parseDataButton_3"));
        parseDataButton_3->setGeometry(QRect(160, 40, 99, 27));
        label = new QLabel(TaTest);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(1040, 200, 67, 17));
        lineEditAssetName = new QLineEdit(TaTest);
        lineEditAssetName->setObjectName(QString::fromUtf8("lineEditAssetName"));
        lineEditAssetName->setGeometry(QRect(1040, 220, 231, 27));
        label_2 = new QLabel(TaTest);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(1040, 260, 111, 17));
        lineEditStartDate = new QLineEdit(TaTest);
        lineEditStartDate->setObjectName(QString::fromUtf8("lineEditStartDate"));
        lineEditStartDate->setGeometry(QRect(1040, 280, 113, 27));
        lineEditStartXAxis = new QLineEdit(TaTest);
        lineEditStartXAxis->setObjectName(QString::fromUtf8("lineEditStartXAxis"));
        lineEditStartXAxis->setGeometry(QRect(1170, 280, 113, 27));
        label_3 = new QLabel(TaTest);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(1170, 260, 111, 17));
        label_4 = new QLabel(TaTest);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(1170, 320, 111, 17));
        label_5 = new QLabel(TaTest);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(1040, 320, 111, 17));
        lineEditStopDate = new QLineEdit(TaTest);
        lineEditStopDate->setObjectName(QString::fromUtf8("lineEditStopDate"));
        lineEditStopDate->setGeometry(QRect(1040, 340, 113, 27));
        lineEditStopXaxis = new QLineEdit(TaTest);
        lineEditStopXaxis->setObjectName(QString::fromUtf8("lineEditStopXaxis"));
        lineEditStopXaxis->setGeometry(QRect(1170, 340, 113, 27));
        pushButtonImportAllData = new QPushButton(TaTest);
        pushButtonImportAllData->setObjectName(QString::fromUtf8("pushButtonImportAllData"));
        pushButtonImportAllData->setGeometry(QRect(1050, 80, 111, 27));
        lineEditImportStartDate = new QLineEdit(TaTest);
        lineEditImportStartDate->setObjectName(QString::fromUtf8("lineEditImportStartDate"));
        lineEditImportStartDate->setGeometry(QRect(1050, 130, 113, 27));
        lineEditImportEndDate = new QLineEdit(TaTest);
        lineEditImportEndDate->setObjectName(QString::fromUtf8("lineEditImportEndDate"));
        lineEditImportEndDate->setGeometry(QRect(1170, 130, 113, 27));
        label_6 = new QLabel(TaTest);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(1050, 110, 67, 17));
        label_7 = new QLabel(TaTest);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(1170, 110, 67, 17));
        Import = new QGroupBox(TaTest);
        Import->setObjectName(QString::fromUtf8("Import"));
        Import->setGeometry(QRect(1030, 50, 251, 121));
        pushButtonTestSendRecvServerData = new QPushButton(TaTest);
        pushButtonTestSendRecvServerData->setObjectName(QString::fromUtf8("pushButtonTestSendRecvServerData"));
        pushButtonTestSendRecvServerData->setGeometry(QRect(1050, 430, 181, 27));
        lineEditShowPlotStartDate = new QLineEdit(TaTest);
        lineEditShowPlotStartDate->setObjectName(QString::fromUtf8("lineEditShowPlotStartDate"));
        lineEditShowPlotStartDate->setGeometry(QRect(350, 60, 113, 27));
        lineEditShowPlotEndDate = new QLineEdit(TaTest);
        lineEditShowPlotEndDate->setObjectName(QString::fromUtf8("lineEditShowPlotEndDate"));
        lineEditShowPlotEndDate->setGeometry(QRect(480, 60, 113, 27));
        label_8 = new QLabel(TaTest);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(350, 40, 67, 17));
        label_9 = new QLabel(TaTest);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(480, 40, 67, 17));
        checkBoxBollingBand = new QCheckBox(TaTest);
        checkBoxBollingBand->setObjectName(QString::fromUtf8("checkBoxBollingBand"));
        checkBoxBollingBand->setGeometry(QRect(720, 60, 111, 22));
        pushButtonS = new QPushButton(TaTest);
        pushButtonS->setObjectName(QString::fromUtf8("pushButtonS"));
        pushButtonS->setGeometry(QRect(600, 60, 99, 27));
        checkBoxShortMvAvg = new QCheckBox(TaTest);
        checkBoxShortMvAvg->setObjectName(QString::fromUtf8("checkBoxShortMvAvg"));
        checkBoxShortMvAvg->setGeometry(QRect(850, 60, 151, 22));
        checkBoxMidMvAvg = new QCheckBox(TaTest);
        checkBoxMidMvAvg->setObjectName(QString::fromUtf8("checkBoxMidMvAvg"));
        checkBoxMidMvAvg->setGeometry(QRect(850, 80, 151, 22));
        checkBoxLongMvAvg = new QCheckBox(TaTest);
        checkBoxLongMvAvg->setObjectName(QString::fromUtf8("checkBoxLongMvAvg"));
        checkBoxLongMvAvg->setGeometry(QRect(850, 100, 151, 22));
        pushButtoGetLatetestDate = new QPushButton(TaTest);
        pushButtoGetLatetestDate->setObjectName(QString::fromUtf8("pushButtoGetLatetestDate"));
        pushButtoGetLatetestDate->setGeometry(QRect(1070, 390, 141, 27));
        Import->raise();
        UpdateButton->raise();
        InsertDataButton->raise();
        OpenFileButton->raise();
        tabWidget->raise();
        AddToTreeButton->raise();
        qwtPlot->raise();
        pushButton->raise();
        pushButton_2->raise();
        progressBar->raise();
        toggleZoomButton_3->raise();
        ZoomOutButton_3->raise();
        PanningOnButton_3->raise();
        panLeftButton_3->raise();
        horizontalSlider->raise();
        panRightButton_3->raise();
        parseDataButton_3->raise();
        label->raise();
        lineEditAssetName->raise();
        label_2->raise();
        lineEditStartDate->raise();
        lineEditStartXAxis->raise();
        label_3->raise();
        label_4->raise();
        label_5->raise();
        lineEditStopDate->raise();
        lineEditStopXaxis->raise();
        pushButtonImportAllData->raise();
        lineEditImportStartDate->raise();
        lineEditImportEndDate->raise();
        label_6->raise();
        label_7->raise();
        pushButtonTestSendRecvServerData->raise();
        lineEditShowPlotStartDate->raise();
        lineEditShowPlotEndDate->raise();
        label_8->raise();
        label_9->raise();
        checkBoxBollingBand->raise();
        pushButtonS->raise();
        checkBoxShortMvAvg->raise();
        checkBoxMidMvAvg->raise();
        checkBoxLongMvAvg->raise();
        pushButtoGetLatetestDate->raise();

        retranslateUi(TaTest);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(TaTest);
    } // setupUi

    void retranslateUi(QDialog *TaTest)
    {
        TaTest->setWindowTitle(QApplication::translate("TaTest", "Dialog", 0, QApplication::UnicodeUTF8));
        UpdateButton->setText(QApplication::translate("TaTest", "Update", 0, QApplication::UnicodeUTF8));
        InsertDataButton->setText(QApplication::translate("TaTest", "ReadData", 0, QApplication::UnicodeUTF8));
        OpenFileButton->setText(QApplication::translate("TaTest", "OpenFile", 0, QApplication::UnicodeUTF8));
        SelStockListButton->setText(QApplication::translate("TaTest", "Select", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("TaTest", "Tab 1", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("TaTest", "Tab 2", 0, QApplication::UnicodeUTF8));
        AddToTreeButton->setText(QApplication::translate("TaTest", "Update Tree", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("TaTest", "Remove Plot", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("TaTest", "Add Plot", 0, QApplication::UnicodeUTF8));
        toggleZoomButton_3->setText(QApplication::translate("TaTest", "Zoom", 0, QApplication::UnicodeUTF8));
        ZoomOutButton_3->setText(QApplication::translate("TaTest", "Zoom out", 0, QApplication::UnicodeUTF8));
        PanningOnButton_3->setText(QApplication::translate("TaTest", "Panning", 0, QApplication::UnicodeUTF8));
        panLeftButton_3->setText(QApplication::translate("TaTest", "<-", 0, QApplication::UnicodeUTF8));
        panRightButton_3->setText(QApplication::translate("TaTest", "->", 0, QApplication::UnicodeUTF8));
        parseDataButton_3->setText(QApplication::translate("TaTest", "Parse Data", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("TaTest", "Namn", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("TaTest", "Start datum", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("TaTest", "Start x axeln", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("TaTest", "Stop x axeln", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("TaTest", "Stop datum", 0, QApplication::UnicodeUTF8));
        pushButtonImportAllData->setText(QApplication::translate("TaTest", "Import all data", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("TaTest", "Start date", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("TaTest", "Stop date", 0, QApplication::UnicodeUTF8));
        Import->setTitle(QApplication::translate("TaTest", "Import data", 0, QApplication::UnicodeUTF8));
        pushButtonTestSendRecvServerData->setText(QApplication::translate("TaTest", "Request server data", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("TaTest", "Start date", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("TaTest", "Stop date", 0, QApplication::UnicodeUTF8));
        checkBoxBollingBand->setText(QApplication::translate("TaTest", "Bollingband", 0, QApplication::UnicodeUTF8));
        pushButtonS->setText(QApplication::translate("TaTest", "Scale", 0, QApplication::UnicodeUTF8));
        checkBoxShortMvAvg->setText(QApplication::translate("TaTest", "Short Moving Avg ", 0, QApplication::UnicodeUTF8));
        checkBoxMidMvAvg->setText(QApplication::translate("TaTest", "Mid Moving Avg", 0, QApplication::UnicodeUTF8));
        checkBoxLongMvAvg->setText(QApplication::translate("TaTest", "Long Moving Avg", 0, QApplication::UnicodeUTF8));
        pushButtoGetLatetestDate->setText(QApplication::translate("TaTest", "Get latest date", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class TaTest: public Ui_TaTest {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TATEST_H
