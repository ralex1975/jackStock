/*********************************************************************
 * File: EfficientPortfolio.cpp
 *
 * Description: .
 *
 *********************************************************************/

#include "efficientPortfolio.h"
#include "ui_efficientPortfolio.h"
#include <QtCore>
#include "util.h"
#include "dbHndl.h"
#include "common.h"
#include <QItemSelectionModel>
#include "inc/guiUtil/guiFinanceCtrls.h"
#include <stdlib.h>




/*********************************************************************
 * Function: ()
 *
 * Description: .
 *
 *********************************************************************/
EfficientPortfolio::EfficientPortfolio(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EfficientPortfolio),
    m_comboBoxIndex(0)
{

    ui->setupUi(this);

    ui->checkBoxShowNumberInGraph->setChecked(false);

    m_plot.initPlotPicker(ui->qwtPlot);
    m_plot.initPlotZoomer(ui->qwtPlot);
    m_plot.enableZoomMode(true);


    // Just create dummy space so we can delete it later before we create the space we need
    createQwtPlotArrMemSpace(2);
    m_nofRiskReturPlotData = 2;

    m_plotflashLabel = new QwtPlotMarker;


    // Create thread that import data from yahoo
    m_threadCalcEfficentFront = new threadCalcEfficentFront(this);

    if(m_threadCalcEfficentFront == 0)
    {
        QMessageBox::information(NULL, QObject::tr("Create thread"), QString::fromUtf8("Fail to create thread"));
    }

    ui->tabWidget->setCurrentIndex(0);
    initDateCtrls();
    initStockList();
    openUserStockListsFile();

     connect(ui->tableView->horizontalHeader() , SIGNAL(sectionClicked(int)), this, SLOT(slotHorHeaderClicked(int)));
     connect(ui->tableView->verticalHeader() , SIGNAL(sectionClicked(int)), this, SLOT(slotVertHeaderClicked(int)));

     connect( ui->treeWidget->header(), SIGNAL( sectionDoubleClicked(int) ), this, SLOT( slotTreeHeaderDoubleClick(int) ) );

}


#if 0
/*********************************************************************
 * Function: ()
 *
 * Description: This function is invoked when user click on a checkbox in
 *              treeWidget. .
 *
 *********************************************************************/
void EfficientPortfolio::updateitem ( QTreeWidgetItem* item ,int col)
{

    //if(Qt::Checked == item->checkState(col))
        //qDebug() << "Hej hopp checked";
    //else
    //    &&qDebug() << "Hej hopp Unchecked";
}
#endif

/*********************************************************************
 * Function: ()
 *
 * Description: .
 *
 *********************************************************************/
EfficientPortfolio::~EfficientPortfolio()
{
    delete ui;
    delete m_threadCalcEfficentFront;
    delete [] m_riskReturPlotArr;
    delete [] m_riskReturTxtArr;
    delete m_xAxisPlot;
    delete m_plotGrid;
    delete m_plotflashLabel;

}


/*********************************************************************
 * Function: ()
 *
 * Description: .
 *
 *********************************************************************/
void EfficientPortfolio::slotVertHeaderClicked(int index)
{
    QString str;
    int nofCol = m_extendedtable.getNofCols();
    double x = 0;

    m_efficentBarGraphDlg.m_efficientFrontHistData.clear();
    for(int i = 0; i< nofCol;i++)
    {
        m_extendedtable.getOneData(index, i, str);

        QwtInterval interval(x, x+1);
        interval.setBorderFlags( QwtInterval::ExcludeMaximum | QwtInterval::ExcludeMinimum);
        m_efficentBarGraphDlg.m_efficientFrontHistData.append (QwtIntervalSample (str.toDouble(),interval));

        x++;
    }

    m_efficentBarGraphDlg.initCorrelationTable(ui->treeWidget);
    m_efficentBarGraphDlg.show();
    m_efficentBarGraphDlg.plot();

}

/*********************************************************************
 * Function: ()
 *
 * Description: .
 *
 *********************************************************************/
void EfficientPortfolio::slotHorHeaderClicked(int index)
{
    QMessageBox::critical(NULL, QObject::tr("tableview"), QObject::tr("Horz tableview"));
}




/*********************************************************************
 * Function: ()
 *
 * Description:
 *
 *
 *
 *********************************************************************/
