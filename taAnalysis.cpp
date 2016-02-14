#include "taAnalysis.h"
#include "ui_taAnalysis.h"

#include "common.h"
#include <QDebug>
#include <QtCore>
#include <QtGui>
#include <QObject>
#include "parseTaData.h"
#include <qwt_scale_widget.h>
#include <qwt_scale_draw.h>
#include "util.h"
#include <QMessageBox>
#include <qwt_plot_layout.h>


#define TIME_2_MIN ((int)120000)

const TaAnalysis::TimePeriodDays_ST TaAnalysis::m_timePeriodDaysArr[TaAnalysis::MAX_NOF_TIME_PERIOD_DAYS_ITEMS] =
{
    {QString::fromUtf8("AllaData"),       TaAnalysis::TIME_PERIOD_DAYS_ALL_DATA,},
    {QString::fromUtf8("10 År"),          TaAnalysis::TIME_PERIOD_DAYS_10_YEAR,},
    {QString::fromUtf8("7 År"),           TaAnalysis::TIME_PERIOD_DAYS_7_YEAR,},
    {QString::fromUtf8("5 År"),           TaAnalysis::TIME_PERIOD_DAYS_5_YEAR,},
    {QString::fromUtf8("3 År"),           TaAnalysis::TIME_PERIOD_DAYS_3_YEAR,},
    {QString::fromUtf8("2 År"),           TaAnalysis::TIME_PERIOD_DAYS_2_YEAR,},
    {QString::fromUtf8("1 År"),           TaAnalysis::TIME_PERIOD_DAYS_1_YEAR,},
    {QString::fromUtf8("6 månader"),      TaAnalysis::TIME_PERIOD_DAYS_6_MONTH,},
    {QString::fromUtf8("3 månader"),      TaAnalysis::TIME_PERIOD_DAYS_3_MONTH,},
    {QString::fromUtf8("1 månader"),      TaAnalysis::TIME_PERIOD_DAYS_1_MONTH,},
    {QString::fromUtf8("2 veckor"),       TaAnalysis::TIME_PERIOD_DAYS_2_WEEK,},
    {QString::fromUtf8("1 vecka"),        TaAnalysis::TIME_PERIOD_DAYS_1_WEEK}
};



/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
TaAnalysis::TaAnalysis(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TaAnalysis),
    m_timeSlideWindowInc(0),
    m_singleStockDataReqStatus(STATUS_REQ_SINGLE_STOCK_IDLE)
{
    int i;
    CDbHndl db;

    ui->setupUi(this);
    initStockList();
    initFa2OperatingIncomeList();
    initFa2netProfitAfterTaxList();
    initFa3ProfitabilityAnalysis();
    initFa3MinMaxPePrice();
    // openUserStockListsFile();

    ui->checkBoxReqestData->setChecked(true);

    ui->StockListComboBox->clear();
    db.addStockListsToComboBox(ui->StockListComboBox);
    ui->StockListComboBox->setCurrentIndex(0);

    on_SelStockListButton_clicked();

    // Create thread that import data from yahoo
    m_importYahooTaDataThread = new importYahooTaData(this);

    if(m_importYahooTaDataThread == 0)
    {
        QMessageBox::information(NULL, QObject::tr("Create thread"), QString::fromUtf8("Fail to create Ta Import Data thread"));
    }

    // Init time period
    initTimePeriodCtrls(TIME_PERIOD_DAYS_1_YEAR);

    settimeSlideWindowInc(0);

    m_plot.initPlotPicker(ui->qwtPlot);
    m_plot.initPlotZoomer(ui->qwtPlot);
    m_plot.initPlotPanner(ui->qwtPlot);

    m_plot.enablePanningMode(false);
    m_plot.enableZoomMode(false);

   m_macdHist = new QwtPlotHistogram ();

   ui->radioButton->setChecked(true);
   ui->radioButRSI->setChecked(true);

  // ui->qwtPlot->hide();
  // ui->qwtPlot_2->resize(600, 200);
  // ui->qwtPlot_2->move(100, 100);

    for(i = 0; i < FA_NOF_DATA; i++ )
    {
        m_faDataPalette[i] = new QPalette();
    }

}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
TaAnalysis::~TaAnalysis()
{
    delete ui;
    delete m_importYahooTaDataThread;
    delete m_macdHist;
    for(int i = 0; i < FA_NOF_DATA; i++ )
    {
        delete m_faDataPalette[i];
    }
}


/*******************************************************************
 *
 * Function:    initFa2OperatingIncomeList()
 *
 * Description:
 *
 *
 *******************************************************************/
void TaAnalysis::initFa2OperatingIncomeList(void)
{

    QString column0 = QString::fromUtf8("År");
    QString column1 = QString("Kr/\nAktie");
    QString column2 = QString("MKr");
    QString column3 = QString::fromUtf8("Tillväxt");

    ui->treeWidget_2->setColumnCount(4);
    ui->treeWidget_2->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);



    if(QTreeWidgetItem* header = ui->treeWidget_2->headerItem())
    {
        header->setText(0, column0);
        header->setText(1, column1);
        header->setText(2, column2);
        header->setText(3, column3);
    }

    ui->treeWidget_2->setColumnWidth(0, 65);
    ui->treeWidget_2->setColumnWidth(1, 65);
    ui->treeWidget_2->setColumnWidth(2, 60);
    ui->treeWidget_2->setColumnWidth(3, 50);
    ui->treeWidget_2->setColumnWidth(4, 50);

}







/*******************************************************************
 *
 * Function:    initStockList()
 *
 * Description:
 *
 *
 *******************************************************************/
void TaAnalysis::initFa2netProfitAfterTaxList(void)
{

    QString column0 = QString::fromUtf8("År");
    QString column1 = QString("Kr/\nAktie");
    QString column2 = QString("MKr");
    QString column3 = QString::fromUtf8("Tillväxt");

    ui->treeWidget_3->setColumnCount(4);
    ui->treeWidget_3->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeWidget_3->setSelectionBehavior(QAbstractItemView::SelectRows);



    if(QTreeWidgetItem* header = ui->treeWidget_3->headerItem())
    {
        header->setText(0, column0);
        header->setText(1, column1);
        header->setText(2, column2);
        header->setText(3, column3);
    }

    ui->treeWidget_3->setColumnWidth(0, 65);
    ui->treeWidget_3->setColumnWidth(1, 65);
    ui->treeWidget_3->setColumnWidth(2, 60);
    ui->treeWidget_3->setColumnWidth(3, 50);
    ui->treeWidget_3->setColumnWidth(4, 50);

}


/*******************************************************************
 *
 * Function:    initStockList()
 *
 * Description:
 *
 *
 *******************************************************************/
void TaAnalysis::initFa3ProfitabilityAnalysis(void)
{

    QString column0 = QString::fromUtf8("År");
    QString column1 = QString::fromUtf8("Vinstmarginal");
    QString column2 = QString::fromUtf8("Ränta / \nEget kapital");


    ui->treeWidget_4->setColumnCount(3);
    ui->treeWidget_4->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeWidget_4->setSelectionBehavior(QAbstractItemView::SelectRows);



    if(QTreeWidgetItem* header = ui->treeWidget_4->headerItem())
    {
        header->setText(0, column0);
        header->setText(1, column1);
        header->setText(2, column2);

    }

    ui->treeWidget_4->setColumnWidth(0, 65);
    ui->treeWidget_4->setColumnWidth(1, 85);
    ui->treeWidget_4->setColumnWidth(2, 65);



}


