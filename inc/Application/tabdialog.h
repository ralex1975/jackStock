/*******************************************************************
 *
 * Filename: tabdialog.c
 * Date:     2012-07-08
 *
 * Description: Tab dialog shown as main window
 *
 *
 *******************************************************************/

#ifndef TABDIALOG_H
#define TABDIALOG_H

#include <QDialog>
#include <QEvent>
#include <QtCore>
#include <QtGui>
#include "selIndicatorsDlg.h"
#include "assistant.h"
#include "canvaspicker.h"


QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QFileInfo;
class QTabWidget;
QT_END_NAMESPACE


//======================================================================================
// class definition
//======================================================================================
class TabDialog : public QDialog
{
    Q_OBJECT

public:
    TabDialog(QWidget *parent = 0);
    bool event( QEvent * e );



private:
    // QStringList m_keyNumberParam;
    QTabWidget *tabWidget;
    QMenuBar *menuBar;
    QMenu *fileMenu;
    QMenu *toolMenu;
    QMenu *helpMenu;


    QAction *exitAction;
    QAction *SelFilterIndicatorAct;

    // Help
    QAction *assistantAct;
    QAction *aboutAct;
    QAction *aboutQtAct;

    Assistant *assistant;

    CSelIndicatorsDlg m_selIndicatorDlg;


    void createMenu(void);
    void showDocumentation(const QString &file);

private slots:
    void newFile();
    void onSelFilterInicatorMenu();
    void showDocumentation();

public slots:


public:
    void changeVisibleTab(void);

};


#endif
