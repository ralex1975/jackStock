#-------------------------------------------------
#
# Project created by QtCreator 2012-10-13T15:52:48
#
#-------------------------------------------------

CONFIG   += qwt

LIBS += -L/home/ajn/qwt-6.0.2/lib -lqwt
LIBS += -lcurl

DEPENDPATH += /home/ajn/qwt-6.0.2/lib

INCLUDEPATH +=/home/ajn/qwt-6.0.2/include

TARGET = curl


INCLUDEPATH += $$PWD/inc/GetHtmlPage
INCLUDEPATH += $$PWD/inc/guiUtil
INCLUDEPATH += $$PWD/inc/LibCurl
INCLUDEPATH += $$PWD/inc/Utils
INCLUDEPATH += $$PWD/inc/debug
INCLUDEPATH += $$PWD/inc/MyOs
INCLUDEPATH += $$PWD/inc/QtLib
INCLUDEPATH += $$PWD/inc/QwtLib
INCLUDEPATH += $$PWD/inc/SqlDb
INCLUDEPATH += $$PWD/inc/TaBuySellIdicator
INCLUDEPATH += $$PWD/Help/src
INCLUDEPATH += $$PWD/inc/Application
INCLUDEPATH += $$PWD/inc/Application/Tabs/1_Web
INCLUDEPATH += $$PWD/inc/Application/Tabs/2_KeyNumber1
INCLUDEPATH += $$PWD/inc/Application/Tabs/3_TechnicalAnalysis
INCLUDEPATH += $$PWD/inc/Application/Tabs/4_FundamentalAnalysis
INCLUDEPATH += $$PWD/inc/Application/Tabs/5_TechFundAnalysis
INCLUDEPATH += $$PWD/inc/Application/Tabs/6_PortfolioOptimizer
INCLUDEPATH += $$PWD/inc/Application/Tabs/7_ImportData
INCLUDEPATH += $$PWD/inc/Application/Tabs/8_Notes
INCLUDEPATH += $$PWD/inc/Application/Tabs/9_KeyNumber2
INCLUDEPATH += $$PWD/inc/Application/Tabs/10_KeyNumber3
INCLUDEPATH += $$PWD/inc/Application/Tabs/11_PortfolioResult
INCLUDEPATH += $$PWD/inc/Application/Tabs/12_Analysis
INCLUDEPATH += $$PWD/inc/Application/Tabs/13_video
INCLUDEPATH += $$PWD/inc/Application/Tabs/14_others




#QT       += core gui webkitwidgets
QT       += core gui
QT       += network
QT	 += webkit
QT       += sql


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = JackStock
TEMPLATE = app


