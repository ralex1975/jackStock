/******************************************************************
 *
 * Filename:        subanalysisdisplaygraphdata.cpp
 *
 * Description:
 *
 *
 *
 *
 *****************************************************************/


#include "subanalysisdisplaygraphdata.h"

/******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 *****************************************************************/
subAnalysisDisplayGraphData::subAnalysisDisplayGraphData()
{
    int i, j;

    for(i = 0; i < NOF_QWT_PLOTS; i++)
    {
        m_barHist[i] = new QwtPlotHistogram;

        for(j = 0; j < MAX_NOF_QWT_MARKERS_IN_EACH_PLOT; j++)
        {
            m_mark[i][j] = new QwtPlotMarker;
        }
    }
}



/******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 *****************************************************************/
subAnalysisDisplayGraphData:: ~subAnalysisDisplayGraphData()
{
    int i, j ;

    for(i = 0; i < NOF_QWT_PLOTS; i++)
    {
        delete m_barHist[i];

        for(j = 0; j < MAX_NOF_QWT_MARKERS_IN_EACH_PLOT; j++)
        {
            delete m_mark[i][j];
        }
    }
}


/******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 *****************************************************************/
void subAnalysisDisplayGraphData::
subAnalysisCalcRation(SubAnalysDataST *numeratorArr,
                      int nofNumeratorArrData,
                      SubAnalysDataST *denominatorArr,
                      int nofDenominatorData,
                      SubAnalysDataST *resultArr,
                      int &nofResultArrData)
{
    double denominator;

    nofResultArrData = 0;

    if(nofNumeratorArrData == nofDenominatorData)
    {
        nofResultArrData = nofNumeratorArrData;

        for(int i = 0; i < nofNumeratorArrData; i++)
        {
            // Do not divide with zero
            denominator = denominatorArr[i].data.toDouble();
            if(denominator == 0)
            {
                denominator = 0.001;
            }


            if(numeratorArr[i].date.toDouble() == denominatorArr[i].date.toDouble())
            {
                resultArr[i].date = numeratorArr[i].date;

                resultArr[i].data.sprintf("%.2f", numeratorArr[i].data.toDouble() / denominator);
            }
        }
    }
}



/******************************************************************
 *
 * Function:    ()
 *
 * Description:  out[i] = arr[i-1] / arr[i]; out[0] is set to 0
 *
 *
 *
 *
 *****************************************************************/
void subAnalysisDisplayGraphData::
subAnalysisOneArrCalcRationPrevCurrSlot(SubAnalysDataST *inputArr,
                                        int nofInputArrData,
                                        SubAnalysDataST *resultArr,
                                        int &nofResultArrData)
{
    nofResultArrData = nofInputArrData;

    if(nofInputArrData > 1)
    {
        resultArr[0].date = inputArr[0].date;

        resultArr[0].data.sprintf("%.2f", 0.0);

        for(int i = 1; i < nofInputArrData; i++)
        {
            // Do not divide with zero
            double denominator = inputArr[i].data.toDouble();
            if(denominator == 0)
                denominator = 0.001;

            resultArr[i].date = inputArr[i].date;

            resultArr[i].data.sprintf("%.2f", inputArr[i-1].data.toDouble() / denominator);
        }
    }
}



/******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 *****************************************************************/
void subAnalysisDisplayGraphData::
subAnalysisGetCompanyType(HtmlStockAnalysPageDataST data, int &companyType)
{
    CDbHndl db;
    int mainAnalysisId;


    companyType = 0;

    // Check company type
    if(true == db.mainAnalysisDataExists(data.stockName, data.stockSymbol, mainAnalysisId))
    {
        if(true ==  db.getSubAnalysisCompanyType(mainAnalysisId, companyType))
        {
            if(companyType >= NOF_COMPANY_TYPE_ARR_DATA)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("Error"), QString::fromUtf8("Error companyType >= NOF_COMPANY_TYPE_ARR_DATA"));
                companyType = 0;
            }
        }
    }
}




/******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 *****************************************************************/
void subAnalysisDisplayGraphData::
subAnalysisPlotGraphicDataTradingComp(HtmlStockAnalysPageDataST data, QwtPlot *qwtPlot[NOF_QWT_PLOTS])
{
    subAnalysisPlotGraphicDataIndustrialComp(data, qwtPlot);

}


