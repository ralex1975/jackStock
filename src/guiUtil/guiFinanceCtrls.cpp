/**********************************************************************************
 *
 * Filename:    guiFinanceCtrl.cpp
 *
 *
 * Description:
 *
 *
 *
 ********************************************************************************/

#include "../../inc/guiUtil/guiFinanceCtrls.h"
#include "../../inc/guiUtil/guiFinanceColor.h"
#include "../../common.h"
#include "../../util.h"



const GuiFinanceCtrls::TimePeriodDays_ST GuiFinanceCtrls::m_timePeriodDaysArr[GuiFinanceCtrls::MAX_NOF_TIME_PERIOD_DAYS_ITEMS] =
{
    {QString::fromUtf8("AllaData"),       GuiFinanceCtrls::TIME_PERIOD_DAYS_ALL_DATA,},
    {QString::fromUtf8("10 År"),          GuiFinanceCtrls::TIME_PERIOD_DAYS_10_YEAR,},
    {QString::fromUtf8("7 År"),           GuiFinanceCtrls::TIME_PERIOD_DAYS_7_YEAR,},
    {QString::fromUtf8("5 År"),           GuiFinanceCtrls::TIME_PERIOD_DAYS_5_YEAR,},
    {QString::fromUtf8("3 År"),           GuiFinanceCtrls::TIME_PERIOD_DAYS_3_YEAR,},
    {QString::fromUtf8("2 År"),           GuiFinanceCtrls::TIME_PERIOD_DAYS_2_YEAR,},
    {QString::fromUtf8("1 År"),           GuiFinanceCtrls::TIME_PERIOD_DAYS_1_YEAR,},
    {QString::fromUtf8("6 månader"),      GuiFinanceCtrls::TIME_PERIOD_DAYS_6_MONTH,},
    {QString::fromUtf8("3 månader"),      GuiFinanceCtrls::TIME_PERIOD_DAYS_3_MONTH,},
    {QString::fromUtf8("1 månader"),      GuiFinanceCtrls::TIME_PERIOD_DAYS_1_MONTH,},
    {QString::fromUtf8("2 veckor"),       GuiFinanceCtrls::TIME_PERIOD_DAYS_2_WEEK,},
    {QString::fromUtf8("1 vecka"),        GuiFinanceCtrls::TIME_PERIOD_DAYS_1_WEEK}
};



/**********************************************************************************
 *
 * Function:
 *
 *
 * Description:
 *
 *
 *
 ********************************************************************************/
GuiFinanceCtrls::GuiFinanceCtrls(QWidget *parent) :
    QWidget(parent),
    m_comboBoxIndex(0)
{

}



/**********************************************************************************
 *
 * Function:        fa5AddData()
 *
 *
 * Description:     Add data and set text color on 5 fundamental analysis controls
 *
 *
 *
 ********************************************************************************/
void GuiFinanceCtrls::fa5AddData(QString inEarningsDivDividend,  QLineEdit *earningsDivDividend,
                                 QString inNavDivStockPrice,     QLineEdit *navDivStockPrice,
                                 QString inPe,                   QLineEdit *pe,
                                 QString inPs,                   QLineEdit *ps,
                                 QString inYield,                QLineEdit *yield,
                                 QString assetType)
{

    GuiFinanceColor gfc;
    MyLineEdit mle;


    // Remove data
    mle.reset5Ctrl(earningsDivDividend,
             navDivStockPrice,
             pe,
             ps,
             yield);

    // Add data
    earningsDivDividend->insert(inEarningsDivDividend);
    navDivStockPrice->insert(inNavDivStockPrice);
    pe->insert(inPe);
    ps->insert(inPs);
    yield->insert(inYield);

    // Set text color
    gfc.setTxtColorEarningsDivDividend(inEarningsDivDividend, earningsDivDividend);
    gfc.setTxtColorNavDivStockPrice(inNavDivStockPrice, navDivStockPrice);
    gfc.setTxtColorPe(inPe, pe, assetType);
    gfc.setTxtColorPs(inPs, ps);
    gfc.setTxtColorYield(inYield, inEarningsDivDividend, yield);
}







