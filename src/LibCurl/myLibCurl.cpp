/*******************************************************************
 *
 * Filename: MyLibCurl.cpp
 *
 * Description:
 *
 *
 *******************************************************************/


//===================================================================
// Imported definitions
//===================================================================
#include "myLibCurl.h"
#include <stdio.h>
#include <string.h>

#include <QMessageBox>
#include<QString>
#include <QDebug>


//===================================================================
// Private constants
//===================================================================
#define MAX_SIZE_COOKIE_ARR 256
#define DEBUG_MODE_ON       1L
#define DEBUG_MODE_OFF      0L
#define YAHOO_DOMAIN_SIZE   10  // ".yahoo.com"

//===================================================================
// Private c-function prototypes
//===================================================================
static size_t writeDataToFile(void *ptr, size_t size, size_t nmemb, FILE *stream);



/******************************************************************
 *
 * Function:    writeDataToFile()
 *
 * Description: libCurl callback function that save data on disk
 *
 * Note:        Pure c-file
 *
 *
 *****************************************************************/
static size_t writeDataToFile(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}



/******************************************************************
 *
 * Function:    ()
 *
 * Description: Invoke this function before you use any
 *              other functions in this class;
 *
 *
 *
 *
 *****************************************************************/
CURL *MyLibCurl::beginCurlSession(void)
{
    CURL *curlHndl;

   // First step, init curl
   curl_global_init(CURL_GLOBAL_ALL);
   curlHndl = curl_easy_init();

    return curlHndl;
}


/******************************************************************
 *
 * Function:        endCurlSession()
 *
 * Description:     Invoke this function when finish using MyLibCurl
 *
 *
 *
 *
 *****************************************************************/
void MyLibCurl::endCurlSession(CURL *curlHndl)
{
    if(curlHndl != NULL)
    {
        curl_easy_cleanup(curlHndl);
    }

    curl_global_cleanup();
}


/******************************************************************
 *
 * Function:        requestYahooWebPageAndCookie(()
 *
 * Description:     Invoke this function when finish using MyLibCurl
 *
 *
 *
 *
 *****************************************************************/
bool MyLibCurl::
getYahooCookies(CURL *curlHandl, char *cookieResArr)
{
    CURLcode res;
    struct curl_slist *cookies;
    struct curl_slist *nc;
    int i;
    bool result = false;
    QString errorStr;

    cookieResArr[0] = 0;

    // Get all known cookies
    qDebug("Get all known cookies:\n");
    res = curl_easy_getinfo(curlHandl, CURLINFO_COOKIELIST, &cookies);

    // Fail to access cookie list
    if (res != CURLE_OK)
    {
        errorStr.sprintf("Curl curl_easy_getinfo failed: %s\n", curl_easy_strerror(res));
        QMessageBox::information(NULL, QString::fromUtf8("Error"), errorStr);
        exit(1);
    }

    nc = cookies;
    i = 1;
    while (nc)
    {
        if((strlen(nc->data) > 0) && (strlen(nc->data) < MAX_SIZE_COOKIE_ARR))
        {
            if(strncmp(".yahoo.com", nc->data, 10) == 0)
            {
                strcpy(cookieResArr, nc->data);
                result = true;
                break;
            }
        }

        qDebug() << "[" << i << "]" << nc->data;
        nc = nc->next;
        i++;
    }

    return result;
}


/******************************************************************
 *
 * Function:        requestYahooWebPageAndCookie(()
 *
 * Description:     Invoke this function when finish using MyLibCurl
 *
 *
 *
 *
 *****************************************************************/
bool MyLibCurl::
clearCookieList(CURL *curlHandl)
{
    CURLcode res;
    struct curl_slist *cookies;
    QString errorStr;

    res = curl_easy_getinfo(curlHandl, CURLINFO_COOKIELIST, &cookies);

    // Fail to access cookie list
    if (res != CURLE_OK)
    {
        errorStr.sprintf("Curl curl_easy_getinfo failed: %s\n", curl_easy_strerror(res));
        QMessageBox::information(NULL, QString::fromUtf8("Error"), errorStr);
        exit(1);
    }

    // Clear cookie list
    curl_slist_free_all(cookies);
    return true;
}


/******************************************************************
 *
 * Function:    addYahooCookie()
 *
 * Description: This function add a cookie in Yahoo finance format.
 *
 *****************************************************************/
