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
    void setTxtColorEarningsDivDividend(QString inValue, QLineEdit *outValue);
    void setTxtColorNavDivStockPrice(QString inValue, QLineEdit *outValue);
    QColor getPeColor(QString pe);
    void setTxtColorPe(QString inValue, QLineEdit *outValue, QString assetType = "");
    void setTxtColorPs(QString inValue, QLineEdit *outValue);
    void setTxtColorYield(QString yield, QString earningsDivDividend, QLineEdit *outValue);


};

#endif // GuiFinanceColor_H
