/****************************************************************
 *
 * Filename:       financemath.h
 *
 * Description:
 *
 *
 ****************************************************************/
#include "financemath.h"



/****************************************************************
 *
 * Function:
 *
 * Description:
 *
 *
 ****************************************************************/
FinanceMath::FinanceMath()
{

}

/****************************************************************
 *
 * Function:    calcGrowth()
 *
 * Description: Data is added in increase order:
 *                  2015, 2016, 2017
 *                  10,   20,   30
 *
 ****************************************************************/
bool FinanceMath::
calcGrowth(SubAnalysDataST *inDataArr, int nofData, SubAnalysDataST *outDataArr)
{
    double currData;
    double prevData;
    double prevPosData;
    double growth;
    QString growthStr;

    if(nofData < 2)
    {
        return false;
    }


    for(int i = 0; i < nofData; i++)
    {
        // Copy year
        outDataArr[i].date = inDataArr[i].date;

        if(i > 0)
        {
            currData = inDataArr[i].data.toDouble();
            prevData = inDataArr[i-1].data.toDouble();

            // Handle negative sign
            if(prevData < 0)
            {
                prevPosData = -prevData;
            }
            else
            {
                prevPosData = prevData;
            }


            // Handle divide by zero
            if(prevData == 0)
            {
                growth = currData / 0.001;
            }
            else
            {
                growth = (currData - prevData) / prevPosData;
            }

            growth = growth * 100;
            growthStr.sprintf("%.2f",  growth);
        }
        else
        {
            growthStr.clear();
        }

        outDataArr[i].data = growthStr;
    }

    return true;
}


