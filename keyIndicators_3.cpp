#include "keyIndicators_3.h"
#include "ui_keyIndicators_3.h"
#include "dbHndl.h"
#include <QMessageBox>
#include "inc/guiUtil/guiFinanceCtrls.h"
#include "inc/guiUtil/guiFinanceCtrls.h"




/****************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
keyIndicators_3::keyIndicators_3(QWidget *parent) :
    QDialog(parent),
    m_showWithoutFilter(false),
    ui(new Ui::keyIndicators_3)
{
    GuiFinanceCtrls gfc;


    ui->setupUi(this);

    ui->PeCheckBoxKey3->setChecked(true);
    ui->peMaxLineEditKey3->insert(QString::fromUtf8("16,0"));
    ui->peMinLineEditKey3->insert(QString::fromUtf8("4,0"));

    ui->PsCheckBoxKey3->setChecked(true);
    ui->psLineEditKey3->insert(QString::fromUtf8("1,5"));

    // Vinst/Direktavkastning
    ui->earningsToDividendRatioCheckBoxKey3->setChecked(true);
    ui->earningsToDividendRatioLineEditKey3->insert(QString::fromUtf8("1,5"));

    // Substans / Aktie pris
    ui->netAssetValueToPriceRatioCheckBoxKey3->setChecked(true);
    ui->netAssetValueToPriceRatioLineEditKey3->insert(QString::fromUtf8("0,8"));

    // Pris / eget kapital
    ui->priceToNetEquityRatioCheckBoxKey3->setChecked(true);
    ui->priceToNetEquityRatioLineEditKey3->insert(QString::fromUtf8("1,2"));

    ui->totalDebtToEquityRatioCheckBoxKey3->setChecked(false);
    ui->totalDebtToEquityRatioLineEditKey3->insert(QString::fromUtf8("75,0"));

    ui->currentRatioCheckBoxKey3->setChecked(false);
    ui->currentRatioLineEditKey3->insert(QString::fromUtf8("1.5"));

    ui->yieldCheckBoxKey3->setChecked(true);
    ui->YieldLineEditKey3->insert(QString::fromUtf8("3,5"));

     ui->turnOffFilterpushButtonKey3->setText(QString::fromUtf8("Filter påslaget"));




    gfc.addAllStockListsToCombobox(ui->stockListscomboBoxKey3);



}


/****************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
keyIndicators_3::~keyIndicators_3()
{
    delete ui;
}


/****************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
void keyIndicators_3::on_loadDataPushButtonKey3_clicked()
{
    CDbHndl db;
    GuiFinanceColor gfc;
    Qt::GlobalColor color1 = Qt::black;
    TableColumnIndicatorInfo_ST headerList[CDbHndl::YNOI_NOF_ITEMS];
    int nofCols = 0;
    int len = 3;
    int row;
    int j;
    QHeaderView *horizHeaderTable;
    bool res;
    GuiFinanceCtrls gfic;
    QString stockListName;
    int stockListId;
    CDbHndl::YahooNordnetInputkeyData_ST inData;
    CDbHndl::YahooNordnetOutputkeyData_ST resData;

    inData.peMaxIsValid = ui->PeCheckBoxKey3->isChecked();
    inData.peMinIsValid = ui->PeCheckBoxKey3->isChecked();
    inData.peMax = ui->peMaxLineEditKey3->text();
    inData.peMin = ui->peMinLineEditKey3->text();

    inData.psIsValid = ui->PsCheckBoxKey3->isChecked();
    inData.psValue = ui->psLineEditKey3->text();

    // Vinst/Direktavkastning
    inData.earningsToDividendRatioIsValid = ui->earningsToDividendRatioCheckBoxKey3->isChecked();
    inData.earningsToDividendRatio = ui->earningsToDividendRatioLineEditKey3->text();

    // Substans / Aktie pris
    inData.netAssetValueToPriceRatioIsValid = ui->netAssetValueToPriceRatioCheckBoxKey3->isChecked();
    inData.netAssetValueToPriceRatio =ui->netAssetValueToPriceRatioLineEditKey3->text();


    // Pris / eget kapital
    inData.priceToJEKRatioIsValid = ui->priceToNetEquityRatioCheckBoxKey3->isChecked();
    inData.priceToJEKRatio = ui->priceToNetEquityRatioLineEditKey3->text();

    inData.totalDebtDivEquityIsValid = ui->totalDebtToEquityRatioCheckBoxKey3->isChecked();
    inData.totalDebtDivEquity = ui->totalDebtToEquityRatioLineEditKey3->text();

    inData.currentRatioIsValid = ui->currentRatioCheckBoxKey3->isChecked();
    inData.currentRatio = ui->currentRatioLineEditKey3->text();

    inData.yieldIsValid = ui->yieldCheckBoxKey3->isChecked();
    inData.yield = ui->YieldLineEditKey3->text();

    if(m_showWithoutFilter == true)
    {
        inData.peMaxIsValid = false;
        inData.peMinIsValid = false;

        inData.psIsValid = false;

        // Vinst/Direktavkastning
        inData.earningsToDividendRatioIsValid = false;

        // Substans / Aktie pris
        inData.netAssetValueToPriceRatioIsValid = false;

        // Pris / eget kapital
        inData.priceToJEKRatioIsValid= false;

        inData.totalDebtDivEquityIsValid = false;

        inData.currentRatioIsValid = false;

        inData.yieldIsValid = false;

    }




    headerList[nofCols++].name = QString::fromUtf8("Företag     ");
    headerList[nofCols++].name = QString::fromUtf8("PE     ");
    headerList[nofCols++].name = QString::fromUtf8("PS     ");
    headerList[nofCols++].name = QString::fromUtf8("DirAvk");
    headerList[nofCols++].name = QString::fromUtf8("Vinst/\nDirAvk");
    headerList[nofCols++].name = QString::fromUtf8("Sub/\nKurs");
    headerList[nofCols++].name = QString::fromUtf8("Kurs/\nJEK");
    headerList[nofCols++].name = QString::fromUtf8("Skuld\n/Eget kap.");
    headerList[nofCols++].name = QString::fromUtf8("Balans-\nlikvid.");
    headerList[nofCols++].name = QString::fromUtf8("Vinst-\nmarginal");
    headerList[nofCols++].name = QString::fromUtf8("Rörels-\nmarginal");
    headerList[nofCols++].name = QString::fromUtf8("Räntabil.\ntot kap.");
    headerList[nofCols++].name = QString::fromUtf8("Avkast.\n eget kap.");
    headerList[nofCols++].name = QString::fromUtf8("Förv.\nRisk");
    headerList[nofCols++].name = QString::fromUtf8("Förv.\nAvkst");
    headerList[nofCols++].name = QString::fromUtf8("Idag %");
    headerList[nofCols++].name = QString::fromUtf8("Idag");
    headerList[nofCols++].name = QString::fromUtf8("Min pris\n1år");
    headerList[nofCols++].name = QString::fromUtf8("Max pris\n1 år");
    headerList[nofCols++].name = QString::fromUtf8("volym");
    headerList[nofCols++].name = QString::fromUtf8("Symbol      ");



    m_ynokd.setHorizontalFont(ui->tableViewKey3, "Helvetica", 8);
    m_ynokd.setVerticalFont(ui->tableViewKey3, "Helvetica", 8);


    res = gfic.getStockListNameAndId(ui->stockListscomboBoxKey3, stockListName, stockListId);


    if(false == res)
    {
        QMessageBox::information(NULL, QString::fromUtf8("Aktielistor"), QString::fromUtf8("Fel ingen data hittad"));
        return;
    }



    if(true == db.findTaStockListId(stockListName, stockListId))
    {
        m_stockArr.clear();

        inData.stockList = stockListName;
        inData.stockListId = stockListId;


        if(false == db.getNordnetYahooKeyData(inData, m_stockArr))
        {
            QMessageBox::information(NULL, QString::fromUtf8("Min Portfölj"), QString::fromUtf8("Fel ingen data hittad"));
            return;
        }


        len = m_stockArr.size();

        // Init table
        m_ynokd.createTableModel(len, nofCols, this);
        m_ynokd.addHeaders(ui->tableViewKey3, headerList, nofCols);

        // Init columm head click event handlers for tableView
        horizHeaderTable = ui->tableViewKey3->horizontalHeader();

        connect(horizHeaderTable,  SIGNAL(sectionClicked(int)), this, SLOT(tableHeaderClicked(int)));

        // One day is added first
        for(row = 0,  j = 0; j < len; j++)
        {
            resData = m_stockArr[j];

            m_ynokd.addDataSetTextColor(ui->tableViewKey3, resData.companyName, row, CDbHndl::YNOI_COMPANYNAME, Qt::black);
            m_ynokd.addDataSetTextColor(ui->tableViewKey3, resData.stockSymbol, row, CDbHndl::YNOI_COMPANY_SYMBOL, Qt::black);

            gfc.getColorPe(resData.pe, color1);
            m_ynokd.addDataSetTextColor(ui->tableViewKey3, resData.pe, row, CDbHndl::YNOI_PE, color1);

            color1 = Qt::black;
            gfc.getColorPs(resData.ps, color1);
            m_ynokd.addDataSetTextColor(ui->tableViewKey3, resData.ps, row, CDbHndl::YNOI_PS, color1);

            m_ynokd.addDataSetTextColor(ui->tableViewKey3, resData.profitMargin, row, CDbHndl::YNOI_PROFITMARGIN, Qt::black);
            m_ynokd.addDataSetTextColor(ui->tableViewKey3, resData.operatingMargin, row, CDbHndl::YNOI_OPERATINGMARGIN, Qt::black);
            m_ynokd.addDataSetTextColor(ui->tableViewKey3, resData.returnOnAssets, row, CDbHndl::YNOI_RETURNONASSETS, Qt::black);
            m_ynokd.addDataSetTextColor(ui->tableViewKey3, resData.returnOnEquity, row, CDbHndl::YNOI_RETURNONEQUITY, Qt::black);
            m_ynokd.addDataSetTextColor(ui->tableViewKey3, resData.week52High, row, CDbHndl::YNOI_WEEK52HIGH, Qt::black);
            m_ynokd.addDataSetTextColor(ui->tableViewKey3, resData.week52Low, row, CDbHndl::YNOI_WEEK52LOW, Qt::black);

            color1 = Qt::black;
            gfc.getColorNavDivStockPrice(resData.netAssetValueToPriceRatio, color1);
            m_ynokd.addDataSetTextColor(ui->tableViewKey3, resData.netAssetValueToPriceRatio, row, CDbHndl::YNOI_NETASSETVALUETOPRICERATIO, color1);

            m_ynokd.addDataSetTextColor(ui->tableViewKey3, resData.totalDebtToEquityRatio, row, CDbHndl::YNOI_TOTALDEBTTOEQUITYRATIO, Qt::black);
            m_ynokd.addDataSetTextColor(ui->tableViewKey3, resData.currentRatio, row, CDbHndl::YNOI_CURRENTRATIO, Qt::black);
            m_ynokd.addDataSetTextColor(ui->tableViewKey3, resData.lastPrice, row, CDbHndl::YNOI_LASTPRICE, Qt::black);
            m_ynokd.addDataSetTextColor(ui->tableViewKey3, resData.procentChangeOneDay, row, CDbHndl::YNOI_PROCENTCHANGEONEDAY, Qt::black);
            m_ynokd.addDataSetTextColor(ui->tableViewKey3, resData.volume, row, CDbHndl::YNOI_VOLUME, Qt::black);

            gfc.getColorEarningsDivDividend(resData.earningsToDividendRatio, color1);
            m_ynokd.addDataSetTextColor(ui->tableViewKey3, resData.earningsToDividendRatio, row, CDbHndl::YNOI_EARNINGSTODIVIDENDRATIO, color1);

            gfc.getColorYield(resData.yield, resData.earningsToDividendRatio, color1);
            m_ynokd.addDataSetTextColor(ui->tableViewKey3, resData.yield, row, CDbHndl::YNOI_YIELD, color1);

            m_ynokd.addDataSetTextColor(ui->tableViewKey3, resData.priceToJEKRatio, row, CDbHndl::YNOI_PRICETOJEKRATIO, Qt::black);
            row++;
        }
    }
    else
    {
        QMessageBox::information(NULL, QString::fromUtf8("Min Portfölj"), QString::fromUtf8("Fel ingen data hittad"));
        return;
    }

    ui->tableViewKey3->resizeColumnsToContents();
}


/**********************************************************************************
 *
 * Function:    tablePlusHeaderClicked()
 *
 *
 * Description:
 *
 *
 *
 ********************************************************************************/
void keyIndicators_3::tableHeaderClicked(int column)
{
    static bool toggle = false;

    if(true == toggle)
    {
        toggle = false;
        ui->tableViewKey3->sortByColumn(column, Qt::AscendingOrder);
    }
    else
    {
        toggle = true;
        ui->tableViewKey3->sortByColumn(column, Qt::DescendingOrder);
    }

}



void keyIndicators_3::on_turnOffFilterpushButtonKey3_clicked()
{

    if(m_showWithoutFilter == true)
    {
        m_showWithoutFilter = false;
        ui->turnOffFilterpushButtonKey3->setText(QString::fromUtf8("Filter påslaget"));
    }
    else
    {
        m_showWithoutFilter = true;
        ui->turnOffFilterpushButtonKey3->setText(QString::fromUtf8("Filter avstängt"));
    }

}
