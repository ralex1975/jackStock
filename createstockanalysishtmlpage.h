#ifndef CREATESTOCKANALYSISHTMLPAGE_H
#define CREATESTOCKANALYSISHTMLPAGE_H

 #include <QtCore/QString>


class createStockAnalysisHtmlPage
{

public:
    createStockAnalysisHtmlPage();
    void createHtmlPage(QString &m_htmlStr,
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
                                 QString &otherInformation);

};

#endif // CREATESTOCKANALYSISHTMLPAGE_H