/******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 *****************************************************************/
void subAnalysisDisplayGraphData::
subAnalysisPlotGraphicDataUtilitisComp(HtmlStockAnalysPageDataST data, QwtPlot *qwtPlot[NOF_QWT_PLOTS])
{
    int graphIndex = 0;
    int companyType;
    SubAnalysDataST tmpRatioArr[MAX_NOF_TOTAL_CURRENT_ASSETS_ARR_DATA];
    int nofResultArrData;


    subAnalysisGetCompanyType(data, companyType);


    graphIndex= 0;
    plotBarGraph(graphIndex, data.nofTotalLiabilitiesData, data.totalLiabilitiesArr, qwtPlot);

    graphIndex= 1;
    plotBarGraph(graphIndex, data.nofEquityData, data.equityArr, qwtPlot);


    // Calc ratio TotalLiabilities / Equity
    subAnalysisCalcRation(data.totalLiabilitiesArr,
                          data.nofTotalLiabilitiesData,
                          data.equityArr,
                          data.nofEquityData,
                          tmpRatioArr,
                          nofResultArrData);

    graphIndex= 2;
    plotBarGraph(graphIndex, nofResultArrData, tmpRatioArr, qwtPlot);

    graphIndex= 3;
    plotBarGraph(graphIndex, data.nofSolidityData, data.solidityArr, qwtPlot);

    graphIndex= 4;
    plotBarGraph(graphIndex, data.nofCoverageRatioData, data.coverageRatioArr, qwtPlot);


    graphIndex= 6;
    plotBarGraph(graphIndex, data.nofEarningsArrData, data.earningsDataArr, qwtPlot);


    subAnalysisOneArrCalcRationPrevCurrSlot(data.earningsDataArr,
                                            data.nofEarningsArrData,
                                            tmpRatioArr,
                                            nofResultArrData);

    // Ratio prev erarnings / current earnings
    graphIndex= 7;
    plotBarGraph(graphIndex, nofResultArrData, tmpRatioArr, qwtPlot);


    graphIndex = 9;
    plotBarGraph(graphIndex, data.nofDividendArrData, data.dividendDataArr, qwtPlot);

}



/******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 *****************************************************************/
void subAnalysisDisplayGraphData::
subAnalysisPlotGraphicDataRealEstateComp(HtmlStockAnalysPageDataST data, QwtPlot *qwtPlot[NOF_QWT_PLOTS])
{
    int graphIndex = 0;
    int companyType;
    SubAnalysDataST tmpRatioArr[MAX_NOF_TOTAL_CURRENT_ASSETS_ARR_DATA];
    int nofResultArrData;


    subAnalysisGetCompanyType(data, companyType);


    graphIndex= 0;
    plotBarGraph(graphIndex, data.nofSolidityData, data.solidityArr, qwtPlot);

    graphIndex= 3;
    plotBarGraph(graphIndex, data.nofCoverageRatioData, data.coverageRatioArr, qwtPlot);


    graphIndex= 6;
    plotBarGraph(graphIndex, data.nofEarningsArrData, data.earningsDataArr, qwtPlot);


    subAnalysisOneArrCalcRationPrevCurrSlot(data.earningsDataArr,
                                            data.nofEarningsArrData,
                                            tmpRatioArr,
                                            nofResultArrData);

    // Ratio prev erarnings / current earnings
    graphIndex= 7;
    plotBarGraph(graphIndex, nofResultArrData, tmpRatioArr, qwtPlot);


    graphIndex = 9;
    plotBarGraph(graphIndex, data.nofDividendArrData, data.dividendDataArr, qwtPlot);

}


/******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 *****************************************************************/
void subAnalysisDisplayGraphData::
subAnalysisPlotGraphicDataBankComp(HtmlStockAnalysPageDataST data, QwtPlot *qwtPlot[NOF_QWT_PLOTS])
{
    int graphIndex = 0;
    int companyType;
    SubAnalysDataST tmpRatioArr[MAX_NOF_TOTAL_CURRENT_ASSETS_ARR_DATA];
    int nofResultArrData;


    subAnalysisGetCompanyType(data, companyType);


    graphIndex= 0;
    plotBarGraph(graphIndex, data.nofCoreCapitalRatioData, data.coreCapitalRatioArr, qwtPlot);


    graphIndex= 3;
    plotBarGraph(graphIndex, data.nofEarningsArrData, data.earningsDataArr, qwtPlot);


    subAnalysisOneArrCalcRationPrevCurrSlot(data.earningsDataArr,
                                            data.nofEarningsArrData,
                                            tmpRatioArr,
                                            nofResultArrData);

    // Ratio prev erarnings / current earnings
    graphIndex= 4;
    plotBarGraph(graphIndex, nofResultArrData, tmpRatioArr, qwtPlot);


    graphIndex = 6;
    plotBarGraph(graphIndex, data.nofDividendArrData, data.dividendDataArr, qwtPlot);

}




