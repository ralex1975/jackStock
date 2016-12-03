#include "createstockanalysishtmlpage.h"
#include <math.h>



createStockAnalysisHtmlPage::createStockAnalysisHtmlPage()
{

}



void createStockAnalysisHtmlPage::createHtmlPage(struct HtmlStockAnalysPageDataST &hSAPData)
{

    QString tmpStr;
    CDbHndl db;
    int mainAnalysisId;
    int companyType = 0;

    if(true == db.mainAnalysisDataExists(hSAPData.stockName, hSAPData.stockSymbol, mainAnalysisId))
    {
        if(true ==  db.getSubAnalysisCompanyType(mainAnalysisId, companyType))
        {
            if(companyType >= NOF_COMPANY_TYPE_ARR_DATA)
            {
                QMessageBox::critical(NULL, QString::fromUtf8("Error"), QString::fromUtf8("Error companyType >= NOF_COMPANY_TYPE_ARR_DATA"));
                companyType = 0;
            }
        }
    }


     hSAPData.htmlStr = QString::fromUtf8("<!DOCTYPE html>\n");
     hSAPData.htmlStr += QString::fromUtf8("<html>\n");

     // Add html page head
     hSAPData.htmlStr += QString::fromUtf8("<head>\n");
     hSAPData.htmlStr += QString::fromUtf8("<meta content=\"text/html; charset=UTF-8\" http-equiv=\"content-type\">\n");

     tmpStr = hSAPData.stockName;
     tmpStr += ", ";
     tmpStr += hSAPData.stockSymbol;
     tmpStr += ", ";
     tmpStr += hSAPData.analysisDate;

     // Add html page title
     hSAPData.htmlStr += QString::fromUtf8("<title>");
     hSAPData.htmlStr += QString::fromUtf8(tmpStr.toLocal8Bit().constData());
     hSAPData.htmlStr += QString::fromUtf8("</title>");

     hSAPData.htmlStr += QString::fromUtf8("</head>");

     // Add html page body
     hSAPData.htmlStr += QString::fromUtf8("<body marginheight=\"20\" marginwidth=\"50\">");

     // Add page title
     tmpStr = hSAPData.stockName;
     hSAPData.htmlStr += QString::fromUtf8("<p style=\"text-align: center;\"><font size=\"6\"><b>");
     hSAPData.htmlStr += tmpStr;
     hSAPData.htmlStr += QString::fromUtf8("</b></font></p>\n");

     // Add html table
     hSAPData.htmlStr += QString::fromUtf8("<table style=\"width: 100%;\" border=\"0\">\n");
     hSAPData.htmlStr += QString::fromUtf8("<tbody>\n");

     // New line
     hSAPData.htmlStr += QString::fromUtf8("<tr>");
     hSAPData.htmlStr += QString::fromUtf8("<td colspan=\"3\" rowspan=\"1\">&nbsp;</td>\n");
     hSAPData.htmlStr += QString::fromUtf8("</tr>");

     // New line
     hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
     hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\"><b>Företagsbeskrivning");
     hSAPData.htmlStr += QString::fromUtf8("</b></td>\n");
     hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px;text-align: left; vertical-align: top;\"><br></td>\n");
     hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px;text-align: left; vertical-align: top;\">\n");
     hSAPData.htmlStr += hSAPData.analysisDate;
     hSAPData.htmlStr += QString::fromUtf8("</td>\n");
     hSAPData.htmlStr += QString::fromUtf8("</tr>\n");

     // New line
     hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
     hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
     hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
     hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
     hSAPData.htmlStr += QString::fromUtf8("</tr>\n");

    // New line
    hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\" colspan=\"3\" rowspan=\"1\">\n");
    hSAPData.htmlStr += QString::fromUtf8(hSAPData.companyDescription.toLocal8Bit().constData());
    hSAPData.htmlStr += QString::fromUtf8("<br><br>\n");
    hSAPData.htmlStr += QString::fromUtf8("</td>\n");
    hSAPData.htmlStr += QString::fromUtf8("</tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<tr><td colspan=\"3\" rowspan=\"1\"> <hr></td></tr>\n");

    // New line
    hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("</tr>\n");

    // New line
    hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\"><b>Kriterium</b></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\"><b>Uppfyllt?</b></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\"><b>Kommentar&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<br></b></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("</tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<tr><td colspan=\"3\" rowspan=\"1\"> <hr></td></tr>\n");

    // New line Big Enough
    hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\"><b>Tillräcklig storlek:</b></td>\n");

    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px;text-align: left; vertical-align: top;\">\n");
    hSAPData.htmlStr += hSAPData.bigEnoughAnswer;
    hSAPData.htmlStr += QString::fromUtf8("</td>\n");

    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\">\n");
    hSAPData.htmlStr += QString::fromUtf8(hSAPData.bigEnoughComment.toLocal8Bit().constData());
    hSAPData.htmlStr += QString::fromUtf8("<br><br></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("</tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<tr><td colspan=\"3\" rowspan=\"1\"> <hr></td></tr>\n");



    // New line Strong Financial Position
    hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"width: 600.133px; text-align: left; vertical-align: top;\"><b>Stark finansiell ställning:</b></td>\n");

    hSAPData.htmlStr += QString::fromUtf8("<td style=\"width: 214.733px; text-align: left; vertical-align: top;\">\n");
    hSAPData.htmlStr += hSAPData.strongFinancialPositionAnswer;
    hSAPData.htmlStr += QString::fromUtf8("</td>\n");

    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\">\n");
    hSAPData.htmlStr += QString::fromUtf8(hSAPData.strongFinancialPositionComment.toLocal8Bit().constData());

    hSAPData.htmlStr += QString::fromUtf8("<br><br></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("</tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<tr><td colspan=\"3\" rowspan=\"1\"> <hr></td></tr>\n");


    // New line Earning Stability
    hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\"><b>Intjäningsstabilitet:</b></td>\n");

    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\">\n");
    hSAPData.htmlStr += hSAPData.earningStabilityAnswer;

    hSAPData.htmlStr += QString::fromUtf8("</td>\n");

    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\">\n");
    hSAPData.htmlStr += QString::fromUtf8(hSAPData.earningStabilityComment.toLocal8Bit().constData());

    hSAPData.htmlStr += QString::fromUtf8("<br><br></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("</tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<tr><td colspan=\"3\" rowspan=\"1\"> <hr></td></tr>\n");


    // New line dividend Stability
    hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\"><b>Utdelningsstabilitet:</b></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\">\n");
    hSAPData.htmlStr += hSAPData.dividendStabilityAnswer;
    hSAPData.htmlStr += QString::fromUtf8("</td>\n");

    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\">\n");
    hSAPData.htmlStr += QString::fromUtf8(hSAPData.dividendStabilityComment.toLocal8Bit().constData());

    hSAPData.htmlStr += QString::fromUtf8("<br><br></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("</tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<tr><td colspan=\"3\" rowspan=\"1\"> <hr></td></tr>\n");


    // New line Earning Growth
    hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\"><b>Intjäningstillväxt:</b></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\">\n");
    hSAPData.htmlStr += hSAPData.earningGrowthAnswer;
    hSAPData.htmlStr += QString::fromUtf8("</td>\n");

    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\">\n");
    hSAPData.htmlStr += QString::fromUtf8(hSAPData.earningGrowthComment.toLocal8Bit().constData());

    hSAPData.htmlStr += QString::fromUtf8("<br><br></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("</tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<tr><td colspan=\"3\" rowspan=\"1\"> <hr></td></tr>\n");


    // New line Trustworthy Leadership
    hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\"><b>Trovärdig ledning:</b></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\">\n");
    hSAPData.htmlStr += hSAPData.trustworthyLeadershipAnswer;
    hSAPData.htmlStr += QString::fromUtf8("</td>\n");

    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\">\n");
    hSAPData.htmlStr += QString::fromUtf8(hSAPData.trustworthyLeadershipComment.toLocal8Bit().constData());

    hSAPData.htmlStr += QString::fromUtf8("<br><br></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("</tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<tr><td colspan=\"3\" rowspan=\"1\"> <hr></td></tr>\n");


    // New line Good Ownership
    hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\"><b>Gynnsam ägarbild:</b></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\">\n");
    hSAPData.htmlStr += hSAPData.goodOwnershipAnswer;
    hSAPData.htmlStr += QString::fromUtf8("</td>\n");

    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\">\n");
    hSAPData.htmlStr += QString::fromUtf8(hSAPData.goodOwnershipComment.toLocal8Bit().constData());

    hSAPData.htmlStr += QString::fromUtf8("<br><br></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("</tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<tr><td colspan=\"3\" rowspan=\"1\"> <hr></td></tr>\n");

    // New line
    hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("</tr>\n");


    // Start
    hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("</tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td colspan=\"3\" rowspan=\"1\"><b>Finansdata:</b></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("</tr>\n");


    if((companyType == UNKNOW_COMPANY_TYPE) || (companyType == INDUSTRIALS_COMPANY) || (companyType == TRADING_COMPANY))
    {
        hSAPData.htmlStr += QString::fromUtf8("<tr><td><br>\n");
        hSAPData.htmlStr += QString::fromUtf8("</td>\n");
        hSAPData.htmlStr += QString::fromUtf8(" <td colspan=\"2\" rowspan=\"1\">\n");
        hSAPData.htmlStr += QString::fromUtf8("<table style=\"width: 100%\" border=\"1\">\n");
        hSAPData.htmlStr += QString::fromUtf8("<tbody>\n");
        hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">&nbsp;&nbsp;&nbsp;&nbsp;År &nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </td>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">Omsättningstillgångarna</td>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">Kortfristiga skulder</td>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">OmsTlg/KS >= 2</td>\n");
        hSAPData.htmlStr += QString::fromUtf8("</tr>\n");


        if(hSAPData.nofTotalCurrentAssetsArrData == hSAPData.nofTotalCurrentLiabilitiesData)
        {

            for(int ii = 0; ii < hSAPData.nofTotalCurrentAssetsArrData; ii++)
            {
                if(hSAPData.totalCurrentAssetsArr[ii].date.toInt() == hSAPData.totalCurrentLiabilitiesArr[ii].date.toInt())
                {
                    hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
                    hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">\n");
                    hSAPData.htmlStr += hSAPData.totalCurrentAssetsArr[ii].date;
                    hSAPData.htmlStr += QString::fromUtf8("</td>\n");
                    hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">\n");
                    hSAPData.htmlStr += hSAPData.totalCurrentAssetsArr[ii].data;
                    hSAPData.htmlStr += QString::fromUtf8("</td>\n");
                    hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">\n");
                    hSAPData.htmlStr += hSAPData.totalCurrentLiabilitiesArr[ii].data;
                    hSAPData.htmlStr += QString::fromUtf8("</td>\n");


                    double tmpRes;
                    if(hSAPData.totalCurrentLiabilitiesArr[ii].data.toDouble() != 0)
                    {
                        tmpRes = (hSAPData.totalCurrentAssetsArr[ii].data.toDouble() - hSAPData.totalCurrentLiabilitiesArr[ii].data.toDouble())/hSAPData.totalCurrentLiabilitiesArr[ii].data.toDouble();
                    }
                    else
                    {
                        tmpRes = hSAPData.totalCurrentAssetsArr[ii].data.toDouble() / 0.001;
                    }

                    tmpRes = 1.0 + tmpRes;


                    if(tmpRes >= 2)
                    {
                        hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\"> <font color=\"blue\">\n");
                    }
                    else
                    {
                        hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\"> <font color=\"red\">\n");
                    }

                    QString tmpDb;
                    tmpDb.sprintf("%.2f", tmpRes);
                    hSAPData.htmlStr += tmpDb;
                    hSAPData.htmlStr += QString::fromUtf8("</font></td>\n");
                    hSAPData.htmlStr += QString::fromUtf8("</tr>\n");
                }
            }
        }
        else
        {
            QMessageBox::information(NULL, QString::fromUtf8("Error:"), QString::fromUtf8("nofTotalCurrentAssetsArrData != nofTotalCurrentLiabilitiesData"));
        }

        hSAPData.htmlStr += QString::fromUtf8("</tbody>\n");
        hSAPData.htmlStr += QString::fromUtf8("</table>\n");
        hSAPData.htmlStr += QString::fromUtf8("<br>\n");
        hSAPData.htmlStr += QString::fromUtf8("</td>\n");

        hSAPData.htmlStr += QString::fromUtf8("</tr>\n");


        hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td><br>\n");
        hSAPData.htmlStr += QString::fromUtf8("</td>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
        hSAPData.htmlStr += QString::fromUtf8("</tr>\n");


        hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
        hSAPData.htmlStr += QString::fromUtf8("</tr>\n");


        hSAPData.htmlStr += QString::fromUtf8("<tr><td><br>\n");
        hSAPData.htmlStr += QString::fromUtf8("</td>\n");
        hSAPData.htmlStr += QString::fromUtf8(" <td colspan=\"2\" rowspan=\"1\">\n");
        hSAPData.htmlStr += QString::fromUtf8("<table style=\"width: 100%\" border=\"1\">\n");
        hSAPData.htmlStr += QString::fromUtf8("<tbody>\n");
        hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">&nbsp;&nbsp;&nbsp;&nbsp;År &nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </td>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">Omsättningstillgångarna</td>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">Totala skulder</td>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">OmsTlg/TS >= 1 </td>\n");
        hSAPData.htmlStr += QString::fromUtf8("</tr>\n");


        if(hSAPData.nofTotalCurrentAssetsArrData == hSAPData.nofTotalLiabilitiesData)
        {
            for(int ii = 0; ii < hSAPData.nofTotalCurrentAssetsArrData; ii++)
            {
                if(hSAPData.totalCurrentAssetsArr[ii].date.toInt() == hSAPData.totalLiabilitiesArr[ii].date.toInt())
                {
                    hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
                    hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">\n");
                    hSAPData.htmlStr += hSAPData.totalCurrentAssetsArr[ii].date;
                    hSAPData.htmlStr += QString::fromUtf8("</td>\n");
                    hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">\n");
                    hSAPData.htmlStr += hSAPData.totalCurrentAssetsArr[ii].data;
                    hSAPData.htmlStr += QString::fromUtf8("</td>\n");
                    hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">\n");
                    hSAPData.htmlStr += hSAPData.totalLiabilitiesArr[ii].data;
                    hSAPData.htmlStr += QString::fromUtf8("</td>\n");

                    double tmpRes = hSAPData.totalCurrentAssetsArr[ii].data.toDouble()/hSAPData.totalLiabilitiesArr[ii].data.toDouble();


                    if(hSAPData.totalLiabilitiesArr[ii].data.toDouble() != 0)
                    {
                        tmpRes = (hSAPData.totalCurrentAssetsArr[ii].data.toDouble() - hSAPData.totalLiabilitiesArr[ii].data.toDouble())/hSAPData.totalCurrentLiabilitiesArr[ii].data.toDouble();
                    }
                    else
                    {
                        tmpRes = hSAPData.totalCurrentAssetsArr[ii].data.toDouble() / 0.001;
                    }

                    tmpRes = 1.0 + tmpRes;


                    if(tmpRes >= 1)
                    {
                        hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\"> <font color=\"blue\">\n");
                    }
                    else
                    {
                        hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\"> <font color=\"red\">\n");
                    }

                    QString tmpDb;
                    tmpDb.sprintf("%.2f", tmpRes);
                    hSAPData.htmlStr += tmpDb;
                    hSAPData.htmlStr += QString::fromUtf8("</font></td>\n");
                    hSAPData.htmlStr += QString::fromUtf8("</tr>\n");
                }
            }
        }
        else
        {
            QMessageBox::information(NULL, QString::fromUtf8("Error:"), QString::fromUtf8("nofTotalCurrentAssetsArrData != nofTotalLiabilitiesData"));
        }

        hSAPData.htmlStr += QString::fromUtf8("</tbody>\n");
        hSAPData.htmlStr += QString::fromUtf8("</table>\n");
        hSAPData.htmlStr += QString::fromUtf8("<br>\n");
        hSAPData.htmlStr += QString::fromUtf8("</td>\n");
        hSAPData.htmlStr += QString::fromUtf8("</tr>\n");

        hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td><br>\n");
        hSAPData.htmlStr += QString::fromUtf8("</td>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
        hSAPData.htmlStr += QString::fromUtf8("</tr>\n");

    }


    hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td><br>\n");
    hSAPData.htmlStr += QString::fromUtf8("</td>\n");


    if((companyType == UNKNOW_COMPANY_TYPE) || (companyType == BANK_COMPANY))
    {
        hSAPData.htmlStr += QString::fromUtf8("<td colspan=\"2\" rowspan=\"1\">\n");
        hSAPData.htmlStr += QString::fromUtf8("<table style=\"width: 100%\" border=\"1\">\n");
        hSAPData.htmlStr += QString::fromUtf8("<tbody>\n");
        hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">&nbsp;&nbsp;&nbsp;År&nbsp; </td>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">Kärnprimärkapitalrelation > 15% &nbsp;&nbsp;&nbsp;\n");
        hSAPData.htmlStr += QString::fromUtf8("</td> </tr>\n");

        for(int ii = 0; ii < hSAPData.nofCoreCapitalRatioData; ii++)
        {

            hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
            hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">\n");
            hSAPData.htmlStr += hSAPData.coreCapitalRatioArr[ii].date;
            hSAPData.htmlStr += QString::fromUtf8("</td>\n");
            if(hSAPData.coreCapitalRatioArr[ii].data.toDouble() > 15.0)
            {
                hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\"><font color=\"blue\">\n");
            }
            else
            {
                hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\"><font color=\"red\">\n");
            }
            hSAPData.htmlStr += hSAPData.coreCapitalRatioArr[ii].data;
            hSAPData.htmlStr += QString::fromUtf8("</font></td>\n");
            hSAPData.htmlStr += QString::fromUtf8("</tr>\n");

        }

        hSAPData.htmlStr += QString::fromUtf8("</tbody>\n");
        hSAPData.htmlStr += QString::fromUtf8("</table>\n");
        hSAPData.htmlStr += QString::fromUtf8("<br>\n");
        hSAPData.htmlStr += QString::fromUtf8("</td>\n");
        hSAPData.htmlStr += QString::fromUtf8("</tr>\n");

        hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
        hSAPData.htmlStr += QString::fromUtf8("</tr>\n");
    }


    if((companyType == UNKNOW_COMPANY_TYPE) ||
       (companyType == UTILITIS_COMPANY)    || (companyType == UTILITIS_COMPANY_2))
    {
        hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td colspan=\"2\" rowspan=\"1\">\n");
        hSAPData.htmlStr += QString::fromUtf8("<table style=\"width: 100%\" border=\"1\">\n");
        hSAPData.htmlStr += QString::fromUtf8("<tbody>\n");
        hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">&nbsp;&nbsp;&nbsp;År&nbsp;&nbsp;&nbsp; </td>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">Totala skulder</td>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">Eget kapital </td>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">TS/EK <= 2 </td>\n");
        hSAPData.htmlStr += QString::fromUtf8("</tr>\n");


        // equity & total liabilities
        for(int ii = 0; ii < hSAPData.nofTotalLiabilitiesData; ii++)
        {
            if(hSAPData.equityArr[ii].date.toInt() == hSAPData.totalLiabilitiesArr[ii].date.toInt())
            {
                hSAPData.htmlStr += QString::fromUtf8("<tr>\n");

                hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">\n");
                hSAPData.htmlStr += hSAPData.equityArr[ii].date;
                hSAPData.htmlStr += QString::fromUtf8("</td>\n");

                hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">\n");
                hSAPData.htmlStr += hSAPData.totalLiabilitiesArr[ii].data;

                hSAPData.htmlStr += QString::fromUtf8("</td>\n");
                hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">\n");
                hSAPData.htmlStr += hSAPData.equityArr[ii].data;
                hSAPData.htmlStr += QString::fromUtf8("</td>\n");

                QString tmpStr;
                double realEquity = hSAPData.equityArr[ii].data.toDouble();
                double realTotalLiabilities = hSAPData.totalLiabilitiesArr[ii].data.toDouble();
                double ratio;
                if(realEquity == 0)
                {
                    realEquity = 0.001;
                    ratio = realTotalLiabilities / realEquity;
                }
                if(realEquity < 0)
                {
                    ratio = realTotalLiabilities / realEquity;
                    if(ratio > 0)
                    {
                        ratio = -ratio;
                    }
                }
                else
                {
                    ratio = realTotalLiabilities / realEquity;
                }

                if(ratio > 2)
                {
                    hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\"><font color=\"red\">\n");
                }
                else
                {
                    hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\"><font color=\"blue\">\n");
                }

                tmpStr.sprintf("%.2f", ratio);
                hSAPData.htmlStr += tmpStr;

                hSAPData.htmlStr += QString::fromUtf8("</font></td>\n");
                hSAPData.htmlStr += QString::fromUtf8("</tr>\n");
            }
        }

        hSAPData.htmlStr += QString::fromUtf8("</tbody>\n");
        hSAPData.htmlStr += QString::fromUtf8("</table>\n");
        hSAPData.htmlStr += QString::fromUtf8("</td>\n");
        hSAPData.htmlStr += QString::fromUtf8("</tr>\n");


        hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
        hSAPData.htmlStr += QString::fromUtf8("</tr>\n");
    }

    if((companyType == UNKNOW_COMPANY_TYPE) || (companyType == REAL_ESTATE_COMPANY) ||
       (companyType == UTILITIS_COMPANY)    || (companyType == UTILITIS_COMPANY_2))
    {
        hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td colspan=\"2\" rowspan=\"1\"><br>\n");
        hSAPData.htmlStr += QString::fromUtf8("<table style=\"width: 100%\" border=\"1\">\n");
        hSAPData.htmlStr += QString::fromUtf8("<tbody>\n");
        hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">&nbsp;&nbsp;År&nbsp;</td>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">Soliditet >= 40% &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
        hSAPData.htmlStr += QString::fromUtf8("</td> </tr>\n");

        for(int ii = 0; ii < hSAPData.nofSolidityData; ii++)
        {
            hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
            hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">\n");
            hSAPData.htmlStr += hSAPData.solidityArr[ii].date;
            hSAPData.htmlStr += QString::fromUtf8("</td>\n");
            if(hSAPData.solidityArr[ii].data.toDouble() >= 40)
            {
                hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\"><font color=\"blue\">\n");
            }
            else
            {
                hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\"><font color=\"red\">\n");
            }
            hSAPData.htmlStr += hSAPData.solidityArr[ii].data;
            hSAPData.htmlStr += QString::fromUtf8("</font></td>\n");

            hSAPData.htmlStr += QString::fromUtf8("</tr>\n");
        }

        hSAPData.htmlStr += QString::fromUtf8("</tbody>\n");
        hSAPData.htmlStr += QString::fromUtf8("</table>\n");
        hSAPData.htmlStr += QString::fromUtf8("<br>\n");
        hSAPData.htmlStr += QString::fromUtf8("</td>\n");
        hSAPData.htmlStr += QString::fromUtf8("</tr>\n");
        hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
        hSAPData.htmlStr += QString::fromUtf8("</tr>\n");


        hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td colspan=\"2\" rowspan=\"1\">\n");
        hSAPData.htmlStr += QString::fromUtf8("<table style=\"width: 100%\" border=\"1\">\n");
        hSAPData.htmlStr += QString::fromUtf8("<tbody>\n");
        hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">&nbsp;&nbsp;År&nbsp; </td>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">Räntetäckningsgraden >= 3 &nbsp;&nbsp;&nbsp;&nbsp;\n");
        hSAPData.htmlStr += QString::fromUtf8("</td> </tr>\n");

        for(int ii = 0; ii < hSAPData.nofCoverageRatioData; ii++)
        {
            hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
            hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">\n");
            hSAPData.htmlStr += hSAPData.coverageRatioArr[ii].date;
            hSAPData.htmlStr += QString::fromUtf8("</td>\n");
            if(hSAPData.coverageRatioArr[ii].data.toDouble() >= 3)
            {
                hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\"><font color=\"blue\">\n");
            }
            else
            {
                hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\"><font color=\"red\">\n");
            }

            hSAPData.htmlStr += hSAPData.coverageRatioArr[ii].data;
            hSAPData.htmlStr += QString::fromUtf8("</font></td>\n");
            hSAPData.htmlStr += QString::fromUtf8("</tr>\n");
        }

        hSAPData.htmlStr += QString::fromUtf8("</tbody>\n");
        hSAPData.htmlStr += QString::fromUtf8("</table>\n");
        hSAPData.htmlStr += QString::fromUtf8("<br>\n");
        hSAPData.htmlStr += QString::fromUtf8("<br>\n");
        hSAPData.htmlStr += QString::fromUtf8("</td>\n");
        hSAPData.htmlStr += QString::fromUtf8("</tr>\n");

        hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
        hSAPData.htmlStr += QString::fromUtf8("</tr>\n");
    }

    hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td colspan=\"2\" rowspan=\"1\">\n");
    hSAPData.htmlStr += QString::fromUtf8("<table style=\"width: 100%\" border=\"1\">\n");
    hSAPData.htmlStr += QString::fromUtf8("<tbody>\n");
    hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">Från År</td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">Till År</td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">Intjäningsförmågan  > 3% &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    hSAPData.htmlStr += QString::fromUtf8("</td></tr>\n");

    if(hSAPData.nofEarningsArrData > 0)
    {



        hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">\n");

        if((hSAPData.nofEarningsArrData > 1))
        {
            hSAPData.htmlStr += hSAPData.earningsDataArr[0].date;
        }
        hSAPData.htmlStr += QString::fromUtf8("</td>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">\n");
        if((hSAPData.nofEarningsArrData > 1))
        {
            hSAPData.htmlStr += hSAPData.earningsDataArr[hSAPData.nofEarningsArrData-1].date;
        }
        hSAPData.htmlStr += QString::fromUtf8("</td>\n");

        hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">\n");

        double firstData = hSAPData.earningsDataArr[0].data.toDouble();
        double lastData = hSAPData.earningsDataArr[hSAPData.nofEarningsArrData-1].data.toDouble();
        double res;
        if((hSAPData.nofEarningsArrData > 1))
        {
            if(lastData < 0.0)
                lastData = -lastData;

            if(firstData < 0.0)
                firstData = -firstData;

            if(firstData == 0.0)
                firstData = 0.001;


            res = pow((lastData/firstData), (1.0/(hSAPData.nofEarningsArrData-1.0)));
            QString tmpStr;

            if(res > 1.03)
                tmpStr = QString::fromUtf8("<font color=\"blue\">Årlig tillväxt: ");
            else
                tmpStr = QString::fromUtf8("<font color=\"red\">Årlig tillväxt: ");

            hSAPData.htmlStr += tmpStr;
            tmpStr.clear();
            tmpStr.sprintf("%.2f",(res - 1) * 100.0);
            hSAPData.htmlStr += tmpStr;
            tmpStr.clear();
            tmpStr = QString::fromUtf8(" %");
            hSAPData.htmlStr += tmpStr;

        }
        if((hSAPData.nofEarningsArrData > 1))
        {
            hSAPData.htmlStr += QString::fromUtf8("</font></td>\n");
        }
        else
        {
            hSAPData.htmlStr += QString::fromUtf8("</font></td>\n");
        }
        hSAPData.htmlStr += QString::fromUtf8("</tr>\n");


    }


    hSAPData.htmlStr += QString::fromUtf8("</tbody>\n");
    hSAPData.htmlStr += QString::fromUtf8("</table>\n");
    hSAPData.htmlStr += QString::fromUtf8("<br>\n");
    hSAPData.htmlStr += QString::fromUtf8("<br>\n");
    hSAPData.htmlStr += QString::fromUtf8("</td>\n");
    hSAPData.htmlStr += QString::fromUtf8("</tr>\n");



    hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("</tr>\n");

    hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td colspan=\"2\" rowspan=\"1\">\n");
    hSAPData.htmlStr += QString::fromUtf8("<table style=\"width: 100%\" border=\"1\">\n");
    hSAPData.htmlStr += QString::fromUtf8("<tbody>\n");
    hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">&nbsp;&nbsp;&nbsp;År</td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">Vinst/Aktie</td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">Vinsttillväxt (%)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td></tr>\n");

    QString earningsGrowthStr;
    double earningsGrowth;
    bool negativeSign = false;
    double currEarnings;
    double lastEarnings;



    for(int ii = 0; ii < hSAPData.nofEarningsArrData; ii++)
    {
        hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">\n");
        hSAPData.htmlStr += hSAPData.earningsDataArr[ii].date;
        hSAPData.htmlStr += QString::fromUtf8("</td>\n");

        if(hSAPData.earningsDataArr[ii].data.toDouble() > 0)
        {
            hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\"><font color=\"blue\">\n");
        }
        else
        {
            hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\"><font color=\"red\">\n");
        }

        hSAPData.htmlStr += hSAPData.earningsDataArr[ii].data;

        hSAPData.htmlStr += QString::fromUtf8("</font></td>\n");

        negativeSign = false;

        if(ii > 0)
        {
            currEarnings = hSAPData.earningsDataArr[ii].data.toDouble();
            lastEarnings = hSAPData.earningsDataArr[ii-1].data.toDouble();

            if(currEarnings < 0)
            {
                negativeSign = true;
            }


            // Handle divide by zero
            if(lastEarnings == 0)
            {
                earningsGrowth = currEarnings / 0.001;
            }
            else
            {
                earningsGrowth = (currEarnings - lastEarnings) / lastEarnings;
            }



            // Change sign if negative growth
            if( ((negativeSign == true) && (earningsGrowth > 0)) || ((currEarnings > 0) && (lastEarnings < 0)) )
            {
                earningsGrowth = -earningsGrowth;
            }

             earningsGrowth = earningsGrowth * 100;

            earningsGrowthStr.sprintf("%.2f",  earningsGrowth);
        }
        else
        {
            earningsGrowthStr.clear();
        }

        if(earningsGrowth > 0)
        {
            hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\"><font color=\"blue\">\n");
        }
        else
        {
            hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\"><font color=\"red\">\n");
        }

        hSAPData.htmlStr += earningsGrowthStr;

        hSAPData.htmlStr += QString::fromUtf8("</font></td>\n");
        hSAPData.htmlStr += QString::fromUtf8("</tr>\n");
    }



    hSAPData.htmlStr += QString::fromUtf8("</tbody>\n");
    hSAPData.htmlStr += QString::fromUtf8("</table>\n");
    hSAPData.htmlStr += QString::fromUtf8("</td>\n");
    hSAPData.htmlStr += QString::fromUtf8("</tr>\n");

    hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("</tr>\n");

    hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td><br></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td colspan=\"2\" rowspan=\"1\">\n");
    hSAPData.htmlStr += QString::fromUtf8("<table style=\"width: 100%\" border=\"1\">\n");
    hSAPData.htmlStr += QString::fromUtf8("<tbody>\n");

    hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">&nbsp;&nbsp;&nbsp; År</td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">Utdelning (%)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    hSAPData.htmlStr += QString::fromUtf8("</td></tr>\n");

    for(int ii = 0; ii < hSAPData.nofDividendArrData; ii++)
    {
        hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
        hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\">\n");
        hSAPData.htmlStr += hSAPData.dividendDataArr[ii].date;
        hSAPData.htmlStr += QString::fromUtf8("</td>\n");
        if(hSAPData.dividendDataArr[ii].data.toDouble() > 0)
        {
            hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\"><font color=\"blue\">\n");
        }
        else
        {
            hSAPData.htmlStr += QString::fromUtf8("<td style=\"text-align: left; vertical-align: top; background-color: white;\"><font color=\"red\">\n");
        }
        QString div;
        div.sprintf("%.2f", hSAPData.dividendDataArr[ii].data.toDouble());

        hSAPData.htmlStr += div;
        hSAPData.htmlStr += QString::fromUtf8("</font></td>\n");

        hSAPData.htmlStr += QString::fromUtf8("</tr>\n");
    }

    hSAPData.htmlStr += QString::fromUtf8("</tbody>\n");
    hSAPData.htmlStr += QString::fromUtf8("</table>\n");
    hSAPData.htmlStr += QString::fromUtf8("</td>\n");
    hSAPData.htmlStr += QString::fromUtf8("</tr>\n");




    // Stop



    // New line Other Information
    hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\"><br>Övrigt:</td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\"><br></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\"><br>\n");
    hSAPData.htmlStr += QString::fromUtf8(hSAPData.otherInformation.toLocal8Bit().constData());

    hSAPData.htmlStr += QString::fromUtf8("<br><br></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("</tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<tr><td colspan=\"3\" rowspan=\"1\"> <hr></td></tr>\n");

    hSAPData.htmlStr += QString::fromUtf8("</tbody>\n");
    hSAPData.htmlStr += QString::fromUtf8("</table>\n");

    // Add html table
    hSAPData.htmlStr += QString::fromUtf8("<table style=\"width: 100%;\" border=\"0\">\n");
    hSAPData.htmlStr += QString::fromUtf8("<tbody>\n");

    // New line
    hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\"><b>PE</b></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\"><b>DirAvk</b></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\"><b>Avkastning eget kapital</b></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\"><b>Skuld/Eget kap</b></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td colspan=\"1\" rowspan=\"1\" style=\"height: 35px;\"><b>Förväntad risk</b></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("</tr>\n");



    // New line Key Values
    hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px;text-align: left; vertical-align: top;\">\n");
    hSAPData.htmlStr += hSAPData.keyValuePe;
    hSAPData.htmlStr += QString::fromUtf8("</td>\n");

    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\">\n");
    hSAPData.htmlStr += hSAPData.keyValueYield;
    hSAPData.htmlStr += QString::fromUtf8("</td>\n");

    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\">\n");
    hSAPData.htmlStr += hSAPData.returnOnEquity;
    hSAPData.htmlStr += QString::fromUtf8("</td>\n");

    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\">");
    hSAPData.htmlStr += hSAPData.keyValueTotalDebtEquityRatio;
    hSAPData.htmlStr += QString::fromUtf8("</td>\n");

    hSAPData.htmlStr += QString::fromUtf8("<td colspan=\"1\" rowspan=\"1\" style=\"height: 35px; text-align: left; vertical-align: top;\">\n");
    hSAPData.htmlStr += hSAPData.riskStdDev;
    hSAPData.htmlStr += QString::fromUtf8("</td>\n");
    hSAPData.htmlStr += QString::fromUtf8("</tr>\n");

    // New line
    hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\"><b>PS</b></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\"><b>Vinst/Utdelning</b></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\"><b>Sub/Kurs</b></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\"><b>Balanslikviditet</b></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td colspan=\"1\" rowspan=\"1\" style=\"height: 35px;text-align: left; vertical-align: top;\"><b>Förväntad avkasning</b></td>\n");
    hSAPData.htmlStr += QString::fromUtf8("</tr>\n");



    // New line
    hSAPData.htmlStr += QString::fromUtf8("<tr>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\">\n");
    hSAPData.htmlStr += hSAPData.keyValuePs;
    hSAPData.htmlStr += QString::fromUtf8("</td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\">\n");
    hSAPData.htmlStr += hSAPData.keyValueErningsDividentRatio;
    hSAPData.htmlStr += QString::fromUtf8("</td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\">\n");
    hSAPData.htmlStr += hSAPData.keyValueNavPriceRatio;
    hSAPData.htmlStr += QString::fromUtf8("</td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td style=\"height: 35px; text-align: left; vertical-align: top;\">\n");
    hSAPData.htmlStr += hSAPData.keyValueCurrentRatio;
    hSAPData.htmlStr += QString::fromUtf8("</td>\n");
    hSAPData.htmlStr += QString::fromUtf8("<td colspan=\"1\" rowspan=\"1\" style=\"height: 35px; text-align: left; vertical-align: top;\">\n");
    hSAPData.htmlStr += hSAPData.meanReturns;
    hSAPData.htmlStr += QString::fromUtf8("</td>\n");
    hSAPData.htmlStr += QString::fromUtf8("</tr>\n");

    hSAPData.htmlStr += QString::fromUtf8("</tbody>\n");
    hSAPData.htmlStr += QString::fromUtf8("</table>\n");
    hSAPData.htmlStr += QString::fromUtf8("</body>\n");
    hSAPData.htmlStr += QString::fromUtf8("</html>\n");

}
