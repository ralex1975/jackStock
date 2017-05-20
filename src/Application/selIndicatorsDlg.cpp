/*******************************************************************
 *
 * Filename: selIndicatorsDlg.cpp
 * Date:     2012-10-06
 *
 * Description: This file create a list that contains a check box and
 *              an item on each row. A stock indicator should be stored
 *              in item field.
 *
 *
 *******************************************************************/

#include "selIndicatorsDlg.h"
#include "ui_selIndicatorsDlg.h"
#include "dbHndl.h"
#include "common.h"





/****************************************************************
 *
 * Function:    CSelIndicatorsDlg()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
CSelIndicatorsDlg::CSelIndicatorsDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CSelIndicatorsDlg)
{

    CDbHndl db;

    m_tableModel = new QStandardItemModel();

    ui->setupUi(this);

    // qDebug("Open db 5 %s, %d", __FILE__, __LINE__);
    // db.openDb(PATH_JACK_STOCK_DB);
    if(db.userSelSnapshotDataIsInitialized()== false)
    {
        db.initKeyNumberList(ui->tableView, &m_tableModel);
    }
    else
    {
        db.initKeyNumberListFromDb(ui->tableView, &m_tableModel);
    }
    ui->tableView->resizeColumnsToContents();
    // qDebug("Close db 5 %s, %d", __FILE__, __LINE__);
    // db.closeDb();

}

/****************************************************************
 *
 * Function:    ~CSelIndicatorsDlg()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
CSelIndicatorsDlg::~CSelIndicatorsDlg()
{
    delete ui;
    delete m_tableModel;
}





/****************************************************************
 *
 * Function:    on_buttonBox_accepted()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
void CSelIndicatorsDlg::on_buttonBox_accepted()
{
    // Make sure that the item is checkable
    Qt::ItemFlags flags = m_tableModel->item(0,0)->flags();

    if (!(flags & Qt::ItemIsUserCheckable) || !(flags & Qt::ItemIsEnabled))
        return;

    // Make sure that we have a check state
    QVariant value = m_tableModel->item(0,0)->data(Qt::CheckStateRole);
    if (!value.isValid())
        return;
}


/****************************************************************
 *
 * Function:    tableRowIsChecked()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CSelIndicatorsDlg::tableRowIsChecked(int row, bool &isChecked)
{
    // Make sure that the item is checkable
    Qt::ItemFlags flags = m_tableModel->item(row, 0)->flags();

    if (!(flags & Qt::ItemIsUserCheckable) || !(flags & Qt::ItemIsEnabled))
        return false;

    // Make sure that we have a check state
    QVariant value = m_tableModel->item(row,0)->data(Qt::CheckStateRole);
    if (!value.isValid())
        return false;

    if((static_cast<Qt::CheckState>(value.toInt()) == Qt::Checked))
    {
        //QMessageBox::information(0, "Title", "Checked");
        isChecked = true;
    }
    else if((static_cast<Qt::CheckState>(value.toInt()) == Qt::Unchecked))//if(state == Qt::Unchecked)
    {
        //QMessageBox::information(0, "Title", "Unchecked");
        isChecked = false;
        return false;
    }
    return true;
}





/****************************************************************
 *
 * Function:    on_tableView_clicked()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
void CSelIndicatorsDlg::on_tableView_clicked(const QModelIndex &index)
{
    bool isChecked;
    tableRowIsChecked(index.row(), isChecked);
}



/****************************************************************
 *
 * Function:    on_UpdateButton_clicked()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
void CSelIndicatorsDlg::on_UpdateButton_clicked()
{
    CDbHndl db;

    qDebug("Open db 6 %s, %d", __FILE__, __LINE__);
    // db.openDb(PATH_JACK_STOCK_DB);
    db.saveUserSelectStockParamsInDb(m_tableModel);
    db.initKeyNumberListFromDb(ui->tableView, &m_tableModel);
    // db.closeDb();
    qDebug("Close db 6 %s, %d", __FILE__, __LINE__);
}
