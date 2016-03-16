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
#include <string>
#include "../../inc/core/typedefs.h"
#include "../../inc/core/mutex.h"
#include "../../inc/core/semaphore.h"

using namespace std;

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
	QUE_PRIO_HIGHEST = QUE_PRIO_4,
};



enum TaskQueMsgType_ET
{
	skaTasBort,
};

enum TaskQuePayloadType_ET
{
	FAN_REQ_PAYLOAD,
	FAN_RESP_PAYLOAD,
};

class CFanPayloadType
{
public:
	char frame[MAX_FAN_PAYLOAD_SIZE];
};

union PayloadMsg_UT
{
	CFanPayloadType fanMsg;
};


//=====================================================================
// Class definition
//=====================================================================
class CTaskQueData
{
public:
	TaskQueMsgType_ET mainMsgType;
	unsigned long finalReplyDest;					// The ID of the task that create the req message
	unsigned long fromTask;							// Changed for each task that handle this message
	time_t msgCreateTime;							// Removes old messages
	TaskQuePrio_ET priority;						// Message priority
	char ipAddr[MAX_IP_ADDR_STR_SIZE];				// If fan message witch destination
	unsigned long msgID;							// Used by internal web server
	TaskQuePayloadType_ET payloadType;				// Tell what type of payload
	PayloadMsg_UT payload;							// This shall be in Air Cleaner frame format
};






//=====================================================================
// Class definition
//=====================================================================
class CTaskQueLink
{
public:
	friend class CInputLinkedList;
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
	bool m_isBlockingForever;
	unsigned long m_blockTimeOutMs;
	CSemaphore m_semaphore;
	CMutex m_mutex;


private:
//===========================================================
// Private function
//===========================================================
	bool createNewNode(CTaskQueLink **newLink);
	void addDataToNode(CTaskQueData &nodeData, CTaskQueData data);
	void extractDataFromNode(CTaskQueData &data, CTaskQueData nodeData);
	void resetData(CTaskQueData &data);
	bool getNodeData(TaskQueMsgType_ET comMsg, CTaskQueData &data);
	bool getFirst(CTaskQueData &data);
	bool getNext(CTaskQueData &data);
	void waitForDataOrTimeOut(void);


	


//===========================================================
// Public functions
//===========================================================
public:
	CTaskQue(unsigned long blockTimeOutMs=100, bool blockForever=false, int semaphoreValue=0);
	~CTaskQue();
	
	bool copyFirstData(CTaskQueData &data);
	bool removeFirst(void);
	bool removeFirst(CTaskQueData &data);
	bool getDataRemoveLink(TaskQueMsgType_ET ComMsg, CTaskQueData &data);

	bool addDataLast(CTaskQueData data);
	bool addDataInPriority(CTaskQueData data);

	bool isEmpty(void);
	void deleteList(void);
};

#endif 
