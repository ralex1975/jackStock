/*******************************************************************
 *
 * Filename: main.cpp
 * Date:     2012-07-08
 *
 * Description: Main entry point
 *
 * Note: To change use the dialog window for each form to change the
 *       size. The smallest form decides the size for the rest of the
 *       tab forms. You need to rebuild the whole project to be able
 *       to change the size.
 *
 *http://download.finance.yahoo.com/d/quotes.csv?s=ABB.ST&f=sl1d1t1c1ohgv&e=.csv
 *
 *******************************************************************/


#include <QApplication>
#include "jackStock.h"
#include "tabdialog.h"
#include <stdint.h>
#include "dbHndl.h"
#include "common.h"
#include "debug.h"

#include "matrix.h"
#include <QSplashScreen>
#include <QPixmap>
#include <curl/curl.h>


size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}


int main(int argc, char *argv[])
{


    QApplication a(argc, argv);

   // CMatrix cm;

    CURL *curl;

       /* First step, init curl */
       curl = curl_easy_init();

           FILE *fp;
           CURLcode res;
           char *url = "https://finance.yahoo.com/quote/ABB?p=ABB";
           char outfilename[FILENAME_MAX] = "abb.txt";
           curl = curl_easy_init();
           if (curl)
           {
               fp = fopen(outfilename,"wb");
               curl_easy_setopt(curl, CURLOPT_URL, url);
               curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
               curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
               res = curl_easy_perform(curl);
               /* always cleanup */
               curl_easy_cleanup(curl);
               fclose(fp);
            }




    //QPixmap pixmap("Screenshot.pgn");
    //QSplashScreen splash(pixmap);
    //splash.show();
    //splash.showMessage("Wait...");

    #if 0
    int j = -2;
    for(int i = 0; i < 5; i++)
    {
        cm.xylista.listax[i] = (double) j;
        cm.xylista.listay[i] = (double) 2*(j * j) - 4;
        qDebug() << "x" << cm.xylista.listax[i];
        qDebug() << "y" << cm.xylista.listay[i];
        j++;
    }


  //  cm.Laddakv2(&n, &mean);
    cm.Minkv2(6);
    #endif



    // Be sure to create database and execute create table before create tabdialog that need db
    CDbHndl db;
    db.createTable();

    TabDialog tabdialog;

    debugInit();

    db.resetSnapshotRamData(db.m_snapshotStockData);


	// adding maximize and minimize button to the dialog
    tabdialog.setWindowFlags(tabdialog.windowFlags() | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
    tabdialog.showMaximized();
    tabdialog.show();

     //splash.finish(&tabdialog);
	 
    return a.exec();
}
