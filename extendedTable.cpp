/*******************************************************************
 *
 * Filename: extendedTable.cpp
 * Date:     2012-09-08
 *
 * Description: This class contains a number of functions that
 *              makes it easer to use the use the tableView
 *
 *
 *******************************************************************/

#include "extendedTable.h"


/*******************************************************************
 *
 * Function:    CExtendedTable()
 *
 * Description: Constructor
 *
 *
 *
 *******************************************************************/
CExtendedTable::CExtendedTable():
                            m_model(0)
{
}


// Ta reda på hur många rader som är valda
//QItemSelectionModel* selectionModel = ui->tableView->selectionModel();

// QModelIndexList selected = selectionModel->selectedRows();
//  QModelIndex index = selected.at(i);






/*******************************************************************
 *
 * Function:    CExtendedTable()
 *
 * Description: Constructor
 *
 *
 *
 *******************************************************************/
void CExtendedTable::createTableModel(int maxRow, int maxColumn, QObject *parent)
{
    if(m_model)
        delete m_model;

    m_model = new QStandardItemModel(maxRow, maxColumn, parent);

}


/*******************************************************************
 *
 * Function:    clearHeaderAndData()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void CExtendedTable::clearHeaderAndData(QTableView *tableView)
{
    m_model->clear();
    tableView->setModel(m_model);
}

/*******************************************************************
 *
 * Function:    deleteAllData()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void CExtendedTable::deleteAllData(QTableView *tableView)
 {
     m_model->removeRows(0, m_model->rowCount());
     tableView->setModel(m_model);
 }


#if 0 // funkar inte
/*******************************************************************
 *
 * Function:    deleteAllData()
 *
 * Description: Removes contens from a row
 *
 *
 *
 *******************************************************************/
void CExtendedTable::deleteRowFrom(int row, QTableView *tableView)
{
    int colum = 1;

    for(int i = 0; i < 216; i++)
    {
        m_model->removeRows (i, colum);
        tableView->setModel(m_model);
    }
 }
#endif


 /*******************************************************************
  *
  * Function:    getNofRows()
  *
  * Description:
  *
  *
  *
  *******************************************************************/
  int CExtendedTable::getNofRows(void)
  {
      return m_model->rowCount();
  }

/*******************************************************************
*
* Function:    getNofRows()
*
* Description:
*
*
*
*******************************************************************/
int CExtendedTable::getNofCols(void)
{
   return m_model->columnCount();
}





  /*******************************************************************
   *
   * Function:    setBackgroundColor()
   *
   * Description:
   *
   *
   *
   *******************************************************************/
   void CExtendedTable::setBackgroundColor(QTableView *tableView, int row, int col, Qt::GlobalColor color)
   {
      if(row >= m_model->rowCount() || col >=m_model->columnCount())
      {
          QMessageBox::critical(NULL, QObject::tr("1Error: Get Table Data 1"), QObject::tr("Invalid table index") );
          return;
      }

     // m_model->setData(m_model->index(0, 0), Qt::red, Qt::BackgroundRole);
      m_model->setData(m_model->index(row, col), color, Qt::BackgroundRole);
      tableView->setModel(m_model);

   }




/*******************************************************************
*
* Function:    setBackgroundColor()
*
* Description:
*
*
*
*******************************************************************/
void CExtendedTable::setTextColor(QTableView *tableView, int row, int col, Qt::GlobalColor color)
{
   if(row >= m_model->rowCount() || col >=m_model->columnCount())
   {
       QString debugStr;
       debugStr.sprintf("Invalid table index, row=%d, rowC=%d, col=%d, colC=%d", row, m_model->rowCount(), col, m_model->columnCount());
       QMessageBox::critical(NULL, QObject::tr("2Error: Get Table Data 2"), debugStr );
       return;
   }

   m_model->setData(m_model->index(row, col), color, Qt::TextColorRole);
   tableView->setModel(m_model);
}



/*******************************************************************
 *
 * Description: Activate hor ver header click selection
 *
 * Create slot:
 *
 * private slots:
 *      void slotHorHeaderClicked(int index);
 *      void slotVertHeaderClicked(int index);
 *
 * In the dialog class constructor add (not in this file):
 *
 *  connect(ui->tableView->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(slotHorHeaderClicked(int)));
 *  connect(ui->tableView->verticalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(slotVertHeaderClicked(int)));
 *
 *******************************************************************/



