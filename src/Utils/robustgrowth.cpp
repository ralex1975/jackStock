#include "robustgrowth.h"
#include "financemath.h"
#include "util.h"

/****************************************************************
 *
 * Function:        RobustGrowth()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
RobustGrowth::RobustGrowth()
{

}


/****************************************************************
 *
 * Function:        calcAnualRobustGrowth()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
bool RobustGrowth::calcAnualRobustGrowth(SubAnalysDataST *inArr,
                                        int nofInArrData,
                                        SubAnalysDataST *trendlineArr,
                                        int &nofTrendlineArrData,
                                        int nofPredicitedYears,
                                        double &growthRate)
{
    double mean;
    double stdDev;
    CUtil cu;

    double k;
    double m;
    double minX;
    double maxX;

    double startYear;
    double startY;
    double endYear;
    double endY;
    int nofYears;


    nofTrendlineArrData = 0;
    if(nofInArrData < 2)
    {
        return false;
    }

    SubAnalysDataST *growthArr = new SubAnalysDataST [nofInArrData + 5];
    int nofGrowthArrData = 0;

    SubAnalysDataST *validSampleArr = new SubAnalysDataST [nofInArrData + 5];
    int nofValidSampleArrData = 0;

    // Calc growth rate between each sample
    if(false == calcGrowthWithOutliers(inArr,
                                nofInArrData,
                                growthArr,
                                nofGrowthArrData))
    {
        delete [] growthArr;
        delete [] validSampleArr;
        return false;
    }

    // Calc mean and standard deviation on growth rate sampes
    if(false == calcMeanAndStdDev(growthArr,
                      nofGrowthArrData,
                      mean,
                      stdDev))
    {
        delete [] growthArr;
        delete [] validSampleArr;
        return false;
    }

    // Remove extreme samples from input data
    if(false == removeOutliers(mean,
                               stdDev,
                               inArr,
                               nofInArrData,
                               growthArr,
                               nofGrowthArrData,
                               validSampleArr,
                               nofValidSampleArrData))
    {
        delete [] growthArr;
        delete [] validSampleArr;
        return false;
    }

    // On the reduced samples calc least square fit
    if(false == calcLeastSqrtFit(validSampleArr,
                                 nofValidSampleArrData,
                                 k,
                                 m,
                                 minX,
                                 maxX))
    {
        delete [] growthArr;
        delete [] validSampleArr;
        return false;
    }



    //----------------------------------------------------------------
    // Calc start and stop sampels based on least square fit calculation
    //----------------------------------------------------------------
    startYear = inArr[0].date.toDouble();
    startY = k * inArr[0].date.toDouble() + m;
    trendlineArr[0].date.sprintf("%.2f", startYear);
    trendlineArr[0].data.sprintf("%.2f", startY);

    // Calc end Y-value predicted including year in the feature
    endYear = inArr[nofInArrData-1].date.toDouble() + nofPredicitedYears;
    endY = k * (inArr[nofInArrData-1].date.toDouble() + nofPredicitedYears) + m;

    trendlineArr[1].date.sprintf("%.2f", endYear);
    trendlineArr[1].data.sprintf("%.2f", endY);
    nofTrendlineArrData = 2;

    // Growth should be positive
    if(k >= 0)
    {
        //----------------------------------------------------------------
        // Calc annual growth rate based on least square fit data
        //----------------------------------------------------------------

        // Calc end Y-value between actual data
        nofYears = (int)(maxX - minX);

        // startY cannot be negative
        for(int i = 0; i < nofYears; i++)
        {
            startYear = (minX + i);
            startY = k * (minX + i) + m;
            if(startY > 0/*>=1*/)
            {
                minX = (minX + i);
                break;
            }
        }

        // endY cannot be negative
        for(int i = 0; i < nofYears; i++)
        {
            endYear = (maxX - i);
            endY = k * (maxX -i) + m;

            if(endY > 0/*>=1*/)
            {
                maxX = (maxX - i);
                break;
            }
        }
        nofYears = (int)(maxX - minX);


        // We must have enough number of sampes
        if(nofYears < 2)
        {
            delete [] growthArr;
            delete [] validSampleArr;
            return false;
        }


        if(false == cu.annualGrowthRate(startY, endY, nofYears, growthRate))
        {
            delete [] growthArr;
            delete [] validSampleArr;
            return false;
        }

        // Lets last predicted growth rate (not so high) instead of the calculated value above
        growthRate = growthRate - 1;
        growthRate = growthRate * 100;
        qDebug() << "-----------------------------------";
        qDebug() << "startY" << startY;
        qDebug() << "endY" << endY;
        qDebug() << "growthRate" << growthRate;
        qDebug() << "k" << k;
        qDebug() << "-----------------------------------";

    }
    else
    {
        // We do not calc negative growth (just dummy value)
        growthRate = -10000;
    }

    delete [] growthArr;
    delete [] validSampleArr;
    return true;
}


