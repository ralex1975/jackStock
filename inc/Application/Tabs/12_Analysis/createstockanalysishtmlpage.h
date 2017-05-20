#ifndef CREATESTOCKANALYSISHTMLPAGE_H
#define CREATESTOCKANALYSISHTMLPAGE_H

 #include <QtCore/QString>
#include <dbHndl.h>





class createStockAnalysisHtmlPage
{



public:
    createStockAnalysisHtmlPage();
    void createHtmlPage(struct HtmlStockAnalysPageDataST &hSAPData);

};

#endif // CREATESTOCKANALYSISHTMLPAGE_H
