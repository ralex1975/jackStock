/*******************************************************************
 *
 * Filename: tabdialog.c
 * Date:     2012-07-08
 *
 * Description: This is the framwork that is used to display
 *              Tabs dialog on main window.
 *
 *              To use it define a tab window (dialog) and add this
 *              tab in tabdialog constuctor.
 *
 *
 *******************************************************************/
#include <QtGui>
#include "WebBrowserTab.h"
#include "tableTab.h"
#include "tabdialog.h"
#include "common.h"
#include "TADlg.h"
#include "assistant.h"
#include "graphtab.h"
#include "portfolio.h"
#include "taAnalysis.h"
#include "efficientPortfolio.h"
#include "jackStock.h"
#include "importdata.h"
#include "tabhelp.h"
#include "fatab.h"
#include "leastsquarestadlg.h"
#include <QVBoxLayout>


TabDialog *g_TabDialog;



/*******************************************************************
 *
 * Function:    TabDialog()
 *
 * Description: Constructor
 *
 *              Add your tabs here.
 *
 *              The size of the tab need to be set in the tab dialog
 *              The tab that has biggest sized define the main window
 *              size.
 *
 *
 *******************************************************************/
TabDialog::TabDialog(QWidget *parent)
    : QDialog(parent)
{
    g_TabDialog = this;


    assistant = new Assistant;

    createMenu();

    tabWidget = new QTabWidget;

    tabWidget->addTab(new CWebBrowserTab(parent), QString::fromUtf8("Webläsare"));
    tabWidget->addTab(new CTableTab(parent),QString::fromUtf8("Nyckeltal"));
    tabWidget->addTab(new FaTab(parent), QString::fromUtf8("FA"));
    tabWidget->addTab(new TaAnalysis(parent), QString::fromUtf8("TA"));
    tabWidget->addTab(new CTADlg(parent), QString::fromUtf8("TA/FA"));
    tabWidget->addTab(new Portfolio(parent), QString::fromUtf8("Portfölj"));
    tabWidget->addTab(new EfficientPortfolio(parent), QString::fromUtf8("Portföljoptimering"));
    tabWidget->addTab(new ImportData(parent), QString::fromUtf8("Importera"));
    tabWidget->addTab(new TabHelp(parent), QString::fromUtf8("Anteckningar"));
    tabWidget->addTab(new GraphTab(parent), QString::fromUtf8("Test, Graf"));
    tabWidget->addTab(new CJackStock(parent), QString::fromUtf8("Färgtest"));
    tabWidget->addTab(new LeastSquaresTaDlg(parent), QString::fromUtf8("Nyckeltal2"));


    tabWidget->setCurrentIndex(1);
    tabWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMenuBar(menuBar);
    mainLayout->setGeometry(QRect(300,10,1200,800));
    mainLayout->setSizeConstraint(QLayout::SetNoConstraint);
    mainLayout->addStretch(1);
    mainLayout->addWidget(tabWidget);

    setLayout(mainLayout);

    setWindowTitle(tr("JackStock"));

}


/*******************************************************************
 *
 * Function:    createMenu()
 *
 * Description: This function create Main menu and sub menu actions
 *
 *
 * Note: Be sure to define the event handler in the class section
 *       private slots:
 *
 *      Example on a event handler is: onSelFilterInicatorMenu()
 *
 *******************************************************************/
void TabDialog::createMenu(void)
 {
     menuBar = new QMenuBar;

     //=======================================
     // Crete main menu item Arkiv
     //=======================================
     fileMenu = new QMenu(tr("&Arkiv"), this);
     exitAction = fileMenu->addAction(tr("E&xit"));
     connect(exitAction, SIGNAL(triggered()), this, SLOT(accept()));
     menuBar->addMenu(fileMenu);


     //=======================================
     // Crete main menu item Verktyg
     //=======================================
     toolMenu = new QMenu(tr("Verktyg"), this);
     QString tmp;
     tmp = QString::fromUtf8("Inställningar");
     QMenu* submenuSettings = toolMenu->addMenu(tmp);
    // QAction* trendIndicatorSetup = submenuSettings->addAction( tr("Trendindikatorer") );
     SelFilterIndicatorAct = submenuSettings->addAction( tr("Nyckeltalsindikatorer") );

     // Create sub menu and connect event handler here:
     SelFilterIndicatorAct->setStatusTip(tr("Välj indikatorer"));
     connect(SelFilterIndicatorAct, SIGNAL(triggered()), this, SLOT(onSelFilterInicatorMenu()));


     // Help menu
     assistantAct = new QAction(tr("Help Contents"), this);
     assistantAct->setShortcut(QKeySequence::HelpContents);
     connect(assistantAct, SIGNAL(triggered()), this, SLOT(showDocumentation()));


     aboutAct = new QAction(tr("&About"), this);

     aboutQtAct = new QAction(tr("About &Qt"), this);
     connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));


     helpMenu = new QMenu(tr("&Help"), this);
     helpMenu->addAction(assistantAct);
     helpMenu->addSeparator();
     helpMenu->addAction(aboutAct);
     helpMenu->addAction(aboutQtAct);

     menuBar->addMenu(toolMenu);
     menuBar->addMenu(helpMenu);

 }


void TabDialog::showDocumentation()
{
    assistant->showDocumentation("index.html");
}



/*******************************************************************
 *
 * Function:    onSelFilterInicatorMenu()
 *
 * Description:
 *
 *
 *
 *
 *******************************************************************/
void TabDialog::onSelFilterInicatorMenu()
{
      m_selIndicatorDlg.show();
     //QMessageBox::information(this, "Title", "SelFilterIndicatorMenu");
}


/*******************************************************************
 *
 * Function:    changeVisibleTab()
 *
 * Description:
 *
 *
 *
 *
 *******************************************************************/
void TabDialog::newFile()
 {
     //infoLabel->setText(tr("Invoked <b>File|New</b>QString ));
    QMessageBox::information(this, "Title", "NewFileMenu");

 }


/*******************************************************************
 *
 * Function:    changeVisibleTab()
 *
 * Description:
 *
 *
 *
 *
 *******************************************************************/
void TabDialog::changeVisibleTab(void)
{

    QMessageBox::information(this, "Title", "Tab changed");
}


/*******************************************************************
 *
 * Function:    event()
 *
 * Description: This function is invoke each time an event is received
 *              by this dialog.
 *
 *              To send an event from dialog class TabDialog to this dialog
 *              do as follows:
 *
 *              QEvent *event = new QEvent(QEvent::User);
 *              QCoreApplication::sendEvent((QObject*)g_TabDialog, event);
 *
 *              Where g_TabDialog is defined as TabDialog *g_TabDialog;
 *
 *******************************************************************/
bool TabDialog::event( QEvent * e )
 {
 if ( e->type() == (QEvent::User) )
 {
    tabWidget->setCurrentIndex(2);
    //QMessageBox::information(this, "Title", "Inläsning färdig 1");
    QApplication::beep();
    return  true;
 }

 return QWidget::event( e );
 }



