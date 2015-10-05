/*******************************************************************
 *
 * Filename: WebBrowserTab.h
 * Date:     2012-07-08
 *
 * Description: Tab1 page
 *
 *
 *******************************************************************/

#ifndef WEB_BROWSER_TAB_H
#define WEB_BROWSER_TAB_H

#include <QWidget>
#include <QEvent>
#include <QtCore>
#include <QtGui>
#include "jackStock.h"


namespace Ui
{
class CWebBrowserTab;
}

//======================================================================================
// class definition
//======================================================================================
class CWebBrowserTab : public QWidget
{
    Q_OBJECT
    
public:
    CJackStock m_jsDlg;

    explicit CWebBrowserTab(QWidget *parent = 0);
    ~CWebBrowserTab();
    
public slots:
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();



private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::CWebBrowserTab *ui;



};

#endif 
