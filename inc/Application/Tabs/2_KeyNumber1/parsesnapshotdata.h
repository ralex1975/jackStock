#ifndef CPARSESNAPSHOTDATA_H
#define CPARSESNAPSHOTDATA_H
#include <QtCore>
#include "parsesnapshotdata.h"
#include <QThread>
#include "taskQue.h"



class CParseSnapshotData : public QThread
{
    Q_OBJECT

signals:
   void sendParserThreadFinish();

private:
    CTaskQue m_inputQue;

public:
    bool addQueData(CTaskQueData data);
    explicit CParseSnapshotData(QObject *parent = 0);

    void run();
};

#endif // CPARSESNAPSHOTDATA_H



