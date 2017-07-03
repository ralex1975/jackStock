#ifndef PARSEOMXBRIDGEDATA_H
#define PARSEOMXBRIDGEDATA_H

#include <QFile>
#include <QString>
#include <QDebug>




struct OmxBridgeDataST
{
    QString stockSymbol;
    QString yahooStockName;
    QString omxStockName;
    QString omxLink;
};

class ParseOmxBridgeData
{
public:
    ParseOmxBridgeData();
    bool readFile(QString filename,
                  struct OmxBridgeDataST *omxBridgeDataArr,
                  int &nofReadData,
                  int maxSize);

};

#endif // PARSEOMXBRIDGEDATA_H
