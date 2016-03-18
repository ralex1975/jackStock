#ifndef TABUYSELLTASK_H
#define TABUYSELLTASK_H


#include <QThread>
#include <QtCore>
#include "../myOs/mytaskQue.h"
#include "stdlib.h"
#include "stdio.h"

extern CMyTaskQue m_taTaskInputQue;
extern CMyTaskQue m_taTaskOutputQue;

class taBuySellTask: public QThread
{
    Q_OBJECT
    FILE *m_file;

public:
    explicit taBuySellTask(QObject *parent = 0);
    void run();

signals:

public slots:

};

#endif // TABUYSELLTASK_H
