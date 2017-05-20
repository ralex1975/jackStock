/**************************************************************************
 *
 * Author:		Ake Jacksén
 * Date:		130112
 * Filename:    linkedList.h
 *
 * 
 *
 *
 ***************************************************************************/

#ifndef LINKED_LIST
#define LINKED_LIST

#include <QString>


class CData
{
public:
    QString stockName;                      // Stock name
    QString lastPrice;                      // Last stock price
    QString keyValueEarningsPerShare;       // Vinst/aktie
    QString keyValueNAVPerShare;            // Substans/aktie
    QString keyValueDividendPerShare;       // Utdelning/aktie

    QString earningsDividedByDividend;      // Vinst/Utdelning
    // bool earningsDividedByDividendIsEmpty;  // Should we do INSERT or UPDATE

    QString navDivLastStockPrice;           // Substansvärde/Senast Aktiekurs
    // bool navDivLastStockPriceIsEmpty;       // Should we do INSERT or UPDATE
};



class CLinkedListLnk
{
public:
	friend class CInputLinkedList;
    friend class CData;

    CData data;
    CLinkedListLnk *next;


private:
	
	
};


class CLinkedList
{
private:

	//=========================================================
	// Member variables
	//==========================================================
    CLinkedListLnk *m_root;
    CLinkedListLnk *currSelectNode;


	//===========================================================
	// Function prototypes
	//===========================================================
    void createNewNode(CLinkedListLnk **newLink);
	
	
public:
    CLinkedList();
    ~CLinkedList();
	
	bool isEmpty(void);
	void deleteList(void);

    bool getFirst(CData &data);
    bool getNext(CData &data);

    void addDataToNode(CData &nodeData, CData data);
    void addNodeLast(CData data);
    void resetData(CData &data);

    CLinkedListLnk *removeFirst(void);

};


#endif 
