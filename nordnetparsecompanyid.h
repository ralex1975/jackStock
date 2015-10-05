#ifndef NORDNETPARSECOMPANYID_H
#define NORDNETPARSECOMPANYID_H

#include <QFile>
#include <QString>
#include <QDebug>
#include <QTextStream>
#include <QObject>

#include <QDialog>
#include "common.h"
#include <QtCore>
#include <QtGui>
#include "httpwindow.h"
#include "common.h"






class nordnetParseCompanyId : public QObject
{
    Q_OBJECT
    HttpWindow m_hw1;
    bool m_waitOnServerResp;

public:
    explicit nordnetParseCompanyId(QObject *parent = 0);
    void reqHtmlFile(QString path, QUrl url);
    bool readFile(QString filename);

private:
    bool parseCompanyIdAndName(QTextStream &inStream);


public slots:
    void slotHtmlPageIsRecv(int number);
};

#endif // NORDNETPARSECOMPANYID_H
