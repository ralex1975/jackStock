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
#include "../../inc/myOs/semaphore.h"
#include "qdebug.h"

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
CSemaphore::CSemaphore(int value):m_semaphore(value)
{
        // qDebug() << "Error: sem_init()";
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
void CSemaphore::post()
{
    m_semaphore.release(1);

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
void CSemaphore::wait()
{
    m_semaphore.acquire(1);
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
bool CSemaphore::tryWait(int ms)
{
    return m_semaphore.tryAcquire(1, ms);
}