/*******************************************************************
 *
 * Function:    initStockList()
 *
 * Description:
 *
 *
 *******************************************************************/
void TaAnalysis::initFa3MinMaxPePrice(void)
{

    QString column0 = QString::fromUtf8("År");
    QString column1 = QString("Min\npris");
    QString column2 = QString::fromUtf8("Medel\npris");
    QString column3 = QString("Max\npris");


    ui->treeWidget_5->setColumnCount(4);
    ui->treeWidget_5->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeWidget_5->setSelectionBehavior(QAbstractItemView::SelectRows);



    if(QTreeWidgetItem* header = ui->treeWidget_5->headerItem())
    {
        header->setText(0, column0);
        header->setText(1, column1);
        header->setText(2, column2);
        header->setText(3, column3);

    }

    ui->treeWidget_5->setColumnWidth(0, 65);
    ui->treeWidget_5->setColumnWidth(1, 65);
    ui->treeWidget_5->setColumnWidth(2, 60);
    ui->treeWidget_5->setColumnWidth(3, 50);


    QString column4 = QString::fromUtf8("År");
    QString column5 = QString("Min\nP/E");
    QString column6 = QString::fromUtf8("Medel\nP/E");
    QString column7 = QString("Max\nP/E");


    ui->treeWidget_6->setColumnCount(4);
    ui->treeWidget_6->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeWidget_6->setSelectionBehavior(QAbstractItemView::SelectRows);



    if(QTreeWidgetItem* header1 = ui->treeWidget_6->headerItem())
    {
        header1->setText(0, column4);
        header1->setText(1, column5);
        header1->setText(2, column6);
        header1->setText(3, column7);
    }

    ui->treeWidget_6->setColumnWidth(0, 65);
    ui->treeWidget_6->setColumnWidth(1, 65);
    ui->treeWidget_6->setColumnWidth(2, 60);
    ui->treeWidget_6->setColumnWidth(3, 50);
    ui->treeWidget_6->setColumnWidth(4, 50);

}



/*******************************************************************
 *
 * Function:    initStockList()
 *
 * Description:
 *
 *
 *******************************************************************/
void TaAnalysis::initStockList(void)
{

    QString column0 = QObject::tr("Namn");
    QString column1 = QObject::tr("Symbol");
    QString column2 = QObject::tr("Notera");

    ui->treeWidget->setColumnCount(3);
    ui->treeWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    if(QTreeWidgetItem* header = ui->treeWidget->headerItem())
    {
        header->setText(0, column0.toLocal8Bit());
        header->setText(1, column1.toLocal8Bit());
        header->setText(2, column2.toLocal8Bit());
    }

}



/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void TaAnalysis::modifyDateList(QString stockSymbol, bool isUpdate)
{
    NameKey node;
     QMutableListIterator <NameKey> iterator(m_dateIsUpdatedList);
     while(iterator.hasNext())
     {
        node = iterator.next();
        if(node.symbolName.compare(stockSymbol)== 0)
        {
            //iterator.remove();
            addStatusDateList(stockSymbol, isUpdate);

            QString tmp;
            tmp.sprintf("%d",isUpdate);
            qDebug() << tmp;
            qDebug() << stockSymbol;
            node.dataIsUpdated = isUpdate;
            iterator.setValue(node);
            return;
        }

     }
}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void TaAnalysis::findDateList(QString stockSymbol, bool &isUpdate)
{
    isUpdate = true;

    NameKey node;
     QMutableListIterator <NameKey> iterator(m_dateIsUpdatedList);
     while(iterator.hasNext())
     {
        node = iterator.next();
        qDebug() << node.symbolName;
        QString tmp;
        tmp.sprintf("%d", node.dataIsUpdated);
        qDebug() << tmp;
        if(node.symbolName.compare(stockSymbol)== 0)
        {
            isUpdate = node.dataIsUpdated;
            return;
        }

     }
}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void TaAnalysis::addStatusDateList(QString stockSymbol, bool isUpdate)
{
    NameKey data;
    data.symbolName = stockSymbol;
    data.dataIsUpdated = isUpdate;
    m_dateIsUpdatedList.append(data);

}



/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void TaAnalysis::getSelStockListItem(QString &stockName, QString &stockSymbol, const QModelIndex &index)
{
    ui->treeWidget->setCurrentItem(ui->treeWidget->topLevelItem(index.row()));
    ui->treeWidget->setFocus();

    stockName.clear();
    stockSymbol.clear();


    stockName = ui->treeWidget->currentItem()->text(STOCKLIST_NAME);
    stockSymbol = ui->treeWidget->currentItem()->text(STOCKLIST_SYMBOL);

}



/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void TaAnalysis::on_SelStockListButton_clicked()
{
    QString stockListName;
    int stockListId;

     stockListName = ui->StockListComboBox->currentText().toUtf8();

     if(true == m_db.findTaStockListId(stockListName, stockListId))
     {
         ui->treeWidget->clear();
         m_db.addStockSymbolAndNameInTreeWidget1(ui->treeWidget,
                                            stockListId,
                                            CDbHndl::SORT_TWIDGET_NAME,
                                            (char *) SQL_STR_ASC);


         ui->treeWidget->setColumnWidth(0, 170);
         ui->treeWidget->scrollToTop();
         ui->treeWidget->setCurrentItem(ui->treeWidget->topLevelItem(0));
     }
}


/*******************************************************************
 *
 * Function:    on_treeWidget_doubleClicked()
 *
 * Description: This function is invoked when user double clicks
 *              on stock list ctrl (treeWidget).
 *
 *
 *******************************************************************/
