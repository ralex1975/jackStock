#include "debug.h"
#include <QMessageBox>
#include <QTextStream>
#include <QDate>


void debugInit(void)
{
    QFile file("database/debug/debug.txt");
    QString date;

    date = QDate::currentDate().toString("yyyy.MM..dd");

    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
         QMessageBox::critical(NULL, QString::fromUtf8("Debug error"), QString::fromUtf8("Unable to open debug file"));
         return;
    }

    QTextStream out(&file);

    out << "date\n";

    file.close();

}



void debugPrintToFile(QString debugStr, int line, QString fileName)
{
    QFile file("database/debug/debug.txt");

    if(!file.open(QFile::Append | QFile::Text))
    {
         QMessageBox::critical(NULL, QString::fromUtf8("Debug error"), QString::fromUtf8("Unable to open debug file"));
         return;
    }

    QTextStream out(&file);

    out << debugStr << "\n" << line << fileName;

    file.close();


    file.close();

}


