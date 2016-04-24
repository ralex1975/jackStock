#include "../../inc/guiUtil/guiFinanceColor.h"
#include "util.h"
#include <QColor>





/**********************************************************************************
 *
 * Function:        GuiFinanceColor()
 *
 *
 * Description:
 *
 *
 *
 ********************************************************************************/
GuiFinanceColor::GuiFinanceColor()
{
    int i;

    for(i = 0; i < FA_NOF_DATA; i++)
    {
        m_faDataPalette[i] = new QPalette();
    }
}


/**********************************************************************************
 *
 * Function:    ()
 *
 *
 * Description:
 *
 *
 *
 ********************************************************************************/
GuiFinanceColor::~GuiFinanceColor()
{
    int i;

    for(i = 0; i < FA_NOF_DATA; i++)
    {
        delete m_faDataPalette[i];
    }
}



/**********************************************************************************
 *
 * Function:    setTxtColorEarningsDivDividend()
 *
 *
 * Description: Vinst/Utdelning
 *
 *
 *
 ********************************************************************************/
Qt::GlobalColor GuiFinanceColor::getColorRiskReturns(double riskStdDev, double meanReturns)
{
    Qt::GlobalColor riskReturnColor = Qt::darkYellow;


    if((riskStdDev) > 20 || (meanReturns) < 0)
    {
        riskReturnColor = Qt::red;
    }
    else if((riskStdDev) >= 15 && (riskStdDev) <= 20|| (meanReturns) > 0)
    {
        riskReturnColor = Qt::magenta;
    }
    else if((riskStdDev) > 10 && (riskStdDev) < 15 && (meanReturns) > 0)
    {
        riskReturnColor = Qt::darkYellow;
    }
    else if((riskStdDev) <= 10 && (meanReturns) > 7)
    {
        riskReturnColor = Qt::darkGreen;
    }

    return riskReturnColor;
}



/**********************************************************************************
 *
 * Function:    setTxtColorEarningsDivDividend()
 *
 *
 * Description: Vinst/Utdelning
 *
 *
 *
 ********************************************************************************/
QColor GuiFinanceColor::
getColorEarningsDivDividend(QString inValue)
{
    Qt::GlobalColor gClolor;

    return getColorEarningsDivDividend(inValue, gClolor);

}


/**********************************************************************************
 *
 * Function:    setTxtColorEarningsDivDividend()
 *
 *
 * Description: Vinst/Utdelning
 *
 *
 *
 ********************************************************************************/
QColor GuiFinanceColor::
getColorEarningsDivDividend(QString inValue, Qt::GlobalColor &gClolor)
{
    CUtil cu;
    QColor color;
    double inValueDouble;

    if(false == cu.number2double(inValue, inValueDouble))
    {
        color = Qt::red;
        gClolor = Qt::red;
        return color;
    }

    // Vinst/Utdelning
    if((inValue.toDouble() >= 1) && (inValue.toDouble() < 2))
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
        gClolor = Qt::magenta;
    }
    else if(inValue.toDouble() >= 2)
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::GREEN);
        gClolor = Qt::darkGreen;
    }
    else
    {
        color = Qt::red;
        gClolor = Qt::red;
    }

    return color;
}


/**********************************************************************************
 *
 * Function:    setTxtColorEarningsDivDividend()
 *
 *
 * Description: Vinst/Utdelning
 *
 *
 *
 ********************************************************************************/
void GuiFinanceColor::
setTxtColorEarningsDivDividend(QString inValue, QLineEdit *outValue)
{
    CUtil cu;
    QColor color;
    double inValueDouble;


    if(false == cu.number2double(inValue, inValueDouble))
    {
        color = Qt::red;
        MyLineEdit::setTxtColor(outValue, m_faDataPalette[FA_ERNING_DIV_DIVIDEN], color);

        return;
    }

    // Vinst/Utdelning
    if((inValue.toDouble() >= 1) && (inValue.toDouble() < 2))
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
    }
    else if(inValue.toDouble() >= 2)
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::GREEN);
    }
    else
    {
        color = Qt::red;
    }

    MyLineEdit::setTxtColor(outValue, m_faDataPalette[FA_ERNING_DIV_DIVIDEN], color);

}

