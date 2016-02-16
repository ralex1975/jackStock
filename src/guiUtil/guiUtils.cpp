/*******************************************************************
 *
 * Filename: guiUtils.h
 * Date:     2012-09-23
 *
 * Description: Utils used to handle GUI
 *
 *
 *******************************************************************/

#include "../../inc/guiUtil/guiUtils.h"
#define COMBO_BOX_NOT_USED " "






/****************************************************************
 *
 * Function:    CGuiUtils()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
CGuiUtils::CGuiUtils()
{
}


/****************************************************************
 *
 * Function:    addSortCompareToComboBox()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
void CGuiUtils::addSortCompareToComboBox(QComboBox *cb)
{
    cb->addItem((const QString)">");
    cb->addItem((const QString)">=");
    cb->addItem((const QString)"==");
    cb->addItem((const QString)"<=");
    cb->addItem((const QString)"<");
}




/****************************************************************
 *
 * Function:    addSortCompareToComboBox()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
void CGuiUtils::
addindicatorToComboBox(QComboBox *cb, TableColumnIndicatorInfo_ST *tableHeaderList, int maxNofData)
{
    cb->addItem((QString) COMBO_BOX_NOT_USED);
    for(int i = 1; i < maxNofData; i++)
    {
        cb->addItem(tableHeaderList[i].name);
    }
}

