#ifndef RobustGrowth_H
#define RobustGrowth_H

#include "dbHndl.h"

class RobustGrowth
{
private:
    bool calcGrowthWithOutliers(SubAnalysDataST *inArr,
                                int nofInArrData,
                                SubAnalysDataST *outArr,
                                int &nofOutArrData);

    bool removeOutliers(double mean,
                        double stdDev,
                        SubAnalysDataST *inSampelArr,
                        int nofInSampleArrData,
                        SubAnalysDataST *inGrowtDataArr,
                        int nofInGrowtArrData,
                        SubAnalysDataST *outSampleArr,
                        int &nofOutSampleArrData);

    bool calcMeanAndStdDev(SubAnalysDataST *inArr,
                           int nofInArrData,
                           double &mean,
                           double &stdDev);

    bool calcLeastSqrtFit(SubAnalysDataST *inArr,
                     int nofInArrData,
                     double &k,
                     double &m,
                     double &minX,
                     double &maxX);


public:
    RobustGrowth();
    bool calcAnualRobustGrowth(SubAnalysDataST *inArr,
                               int nofInArrData,
                               SubAnalysDataST *trendlineArr,
                               int &nofTrendlineArrData,
                               int nofPredicitedYears,
                               double &growthRate);



};

#endif // RobustGrowth_H