bool EfficientPortfolio::createMemSpace(int nofStocks)
{
    CUtil cu;
    bool res;

    res = cu.createMatrixMemSpace(nofStocks, &m_varCovMatrix);

    if(res == false)
    {
        return false;
    }

    res = cu.createMatrixMemSpace(nofStocks, &m_correlationMatrix);

    if(res == false)
    {
        return false;
    }


    res = cu.createMatrixMemSpace(nofStocks, &m_weightCorrResultMatrix);

    if(res == false)
    {
        return false;
    }



    res = cu.createArrMemSpace(nofStocks, &m_expectedReturnArr);

    if(res == false)
    {
        return false;
    }

    res = cu.createArrMemSpace(nofStocks, &m_expectedRiskArr);

    if(res == false)
    {
        return false;
    }

    res = cu.createArrMemSpace(nofStocks, &m_weightArr);

    if(res == false)
    {
        return false;
    }


    res = createArrMemSpace(nofStocks, &m_stockArr);

    if(res == false)
    {
        return false;
    }
    return true;
}




/*********************************************************************
 * Function:    on_pushButtSel_clicked()
 *
 * Description: This function is invoked when select button is pressed.
 *
 *********************************************************************/
void EfficientPortfolio::on_pushButtSel_clicked()
{
    openUserStockListsFile();
}


/*********************************************************************
 * Function: createArrMemSpace()
 *
 * Description: .
 *
 *********************************************************************/
bool EfficientPortfolio::createArrMemSpace(int nofStocks, CDbHndl::EfficPortStockData_ST **arr)
{
    // Create variances covariances matrix space
    (*arr) = new CDbHndl::EfficPortStockData_ST [nofStocks + 5];

    if((*arr) == 0)
    {
        return false;
    }
    return true;
}


/*********************************************************************
 * Function: createArrMemSpace()
 *
 * Description: .
 *
 *********************************************************************/
void EfficientPortfolio::initDateCtrls(void)
{
    CUtil cu;

    QString endDate = "2013-12-30";
    QString startDate = "2013-01-20";
    int nofYears = -1;

    cu.getCurrentDate(endDate);
    cu.addYear(endDate, startDate, nofYears);

    ui->lineEditEndDate->insert(endDate);
    ui->lineEditStartDate->insert(startDate);
}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void EfficientPortfolio::addCorrelationDataToTable(void)
{
    QString endDate = ui->lineEditEndDate->text();
    QString startDate = ui->lineEditStartDate->text();

    createMemSpace(m_stockArr1.size());

     // mainTraversTreeWidget(m_stockArr1);

    m_threadCalcEfficentFront->initCalcEfficPortData(startDate,
                                                     endDate,
                                                     &ui->tableView,
                                                     m_extendedtable,
                                                     m_varCovMatrix,
                                                     m_correlationMatrix,
                                                     m_stockArr1);
    m_threadCalcEfficentFront->start(QThread::HighPriority);
}

/*******************************************************************
 *
 * Function:    ()
 *
 * Description: This function traverse treewidget and check which
 *              of the nodes that are checked in column 1
 *
 *******************************************************************/
void EfficientPortfolio::mainTraversTreeWidget(void)
{
    CUtil cu;
    int numberOfTopLevelItems = ui->treeWidget->topLevelItemCount();
    for ( int topLevelindex=0 ; topLevelindex < numberOfTopLevelItems ; topLevelindex++)
    {
        QTreeWidgetItem *item = ui->treeWidget->topLevelItem(topLevelindex);
        item->setBackground(2,cu.getQColor((CUtil::ColorRgb_ET)topLevelindex) /*QColor(202, 0, 0, 255)*/);
        // qDebug() << "top_0" << 01 << "#" << item->text(0);
        // qDebug() << "top_1" << 1 << "#" << item->text(1);
         // qDebug() << "top_2" << 1 << "#" << stockArr1[topLevelindex].stockSymbol;

         // if(stockArr1[topLevelindex].stockSymbol==item->text(1))
         {
             if(Qt::Checked == ui->treeWidget->topLevelItem(topLevelindex)->checkState(2))
             {
                 // stockArr1[topLevelindex].isSelected = true;
                 // qDebug() << "top" << 2 << "state" << "Qt::checked";
                 // item->setCheckState(3, Qt::Checked);
             }
             else
             {
                 m_stockArr1[topLevelindex].isSelected = false;
                 // qDebug() << "top" << 1 << "state" << "Qt::Unchecked";
                 // item->setCheckState(3, Qt::Unchecked);
             }
         }
//         else
//         {
//             qDebug() << "stockSymbol" << m_stockArr1[topLevelindex].stockSymbol;
//             qDebug() << "ArrStockymbol" << item->text(1);
//             QMessageBox::critical(this, QObject::tr("Missmatch"), QObject::tr("Error treewidget and stock array do not mach"));
//             return;
//         }

        // Not needed we have no children
        processItem(item, 1);
    }
}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description: Note in this treewidget we have no child nodes
 *              So we do not need this function.
 *
 *
 *******************************************************************/
