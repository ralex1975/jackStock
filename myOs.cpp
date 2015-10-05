#include "myOs.h"

CMyOs::CMyOs()
{
}


void CMyOs::beep()
{
    QApplication::beep();
}


void CMyOs::sleep(int ms)
{
    m_waitCondition.wait(&m_mutex, ms);
}
