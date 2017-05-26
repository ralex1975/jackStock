/*******************************************************************
 *
 * Filename: CWebBrowserTab.cpp
 * Date:     2012-07-08
 *
 * Description: Tab1 page
 *
 *
 *******************************************************************/

#include "common.h"
#include "WebBrowserTab.h"
#include "ui_WebBrowserTab.h"
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <QtWebKit/QWebView>






/*******************************************************************
 *
 * Function:    CWebBrowserTab()
 *
 * Description: Constructor
 *
 *
 *
 *******************************************************************/
CWebBrowserTab::CWebBrowserTab(QWidget *parent) :
    QWidget(parent),
                ui(new Ui::CWebBrowserTab)
{
    // http://stockcharts.com/h-sc/ui?s=ABB

    // ajn 170523 Gamla som ska vara kvar
    //QUrl url("https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Sverige&lista=1_1&large=on&mid=on&sektor=0&subtyp=price&sortera=aktie&sorteringsordning=stigande");

    // funkar med nya browsern qt 5.7
    //QUrl url("http://www.google.com/finance/historical?q=AAPL&startdate=Nov 1, 2011&enddate=Nov 30, 2011&output=csv");
    QUrl url("https://borsdata.se/");

    // Ska funka
   //QUrl url("http://www.google.com/finance/historical?q=NASDAQ:ADBE&startdate=Jan+01%2C+2009&enddate=Aug+2%2C+2012&output=csv");

    // QUrl url("http://performance.morningstar.com/perform/Performance/stock/exportStockPrice.action?t=XNYS:ABB&pd=1y&freq=d&sd=&ed=&pg=0&culture=en-US&cur=USD");


    // Test nya Yahoo funkar inte
    // QUrl url("https://query1.finance.yahoo.com/v7/finance/download/AZN.ST?period1=1494540000&period2=1495404000&interval=1d&events=history&crumb=.dCDNsa0z5Q");


   // QUrl url("file:///home/ajn/Documents/myAnalysisTemplate.html");

    ui->setupUi(this);
    ui->webView->setUrl(url);
    //ui->webView->load(url);
    //QString str;



    installEventFilter(this);
}

/*******************************************************************
 *
 * Function:    ~CWebBrowserTab()
 *
 * Description: Destructor
 *
 *
 *
 *******************************************************************/
CWebBrowserTab::~CWebBrowserTab()
{
    delete ui;
}


/*******************************************************************
 *
 * Function:    on_pushButton_clicked()
 *
 * Description: Push button event handler
 *
 *
 *
 *******************************************************************/
void CWebBrowserTab::on_pushButton_clicked()
{
    QUrl url("http://www.yieldcurve.com/");
    ui->webView->setUrl(url);

   //QMessageBox::information(this, "Title", "Do not hit me");
}


/*******************************************************************
 *
 * Function:    on_pushButton_clicked()
 *
 * Description: Push button event handler
 *
 *
 *
 *******************************************************************/
void CWebBrowserTab::on_pushButton_3_clicked()
{
    QUrl url("https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Sverige&lista=1_1&large=on&mid=on&sektor=0&subtyp=price&sortera=aktie&sorteringsordning=stigande");
    ui->webView->setUrl(url);

    // http://quotes.wsj.com/bond/BX/TMBMKSE-10Y
}



/*******************************************************************
 *
 * Function:    on_pushButton_clicked()
 *
 * Description: Push button event handler
 *
 *
 *
 *******************************************************************/
void CWebBrowserTab::on_pushButton_2_clicked()
{

}

void CWebBrowserTab::on_pushButton_4_clicked()
{
    ui->webView->back();
}

void CWebBrowserTab::on_pushButton_5_clicked()
{
    ui->webView->forward();
}

void CWebBrowserTab::on_pushButton_6_clicked()
{
    QUrl url("https://www.investing.com/rates-bonds/sweden-government-bonds");
    ui->webView->setUrl(url);
}

void CWebBrowserTab::on_pushButtonShillerPe_clicked()
{
    QUrl url("http://www.gurufocus.com/shiller-PE.php");
    ui->webView->setUrl(url);
}
