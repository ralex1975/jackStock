#ifndef COMMON_H
#define COMMON_H

#include <QString>
#define PATH_JACK_STOCK_DB "jackStock.db"

#define NO_DATA_RECIVED 0x00
#define PRICE_DATA 0x01
#define YIELD_DATA 0x02
#define KEY_NUMBER_DATA 0x04

#define ERR_PRICE_DATA 0x10
#define ERR_YIELD_DATA 0x20
#define ERR_KEY_NUMBER_DATA 0x40

#define ERR_RECEIVE_DATA 0x88

#define DWLD_PATH_STOCK_PRICE_FILE "database/inputData/stockPrice1.html"
#define DWLD_PATH_STOCK_YIELD_FILE "database/inputData/yields.html"
#define DWLD_PATH_STOCK_KEY_NO_FILE "database/inputData/KeyNumberData.html"
#define DWLD_PATH_TA_LIST_FILE "database/inputData/talist.txt"
#define DWLD_PATH_TA_STOCKLIST_FILE "database/inputData/taData/stockLists.txt"
#define DWLD_PATH_TA_INPUT_DATA_PATH "/database/inputData/taData/"
#define DWLD_PATH_FA_TA_BRIDGE_DATA  "database/inputData/KeyTaBridgeData/NordnetNameYahooSymbol.csv"
#define DWLD_PATH_FA_TA_COMPANY_DESCRIPTION  "database/inputData/companyInfo/bolagsfakta.html"


// #define DWLD_PATH_ALL_STOCK_SYMBOL_LIST_PATH "database/inputData/taData/omx_large_cap.csv"

#define PATH_COMPANY_HIST_INFO "database/inputData/companyInfo/companyInfo.html"


enum DataParseSel_ET
{
    PDATESEL_NOT_INIT       = 0x0000,
    PDATASEL_SWEDEN         = 0x0001,
    PDATASEL_FINLAND        = 0x0002,
    PDATASEL_NORWAY         = 0x0004,
    PDATASEL_DENMARK        = 0x0008,
    PDATASEL_NORDIC         = 0x0010,

    PDATASEL_LARGE_CAP      = 0x0100,
    PDATASEL_MID_CAP        = 0x0200,
    PDATASEL_SMALL_CAP      = 0x0400,

    PDATASEL_INVALID_SEL    = 0xFFFF
};

struct CompanyNameAndId_ST
{
    QString assetId;
    QString AssetName;
};


struct NameKey
{
    QString    symbolName;
    bool       dataIsUpdated;
};


#if 1

// www.nordnet.se
#define HTTPS_PORT 443
#define PATH_HOST "www.nordnet.se"
#define PATH_PRICE_DATA "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Sverige&lista=1_1&large=on&mid=on&sektor=0&subtyp=price&sortera=aktie&sorteringsordning=stigande"
#define PATH_YIELD_DATA "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Sverige&lista=1_1&large=on&mid=on&sektor=0&subtyp=historic_return&sortera=&sorteringsordning="
#define PATH_KEY_NO_DATA "https://www.nordnet.se/mux/web/marknaden/kurslista/aktier.html?marknad=Sverige&lista=1_1&large=on&mid=on&sektor=0&subtyp=key_ratios&sortera=&sorteringsordning="
#endif

#endif // COMMON_H