void TaAnalysis::on_treeWidget_doubleClicked(const QModelIndex &index)
{

    QString stockName;
    QString stockSymbol;
    CDbHndl cd;
    CUtil cu;
    QString lastDbDate;
    QString currDate;
    QString tmp;
    int nofDays;
    NordnetCompanyDescription_ST data;

    // Are we already processing data?
    if(m_singleStockDataReqStatus == STATUS_REQ_SINGLE_STOCK_IDLE)
    {
        m_singleStockDataReqStatus = STATUS_REQ_SINGLE_STOCK_PENDING;
        getSelStockListItem(stockName, stockSymbol, index);

        m_reqStockSymbol = stockSymbol;
        m_reqStockName = stockName;

        qDebug() << m_reqStockSymbol;
        qDebug() << m_reqStockName;

        CDbHndl::snapshotStockData_ST keyFaData;

        ui->lineEditPE->setDisabled(false);
        ui->lineEditPE_2->setDisabled(false);


        ui->lineEditMoreInfoComapnyName->clear();
        ui->lineEditMoreInfoexecutiveDirector->clear();
        ui->lineEditMoreInfoBransch->clear();
        ui->lineEditMoreInfoSector->clear();
        ui->textEditMoreInfobusinessDescription->clear();

        //ui->lineEditMoreInfoComapnyName_2->clear();
        ui->lineEditMoreInfoexecutiveDirector_2->clear();
        ui->lineEditMoreInfoBransch_2->clear();
        ui->lineEditMoreInfoSector_2->clear();
        ui->textEditMoreInfobusinessDescription_2->clear();


        ui->lineEditEarningsDivByDividend->clear();
        ui->lineEditNavDivLastStockPrice->clear();
        ui->lineEditPE->clear();
        ui->lineEditPs->clear();
        ui->lineEditRisk->clear();
        ui->lineEditYield->clear();
        ui->lineMeanReturn->clear();

        ui->lineEditEarningsDivByDividend_2->clear();
        ui->lineEditNavDivLastStockPrice_2->clear();
        ui->lineEditPE_2->clear();
        ui->lineEditPs_2->clear();
        ui->lineEditYield_2->clear();

        ui->lineEditEarningsDivByDividend_3->clear();
        ui->lineEditNavDivLastStockPrice_3->clear();
        ui->lineEditPE_3->clear();
        ui->lineEditPs_3->clear();
        ui->lineEditYield_3->clear();

        ui->treeWidget_2->clear();
        ui->treeWidget_3->clear();
        ui->treeWidget_4->clear();
        ui->treeWidget_5->clear();
        ui->treeWidget_6->clear();
        QString assetType;
        assetType.clear();

        ui->assetType->clear();

        ui->labelLastPrice->clear();
        ui->labelOneDayProcentChange->clear();

        if(m_startDate.isEmpty() == false && m_endDate.isEmpty() == false)
        {
            if(true == cd.getKeyDataUseBridge(m_reqStockSymbol,keyFaData))
            {
                qDebug() << keyFaData.companyName;

                if(true == cd.getNordnetCompanyDescription(data, keyFaData.companyName))
                {
                    ui->lineEditMoreInfoComapnyName->insert(data.assetName);
                    ui->lineEditMoreInfoexecutiveDirector->insert(data.executiveDirector);
                    ui->lineEditMoreInfoBransch->insert(data.bransch);
                    ui->lineEditMoreInfoSector->insert(data.sector);
                    ui->textEditMoreInfobusinessDescription->insertPlainText(data.businessDescription);


                    // ui->lineEditMoreInfoComapnyName_2->insert(data.assetName);
                    ui->lineEditMoreInfoexecutiveDirector_2->insert(data.executiveDirector);
                    ui->lineEditMoreInfoBransch_2->insert(data.bransch);
                    ui->lineEditMoreInfoSector_2->insert(data.sector);
                    ui->textEditMoreInfobusinessDescription_2->insertPlainText(data.businessDescription);

                }


                cd.addFa2operatingIncomeToTreeWidget(ui->treeWidget_2, keyFaData.companyName);
                cd.addFa2NetProfitAfterTaxToTreeWidget(ui->treeWidget_3, keyFaData.companyName, assetType);

                ui->assetType->setText(assetType);

                tmp.sprintf("Pris %s",keyFaData.lastPrice.toLocal8Bit().constData());
                ui->labelLastPrice->setText(tmp);
                tmp.sprintf("Idag %s \%%",keyFaData.procentChangeOneDay.toLocal8Bit().constData());
                ui->labelOneDayProcentChange->setText(tmp);

                cd.addFa3NetProfitAfterTaxToTreeWidget(ui->treeWidget_4, keyFaData.companyName);

                cd.addFa3MinMaxPEAndPrice(ui->treeWidget_5, ui->treeWidget_6, keyFaData.companyName, m_reqStockSymbol);

                ui->lineEditEarningsDivByDividend->insert(keyFaData.earningsDividedByDividend);
                ui->lineEditNavDivLastStockPrice->insert(keyFaData.navDivLastStockPrice);
                ui->lineEditPE->insert(keyFaData.keyValuePE);
                ui->lineEditPs->insert(keyFaData.keyValuePS);
                ui->lineEditYield->insert(keyFaData.keyValueYield);

                ui->lineEditEarningsDivByDividend_2->insert(keyFaData.earningsDividedByDividend);
                ui->lineEditNavDivLastStockPrice_2->insert(keyFaData.navDivLastStockPrice);
                ui->lineEditPE_2->insert(keyFaData.keyValuePE);
                ui->lineEditPs_2->insert(keyFaData.keyValuePS);
                ui->lineEditYield_2->insert(keyFaData.keyValueYield);

                ui->lineEditEarningsDivByDividend_3->insert(keyFaData.earningsDividedByDividend);
                ui->lineEditNavDivLastStockPrice_3->insert(keyFaData.navDivLastStockPrice);
                ui->lineEditPE_3->insert(keyFaData.keyValuePE);
                ui->lineEditPs_3->insert(keyFaData.keyValuePS);
                ui->lineEditYield_3->insert(keyFaData.keyValueYield);



                CDbHndl::EfficPortStockData_ST stockRiskReturnData;
                stockRiskReturnData.stockSymbol = m_reqStockSymbol;
                if(true == cd.efficPortfCalcMeanAndStdDev(m_startDate, m_endDate, stockRiskReturnData))
                {
                    QString tmp;
                    tmp.sprintf("%.2f",stockRiskReturnData.riskStdDev * 100);
                    ui->lineEditRisk->insert(tmp);
                    tmp.sprintf("%.2f",stockRiskReturnData.meanReturns * 100);
                    ui->lineMeanReturn->insert(tmp);
                }
                setFundametalAnalysisCtrlTxtColor(keyFaData, stockRiskReturnData, assetType);
            }
        }


        if(true == cd.getLatestDateYahooTaData(lastDbDate, stockSymbol))
        {
            cu.getCurrentDate(currDate);
            qDebug() << currDate;
            qDebug() << lastDbDate;
            cu.nofDaysBeteenDates(currDate, lastDbDate, nofDays);
            if(nofDays < -1)
            {
                bool isUpdate;

                findDateList(stockSymbol, isUpdate);
                QString tmp;
                tmp.sprintf("%d",isUpdate);
                qDebug() << tmp;
                qDebug() << stockSymbol;
                if(/*isUpdate == false &&*/ ui->checkBoxReqestData->isChecked() == true)
                {
                    // Prepare request of data from Yahoo
                    qDebug() << stockName;
                    qDebug() << stockSymbol;
                    qDebug() << lastDbDate;
                    qDebug() << currDate;

                    prepReqTaDataFromServer(stockName, stockSymbol, lastDbDate, currDate);
                }
                else
                {
                    m_singleStockDataReqStatus = STATUS_REQ_SINGLE_STOCK_IDLE;
                    // ajn 160118 displayStockData(true, keyFaData.lastPrice.toDouble());
                    displayStockData(false, keyFaData.lastPrice.toDouble());
                }
            }
            else
            {
                m_singleStockDataReqStatus = STATUS_REQ_SINGLE_STOCK_IDLE;
                // ajn 160118 displayStockData(true, keyFaData.lastPrice.toDouble());
                displayStockData(false, keyFaData.lastPrice.toDouble());
            }
       }
        // No latest date was found lets add our own
        else
        {
            // Prepare request of data from Yahoo
            lastDbDate = "1992-01-01";
            cu.getCurrentDate(currDate);
            prepReqTaDataFromServer(stockName, stockSymbol, lastDbDate, currDate);
        }
    }
    else
    {
        m_singleStockDataReqStatus = STATUS_REQ_SINGLE_STOCK_IDLE;
        QMessageBox::information(this, QString::fromUtf8("Vänta"), QString::fromUtf8("Vänta Processar redan data.."));
    }
}