/*******************************************************************
 *
 * Function:    getOneData()
 *
 * Description: Get one data from tableView.
 *
 * Create slot: on_tableView_clicked()
 *
 *******************************************************************/
void CExtendedTable::getOneData(QTableView *tableView, const QModelIndex &index, QString &data)
{
   data = tableView->model()->data(index).toString();
}

  /*******************************************************************
   *
   * Function:    getOneData()
   *
   * Description:
   *
   *
   *
   *******************************************************************/
   void CExtendedTable::getOneData(int row, int col, QString &data)
   {
      QVariant vData;
      if(row > m_model->rowCount() || col >m_model->columnCount())
      {
          QMessageBox::critical(NULL, QObject::tr("3Error: Get Table Data 3"), QObject::tr("Invalid table index") );
          return;
      }

      if(m_model->item(row, col) != 0)
      {
        vData = m_model->item(row, col)->text();
        data = vData.toString();
      }
   }



/*******************************************************************
*
* Function:    getNofRows()
*
* Description:
*
*
*
*******************************************************************/
void CExtendedTable::getOneRowWithData(QStringList &oneRowWithData, int row)
{
    int col;
    QString data;

    if(row > getNofRows())
    {
        QMessageBox::critical(NULL, QObject::tr("Error: Create Table"), QObject::tr("Invalid row number") );
        return;
    }

    for(col = 0; col < m_model->columnCount(); col++)
    {
        getOneData(row, col, data);
        oneRowWithData.append(data);
    }
}



/*******************************************************************
 *
 * Function:    addHeaders()
 *
 * Description: Constructor
 *
 *
 *
 *******************************************************************/
void CExtendedTable::
addHeaders(QTableView *tableView, TableColumnIndicatorInfo_ST *tableHeaderList, int maxNofData)
{
    //QString header;
    int i;
    QString debugStr;


    if(maxNofData > m_model->columnCount())
    {
        debugStr.sprintf("Invalid number of columns maxNofData=%d > %d=m_model->columnCount()", maxNofData, m_model->columnCount());
        QMessageBox::critical(NULL, QObject::tr("Error: Create Table"), debugStr );
        return;
    }

    for(i = 0; i < maxNofData; i++)
    {
        m_model->setHeaderData(i, Qt::Horizontal, tableHeaderList[i].name);
    }

    tableView->setModel(m_model);
}

/*******************************************************************
 *
 * Function:    getHorizontalHeaderItem()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void CExtendedTable::getHorizontalHeaderItem(int col, QString &str)
{
    str = m_model->horizontalHeaderItem(col)->text();
}


/*******************************************************************
 *
 * Function:    getHorizontalHeaderItem()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void CExtendedTable::getVerticalHeaderItem(int row, QString &str)
{
    str = m_model->verticalHeaderItem(row)->text();
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
void CExtendedTable::
setHorizontalFont(QTableView *tableView, const QString font, int size)
{
    tableView->horizontalHeader()->setFont(QFont(font, size));
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
void CExtendedTable::
resizeColumnsToContents(QTableView *tableView)
{
    tableView->resizeColumnsToContents();
}

/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 * ui->tableView->verticalHeader()->setFont(QFont("Helvetica", 9));
 *
 *******************************************************************/
