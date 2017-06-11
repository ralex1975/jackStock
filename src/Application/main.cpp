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

// #include "matrix.h"
#include <QSplashScreen>
#include <QPixmap>



int main(int argc, char *argv[])
{


    QApplication a(argc, argv);

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