void TaAnalysis::setFaEditControlTxtColor(QLineEdit *lineEdit, QPalette *palette, const QColor &color)
{
    palette->setColor(QPalette::Text, color); //Qt::red
    lineEdit->setPalette(*palette);
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

void TaAnalysis::setFundametalAnalysisCtrlTxtColor(CDbHndl::snapshotStockData_ST keyData,
                                                   CDbHndl::EfficPortStockData_ST stockRiskReturnData,
                                                   QString assetType)
{
    CUtil cu;
    QColor color;

    if(keyData.earningsDividedByDividend.toDouble() >= 1 && keyData.earningsDividedByDividend.toDouble() < 2)
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
    }
    else if(keyData.earningsDividedByDividend.toDouble() >= 2)
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::GREEN);
    }
    else
    {
        color = Qt::black;
    }

    setFaEditControlTxtColor(ui->lineEditEarningsDivByDividend, m_faDataPalette[FA_ERNING_DIV_DIVIDEN], color);
    setFaEditControlTxtColor(ui->lineEditEarningsDivByDividend_2, m_faDataPalette[FA_ERNING_DIV_DIVIDEN], color);
    setFaEditControlTxtColor(ui->lineEditEarningsDivByDividend_3, m_faDataPalette[FA_ERNING_DIV_DIVIDEN], color);


    if(keyData.navDivLastStockPrice.toDouble() >= 0.7 && keyData.navDivLastStockPrice.toDouble() < 1)
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3);
    }
    else if(keyData.navDivLastStockPrice.toDouble() >= 1 && keyData.navDivLastStockPrice.toDouble() < 2)
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
    }
    else if(keyData.navDivLastStockPrice.toDouble() >= 2)
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::GREEN);
    }
    else
    {
        color = Qt::red;
    }

    setFaEditControlTxtColor(ui->lineEditNavDivLastStockPrice, m_faDataPalette[FA_NAV_DIV_LAST_PRICE], color);
    setFaEditControlTxtColor(ui->lineEditNavDivLastStockPrice_2, m_faDataPalette[FA_NAV_DIV_LAST_PRICE], color);
    setFaEditControlTxtColor(ui->lineEditNavDivLastStockPrice_3, m_faDataPalette[FA_NAV_DIV_LAST_PRICE], color);



    if(keyData.keyValuePE.toDouble() < 5)
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
    }
    else if(keyData.keyValuePE.toDouble() >= 5 && keyData.keyValuePE.toDouble() <= 15)
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::GREEN);
    }
    else if(keyData.keyValuePE.toDouble() > 15 && keyData.keyValuePE.toDouble() <= 18)
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3);
    }
    else if(keyData.keyValuePE.toDouble() > 18 && keyData.keyValuePE.toDouble() <= 25)
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
    }
    else
    {
        color = Qt::red;
    }

    // Disable P/E when company type is finance and real estate
    if(assetType.compare("Finans och fastighet")== 0)
    {
        ui->lineEditPE->setDisabled(true);
        ui->lineEditPE_2->setDisabled(true);
        ui->lineEditPE_3->setDisabled(true);
    }
    else
    {
        ui->lineEditPE->setDisabled(false);
        ui->lineEditPE_2->setDisabled(false);
        ui->lineEditPE_3->setDisabled(false);
    }
    setFaEditControlTxtColor(ui->lineEditPE, m_faDataPalette[FA_PE], color);
    setFaEditControlTxtColor(ui->lineEditPE_2, m_faDataPalette[FA_PE], color);
    setFaEditControlTxtColor(ui->lineEditPE_3, m_faDataPalette[FA_PE], color);

    double doubleNumber;
    if(false == cu.number2double(keyData.earningsDividedByDividend, doubleNumber))
    {
        if(keyData.keyValueYield.toDouble() >= 3 && keyData.keyValueYield.toDouble() < 4)
        {
            color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
        }
        else if(keyData.keyValueYield.toDouble() >= 4 && keyData.keyValueYield.toDouble() <= 6)
        {
            color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::GREEN);
        }
        else if(keyData.keyValueYield.toDouble() > 6 && keyData.keyValueYield.toDouble() < 7)
        {
            color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
        }
        else if(keyData.keyValueYield.toDouble() > 7 && keyData.keyValueYield.toDouble() < 10)
        {
            color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3);
        }
        else if(keyData.keyValueYield.toDouble() > 10)
        {
            color = Qt::red;
        }
        else
        {
            color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3);
        }
    }
    else
    {
        //  Grön Om vinst/Utdelning är >= 2 && direktavkastningn = 4 - 6 %
        if(keyData.earningsDividedByDividend.toDouble() > 1 && keyData.earningsDividedByDividend.toDouble() < 2)
        {
            if(keyData.keyValueYield.toDouble() >= 3 && keyData.keyValueYield.toDouble() < 4)
            {
                color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3);
            }
            else if(keyData.keyValueYield.toDouble() >= 4 )
            {
                color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
            }
            else
            {
                color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3);
            }
        }
        else if(keyData.earningsDividedByDividend.toDouble() >= 2)
        {
            if(keyData.keyValueYield.toDouble() >= 3 && keyData.keyValueYield.toDouble() < 4)
            {
                color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
            }
            else if(keyData.keyValueYield.toDouble() >= 4)
            {
                color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::GREEN);
            }
            else
            {
                color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3);
            }
        }
        else
        {
            color = Qt::red;
        }
    }



    setFaEditControlTxtColor(ui->lineEditYield, m_faDataPalette[FA_YIELD], color);
    setFaEditControlTxtColor(ui->lineEditYield_2, m_faDataPalette[FA_YIELD], color);
    setFaEditControlTxtColor(ui->lineEditYield_3, m_faDataPalette[FA_YIELD], color);


    if(stockRiskReturnData.meanReturns <= 0)
    {
        color = Qt::red;
    }
    else
    {
        color = Qt::black;
    }


    setFaEditControlTxtColor(ui->lineMeanReturn, m_faDataPalette[FA_MEAN_RETURN], color);



    if(keyData.keyValuePS.toDouble() <= 0.75)
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::GREEN);
    }
    else if(keyData.keyValuePS.toDouble() > 0.75 && keyData.keyValuePS.toDouble() <= 1.5)
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::YELLOW_3);
    }
    else if(keyData.keyValuePS.toDouble() > 1.5 && keyData.keyValuePS.toDouble() <= 3.0)
    {
        color = cu.getQColor((CUtil::ColorRgb_ET) CUtil::ORANGE);
    }
    else
    {
        color = Qt::red;
    }

    setFaEditControlTxtColor(ui->lineEditPs, m_faDataPalette[FA_YIELD], color);
    setFaEditControlTxtColor(ui->lineEditPs_2, m_faDataPalette[FA_YIELD], color);
    setFaEditControlTxtColor(ui->lineEditPs_3, m_faDataPalette[FA_YIELD], color);




}



/*******************************************************************
 *
 * Function:    startResendTimer()
 *
 * Description: This function activate resend timer that is used when
 *              request data from Yahoo server.
 *
 *              This is a single shot timer.
 *
 *******************************************************************/
void TaAnalysis::startReqSingleStockDataTimeoutTimer(int ms)
{
    QTimer::singleShot(ms, this, SLOT(slotReqSingleStockDataTimerExpired()));
    qDebug() << "startReqSingleStockDataTimeoutTimer()";

}

/*******************************************************************
 *
 * Function:    slotReqSingleStockDataTimerExpired()
 *
 * Description: This function is invoked when resend timer has expired
 *
 *
 *******************************************************************/
