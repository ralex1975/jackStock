/****************************************************************************
 * Filename    : taskQue.cpp
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
#include "string.h"
#include "../../inc/taskQue/taskQue.h"
#include "../../inc/core/debug.h"
#include "../../inc/core/util.h"





/***************************************************************************
 *
 * Function: CBaseQue()
 *
 * Description: Constructor
 *
 *
 *
 *****************************************************************************/
CTaskQue::
CTaskQue(unsigned long blockTimeOutMs, bool blockForever, int semaphoreValue):
										m_root(NULL),
										m_currSelectNode(NULL),
										m_isBlockingForever(blockForever),
										m_blockTimeOutMs(blockTimeOutMs),
										m_semaphore(semaphoreValue)
{


}



/***************************************************************************
 *
 * Function: CTaskQue()
 *
 * Description: Destructor
 *
 *
 *
 *****************************************************************************/
CTaskQue::~CTaskQue()
{
	deleteList();

}

/***************************************************************************
 *
 * Function: 	waitForDataOrTimeOut()
 *
 * Description: This function blocks the thread until data data is available
 *              queue or until we get an timeout.
 *
 *****************************************************************************/
void CTaskQue::waitForDataOrTimeOut(void)
{

	if(m_isBlockingForever == true)
	{
		m_semaphore.wait();
	}
	else
	{
		m_semaphore.tryWait();
		if(isEmpty()== true)
		{
			sleepMs(m_blockTimeOutMs);
		}
	}

}




/***************************************************************************
 *
 * Function: isEmpty()
 *
 * Description: Indicates if list is empty or not.
 *
 *
 *
 *****************************************************************************/
bool CTaskQue::isEmpty(void)
{

	if(m_root == NULL)
		return true;

	return false;
}


/***************************************************************************
 *
 * Function: copyFirstData()
 *
 * Description: Copy the first data in this list
 *
 *
 *
 *****************************************************************************/
bool CTaskQue::copyFirstData(CTaskQueData &data)
{
	if((m_isBlockingForever == false) && (m_root == NULL))
	{
		sleepMs(m_blockTimeOutMs);
	}

	if(m_root == NULL)
		return false;

	extractDataFromNode(data, m_root->data);
	return true;
}



/***************************************************************************
 *
 * Function: removeFirst()
 *
 * Description: Remove the first link in linked list
 *
 *
 *
 *****************************************************************************/
bool CTaskQue::removeFirst(void)
{
	CTaskQueLink *curr;

	waitForDataOrTimeOut();

	if(m_root == NULL)
	{
		return false;
	}

	m_mutex.lock();
	curr = m_root;
	m_root = m_root->next;
	delete curr;
	m_mutex.unlock();

	return true;
}



/***************************************************************************
 *
 * Function: removeFirst()
 *
 * Description: Remove the first link in linked list
 *
 *
 *
 *****************************************************************************/
bool CTaskQue::removeFirst(CTaskQueData &data)
{
	CTaskQueLink *curr = NULL;

	waitForDataOrTimeOut();

	if(m_root == NULL)
		return false;

	m_mutex.lock();
	curr = m_root;
	m_root = m_root->next;
	//data = curr->data;
	extractDataFromNode(data, curr->data);
	//printf("qFrame: %s\n", curr->data.payload.fanMsg.frame);
	delete curr;
	m_mutex.unlock();

	return true;
}


/***************************************************************************
 *
 * Function: getDataRemoveLink()
 *
 * Description: This function search for a searchKey if it is found
 *              this data is retrieved and the link is removed from the list.
 *
 *
 *****************************************************************************/
bool CTaskQue::getDataRemoveLink(TaskQueMsgType_ET comMsgType, CTaskQueData &data)
{
	CTaskQueLink *tmpLink = NULL;
	CTaskQueLink *curr = NULL;
	CTaskQueLink *prev = NULL;

	waitForDataOrTimeOut();


	if(m_root == NULL)
	{
		return false;
	}

	m_mutex.lock();

	curr = m_root;
	while(curr != NULL)
	{
		if(comMsgType == data.mainMsgType)
		{
			// Is it first node
			if(prev == NULL)
			{
				tmpLink = m_root;
				//data = tmpLink->data;
				extractDataFromNode(data, tmpLink->data);
				m_root = m_root->next;
				delete tmpLink;
				m_mutex.unlock();
				return true;
			}
			else
			{
				prev->next = curr->next;
				tmpLink = curr;
				// data = tmpLink->data;
				extractDataFromNode(data, tmpLink->data);
				delete tmpLink;
				m_mutex.unlock();
				return true;
			}
		}
		prev = curr;
		curr = curr->next;
	}
	m_mutex.unlock();

	return false;
}






