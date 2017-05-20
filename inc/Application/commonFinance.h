/********************************************************
 *
 * Author:		Ake Jacksen
 * Date:		001213
 * Filename:	common.hpp
 *
 ********************************************************/
#ifndef COMMON_HPP
#define COMMON_HPP

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// This definitions is used when any of the trendanalysis parameters
// have been calculated for a fund and stored in a sub list
#define NO_TREND_PARAMS_IS_CALC	0

#define SHORT_SIMPLE_MOV_AVG				(1 << 0)
#define MEDIUM_SIMPLE_MOV_AVG				(1 << 1)  
#define LONG_SIMPLE_MOV_AVG					(1 << 2)
#define SHORT_EXP_MOV_AVG					(1 << 3)
#define MEDIUM_EXP_MOV_AVG					(1 << 4)  
#define LONG_EXP_MOV_AVG					(1 << 5)
#define BOLLINGER_BANDS						(1 << 6)
#define LOWER_BOLLINGER_BANDS				(1 << 7)  
#define MIDDLE_BOLLINGER_BANDS				(1 << 8) 
#define UPPER_BOLLINGER_BANDS				(1 << 9)         


// This definitions is used when any of the indicatoranalysis parameters
// have been calculated for a fund and stored in a sub list
// Note the values selectet below depends on the order of the radiobuttons
// In the Oscillator dialog
#define NO_INDICATOR_PARAMS_IS_CALC			 0
#define IND_MOMENTUM						 1
#define IND_STO								 2 		
#define IND_MACD							 3 	
#define IND_RSI								 4
#define IND_RATE_OF_CHANGE                   5 


#define MAX_COLORS							50	
#define MAX_COLOR_TEXT_LENGTH				30

#define MAX_LINE_TYPES						20
#define MAX_LINE_TYPES_LENGTH				30

#define MAX_AVG_TYPES						10
#define MAX_AVG_TYPES_LENGTH				30


// This definds are used when default main settings are init
// MS_ = Main settings
#define MS_BLACK							0
#define MS_BLUE								1
#define MS_GREEN							2
#define MS_CYAN								3
#define MS_RED								4
#define MS_MAGENTA							5
#define MS_YELLOW							6
#define MS_DARK_BLUE						7
#define MS_DARK_GREEN						8
#define MS_DARK_CYAN						9
#define MS_DARK_RED							10
#define MS_DARK_MAGENTA						11
#define MS_DARK_YELLOW						12
#define MS_MAX_COLORS						13

#define MS_SOLID							0
#define MS_DOT								1
#define MS_DASH								2
#define MS_DASHDOT							3
#define MS_DASHDOTDOT						4
#define MS_MAX_LINE_TYPES					5
	
#define MS_SIMPLE							0
#define MS_EXP								1
#define MS_MAX_AVG_TYPES					2		


// Is used when lines are draw
#define DRAW_COLOR_GREEN					0
#define DRAW_COLOR_RED						1
#define DRAW_COLOR_NOT_SEL					2
#define DRAW_REMOVE_LINE					3

#define DRAW_BETWEEN_BEGIN_END				0
#define DRAW_BEGIN							1
#define DRAW_END							2
#define DRAW_NOTHING						3


// Define where data displayed, in the small windows, is stored i the sublist 
#define A_DATA 0
#define B_DATA 1
#define C_DATA 2


typedef struct colorStruct COLOR_T;
typedef struct lineTypeStruct LINE_T;
typedef struct trendPropPageStruct TREND_PROP_PAGE_T;
typedef struct mainPropPageSettings MAIN_PROP_PAGE_T;


// These structures is used store the adjustable trend settings in the program
typedef struct commonAverageStruct COMMON_AVG_T;
typedef struct commonBollingerBandStruct COMMON_BB_T;
typedef struct commonTrendSettingStruct COMMON_TREND_SETTING_T;
typedef struct CommonMainSettingsStruct COMMON_MAIN_SETTINGS_T;
typedef struct TrendSettingPropPgDataStruct TREND_SETTING_PROP_PG_DATA;


// These structures is used store the adjustable indicator settings in the program
typedef struct commonIndicatorStruct COMMON_INDICATOR_SETTING_T;


// Is used when lines are draw
typedef struct DrawListLink DRAW_LIST_LINK_T;

// Is used when lines are draw
struct DrawListLink
{
	unsigned long x;
	unsigned long y;
	int status; // begin,  end
	int color;
};

// ajn 131211
#define COLORREF long
#define AfxMessageBox


//COLOR_T
struct colorStruct
{
	char text[40];
	COLORREF RGB;
};



//LINE_T
struct lineTypeStruct
{
	char text[40];
	int lineStyle;
};


