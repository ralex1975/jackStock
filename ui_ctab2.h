/********************************************************************************
** Form generated from reading UI file 'ctab2.ui'
**
** Created: Tue 4. Sep 22:26:24 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CTAB2_H
#define UI_CTAB2_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_CTab2
{
public:
    QPushButton *pushButton;

    void setupUi(QDialog *CTab2)
    {
        if (CTab2->objectName().isEmpty())
            CTab2->setObjectName(QString::fromUtf8("CTab2"));
        CTab2->resize(963, 681);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CTab2->sizePolicy().hasHeightForWidth());
        CTab2->setSizePolicy(sizePolicy);
        pushButton = new QPushButton(CTab2);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(490, 20, 75, 23));

        retranslateUi(CTab2);

        QMetaObject::connectSlotsByName(CTab2);
    } // setupUi

    void retranslateUi(QDialog *CTab2)
    {
        CTab2->setWindowTitle(QApplication::translate("CTab2", "Dialog", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("CTab2", "Get Data", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CTab2: public Ui_CTab2 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CTAB2_H
