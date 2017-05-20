#ifndef PARSECOMPANYECONOMYINFO_H
#define PARSECOMPANYECONOMYINFO_H

#include <QDialog>

#include <QFile>
#include <QString>
#include <QDebug>
#include <QTextStream>
#include <QObject>

#include "common.h"
#include <QtCore>
#include <QtGui>

#include <QObject>

class ParseCompanyEconomyInfo : public QObject
{
    Q_OBJECT

    enum StateParseHistFinance_ET
    {
        STATE_MULTI_YEAR_SUMMARY,
        STATE_FIND_BALANCE_SHEET,
        STATE_FIND_YEAR,
        STATE_GET_START_TAG_RESULT_NUMBER,
        STATE_GET_RESULT_NUMBER
    };

    enum HistFinSubState_ET
    {
        SUB_STATE_TH_END,
        SUB_STATE_TH_START,
        SUB_STATE_YEAR,
        SUB_STATE_TD_END,
        SUB_STATE_TD_START,
        SUB_STATE_NUMBER
    };

public:
    explicit ParseCompanyEconomyInfo(QObject *parent = 0);
    bool readFile(QString filename, QString assetName);

private:
    StateParseHistFinance_ET m_mainState;
    HistFinSubState_ET m_subState;

    QStringList m_tagBalanceArr;      // Balansräkning
    QStringList m_tagProfitLossArr;     // resultat räkning
    bool parseProfitloss(QTextStream &inStream, int &dateIndex);
    bool parseBalance(QTextStream &inStream, int &dateIndex);
    bool parseFile2(QTextStream &inStream, QString &result);

    bool parseFileGetId(QTextStream &inStream, QString &result);


    bool parseFinNumberInfo(QString str);
    bool parseCompanyInfo(QString str, QString &result);


    QStringList m_resultProfitLossDateArr;
    QStringList m_resultProfitLossNumberArr;

    
signals:
    
public slots:
    
};

#endif // PARSECOMPANYECONOMYINFO_H