/**********************************************************************************
 *
 * Function:    setTxtColorNavDivStockPrice()
 *
 *
 * Description: Substans/Aktiepris
 *
 *
 *
 ********************************************************************************/
QColor GuiFinanceColor::
getColorNavDivStockPrice(QString inValue)
{
    Qt::GlobalColor gColor;

    return getColorNavDivStockPrice(inValue, gColor);

}



/**********************************************************************************
 *
 * Function:    setTxtColorNavDivStockPrice()
 *
 *
 * Description: Substans/Aktiepris
 *
 *
 *
 ********************************************************************************/
QColor GuiFinanceColor::
getColorNavDivStockPrice(QString inValue, Qt::GlobalColor &gColor)
{
    CUtil cu;
    QColor color;
    double inValueDouble;


    if(false == cu.number2double(inValue, inValueDouble))
    {
        color = Qt::red;
        gColor = Qt::red;
        return color;
    }

    // Substans/Aktiepris
    if((inValue.toDouble() >= 0.7) && (inValue.toDouble() < 1))
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3);
        gColor = Qt::darkYellow;
    }
    else if((inValue.toDouble() >= 1) && (inValue.toDouble() < 2))
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
        gColor = Qt::magenta;
    }
    else if(inValue.toDouble() >= 2)
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::GREEN);
        gColor = Qt::darkGreen;
    }
    else
    {
        color = Qt::red;
        gColor = Qt::red;
    }

    return color;

}


/**********************************************************************************
 *
 * Function:    setTxtColorNavDivStockPrice()
 *
 *
 * Description: Substans/Aktiepris
 *
 *
 *
 ********************************************************************************/
void GuiFinanceColor::
setTxtColorNavDivStockPrice(QString inValue, QLineEdit *outValue)
{
    //CUtil cu;
    QColor color;
    //double inValueDouble;

#if 0

    if(false == cu.number2double(inValue, inValueDouble))
    {
        color = Qt::red;
        MyLineEdit::setTxtColor(outValue, m_faDataPalette[FA_NAV_DIV_LAST_PRICE], color);

        return;
    }

    // Substans/Aktiepris
    if((inValue.toDouble() >= 0.7) && (inValue.toDouble() < 1))
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3);
    }
    else if((inValue.toDouble() >= 1) && (inValue.toDouble() < 2))
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
    }
    else if(inValue.toDouble() >= 2)
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::GREEN);
    }
    else
    {
        color = Qt::red;
    }
#endif
    color = getColorNavDivStockPrice(inValue);

    MyLineEdit::setTxtColor(outValue, m_faDataPalette[FA_NAV_DIV_LAST_PRICE], color);

}


/**********************************************************************************
 *
 * Function:    setTxtColorPe()
 *
 *
 * Description:
 *
 *
 *
 ********************************************************************************/
QColor GuiFinanceColor::getColorPe(QString pe)
{
    Qt::GlobalColor gColor;

    return getColorPe(pe, gColor);

}


/**********************************************************************************
 *
 * Function:    setTxtColorPe()
 *
 *
 * Description:
 *
 *
 *
 ********************************************************************************/
QColor GuiFinanceColor::getColorPe(QString pe, Qt::GlobalColor &gColor)
{
    CUtil cu;
    QColor color;
    double peDouble;


    if(false == cu.number2double(pe, peDouble))
    {
        color = Qt::red;
        gColor = Qt::red;
        return color;
    }

    // PE Vaue
    if(pe.toDouble() > 2 && pe.toDouble() < 5)
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
        gColor = Qt::magenta; // Orange is missing
    }
    else if((pe.toDouble() >= 5) && (pe.toDouble() <= 15))
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::GREEN);
        gColor = Qt::darkGreen;
    }
    else if((pe.toDouble() > 15) && (pe.toDouble() <= 18))
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3);
        gColor = Qt::darkYellow;
    }
    else if((pe.toDouble() > 18) && (pe.toDouble() <= 25))
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
        gColor = Qt::magenta; // Orange is missing
    }
    else
    {
        color = Qt::red;
        gColor = Qt::red;
    }

    return color;

}