void TaAnalysis::slotReqSingleStockDataTimerExpired()
{
    if(m_singleStockDataReqStatus != STATUS_REQ_SINGLE_STOCK_IDLE)
    {
        m_singleStockDataReqStatus = STATUS_REQ_SINGLE_STOCK_IDLE;
        modifyDateList(m_reqStockSymbol, true);
        qDebug() << m_reqStockSymbol;
        QMessageBox::information(this, QString::fromUtf8("Timeout"), QString::fromUtf8("Timeout: ingen data kunde hämtas"));
    }

}




/*******************************************************************
 *
 * Function:    on_pushButtonImportAllData_clicked()
 *
 * Description:
 *
 *
 *******************************************************************/
void TaAnalysis::prepReqTaDataFromServer(QString stockName, QString stockSymbol, QString startDate, QString endDate)
{

    if(m_singleStockDataReqStatus == STATUS_REQ_SINGLE_STOCK_PENDING)
    {
        m_reqStartDate = startDate;
        m_reqEndDate = endDate;
        m_reqStockSymbol = stockSymbol;
        m_reqStockName = stockName;

        qDebug() << m_reqStartDate;
        qDebug() << m_reqEndDate;
        qDebug() << m_reqStockSymbol;
        qDebug() << m_reqStockName;


        // Signal used when thread is finish importing data
       connect(m_importYahooTaDataThread, SIGNAL(emitImportSingelStockDataIsFinish(int)), this, SLOT(slotImportSingelStockDataIsFinish(int)));
       connect(this, SIGNAL(emitReqSingleStockDataFromServer()), this, SLOT(slotReqSingleStockDataFromServer()));

       emit emitReqSingleStockDataFromServer();

    }

}



/*******************************************************************
 *
 * Function:    slotReqSingleStockDataFromServer()
 *
 * Description: This function is invoked when singel stock data need
 *              to be updated. (price data is old)
 *
 *******************************************************************/
void TaAnalysis::slotReqSingleStockDataFromServer()
{
    QString qry;
    CUtil cu;

    QString filename = DWLD_PATH_TA_LIST_FILE;

//    QString startDate;
    QString startYear;
    QString startMonth;
    QString startDay;

  //  QString endDate;
    QString endYear;
    QString endMonth;
    QString endDay;


    if(m_singleStockDataReqStatus == STATUS_REQ_SINGLE_STOCK_PENDING)
    {
        m_singleStockDataReqStatus = STATUS_REQ_SINGLE_STOCK_PROCESSING;

        QByteArray ba1 = m_reqStockSymbol.toLocal8Bit();
        const char *c_reqStockSymbol = ba1.data();

  //      m_reqStartDate = "2013-12-01";

        cu.splitDate(m_reqStartDate, startYear, startMonth, startDay);

        cu.splitDate(m_reqEndDate, endYear, endMonth, endDay);

        qry.sprintf("http://ichart.finance.yahoo.com/table.csv?s=%s&d=%d&e=%d&f=%d&g=d&a=%d&b=%d&c=%d&ignore=.csv",
                    c_reqStockSymbol,
                    (endMonth.toInt()-1),
                    endDay.toInt(),
                    endYear.toInt(),
                    (startMonth.toInt()-1),
                    startDay.toInt(),
                    startYear.toInt());

    //    qDebug() << startDate;
        qDebug() << startYear;
        qDebug() << startMonth;
        qDebug() << startDay;

      //  qDebug() << endDate;
        qDebug() << endYear;
        qDebug() << endMonth;
        qDebug() << endDay;

        qDebug() << qry;


        QObject::connect(&m_hw1, SIGNAL(sendSignalTextToDlg2(int)), this, SLOT(slotReceivedAssetTaDataFromServer(int)));
        QUrl url(qry);

        startReqSingleStockDataTimeoutTimer(TIME_2_MIN);
        m_hw1.startRequest(url, filename, 0x01);
    }
}


/*******************************************************************
 *
 * Function:    slotImportSingelStockDataIsFinish()
 *
 * Description: This function is invoked singel stock data is
 *              received from http thread
 *
 *******************************************************************/
void TaAnalysis::slotReceivedAssetTaDataFromServer(int)
{
    if(m_singleStockDataReqStatus == STATUS_REQ_SINGLE_STOCK_PROCESSING)
    {
        // Init thread with data before starting it
        m_importYahooTaDataThread->setImportInfoSingleStock(m_reqStockName, m_reqStockSymbol);
        m_importYahooTaDataThread->start(QThread::NormalPriority);
    }
}


/*******************************************************************
 *
 * Function:    slotImportSingelStockDataIsFinish()
 *
 * Description: This function is invoked singel stock data is
 *              parsed by: m_importYahooTaDataThread
 *
 *******************************************************************/
void TaAnalysis::slotImportSingelStockDataIsFinish(int dummy)
{
    dummy = dummy;
    if(m_singleStockDataReqStatus == STATUS_REQ_SINGLE_STOCK_PROCESSING)
    {
        m_singleStockDataReqStatus = STATUS_REQ_SINGLE_STOCK_IDLE;
        modifyDateList(m_reqStockSymbol, true);
        qDebug() << m_reqStockSymbol;
        displayStockData();
    }

}


/*******************************************************************
 *
 * Function:    displayStockData()
 *
 * Description:
 *
 *
 *******************************************************************/
