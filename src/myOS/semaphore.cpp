/****************************************************************************
 * Filename    : semaphore.cpp
 * Author      : Åke Jacksén
 * Date        : 2012-02-17
 * Copyright   :
 *
 * Description :
 *
 *
 ****************************************************************************/

#define SHARED_BETWEEN_THREADS 0


//=====================================================================
// Imported definitions
//=====================================================================
#include <time.h>
#include "../../inc/core/semaphore.h"
#include "../../inc/core/debug.h"


/***************************************************************
 *
 * Function: CSemaphore()
 *
 * Description: Constructor
 *
 *
 *
 *
 *****************************************************************/
CSemaphore::CSemaphore(int value)
{
    int res;

    res = sem_init(&m_semaphore, SHARED_BETWEEN_THREADS, value);
    if (res)
        ERR("Error: sem_init()");
}




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
CSemaphore::~CSemaphore()
{
    sem_destroy(&m_semaphore);
}


/***************************************************************
 *
 * Function: CMutex()
 *
 * Description: Increment semaphore value
 *
 *
 *
 *
 *****************************************************************/
int CSemaphore::post()
{
    return sem_post(&m_semaphore);
}

/***************************************************************
 *
 * Function: wait()
 *
 * Description: Decrement semaphore value if greater than zero.
 *              When checking value and it equal to zero, block
 *              until value > 0.
 *
 *
 *
 *****************************************************************/
int CSemaphore::wait()
{
    return sem_wait(&m_semaphore);
}


/***************************************************************
 *
 * Function: tryWait()
 *
 * Description: D
 *
 *
 *
 *
 *****************************************************************/
int CSemaphore::tryWait()
{
    return sem_trywait(&m_semaphore);
}


