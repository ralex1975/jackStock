#include "subanalysisdisplaygraphdata.h"

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




void subAnalysisDisplayGraphData::subAnalysisShowDataInGraphs(HtmlStockAnalysPageDataST data, QwtPlot *qwtPlot[NOF_QWT_PLOTS])
{
    int graphIndex = 0;
    CDbHndl db;
    int mainAnalysisId;
    int companyType = 0;
    SubAnalysDataST tmpRatioArr[MAX_NOF_TOTAL_CURRENT_ASSETS_ARR_DATA];



    // double m_y[1000];
    //QwtText txt;
    //QString lableText;



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


    graphIndex= 0;
    plotBarGraph(graphIndex, data.nofTotalCurrentAssetsArrData, data.totalCurrentAssetsArr, qwtPlot);

    graphIndex= 1;
    plotBarGraph(graphIndex, data.nofTotalCurrentLiabilitiesData, data.totalCurrentLiabilitiesArr, qwtPlot);

    if(data.nofTotalCurrentAssetsArrData == data.nofTotalCurrentLiabilitiesData)
    {
        for(int i = 0; i < data.nofTotalCurrentAssetsArrData; i++)
        {
            if(data.totalCurrentAssetsArr[i].date.toDouble() == data.totalCurrentLiabilitiesArr[i].date.toDouble())
            {
                tmpRatioArr[i].date = data.totalCurrentAssetsArr[i].date;

                tmpRatioArr[i].data.sprintf("%.2f", data.totalCurrentAssetsArr[i].data.toDouble() /
                                 data.totalCurrentLiabilitiesArr[i].data.toDouble());
            }
        }
    }

    // Ratio CurrentAssets / TotalCurrentLiabilities
    graphIndex= 2;
    plotBarGraph(graphIndex, data.nofTotalCurrentLiabilitiesData, tmpRatioArr, qwtPlot);


    graphIndex= 3;
    plotBarGraph(graphIndex, data.nofTotalCurrentAssetsArrData, data.totalCurrentAssetsArr, qwtPlot);

    graphIndex= 4;
    plotBarGraph(graphIndex, data.nofTotalLiabilitiesData, data.totalLiabilitiesArr, qwtPlot);


    if(data.nofTotalCurrentAssetsArrData == data.nofTotalCurrentLiabilitiesData)
    {
        for(int i = 0; i < data.nofTotalCurrentAssetsArrData; i++)
        {
            if(data.totalCurrentAssetsArr[i].date.toDouble() == data.totalLiabilitiesArr[i].date.toDouble())
            {
                tmpRatioArr[i].date = data.totalCurrentAssetsArr[i].date;

                tmpRatioArr[i].data.sprintf("%.2f", data.totalCurrentAssetsArr[i].data.toDouble() /
                                 data.totalLiabilitiesArr[i].data.toDouble());
            }
        }
    }

    // Ratio CurrentAssets / TotalLiabilities
    graphIndex= 5;
    plotBarGraph(graphIndex, data.nofTotalLiabilitiesData, tmpRatioArr, qwtPlot);


    graphIndex= 6;
    plotBarGraph(graphIndex, data.nofEarningsArrData, data.earningsDataArr, qwtPlot);


    if(data.nofEarningsArrData > 1)
    {
        tmpRatioArr[0].date = data.earningsDataArr[0].date;

        tmpRatioArr[0].data.sprintf("%.2f", 0);

        for(int i = 1; i < data.nofEarningsArrData; i++)
        {
            tmpRatioArr[i].date = data.earningsDataArr[i].date;

            tmpRatioArr[i].data.sprintf("%.2f", data.earningsDataArr[i].data.toDouble() /
                             data.earningsDataArr[i-1].data.toDouble());
    }

        // Ratio CurrentAssets / TotalLiabilities
        graphIndex= 7;
        plotBarGraph(graphIndex, data.nofEarningsArrData, tmpRatioArr, qwtPlot);
    }

    graphIndex = 9;
    plotBarGraph(graphIndex, data.nofDividendArrData, data.dividendDataArr, qwtPlot);



#if 0
    switch(companyType)
    {
    case UNKNOW_COMPANY_TYPE:			// Okänd
        break;

    case TRADING_COMPANY:                // handelsföretag
        if(data.nofTotalCurrentAssetsArrData > 0)
        {

        }

        if(data.nofTotalCurrentLiabilitiesData > 0)
        {

        }

        if(data.nofTotalLiabilitiesData > 0)
        {

        }

        break;

    case INDUSTRIALS_COMPANY:            // Industriföretag
        break;

    case BANK_COMPANY:					// Bank
        break;

    case REAL_ESTATE_COMPANY:	    	// Fastighetsbolag
        break;

    case UTILITIS_COMPANY:               // Allmännyttiga företag, Försörjningsbolag
        break;

    case INVESTMENT_TRUST_COMPANY:    	// Investmentbolag
        break;

    case UTILITIS_COMPANY_2:             // Ska tas bort, Allmännyttiga företag, Försörjningsbolag
        break;

    default:
        QMessageBox::Critical(NULL, QString::fromUtf8("Error"), QString::fromUtf8("Error: Invalid company type"));
        return;
    }



    //data.


    // ajn
    m_barHistData[0].clear();
    m_barHist[0]->detach();
    m_barHist[0]->setData(NULL);
    m_barHist[0]-> setStyle (QwtPlotHistogram :: Columns);
    //m_mark[0]->detach();

    for(int i = 0; i < 10; i++)
    {
        lableText.clear();

         lableText.sprintf("%d", i+j);
         txt.setText(lableText);
         m_mark[0][i]->setLabel(txt);
         m_mark[0][i]->setValue((double)(i+0.5),(double)(i+0.5));//here you have to set the coordinate axis i.e. where the axis are meeting.
         m_mark[0][i]->attach(qwtPlot[0]);

        //txt.setFont( QFont( "Helvetica", 14, QFont::Bold) );
        //txt.setColor( QColor(Qt::black));
        //txt.setBackgroundBrush(QColor(Qt::white));


        m_y[i] = (double) i + j;
        QwtInterval interval(i, i + 1);
        interval.setBorderFlags( QwtInterval::ExcludeMaximum | QwtInterval::ExcludeMinimum);
         //double diff = (m_y[i]-m_y[i-1]);
        m_barHistData[0].append (QwtIntervalSample(m_y[i], interval));
        // qDebug() << m_y[i-1] << m_y[i] << "diff" << diff;
    }

    qwtPlot[0]->enableAxis(QwtPlot::xBottom, false);
    qwtPlot[0]->enableAxis(QwtPlot::yLeft, false);
    m_barHist[0]->setBrush(Qt::blue);
    m_barHist[0]->setPen(QPen(Qt::black));
    m_barHist[0]->setSamples(m_barHistData[0]);
    m_barHist[0]->attach(qwtPlot[0]);
    qwtPlot[0]->replot();

#endif
    //j++;

#if 0

    double m_x[1000];
    double m_y[1000];
    m_barHist = new QwtPlotHistogram();

     QwtPlotMarker m_markers[10];



    // ajn
    m_barHistData.clear();
    m_barHist->detach();
    m_barHist->setData(NULL);
    m_barHist-> setStyle (QwtPlotHistogram :: Columns);

    for(int i = 0; i < 10; i++)
    {
        QwtText txt;
        QString lableText;

        //QwtSymbol *sym=new QwtSymbol(QwtSymbol::Diamond,QBrush(Qt::red),QPen(Qt::red),QSize(50,50));
        QwtPlotMarker *mark = new QwtPlotMarker;
         //mark->setSymbol(sym);
         lableText.sprintf("%d", i);
         txt.setText(lableText);
         mark->setLabel(txt);
         mark->setValue((double)(i+0.5),(double)(i+0.5));//here you have to set the coordinate axis i.e. where the axis are meeting.
         mark->attach(ui->qwtPlot_2);


        lableText.sprintf("Hej hopp %d", i);
        txt.setText(lableText);

        txt.setFont( QFont( "Helvetica", 14, QFont::Bold) );
        txt.setColor( QColor(Qt::black));
        txt.setBackgroundBrush(QColor(Qt::white));


        m_y[i] = (double) i;
        m_markers[i].setLabel(txt);
        m_markers[i].setValue((double)i, (double)i);
        m_markers[i].attach(ui->qwtPlot_2);
        //m_markers
        QwtInterval interval(i, i + 1);
        interval.setBorderFlags( QwtInterval::ExcludeMaximum | QwtInterval::ExcludeMinimum);
         double diff = (m_y[i]-m_y[i-1]);
        m_barHistData.append (QwtIntervalSample(m_y[i], interval));
        qDebug() << m_y[i-1] << m_y[i] << "diff" << diff;
    }


    ui->qwtPlot_2->enableAxis(QwtPlot::xBottom, false);
    ui->qwtPlot_2->enableAxis(QwtPlot::yLeft, false);
    m_barHist->setBrush(Qt::blue);
    m_barHist->setPen(QPen(Qt::black));
    m_barHist->setSamples(m_barHistData);
    m_barHist->attach(ui->qwtPlot_2);
    ui->qwtPlot_2->replot();
#endif

}


//txt.setFont( QFont( "Helvetica", 14, QFont::Bold) );
//txt.setColor( QColor(Qt::black));
//txt.setBackgroundBrush(QColor(Qt::white));


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