void TaAnalysis::displayStockData(bool addLastPrice, double lastPrice)
{
    CDbHndl db;
    CYahooStockPlotUtil cy;
    DataPlotIndex_ET plotIndex;
    SubPlotIndex_ET subplotIndex;


    QString legendLable;
    QString status;
    int indicatorIndex;

    QString titleXAxis;
    CYahooStockPlotUtil::MinMaxAxisValues_ST mainPlotAxis;
    CYahooStockPlotUtil::MinMaxAxisValues_ST subPlotAxis;


    deleteAllPlotData(m_qwtStockPlotData, ui->qwtPlot);
    deleteAllPlotData(m_qwtsubPlotData, ui->qwtPlot_2);

    m_macdHistData.clear();
    m_macdHist->detach();
    m_macdHist->setData(NULL);


    QString startDate = ui->setStartDateLineEdit->text();
    QString endDate = ui->setEndDateLineEdit->text();

    m_qwtStockPlotData.nofStocksToPlot = 0;

    // This variable is automaticly updated when first data is handled by UpdateMinMaxAxis()
    m_qwtStockPlotData.axis.minMaxIsInit = false;
    clearStockAndIndicatorTempMem();

    db.getYahooTaPriceData(m_reqStockSymbol, startDate, endDate, m_qwtStockPlotData, m_stockData);
    initMinMaxAxis(m_qwtStockPlotData.axis, mainPlotAxis);

    if(addLastPrice == true)
    {
        QString date("");
        cy.addLastOnePlotData(m_stockData, m_qwtStockPlotData, lastPrice, date);
    }

    titleXAxis = "Tid (dagar)";
    legendLable = "Pris (SEK)";
    setAxisTitles(titleXAxis, legendLable,ui->qwtPlot);

    plotIndex = PLOT_INDEX_PRICE;
    addStockDataToPlot(plotIndex, legendLable);


    //=====

    legendLable.clear();
    if(true == ui->checkBoxLinjerReg->isChecked())
    {
        clearStockAndIndicatorTempMem();
        QString kValue;
        QString rValue;
        updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);
        db.getYahooTaLeastSqrFit(m_reqStockSymbol, startDate, endDate, m_qwtStockPlotData, m_stockData, kValue, rValue);
        updateMinMaxXAxisScales(m_qwtStockPlotData.axis, mainPlotAxis);

        indicatorIndex = 1;
        plotIndex = PLOT_INDEX_LINJER_REG;
        legendLable = "Linjer Regression";

        status = "K= ";
        status += kValue;
        status += ", R= ";
        status += rValue;

        ui->StatusInfoLabel->setText(status);

        addStockIndicatorToPlot(indicatorIndex, plotIndex, legendLable, mainPlotAxis, ui->qwtPlot, m_qwtStockPlotData);
        // updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);
    }



    legendLable.clear();
    // Check the checkbox
    if(true == ui->checkBoxShortMvAvg->isChecked())
    {
        clearStockAndIndicatorTempMem();

        qDebug() << "xx size " << m_stockData.data.x.size();
        updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);
        db.getYahooTaMovingAverage(m_reqStockSymbol, startDate, endDate, 20, m_qwtStockPlotData, m_stockData);
        updateMinMaxXAxisScales(m_qwtStockPlotData.axis, mainPlotAxis);

        indicatorIndex = 1;
        plotIndex = PLOT_INDEX_SHORT_MOVING_AVG;
        legendLable = "Short moving average";

        addStockIndicatorToPlot(indicatorIndex, plotIndex, legendLable, mainPlotAxis, ui->qwtPlot, m_qwtStockPlotData);
        // updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);

    }


    // Check the checkbox
    legendLable.clear();
    if(true == ui->checkBoxMidMvAvg->isChecked())
    {
        /// updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);
        clearStockAndIndicatorTempMem();

        qDebug() << "xx size " << m_stockData.data.x.size();
        updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);
        db.getYahooTaMovingAverage(m_reqStockSymbol, startDate, endDate, 50, m_qwtStockPlotData, m_stockData);
        updateMinMaxXAxisScales(m_qwtStockPlotData.axis, mainPlotAxis);

        indicatorIndex = 1;
        plotIndex = PLOT_INDEX_MID_MOVING_AVG;
        legendLable = "Mid moving average";

        addStockIndicatorToPlot(indicatorIndex, plotIndex, legendLable, mainPlotAxis, ui->qwtPlot, m_qwtStockPlotData);
        // updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);
    }



    // Check the checkbox
    legendLable.clear();
    if(true == ui->checkBoxLongMvAvg->isChecked())
    {
        // updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);
        clearStockAndIndicatorTempMem();
        qDebug() << "xx size " << m_stockData.data.x.size();
        updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);
        db.getYahooTaMovingAverage(m_reqStockSymbol, startDate, endDate, 150, m_qwtStockPlotData, m_stockData);
        updateMinMaxXAxisScales(m_qwtStockPlotData.axis, mainPlotAxis);

        indicatorIndex = 1;
        plotIndex = PLOT_INDEX_LONG_MOVING_AVG;
        legendLable = "Long moving average";

        addStockIndicatorToPlot(indicatorIndex, plotIndex, legendLable, mainPlotAxis, ui->qwtPlot, m_qwtStockPlotData);
        // updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);
    }



    // Check the checkboxes
    legendLable.clear();
    if(true == ui->checkBoxBollingBand->isChecked())
    {
        clearStockAndIndicatorTempMem();

        qDebug() << "xx size " << m_stockData.data.x.size();
        updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);
        db.getYahooTaBoillingerBand(m_reqStockSymbol, startDate, endDate, 20, 2, m_qwtStockPlotData, m_stockData);
        updateMinMaxXAxisScales(m_qwtStockPlotData.axis, mainPlotAxis);

        // Lower Bollingerband
        indicatorIndex = 1;
        plotIndex = PLOT_INDEX_BOLLINGERBAND_LOW;
        legendLable = "Lower bollingerband";

        addStockIndicatorToPlot(indicatorIndex, plotIndex, legendLable, mainPlotAxis, ui->qwtPlot, m_qwtStockPlotData);
        // updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);


        // Mid Bollingerband
        indicatorIndex++;
        plotIndex = PLOT_INDEX_BOLLINGERBAND_MID;
        legendLable = "Mid bollingerband";


        m_stockArr.clear();
        addStockIndicatorToPlot(indicatorIndex, plotIndex, legendLable, mainPlotAxis, ui->qwtPlot, m_qwtStockPlotData);
        // updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);

        // Upper Bollingerband
        indicatorIndex++;
        plotIndex = PLOT_INDEX_BOLLINGERBAND_UPPER;
        legendLable = "Upper bollingerband";

        m_stockArr.clear();
        addStockIndicatorToPlot(indicatorIndex, plotIndex, legendLable, mainPlotAxis, ui->qwtPlot, m_qwtStockPlotData);
        // updateMinMaxXAxisScales(mainPlotAxis, m_qwtStockPlotData.axis);
    }




    //=========

    legendLable.clear();
    if(ui->radioButMacd->isChecked() == true)
    {
        clearStockAndIndicatorTempMem();
        db.getYahooTaMacd(m_reqStockSymbol, startDate, endDate, 26, 12, 9, m_qwtsubPlotData, m_stockData);

        setAxisTitles(titleXAxis, " ",ui->qwtPlot_2, false);
        subplotIndex = SUBPLOT_MACD_LINE;
        legendLable = "MACD";
        indicatorIndex = 1;
        addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET)subplotIndex, legendLable, subPlotAxis, ui->qwtPlot_2, m_qwtsubPlotData);
        updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);

        subPlotAxis.maxX = mainPlotAxis.maxX;
        subPlotAxis.minX = mainPlotAxis.minX;
        m_qwtsubPlotData.axis.maxX = mainPlotAxis.maxX;
        m_qwtsubPlotData.axis.minX = mainPlotAxis.minX;


        setAxisTitles(titleXAxis, " ",ui->qwtPlot_2, false);
        subplotIndex = SUBPLOT_MACD_SIGNAL;
        legendLable = "MACD signal";
        indicatorIndex = 2;
        addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET)subplotIndex, legendLable, subPlotAxis, ui->qwtPlot_2, m_qwtsubPlotData);
        updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);



        setAxisTitles(titleXAxis, " ",ui->qwtPlot_2, false);
        subplotIndex = SUBPLOT_MACD_HISTOGRAM;
        legendLable = "MACD Hist";
        indicatorIndex = 3;
        addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET)subplotIndex, legendLable, subPlotAxis, ui->qwtPlot_2, m_qwtsubPlotData);
        updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);


        m_macdHist-> setStyle (QwtPlotHistogram :: Columns);

        m_macdHistData.clear();
        m_macdHist->detach();
        m_macdHist->setData(NULL);

        for(int ii = 0; ii < m_stockData.data.indicator3.size(); ii++)
        {
            m_stockData.data.x.at(ii);
            m_macdHistData.append (QwtIntervalSample (m_stockData.data.indicator3.at(ii),m_stockData.data.x.at(ii),m_stockData.data.x.at(ii)+1));
        }

        //m_macdHist->setBrush(Qt::red);
        m_macdHist->setPen( QPen( Qt::black) );
        m_macdHist->setSamples(m_macdHistData);
        m_macdHist->attach(ui->qwtPlot_2);
    }

    legendLable.clear();
    if(true == ui->radioButRoc->isChecked())
    {
        clearStockAndIndicatorTempMem();
        subPlotAxis.minMaxIsInit = false;
        m_qwtsubPlotData.axis.minMaxIsInit = false;
        db.getYahooTaRateOfChange(m_reqStockSymbol, startDate, endDate, 10, m_qwtsubPlotData, m_stockData);

        setAxisTitles(titleXAxis, "(%)",ui->qwtPlot_2, false);
        subplotIndex = SUBPLOT_RATE_OF_CHANGE;
        legendLable = "Rate of change";
        indicatorIndex = 1;
        addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET)subplotIndex, legendLable, subPlotAxis, ui->qwtPlot_2, m_qwtsubPlotData);
        updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);

        subplotIndex = SUBPLOT_RATE_OF_CHANGE_LIMIT_100;
        legendLable = " ";
        indicatorIndex = 2;
        addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET)subplotIndex, legendLable, subPlotAxis, ui->qwtPlot_2, m_qwtsubPlotData);
        updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);
    }


    // RSI = Relative Strength Index
    legendLable.clear();
    if(true == ui->radioButRSI->isChecked())
    {
        clearStockAndIndicatorTempMem();
        subPlotAxis.minMaxIsInit = false;
        m_qwtsubPlotData.axis.minMaxIsInit = false;
        db.getYahooTaRsi(m_reqStockSymbol, startDate, endDate, 14, m_qwtsubPlotData, m_stockData);
        m_qwtsubPlotData.axis.maxY=100;
        m_qwtsubPlotData.axis.minY=0;
        m_qwtsubPlotData.axis.minMaxIsInit = true;

        subplotIndex = SUBPLOT_RSI_LIMIT_30;
        legendLable = "";
        indicatorIndex = 2;
        addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET)subplotIndex, legendLable, subPlotAxis, ui->qwtPlot_2, m_qwtsubPlotData);
        updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);


        subplotIndex = SUBPLOT_RSI_LIMIT_70;
        legendLable = "";
        indicatorIndex = 3;
        addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET)subplotIndex, legendLable, subPlotAxis, ui->qwtPlot_2, m_qwtsubPlotData);
        updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);

        setAxisTitles(titleXAxis, "(%)",ui->qwtPlot_2, false);
        subplotIndex = SUBPLOT_RSI;
        legendLable = "";
        indicatorIndex = 1;
        addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET)subplotIndex, legendLable, subPlotAxis, ui->qwtPlot_2, m_qwtsubPlotData);
        updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);


    }


    legendLable.clear();
    if(true == ui->radioButStochastics->isChecked())
    {
        clearStockAndIndicatorTempMem();
        db.getYahooTaStochastics(m_reqStockSymbol, startDate, endDate,  9, 3, m_qwtsubPlotData, m_stockData);

        setAxisTitles(titleXAxis, "(%)",ui->qwtPlot_2, false);
        subplotIndex = SUBPLOT_STOCHASTIC_LIMIT_20;
        legendLable = "";

        indicatorIndex = 3;
        addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET)subplotIndex, legendLable, subPlotAxis, ui->qwtPlot_2, m_qwtsubPlotData);
        updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);

        legendLable = "";
        indicatorIndex = 4;
        subplotIndex = SUBPLOT_STOCHASTIC_LIMIT_80;
        addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET) subplotIndex, legendLable, subPlotAxis, ui->qwtPlot_2, m_qwtsubPlotData);
        updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);

        legendLable = "Stochastics D";
        indicatorIndex = 2;
        subplotIndex = SUBPLOT_STOCHASTIC_D;
        addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET) subplotIndex, legendLable, subPlotAxis, ui->qwtPlot_2, m_qwtsubPlotData);
        updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);

        legendLable = "Stochastics K";
        indicatorIndex = 1;
        subplotIndex = SUBPLOT_STOCHASTIC_K;

        addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET) subplotIndex, legendLable, subPlotAxis, ui->qwtPlot_2, m_qwtsubPlotData);
        updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);
    }

    legendLable.clear();
    if(true == ui->radioButMomentum->isChecked())
    {
        clearStockAndIndicatorTempMem();
        db.getYahooTaMomentum(m_reqStockSymbol, startDate, endDate, 20, m_qwtsubPlotData, m_stockData);

        indicatorIndex = 2;
        subplotIndex = SUBPLOT_INDEX_MOMENTUM_ZERO_LINE;
        addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET)subplotIndex, legendLable, subPlotAxis, ui->qwtPlot_2, m_qwtsubPlotData);
        updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);

        indicatorIndex = 1;
        subplotIndex = SUBPLOT_INDEX_MOMENTUM;
        legendLable = "Momentum";


        ui->StatusInfoLabel->setText(status);
        addStockIndicatorToPlot(indicatorIndex, (DataPlotIndex_ET) subplotIndex, legendLable, subPlotAxis, ui->qwtPlot_2, m_qwtsubPlotData);
        updateMinMaxXAxisScales(subPlotAxis, m_qwtsubPlotData.axis);
    }