/**********************************************************************************
 *
 * Function:    getColorDebtToEquityRatio()
 *
 *
 * Description:
 *
 *
 *
 ********************************************************************************/
QColor GuiFinanceColor::getColorDebtToEquityRatio(QString debtToEquityRatio, Qt::GlobalColor &gColor)
{
    CUtil cu;
    QColor color;
    double doubleValue;


    if(false == cu.number2double(debtToEquityRatio, doubleValue))
    {
        color = Qt::red;
        gColor = Qt::red;
        return color;
    }

    // PE Vaue
    if(debtToEquityRatio.toDouble() > 150.0)
    {
        color = Qt::red;
        gColor = Qt::red;
    }
    else if((debtToEquityRatio.toDouble() >= 100.0) && (debtToEquityRatio.toDouble() <= 150.0))
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
        gColor = Qt::magenta; // Orange is missing
    }
    else
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::GREEN);
        gColor = Qt::darkGreen;
    }

    return color;

}


/**********************************************************************************
 *
 * Function:    getColorCurrentRatio()
 *
 *
 * Description:
 *
 *
 *
 ********************************************************************************/
QColor GuiFinanceColor::getColorCurrentRatio(QString currentRatio, Qt::GlobalColor &gColor)
{
    CUtil cu;
    QColor color;
    double doubleValue;


    if(false == cu.number2double(currentRatio, doubleValue))
    {
        color = Qt::red;
        gColor = Qt::red;
        return color;
    }


    if(currentRatio.toDouble() > 1.5)
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::GREEN);
        gColor = Qt::darkGreen;
    }
    else if((currentRatio.toDouble() >= 1.0) && (currentRatio.toDouble() <= 1.5))
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
        gColor = Qt::magenta; // Orange is missing
    }
    else
    {
        color = Qt::red;
        gColor = Qt::red;
    }

    return color;

}



/**********************************************************************************
 *
 * Function:    setTxtColorPe()
 *
 *
 * Description:
 *
 *
 *
 ********************************************************************************/
void GuiFinanceColor::
setTxtColorPe(QString inValue, QLineEdit *outValue, QString assetType)
{
    QColor color;

    color = getColorPe(inValue);

    // Disable P/E when company type is finance and real estate
    if(assetType.compare("Finans och fastighet")== 0)
    {
        outValue->setDisabled(true);
    }
    else
    {
        outValue->setDisabled(false);
    }

    MyLineEdit::setTxtColor(outValue, m_faDataPalette[FA_PE], color);

}



#if 0
/**********************************************************************************
 *
 * Function:    setTxtColorPe()
 *
 *
 * Description:
 *
 *
 *
 ********************************************************************************/
void GuiFinanceColor::
setTxtColorPe(QString inValue, QLineEdit *outValue, QString assetType)
{
    CUtil cu;
    QColor color;
    double inValueDouble;


    if(false == cu.number2double(inValue, inValueDouble))
    {
        color = Qt::red;
        MyLineEdit::setTxtColor(outValue, m_faDataPalette[FA_PE], color);

        return;
    }

    // PE Vaue
    if(inValue.toDouble() < 5)
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
    }
    else if((inValue.toDouble() >= 5) && (inValue.toDouble() <= 15))
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::GREEN);
    }
    else if((inValue.toDouble() > 15) && (inValue.toDouble() <= 18))
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3);
    }
    else if((inValue.toDouble() > 18) && (inValue.toDouble() <= 25))
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
    }
    else
    {
        color = Qt::red;
    }

    // Disable P/E when company type is finance and real estate
    if(assetType.compare("Finans och fastighet")== 0)
    {
        outValue->setDisabled(true);
    }
    else
    {
        outValue->setDisabled(false);
    }

    MyLineEdit::setTxtColor(outValue, m_faDataPalette[FA_PE], color);

}
#endif

/**********************************************************************************
 *
 * Function:    setTxtColorPs()
 *
 *
 * Description:
 *
 *
 *
 ********************************************************************************/
QColor GuiFinanceColor::
getColorPs(QString inValue)
{
    Qt::GlobalColor gColor;

    return getColorPs(inValue, gColor);
}