/*******************************************************************
 *
 * Function:    initHistoricalPe()
 *
 * Description: Init headers in treeWidget.
 *
 *
 *******************************************************************/
void GuiFinanceCtrls::
initHistoricalPe(QTreeWidget *treeWidget,
                 QString header0, int headerWidth0,
                 QString header1, int headerWidth1,
                 QString header2, int headerWidth2,
                 QString header3, int headerWidth3)
{

    MyTreeWidget mtw;

    mtw.setHeader4ColumnTreeWidget(treeWidget,
                               header0, headerWidth0,
                               header1, headerWidth1,
                               header2, headerWidth2,
                               header3, headerWidth3);
}



/*******************************************************************
 *
 * Function:    initHistoricalStockPrice()
 *
 * Description: Init headers in treeWidget.
 *
 *
 *
 *******************************************************************/
void GuiFinanceCtrls::
initHistoricalStockPrice(QTreeWidget *treeWidget,
                         QString header0, int headerWidth0,
                         QString header1, int headerWidth1,
                         QString header2, int headerWidth2,
                         QString header3, int headerWidth3)
{


    MyTreeWidget mtw;

    mtw.setHeader4ColumnTreeWidget(treeWidget,
                               header0, headerWidth0,
                               header1, headerWidth1,
                               header2, headerWidth2,
                               header3, headerWidth3);
}






/*******************************************************************
 *
 * Function:    initSalesGrowth()
 *
 * Description: Init headers in treeWidget.
 *
 * Försäljningstillväxt
 *
 *******************************************************************/
void GuiFinanceCtrls::
initSalesGrowth(QTreeWidget *treeWidget,
                QString header0, int headerWidth0,
                QString header1, int headerWidth1,
                QString header2, int headerWidth2,
                QString header3, int headerWidth3)
{


    MyTreeWidget mtw;

    mtw.setHeader4ColumnTreeWidget(treeWidget,
                                   header0, headerWidth0,
                                   header1, headerWidth1,
                                   header2, headerWidth2,
                                   header3, headerWidth3);
}




/*******************************************************************
 *
 * Function:    initProfitGrowth()
 *
 * Description: Init headers in treeWidget.
 *
 * Vinsttillväxt
 *
 *******************************************************************/
void GuiFinanceCtrls::
initProfitGrowth(QTreeWidget *treeWidget,
                QString header0, int headerWidth0,
                QString header1, int headerWidth1,
                QString header2, int headerWidth2,
                QString header3, int headerWidth3)
{


    MyTreeWidget mtw;

    mtw.setHeader4ColumnTreeWidget(treeWidget,
                                   header0, headerWidth0,
                                   header1, headerWidth1,
                                   header2, headerWidth2,
                                   header3, headerWidth3);
}




/*******************************************************************
 *
 * Function:    initProfitabilityAnalysis()
 *
 * Description: Init headers in treeWidget.
 *
 * Lönsamhetsanalys
 *
 *******************************************************************/
void GuiFinanceCtrls::
initProfitabilityAnalysis(QTreeWidget *treeWidget,
                          QString header0, int headerWidth0,
                          QString header1, int headerWidth1,
                          QString header2, int headerWidth2)
{


    MyTreeWidget mtw;

    mtw.setHeader3ColumnTreeWidget(treeWidget,
                                   header0, headerWidth0,
                                   header1, headerWidth1,
                                   header2, headerWidth2);
}



/*******************************************************************
 *
 * Function:    initStockList()
 *
 * Description: Init headers in treeWidget.
 *
 *
 *
 *
 *******************************************************************/
