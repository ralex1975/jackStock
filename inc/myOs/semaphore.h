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
#include <QSemaphore>
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
    QSemaphore m_semaphore;


public:
    CSemaphore(int value = 0);
   ~CSemaphore();

    void post(void);
    void wait(void);
    bool tryWait(int ms);
};



#endif