//    updateMinMaxXAxisScales(subPlotAxis);




    //  boxesUpdate edit
    ui->lineEditAssetName->clear();
    ui->lineEditAssetName->insert(m_reqStockName);

    ui->lineEditStartDate->clear();
    ui->lineEditStartDate->insert(m_qwtStockPlotData.axis.minXDate);

    ui->lineEditStopDate->clear();
    ui->lineEditStopDate->insert(m_qwtStockPlotData.axis.maxXDate);



    // Reset zoomer
    m_plot.initPlotPicker(ui->qwtPlot);
}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void TaAnalysis::initMinMaxAxis(CYahooStockPlotUtil::MinMaxAxisValues_ST inputAxis,
               CYahooStockPlotUtil::MinMaxAxisValues_ST &outputAxis)
{
    outputAxis.minMaxIsInit = inputAxis.minMaxIsInit;
    outputAxis.maxX = inputAxis.maxX;
    outputAxis.maxY = inputAxis.maxY;
    outputAxis.minX  = inputAxis.minX;
    outputAxis.maxXDate  = inputAxis.maxXDate;
    outputAxis.minXDate  = inputAxis.minXDate;
    outputAxis.minY  = inputAxis.minY;
}

/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void TaAnalysis::addStockDataToPlot(DataPlotIndex_ET plotIndex,
                                    QString legendLable)
{
    m_stockArr.insert(0, m_stockData);

    // This is the first added graph
    m_plot.addStockToPlot(m_qwtStockPlotData, m_stockArr[0], m_qwtStockPlotData.axis, plotIndex, legendLable);

   //  m_plot.setRightLegend(ui->qwtPlot);

    m_plot.plotData(m_qwtStockPlotData, ui->qwtPlot, m_qwtStockPlotData.nofStocksToPlot-1);

    m_plot.removeAllStockArrData(m_stockArr);

}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void TaAnalysis::addStockIndicatorToPlot(int indicatorIndex,
                                         DataPlotIndex_ET plotIndex,
                                         QString legendLable,
                                         CYahooStockPlotUtil::MinMaxAxisValues_ST &axis,
                                         QwtPlot *qwtPlot,
                                         CYahooStockPlotUtil::PlotData_ST &qwtPlotData)
{
        m_stockArr.insert(0, m_stockData);

        m_plot.addIndicatorToPlot(qwtPlotData, m_stockArr[0], qwtPlotData.axis, indicatorIndex, plotIndex, legendLable);

        // m_plot.setRightLegend(qwtPlot);

        updateMinMaxXAxisScales(axis, qwtPlotData.axis);

        m_plot.plotData(qwtPlotData, qwtPlot, plotIndex);

        m_plot.removeAllStockArrData(m_stockArr);
}


