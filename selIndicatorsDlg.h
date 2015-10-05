/*******************************************************************
 *
 * Filename: selIndicatorsDlg.h
 * Date:     2012-10-06
 *
 * Description: This file create a list that contains a check box and
 *              an item on each row. A stock indicator should be stored
 *              in item field.
 *
 *
 *******************************************************************/
#ifndef SELINDICATORSDLG_H
#define SELINDICATORSDLG_H

#include <QDialog>
#include <QtCore>
#include <QtGui>

namespace Ui
{
    class CSelIndicatorsDlg;
}



class CSelIndicatorsDlg : public QDialog
{
    Q_OBJECT

private:
    QStringList m_keyNumberList;
    
public:
    explicit CSelIndicatorsDlg(QWidget *parent = 0);
    ~CSelIndicatorsDlg();
    bool tableRowIsChecked(int row, bool &isChecked);

    
private slots:
    void on_buttonBox_accepted();

    void on_tableView_clicked(const QModelIndex &index);

    void on_UpdateButton_clicked();

private:
    Ui::CSelIndicatorsDlg *ui;
    QStandardItemModel* m_tableModel;

};

#endif // SELINDICATORSDLG_H