void MyLibCurl::addYahooCookie(CURL *curlHandl,
                               char *hostname,            // ".yahoo.com",
                               char *incSubdomains,       // "TRUE"
                               char *path,                // "/",
                               char *secure,              // "FALSE"
                               char *expirationDate,      // "1527424259" (Linux time: Sun, 27 May 2018 12:30:59 GMT)
                               char *name,                // "B"
                               char *value,               // "95jdfnpci12gs&b=3&s=to",
                               char *cookieArr)
{
    sprintf(cookieArr, "%s\t"  // Hostname
                       "%s\t"  // incSubdomains
                       "%s\t"  // Path
                       "%s\t"  // Secure
                       "%s\t"  // Expiration date
                       "%s\t"  // Name
                       "%s",   // Value
                        hostname,
                        incSubdomains,
                        path,
                        secure,
                        expirationDate,
                        name,
                        value);

    // cookieArr is imported immediately via CURLOPT_COOKIELIST.
    curl_easy_setopt(curlHandl, CURLOPT_COOKIELIST, cookieArr);

}




/******************************************************************
 *
 * Function:        requestYahooWebPageAndCookie(()
 *
 * Description:     Invoke this function when finish using MyLibCurl
 *
 *
 *
 *
 *****************************************************************/
bool MyLibCurl::
requestYahooWebPageAndCookie(CURL *curlHndl,
                             char *url,         // "https://finance.yahoo.com/quote/ABB?p=ABB"
                             char *filename,
                             char *cookieResArr)
{
    CURLcode res;
    QString errorStr;
    FILE *fp;
    bool result = false;


    if (curlHndl != NULL)
    {
        // Open file
        fp = fopen(filename,"wb");

        if (!fp)
        {
            QMessageBox::information(NULL, QString::fromUtf8("Error"), QString::fromUtf8("Fail topen curlfile"));
            return 1;
        }


       // Add requested http address
       curl_easy_setopt(curlHndl, CURLOPT_URL, url);

       // Turn on debug mode
       curl_easy_setopt(curlHndl, CURLOPT_VERBOSE, DEBUG_MODE_ON);

       // Start the cookie engine
       curl_easy_setopt(curlHndl, CURLOPT_COOKIEFILE, "");
       // curl_easy_setopt(curlHndl, CURLOPT_COOKIEFILE, "/home/ajn/Documents/OldPC/swProj/MyQtProj/JackStockProj/JackStock/cookie.txt");

       clearCookieList(curlHndl);

       #if 1
       //--------------------------------------------------------
       // DEbug
       //--------------------------------------------------------
       //char cookieResArr[256];

       char hostname[80];
       char incSubdomains[80];       // "TRUE"
       char path[80];                // "/",
       char secure[80];              // "FALSE"
       char expirationDate[80];      // "1527424259" (Linux time: Sun, 27 May 2018 12:30:59 GMT)
       char name[80];                // "B"
       char value[80];               // "5ljb2u1ciiiki&b=3&s=m8",
       char cookieArr1[80];
       char cookieArr2[80];


       strcpy(hostname, ".yahoo.com");
       strcpy(incSubdomains, "TRUE");
       strcpy(path, "/");
       strcpy(secure, "FALSE");
       strcpy(expirationDate, "1527424259");
       strcpy(name, "B");
       strcpy(value, "5ljb2u1ciiiki&b=3&s=m8");


       addYahooCookie(curlHndl,
                      hostname,            // ".yahoo.com",
                      incSubdomains,       // "TRUE"
                      path,                // "/",
                      secure,              // "FALSE"
                      expirationDate,      // "1527424259" (Linux time: Sun, 27 May 2018 12:30:59 GMT)
                      name,                // "B"
                      value,               // "95jdfnpci12gs&b=3&s=to",
                      cookieArr1);

        QString Str1;

        Str1.sprintf("cookieArr1: %s\n", cookieArr1);
        qDebug() <<  Str1;
#endif
        getYahooCookies(curlHndl, cookieArr2);
        Str1.sprintf("cookieArr2: %s\n", cookieArr2);
        qDebug() <<  Str1;
        // .yahoo.com	TRUE	/	FALSE	1527296628	B	1bakt8lc5lpd3&b=3&s=nn

        Str1.sprintf("url: %s\n", url);
        qDebug()  << Str1;
       //----------------------------------------------------------

       // Init callback function that will handle received data (write to disk)
       curl_easy_setopt(curlHndl, CURLOPT_WRITEFUNCTION, writeDataToFile);

       // A data pointer to pass to the write callback function above.
       // When using CURLOPT_WRITEFUNCTION, this fp will be used
       // as the 4th argument in that callback function.
       curl_easy_setopt(curlHndl, CURLOPT_WRITEDATA, fp);

       // Invoke the request
       res = curl_easy_perform(curlHndl);

       // Close file
       fclose(fp);

       if (res != CURLE_OK)
       {
           errorStr.sprintf("Curl perform failed: %s\n", curl_easy_strerror(res));
           QMessageBox::information(NULL, QString::fromUtf8("Error"), errorStr);
           return result;
       }

       result = getYahooCookies(curlHndl, cookieResArr);
       if(result == true)
       {
           qDebug() << cookieResArr;
       }
       return result;
   }

   return result;

}