void EfficientPortfolio::processItem(QTreeWidgetItem * parent, int level)
{
    for (int childIndex = 0 ; childIndex < parent->childCount(); childIndex++)
    {
        QTreeWidgetItem *child = parent->child(childIndex);

        if (child->checkState(2) == Qt::Unchecked)
        {
            qDebug() << "child" << 1 << "state" << "Qt::Unchecked";
            // item->setCheckState(Qt::Checked);
        }
        else
        {
            qDebug() << "child" << 1 << "state" << "Qt::Unchecked";
            // item->setCheckState(Qt::Unchecked);
        }

        qDebug() << "#" << level << "#" << child->text(0);
        processItem(child, level + 1);
    }
}


/*********************************************************************
 * Function: ()
 *
 * Description: .
 *
 *********************************************************************/
void EfficientPortfolio::deleteQwtPlotData(int nofStocks)
{
    int i;

    // Clear graph
    for(i=0; i < nofStocks; i++)
    {
        m_riskReturPlotArr[i].detach();
        m_riskReturPlotArr[i].setData(NULL);
    }
}




/*********************************************************************
 * Function: ()
 *
 * Description: .
 *
 *********************************************************************/
void EfficientPortfolio::flashLabelOnPlot(int index, double x, double y, QwtPlot *qwtPlot)
{
    //static bool showLabel = true;
    static int oldIndex;
    QString label;

    if(oldIndex != index)
    {
        //showLabel = false;
        label.sprintf("%d.", index);

        QwtText text(label);
        text.setFont( QFont( "Helvetica", 14, QFont::Bold) );
        text.setColor( QColor(Qt::black));
        text.setBackgroundBrush(QColor(Qt::white));


        m_plotflashLabel->setLabel( text );
        m_plotflashLabel->setValue((x), y);

        m_plotflashLabel->attach(qwtPlot );
        oldIndex = index;
    }
    else
    {
        oldIndex = -1;
        m_plotflashLabel->detach();
        // m_plotflashLabel->setData(NULL);
    }


    qwtPlot->replot();

}



/*********************************************************************
 * Function: ()
 *
 * Description: .
 *
 *********************************************************************/
void EfficientPortfolio::enterPlotLabel(int index, double x, double y, QwtPlot *qwtPlot)
{
    QString label;
    label.sprintf("%d.", index);

    QwtText text(label);
    text.setFont( QFont( "Helvetica", 14, QFont::Bold) );
    text.setColor( QColor(Qt::black));
    text.setBackgroundBrush(QColor(Qt::white));


    m_riskReturTxtArr[index].setLabel( text );
    m_riskReturTxtArr[index].setValue((x), y);

    m_riskReturTxtArr[index].attach(qwtPlot );

}


/*********************************************************************
 * Function: ()
 *
 * Description: .
 *
 *********************************************************************/
void EfficientPortfolio::plotXAxis(QwtPlot *qwtPlot)
{
    //QwtPlotGrid *grid = new QwtPlotGrid;
    m_plotGrid->enableXMin(true);
    m_plotGrid->setMajPen(QPen(Qt::darkYellow, 0, Qt::DotLine));
    m_plotGrid->setMinPen(QPen(Qt::darkYellow, 0 , Qt::DotLine));
    m_plotGrid->attach(qwtPlot);

    double x[2];
    double y[2];

    double minX, minY, maxX, maxY;

    if(m_minMaxReturPlotArr.minX >= 0)
        minX = m_minMaxReturPlotArr.minX*0.9;
    else
        minX = m_minMaxReturPlotArr.minX*1.1;


    if(m_minMaxReturPlotArr.minY >= 0)
        minY = m_minMaxReturPlotArr.minY*0.9;
    else
        minY = m_minMaxReturPlotArr.minY*1.1;

    if(m_minMaxReturPlotArr.maxX >= 0)
        maxX = m_minMaxReturPlotArr.maxX*1.1;
    else
        maxX = m_minMaxReturPlotArr.maxX*0.9;


    if(m_minMaxReturPlotArr.maxY >= 0)
        maxY = m_minMaxReturPlotArr.maxY*1.1;
    else
        maxY = m_minMaxReturPlotArr.maxY*0.9;



    x[0] = minX;
    x[1] = maxX;
    y[0] = 0;
    y[1] = 0;
    m_xAxisPlot->setSamples(x,y, 2);
    m_xAxisPlot->setPen(QPen( Qt::black, 2));


    qwtPlot->setAxisScale(QwtPlot::xBottom,
                          minX,
                          (maxX));
    qwtPlot->setAxisScale(QwtPlot::yLeft,
                          minY,
                          (maxY)); // Max av % satser

    m_xAxisPlot->attach(qwtPlot);

}



