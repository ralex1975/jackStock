/********************************************************************************
** Form generated from reading UI file 'graphTableTab.ui'
**
** Created: Fri Feb 28 23:38:46 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRAPHTABLETAB_H
#define UI_GRAPHTABLETAB_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSplitter>
#include <QtGui/QTableView>
#include "qwt_plot.h"

QT_BEGIN_NAMESPACE

class Ui_CGraphTableTab
{
public:
    QSplitter *splitter;
    QwtPlot *qwtPlot;
    QTableView *tableView;
    QGroupBox *groupBox;
    QPushButton *getWebDataButton;
    QPushButton *FilterButton;
    QPushButton *ShowLatestButton;
    QComboBox *SelDataCountryComboBox;
    QComboBox *compareParam6ComboBox;
    QLineEdit *indicatorValue5LineEdit;
    QLineEdit *indicatorValue7LineEdit;
    QComboBox *compareParam4ComboBox;
    QComboBox *indicator5ComboBox;
    QComboBox *compareParam2ComboBox;
    QComboBox *indicator1ComboBox;
    QLineEdit *indicatorValue1LineEdit;
    QComboBox *indicator3ComboBox;
    QComboBox *compareParam1ComboBox;
    QComboBox *compareParam5ComboBox;
    QComboBox *compareParam7ComboBox;
    QLineEdit *indicatorValue3LineEdit;
    QComboBox *indicator6ComboBox;
    QLineEdit *indicatorValue6LineEdit;
    QLineEdit *indicatorValue4LineEdit;
    QComboBox *indicator2ComboBox;
    QLineEdit *indicatorValue2LineEdit;
    QComboBox *compareParam3ComboBox;
    QComboBox *indicator4ComboBox;
    QComboBox *indicator7ComboBox;
    QCheckBox *largeCapCheckBox;
    QCheckBox *midCapCheckBox;
    QCheckBox *smallCapCheckBox;

    void setupUi(QDialog *CGraphTableTab)
    {
        if (CGraphTableTab->objectName().isEmpty())
            CGraphTableTab->setObjectName(QString::fromUtf8("CGraphTableTab"));
        CGraphTableTab->resize(1296, 622);
        splitter = new QSplitter(CGraphTableTab);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setGeometry(QRect(200, 40, 961, 571));
        splitter->setOrientation(Qt::Vertical);
        qwtPlot = new QwtPlot(splitter);
        qwtPlot->setObjectName(QString::fromUtf8("qwtPlot"));
        splitter->addWidget(qwtPlot);
        tableView = new QTableView(splitter);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        splitter->addWidget(tableView);
        groupBox = new QGroupBox(CGraphTableTab);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 201, 161));
        getWebDataButton = new QPushButton(groupBox);
        getWebDataButton->setObjectName(QString::fromUtf8("getWebDataButton"));
        getWebDataButton->setGeometry(QRect(10, 30, 81, 23));
        FilterButton = new QPushButton(groupBox);
        FilterButton->setObjectName(QString::fromUtf8("FilterButton"));
        FilterButton->setGeometry(QRect(10, 60, 81, 23));
        ShowLatestButton = new QPushButton(groupBox);
        ShowLatestButton->setObjectName(QString::fromUtf8("ShowLatestButton"));
        ShowLatestButton->setGeometry(QRect(10, 120, 81, 23));
        SelDataCountryComboBox = new QComboBox(groupBox);
        SelDataCountryComboBox->setObjectName(QString::fromUtf8("SelDataCountryComboBox"));
        SelDataCountryComboBox->setGeometry(QRect(10, 0, 181, 22));
        compareParam6ComboBox = new QComboBox(CGraphTableTab);
        compareParam6ComboBox->setObjectName(QString::fromUtf8("compareParam6ComboBox"));
        compareParam6ComboBox->setGeometry(QRect(20, 510, 69, 22));
        indicatorValue5LineEdit = new QLineEdit(CGraphTableTab);
        indicatorValue5LineEdit->setObjectName(QString::fromUtf8("indicatorValue5LineEdit"));
        indicatorValue5LineEdit->setGeometry(QRect(100, 450, 61, 21));
        indicatorValue7LineEdit = new QLineEdit(CGraphTableTab);
        indicatorValue7LineEdit->setObjectName(QString::fromUtf8("indicatorValue7LineEdit"));
        indicatorValue7LineEdit->setGeometry(QRect(100, 570, 61, 21));
        compareParam4ComboBox = new QComboBox(CGraphTableTab);
        compareParam4ComboBox->setObjectName(QString::fromUtf8("compareParam4ComboBox"));
        compareParam4ComboBox->setGeometry(QRect(20, 390, 69, 22));
        indicator5ComboBox = new QComboBox(CGraphTableTab);
        indicator5ComboBox->setObjectName(QString::fromUtf8("indicator5ComboBox"));
        indicator5ComboBox->setGeometry(QRect(20, 420, 141, 22));
        compareParam2ComboBox = new QComboBox(CGraphTableTab);
        compareParam2ComboBox->setObjectName(QString::fromUtf8("compareParam2ComboBox"));
        compareParam2ComboBox->setGeometry(QRect(20, 270, 69, 22));
        indicator1ComboBox = new QComboBox(CGraphTableTab);
        indicator1ComboBox->setObjectName(QString::fromUtf8("indicator1ComboBox"));
        indicator1ComboBox->setGeometry(QRect(20, 180, 141, 22));
        indicatorValue1LineEdit = new QLineEdit(CGraphTableTab);
        indicatorValue1LineEdit->setObjectName(QString::fromUtf8("indicatorValue1LineEdit"));
        indicatorValue1LineEdit->setGeometry(QRect(100, 210, 61, 21));
        indicator3ComboBox = new QComboBox(CGraphTableTab);
        indicator3ComboBox->setObjectName(QString::fromUtf8("indicator3ComboBox"));
        indicator3ComboBox->setGeometry(QRect(20, 300, 141, 22));
        compareParam1ComboBox = new QComboBox(CGraphTableTab);
        compareParam1ComboBox->setObjectName(QString::fromUtf8("compareParam1ComboBox"));
        compareParam1ComboBox->setGeometry(QRect(20, 210, 69, 22));
        compareParam5ComboBox = new QComboBox(CGraphTableTab);
        compareParam5ComboBox->setObjectName(QString::fromUtf8("compareParam5ComboBox"));
        compareParam5ComboBox->setGeometry(QRect(20, 450, 69, 22));
        compareParam7ComboBox = new QComboBox(CGraphTableTab);
        compareParam7ComboBox->setObjectName(QString::fromUtf8("compareParam7ComboBox"));
        compareParam7ComboBox->setGeometry(QRect(20, 570, 69, 22));
        indicatorValue3LineEdit = new QLineEdit(CGraphTableTab);
        indicatorValue3LineEdit->setObjectName(QString::fromUtf8("indicatorValue3LineEdit"));
        indicatorValue3LineEdit->setGeometry(QRect(100, 330, 61, 21));
        indicator6ComboBox = new QComboBox(CGraphTableTab);
        indicator6ComboBox->setObjectName(QString::fromUtf8("indicator6ComboBox"));
        indicator6ComboBox->setGeometry(QRect(20, 480, 141, 22));
        indicatorValue6LineEdit = new QLineEdit(CGraphTableTab);
        indicatorValue6LineEdit->setObjectName(QString::fromUtf8("indicatorValue6LineEdit"));
        indicatorValue6LineEdit->setGeometry(QRect(100, 510, 61, 21));
        indicatorValue4LineEdit = new QLineEdit(CGraphTableTab);
        indicatorValue4LineEdit->setObjectName(QString::fromUtf8("indicatorValue4LineEdit"));
        indicatorValue4LineEdit->setGeometry(QRect(100, 390, 61, 21));
        indicator2ComboBox = new QComboBox(CGraphTableTab);
        indicator2ComboBox->setObjectName(QString::fromUtf8("indicator2ComboBox"));
        indicator2ComboBox->setGeometry(QRect(20, 240, 141, 22));
        indicatorValue2LineEdit = new QLineEdit(CGraphTableTab);
        indicatorValue2LineEdit->setObjectName(QString::fromUtf8("indicatorValue2LineEdit"));
        indicatorValue2LineEdit->setGeometry(QRect(100, 270, 61, 21));
        compareParam3ComboBox = new QComboBox(CGraphTableTab);
        compareParam3ComboBox->setObjectName(QString::fromUtf8("compareParam3ComboBox"));
        compareParam3ComboBox->setGeometry(QRect(20, 330, 69, 22));
        indicator4ComboBox = new QComboBox(CGraphTableTab);
        indicator4ComboBox->setObjectName(QString::fromUtf8("indicator4ComboBox"));
        indicator4ComboBox->setGeometry(QRect(20, 360, 141, 22));
        indicator7ComboBox = new QComboBox(CGraphTableTab);
        indicator7ComboBox->setObjectName(QString::fromUtf8("indicator7ComboBox"));
        indicator7ComboBox->setGeometry(QRect(20, 540, 141, 22));
        largeCapCheckBox = new QCheckBox(CGraphTableTab);
        largeCapCheckBox->setObjectName(QString::fromUtf8("largeCapCheckBox"));
        largeCapCheckBox->setGeometry(QRect(220, 10, 171, 17));
        midCapCheckBox = new QCheckBox(CGraphTableTab);
        midCapCheckBox->setObjectName(QString::fromUtf8("midCapCheckBox"));
        midCapCheckBox->setGeometry(QRect(330, 10, 171, 17));
        smallCapCheckBox = new QCheckBox(CGraphTableTab);
        smallCapCheckBox->setObjectName(QString::fromUtf8("smallCapCheckBox"));
        smallCapCheckBox->setEnabled(false);
        smallCapCheckBox->setGeometry(QRect(430, 10, 181, 20));
        groupBox->raise();
        splitter->raise();
        compareParam6ComboBox->raise();
        indicatorValue5LineEdit->raise();
        indicatorValue7LineEdit->raise();
        compareParam4ComboBox->raise();
        indicator5ComboBox->raise();
        compareParam2ComboBox->raise();
        indicator1ComboBox->raise();
        indicatorValue1LineEdit->raise();
        indicator3ComboBox->raise();
        compareParam1ComboBox->raise();
        compareParam5ComboBox->raise();
        compareParam7ComboBox->raise();
        indicatorValue3LineEdit->raise();
        indicator6ComboBox->raise();
        indicatorValue6LineEdit->raise();
        indicatorValue4LineEdit->raise();
        indicator2ComboBox->raise();
        indicatorValue2LineEdit->raise();
        compareParam3ComboBox->raise();
        indicator4ComboBox->raise();
        indicator7ComboBox->raise();
        largeCapCheckBox->raise();
        midCapCheckBox->raise();
        smallCapCheckBox->raise();

        retranslateUi(CGraphTableTab);

        QMetaObject::connectSlotsByName(CGraphTableTab);
    } // setupUi

    void retranslateUi(QDialog *CGraphTableTab)
    {
        CGraphTableTab->setWindowTitle(QApplication::translate("CGraphTableTab", "Dialog", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QString());
        getWebDataButton->setText(QApplication::translate("CGraphTableTab", "H\303\244mta", 0, QApplication::UnicodeUTF8));
        FilterButton->setText(QApplication::translate("CGraphTableTab", "Filtrera", 0, QApplication::UnicodeUTF8));
        ShowLatestButton->setText(QApplication::translate("CGraphTableTab", "Visa", 0, QApplication::UnicodeUTF8));
        largeCapCheckBox->setText(QApplication::translate("CGraphTableTab", "Large Cap", 0, QApplication::UnicodeUTF8));
        midCapCheckBox->setText(QApplication::translate("CGraphTableTab", "Mid Cap", 0, QApplication::UnicodeUTF8));
        smallCapCheckBox->setText(QApplication::translate("CGraphTableTab", "Small Cap", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CGraphTableTab: public Ui_CGraphTableTab {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRAPHTABLETAB_H