// ".yahoo.com	TRUE	/	FALSE	1527437638	B	btn35mhcij9e6&b=3&s=e6"


#if 0
/* This example shows an inline import of a cookie in Netscape format.
You can set the cookie as HttpOnly to prevent XSS attacks by prepending
#HttpOnly_ to the hostname. That may be useful if the cookie will later
be imported by a browser.
*/
#define TAB_SEPARATOR  "\t"  // Tab separates the fields
#define SEP  "\t"  /* Tab separates the fields */

char *my_cookie =
  "example.com"    /* Hostname */
  SEP "FALSE"      /* Include subdomains */
  SEP "/"          /* Path */
  SEP "FALSE"      /* Secure */
  SEP "0"          /* Expiry in epoch time format. 0 == Session */
  SEP "foo"        /* Name */
  SEP "bar";       /* Value */

/* my_cookie is imported immediately via CURLOPT_COOKIELIST.
*/
curl_easy_setopt(curl, CURLOPT_COOKIELIST, my_cookie);

/* The list of cookies in cookies.txt will not be imported until right
before a transfer is performed. Cookies in the list that have the same
hostname, path and name as in my_cookie are skipped. That is because
libcurl has already imported my_cookie and it's considered a "live"
cookie. A live cookie won't be replaced by one read from a file.
*/
curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookies.txt");  /* import */

/* Cookies are exported after curl_easy_cleanup is called. The server
may have added, deleted or modified cookies by then. The cookies that
were skipped on import are not exported.
*/
curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "cookies.txt");  /* export */

curl_easy_perform(curl);  /* cookies imported from cookies.txt */

curl_easy_cleanup(curl);  /* cookies exported to cookies.txt */


#endif



#if 0
/******************************************************************
 *
 * Function:    addYahooCookie()
 *
 * Description: This function add a cookie in Yahoo finance format.
 *
 *****************************************************************/
void MyLibCurl::addYahooCookie(char *hostname,            // ".yahoo.com",
                               char *incSubdomains,       // "TRUE"
                               char *path,                // "/",
                               char *secure,              // "FALSE"
                               char *expirationDate,      // "1527424259" (Linux time: Sun, 27 May 2018 12:30:59 GMT)
                               char *name,                // "B"
                               char *value,               // "95jdfnpci12gs&b=3&s=to",
                               char *cookieArr)
{
    sprintf(cookieArr, "%s\t"  // Hostname
                       "%s\t"  // incSubdomains
                       "%s\t"  // Path
                       "%s\t"  // Secure
                       "%s\t"  // Expiration date
                       "%s\t"  // Name
                       "%s",   // Value
                        hostname,
                        incSubdomains,
                        path,
                        secure,
                        expirationDate,
                        name,
                        value);

    // cookieArr is imported immediately via CURLOPT_COOKIELIST.
    curl_easy_setopt(curl, CURLOPT_COOKIELIST, cookieArr);

}


#if 0
CURL *curl;

   /* First step, init curl */
   curl_global_init(CURL_GLOBAL_ALL);
   curl = curl_easy_init();

       FILE *fp;
       CURLcode res;
       char *url = "https://finance.yahoo.com/quote/ABB?p=ABB";
       char outfilename[FILENAME_MAX] = "abb.txt";
       curl = curl_easy_init();
       if (curl)
       {
           fp = fopen(outfilename,"wb");
           curl_easy_setopt(curl, CURLOPT_URL, url);
           curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
           curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
           res = curl_easy_perform(curl);
           /* always cleanup */
           curl_easy_cleanup(curl);
           fclose(fp);
        }

#endif
#endif