/******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 *****************************************************************/
void subAnalysisDisplayGraphData::
subAnalysisPlotGraphicDataIndustrialComp(HtmlStockAnalysPageDataST data, QwtPlot *qwtPlot[NOF_QWT_PLOTS])
{
    int graphIndex = 0;
    int companyType;
    SubAnalysDataST tmpRatioArr[MAX_NOF_TOTAL_CURRENT_ASSETS_ARR_DATA];
    int nofResultArrData;


    subAnalysisGetCompanyType(data, companyType);


    graphIndex= 0;
    plotBarGraph(graphIndex, data.nofTotalCurrentAssetsArrData, data.totalCurrentAssetsArr, qwtPlot);

    graphIndex= 1;
    plotBarGraph(graphIndex, data.nofTotalCurrentLiabilitiesData, data.totalCurrentLiabilitiesArr, qwtPlot);


    // Ratio CurrentAssets / TotalCurrentLiabilities
    subAnalysisCalcRation(data.totalCurrentAssetsArr,
                          data.nofTotalCurrentAssetsArrData,
                          data.totalCurrentLiabilitiesArr,
                          data.nofTotalCurrentLiabilitiesData,
                          tmpRatioArr,
                          nofResultArrData);

    graphIndex= 2;
    plotBarGraph(graphIndex, nofResultArrData, tmpRatioArr, qwtPlot);

    graphIndex= 3;
    plotBarGraph(graphIndex, data.nofTotalCurrentAssetsArrData, data.totalCurrentAssetsArr, qwtPlot);

    graphIndex= 4;
    plotBarGraph(graphIndex, data.nofTotalLiabilitiesData, data.totalLiabilitiesArr, qwtPlot);


    // Ratio CurrentAssets / TotalLiabilities
    subAnalysisCalcRation(data.totalCurrentAssetsArr,
                          data.nofTotalCurrentAssetsArrData,
                          data.totalLiabilitiesArr,
                          data.nofTotalLiabilitiesData,
                          tmpRatioArr,
                          nofResultArrData);

    graphIndex= 5;
    plotBarGraph(graphIndex, nofResultArrData, tmpRatioArr, qwtPlot);


    graphIndex= 6;
    plotBarGraph(graphIndex, data.nofEarningsArrData, data.earningsDataArr, qwtPlot);


    subAnalysisOneArrCalcRationPrevCurrSlot(data.earningsDataArr,
                                            data.nofEarningsArrData,
                                            tmpRatioArr,
                                            nofResultArrData);

    // Ratio prev erarnings / curr erarnings
    graphIndex= 7;
    plotBarGraph(graphIndex, nofResultArrData, tmpRatioArr, qwtPlot);

    graphIndex = 9;
    plotBarGraph(graphIndex, data.nofDividendArrData, data.dividendDataArr, qwtPlot);

}



/******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 *****************************************************************/
void subAnalysisDisplayGraphData::
subAnalysisShowDataInGraphs(HtmlStockAnalysPageDataST data, QwtPlot *qwtPlot[NOF_QWT_PLOTS])
{
    int companyType;

    subAnalysisClearAllGraphs(qwtPlot);

    subAnalysisGetCompanyType(data, companyType);

    switch(companyType)
    {
    // Okänd
    case UNKNOW_COMPANY_TYPE:

        // Detta är fel fixa detta
        subAnalysisPlotGraphicDataIndustrialComp(data, qwtPlot);
        break;

    // Handelsföretag
    case TRADING_COMPANY:
        subAnalysisPlotGraphicDataTradingComp(data, qwtPlot);
        break;

    // Industriföretag
    case INDUSTRIALS_COMPANY:
        subAnalysisPlotGraphicDataIndustrialComp(data, qwtPlot);
        break;

    // Bank
    case BANK_COMPANY:
        subAnalysisPlotGraphicDataBankComp(data, qwtPlot);
        break;

    // Fastighetsbolag
    case REAL_ESTATE_COMPANY:
        subAnalysisPlotGraphicDataRealEstateComp(data, qwtPlot);
        break;

    // Allmännyttiga företag, Försörjningsbolag
    case UTILITIS_COMPANY:
        subAnalysisPlotGraphicDataUtilitisComp(data, qwtPlot);
        break;

    // Investmentbolag
    case INVESTMENT_TRUST_COMPANY:
        // Fixa detta
        subAnalysisPlotGraphicDataRealEstateComp(data, qwtPlot);
        break;

    // Ska tas bort, Allmännyttiga företag, Försörjningsbolag
    case UTILITIS_COMPANY_2:
        subAnalysisPlotGraphicDataUtilitisComp(data, qwtPlot);
        break;

    default:
        QMessageBox::critical(NULL, QString::fromUtf8("Error"), QString::fromUtf8("Error: Invalid company type"));
        return;
    }
}





