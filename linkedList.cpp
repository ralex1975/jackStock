/**************************************************************************
 *
 * Author:          Ake Jacksén
 * Date:            130112
 *
 * Description:
 * 
 * 
 *
 *
 ***************************************************************************/
#include "linkedList.h"
#include <QtCore>
#include <QMessageBox>




/***************************************************************************
 *
 * Class:    CLinkedList
 *
 * Function: CLinkedList()
 *
 * Description:
 *
 *
 *
 *****************************************************************************/
CLinkedList::~CLinkedList()
{
	deleteList();
}



/***************************************************************************
 *
 * Class:    CLinkedList
 *
 * Function: CLinkedList()
 *
 * Description:
 *
 *
 *
 *****************************************************************************/
CLinkedList::CLinkedList()
{
	m_root = NULL;
	currSelectNode = NULL;
	
}






/***************************************************************************
 *
 * Class:    CFundProcList
 *
 * Function: createNewNode()
 *
 * Description:
 *
 *
 *
 *****************************************************************************/
void CLinkedList::createNewNode(CLinkedListLnk **newLink)
{

    (*newLink) = new CLinkedListLnk;
	(*newLink)->next = NULL;

	if(!(*newLink))
	{
        QMessageBox::critical(NULL, QObject::tr("Fail to create node"), QObject::tr("Could not create a new CLinkedListLnk link"));
		exit(0);
	}

	
}



/***************************************************************************
 *
 * Class:    CFundProcList
 *
 * Function: addNodeInIncreaseOrder()
 *
 * Description:
 *
 *
 *
 *****************************************************************************/
void CLinkedList::
addNodeLast(CData data)
{
    CLinkedListLnk *curr = NULL;
    CLinkedListLnk *prev = NULL;
    CLinkedListLnk *newLink;
		

	if(m_root == NULL)
	{
		createNewNode(&m_root);
		addDataToNode(m_root->data, data);
		return;
	}
	
	curr = m_root;
	while(curr)
	{
		prev = curr;
		curr = curr->next;
	}

	
	// Place new link last in list
	createNewNode(&newLink);
	addDataToNode(newLink->data, data);
	prev->next = newLink;

}




/***************************************************************************
 *
 * Function: addNodeInIncreaseOrder()
 *
 * Description:
 *
 *
 *
 *****************************************************************************/
void CLinkedList::
resetData(CData &data)
{
    data.earningsDividedByDividend.clear();
    data.keyValueDividendPerShare.clear();
    data.keyValueEarningsPerShare.clear();
    data.keyValueNAVPerShare.clear();
    data.lastPrice.clear();
    data.navDivLastStockPrice.clear();
    data.stockName.clear();
}





/***************************************************************************
 *
 * Function: addNodeInIncreaseOrder()
 *
 * Description:
 *
 *
 *
 *****************************************************************************/
void CLinkedList::
addDataToNode(CData &nodeData, CData data)
{
    resetData(nodeData);

    nodeData.earningsDividedByDividend = data.earningsDividedByDividend;
    nodeData.keyValueDividendPerShare = data.keyValueDividendPerShare;
    nodeData.keyValueEarningsPerShare = data.keyValueEarningsPerShare;
    nodeData.keyValueNAVPerShare = data.keyValueNAVPerShare;
    nodeData.lastPrice = data.lastPrice;
    nodeData.navDivLastStockPrice = data.navDivLastStockPrice;
    nodeData.stockName = data.stockName;
}





/***************************************************************************
 *
 * Class:    CLinkedList
 *
 * Function: removeFirst()
 *
 * Description:
 *
 *
 *
 *****************************************************************************/
CLinkedListLnk *CLinkedList::removeFirst(void)
{
	
    CLinkedListLnk *curr = NULL;

	if(m_root == NULL)
		return NULL;

	curr = m_root;
	m_root = m_root->next;
	
	return curr;
	
}


/***************************************************************************
 *
 * Class:    CLinkedList
 *
 * Function: getFirst()
 *
 * Description:
 *
 *
 *
 *****************************************************************************/
bool CLinkedList::isEmpty(void)
{

    if(m_root == NULL)
        return true;
    else
        return false;
}


/***************************************************************************
 *
 * Class:    CLinkedList
 *
 * Function: getFirst()
 *
 * Description:
 *
 *
 *
 *****************************************************************************/
bool CLinkedList::getFirst(CData &data)
{
	
	if(m_root == NULL)
		return false;

	currSelectNode = m_root;
	data = currSelectNode->data;
	return true;
	
}






/***************************************************************************
 *
 * Class:    CLinkedList
 *
 * Function: getNext()
 *
 * Description:
 *
 *
 *
 *****************************************************************************/
bool CLinkedList::getNext(CData &data)
{
	
	if(currSelectNode == NULL)
		return false;
	else if (currSelectNode->next == NULL)
		return false;

	currSelectNode = currSelectNode->next;
	data = currSelectNode->data;
	return true;
	
}







/***************************************************************************
 *
 * Class:    CLinkedList
 *
 * Function: deleteList()
 *
 * Description:
 *
 *
 *
 *****************************************************************************/
void CLinkedList::deleteList(void)
{
    CLinkedListLnk *curr = NULL;
		
	while(m_root)
	{
		curr = m_root;
		m_root = m_root->next;
		delete curr;
	}
}