void GuiFinanceCtrls::initStockList(QTreeWidget *treeWidget,
                                    QString header0, int headerWidth0, bool hideCol0,
                                    QString header1, int headerWidth1, bool hideCol1,
                                    QString header2, int headerWidth2, bool hideCol2)
{

    MyTreeWidget mtw;

    mtw.setHeader3ColumnTreeWidget(treeWidget,
                                   header0, headerWidth0, hideCol0,
                                   header1, headerWidth1, hideCol1,
                                   header2, headerWidth2, hideCol2);
}



/*******************************************************************
 *
 * Function:    addAllStockListsToCombobox()
 *
 * Description: This function add all stocklist name stored in db
 *              and add it to a combobox.
 *
 *******************************************************************/
void GuiFinanceCtrls::addAllStockListsToCombobox(QComboBox *comboBox)
{
    comboBox->clear();

    // m_db.delAllTaStockLists();
    // m_db.delAllTaStockNames();

    if(false == m_db.addStockListsToComboBox(comboBox))
    {
        QMessageBox::information(NULL, QString::fromUtf8("Open file"), QString::fromUtf8("Fail to retrive any stocklist from db"));
        return;
    }

    comboBox->setCurrentIndex(m_comboBoxIndex);
}



/*******************************************************************
 *
 * Function:    addAllStockListsToCombobox()
 *
 * Description: This function retrieve current selected stocklist
 *              name and id.
 *
 *
 *
 *******************************************************************/
bool GuiFinanceCtrls::getStockListNameAndId(QComboBox *comboBox,
                                            QString &stockListName,
                                            int &stockListId)
{
    stockListName = comboBox->currentText().toUtf8();

    return m_db.findTaStockListId(stockListName, stockListId);
}


/*******************************************************************
 *
 * Function:    initStockList()
 *
 * Description:
 *
 *
 *******************************************************************/
void GuiFinanceCtrls::initStockList1(QTreeWidget *treeWidgetStockList, bool hideSymbol)
{

    QString column0 = QString::fromUtf8("Namn");
    QString column1 = QString::fromUtf8("Symbol");
    QString column2 = QString::fromUtf8("Notera");

    treeWidgetStockList->setColumnCount(3);
    treeWidgetStockList->setSelectionMode(QAbstractItemView::SingleSelection);
    treeWidgetStockList->setSelectionBehavior(QAbstractItemView::SelectRows);

    if(QTreeWidgetItem* header = treeWidgetStockList->headerItem())
    {
        header->setText(0, column0.toLocal8Bit());
        header->setText(1, column1.toLocal8Bit());
        header->setText(2, column2.toLocal8Bit());
    }

    treeWidgetStockList->setColumnHidden(1, hideSymbol);

}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void GuiFinanceCtrls::getSelStockListItem(QTreeWidget *treeWidgetStockList,
                                QString &stockName,
                                QString &stockSymbol,
                                const QModelIndex &index)
{
    treeWidgetStockList->setCurrentItem(treeWidgetStockList->topLevelItem(index.row()));
    treeWidgetStockList->setFocus();

    stockName.clear();
    stockSymbol.clear();


    stockName = treeWidgetStockList->currentItem()->text(STOCKLIST_NAME);
    stockSymbol = treeWidgetStockList->currentItem()->text(STOCKLIST_SYMBOL);

}




/*******************************************************************
 *
 * Function:    initTimePeriodCtrls()
 *
 * Description:
 *
 *
 *******************************************************************/
void GuiFinanceCtrls::initTimePeriodCtrls(int value,
                                          QLineEdit *timePeriodLineEdit,
                                          QLineEdit *setEndDateLineEdit,
                                          QLineEdit *setStartDateLineEdit,
                                          QSlider *selTimePeriodSlider)
{
    CUtil cu;

    // Init to valid date
    cu.getCurrentDate(m_startDate);
    cu.getCurrentDate(m_endDate);
    m_endDateRef = m_endDate;


    m_timePeriodDaysInc = value;
    timePeriodLineEdit->clear();
    timePeriodLineEdit->insert((QString)m_timePeriodDaysArr[value].TxtTimePeriod.toLatin1());
    selTimePeriodSlider->setValue(value);
    setTimePeriodDaysUpdateStartStopDate(m_startDate, m_endDate, value, setEndDateLineEdit, setStartDateLineEdit);
}