/***************************************************************************
 *
 * Function:	addDataLast()
 *
 * Description: Add data last in linked list
 *
 *
 *
 *****************************************************************************/
bool CTaskQue::
addDataLast(CTaskQueData data)
{
	CTaskQueLink *curr = NULL;
	CTaskQueLink *prev = NULL;
	CTaskQueLink *newLink;

	m_mutex.lock();

	if(m_root == NULL)
	{
		if(createNewNode(&m_root) == true)
		{
			addDataToNode(m_root->data, data);
			m_mutex.unlock();
			m_semaphore.post();
			return true;
		}
		m_mutex.unlock();
		return false;
	}
	
	curr = m_root;

	while(curr)
	{
		prev = curr;
		curr = curr->next;
	}

	// Place new link last in list
	if(createNewNode(&newLink) == true)
	{
		addDataToNode(newLink->data, data);
		prev->next = newLink;
		m_mutex.unlock();
		m_semaphore.post();
		return true;
	}
	m_mutex.unlock();
	return false;
}




/***************************************************************************
 *
 * Function: addDataInPriority()
 *
 * Description: Insert data in sort order.
 *
 *
 *
 *****************************************************************************/
bool CTaskQue::
addDataInPriority(CTaskQueData data)
{
	CTaskQueLink *curr = NULL;
	CTaskQueLink *prev = NULL;
	CTaskQueLink *newLink;
	
	m_mutex.lock();
	
	if(m_root == NULL)
	{
		if(createNewNode(&m_root) == true)
		{
			addDataToNode(m_root->data, data);
			m_mutex.unlock();
			m_semaphore.post();
			return true;
		}
		m_mutex.unlock();
		return false;
	}
	
	curr = m_root;
	while(curr)
	{
		// Is new key smaller than current node search key?
		if( data.priority > curr->data.priority )
		{
			if(createNewNode(&newLink)== true)
			{
				addDataToNode(newLink->data, data);
			}
			else
			{
				m_mutex.unlock();
				return false;
			}
		
			// Place new link first in list?
			if(prev == NULL)
			{
				newLink->next = m_root;
				m_root = newLink;
				m_mutex.unlock();
				m_semaphore.post();
				return true;
			}
			// Place new link between first and last link in list
			else
			{
				newLink->next = prev->next;
				prev->next = newLink;
				m_mutex.unlock();
				m_semaphore.post();
				return true;
			}
		}
		prev = curr;
		curr = curr->next;
	}

	// Place new link last in list
	if(createNewNode(&newLink)==true)
	{
		addDataToNode(newLink->data, data);
		prev->next = newLink;
		m_mutex.unlock();
		m_semaphore.post();
		return true;
	}
	m_mutex.unlock();
	return false;
}




/***************************************************************************
 *
 * Function: deleteList()
 *
 * Description: erase the whole linked list
 *
 *
 *
 *****************************************************************************/
void CTaskQue::deleteList(void)
{
	CTaskQueLink *curr = NULL;

	m_mutex.lock();

	while(m_root)
	{
		curr = m_root;
		m_root = m_root->next;
		m_semaphore.tryWait();
		delete curr;
	}
	m_mutex.unlock();

}



/***************************************************************************
 *
 * Function: getFirst()
 *
 * Description: Extract data from first link in liked list.
 *
 * Note:		Link is not removed from list
 *
 *****************************************************************************/
bool CTaskQue::getFirst(CTaskQueData &data)
{
	if(m_root == NULL)
		return false;

	m_mutex.lock();
	m_currSelectNode = m_root;
	//data = m_currSelectNode->data;
	extractDataFromNode(data, m_currSelectNode->data);
	m_mutex.unlock();
	return true;
}



/***************************************************************************
 *
 * Function: getNext()
 *
 * Description: Get next data in linked list.
 *
 * Note: The function getFirst() must be used before the function getNext() is used 
 *
 *****************************************************************************/
bool CTaskQue::getNext(CTaskQueData &data)
{
	
	if(m_currSelectNode == NULL)
		return false;
	else if (m_currSelectNode->next == NULL)
		return false;
	
	m_mutex.lock();
	m_currSelectNode = m_currSelectNode->next;
	//data = m_currSelectNode->data;
	extractDataFromNode(data, m_currSelectNode->data);
	m_mutex.unlock();

	return true;	
}


