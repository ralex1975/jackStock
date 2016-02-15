/*******************************************************************
 *
 * Filename: guiUtils.cpp
 * Date:     2012-09-23
 *
 * Description: Utils used to handle GUI
 *
 *
 *******************************************************************/

#ifndef GUIUTILS_H
#define GUIUTILS_H

#include <QtCore>
#include <QtGui>
#include "dbHndl.h"


class CGuiUtils
{
public:


private:


public:
    CGuiUtils();
    void addSortCompareToComboBox(QComboBox *cb);
    void addindicatorToComboBox(QComboBox *cb, TableColumnIndicatorInfo_ST *tableHeaderList, int maxNofData);

};

#endif // GUIUTILS_H
