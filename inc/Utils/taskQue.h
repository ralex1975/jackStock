/****************************************************************************
 * Filename    : taskQue.h
 * Author      : Åke Jacksén
 * Date        : 2012-02-17
 * Copyright   :
 *
 * Description :
 *
 *
 ****************************************************************************/

#ifndef BASE_MESSAGE_QUE
#define BASE_MESSAGE_QUE

//=====================================================================
// Imported definitions
//=====================================================================
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <QtCore>




//=====================================================================
// Forward declaration
//=====================================================================


//=====================================================================
// Constants
//=====================================================================
#define MAX_IP_ADDR_STR_SIZE 40
#define MAX_FAN_PAYLOAD_SIZE 1024

//=====================================================================
// Constants
//=====================================================================
enum TaskQuePrio_ET
{
	QUE_PRIO_LOWEST = 1,
	QUE_PRIO_0 = QUE_PRIO_LOWEST,
	QUE_PRIO_1,
	QUE_PRIO_2,
	QUE_PRIO_3,
	QUE_PRIO_4,
    QUE_PRIO_HIGHEST = QUE_PRIO_4
};


enum TaskQueToDo_ET
{
    PARSER_IS_IDLE,
    PARSE_COMPLETE_SNAPSHOT_DATA
};



//=====================================================================
// Class definition
//=====================================================================
class CTaskQueData
{
public:
    TaskQueToDo_ET toDo;
	TaskQuePrio_ET priority;						// Message priority
};






//=====================================================================
// Class definition
//=====================================================================
class CTaskQueLink
{
public:
	friend class CLinkedList;
	friend class CTaskQueData;
	
	CTaskQueData data;
	CTaskQueLink *next;
};



//=====================================================================
// Class definition
//=====================================================================
class CTaskQue
{

private:
//=========================================================
// Private variables
//==========================================================
	CTaskQueLink *m_root;
	CTaskQueLink *m_currSelectNode;



private:
//===========================================================
// Private function
//===========================================================
	bool createNewNode(CTaskQueLink **newLink);
	bool getFirst(CTaskQueData &data);
	bool getNext(CTaskQueData &data);



	


//===========================================================
// Public functions
//===========================================================
public:
    CTaskQue();
	~CTaskQue();
	
	bool copyFirstData(CTaskQueData &data);
	bool removeFirst(void);
	bool removeFirst(CTaskQueData &data);

	bool addDataLast(CTaskQueData data);
	bool addDataInPriority(CTaskQueData data);

	bool isEmpty(void);
	void deleteList(void);
};

#endif 
