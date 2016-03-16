/****************************************************************************
 * Filename    : mutex.cpp
 * Author      : Åke Jacksén
 * Date        : 2012-02-17
 * Copyright   :
 *
 * Description :
 *
 *
 ****************************************************************************/


//=====================================================================
// Imported definitions
//=====================================================================
#include "../../inc/core/mutex.h"
#include "../../inc/core/debug.h"


/***************************************************************
 *
 * Function: CMutex()
 *
 * Description: Constructor
 *
 *
 *
 *
 *****************************************************************/
CMutex::CMutex()
{
	int res;
	pthread_mutexattr_t attr;

	//INFO("Mutex init\n");
	res = pthread_mutexattr_init(&attr);
	if(res)
		ERR("Error: pthread_mutexattr_init()");


	res = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
	if(res)
		ERR("Error: pthread_mutexattr_settype()");


	res = pthread_mutex_init(&m_mutex, &attr);
	if (res)
		ERR("Error: pthread_mutex_init()");

	    pthread_mutexattr_destroy(&attr);
}



/***************************************************************
 *
 * Function: ~CMutex()
 *
 * Description: Destructor
 *
 *
 *
 *
 *****************************************************************/
CMutex::~CMutex()
{
	//INFO("Mutex destroy\n");
	pthread_mutex_destroy(&m_mutex);
}


/***************************************************************
 *
 * Function: lock()
 *
 * Description:
 *
 *
 *
 *
 *****************************************************************/
int CMutex::lock()
{
	//INFO("Mutex lock\n");
	return pthread_mutex_lock(&m_mutex);
}


/***************************************************************
 *
 * Function: unlock()
 *
 * Description:
 *
 *
 *
 *
 *****************************************************************/
int CMutex::unlock()
{
	//INFO("Mutex unlock\n");
	return pthread_mutex_unlock(&m_mutex);

}