SOURCES += 	\
		src/TaBuySellIdicator/taBuySellIdicator.cpp \
    		src/TaBuySellIdicator/taBuySellTask.cpp \
		src/debug/debug.cpp \
                src/guiUtil/guiUtils.cpp \
                src/guiUtil/guiFinanceColor.cpp \
    		src/guiUtil/guiFinanceCtrls.cpp \
    		src/guiUtil/myTreeWidget.cpp \
    		src/guiUtil/myLineEdit.cpp \
                src/MyOS/myOs.cpp \
                src/MyOS/mutex.cpp \
                src/MyOS/semaphore.cpp \
                src/MyOS/mytaskQue.cpp \
		src/QtLib/extendedLineEdit.cpp \
   		src/QtLib/extendedTable.cpp \
  		src/QtLib/findfiledialog.cpp \
                src/QwtLib/extendedQwtPlot.cpp \
                src/QwtLib/canvaspicker.cpp \
		src/SqlDb/dbHndl.cpp \
		src/SqlDb/dbSubHndl.cpp \
                src/SqlDb/dbvideohelp.cpp \
		src/Utils/fifo.cpp \
    		src/Utils/matrix.cpp \
		src/Utils/util.cpp \
		src/Utils/taskQue.cpp \
    		src/Utils/linkedList.cpp\
		src/Application/assistant.cpp \
                src/Application/main.cpp \
		src/Application/selIndicatorsDlg.cpp \
		src/Application/stockPlotUtil.cpp \
		src/Application/tabdialog.cpp \
    		src/Application/textedit.cpp \
    		src/Application/yahooStockPlotUtil.cpp \
    		src/Application/Tabs/1_Web/WebBrowserTab.cpp \
                src/Application/Tabs/2_KeyNumber1/parsesnapshotdata.cpp \
                src/Application/Tabs/2_KeyNumber1/parsePriceData.cpp \
                src/Application/Tabs/2_KeyNumber1/parseYieldData.cpp \
                src/Application/Tabs/2_KeyNumber1/parseKeyNumberData.cpp \
                src/Application/Tabs/2_KeyNumber1/tableTab.cpp \
                src/Application/Tabs/3_TechnicalAnalysis/taAnalysis.cpp \
                src/Application/Tabs/3_TechnicalAnalysis/importYahooTaData.cpp \
		src/Application/Tabs/3_TechnicalAnalysis/parseTaData.cpp \
                src/Application/Tabs/4_FundamentalAnalysis/nordnetparsecompanyid.cpp \
    		src/Application/Tabs/4_FundamentalAnalysis/fatab.cpp \
                src/Application/Tabs/5_TechFundAnalysis/TADlg.cpp \
                src/Application/Tabs/6_PortfolioOptimizer/efficientPortfolio.cpp \
    		src/Application/Tabs/6_PortfolioOptimizer/efficentbargraphdlg.cpp \
    		src/Application/Tabs/6_PortfolioOptimizer/threadcalcefficentfront.cpp \
    		src/Application/Tabs/7_ImportData/parsekeytabridgedata.cpp \
    		src/Application/Tabs/7_ImportData/importdata.cpp \
                src/Application/Tabs/7_ImportData/parsecompanyeconomyinfo.cpp \
		src/Application/Tabs/7_ImportData/parsecompdescription.cpp \
		src/Application/Tabs/7_ImportData/readexercisefile.cpp \
		src/Application/Tabs/8_Notes/tabhelp.cpp \
		src/Application/Tabs/9_KeyNumber2/leastSquareStaDlg.cpp \
                src/Application/Tabs/10_KeyNumber3/keyIndicators_3.cpp \
                src/Application/Tabs/11_PortfolioResult/adminMyPortFolio.cpp \
		src/Application/Tabs/12_Analysis/stockAnalysisTab.cpp \
		src/Application/Tabs/12_Analysis/createstockanalysishtmlpage.cpp \
		src/Application/Tabs/12_Analysis/subanalysisdisplaygraphdata.cpp \
    		src/Application/Tabs/12_Analysis/parseomxallfinancelinkdata.cpp \
    		src/Application/Tabs/12_Analysis/parseomxbridgedata.cpp \
    		src/Application/Tabs/12_Analysis/calcavgannualgrowthrateequity.cpp \
		src/Application/Tabs/13_video/video.cpp \
		src/Application/Tabs/14_others/graphtab.cpp \
		src/Application/Tabs/14_others/parseavanzadata.cpp \
                src/Application/Tabs/14_others/portfolio.cpp \
                src/Application/Tabs/14_others/jackStock.cpp \
                src/Application/Tabs/7_ImportData/getYahooKeyStatistics.cpp \
                src/GetHtmlPage/httpwindow.cpp \
                src/LibCurl/myLibCurl.cpp \
                src/QtLib/treewidgetfinance.cpp \
                src/Utils/financemath.cpp \
                src/Application/Tabs/12_Analysis/altgcalcintrinsicvaluedlg.cpp \
                src/Utils/robustgrowth.cpp \
    src/Application/Tabs/7_ImportData/parseborsdata.cpp

			    
    
