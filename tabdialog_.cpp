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

    tabWidget->addTab(new CWebBrowserTab(parent), QString::fromLatin1("Webl‰sare"));
    tabWidget->addTab(new CTableTab(parent),QString::fromLatin1("Nyckeltal"));
    tabWidget->addTab(new FaTab(parent), QString::fromLatin1("FA"));
    tabWidget->addTab(new TaAnalysis(parent), QString::fromLatin1("TA"));
    tabWidget->addTab(new CTADlg(parent), QString::fromLatin1("TA/FA"));
    tabWidget->addTab(new Portfolio(parent), QString::fromLatin1("Portfˆlj"));
    tabWidget->addTab(new EfficientPortfolio(parent), QString::fromLatin1("Portfˆljoptimering"));
    tabWidget->addTab(new ImportData(parent), QString::fromLatin1("Importera"));
    tabWidget->addTab(new TabHelp(parent), QString::fromLatin1("Anteckningar"));
    tabWidget->addTab(new GraphTab(parent), QString::fromLatin1("Test, Graf"));
    tabWidget->addTab(new CJackStock(parent), QString::fromLatin1("F‰rgtest"));


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
     tmp = QString::fromLatin1("Inst√§llningar");
     QMenu* submenuSettings = toolMenu->addMenu(tmp);
    // QAction* trendIndicatorSetup = submenuSettings->addAction( tr("Trendindikatorer") );
     SelFilterIndicatorAct = submenuSettings->addAction( tr("Nyckeltalsindikatorer") );

     // Create sub menu and connect event handler here:
     //SelFilterIndicatorAct = new QAction(tr("Indikatorer"), this);
     SelFilterIndicatorAct->setStatusTip(tr("V√§lj indikatorer"));
     // toolMenu->addAction(SelFilterIndicatorAct);
     connect(SelFilterIndicatorAct, SIGNAL(triggered()), this, SLOT(onSelFilterInicatorMenu()));


     // Help menu
     assistantAct = new QAction(tr("Help Contents"), this);
     assistantAct->setShortcut(QKeySequence::HelpContents);
     connect(assistantAct, SIGNAL(triggered()), this, SLOT(showDocumentation()));


     aboutAct = new QAction(tr("&About"), this);
     //connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

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
    //QMessageBox::information(this, "Title", "Inl√§sning f√§rdig 1");
    QApplication::beep();
    return  true;
 }

 return QWidget::event( e );
 }



