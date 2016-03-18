/****************************************************************************
 * Filename    : baseMutex.h
 * Author      : Åke Jacksén
 * Date        : 2012-02-17
 * Copyright   :
 *
 * Description :
 *
 *
 ****************************************************************************/
#ifndef BASE_MUTEX_H
#define BASE_MUTEX_H


//=====================================================================
// Imported definitions
//=====================================================================
#include "QtCore"
#include <QMutex>


//=====================================================================
// Class prototype
//=====================================================================
class CMutex
{
private:
    QMutex m_mutex;

public:
	CMutex();
	~CMutex();
    void lock();
    void unlock();

};

#endif

