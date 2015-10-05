/********************************************************************************
** Form generated from reading UI file 'graphtable2tab.ui'
**
** Created: Fri Feb 28 23:02:19 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRAPHTABLE2TAB_H
#define UI_GRAPHTABLE2TAB_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QListView>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QTableView>
#include <QtGui/QWidget>
#include "qwt_plot.h"

QT_BEGIN_NAMESPACE

class Ui_graphTable2Tab
{
public:
    QTableView *tableView;
    QTabWidget *tabWidget;
    QWidget *tab;
    QComboBox *compareParam3ComboBox;
    QLineEdit *indicatorValue4LineEdit;
    QLineEdit *indicatorValue7LineEdit;
    QComboBox *compareParam4ComboBox;
    QComboBox *indicator4ComboBox;
    QLineEdit *indicatorValue3LineEdit;
    QComboBox *compareParam2ComboBox;
    QComboBox *compareParam1ComboBox;
    QComboBox *SelDataCountryComboBox;
    QComboBox *compareParam6ComboBox;
    QComboBox *indicator7ComboBox;
    QComboBox *indicator2ComboBox;
    QLineEdit *indicatorValue5LineEdit;
    QComboBox *indicator6ComboBox;
    QLineEdit *indicatorValue2LineEdit;
    QLineEdit *indicatorValue6LineEdit;
    QComboBox *compareParam5ComboBox;
    QComboBox *indicator1ComboBox;
    QPushButton *FilterButton;
    QComboBox *compareParam7ComboBox;
    QLineEdit *indicatorValue1LineEdit;
    QPushButton *getWebDataButton;
    QPushButton *ShowLatestButton;
    QComboBox *indicator3ComboBox;
    QComboBox *indicator5ComboBox;
    QCheckBox *largeCapCheckBox;
    QCheckBox *midCapCheckBox;
    QCheckBox *smallCapCheckBox;
    QWidget *tab_2;
    QListView *listView;
    QWidget *tab_3;
    QwtPlot *qwtPlot;

    void setupUi(QDialog *graphTable2Tab)
    {
        if (graphTable2Tab->objectName().isEmpty())
            graphTable2Tab->setObjectName(QString::fromUtf8("graphTable2Tab"));
        graphTable2Tab->resize(1296, 622);
        tableView = new QTableView(graphTable2Tab);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setGeometry(QRect(290, 320, 961, 277));
        tabWidget = new QTabWidget(graphTable2Tab);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(0, 0, 271, 601));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        compareParam3ComboBox = new QComboBox(tab);
        compareParam3ComboBox->setObjectName(QString::fromUtf8("compareParam3ComboBox"));
        compareParam3ComboBox->setGeometry(QRect(20, 290, 69, 22));
        indicatorValue4LineEdit = new QLineEdit(tab);
        indicatorValue4LineEdit->setObjectName(QString::fromUtf8("indicatorValue4LineEdit"));
        indicatorValue4LineEdit->setGeometry(QRect(100, 350, 61, 21));
        indicatorValue7LineEdit = new QLineEdit(tab);
        indicatorValue7LineEdit->setObjectName(QString::fromUtf8("indicatorValue7LineEdit"));
        indicatorValue7LineEdit->setGeometry(QRect(100, 530, 61, 21));
        compareParam4ComboBox = new QComboBox(tab);
        compareParam4ComboBox->setObjectName(QString::fromUtf8("compareParam4ComboBox"));
        compareParam4ComboBox->setGeometry(QRect(20, 350, 69, 22));
        indicator4ComboBox = new QComboBox(tab);
        indicator4ComboBox->setObjectName(QString::fromUtf8("indicator4ComboBox"));
        indicator4ComboBox->setGeometry(QRect(20, 320, 141, 22));
        indicatorValue3LineEdit = new QLineEdit(tab);
        indicatorValue3LineEdit->setObjectName(QString::fromUtf8("indicatorValue3LineEdit"));
        indicatorValue3LineEdit->setGeometry(QRect(100, 290, 61, 21));
        compareParam2ComboBox = new QComboBox(tab);
        compareParam2ComboBox->setObjectName(QString::fromUtf8("compareParam2ComboBox"));
        compareParam2ComboBox->setGeometry(QRect(20, 230, 69, 22));
        compareParam1ComboBox = new QComboBox(tab);
        compareParam1ComboBox->setObjectName(QString::fromUtf8("compareParam1ComboBox"));
        compareParam1ComboBox->setGeometry(QRect(20, 170, 69, 22));
        SelDataCountryComboBox = new QComboBox(tab);
        SelDataCountryComboBox->setObjectName(QString::fromUtf8("SelDataCountryComboBox"));
        SelDataCountryComboBox->setGeometry(QRect(20, 80, 181, 22));
        compareParam6ComboBox = new QComboBox(tab);
        compareParam6ComboBox->setObjectName(QString::fromUtf8("compareParam6ComboBox"));
        compareParam6ComboBox->setGeometry(QRect(20, 470, 69, 22));
        indicator7ComboBox = new QComboBox(tab);
        indicator7ComboBox->setObjectName(QString::fromUtf8("indicator7ComboBox"));
        indicator7ComboBox->setGeometry(QRect(20, 500, 141, 22));
        indicator2ComboBox = new QComboBox(tab);
        indicator2ComboBox->setObjectName(QString::fromUtf8("indicator2ComboBox"));
        indicator2ComboBox->setGeometry(QRect(20, 200, 141, 22));
        indicatorValue5LineEdit = new QLineEdit(tab);
        indicatorValue5LineEdit->setObjectName(QString::fromUtf8("indicatorValue5LineEdit"));
        indicatorValue5LineEdit->setGeometry(QRect(100, 410, 61, 21));
        indicator6ComboBox = new QComboBox(tab);
        indicator6ComboBox->setObjectName(QString::fromUtf8("indicator6ComboBox"));
        indicator6ComboBox->setGeometry(QRect(20, 440, 141, 22));
        indicatorValue2LineEdit = new QLineEdit(tab);
        indicatorValue2LineEdit->setObjectName(QString::fromUtf8("indicatorValue2LineEdit"));
        indicatorValue2LineEdit->setGeometry(QRect(90, 230, 61, 21));
        indicatorValue6LineEdit = new QLineEdit(tab);
        indicatorValue6LineEdit->setObjectName(QString::fromUtf8("indicatorValue6LineEdit"));
        indicatorValue6LineEdit->setGeometry(QRect(100, 470, 61, 21));
        compareParam5ComboBox = new QComboBox(tab);
        compareParam5ComboBox->setObjectName(QString::fromUtf8("compareParam5ComboBox"));
        compareParam5ComboBox->setGeometry(QRect(20, 410, 69, 22));
        indicator1ComboBox = new QComboBox(tab);
        indicator1ComboBox->setObjectName(QString::fromUtf8("indicator1ComboBox"));
        indicator1ComboBox->setGeometry(QRect(20, 140, 141, 22));
        FilterButton = new QPushButton(tab);
        FilterButton->setObjectName(QString::fromUtf8("FilterButton"));
        FilterButton->setGeometry(QRect(20, 110, 81, 23));
        compareParam7ComboBox = new QComboBox(tab);
        compareParam7ComboBox->setObjectName(QString::fromUtf8("compareParam7ComboBox"));
        compareParam7ComboBox->setGeometry(QRect(20, 530, 69, 22));
        indicatorValue1LineEdit = new QLineEdit(tab);
        indicatorValue1LineEdit->setObjectName(QString::fromUtf8("indicatorValue1LineEdit"));
        indicatorValue1LineEdit->setGeometry(QRect(100, 170, 61, 21));
        getWebDataButton = new QPushButton(tab);
        getWebDataButton->setObjectName(QString::fromUtf8("getWebDataButton"));
        getWebDataButton->setGeometry(QRect(10, 40, 81, 23));
        ShowLatestButton = new QPushButton(tab);
        ShowLatestButton->setObjectName(QString::fromUtf8("ShowLatestButton"));
        ShowLatestButton->setGeometry(QRect(110, 110, 81, 23));
        indicator3ComboBox = new QComboBox(tab);
        indicator3ComboBox->setObjectName(QString::fromUtf8("indicator3ComboBox"));
        indicator3ComboBox->setGeometry(QRect(20, 260, 141, 22));
        indicator5ComboBox = new QComboBox(tab);
        indicator5ComboBox->setObjectName(QString::fromUtf8("indicator5ComboBox"));
        indicator5ComboBox->setGeometry(QRect(20, 380, 141, 22));
        largeCapCheckBox = new QCheckBox(tab);
        largeCapCheckBox->setObjectName(QString::fromUtf8("largeCapCheckBox"));
        largeCapCheckBox->setGeometry(QRect(10, 10, 101, 17));
        midCapCheckBox = new QCheckBox(tab);
        midCapCheckBox->setObjectName(QString::fromUtf8("midCapCheckBox"));
        midCapCheckBox->setGeometry(QRect(110, 10, 91, 17));
        smallCapCheckBox = new QCheckBox(tab);
        smallCapCheckBox->setObjectName(QString::fromUtf8("smallCapCheckBox"));
        smallCapCheckBox->setEnabled(false);
        smallCapCheckBox->setGeometry(QRect(110, 40, 101, 20));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        listView = new QListView(tab_2);
        listView->setObjectName(QString::fromUtf8("listView"));
        listView->setGeometry(QRect(10, 10, 241, 531));
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        tabWidget->addTab(tab_3, QString());
        qwtPlot = new QwtPlot(graphTable2Tab);
        qwtPlot->setObjectName(QString::fromUtf8("qwtPlot"));
        qwtPlot->setGeometry(QRect(290, 20, 961, 288));

        retranslateUi(graphTable2Tab);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(graphTable2Tab);
    } // setupUi

    void retranslateUi(QDialog *graphTable2Tab)
    {
        graphTable2Tab->setWindowTitle(QApplication::translate("graphTable2Tab", "Dialog", 0, QApplication::UnicodeUTF8));
        FilterButton->setText(QApplication::translate("graphTable2Tab", "Filtrera", 0, QApplication::UnicodeUTF8));
        getWebDataButton->setText(QApplication::translate("graphTable2Tab", "H\303\244mta", 0, QApplication::UnicodeUTF8));
        ShowLatestButton->setText(QApplication::translate("graphTable2Tab", "Visa", 0, QApplication::UnicodeUTF8));
        largeCapCheckBox->setText(QApplication::translate("graphTable2Tab", "Large Cap", 0, QApplication::UnicodeUTF8));
        midCapCheckBox->setText(QApplication::translate("graphTable2Tab", "Mid Cap", 0, QApplication::UnicodeUTF8));
        smallCapCheckBox->setText(QApplication::translate("graphTable2Tab", "Small Cap", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("graphTable2Tab", "Tab 1", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("graphTable2Tab", "Tab 2", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("graphTable2Tab", "Page 3", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class graphTable2Tab: public Ui_graphTable2Tab {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRAPHTABLE2TAB_H