/*******************************************************************
 *
 * Function:    calcLeastSqrtFit()
 *
 * Description: This calculate least square fit
 *
 *
 *
 *******************************************************************/
bool RobustGrowth::
calcLeastSqrtFit(SubAnalysDataST *inArr,
                 int nofInArrData,
                 double &k,
                 double &m,
                 double &minX,
                 double &maxX)
{
    CUtil cu;
    CDbHndl db;
    int nofData = 0;
    double meanXSum = 0;
    double meanYSum = 0;
    double prodXXSum = 0;
    double prodYYSum = 0;
    double prodXYSum = 0;
    double x;
    double y;
    bool minMaxXIsInit = false;


    db.init1dLeastSrq(nofData, meanXSum, meanYSum, prodXXSum, prodYYSum, prodXYSum);


    if(nofInArrData < 2)
    {
        return false;
    }

    for (int i = 0 ; i < nofInArrData; i++)
    {
        if(true == cu.number2double(inArr[i].date, x) &&
           true == cu.number2double(inArr[i].data, y))
        {
            qDebug() << "xx" << x << "yy" << y;
            db.gather1dLeastSrqData(x,
                                    y,
                                    nofData,
                                    meanXSum,
                                    meanYSum,
                                    prodXXSum,
                                    prodYYSum,
                                    prodXYSum);

            if(minMaxXIsInit == false)
            {
                minMaxXIsInit = true;
                maxX = x;
                minX = x;
            }
            else
            {
                if(minX > x)
                {
                    minX = x;
                }

                if(maxX < x)
                {
                    maxX = x;
                }
            }
        }
    }

    if(nofData < 2)
    {
        return false;
    }

    if(false == db.calc1dLeastSrqFitParams(nofData, meanXSum, meanYSum, prodXXSum, prodYYSum, prodXYSum, m, k))
    {
       return false;
    }

    qDebug() << "k" << k << "m" << m;

    return true;
}




/****************************************************************
 *
 * Function:        calcGrowthWithOutliers()
 *
 * Description:     Calc growth do not care if it is extream data points.
 *
 *
 *
 ****************************************************************/
bool RobustGrowth::calcGrowthWithOutliers(SubAnalysDataST *inArr,
                                              int nofInArrData,
                                              SubAnalysDataST *outArr,
                                              int &nofOutArrData)
{
    FinanceMath fm;

    nofOutArrData = nofInArrData;
    if(false == fm.calcGrowth(inArr, nofInArrData, outArr))
    {
        nofOutArrData = 0;
        return false;
    }

    return true;
}