/**********************************************************************************
 *
 * Function:    setTxtColorPs()
 *
 *
 * Description:
 *
 *
 *
 ********************************************************************************/
QColor GuiFinanceColor::
getColorPs(QString inValue, Qt::GlobalColor &gColor)
{
    CUtil cu;
    QColor color;
    double inValueDouble;

    if(false == cu.number2double(inValue, inValueDouble))
    {
        color = Qt::red;
        gColor = Qt::red;
        return color;
    }


    if(inValue.toDouble() <= 0.75)
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::GREEN);
        gColor = Qt::darkGreen;

    }
    else if((inValue.toDouble() > 0.75) && (inValue.toDouble() <= 1.5))
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3);
        gColor = Qt::darkYellow;
    }
    else if((inValue.toDouble() > 1.5) && (inValue.toDouble() <= 3.0))
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
        gColor = Qt::magenta;  // Orange is missing
    }
    else
    {
        color = Qt::red;
        gColor = Qt::red;
    }

    return color;

}

/**********************************************************************************
 *
 * Function:    setTxtColorPs()
 *
 *
 * Description:
 *
 *
 *
 ********************************************************************************/
void GuiFinanceColor::
setTxtColorPs(QString inValue, QLineEdit *outValue)
{
    CUtil cu;
    QColor color;
    double inValueDouble;

#if 0

    if(false == cu.number2double(inValue, inValueDouble))
    {
        color = Qt::red;
        MyLineEdit::setTxtColor(outValue, m_faDataPalette[FA_PS], color);

        return;
    }


    if(inValue.toDouble() <= 0.75)
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::GREEN);
    }
    else if((inValue.toDouble() > 0.75) && (inValue.toDouble() <= 1.5))
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3);
    }
    else if((inValue.toDouble() > 1.5) && (inValue.toDouble() <= 3.0))
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
    }
    else
    {
        color = Qt::red;
    }
#endif

    color = getColorPs(inValue);

    MyLineEdit::setTxtColor(outValue, m_faDataPalette[FA_PS], color);

}

#if 0
/**********************************************************************************
 *
 * Function:    convertQColor()
 *
 *
 * Description:
 *
 *
 *
 ********************************************************************************/
Qt::GlobalColor GuiFinanceColor::convertQColor(QColor color)
{
    CUtil cu;

    Qt::GlobalColor gColor = Qt::black;

    if(color == Qt::red)
    {
        gColor = Qt::red;
    }
    else if(color == cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3))
    {
        gColor = Qt::darkYellow;
    }
    else if(color == cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE))
    {
        gColor = Qt::magenta; // Qt::darkMagenta; // Orange missing
    }
    else if(color == cu.getQColor((CUtil::ColorRgb_ET) CUtil::GREEN))
    {
        gColor = Qt::magenta; // Qt::darkGreen; // Orange missing
    }

    return gColor;

}
#endif

/**********************************************************************************
 *
 * Function:    setTxtColorYield()
 *
 *
 * Description:
 *
 *
 *
 ********************************************************************************/
QColor GuiFinanceColor::
getColorYield(QString yield, QString earningsDivDividend)
{
    Qt::GlobalColor gColor;

    return getColorYield(yield, earningsDivDividend, gColor);

}



/**********************************************************************************
 *
 * Function:    setTxtColorYield()
 *
 *
 * Description:
 *
 *
 *
 ********************************************************************************/
