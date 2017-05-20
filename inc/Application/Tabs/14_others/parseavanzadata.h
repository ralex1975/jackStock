#ifndef CPARSEAVANZADATA_H
#define CPARSEAVANZADATA_H

#include <QString>


class CParseAvanzaData
{
public:
        struct parserData_ST
        {
            QString date;
            QString account;
            QString typeOfData;
            QString assetName;
            QString nofItems;
            QString assetPrice;
            QString totalPrice;
            QString currency;
        };


    CParseAvanzaData();
    bool parseData(QString filename);


private:
    bool parseOneLine(QString str, parserData_ST &result);
    bool validNofParsedTockens(const int i, const int maxNofLineTockens);
    void ifnumberConvertCommaToDot(QString &data);
    bool updateResult(QString subresult, parserData_ST &result, int &i, const int maxNofLineTockens);


};

#endif // CPARSEAVANZADATA_H