// This struct store the curr selections for each combo and editboxes
// on the trend property page
// TREND_PROP_PAGE_T;
struct trendPropPageStruct
{
	int	shortAvgColorIndex;     // mean average
	int	shortAvgLineTypeIndex;
	int	shortAvgTypeIndex;
	long shortAvgDays;

	int	mediumAvgColorIndex;
	int	mediumAvgLineTypeIndex;
	int	mediumAvgTypeIndex;
	long mediumAvgDays;

	int	longAvgLineTypeIndex;
	int	longAvgColorIndex;
	int	longAvgTypeIndex;
	long longAvgDays;

	double BBandNumbStdDev;   // Bollinger band
	double BBAvgDays;
	int	BBUpColorIndex;
	int	BBMidColorIndex;
	int	BBLowColorIndex;

};



// MAIN_PROP_PAGE_T	
struct mainPropPageSettings
{
	TREND_PROP_PAGE_T trend;

};



// These structs are used when curves are plotted
// COMMON_AVG_T
struct commonAverageStruct
{
	int	longAvgLineType;
	COLORREF longAvgColor;
	unsigned short longAvgType;
	unsigned long longAvgDays;

	int mediumAvgLineType;
	COLORREF mediumAvgColor;
	unsigned short mediumAvgType;
	unsigned long mediumAvgDays;

	int shortAvgLineType;
	COLORREF shortAvgColor;
	unsigned short shortAvgType;
	unsigned long shortAvgDays;
	
};



// These structs are used when graph plots curves
// COMMON_BB_T
struct commonBollingerBandStruct
{
	COLORREF UpperColor;
	COLORREF MidColor;
	COLORREF LowColor;
	double AvgDays;
	double NumbStdDev;
};


//COMMON_TREND_SETTING_T
struct commonTrendSettingStruct
{
	COMMON_AVG_T avg;
	COMMON_BB_T bb;
	unsigned long trendDialogSettings;// Store settings of trendTools dialog
									  // and the settings of the prop page
									  // and combines these two settings to	
									  // for example SHORT_SIMPLE_MOV_AVG 	
};


//COMMON_INDICATOR_SETTING_T
struct commonIndicatorStruct
{
 unsigned char indicatorType;  // Only one indicator may be selected at each time.   
 double RSIAvgDays;
 double RateOfChargeDays;
 double StochasticMinMaxDays;
 double procDAvgTime;
 double momentumDays;
 double MACDAvgLongTimePeriod;
 double MACDAvgShortTimePeriod;
 double MACDAvgSignalTimePeriod;
 bool plotDataInTwoWindows;
 int radiatorButtonSettingsOscDialog;
};


//COMMON_MAIN_SETTINGS_T
struct CommonMainSettingsStruct
{
	COMMON_TREND_SETTING_T trend;
	COMMON_INDICATOR_SETTING_T indicator;

};




// TREND_SETTING_PROP_PG_DATA
// This data are found in the combo boxes and edit boxes
// on the property page that shows the trend data. This data has 
// to be converted be for it can be used by graph.cpp and so on
struct TrendSettingPropPgDataStruct
{
	char color[30];
	char lineType[30];
	char movAvgType[30];
};
 



extern char errMes[50][80];

typedef unsigned short USHORT;

// You need to update both ERR_ENUM_T and ERR_TXT_T
// ERR_TXT_T is located in common.cpp
typedef enum errorList
{ 
	ERR_OK,
	ERR_CREATE_MAIN_LINK, 
	ERR_CREATE_SUB_LINK,
	ERR_PARSER_FILE_NOT_A_NUMB,
	ERR_PARSER_FILE_TO_MANY_FUNDS,
	ERR_CANNOT_OPEN_PARSER_FILE,
	ERR_CANNOT_OPEN_FUND_DATA_FILE
}ERR_ENUM_T;




class CMainSettings
{

public:
    // STORES GLOBAL SETTINGS
	// This is used when init the property page editboxes and comboboxes
	MAIN_PROP_PAGE_T mPropPgSet; 
	
	// This is used when graph plots curves
	COMMON_MAIN_SETTINGS_T mWorkSet;

	// This convert poperty page to data used by graph.cpp
	COLOR_T mColor[MAX_COLORS];
	LINE_T mLineType[MAX_LINE_TYPES];
	char mAvgTypeArr[MAX_AVG_TYPES][MAX_AVG_TYPES_LENGTH];
	short mNumbAvgTypes;
	short mNumbColor;
	short mNumbLineTypes;

	 CMainSettings();
	void readMainSettings(void);
	void updateMainSettings(MAIN_PROP_PAGE_T newMainPropPgSettings);
	void storeMainSettings(void);
	void storeMainDataFirstTime(void);
	void defaultSettingsOscillators(void);

private:
	FILE *mFd;

	
};



#endif