/*********************************************************************
 * Function: ()
 *
 * Description: .
 *
 *********************************************************************/
void EfficientPortfolio::plotQwtData(int nofCurves, QwtPlot *qwtPlot)
{
    CUtil cu;

    #if 1
    for(int i = 0; i < nofCurves; i++)
    {
        m_riskReturPlotArr[i].setSymbol( new QwtSymbol( QwtSymbol::Ellipse, cu.getQColor((CUtil::ColorRgb_ET)i)/*Qt::black*/, QPen(cu.getQColor((CUtil::ColorRgb_ET)i)/*Qt::red*/ ), QSize( 15, 15 ) ) );
        m_riskReturPlotArr[i].setPen( QPen( Qt::black) );
        m_riskReturPlotArr[i].attach(qwtPlot);
    }
    #endif
    qwtPlot->replot();
}


/*********************************************************************
 * Function: ()
 *
 * Description: .
 *
 *********************************************************************/
bool EfficientPortfolio::createQwtPlotArrMemSpace(int nofStocks)
{

    // Create variances covariances matrix space
    m_riskReturPlotArr = new QwtPlotCurve [nofStocks + 5];
    m_nofRiskReturPlotData = nofStocks;

    if((m_riskReturPlotArr) == 0)
    {
        return false;
    }

    m_riskReturTxtArr = new QwtPlotMarker [nofStocks + 5];

    if((m_riskReturTxtArr) == 0)
    {
        return false;
    }


    m_xAxisPlot = new QwtPlotCurve;

    if((m_xAxisPlot) == 0)
    {
        return false;
    }


    m_plotGrid = new QwtPlotGrid;

    if((m_plotGrid) == 0)
    {
        return false;
    }

    return true;
}


/*********************************************************************
 * Function: ()
 *
 * Description: .
 *
 *********************************************************************/
void EfficientPortfolio::removeQwtPlotArrMemSpace(void)
{
    delete [] m_riskReturPlotArr;
    delete [] m_riskReturTxtArr;
    delete m_xAxisPlot;
    delete m_plotGrid;
}

