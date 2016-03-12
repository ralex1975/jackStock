#ifndef GuiFinanceColor_H
#define GuiFinanceColor_H

#include <QLineEdit>
#include "myLineEdit.h"

class GuiFinanceColor
{

    enum FAData_ET
    {
        FA_PE,
        FA_PS,
        FA_ERNING_DIV_DIVIDEN,      // Vinst / utdelning
        FA_NAV_DIV_LAST_PRICE,      // Substans / Kurs
        FA_YIELD,                   // Direktavkastning
        FA_MEAN_RETURN,
        FA_MEAN_RISK,
        FA_NOF_DATA
    };


     QPalette *m_faDataPalette[FA_NOF_DATA];



public:
    GuiFinanceColor();
    ~GuiFinanceColor();
    QColor getColorEarningsDivDividend(QString inValue);
    QColor getColorEarningsDivDividend(QString inValue, Qt::GlobalColor &gClolor);
    void setTxtColorEarningsDivDividend(QString inValue, QLineEdit *outValue);

    QColor getColorNavDivStockPrice(QString inValue);
    QColor getColorNavDivStockPrice(QString inValue, Qt::GlobalColor &gColor);
    void setTxtColorNavDivStockPrice(QString inValue, QLineEdit *outValue);

    QColor getColorPe(QString pe);
    QColor getColorPe(QString pe, Qt::GlobalColor &gColor);
    void setTxtColorPe(QString inValue, QLineEdit *outValue, QString assetType = "");

    QColor getColorPs(QString inValue);
    QColor getColorPs(QString inValue, Qt::GlobalColor &gColor);
    void setTxtColorPs(QString inValue, QLineEdit *outValue);

    QColor getColorYield(QString yield, QString earningsDivDividend);
    QColor getColorYield(QString yield, QString earningsDivDividend, Qt::GlobalColor &gColor);
    void setTxtColorYield(QString yield, QString earningsDivDividend, QLineEdit *outValue);

    // Qt::GlobalColor convertQColor(QColor color);

};

#endif // GuiFinanceColor_H
