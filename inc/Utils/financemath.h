#ifndef FINANCEMATH_H
#define FINANCEMATH_H

#include "dbHndl.h"

class FinanceMath
{


public:
    FinanceMath();
    bool modifiedZScore(double *inArr, int arrLen, bool *isOutlierArr);
    bool mAD(double *inArr, int arrLen, double &mad);
    double absolute(double value);
    bool getMedian(double *inArr,int arrLen, double &median);
    bool calcGrowth(SubAnalysDataST *inDataArr, int nofData, SubAnalysDataST *outDataArr);
    void singleEventGrowth(double curr, double prev,  double &result);


};

#endif // FINANCEMATH_H