//txt.setFont( QFont( "Helvetica", 14, QFont::Bold) );
//txt.setColor( QColor(Qt::black));
//txt.setBackgroundBrush(QColor(Qt::white));

/******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 *****************************************************************/
void subAnalysisDisplayGraphData::subAnalysisClearAllGraphs(QwtPlot *qwtPlot[NOF_QWT_PLOTS])
{

    QwtText txt;
    QString lableText = " ";

    txt.setText(lableText);

    for(int i = 0; i < NOF_QWT_PLOTS; i++)
    {
        m_barHistData[i].clear();
        m_barHist[i]->detach();
        m_barHist[i]->setData(NULL);

        for(int j = 0; j < MAX_NOF_QWT_MARKERS_IN_EACH_PLOT; j++)
        {
            m_mark[i][j]->setLabel(txt);
            m_mark[i][j]->detach();
        }

        qwtPlot[i]->replot();
    }

}

/******************************************************************
 *
 * Function:    plotBarGraph()
 *
 * Description:
 *
 *
 *
 *
 *****************************************************************/
void subAnalysisDisplayGraphData::plotBarGraph(int graphIndex,
                                               int nofArrData,
                                               SubAnalysDataST *subAnalysDataArr,
                                               QwtPlot *qwtPlot[NOF_QWT_PLOTS])
{
    QwtText txt;
    QString lableText;

    double date;
    double data;

    double xMin;
    double xMax;

    double yMin;
    double yMax;


    if((nofArrData < 1) || (graphIndex > 9))
    {
        return;
    }

    // Clear graph
    m_barHistData[graphIndex].clear();
    m_barHist[graphIndex]->detach();
    m_barHist[graphIndex]->setData(NULL);
    m_barHist[graphIndex]-> setStyle (QwtPlotHistogram :: Columns);


    for(int i = 0; i < nofArrData; i++)
    {
        date = subAnalysDataArr[i].date.toDouble();
        data = subAnalysDataArr[i].data.toDouble();

        if(i == 0)
        {
           xMin = date;
           xMax = date;

           yMin = data;
           yMax = data;

        }
        else
        {
            if(xMin > (date))
            {
                xMin = (date + 0.5);
            }

            if(xMax < (date + 1.0))
            {
                xMax = date + 1.0;
            }


            if(yMin > data)
            {
                yMin = (data);
            }

            if(yMax < (data + 1.0))
            {
                yMax = (data + 1.0);
            }

        }


        // Insert data value at the top of the bar
        lableText.clear();
        lableText.sprintf("%.2f", data);
        txt.setText(lableText);

        m_mark[graphIndex][i]->detach();
        m_mark[graphIndex][i]->setLabel(txt);
        m_mark[graphIndex][i]->setValue((double)(date+0.5),(double)(data+0.5)); //here you have to set the coordinate axis i.e. where the axis are meeting.
        m_mark[graphIndex][i]->attach(qwtPlot[graphIndex]);

        // Add data to graph
        QwtInterval interval(date, date + 1);
        interval.setBorderFlags( QwtInterval::ExcludeMaximum | QwtInterval::ExcludeMinimum);
        m_barHistData[graphIndex].append (QwtIntervalSample(data, interval));
    }

    // qwtPlot[graphIndex]->enableAxis(QwtPlot::xBottom, false);
    // qwtPlot[graphIndex]->enableAxis(QwtPlot::yLeft, false);

    if(yMin > 0)
        yMin = 0;

    //qwtPlot[graphIndex]->setAxisAutoScale(QwtPlot::yLeft, true);
    qwtPlot[graphIndex]->setAxisScale(QwtPlot::yLeft, yMin, yMax);
//    qwtPlot[graphIndex]->setAxisScale(QwtPlot::xBottom, xMin, xMax);



    // Display graph data
    m_barHist[graphIndex]->setBrush(Qt::blue);
    m_barHist[graphIndex]->setPen(QPen(Qt::black));
    m_barHist[graphIndex]->setSamples(m_barHistData[graphIndex]);
    m_barHist[graphIndex]->attach(qwtPlot[graphIndex]);
    qwtPlot[graphIndex]->replot();

}
