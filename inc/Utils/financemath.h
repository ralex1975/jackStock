#ifndef FINANCEMATH_H
#define FINANCEMATH_H

#include "dbHndl.h"

class FinanceMath
{
public:
    FinanceMath();
    bool calcGrowth(SubAnalysDataST *inDataArr, int nofData, SubAnalysDataST *outDataArr);

};

#endif // FINANCEMATH_H
