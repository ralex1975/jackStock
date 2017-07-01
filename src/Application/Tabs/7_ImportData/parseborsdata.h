#ifndef PARSEBORSDATA_H
#define PARSEBORSDATA_H

#include <QString>


class ParseBorsData
{
    enum MainStateET
    {
        MSTATE_YEAR,
        MSTATE_SOLIDITY,
        MSTATE_EQUITY_PER_SHARE,
        MSTATE_EARNINGS_PER_SHARE,
        MSTATE_DIVIDEND
    };

    MainStateET m_mainState;

public:
    ParseBorsData();
    bool helpParser(QString &result, QRegExp rx, MainStateET mainState, QString dbgStr, QString *resArr, int &nofArrData);
    bool cleanupParsedProcentValue(QString &value, QString str);
    bool cleanupParsedRealValue(QString &value, QString str);
    bool readFile(QString filename);
};

#endif // PARSEBORSDATA_H