/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void EfficientPortfolio::uppdateStockArrWithRiskAndReturn(void)
{
    CUtil cu;
    QString filename;
    CDbHndl db;
    CDbHndl::EfficPortStockData_ST data;
    int row;


    removeQwtPlotArrMemSpace();
    createQwtPlotArrMemSpace(m_stockArr1.size());
    m_minMaxReturPlotArr.minMaxIsInit = false;


    ui->treeWidget->clear();

    QString tmp;
    double x;
    double y;

    QString startDate = ui->lineEditStartDate->text();
    QString endDate = ui->lineEditEndDate->text();
    int colorNumber = 0;

    filename = QString::fromUtf8("RiskAndReturn_");
    filename += ui->StockListComboBox->currentText();
    filename += "_";
    filename += endDate;
    filename += "_";
    filename += startDate;
    filename += ".txt";


    QFile caFile(filename);

    caFile.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!caFile.isOpen())
    {
       qDebug() << "Error: open file";
       QMessageBox::information(NULL, QString::fromUtf8("Fail to open file"), QString::fromUtf8("Fail to open file"));
       return;
    }

    QTextStream outStream(&caFile);



    outStream << "Stockname|" << "Risk|" << "Return|" << "startDate|"     << "endDate" << "|\n";
    outStream <<          "|" <<     "|" <<       "|" << startDate << "|" << endDate   << "|\n";


    for(row = 0; row < m_stockArr1.size(); row++)
    {
        data = m_stockArr1.at(row);
        db.efficPortfCalcMeanAndStdDev(startDate, endDate, data);
        m_stockArr1[row] = data;


        if(data.isValid == true)
        {
            qDebug() << "valid risk return" << data.stockName;
            // Show data on graph
            x = data.riskStdDev * 100;
            y = data.meanReturns * 100;
            updateMinMax(x, y);
            m_riskReturPlotArr[row].setSamples(&x, &y, 1);

            // Display digits as label
            if(ui->checkBoxShowNumberInGraph->isChecked() == true)
                enterPlotLabel(row, x, y, ui->qwtPlot);
            // qDebug() << data.riskStdDev <<  data.meanReturns;

            QString str;
            if(ui->checkBoxShowNumberInGraph->isChecked()== true)
            {
                str.sprintf("%d ", row);
                str += data.stockName;
            }
            else
            {
                str = data.stockName;
            }
            QTreeWidgetItem *item = new QTreeWidgetItem;
            item->setText(CDbHndl::TWColum_Name, str);
            item->setText(CDbHndl::TWColum_SYMBOL, data.stockSymbol);
            outStream  << data.stockName << "|";

            item->setExpanded(true);
                           item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
                           item->setCheckState(CDbHndl::TWCOLUM_IS_SELECTED, Qt::Checked); // Qt::Unchecked
                           item->setBackground(CDbHndl::TWCOLUM_IS_SELECTED,cu.getQColor((CUtil::ColorRgb_ET) colorNumber) /*QColor(202, 0, 0, 255)*/);
                           item->setCheckState(CDbHndl::TWCOLUM_DEFAULT_SETTINGS, Qt::Unchecked); // Qt::Unchecked
                           item->setText(CDbHndl::TWCOLUM_IS_SELECTED, " ");
                           item->setText(CDbHndl::TWCOLUM_DEFAULT_SETTINGS, " ");

            tmp.sprintf("%.1f", data.riskStdDev*100);
            item->setText(CDbHndl::TWCOLUM_RISK, tmp);
            outStream  << tmp << "|";


            tmp.sprintf("%.1f", data.meanReturns*100);
            item->setText(CDbHndl::TWCOLUM_RETURN, tmp);
            outStream  << tmp << "|||\n";


            tmp.sprintf("%d", 0);
            item->setText(CDbHndl::TWCOLUM_MIN_PROCENT, tmp);
            tmp.sprintf("%d", 100);
            item->setText(CDbHndl::TWCOLUM_MAX_PROCENT, tmp);
            tmp.sprintf("%d", 12);
            item->setText(CDbHndl::TWCOLUM_SEL_PROCENT, tmp);
            tmp.sprintf("%ld", data.nofSamples);
            item->setText(CDbHndl::TWCOLUM_NOF_DATA, tmp);
            tmp.sprintf("%d", colorNumber);
            item->setText(CDbHndl::TWCOLUM_COLOR_NUMBER, tmp);
            colorNumber++;


            ui->treeWidget->addTopLevelItem(item);
        }
        else
        {
            qDebug() << "invalid risk return calc" << data.stockName;
        }
    }
    caFile.close();

}



/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void EfficientPortfolio::initCorrelationTable(void)
{
    // QString tmp;

    // m_extendedtable.setFont(QFont::Helvetica);
    m_extendedtable.createTableModel(m_stockArr1.size(), m_stockArr1.size(), this);
    m_extendedtable.addHorzVertHeaders(ui->tableView, m_stockArr1, m_stockArr1.size());

    // ui->tableView->resizeColumnsToContents();
    m_extendedtable.setHorizontalFont(ui->tableView, "Helvetica", 9);
    m_extendedtable.setVerticalFont(ui->tableView, "Helvetica", 9);

}

/*******************************************************************
 *
 * Function:    ()
 *
 * Description: ska tas bort
 *
 *
 *******************************************************************/
void EfficientPortfolio::setTableWidth(int nofColumns)
{
    int totColumnWith = 0;
    int i;

    for(i = 0; i < nofColumns; i++)
    {
       totColumnWith += ui->tableView->columnWidth(i);
    }

    QRect rect = ui->tableView->geometry();
    rect.setWidth(1 + ui->tableView->verticalHeader()->width() + totColumnWith);
    ui->tableView->setGeometry(rect);
}

/*******************************************************************
 *
 * Function:    initStockList()
 *
 * Description:
 *
 *
 *******************************************************************/
