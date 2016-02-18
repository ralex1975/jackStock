/**********************************************************************************
 *
 * Filename:    guiFinanceCtrl.cpp
 *
 *
 * Description:
 *
 *
 *
 ********************************************************************************/

#include "../../inc/guiUtil/guiFinanceCtrls.h"
#include "../../inc/guiUtil/guiFinanceColor.h"



/**********************************************************************************
 *
 * Function:
 *
 *
 * Description:
 *
 *
 *
 ********************************************************************************/
GuiFinanceCtrls::GuiFinanceCtrls(QWidget *parent) :
    QWidget(parent)
{

}



/**********************************************************************************
 *
 * Function:        fa5AddData()
 *
 *
 * Description:     Add data and set text color on 5 fundamental analysis controls
 *
 *
 *
 ********************************************************************************/
void GuiFinanceCtrls::fa5AddData(QString inEarningsDivDividend,  QLineEdit *earningsDivDividend,
                                 QString inNavDivStockPrice,     QLineEdit *navDivStockPrice,
                                 QString inPe,                   QLineEdit *pe,
                                 QString inPs,                   QLineEdit *ps,
                                 QString inYield,                QLineEdit *yield,
                                 QString assetType)
{
    GuiFinanceColor gfc;

    // Remove data
    fa5Reset(earningsDivDividend,
             navDivStockPrice,
             pe,
             ps,
             yield);

    // Add data
    earningsDivDividend->insert(inEarningsDivDividend);
    navDivStockPrice->insert(inNavDivStockPrice);
    pe->insert(inPe);
    ps->insert(inPs);
    yield->insert(inYield);

    // Set text color
    gfc.SetTxtColorEarningsDivDividend(inEarningsDivDividend, earningsDivDividend);
    gfc.setTxtColorNavDivStockPrice(inNavDivStockPrice, navDivStockPrice);
    gfc.setTxtColorPe(inPe, pe, assetType);
    gfc.setTxtColorPs(inPs, ps);
    gfc.setTxtColorYield(inYield, inEarningsDivDividend, yield);
}




/**********************************************************************************
 *
 * Function:        fa5CtrlReset()
 *
 *
 * Description: Remove data from 5 fundamental analysis controls
 *
 *
 *
 ********************************************************************************/
void GuiFinanceCtrls::fa5Reset(QLineEdit *earningsDivDividend,
                               QLineEdit *navDivStockPrice,
                               QLineEdit *pe,
                               QLineEdit *ps,
                               QLineEdit *yield)
{
    earningsDivDividend->clear();
    navDivStockPrice->clear();
    pe->clear();
    ps->clear();
    yield->clear();
}


