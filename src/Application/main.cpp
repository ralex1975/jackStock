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


static void print_cookies(CURL *curl)
{
  CURLcode res;
  struct curl_slist *cookies;
  struct curl_slist *nc;
  int i;

  qDebug("Cookies, curl knows:\n");

  res = curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);

  if (res != CURLE_OK)
  {
    qDebug() << "Curl curl_easy_getinfo failed: %s\n" << curl_easy_strerror(res);
    exit(1);
  }

  nc = cookies, i = 1;

  while (nc)
  {
    qDebug() << "[" << i << "]" << nc->data;
    // printf("[%d]: %s\n", i, nc->data);
    nc = nc->next;
    i++;
  }

  if (i == 1)
  {
    qDebug("(none)\n");
  }

  curl_slist_free_all(cookies);
}


int main(int argc, char *argv[])
{


    QApplication a(argc, argv);

   // CMatrix cm;

 #if 0
     CURL *curl;
     CURLcode res;
     curl_global_init(CURL_GLOBAL_ALL);
     curl = curl_easy_init();

     if (curl)
     {
        //char nline[256];
        curl_easy_setopt(curl, CURLOPT_URL, "https://finance.yahoo.com/quote/ABB?p=ABB");
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, ""); /* just to start the cookie engine */
        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(res));
            return 1;
        }
        print_cookies(curl);
    }
#endif

#if 0
    CURL *curl;

       /* First step, init curl */
       curl_global_init(CURL_GLOBAL_ALL);
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
               curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
               // Start cookie engine
               curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
               curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
               curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
               res = curl_easy_perform(curl);
               /* always cleanup */
               //curl -c cookies.txt url;
               curl_easy_cleanup(curl);
               fclose(fp);
            }
#endif

           // You can export all cookies a libcurl handle keeps, using something like this:
           // curl_easy_getinfo(..., CURLINFO_COOKIELIST, ...)


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
