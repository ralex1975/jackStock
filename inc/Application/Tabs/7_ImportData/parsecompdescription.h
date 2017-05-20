#ifndef PARSECOMPDESCRIPTION_H
#define PARSECOMPDESCRIPTION_H

#include <QDialog>
#include <QFile>
#include <QString>
#include <QDebug>
#include <QTextStream>
#include <QObject>

#include "common.h"
#include <QtCore>
#include <QtGui>



class parseCompDescription
{
private:
    enum MainState_ET
    {
        // MSTATE_COMPANY_NAME,
        MSTATE_MAIN_COMPANY_INFO,
        // MSTATE_COMPANY_INFO_SECTOR,
        // MSTATE_COMPANY_INFO_BRANCH,
        MSTATE_COMPANY_INFO_EXECUTIVE_DIRECTOR,
        MSTATE_COMPANY_DESCR,
        MSTATE_COMPANY_DESCR_TXT
    };

    MainState_ET m_mainState;

    static const QString m_removeTockenArr[2];
public:
    parseCompDescription();
    bool readFile(QString filename, QString assetName, QString assetSymbol);

};

#endif // PARSECOMPDESCRIPTION_H