void CExtendedTable::
setVerticalFont(QTableView *tableView, const QString font, int size)
{
    tableView->verticalHeader()->setFont(QFont(font, size));
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
void CExtendedTable::
addHorzVertHeaders(QTableView *tableView, QVector <CDbHndl::EfficPortStockData_ST> stockArr1, int maxNofData)
{
    //QString header;
    int i;
    QString debugStr;
    CDbHndl::EfficPortStockData_ST *data;


    if(maxNofData > m_model->columnCount())
    {
        debugStr.sprintf("Invalid number of columns maxNofData=%d > %d=m_model->columnCount()", maxNofData, m_model->columnCount());
        QMessageBox::critical(NULL, QObject::tr("Error: Create Table"), debugStr );
        return;
    }

    for(i = 0; i < maxNofData; i++)
    {
        data = & stockArr1[i];

        if(data->isValid == true)
        {
            m_model->setHeaderData(i, Qt::Horizontal, data->stockName);
            m_model->setHeaderData(i, Qt::Vertical, data->stockName);
        }
    }


    tableView->setModel(m_model);
}

/****************************************************************
 *
 * Function:    initFilter1Dividend()
 *
 * Description:.
 *
 *
 *
 ****************************************************************/
bool CExtendedTable::
addSnappshotData(QTableView *tableView,  QString dbName, QString str, int row, int col, QSqlRecord rec)
{
    CDbHndl db;
    QString indicator1;
    QString indicator2;

    addData(tableView, str, row, col);

    if(dbName.compare(db.m_gpSelLookUpTable[CDbHndl::GPSEL_INDEX_STOCK_PRICE].nameInDb)== 0)
    {
        indicator1 = db.m_gpSelLookUpTable[CDbHndl::GPSEL_INDEX_ONE_DAY_PRICE_CHANGE].nameInDb;
        indicator2 = db.m_gpSelLookUpTable[CDbHndl::GPSEL_INDEX_ONE_DAY_PERCENT_CHANGE].nameInDb;

        if(rec.value(indicator1).isNull() == false)
        {
            if(atof(rec.value(indicator1).toString().toStdString().c_str())== 0)
                addDataSetTextColor(tableView, str, row, col, Qt::black);
            else if(atof(rec.value(indicator1).toString().toStdString().c_str())< 0)
                addDataSetTextColor(tableView, str, row, col, Qt::red);
            else if(atof(rec.value(indicator1).toString().toStdString().c_str()) > 0)
                addDataSetTextColor(tableView, str, row, col, Qt::blue);


        }
        else if(rec.value(indicator2).isNull() == false)
        {
            if(atof(rec.value(indicator2).toString().toStdString().c_str())== 0)
                addDataSetTextColor(tableView, str, row, col, Qt::black);
            else if(atof(rec.value(indicator2).toString().toStdString().c_str())< 0)
                addDataSetTextColor(tableView, str, row, col, Qt::red);
            else if(atof(rec.value(indicator2).toString().toStdString().c_str()) > 0)
                addDataSetTextColor(tableView, str, row, col, Qt::blue);
        }
        else
        {
            addData(tableView, str, row, col);
        }
    }
    else if(dbName.compare( db.m_gpSelLookUpTable[CDbHndl::GPSEL_INDEX_ONE_DAY_PRICE_CHANGE].nameInDb)== 0)
    {
        addNumberIndicateSignWithColor(tableView, str, row, col);
    }
    else if(dbName.compare( db.m_gpSelLookUpTable[CDbHndl::GPSEL_INDEX_ONE_DAY_PERCENT_CHANGE].nameInDb)== 0)
    {
        addNumberIndicateSignWithColor(tableView, str, row, col);
    }
    else if(dbName.compare(db.m_gpSelLookUpTable[CDbHndl::GPSEL_INDEX_ONE_WEEK_PROCENT_CHANGE].nameInDb)== 0)
    {
        addNumberIndicateSignWithColor(tableView, str, row, col);
    }
    else if(dbName.compare(db.m_gpSelLookUpTable[CDbHndl::GPSEL_INDEX_ONE_MONTH_PERCENT_CHANGE].nameInDb)== 0)
    {
        addNumberIndicateSignWithColor(tableView, str, row, col);
    }
    else if(dbName.compare(db.m_gpSelLookUpTable[CDbHndl::GPSEL_INDEX_THREE_MONTH_PERCENT_CHANGE].nameInDb)== 0)
    {
        addNumberIndicateSignWithColor(tableView, str, row, col);
    }
    else if(dbName.compare(db.m_gpSelLookUpTable[CDbHndl::GPSEL_INDEX_SIX_MONTH_PERCENT_CHANGE].nameInDb)== 0)
    {
        addNumberIndicateSignWithColor(tableView, str, row, col);
    }
    else if(dbName.compare(db.m_gpSelLookUpTable[CDbHndl::GPSEL_INDEX_ONE_YEAR_PERCENT_CHANGE].nameInDb)== 0)
    {
        addNumberIndicateSignWithColor(tableView, str, row, col);
    }
    else if(dbName.compare(db.m_gpSelLookUpTable[CDbHndl::GPSEL_INDEX_TWO_YEAR_PERCENT_CHANGE].nameInDb)== 0)
    {
        addNumberIndicateSignWithColor(tableView, str, row, col);
    }
    else if(dbName.compare(db.m_gpSelLookUpTable[CDbHndl::GPSEL_INDEX_THREE_YEAR_PERCENT_CHANGE].nameInDb)== 0)
    {
        addNumberIndicateSignWithColor(tableView, str, row, col);
    }
    else if(dbName.compare(db.m_gpSelLookUpTable[CDbHndl::GPSEL_INDEX_FIVE_YEAR_PERCENT_CHANGE].nameInDb)== 0)
    {
        addNumberIndicateSignWithColor(tableView, str, row, col);
    }
    else
    {
        addData(tableView, str, row, col);
    }

    return true;

}



/*******************************************************************
 *
 * Function:    addHeaders()
 *
 * Description: Constructor
 *
 *
 *
 *******************************************************************/
void CExtendedTable::
addDataSetTextColor(QTableView *tableView, QString data, int row, int col, Qt::GlobalColor color)
{
    m_model->setData(m_model->index(row, col), QVariant(data));
    setTextColor(tableView, row, col, color);
    //tableView->setModel(m_model);
}


/*******************************************************************
 *
 * Function:    addNumberIndicateSignWithColor()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void CExtendedTable::
addNumberIndicateSignWithColor(QTableView *tableView, QString data, int row, int col)
{
    m_model->setData(m_model->index(row, col), QVariant(data));
    if(data.toDouble() < 0 )
        setTextColor(tableView, row, col, Qt::red);
    else if(data.toDouble() == 0 || data.toFloat() == 0)
        setTextColor(tableView, row, col, Qt::black);
    else
        setTextColor(tableView, row, col, Qt::blue);
    //tableView->setModel(m_model);
}

/*******************************************************************
 *
 * Function:    addHeaders()
 *
 * Description: Constructor
 *
 *
 *
 *******************************************************************/
void CExtendedTable::
addData(QTableView *tableView, QString data, int row, int col)
{
    m_model->setData(m_model->index(row, col), QVariant(data));
    tableView->setModel(m_model);
}


/*******************************************************************
 *
 * Function:    setTableWidth()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void CExtendedTable::setTableWidth(QTableView *tableView, int nofColumns)
{
    int totColumnWith = 0;
    int i;

    for(i = 0; i < nofColumns; i++)
    {
       totColumnWith += tableView->columnWidth(i);
    }

    QRect rect = tableView->geometry();
    rect.setWidth(2 + tableView->verticalHeader()->width() + totColumnWith);
    tableView->setGeometry(rect);
}



/*******************************************************************
 *
 * Function:    setTableWidth()
 *
 * Description: Create and add a check box item
 *
 * bool checked Qt::Checked  Qt::Unchecked
 *
 *******************************************************************/
void CExtendedTable::
addCheckBoxItem(QTableView *tableView, int row, int col, Qt::CheckState checked, QString str)
{
        // create check box item
        QStandardItem* item0 = new QStandardItem(true);
        item0->setCheckable(true);
        item0->setCheckState(checked);
        item0->setText(str);
        m_model->setItem(row, col, item0);
        tableView->setModel(m_model);
}


/****************************************************************
 *
 * Function:    tableRowIsChecked()
 *
 * Description:.
 *
 *
 * Note: we do not need any set checked item it will happends
 *       when the check box is clicked.
 *
 ****************************************************************/
bool CExtendedTable::tableRowIsChecked(int row, int col, bool &isChecked)
{
    QString str;


    // Make sure that item is checkable
    Qt::ItemFlags flags = m_model->item(row, col)->flags();


    if (!(flags & Qt::ItemIsUserCheckable) || !(flags & Qt::ItemIsEnabled))
    {
        str.sprintf("row=%d, col=%d, %s", row, col, m_model->item(row,col)->text().toStdString().c_str());
        QMessageBox::information(0, "Title",  str);
        return false;
    }


    // Make sure that we have a check state
    QVariant value = m_model->item(row,col)->data(Qt::CheckStateRole);
    if (!value.isValid())
    {
        QMessageBox::information(0, "Title", "invalid value");
        return false;
    }

    if((static_cast<Qt::CheckState>(value.toInt()) == Qt::Checked))
    {
        //QMessageBox::information(0, "Title", "Checked");
        isChecked = true;
    }
    else if((static_cast<Qt::CheckState>(value.toInt()) == Qt::Unchecked))//if(state == Qt::Unchecked)
    {
        //QMessageBox::information(0, "Title", "Unchecked");
        isChecked = false;
    }
    return true;
}


//  // m_model->item(Item(row, col, item0);
