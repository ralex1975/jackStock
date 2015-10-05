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
#include "taskQue.h"
#include <QMessageBox>

//#define ERR




/***************************************************************************
 *
 * Function: CBaseQue()
 *
 * Description: Constructor
 *
 *
 *
 *****************************************************************************/
CTaskQue::CTaskQue():
                m_root(NULL),
                m_currSelectNode(NULL)
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


	if(m_root == NULL)
		return false;

    data = m_root->data;
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


	if(m_root == NULL)
	{
		return false;
	}


	curr = m_root;
	m_root = m_root->next;
	delete curr;


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


	if(m_root == NULL)
		return false;


	curr = m_root;
	m_root = m_root->next;
    data = curr->data;
	delete curr;


	return true;
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
bool CTaskQue::addDataLast(CTaskQueData data)
{
	CTaskQueLink *curr = NULL;
	CTaskQueLink *prev = NULL;
	CTaskQueLink *newLink;



	if(m_root == NULL)
	{
		if(createNewNode(&m_root) == true)
		{
            m_root->data = data;
			return true;
		}
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
        newLink->data = data;
		prev->next = newLink;
		return true;
	}

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
bool CTaskQue::addDataInPriority(CTaskQueData data)
{
	CTaskQueLink *curr = NULL;
	CTaskQueLink *prev = NULL;
	CTaskQueLink *newLink;
	

	
	if(m_root == NULL)
	{
		if(createNewNode(&m_root) == true)
		{
            m_root->data = data;
			return true;
		}
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
                newLink->data = data;
			}
			else
			{

				return false;
			}
		
			// Place new link first in list?
			if(prev == NULL)
			{
				newLink->next = m_root;
				m_root = newLink;
				return true;
			}
			// Place new link between first and last link in list
			else
			{
				newLink->next = prev->next;
				prev->next = newLink;
				return true;
			}
		}
		prev = curr;
		curr = curr->next;
	}

	// Place new link last in list
	if(createNewNode(&newLink)==true)
	{
        newLink->data = data;
		prev->next = newLink;
		return true;
	}
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


	while(m_root)
	{
		curr = m_root;
		m_root = m_root->next;
		delete curr;
	}

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


    m_currSelectNode = m_root;
    data = m_currSelectNode->data;

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
		
	m_currSelectNode = m_currSelectNode->next;
    data = m_currSelectNode->data;
	return true;	
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

	(*newLink) = new CTaskQueLink;

	if(!(*newLink))
	{
        //ERR("Internal Error: Could not create a new CTaskQueLink link");
        QMessageBox::critical(NULL, QObject::tr("CTaskQue::createNewNode()"), QObject::tr("Internal Error: Could not create a new CTaskQueLink link"));
		return false;
	}

    (*newLink)->next = NULL;

	return true;
}



