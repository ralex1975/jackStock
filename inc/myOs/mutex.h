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
#include "../../inc/core/typedefs.h"
#include <pthread.h>
#include <string>


//=====================================================================
// Class prototype
//=====================================================================
class CMutex
{
private:
	pthread_mutex_t m_mutex;

public:
	CMutex();
	~CMutex();
	int lock();
	int unlock();

};

#endif

