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

#ifndef MY_TASK_QUEUE_QUE
#define MY_TASK_QUEUE_QUE

//=====================================================================
// Imported definitions
//=====================================================================
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include "../../inc/myOs/mutex.h"
#include "../../inc/myOs/semaphore.h"
#include "yahooStockPlotUtil.h"


//=====================================================================
// Forward declaration
//=====================================================================



//=====================================================================
// Constants
//=====================================================================






//=====================================================================
// Class definition
//=====================================================================
class CMyTaskQueData
{
public:
    QString stockSymbol;
    QString endDate;
    QString startDate;

    // Store one stock with data
    CYahooStockPlotUtil::StockData_ST stockPrice;
    CYahooStockPlotUtil::StockData_ST avgShortData;
    CYahooStockPlotUtil::StockData_ST avgMidData;
    CYahooStockPlotUtil::StockData_ST avgLongData;

    CYahooStockPlotUtil::StockData_ST macdData;
    CYahooStockPlotUtil::StockData_ST rsiData;
    CYahooStockPlotUtil::StockData_ST stochasticsData;
};






//=====================================================================
// Class definition
//=====================================================================
class CMyTaskQueLink
{
public:
	friend class CInputLinkedList;
	friend class CTaskQueData;
	
    CMyTaskQueData data;
    CMyTaskQueLink *next;
};



//=====================================================================
// Class definition
//=====================================================================
class CMyTaskQue
{

private:
//=========================================================
// Private variables
//==========================================================
    CMyTaskQueLink *m_root;
    CMyTaskQueLink *m_currSelectNode;
	bool m_isBlockingForever;
    int m_blockTimeOutMs;
	CSemaphore m_semaphore;
	CMutex m_mutex;


private:
//===========================================================
// Private function
//===========================================================
    bool createNewNode(CMyTaskQueLink **newLink);
    void addDataToNode(CMyTaskQueData &nodeData, CMyTaskQueData data);
    void extractDataFromNode(CMyTaskQueData &data, CMyTaskQueData nodeData);
    void resetData(CMyTaskQueData &data);
    bool getNodeData(CMyTaskQueData &data);
    bool getFirst(CMyTaskQueData &data);
    bool getNext(CMyTaskQueData &data);
	void waitForDataOrTimeOut(void);
    void copyDataVectorData(QVector <double> &outData, QVector <double> inData);
    void copyDataVectorData(QVector <QString> &outData, QVector <QString> inData);
    void copyXYPlotData(CYahooStockPlotUtil::XYPlotData_ST &outData, CYahooStockPlotUtil::XYPlotData_ST inData);




	


//===========================================================
// Public functions
//===========================================================
public:
    CMyTaskQue(int blockTimeOutMs = 100, bool blockForever = false, int semaphoreValue = 0);
    ~CMyTaskQue();
	
    bool copyFirstData(CMyTaskQueData &data);
	bool removeFirst(void);
    bool removeFirst(CMyTaskQueData &data);

    bool addDataLast(CMyTaskQueData data);
    bool addDataInPriority(CMyTaskQueData data);

	bool isEmpty(void);
	void deleteList(void);
};

#endif 
