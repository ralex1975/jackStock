/*******************************************************************
 *
 * Filename: MyLibCurl.cpp
 *
 * Description:
 *
 *
 *******************************************************************/

#ifndef MY_LIB_CURL_H
#define MY_LIB_CURL_H

#include <curl/curl.h>
#include <QString>

#define C_SEPARATOR      "\t"  // Tab separates the fields
#define C_FALSE         "FALSE"
#define C_TRUE          "TRUE"


//=====================================================
// class MyLibCurl
//=====================================================
class MyLibCurl
{
public:
    MyLibCurl(){}
    ~MyLibCurl(){}
    static CURL *beginCurlSession(void);
    static void endCurlSession(CURL *curlHndl);
    static bool getCookieExpirationTime(QString cookieArr, QString &expirationTime);
    static bool getYahooCookies(CURL *curlHandl, char *cookieResArr);
    static bool clearCookieList(CURL *curlHandl);

    static void addYahooCookie(CURL *curlHandl,
                          char *hostname,            // ".yahoo.com",
                          char *incSubdomains,       // "TRUE"
                          char *path,                // "/",
                          char *secure,              // "FALSE"
                          char *expirationDate,      // "1527424259" (Linux time: Sun, 27 May 2018 12:30:59 GMT)
                          char *name,                // "B"
                          char *value,               // "95jdfnpci12gs&b=3&s=to",
                          char *cookieArr);

    void createYahooCookieUrl(QString stockSymbol,
                              QString &outUrl);

    static bool requestYahooCrumbWebPageAndCookie(CURL *curlHndl,
                                                  char *url,
                                                  char *filename,
                                                  char *cookieResArr);



    bool requestYahooWebPage(CURL *curlHndl,
                                 char *url,         // "https://finance.yahoo.com/quote/ABB?p=ABB"
                                 char *filename,
                                 char *cookieResArr);

    bool requestYahooWebPageAndCookie(CURL *curlHndl,
                                 char *url,         // "https://finance.yahoo.com/quote/ABB?p=ABB"
                                 char *filename,
                                 char *cookieResArr);


};



#endif