/****************************************************************
 *
 * Function:        calcMeanAndStdDev()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
bool RobustGrowth::calcMeanAndStdDev(SubAnalysDataST *inArr,
                                         int nofInArrData,
                                         double &mean,
                                         double &stdDev)
{
    CUtil cu;

   if(nofInArrData < 2)
    {
        return false;
    }

    double *inputDataArr = new double [nofInArrData + 5];

    for(int i = 1; i < nofInArrData; i++)
    {
        inputDataArr[i-1] = inArr[i].data.toDouble();
    }

    cu.calcMeanAndStdDeviation(inputDataArr,
                               (nofInArrData - 1),
                               mean,
                               stdDev);


    delete [] inputDataArr;

    return true;
}



#if 0 // modifiedZScore
/****************************************************************
 *
 * Function:        removeOutliers()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
bool RobustGrowth::removeOutliers(double mean,
                                      double stdDev,
                                      SubAnalysDataST *inSampelArr,
                                      int nofInSampleArrData,
                                      SubAnalysDataST *inGrowtDataArr,
                                      int nofInGrowtArrData,
                                      SubAnalysDataST *outSampleArr,
                                      int &nofOutSampleArrData)
{
    FinanceMath fm;

    if(nofInSampleArrData != nofInGrowtArrData)
    {
        return false;
    }

    double *dataArr  = new double [nofInSampleArrData];
    bool *isOutlierArr = new bool [nofInSampleArrData];

    if(isOutlierArr == NULL)
    {
        return NULL;
    }

    for(int i = 0; i < nofInSampleArrData; i++)
    {
        dataArr[i] = inSampelArr[i].data.toDouble();
    }

    if(false == fm.modifiedZScore(dataArr, nofInSampleArrData, isOutlierArr))
    {
        delete [] dataArr;
        delete [] isOutlierArr;
        return false;
    }

    for(int i = 0; i < nofInSampleArrData; i++)
    {
        qDebug() << "---------------";
        qDebug() << isOutlierArr[i];
        qDebug() << inSampelArr[i].data;
        qDebug() << "---------------";
    }


    nofOutSampleArrData = 0;
    for(int i = 0; i < nofInGrowtArrData; i++)
    {
        //inputGrowth = inGrowtDataArr[i].data.toDouble();

        if(isOutlierArr[i] == false)
        {
            outSampleArr[nofOutSampleArrData].date = inSampelArr[i].date;
            outSampleArr[nofOutSampleArrData].data = inSampelArr[i].data;

            nofOutSampleArrData++;
        }
    }

    delete [] dataArr;
    delete [] isOutlierArr;

    return true;
}
#endif


#if 1
/****************************************************************
 *
 * Function:        removeOutliers()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
bool RobustGrowth::removeOutliers(double mean,
                                      double stdDev,
                                      SubAnalysDataST *inSampelArr,
                                      int nofInSampleArrData,
                                      SubAnalysDataST *inGrowtDataArr,
                                      int nofInGrowtArrData,
                                      SubAnalysDataST *outSampleArr,
                                      int &nofOutSampleArrData)
{
    double maxLimit = mean + (1.7 * stdDev);
    double minLimit = mean - (1.7 * stdDev);
    double inputGrowth;

    if(nofInSampleArrData != nofInGrowtArrData)
    {
        return false;
    }

    nofOutSampleArrData = 0;
    for(int i = 1; i < nofInGrowtArrData; i++)
    {
        inputGrowth = inGrowtDataArr[i].data.toDouble();

        if((inputGrowth >= minLimit) &&
           (inputGrowth <= maxLimit))
        {
            if(i == 1)
            {
                outSampleArr[nofOutSampleArrData].date = inSampelArr[0].date;
                outSampleArr[nofOutSampleArrData].data = inSampelArr[0].data;
                nofOutSampleArrData++;

                outSampleArr[nofOutSampleArrData].date = inSampelArr[1].date;
                outSampleArr[nofOutSampleArrData].data = inSampelArr[1].data;
                nofOutSampleArrData++;
            }
            else
            {
                outSampleArr[nofOutSampleArrData].date = inSampelArr[i].date;
                outSampleArr[nofOutSampleArrData].data = inSampelArr[i].data;
                nofOutSampleArrData++;
            }
        }
    }
    return true;
}
#endif
