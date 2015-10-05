/********************************************************************************
** Form generated from reading UI file 'ctab3.ui'
**
** Created: Sun Feb 3 23:00:15 2013
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CTAB3_H
#define UI_CTAB3_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFormLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CTab3
{
public:
    QWidget *widget;
    QFormLayout *formLayout;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *verticalSpacer;
    QLineEdit *PathPriceDataLineEdit;
    QLineEdit *PathYieldDataLineEdit;
    QLineEdit *PathKeyNoDataLineEdit;

    void setupUi(QDialog *CTab3)
    {
        if (CTab3->objectName().isEmpty())
            CTab3->setObjectName(QString::fromUtf8("CTab3"));
        CTab3->resize(1200, 800);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CTab3->sizePolicy().hasHeightForWidth());
        CTab3->setSizePolicy(sizePolicy);
        widget = new QWidget(CTab3);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(30, 10, 1121, 813));
        formLayout = new QFormLayout(widget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        horizontalSpacer = new QSpacerItem(838, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        formLayout->setItem(2, QFormLayout::FieldRole, horizontalSpacer);

        label_2 = new QLabel(widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_2);

        horizontalSpacer_2 = new QSpacerItem(838, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        formLayout->setItem(5, QFormLayout::FieldRole, horizontalSpacer_2);

        label_3 = new QLabel(widget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(6, QFormLayout::LabelRole, label_3);

        horizontalSpacer_3 = new QSpacerItem(838, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        formLayout->setItem(8, QFormLayout::FieldRole, horizontalSpacer_3);

        verticalSpacer = new QSpacerItem(20, 598, QSizePolicy::Minimum, QSizePolicy::Expanding);

        formLayout->setItem(9, QFormLayout::FieldRole, verticalSpacer);

        PathPriceDataLineEdit = new QLineEdit(widget);
        PathPriceDataLineEdit->setObjectName(QString::fromUtf8("PathPriceDataLineEdit"));

        formLayout->setWidget(1, QFormLayout::SpanningRole, PathPriceDataLineEdit);

        PathYieldDataLineEdit = new QLineEdit(widget);
        PathYieldDataLineEdit->setObjectName(QString::fromUtf8("PathYieldDataLineEdit"));

        formLayout->setWidget(4, QFormLayout::SpanningRole, PathYieldDataLineEdit);

        PathKeyNoDataLineEdit = new QLineEdit(widget);
        PathKeyNoDataLineEdit->setObjectName(QString::fromUtf8("PathKeyNoDataLineEdit"));

        formLayout->setWidget(7, QFormLayout::SpanningRole, PathKeyNoDataLineEdit);


        retranslateUi(CTab3);

        QMetaObject::connectSlotsByName(CTab3);
    } // setupUi

    void retranslateUi(QDialog *CTab3)
    {
        CTab3->setWindowTitle(QApplication::translate("CTab3", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CTab3", "Path Price Data:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CTab3", "Path Yield Data:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("CTab3", "Path Key Number Data:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CTab3: public Ui_CTab3 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CTAB3_H
