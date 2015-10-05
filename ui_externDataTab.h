/********************************************************************************
** Form generated from reading UI file 'externDataTab.ui'
**
** Created: Sun Feb 3 23:00:15 2013
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXTERNDATATAB_H
#define UI_EXTERNDATATAB_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_CExternDataTab
{
public:
    QComboBox *dataSelComboBox;
    QPushButton *getExternDataButton;
    QPushButton *ProcessDataButton;

    void setupUi(QDialog *CExternDataTab)
    {
        if (CExternDataTab->objectName().isEmpty())
            CExternDataTab->setObjectName(QString::fromUtf8("CExternDataTab"));
        CExternDataTab->resize(1200, 800);
        dataSelComboBox = new QComboBox(CExternDataTab);
        dataSelComboBox->setObjectName(QString::fromUtf8("dataSelComboBox"));
        dataSelComboBox->setGeometry(QRect(30, 30, 151, 22));
        getExternDataButton = new QPushButton(CExternDataTab);
        getExternDataButton->setObjectName(QString::fromUtf8("getExternDataButton"));
        getExternDataButton->setGeometry(QRect(30, 70, 151, 23));
        ProcessDataButton = new QPushButton(CExternDataTab);
        ProcessDataButton->setObjectName(QString::fromUtf8("ProcessDataButton"));
        ProcessDataButton->setGeometry(QRect(30, 100, 151, 23));

        retranslateUi(CExternDataTab);

        QMetaObject::connectSlotsByName(CExternDataTab);
    } // setupUi

    void retranslateUi(QDialog *CExternDataTab)
    {
        CExternDataTab->setWindowTitle(QApplication::translate("CExternDataTab", "Dialog", 0, QApplication::UnicodeUTF8));
        getExternDataButton->setText(QApplication::translate("CExternDataTab", "H\303\244mta", 0, QApplication::UnicodeUTF8));
        ProcessDataButton->setText(QApplication::translate("CExternDataTab", "Process Data", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CExternDataTab: public Ui_CExternDataTab {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXTERNDATATAB_H
