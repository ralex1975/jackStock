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
#include "mytaskQue.h"
#include "qdebug.h"





/***************************************************************************
 *
 * Function: CBaseQue()
 *
 * Description: Constructor
 *
 *
 *
 *****************************************************************************/
 CMyTaskQue::
 CMyTaskQue(int blockTimeOutMs,
            bool blockForever,
            int semaphoreValue):m_root(NULL),
                                m_currSelectNode(NULL),
                                m_isBlockingForever(blockForever),
                                m_blockTimeOutMs(blockTimeOutMs),
                                m_semaphore(semaphoreValue)
{


}



/***************************************************************************
 *
 * Function:  CMyTaskQue()
 *
 * Description: Destructor
 *
 *
 *
 *****************************************************************************/
CMyTaskQue::~ CMyTaskQue()
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
void  CMyTaskQue::waitForDataOrTimeOut(void)
{

	if(m_isBlockingForever == true)
	{
		m_semaphore.wait();
	}
	else
	{
        m_semaphore.tryWait(m_blockTimeOutMs);
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
bool  CMyTaskQue::isEmpty(void)
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
bool  CMyTaskQue::copyFirstData( CMyTaskQueData &data)
{
	if((m_isBlockingForever == false) && (m_root == NULL))
	{
        m_semaphore.tryWait(m_blockTimeOutMs);
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
bool  CMyTaskQue::removeFirst(void)
{
     CMyTaskQueLink *curr;

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
bool  CMyTaskQue::removeFirst( CMyTaskQueData &data)
{
     CMyTaskQueLink *curr = NULL;

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
 * Function:	addDataLast()
 *
 * Description: Add data last in linked list
 *
 *
 *
 *****************************************************************************/
bool  CMyTaskQue::
addDataLast( CMyTaskQueData data)
{
     CMyTaskQueLink *curr = NULL;
     CMyTaskQueLink *prev = NULL;
     CMyTaskQueLink *newLink;

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
 * Function: deleteList()
 *
 * Description: erase the whole linked list
 *
 *
 *
 *****************************************************************************/
void  CMyTaskQue::deleteList(void)
{
     CMyTaskQueLink *curr = NULL;

	m_mutex.lock();

	while(m_root)
	{
		curr = m_root;
		m_root = m_root->next;
        // m_semaphore.tryWait();
        m_semaphore.wait();
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
bool  CMyTaskQue::getFirst( CMyTaskQueData &data)
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
bool  CMyTaskQue::getNext( CMyTaskQueData &data)
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
void  CMyTaskQue::
addDataToNode( CMyTaskQueData &nodeData,  CMyTaskQueData data)
{
	m_mutex.lock();

    nodeData.stockSymbol = data.stockSymbol;
    nodeData.endDate  = data.endDate;
    nodeData.startDate  = data.startDate;

    copyXYPlotData(nodeData.stockPrice.data, data.stockPrice.data);
    copyXYPlotData(nodeData.avgShortData.data, data.avgShortData.data);
    copyXYPlotData(nodeData.avgMidData.data, data.avgMidData.data);
    copyXYPlotData(nodeData.avgLongData.data, data.avgLongData.data);
    copyXYPlotData(nodeData.macdData.data, data.macdData.data);
    copyXYPlotData(nodeData.rsiData.data, data.rsiData.data);
    copyXYPlotData(nodeData.stochasticsData.data, data.stochasticsData.data);

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
void  CMyTaskQue::
extractDataFromNode(CMyTaskQueData &data,  CMyTaskQueData nodeData)
{

	m_mutex.lock();

    data.stockSymbol = nodeData.stockSymbol;
    data.endDate  = nodeData.endDate;
    data.startDate  = nodeData.startDate;

    copyXYPlotData(data.stockPrice.data, nodeData.stockPrice.data);
    copyXYPlotData(data.avgShortData.data, nodeData.avgShortData.data);
    copyXYPlotData(data.avgMidData.data, nodeData.avgMidData.data);
    copyXYPlotData(data.avgLongData.data, nodeData.avgLongData.data);
    copyXYPlotData(data.macdData.data, nodeData.macdData.data);
    copyXYPlotData(data.rsiData.data, nodeData.rsiData.data);
    copyXYPlotData(data.stochasticsData.data, nodeData.stochasticsData.data);

    m_mutex.unlock();
}



/***************************************************************************
 *
 * Function: ()
 *
 * Description:
 *
 *
 *
 *
 *****************************************************************************/
void CMyTaskQue::
copyXYPlotData(CYahooStockPlotUtil::XYPlotData_ST &outData, CYahooStockPlotUtil::XYPlotData_ST inData)
{
    copyDataVectorData(outData.x, inData.x);
    copyDataVectorData(outData.y, inData.y);
    copyDataVectorData(outData.xDate, inData.xDate);

    copyDataVectorData(outData.indicator1, inData.indicator1);
    copyDataVectorData(outData.indicator2, inData.indicator2);
    copyDataVectorData(outData.indicator3, inData.indicator3);
    copyDataVectorData(outData.indicator4, inData.indicator4);
    copyDataVectorData(outData.indicator5, inData.indicator5);
}


/***************************************************************************
 *
 * Function: ()
 *
 * Description:
 *
 *
 *
 *
 *****************************************************************************/
void CMyTaskQue::copyDataVectorData(QVector <QString> &outData, QVector <QString> inData)
{
    int i;

    if((false == inData.isEmpty()))
    {
        for(i = 0; i < inData.size(); i++)
        {
            outData[i] = inData[i];
        }
    }


}


/***************************************************************************
 *
 * Function: ()
 *
 * Description:
 *
 *
 *
 *
 *****************************************************************************/
void CMyTaskQue::copyDataVectorData(QVector <double> &outData, QVector <double> inData)
{
    int i;

    if((false == inData.isEmpty()))
    {
        for(i = 0; i < inData.size(); i++)
        {
            outData[i] = inData[i];
        }
    }
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
bool  CMyTaskQue::getNodeData(CMyTaskQueData &data)
{
	bool res;

	m_mutex.lock();
	res = getFirst(data);
	m_mutex.unlock();
    return res;
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
void  CMyTaskQue::resetData(CMyTaskQueData &data)
{
	m_mutex.lock();

    data.stockSymbol.clear();
    data.endDate.clear();
    data.startDate.clear();

    data.stockPrice.data.x.clear();
    data.stockPrice.data.y.clear();
    data.stockPrice.data.xDate.clear();
    data.stockPrice.data.indicator1.clear();
    data.stockPrice.data.indicator2.clear();
    data.stockPrice.data.indicator3.clear();
    data.stockPrice.data.indicator4.clear();
    data.stockPrice.data.indicator5.clear();


    data.avgShortData.data.x.clear();
    data.avgShortData.data.y.clear();
    data.avgShortData.data.xDate.clear();
    data.avgShortData.data.indicator1.clear();
    data.avgShortData.data.indicator2.clear();
    data.avgShortData.data.indicator3.clear();
    data.avgShortData.data.indicator4.clear();
    data.avgShortData.data.indicator5.clear();



    data.avgMidData.data.x.clear();
    data.avgMidData.data.y.clear();
    data.avgMidData.data.xDate.clear();
    data.avgMidData.data.indicator1.clear();
    data.avgMidData.data.indicator2.clear();
    data.avgMidData.data.indicator3.clear();
    data.avgMidData.data.indicator4.clear();
    data.avgMidData.data.indicator5.clear();


    data.avgLongData.data.x.clear();
    data.avgLongData.data.y.clear();
    data.avgLongData.data.xDate.clear();
    data.avgLongData.data.indicator1.clear();
    data.avgLongData.data.indicator2.clear();
    data.avgLongData.data.indicator3.clear();
    data.avgLongData.data.indicator4.clear();
    data.avgLongData.data.indicator5.clear();


    data.macdData.data.x.clear();
    data.macdData.data.y.clear();
    data.macdData.data.xDate.clear();
    data.macdData.data.indicator1.clear();
    data.macdData.data.indicator2.clear();
    data.macdData.data.indicator3.clear();
    data.macdData.data.indicator4.clear();
    data.macdData.data.indicator5.clear();

    data.rsiData.data.x.clear();
    data.rsiData.data.y.clear();
    data.rsiData.data.xDate.clear();
    data.rsiData.data.indicator1.clear();
    data.rsiData.data.indicator2.clear();
    data.rsiData.data.indicator3.clear();
    data.rsiData.data.indicator4.clear();
    data.rsiData.data.indicator5.clear();

    data.stochasticsData.data.x.clear();
    data.stochasticsData.data.y.clear();
    data.stochasticsData.data.xDate.clear();
    data.stochasticsData.data.indicator1.clear();
    data.stochasticsData.data.indicator2.clear();
    data.stochasticsData.data.indicator3.clear();
    data.stochasticsData.data.indicator4.clear();
    data.stochasticsData.data.indicator5.clear();


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
bool  CMyTaskQue::createNewNode( CMyTaskQueLink **newLink)
{
	m_mutex.lock();

    (*newLink) = new  CMyTaskQueLink;

	if(!(*newLink))
	{
        qDebug() << "Internal Error: Could not create a new  CMyTaskQueLink link";
		m_mutex.unlock();
		return false;
	}

	(*newLink)->next = NULL;

	m_mutex.unlock();
	return true;
}