/*******************************************************************
 *
 * Function:    setTimePeriodDaysUpdateStart()
 *
 * Description: This function is used when user changes set
 *              Time Period Slider position.
 *
 *
 *******************************************************************/
void GuiFinanceCtrls::
setTimePeriodDaysUpdateStartStopDate(QString &startDate,
                                     QString &endDate,
                                     int value,
                                     QLineEdit *setEndDateLineEdit,
                                     QLineEdit *setStartDateLineEdit)
{
    CUtil cu;
    int intYear;
    int intMonth;
    int intDays;
    int inc = 0;


    if(false == cu.dateIsValid(endDate))
    {
         QMessageBox::information(this, tr("Datum"), QString::fromUtf8("1. Slut datum felaktigt"));
        return;
    }

    inc = -m_timeSlideWindowInc;
    cu.addDays(endDate, endDate, inc);


    qDebug() << "sw" << m_timePeriodDaysArr[value].timePeriod;

    switch(m_timePeriodDaysArr[value].timePeriod)
    {
    case TIME_PERIOD_DAYS_ALL_DATA:
        // TBD
        break;
    case TIME_PERIOD_DAYS_10_YEAR:
        intYear = -10;
        cu.addYear(endDate, startDate, intYear);
        break;
    case TIME_PERIOD_DAYS_7_YEAR:
        intYear = -7;
        cu.addYear(endDate, startDate, intYear);
        break;
    case TIME_PERIOD_DAYS_5_YEAR:
        intYear = -5;
        cu.addYear(endDate, startDate, intYear);
        break;
    case TIME_PERIOD_DAYS_3_YEAR:
        intYear = -3;
        cu.addYear(endDate, startDate, intYear);
        break;
    case TIME_PERIOD_DAYS_2_YEAR:
        intYear = -2;
        cu.addYear(endDate, startDate, intYear);
        break;
    case TIME_PERIOD_DAYS_1_YEAR:
        intYear = -1;
        cu.addYear(endDate, startDate, intYear);
        break;
    case TIME_PERIOD_DAYS_6_MONTH:
        intMonth = -6;
        cu.addMonth(endDate, startDate, intMonth);
        break;
    case TIME_PERIOD_DAYS_3_MONTH:
        intMonth = -3;
        cu.addMonth(endDate, startDate, intMonth);
        break;
    case TIME_PERIOD_DAYS_1_MONTH:
        intMonth = -1;
        cu.addMonth(endDate, startDate, intMonth);
        break;
    case TIME_PERIOD_DAYS_2_WEEK:
        intDays = -14;
        cu.addDays(endDate, startDate, intDays);
        break;
    case TIME_PERIOD_DAYS_1_WEEK:
        intDays = -7;
        cu.addDays(endDate, startDate, intDays);
        break;
    default:
        break;
    }

    qDebug() << "startTime" << startDate;
    qDebug() << "endTime" << endDate;

    setEndDateLineEdit->clear();
    setStartDateLineEdit->clear();
    setEndDateLineEdit->insert(endDate);
    setStartDateLineEdit->insert(startDate);

}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 *******************************************************************/
bool GuiFinanceCtrls::
calcRiskAndReturn(QString startDate,
                  QString endDate,
                  CDbHndl::EfficPortStockData_ST &data)
{
    CDbHndl db;
    data.isValid = false;

    if(true == db.efficPortfCalcMeanAndStdDev(startDate, endDate, data))
    {
        data.meanReturns = data.meanReturns *100;
        data.riskStdDev = data.riskStdDev * 100;

        if(false == data.isValid)
        {
            qDebug() << "invalid risk return calc" << data.stockName;
        }
    }

    return data.isValid;
}