/*******************************************************************
 *
 * Function:    deleteAllPlotData()
 *
 * Description:
 *
 *
 *******************************************************************/
void TaAnalysis::deleteAllPlotData(CYahooStockPlotUtil::PlotData_ST &allPlotData, QwtPlot *qwtPlot)
{
    for(int i = 0; i < PLOT_INDEX_MAX; i++)
    {
        m_plot.removePlotData(allPlotData, i, qwtPlot);
    }

}

/*******************************************************************
 *
 * Function:    clearStockAndIndicatorTempMem()
 *
 * Description:
 *
 *
 *******************************************************************/
void TaAnalysis::clearStockAndIndicatorTempMem(void)
{
    m_stockData.data.x.clear();
    m_stockData.data.y.clear();
    m_stockData.data.indicator1.clear();
    m_stockData.data.indicator2.clear();
    m_stockData.data.indicator3.clear();
    m_stockData.data.indicator4.clear();

    m_stockArr.clear();
}

/*******************************************************************
 *
 * Function:    setAxisTitles()
 *
 * Description:
 *
 *
 *******************************************************************/
void TaAnalysis::setAxisTitles(QString titleXAxis, QString titleYAxis, QwtPlot *qwtPlot,bool showXAxis)
{
    if(showXAxis==true)
        m_plot.setXAxisTitle(qwtPlot, titleXAxis);

    m_plot.setYAxisTitle(qwtPlot, titleYAxis);

}


/*******************************************************************
 *
 * Function:    updateMinMaxXAxisScales()
 *
 * Description:
 *
 *
 *******************************************************************/
void TaAnalysis::updateMinMaxXAxisScales(CYahooStockPlotUtil::MinMaxAxisValues_ST &axis, CYahooStockPlotUtil::MinMaxAxisValues_ST &qwtPlotAxis)
{
    CYahooStockPlotUtil cy;

    // Update min max scale of x and y axis
    cy.updateMinMaxAxis(axis, qwtPlotAxis.maxX, qwtPlotAxis.maxY, qwtPlotAxis.maxXDate);
    cy.updateMinMaxAxis(axis, qwtPlotAxis.minX, qwtPlotAxis.minY, qwtPlotAxis.minXDate);

    cy.updateMinMaxAxis(qwtPlotAxis, axis.maxX, axis.maxY, axis.maxXDate);
    cy.updateMinMaxAxis(qwtPlotAxis, axis.minX, axis.minY, axis.minXDate);

}


/*******************************************************************
 *
 * Function:    on_selTimePeriodSlider_valueChanged()
 *
 * Description: This function is invoked when user change set
 *              time period slider.
 *
 *
 *******************************************************************/
void TaAnalysis::on_selTimePeriodSlider_valueChanged(int value)
{
    m_timePeriodDaysInc = value;

    ui->timePeriodLlineEdit->clear();
    ui->timePeriodLlineEdit->insert((QString)m_timePeriodDaysArr[value].TxtTimePeriod.toLatin1());
    setTimePeriodDaysUpdateStartStopDate(m_startDate, m_endDate, value);
}


/*******************************************************************
 *
 * Function:    initTimePeriodCtrls()
 *
 * Description:
 *
 *
 *******************************************************************/
void TaAnalysis::initTimePeriodCtrls(int value)
{
    CUtil cu;
    int nofDays;

    // Init to valid date
    cu.getCurrentDate(m_startDate);
    cu.getCurrentDate(m_endDate);
    m_endDateRef = m_endDate;


    m_timePeriodDaysInc = value;
    ui->timePeriodLlineEdit->clear();
    ui->timePeriodLlineEdit->insert((QString)m_timePeriodDaysArr[value].TxtTimePeriod.toLatin1());
    ui->selTimePeriodSlider->setValue(value);
    setTimePeriodDaysUpdateStartStopDate(m_startDate, m_endDate, value);

    if(cu.nofDaysBeteenDates(m_startDate, m_endDate, nofDays) == true)
    {
       ui->moveTimePeriodSlider_2->setMaximum(nofDays);
    }


    // m_startDate.daysTo (m_endDate);

}

/*******************************************************************
 *
 * Function:    on_toggleZoomButton_clicked()
 *
 * Description:
 *
 *
 *******************************************************************/
void TaAnalysis::on_toggleZoomButton_clicked()
{
    static bool on = false;
    if(on == false)
        on = true;
    else
        on =false;
    m_plot.enableZoomMode(on);

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
void TaAnalysis::
setTimePeriodDaysUpdateStartStopDate(QString &startDate, QString &endDate, int value)
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
        intMonth = -6;
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

    ui->setEndDateLineEdit->clear();
    ui->setStartDateLineEdit->clear();
    ui->setEndDateLineEdit->insert(endDate);
    ui->setStartDateLineEdit->insert(startDate);

}


/*******************************************************************
 *
 * Function:    settimeSlideWindowInc()
 *
 * Description:
 *
 *
 *******************************************************************/
void TaAnalysis::settimeSlideWindowInc(int value)
{
    ui->moveTimePeriodSlider_2->setValue(value);
    m_timeSlideWindowInc = value;

}

/*******************************************************************
 *
 * Function:    settimeSlideWindowInc()
 *
 * Description: This function is invoked when siding window slider
 *              is changed.
 *
 *
 *******************************************************************/
void TaAnalysis::on_moveTimePeriodSlider_2_valueChanged(int value)
{
    setTimeSlideWindowUpdateStartStopDate(m_startDate, m_endDate, value);
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
void TaAnalysis::
setTimeSlideWindowUpdateStartStopDate(QString &startDate, QString &endDate, int value)
{
    CUtil cu;
    int intYear;
    int intMonth;
    int intDays;





    if(false == cu.dateIsValid(endDate))
    {
         QMessageBox::information(this, tr("Datum"), QString::fromUtf8("Slut datum felaktigt"));
        return;
    }

    // m_endDateRef = m_endDate;

     m_timeSlideWindowInc = value;
    int inc = - m_timeSlideWindowInc;
    cu.addDays(m_endDateRef, endDate, inc);




    qDebug() << "sw" << m_timePeriodDaysArr[m_timePeriodDaysInc].timePeriod;

    switch(m_timePeriodDaysArr[m_timePeriodDaysInc].timePeriod)
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
        intMonth = -6;
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

    ui->setEndDateLineEdit->clear();
    ui->setStartDateLineEdit->clear();
    ui->setEndDateLineEdit->insert(endDate);
    ui->setStartDateLineEdit->insert(startDate);

}


/*******************************************************************
 *
 * Function:    on_ZoomOutButton_clicked()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void TaAnalysis::on_ZoomOutButton_clicked()
{
     m_plot.ZoomOut();
}


/*******************************************************************
 *
 * Function:    on_togglePanButton_clicked()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void TaAnalysis::on_togglePanButton_clicked()
{
    static bool on = false;

    if(on == false)
    {
        on = true;
    }
    else
    {
        on = false;
    }

    m_plot.enablePanningMode(on);
}
