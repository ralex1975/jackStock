#ifndef CPARSETADATA_H
#define CPARSETADATA_H
#include <QtCore>
#include <QtGui>

class CParseTaData
{
public:
    CParseTaData();
    bool parseData(QString filename, QString StockName, QString stockSymbol, bool m_disableMsgBoxes=false);
};

#endif // CPARSETADATA_H