HEADERS  += \
		inc/TaBuySellIdicator/taBuySellTask.h \
		inc/TaBuySellIdicator/taBuySellIdicator.h \
	    	inc/debug/debug.h \
		inc/guiUtil/guiFinanceCtrls.h \
                inc/guiUtil/myLineEdit.h \
    		inc/guiUtil/guiFinanceColor.h \
    		inc/guiUtil/myTreeWidget.h \
   		inc/guiUtil/guiUtils.h \
		inc/QtLib/extendedTable.h \
		inc/QtLib/findfiledialog.h \
		inc/QtLib/extendedLineEdit.h \
		inc/QwtLib/canvaspicker.h \
		inc/QwtLib/extendedQwtPlot.h \
		inc/MyOs/myOs.h \
                inc/MyOs/mutex.h \
                inc/MyOs/mytaskQue.h \
                inc/MyOs/semaphore.h \
		inc/SqlDb/dbHndl.h \
		inc/Utils/matrix.h \
    		inc/Utils/taskQue.h \
    		inc/Utils/linkedList.h \
		inc/Utils/util.h \
		inc/Utils/fifo.h \
    		inc/Utils/sublist.h \
    		inc/Application/commonFinance.h \
		inc/Application/stockPlotUtil.h \
    		inc/Application/yahooStockPlotUtil.h \
		inc/Application/tabdialog.h \
	    	inc/Application/selIndicatorsDlg.h \
	    	inc/Application/common.h \
	    	inc/Application/assistant.h \
	    	inc/Application/textedit.h \
		inc/Application/Tabs/1_Web/WebBrowserTab.h \
                inc/Application/Tabs/2_KeyNumber1/tableTab.h \
                inc/Application/Tabs/2_KeyNumber1/parsesnapshotdata.h \
                inc/Application/Tabs/2_KeyNumber1/parseKeyNumberData.h \
                inc/Application/Tabs/2_KeyNumber1/parseYieldData.h \
                inc/Application/Tabs/2_KeyNumber1/parsePriceData.h \
                inc/Application/Tabs/3_TechnicalAnalysis/taAnalysis.h \
                inc/Application/Tabs/3_TechnicalAnalysis/parseTaData.h \
                inc/Application/Tabs/3_TechnicalAnalysis/importYahooTaData.h \
		inc/Application/Tabs/4_FundamentalAnalysis/fatab.h \
		inc/Application/Tabs/4_FundamentalAnalysis/nordnetparsecompanyid.h \
                inc/Application/Tabs/5_TechFundAnalysis/TADlg.h \
                inc/Application/Tabs/6_PortfolioOptimizer/efficientPortfolio.h \
    		inc/Application/Tabs/6_PortfolioOptimizer/efficentbargraphdlg.h \
    		inc/Application/Tabs/6_PortfolioOptimizer/threadcalcefficentfront.h \
		inc/Application/Tabs/7_ImportData/parsecompdescription.h \
    		inc/Application/Tabs/7_ImportData/parsekeytabridgedata.h \
    		inc/Application/Tabs/7_ImportData/importdata.h \
    		inc/Application/Tabs/7_ImportData/parsecompanyeconomyinfo.h \
		inc/Application/Tabs/7_ImportData/readexercisefile.h \
		inc/Application/Tabs/8_Notes/tabhelp.h \
		inc/Application/Tabs/9_KeyNumber2/leastSquareStaDlg.h \
                inc/Application/Tabs/10_KeyNumber3/keyIndicators_3.h \
                inc/Application/Tabs/11_PortfolioResult/adminMyPortFolio.h \
		inc/Application/Tabs/12_Analysis/parseomxallfinancelinkdata.h \
    		inc/Application/Tabs/12_Analysis/parseomxbridgedata.h \
    		inc/Application/Tabs/12_Analysis/calcavgannualgrowthrateequity.h \
		inc/Application/Tabs/12_Analysis/stockAnalysisTab.h \
    		inc/Application/Tabs/12_Analysis/createstockanalysishtmlpage.h \
    		inc/Application/Tabs/12_Analysis/subanalysisdisplaygraphdata.h \
                inc/Application/Tabs/13_video/video.h \
    		inc/Application/Tabs/14_others/graphtab.h \
		inc/Application/Tabs/14_others/parseavanzadata.h \
                inc/Application/Tabs/14_others/portfolio.h \
                inc/Application/Tabs/14_others/ui_ctab2.h \
                inc/Application/Tabs/14_others/ui_ctab3.h \
                inc/Application/Tabs/14_others/jackStock.h \
                inc/Application/Tabs/7_ImportData/getYahooKeyStatistics.h \
                inc/GetHtmlPage/httpwindow.h \
                inc/LibCurl/myLibCurl.h \
                inc/QtLib/treewidgetfinance.h \
                inc/Utils/financemath.h \
                inc/QtLib/treewidgetfinance.h \
                inc/Utils/financemath.h \
                inc/Application/Tabs/12_Analysis/altgcalcintrinsicvaluedlg.h \
                inc/Utils/robustgrowth.h \
    src/Application/Tabs/7_ImportData/parseborsdata.h

FORMS    += \
    src/Application/selIndicatorsDlg.ui \
    src/Application/Tabs/1_Web/WebBrowserTab.ui \
    src/Application/Tabs/2_KeyNumber1/tableTab.ui \
    src/Application/Tabs/3_TechnicalAnalysis/taAnalysis.ui \
    src/Application/Tabs/4_FundamentalAnalysis/fatab.ui \
    src/Application/Tabs/5_TechFundAnalysis/TADlg.ui \
    src/Application/Tabs/6_PortfolioOptimizer/efficientPortfolio.ui \
    src/Application/Tabs/6_PortfolioOptimizer/efficentbargraphdlg.ui \
    src/Application/Tabs/7_ImportData/importdata.ui \
    src/Application/Tabs/8_Notes/tabhelp.ui \
    src/Application/Tabs/9_KeyNumber2/leastsquarestadlg.ui \
    src/Application/Tabs/10_KeyNumber3/keyIndicators_3.ui \
    src/Application/Tabs/11_PortfolioResult/adminmyportfolio.ui \
    src/Application/Tabs/12_Analysis/stockanalysistab.ui \
    src/Application/Tabs/12_Analysis/calcavgannualgrowthrateequity.ui \
    src/Application/Tabs/13_video/video.ui \
    src/Application/Tabs/14_others/portfolio.ui \
    src/Application/Tabs/14_others/graphtab.ui \
    src/Application/Tabs/14_others/jackStock.ui \
    src/Application/Tabs/12_Analysis/altgcalcintrinsicvaluedlg.ui

RESOURCES += \
    database/resourcedb.qrc