/***************************************************************************
 *
 * Function: addDataToNode()
 *
 * Description: Insert data in a link
 *
 *
 *
 *****************************************************************************/
void CTaskQue::
addDataToNode(CTaskQueData &nodeData, CTaskQueData data)
{
	m_mutex.lock();
	int i;

	nodeData.finalReplyDest = data.finalReplyDest;
	nodeData.fromTask = data.fromTask;

	// Check if it is possible to copy IP addr
	for(i = 0; i < MAX_IP_ADDR_STR_SIZE; i++)
	{
		if(data.ipAddr[i]== 0)
			strcpy(nodeData.ipAddr, data.ipAddr);
	}

	nodeData.mainMsgType = data.mainMsgType;
	nodeData.msgCreateTime = data.msgCreateTime;
	nodeData.msgID = data.msgID;
	nodeData.payloadType = data.payloadType;


	switch(nodeData.payloadType)
	{
	case FAN_REQ_PAYLOAD:
	case FAN_RESP_PAYLOAD:
		for(i = 0; i < MAX_FAN_PAYLOAD_SIZE; i++)
		{
			if(data.ipAddr[i] == 0)
				strcpy(nodeData.payload.fanMsg.frame, data.payload.fanMsg.frame);
		}
		break;
	default:
		ERR("Error: Unknown payload type. Line: %d, file: %s", __LINE__, __FILE__);
	}

	nodeData.priority = data.priority;
	m_mutex.unlock();
}



/***************************************************************************
 *
 * Function: 	extractDataFromNode()
 *
 * Description: Extract data from link
 *
 *
 *
 *****************************************************************************/
void CTaskQue::
extractDataFromNode(CTaskQueData &data, CTaskQueData nodeData)
{

	int i;

	m_mutex.lock();
	data.finalReplyDest = nodeData.finalReplyDest;
	data.fromTask = nodeData.fromTask;

	// Check if it is possible to copy IP addr
	for(i = 0; i < MAX_IP_ADDR_STR_SIZE; i++)
	{
		if(nodeData.ipAddr[i]== 0)
			strcpy(data.ipAddr, nodeData.ipAddr);
	}

	data.mainMsgType = nodeData.mainMsgType;
	data.msgCreateTime = nodeData.msgCreateTime;
	data.msgID = nodeData.msgID;
	data.payloadType = nodeData.payloadType;

	switch(nodeData.payloadType)
	{
	case FAN_REQ_PAYLOAD:
	case FAN_RESP_PAYLOAD:
		for(i = 0; i < MAX_FAN_PAYLOAD_SIZE; i++)
		{
				if(nodeData.payload.fanMsg.frame[i] == 0)
					strcpy(data.payload.fanMsg.frame, nodeData.payload.fanMsg.frame);
		}
		break;
	default:
		ERR("Error: Unknown payload type Line: %d, file: %s", __LINE__, __FILE__);
	}


	data.priority = nodeData.priority;
	m_mutex.unlock();
}




/***************************************************************************
 *
 * Function: getNodeData()
 *
 * Description: This function find the link that contains comMsgType search key
 *				and extract all data from this link.
 *
 *				Note: No data is removed
 *
 *
 *
 *****************************************************************************/
bool CTaskQue::getNodeData(TaskQueMsgType_ET comMsgType, CTaskQueData &data)
{
	bool res;

	m_mutex.lock();
	res = getFirst(data);
	while(res)
	{
		if(comMsgType == data.mainMsgType)
		{
			m_mutex.unlock();
			return true;
		}
		res = getNext(data);
	}
	m_mutex.unlock();
	return false;
}



/***************************************************************************
 *
 * Function:	resetData()
 *
 * Description: Reset all arrays
 *
 *
 *
 *****************************************************************************/
void CTaskQue::resetData(CTaskQueData &data)
{
	m_mutex.lock();
	data.ipAddr[0] = 0;
	data.payload.fanMsg.frame[0] = 0;
	m_mutex.unlock();
}





/***************************************************************************
 *
 * Function:	createNewNode()
 *
 * Description: Creates a new linked list node.
 *
 *
 *
 *****************************************************************************/
bool CTaskQue::createNewNode(CTaskQueLink **newLink)
{
	m_mutex.lock();

	(*newLink) = new CTaskQueLink;

	if(!(*newLink))
	{
		ERR("Internal Error: Could not create a new CTaskQueLink link");
		m_mutex.unlock();
		return false;
	}

	(*newLink)->next = NULL;

	m_mutex.unlock();
	return true;
}