QColor GuiFinanceColor::
getColorYield(QString yield, QString earningsDivDividend, Qt::GlobalColor &gColor)
{
    CUtil cu;
    QColor color;
    double doubleEarningsDivDividend;
    double doubleYield;



    if(false == cu.number2double(yield, doubleYield))
    {
        color = Qt::red;
        gColor = Qt::red;
        return color;
    }


    if(false == cu.number2double(earningsDivDividend, doubleEarningsDivDividend))
    {
        if(yield.toDouble() < 2)
        {
            color = Qt::red;
            gColor = Qt::red;
        }
        else if((yield.toDouble() >= 3) && (yield.toDouble() < 4))
        {
            color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
            gColor = Qt::magenta;
        }
        else if((yield.toDouble() >= 4) && (yield.toDouble() <= 6))
        {
            color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::GREEN);
            gColor = Qt::darkGreen;
        }
        else if((yield.toDouble() > 6) && (yield.toDouble() < 7))
        {
            color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
            gColor = Qt::magenta;
        }
        else if((yield.toDouble() > 7) && (yield.toDouble() < 10))
        {
            color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3);
            gColor = Qt::darkYellow;
        }
        else if(yield.toDouble() > 10)
        {
            color = Qt::red;
            gColor = Qt::red;
        }
        else
        {
            color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3);
            gColor = Qt::darkYellow;
        }
    }
    else
    {
        //  Grön Om vinst/Utdelning är >= 2 && direktavkastningn = 4 - 6 %
        if((doubleEarningsDivDividend > 1) && (doubleEarningsDivDividend < 2))
        {
            if(yield.toDouble() < 2)
            {
                color = Qt::red;
                gColor = Qt::red;
            }
            else if((yield.toDouble() >= 3) && (yield.toDouble() < 4))
            {
                color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3);
                gColor = Qt::darkYellow;
            }
            else if(yield.toDouble() >= 4 )
            {
                color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
                gColor = Qt::magenta;
            }
            else
            {
                color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3);
                gColor = Qt::darkYellow;
            }
        }
        else if(doubleEarningsDivDividend >= 2)
        {
            if(yield.toDouble() < 2)
            {
                color = Qt::red;
                gColor = Qt::red;
            }
            else if((yield.toDouble() >= 3) && (yield.toDouble() < 4))
            {
                color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
                gColor = Qt::magenta;
            }
            else if(yield.toDouble() >= 4)
            {
                color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::GREEN);
                gColor = Qt::darkGreen;
            }
            else
            {
                color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3);
                gColor = Qt::darkYellow;
            }
        }
        else
        {
            color = Qt::red;
            gColor = Qt::red;
        }
    }

    return color;
}


/**********************************************************************************
 *
 * Function:    setTxtColorYield()
 *
 *
 * Description:
 *
 *
 *
 ********************************************************************************/
void GuiFinanceColor::
setTxtColorYield(QString yield, QString earningsDivDividend, QLineEdit *outValue)
{
    // CUtil cu;
    QColor color;
    //double doubleEarningsDivDividend;
    //double doubleYield;


#if 0
    if(false == cu.number2double(yield, doubleYield))
    {
        color = Qt::red;
        MyLineEdit::setTxtColor(outValue, m_faDataPalette[FA_YIELD], color);
        return;
    }


    if(false == cu.number2double(earningsDivDividend, doubleEarningsDivDividend))
    {
        if((yield.toDouble() >= 3) && (yield.toDouble() < 4))
        {
            color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
        }
        else if((yield.toDouble() >= 4) && (yield.toDouble() <= 6))
        {
            color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::GREEN);
        }
        else if((yield.toDouble() > 6) && (yield.toDouble() < 7))
        {
            color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
        }
        else if((yield.toDouble() > 7) && (yield.toDouble() < 10))
        {
            color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3);
        }
        else if(yield.toDouble() > 10)
        {
            color = Qt::red;
        }
        else
        {
            color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3);
        }
    }
    else
    {
        //  Grön Om vinst/Utdelning är >= 2 && direktavkastningn = 4 - 6 %
        if((doubleEarningsDivDividend > 1) && (doubleEarningsDivDividend < 2))
        {
            if((yield.toDouble() >= 3) && (yield.toDouble() < 4))
            {
                color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3);
            }
            else if(yield.toDouble() >= 4 )
            {
                color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
            }
            else
            {
                color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3);
            }
        }
        else if(doubleEarningsDivDividend >= 2)
        {
            if((yield.toDouble() >= 3) && (yield.toDouble() < 4))
            {
                color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
            }
            else if(yield.toDouble() >= 4)
            {
                color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::GREEN);
            }
            else
            {
                color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3);
            }
        }
        else
        {
            color = Qt::red;
        }
    }
#endif
    color = getColorYield(yield, earningsDivDividend);


    MyLineEdit::setTxtColor(outValue, m_faDataPalette[FA_YIELD], color);

}


