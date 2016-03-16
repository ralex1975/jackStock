/****************************************************************************
 * Filename    : semaphore.h
 * Author      : Åke Jacksén
 * Date        : 2012-02-17
 * Copyright   :
 *
 * Description :
 *
 *
 ****************************************************************************/


#ifndef BASE_SEMAPHORE_H
#define BASE_SEMAPHORE_H

//=====================================================================
// Imported definitions
//=====================================================================
#include <semaphore.h>
#include <errno.h>
#include <time.h>

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


#define SEMAPHORE_INIT_CNT_VALUE   0

//=====================================================================
// Class definitions
//=====================================================================
class CSemaphore
{
private:
    sem_t m_semaphore;


public:
    CSemaphore(int value = 0);
   ~CSemaphore();

    int post(void);
    int wait(void);
    int tryWait(void);
};



#endif