void EfficientPortfolio::initStockList(void)
{

    QString column0 = QString::fromUtf8("Namn");
    QString column1 = QString::fromUtf8("Sym");
    QString column2 = QString::fromUtf8("Ink");
    QString column3 = QString::fromUtf8("Std");
    QString column4 = QString::fromUtf8("Förv.\nRisk");
    QString column5 = QString::fromUtf8("Förv.\nAvkst");
    QString column6 = QString::fromUtf8("Min\nvikt");
    QString column7 = QString::fromUtf8("Max\nvikt");
    QString column8 = QString::fromUtf8("Vald\nvikt");
    QString column9 = QString::fromUtf8("Antal");



    ui->treeWidget->setColumnCount(10);
    ui->treeWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->treeWidget->setColumnWidth(0, 120);
    ui->treeWidget->setColumnWidth(1, 0);
    ui->treeWidget->setColumnWidth(2, 30);
    ui->treeWidget->setColumnWidth(3, 30);
    ui->treeWidget->setColumnWidth(4, 40);
    ui->treeWidget->setColumnWidth(5, 40);
    ui->treeWidget->setColumnWidth(6, 40);
    ui->treeWidget->setColumnWidth(7, 40);
    ui->treeWidget->setColumnWidth(8, 40);
    ui->treeWidget->setColumnWidth(9, 40);

    if(QTreeWidgetItem* header = ui->treeWidget->headerItem())
    {
        header->setText(0, column0.toLatin1());
        header->setText(1, column1.toLatin1());
        header->setText(2, column2.toLatin1());
        header->setText(3, column3.toLatin1());
        header->setText(4, column4.toLatin1());
        header->setText(5, column5.toLatin1());
        header->setText(6, column6.toLatin1());
        header->setText(7, column7.toLatin1());
        header->setText(8, column8.toLatin1());
        header->setText(9, column9.toLatin1());
    }

    ui->treeWidget->hideColumn(1);
}



/*******************************************************************
 *
 * Function:    on_OpenFileButton_clicked()
 *
 * Description:
 *
 *
 *******************************************************************/
void EfficientPortfolio::openUserStockListsFile(void)
{
    QString filename = DWLD_PATH_TA_STOCKLIST_FILE;


    m_db.delAllTaStockLists();
    m_db.delAllTaStockNames();

    if(false == openStockListFile(filename))
    {
        QMessageBox::information(NULL, QObject::tr("Open file"), QObject::tr("Fail to open file"));
    }
    addDataToStockListAndStockListCombo();

}



/*******************************************************************
 *
 * Function:    openStockListFile()
 *
 * Description: This function open the main file that contains
 *              all user defined stock lists stored as csv files
 *              and add the name of these list into the stocklist
 *              combobox.
 *
 *
 *******************************************************************/
bool EfficientPortfolio::openStockListFile(QString filename)
{
    QFile file(filename);
    QString str;
    QString stockListName;
    QString stockNameFile;


    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QString errStr = QString("Fail to open file: %1").arg(filename);
        QMessageBox::critical(NULL, QObject::tr("Fail to open file"), errStr);
        return false;
    }

    QTextStream inStream(&file);

    while(!inStream.atEnd())
    {
        str = inStream.readLine();
        qDebug() << str << "\n";

        // Get filename of stockname
        if(str.indexOf("\t") != -1)
        {
            stockListName = str.left(str.indexOf("\t"));
            qDebug() << stockListName;
        }

        // Get filename of stockname
        if(str.indexOf("\t") != -1)
        {
            stockNameFile = qApp->applicationDirPath();
            stockNameFile += DWLD_PATH_TA_INPUT_DATA_PATH;
            stockNameFile += str.right(str.length() - str.indexOf("\t")-1);
            stockNameFile.trimmed();


            if(false == m_db.addTaStockList(stockListName))
            {
                str.sprintf("Kan inte lägga till namn i stockList");
                QMessageBox::critical(NULL, QString::fromUtf8("Lägga till data"), str.toLocal8Bit().constData());
                return false;
            }
            else
            {
                openStockNameFile(stockNameFile, stockListName);
            }
        }
    }

    qDebug() << "close file\n";
    file.close();
    return true;
}


/*******************************************************************
 *
 * Function:    openStockListFile()
 *
 * Description:
 *
 *
 *******************************************************************/
