#include "createstockanalysishtmlpage.h"


createStockAnalysisHtmlPage::createStockAnalysisHtmlPage()
{

}



void createStockAnalysisHtmlPage::createHtmlPage(QString &m_htmlStr,
                                                 QString stockName,
                                                 QString stockSymbol,
                                                 QString analysisDate,
                                                 QString &companyDescription,
                                                 QString &bigEnoughAnswer,
                                                 QString &bigEnoughComment,
                                                 QString &strongFinancialPositionAnswer,
                                                 QString &strongFinancialPositionComment,
                                                 QString &earningStabilityAnswer,
                                                 QString &earningStabilityComment,
                                                 QString &dividendStabilityAnswer,
                                                 QString &dividendStabilityComment,
                                                 QString &earningGrowthAnswer,
                                                 QString &earningGrowthComment,
                                                 QString &keyValuePe,
                                                 QString &keyValuePs,
                                                 QString &keyValueNavPriceRatio,
                                                 QString &keyValueYield,
                                                 QString &keyValuePriceJEKRatio,
                                                 QString &keyValueErningsDividentRatio,
                                                 QString &keyValueTotalDebtEquityRatio,
                                                 QString &keyValueCurrentRatio,
                                                 QString &trustworthyLeadershipAnswer,
                                                 QString &trustworthyLeadershipComment,
                                                 QString &goodOwnershipAnswer,
                                                 QString &goodOwnershipComment,
                                                 QString &otherInformation)
{

    QString tmpStr;



     m_htmlStr = QString::fromUtf8("<!DOCTYPE html>\n");
     m_htmlStr += QString::fromUtf8("<html>\n");

     // Add html page head
     m_htmlStr += QString::fromUtf8("<head>\n");
     m_htmlStr += QString::fromUtf8("<meta content=\"text/html; charset=UTF-8\" http-equiv=\"content-type\">\n");

     tmpStr = stockName;
     tmpStr += ", ";
     tmpStr += stockSymbol;
     tmpStr += ", ";
     tmpStr += analysisDate;

     // Add html page title
     m_htmlStr += QString::fromUtf8("<title>");
     m_htmlStr += QString::fromUtf8(tmpStr.toLocal8Bit().constData());
     m_htmlStr += QString::fromUtf8("</title>");

     m_htmlStr += QString::fromUtf8("</head>");

     // Add html page body
     m_htmlStr += QString::fromUtf8("<body marginheight=\"20\" marginwidth=\"50\">");

     // Add page title
     tmpStr = stockName;
     m_htmlStr += QString::fromUtf8("<p style=\"text-align: center;\"><b>");
     m_htmlStr += tmpStr;
     m_htmlStr += QString::fromUtf8("</b></p>\n");

     // Add html table
     m_htmlStr += QString::fromUtf8("<table style=\"width: 100%;\" border=\"0\">\n");
     m_htmlStr += QString::fromUtf8("<tbody>\n");

     // New line
     m_htmlStr += QString::fromUtf8("<tr>");
     m_htmlStr += QString::fromUtf8("<td colspan=\"3\" rowspan=\"1\">&nbsp;</td>\n");
     m_htmlStr += QString::fromUtf8("</tr>");

     // New line
     m_htmlStr += QString::fromUtf8("<tr>\n");
     m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\"><b>Företagsbeskrivning");
     m_htmlStr += QString::fromUtf8("</b></td>\n");
     m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\"><br></td>\n");
     m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\">\n");
     m_htmlStr += analysisDate;
     m_htmlStr += QString::fromUtf8("</td>\n");
     m_htmlStr += QString::fromUtf8("</tr>\n");

     // New line
     m_htmlStr += QString::fromUtf8("<tr>\n");
     m_htmlStr += QString::fromUtf8("<td><br></td>\n");
     m_htmlStr += QString::fromUtf8("<td><br></td>\n");
     m_htmlStr += QString::fromUtf8("<td><br></td>\n");
     m_htmlStr += QString::fromUtf8("</tr>\n");

    // New line
    m_htmlStr += QString::fromUtf8("<tr>\n");
    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\" colspan=\"3\" rowspan=\"1\">\n");
    m_htmlStr += QString::fromUtf8(companyDescription.toLocal8Bit().constData());
    m_htmlStr += QString::fromUtf8("<br>\n");
    m_htmlStr += QString::fromUtf8("</td>\n");
    m_htmlStr += QString::fromUtf8("</tr>\n");

    // New line
    m_htmlStr += QString::fromUtf8("<tr>\n");
    m_htmlStr += QString::fromUtf8("<td><br></td>\n");
    m_htmlStr += QString::fromUtf8("<td><br></td>\n");
    m_htmlStr += QString::fromUtf8("<td><br></td>\n");
    m_htmlStr += QString::fromUtf8("</tr>\n");

    // New line
    m_htmlStr += QString::fromUtf8("<tr>\n");
    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\"><b>Kriterium</b></td>\n");
    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\"><b>Uppfyllt?</b></td>\n");
    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\"><b>Kommentar&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<br></b></td>\n");
    m_htmlStr += QString::fromUtf8("</tr>\n");

    // New line Big Enough
    m_htmlStr += QString::fromUtf8("<tr>\n");
    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\"><b>Tillräcklig storlek:</b></td>\n");

    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\">\n");
    m_htmlStr += bigEnoughAnswer;
    m_htmlStr += QString::fromUtf8("</td>\n");

    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\">\n");
    m_htmlStr += QString::fromUtf8(bigEnoughComment.toLocal8Bit().constData());
    m_htmlStr += QString::fromUtf8("<br></td>\n");
    m_htmlStr += QString::fromUtf8("</tr>\n");



    // New line Strong Financial Position
    m_htmlStr += QString::fromUtf8("<tr>\n");
    m_htmlStr += QString::fromUtf8("<td style=\"width: 600.133px;\"><b>Stark finansiell ställning:</b></td>\n");

    m_htmlStr += QString::fromUtf8("<td style=\"width: 214.733px;\">\n");
    m_htmlStr += strongFinancialPositionAnswer;
    m_htmlStr += QString::fromUtf8("</td>\n");

    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\">\n");
    m_htmlStr += QString::fromUtf8(strongFinancialPositionComment.toLocal8Bit().constData());

    m_htmlStr += QString::fromUtf8("<br></td>\n");
    m_htmlStr += QString::fromUtf8("</tr>\n");


    // New line Earning Stability
    m_htmlStr += QString::fromUtf8("<tr>\n");
    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\"><b>Intjäningsstabilitet:</b></td>\n");

    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\">\n");
    m_htmlStr += earningStabilityAnswer;

    m_htmlStr += QString::fromUtf8("</td>\n");

    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\">\n");
    m_htmlStr += QString::fromUtf8(earningStabilityComment.toLocal8Bit().constData());

    m_htmlStr += QString::fromUtf8("<br></td>\n");
    m_htmlStr += QString::fromUtf8("</tr>\n");



    // New line dividend Stability
    m_htmlStr += QString::fromUtf8("<tr>\n");
    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\"><b>Utdelningsstabilitet:</b></td>\n");
    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\">\n");
    m_htmlStr += dividendStabilityAnswer;
    m_htmlStr += QString::fromUtf8("</td>\n");

    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\">\n");
    m_htmlStr += QString::fromUtf8(dividendStabilityComment.toLocal8Bit().constData());

    m_htmlStr += QString::fromUtf8("</td>\n");
    m_htmlStr += QString::fromUtf8("</tr>\n");


    // New line Earning Growth
    m_htmlStr += QString::fromUtf8("<tr>\n");
    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\"><b>Intjäningstillväxt:</b></td>\n");
    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\">\n");
    m_htmlStr += earningGrowthAnswer;
    m_htmlStr += QString::fromUtf8("</td>\n");

    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\">\n");
    m_htmlStr += earningGrowthComment;
    m_htmlStr += QString::fromUtf8(earningGrowthComment.toLocal8Bit().constData());

    m_htmlStr += QString::fromUtf8("</td>\n");
    m_htmlStr += QString::fromUtf8("</tr>\n");



    // New line Trustworthy Leadership
    m_htmlStr += QString::fromUtf8("<tr>\n");
    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\"><b>Trovärdig ledning:</b></td>\n");
    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\">\n");
    m_htmlStr += trustworthyLeadershipAnswer;
    m_htmlStr += QString::fromUtf8("</td>\n");

    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\">\n");
    m_htmlStr += QString::fromUtf8(trustworthyLeadershipComment.toLocal8Bit().constData());

    m_htmlStr += QString::fromUtf8("</td>\n");
    m_htmlStr += QString::fromUtf8("</tr>\n");


    // New line Good Ownership
    m_htmlStr += QString::fromUtf8("<tr>\n");
    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\"><b>Gynnsam ägarbild:</b></td>\n");
    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\">\n");
    m_htmlStr += goodOwnershipAnswer;
    m_htmlStr += QString::fromUtf8("</td>\n");

    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\">\n");
    m_htmlStr += QString::fromUtf8(goodOwnershipComment.toLocal8Bit().constData());

    m_htmlStr += QString::fromUtf8("</td>\n");
    m_htmlStr += QString::fromUtf8("</tr>\n");

    // New line
    m_htmlStr += QString::fromUtf8("<tr>\n");
    m_htmlStr += QString::fromUtf8("<td><br></td>\n");
    m_htmlStr += QString::fromUtf8("<td><br></td>\n");
    m_htmlStr += QString::fromUtf8("<td><br></td>\n");
    m_htmlStr += QString::fromUtf8("</tr>\n");

    // New line Other Information
    m_htmlStr += QString::fromUtf8("<tr>\n");
    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\"><br>Övrigt:</td>\n");
    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\"><br></td>\n");
    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\"><br>\n");
    m_htmlStr += QString::fromUtf8(otherInformation.toLocal8Bit().constData());

    m_htmlStr += QString::fromUtf8("</td>\n");

    m_htmlStr += QString::fromUtf8("</tr>\n");
    m_htmlStr += QString::fromUtf8("</tbody>\n");
    m_htmlStr += QString::fromUtf8("</table>\n");

    // Add html table
    m_htmlStr += QString::fromUtf8("<table style=\"width: 100%;\" border=\"0\">\n");
    m_htmlStr += QString::fromUtf8("<tbody>\n");

    // New line
    m_htmlStr += QString::fromUtf8("<tr>\n");
    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\"><b>PE</b></td>\n");
    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\"><b>DirAvk</b></td>\n");
    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\"><b>Avkastning eget kapital</b></td>\n");
    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\"><b>Skuld/Eget kap</b></td>\n");
    m_htmlStr += QString::fromUtf8("<td colspan=\"1\" rowspan=\"1\" style=\"height: 35px;\"><b>Förväntad risk</b></td>\n");
    m_htmlStr += QString::fromUtf8("</tr>\n");



    // New line Key Values
    m_htmlStr += QString::fromUtf8("<tr>\n");
    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\">\n");
    m_htmlStr += keyValuePe;
    m_htmlStr += QString::fromUtf8("</td>\n");

    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\">\n");
    m_htmlStr += keyValueYield;
    m_htmlStr += QString::fromUtf8("</td>\n");

    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\">\n");
    m_htmlStr += keyValuePriceJEKRatio;
    m_htmlStr += QString::fromUtf8("</td>\n");

    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\">RISK\n");
    //m_htmlStr += risk fixa detta
    m_htmlStr += QString::fromUtf8("</td>\n");

    m_htmlStr += QString::fromUtf8("<td colspan=\"1\" rowspan=\"1\" style=\"height: 35px;\">\n");
    m_htmlStr += keyValueTotalDebtEquityRatio;
    m_htmlStr += QString::fromUtf8("</td>\n");
    m_htmlStr += QString::fromUtf8("</tr>\n");

    // New line
    m_htmlStr += QString::fromUtf8("<tr>\n");
    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\"><b>PS</b></td>\n");
    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\"><b>Vinst/Utdelning</b></td>\n");
    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\"><b>Sub/Kurs</b></td>\n");
    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\"><b>Balanslikviditet</b></td>\n");
    m_htmlStr += QString::fromUtf8("<td colspan=\"1\" rowspan=\"1\" style=\"height: 35px;\"><b>Förväntad avkasning</b></td>\n");
    m_htmlStr += QString::fromUtf8("</tr>\n");



    // New line
    m_htmlStr += QString::fromUtf8("<tr>\n");
    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\">\n");
    m_htmlStr += keyValuePs;
    m_htmlStr += QString::fromUtf8("</td>\n");
    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\">\n");
    m_htmlStr += keyValueErningsDividentRatio;
    m_htmlStr += QString::fromUtf8("</td>\n");
    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\">\n");
    m_htmlStr += keyValueNavPriceRatio;
    m_htmlStr += QString::fromUtf8("</td>\n");
    m_htmlStr += QString::fromUtf8("<td style=\"height: 35px;\">\n");
    m_htmlStr += keyValueCurrentRatio;
    m_htmlStr += QString::fromUtf8("</td>\n");
    m_htmlStr += QString::fromUtf8("<td colspan=\"1\" rowspan=\"1\" style=\"height: 35px;\">VÄRDE10</td>\n");
    m_htmlStr += QString::fromUtf8("</tr>\n");

    m_htmlStr += QString::fromUtf8("</tbody>\n");
    m_htmlStr += QString::fromUtf8("</table>\n");
    m_htmlStr += QString::fromUtf8("</body>\n");
    m_htmlStr += QString::fromUtf8("</html>\n");

}
