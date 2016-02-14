/*******************************************************************
 *
 * Filename: CTableTab.h
 * Date:     2012-07-08
 *
 * Description: Tab2 page
 *
 *
 *******************************************************************/

#ifndef CTableTab_H
#define CTableTab_H

#include <QDialog>
#include "common.h"
#include <QtCore>
#include <QtGui>
#include "extendedTable.h"
#include "parsesnapshotdata.h"
#include "httpwindow.h"
#include "common.h"
#include <QCheckBox>




namespace Ui
{
class CTableTab;
}





//======================================================================================
// class definition
//======================================================================================
class CTableTab : public QDialog
{
    Q_OBJECT

private:
    CExtendedTable m_et;
    QHeaderView *m_horizHeader;
    CDbHndl::TableTabWidget_ST m_filterCtrls;

    void setTableWidth(int nofColumns);

public:
    explicit CTableTab(QWidget *parent = 0);
    ~CTableTab();
    void initComboCountrySel(QComboBox *combo);
    void getSelectionDataToParse(QComboBox *combo, QCheckBox *largeCap, QCheckBox *midCap, QCheckBox *smallCap, int &result);
    bool getUrlToSnapshopParseData(int result, QUrl &url, QUrl &ur2, QUrl &ur3);



public slots:
    void recvParserThreadIsFinish(void);
    void sectionClicked1(int logicalIndex);
    void recvHtmlPageIsDownloaded(int);


private slots:
    void on_pushButton_clicked();
    void on_FilterData_clicked();
    void on_GetDbDataButton_clicked();
    void on_GetDbDataButton_2_clicked();



private:
    Ui::CTableTab *ui;

    HttpWindow m_hw1;
    HttpWindow m_hw2;
    HttpWindow m_hw3;

    int m_filesReceived;
    bool m_sortWithoutfilter;


public:
    CParseSnapshotData *m_parserThread;
};

#endif // CTableTab_H
