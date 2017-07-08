/****************************************************************
 *
 * Filename:       financemath.h
 *
 * Description:
 *
 *
 ****************************************************************/
#include "financemath.h"
#include <iostream>
#include <algorithm>

using namespace std;




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
 * Function:
 *
 * Description:
 *
 *
 ****************************************************************/
bool FinanceMath::getMinMaxAvgYData(SubAnalysDataST  *dataArr,
                                    int nofArrData,
                                    QString &minY,
                                    QString &maxY,
                                    QString &avgY)
{
    double y;
    double dbMinY;
    double dbMaxY;
    double dbAvg;
    double sum;
    bool isValid;

    if(nofArrData < 1)
    {
        return false;
    }


    y = dataArr[0].data.toDouble(&isValid);
    if(isValid == false)
    {
        return false;
    }

    dbMinY = y;
    dbMaxY = y;
    sum = y;

    for(int i = 1; i < nofArrData; i++)
    {
        y = dataArr[i].data.toDouble(&isValid);
        if(isValid == false)
        {
            return false;
        }

        if(dbMinY > y)
        {
            dbMinY = y;
        }

        if(dbMaxY < y)
        {
            dbMaxY = y;
        }

        sum += y;
    }

    dbAvg = (sum / (double) nofArrData);

    minY.sprintf("%.2f", dbMinY);
    maxY.sprintf("%.2f", dbMaxY);
    avgY.sprintf("%.2f", dbAvg);

    return true;
}






/****************************************************************
 *
 * Function:    modifiedZScore()
 *
 * Description:  This function finds Outliers in in any random data set.
 *               An Outlier is a data in the data set that have
 *               an unusually high or low deviation from the data set
 *               median value.
 *
 *              Mi=0.6745 * (Xi -Median(Xi)) / MAD,
 *
 *  Note: standard limit for this function is:
 *          - const double maxLimit =  3.5;
 *          - const double minLimit = -3.5;
 *
 ****************************************************************/
bool FinanceMath::modifiedZScore(double *inArr, int arrLen, bool *isOutlierArr)
{
    double median;
    double mad;
    double zScore;

    const double maxLimit = 3.3; //3.5;
    const double minLimit = -3.3; //-3.5;

    if(false == getMedian(inArr, arrLen, median))
    {
        return false;
    }

    if(false == mAD(inArr, arrLen, mad))
    {
        return false;
    }

    if(mad == 0)
    {
        return false;
    }


    for(int i = 0; i < arrLen; i++)
    {
        zScore = 0.6745 * (inArr[i] - median) / mad;
        qDebug() << i << zScore;

        isOutlierArr[i] = false;
        if((zScore > maxLimit) || (zScore < minLimit))
        {
            isOutlierArr[i] = true;
        }
    }

    return true;

}



/****************************************************************
 *
 * Function:     MAD()
 *
 * Description:  Median absolute deviation. Find the absolute
 *               distance for each value to the median value.
 *
 *
 *              {=MEDIAN(ABS(MEDIAN(A1:A10)-A1:A10))}
 *
 *
 ****************************************************************/
bool FinanceMath::mAD(double *inArr, int arrLen, double &mad)
{
    double *tmpArr = new double [arrLen];
    double median;

    if(tmpArr == NULL)
    {
        return false;
    }


    getMedian(inArr,arrLen, median);

    for(int i = 0; i < arrLen; i++)
    {
        tmpArr[i] = absolute(median - inArr[i]);
    }

    getMedian(tmpArr,arrLen, mad);


    delete [] tmpArr;

    return true;

}



/****************************************************************
 *
 * Function:
 *
 * Description:
 *
 *
 ****************************************************************/
double FinanceMath::absolute(double value)
{
    if(value < 0)
        value = -value;

    return value;

}




/****************************************************************
 *
 * Function:
 *
 * Description:
 *
 *
 ****************************************************************/
bool FinanceMath::getMedian(double *inArr,int arrLen, double &median)
{
    double *tmpArr = new double [arrLen];

    if(tmpArr == NULL)
    {
        return false;
    }

    memcpy(tmpArr, inArr, (sizeof(double) * arrLen));

    sort(tmpArr, tmpArr + arrLen);

    // Odd number of data
    if((arrLen % 2) != 0)
    {
        median = tmpArr[(arrLen/2)];
    }
    else
    {
        median = ((tmpArr[(arrLen/2) -1] + tmpArr[arrLen/2])/2);
    }

    delete [] tmpArr;

    return true;
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

            singleEventGrowth(currData, prevData,  growth);

            // Convert to procent
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





/****************************************************************
 *
 * Function:    calcGrowth()
 *
 * Description: Calc single event growth()
 *
 *
 *
 ****************************************************************/
void FinanceMath::
singleEventGrowth(double curr, double prev,  double &result)
{
    double prevNumerator;       // Täljare
    double currNumerator;       // Täljare
    double prevDenominator;     // Nämnare

    currNumerator = curr;
    prevNumerator = prev;
    prevDenominator = prev;

    // Handle sign issues
    if((curr >= 0) && (prev < 0))
    {
        prevDenominator = -prevDenominator;
    }
    else if((curr < 0) && (prev < 0))
    {
           currNumerator = -currNumerator;
           prevNumerator = -prevNumerator;

           if(curr > prev)
           {
                  prevDenominator = -prevDenominator;
           }
    }


    // Handle divide by zero
    if(prevDenominator == 0)
    {
          prevDenominator = 0.0001;
    }

       result = ((currNumerator - prevNumerator) / prevDenominator);
}