bool EfficientPortfolio::openStockNameFile(QString filename, QString stockListName)
{
    QFile file(filename);
    QString str;
    QString stockSymbol;
    QString stockName;
    int stockListId;


    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QString errStr = QString("Fail to open file: %1").arg(filename);
        QMessageBox::critical(NULL, QObject::tr("Fail to open file"), errStr);
        return false;
    }

    m_db.openDb(PATH_JACK_STOCK_DB, true);


    QTextStream inStream(&file);



    while(!inStream.atEnd())
    {
        str = inStream.readLine(); //.toUtf8();


        // Get filename of stockname
        if(str.indexOf("\t") != -1)
        {
            stockSymbol = str.left(str.indexOf("\t"));
            qDebug() << stockSymbol;
        }

        // Get filename of stockname
        if(str.indexOf("\t") != -1)
        {
            stockName = str.right(str.length() - str.indexOf("\t")-1);
            qDebug() << stockName;
        }

        // QString stockListName = QObject::tr("Stockholm OMX");

        if(true == m_db.findTaStockListId(stockListName, stockListId, true))
        {
            m_db.addTaStockName(stockSymbol, stockName, stockListId, true);
            addStatusDateList(stockSymbol, false);
        }
    }

    qDebug() << "close file\n";
    file.close();
    m_db.closeDb(true);
    return true;
}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void EfficientPortfolio::updateMinMax(double x, double y)
{
    if(m_minMaxReturPlotArr.minMaxIsInit == false)
    {
        m_minMaxReturPlotArr.minMaxIsInit = true;
        m_minMaxReturPlotArr.maxX = x;
        m_minMaxReturPlotArr.minX = x;

        m_minMaxReturPlotArr.maxY = y;
        m_minMaxReturPlotArr.minY = y;
    }
    else
    {
        if(x > m_minMaxReturPlotArr.maxX)
            m_minMaxReturPlotArr.maxX = x;

        if(x < m_minMaxReturPlotArr.minX)
            m_minMaxReturPlotArr.minX = x;

        if(y > m_minMaxReturPlotArr.maxY)
            m_minMaxReturPlotArr.maxY = y;

        if(y < m_minMaxReturPlotArr.minY)
            m_minMaxReturPlotArr.minY = y;
     }
}




/*******************************************************************
 *
 * Function:    addDataToStockListAndStockListCombo()
 *
 * Description: Populates comboBox with all user defined stock lists
 *              and populate list ctrl with all stocks in selected
 *              stock list.
 *
 *******************************************************************/
void EfficientPortfolio::addDataToStockListAndStockListCombo(void)
{
   QString stockListName;
   int stockListId;
   int cnt = 0;

    // ui->StockListComboBox->clear();
   if(false == m_db.addStockListsToComboBox(ui->StockListComboBox))
   {
       return;
   }

   // First time we start up system just select first item displayed in combobox
   //if(initSystem == true)
   {
        ui->StockListComboBox->setCurrentIndex(m_comboBoxIndex);
   }


    stockListName = ui->StockListComboBox->currentText().toLatin1();

    if(true == m_db.findTaStockListId(stockListName, stockListId))
    {
        ui->treeWidget->clear();
        m_stockArr1.clear();
        m_db.addStockSymbolAndNameInTreeWidget1(ui->treeWidget,
                                                m_stockArr1,
                                                stockListId,
                                                CDbHndl::SORT_TWIDGET_NAME,
                                                (char *) SQL_STR_ASC);

        cnt++;
        // ui->treeWidget->setColumnWidth(0, 170);
        ui->treeWidget->scrollToTop();
        ui->treeWidget->setCurrentItem(ui->treeWidget->topLevelItem(0));
    }

    initCorrelationTable();
    uppdateStockArrWithRiskAndReturn();
    mainTraversTreeWidget();
    plotXAxis(ui->qwtPlot);
    plotQwtData(m_nofRiskReturPlotData, ui->qwtPlot);
}



/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void EfficientPortfolio::addStatusDateList(QString stockSymbol, bool isUpdate)
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
 * Description: This function is invoked when user dubble click om
 *              tableview.
 *
 *
 *******************************************************************/
void EfficientPortfolio::on_tableView_clicked(const QModelIndex &index)
{

    qDebug() <<  ui->tableView->model()->data(index).toString();
    //ui->tableView->horizontalHeader()->sectionClicked(1);
    //ui->tableView->selectRow(index);

    // QItemSelectionModel * QAbstractItemView::selectionModel () const
    // QModelIndexList QItemSelectionModel::selectedRows ( int column = 0 ) const
    // myTableView->selectionModel()->selectedRows();
    // void QItemSelectionModel::selectionChanged ( const QItemSelection & selected, const QItemSelection & deselected )   [signal]



}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description: calc efficent front
 *
 *
 *
 *******************************************************************/
void EfficientPortfolio::on_pushButtCalc_clicked()
{
    addCorrelationDataToTable();
}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void EfficientPortfolio::on_StockListComboBox_currentIndexChanged(int index)
{
    ui->StockListComboBox->setCurrentIndex(index);
    if(index >= 0)
        m_comboBoxIndex = index;
    qDebug() << "m_comboBoxIndex" << m_comboBoxIndex;
}

