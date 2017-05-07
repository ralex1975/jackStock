#ifndef PARSEOMXALLFINANCELINKDATA_H
#define PARSEOMXALLFINANCELINKDATA_H

#include <QFile>
#include <QString>
#include <QDebug>

#include "parseomxbridgedata.h"


#define PARSE_ANUAL_ARR_SIZE 10


class ParseOmxAllFinanceLinkData
{
    QString m_yearArr[PARSE_ANUAL_ARR_SIZE];
    int m_yearArrIndex;
    QString m_dataName;
    QString m_dataArr[PARSE_ANUAL_ARR_SIZE];
    int m_dataArrIndex;

    QString m_tmpEquityDataArr[PARSE_ANUAL_ARR_SIZE];
    int m_tmpEquityDataArrIndex;
    bool m_tmpEquityDatatmpEquityDatIsClear;



public:
    enum StateParseAnualDataET
    {
        ASTATE_PARSE_HEADER,
        ASTATE_PARSE_YEAR,
        ASTATE_PARSE_DATA_NAME,
        ASTATE_PARSE_DATA,
        ASTATE_PARSE_UNKNOWN
    };





    ParseOmxAllFinanceLinkData();
    bool parseFinanceLinks(QString filename);
    bool parseAnualData(QString filename, struct OmxBridgeDataST *omxBridgeDataArr, int currArrIndex);


private:
    void addDataToDb(struct OmxBridgeDataST *omxBridgeDataArr, int currArrIndex, QString sheetType);
    void insertDividendPerShareIntoDb(struct OmxBridgeDataST *omxBridgeDataArr, int currArrIndex);
    void insertEarningsPerShareIntoDb(struct OmxBridgeDataST *omxBridgeDataArr, int currArrIndex);
    void insertTotalCurrentAssetIntoDb(struct OmxBridgeDataST *omxBridgeDataArr, int currArrIndex);
    void insertTotalCurrentLiabilitiesIntoDb(struct OmxBridgeDataST *omxBridgeDataArr, int currArrIndex);
    void insertTotalLiabilitiesIntoDb(struct OmxBridgeDataST *omxBridgeDataArr, int currArrIndex);
    void insertEquityIntoDb(struct OmxBridgeDataST *omxBridgeDataArr, int currArrIndex);
    void insertSolidityIntoDb(struct OmxBridgeDataST *omxBridgeDataArr, int currArrIndex);
    void insertCapexIntoDb(struct OmxBridgeDataST *omxBridgeDataArr, int currArrIndex);
    void insertOperatingCashFlowIntoDb(struct OmxBridgeDataST *omxBridgeDataArr, int currArrIndex);
    void insertTotDividendsIntoDb(struct OmxBridgeDataST *omxBridgeDataArr, int currArrIndex);
    void insertRevenueIntoDb(struct OmxBridgeDataST *omxBridgeDataArr, int currArrIndex);
    void insertNetIncomeIntoDb(struct OmxBridgeDataST *omxBridgeDataArr, int currArrIndex);







    bool getParserState(int indexHeader,
                        int indexYear,
                        int indexDataName,
                        int indexData,
                        ParseOmxAllFinanceLinkData::StateParseAnualDataET &cmd);

};

#endif // PARSEOMXALLFINANCELINKDATA_H
