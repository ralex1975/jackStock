#ifndef MYOS_H
#define MYOS_H

#include <QtCore>
#include <QtGui>

class CMyOs
{
public:
    CMyOs();
    void beep();
    void sleep(int ms);

private:
    QWaitCondition m_waitCondition;
    QMutex m_mutex;

};

#endif // MYOS_H