/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void EfficientPortfolio::on_treeWidget_doubleClicked(const QModelIndex &index)
{
    double risk;
    double meanReturn;

    getRiskReturFromTreeWidget(risk, meanReturn, index);

    flashLabelOnPlot(index.row(), risk, meanReturn, ui->qwtPlot);


}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void EfficientPortfolio::
getRiskReturFromTreeWidget(double &risk, double &meanReturn, const QModelIndex &index)
{
    QString tmp;
    ui->treeWidget->setCurrentItem(ui->treeWidget->topLevelItem(index.row()));
    ui->treeWidget->setFocus();


    tmp = ui->treeWidget->currentItem()->text(CDbHndl::TWCOLUM_RISK);
    risk = tmp.toDouble();
    tmp = ui->treeWidget->currentItem()->text(CDbHndl::TWCOLUM_RETURN);
    meanReturn = tmp.toDouble();

}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void EfficientPortfolio::slotTreeHeaderDoubleClick(int column)
{
    static int oldColumn;
    static bool pending = false;

    CDbHndl db;
    db.openDb(PATH_JACK_STOCK_DB);
    bool dbIsHandledExternly = true;


    db.delAllTblTmpEfficPortTreeWidget(dbIsHandledExternly);
    copyTreeWidgetDataToTmpDb(db, dbIsHandledExternly);

    if(pending == false)
    {
        pending = true;

        if(oldColumn != column)
        {
            oldColumn = column;
            db.addTmpEfficPortDataToTreeWidget(ui->treeWidget,
                                               column,
                                               (char *)SQL_STR_ASC,dbIsHandledExternly);
        }
        else
        {
            db.addTmpEfficPortDataToTreeWidget(ui->treeWidget,
                                               column,
                                               (char *)SQL_STR_DESC, dbIsHandledExternly);
        oldColumn = -1;
        }
        pending = false;
        db.closeDb(true);
    }
#if 0
    if(oldColumn != column)
    {
        ui->treeWidget->setSortingEnabled(true);
        ui->treeWidget->sortItems(column, Qt::AscendingOrder);
        ui->treeWidget->sortByColumn(column, Qt::AscendingOrder);
        // QMessageBox::information(this, "Treewidget", "Header clicked");
    }
    else
    {
        ui->treeWidget->setSortingEnabled(true);
        ui->treeWidget->sortItems(column, Qt::DescendingOrder);
        ui->treeWidget->sortByColumn(column, Qt::DescendingOrder);

    }
#endif


}



/*******************************************************************
 *
 * Function:    ()
 *
 * Description: This function traverse treewidget and check which
 *              of the nodes that are checked in column 1
 *
 *******************************************************************/
void EfficientPortfolio::copyTreeWidgetDataToTmpDb(CDbHndl &db, bool dbIsHandledExternly)
{
    int numberOfTopLevelItems = ui->treeWidget->topLevelItemCount();
    for ( int topLevelindex=0 ; topLevelindex < numberOfTopLevelItems ; topLevelindex++)
    {
        QString tmp;
        double risk;
        double meanReturn;
        int minProcent;
        int maxProcent;
        int selProcent;
        int nofItems;
        int colorNumber;

        QTreeWidgetItem *item = ui->treeWidget->topLevelItem(topLevelindex);


        tmp = item->text(CDbHndl::TWCOLUM_RISK);
        risk = tmp.toDouble();

        tmp = item->text(CDbHndl::TWCOLUM_RETURN);
        meanReturn = tmp.toDouble();

        tmp = item->text(CDbHndl::TWCOLUM_MIN_PROCENT);
        minProcent = tmp.toInt();

        tmp = item->text(CDbHndl::TWCOLUM_MAX_PROCENT);
        maxProcent = tmp.toInt();

        tmp = item->text(CDbHndl::TWCOLUM_SEL_PROCENT);
        selProcent = tmp.toInt();

        tmp = item->text(CDbHndl::TWCOLUM_NOF_DATA);
        nofItems = tmp.toInt();

        tmp = item->text(CDbHndl::TWCOLUM_COLOR_NUMBER);
        colorNumber = tmp.toInt();


        db.insertTmpEfficPortTreeWidgetData(item->text(CDbHndl::TWColum_Name),
                                            item->text(CDbHndl::TWColum_SYMBOL),
                                            (bool) item->checkState(CDbHndl::TWCOLUM_IS_SELECTED),
                                            (bool) item->checkState(CDbHndl::TWCOLUM_DEFAULT_SETTINGS),
                                            risk,
                                            meanReturn,
                                            minProcent,
                                            maxProcent,
                                            selProcent,
                                            nofItems,
                                            colorNumber,
                                            dbIsHandledExternly);

        // Not needed we have no children
        //processItem(item, 1);
    }
}
