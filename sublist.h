/***************************************************************************
 *
 * Author:		Ake Jacksen
 * Date:		01-01-02	
 * Filename:	sublist.hpp
 *
 *
 ****************************************************************************/

#ifndef SUBLIST_HPP
#define SUBLIST_HPP

#include <string.h>
#include "commonFinance.h"





typedef struct linkStruct LINK_STRUCT_T;
typedef struct dataStruct DATA_STRUCT_T;
typedef struct minMaxStruct MIN_MAX_PRICE_TIME_T;

// Trend data
typedef struct trendDataStruct TREND_DATA_T;
typedef struct simpleMovingAvgStruct SIMPLE_MOV_AVG_T;
typedef struct bollingerBandsStruct BOLLINGER_BANDS_T;


// Indicator data
typedef struct indicatorDataStruct INDICATOR_DATA_T;


// ajn 131211
#define BOOL int

struct minMaxStruct
{
	char minDate[10];
	char maxDate[10];

    BOOL autoScale;
    double minTime;  // Försök att ta bort denna
    double maxTime;	 // Försök att ta bort denna
    double minPrice;
    double maxPrice;
};


// DATA_STRUCT_T
struct dataStruct
{
	char date[15];			// 2001-01-00
//	char month[10];			// Jan
	double price;			// y-value
	unsigned long time;		// x-value (converted date to a 0, 1, 2 ...)
};


// SIMPLE_MOV_AVG_T
struct simpleMovingAvgStruct
{
	bool shortIsUsed;      // Tells if ShortData is valid
	double shortData;		// Store the short simple moving average
	bool mediumIsUsed;
	double mediumData;
	bool longIsUsed;
	double longData;
};





// BOLLINGER_BANDS_T
struct bollingerBandsStruct
{
	double lowerBand;		// Store the short simple moving average
	bool lowerIsUsed;
	double middleBand;
	bool middleIsUsed;
	double upperBand;
	bool upperIsUsed;
};

// TREND_DATA_T
struct trendDataStruct
{
   SIMPLE_MOV_AVG_T simpleMovAvg;
   BOLLINGER_BANDS_T bollBands;
};



// INDICATOR_DATA_T
struct indicatorDataStruct
{
	double aData;			// General storing place for indicators that are shown in ths small window
	bool   aDataIsUsed;
	double bData;
	bool   bDataIsUsed;
	double cData;
	bool   cDataIsUsed;

};






// LINK_STRUCT_T
struct linkStruct
{
    DATA_STRUCT_T data;
	TREND_DATA_T trend;
	INDICATOR_DATA_T indicator;  // ajn 010618
    LINK_STRUCT_T *next;
};







/********************************************************************************
 *
 * Class:		CsubLinkedList
 *
 * Description:
 *
 *
 ********************************************************************************/
class CSubLinkedList{
    
public:
    CSubLinkedList();
    ~CSubLinkedList();
    bool addNode(DATA_STRUCT_T newData);
    void updateTime(void);
    void printList(void);
    bool deleteFirstLink(void);
    bool dateAlreadyExist(DATA_STRUCT_T data);
   	bool minMaxPriceTime(MIN_MAX_PRICE_TIME_T &data, unsigned int typeOfTrendData);
    LINK_STRUCT_T *getRoot(void);
    LINK_STRUCT_T *getNextNode(LINK_STRUCT_T *nextLink);
    void getDatafromLink(DATA_STRUCT_T &newData, const LINK_STRUCT_T *newLink);
	bool minMaxPriceTime(MIN_MAX_PRICE_TIME_T &data, char *minDate, char *maxDate);
	void addTrendDataToLink(unsigned long typeOfTrendData, const TREND_DATA_T newData, 
							LINK_STRUCT_T *newLink); // This data has been calc from base data
	void addIndicatorDataToLink(const INDICATOR_DATA_T newData, LINK_STRUCT_T *newLink);
	bool getMinMaxScale(MIN_MAX_PRICE_TIME_T &data, unsigned long typeOfTrendData,
						char *minDate, char *maxDate);

	
    
    
protected:
	LINK_STRUCT_T   *mRoot;
    void addDataToLink(const DATA_STRUCT_T newData, LINK_STRUCT_T *newLink);
    LINK_STRUCT_T *isEmpty(void);
    void printData(DATA_STRUCT_T data );
    void deleteLink(LINK_STRUCT_T *curr);

	bool getMinMaxScaleBollingerBands(MIN_MAX_PRICE_TIME_T &data, char *minDate, char *maxDate);
	bool getMinMaxScaleLongAvg(MIN_MAX_PRICE_TIME_T &data, char *minDate, char *maxDate);
	bool getMinMaxScaleMiddleAvg(MIN_MAX_PRICE_TIME_T &data, char *minDate, char *maxDate);
	bool getMinMaxScaleShortAvg(MIN_MAX_PRICE_TIME_T &data, char *minDate, char *maxDate);

    
};


#endif


