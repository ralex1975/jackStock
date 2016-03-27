/****************************************************************
 *
 * File:        parseCompDescription.cpp
 *
 * Description:
 *
 *
 ****************************************************************/


#include "parsecompdescription.h"
#include "util.h"
#include "dbHndl.h"




#define MAIN_TAG2_START_END_TAG QString::fromUtf8("<h2>Företagsinformation</h2>")
#define MAIN_TAG3_START_END_TAG QString::fromUtf8("<h2>Företagsbeskrivning</h2>")
#define SUBTAG_2A_SUB_STRING QString::fromUtf8(">Sektor</a></td>")
#define SUBTAG_2B_SUB_STRING QString::fromUtf8(">Bransch</a></td>")
#define SUBTAG_2C_SUB_STRING QString::fromUtf8("<td class=\"betona\">VD</td>")
#define SUBTAG_2C_START_SIGN QString::fromUtf8("<td colspan=\"3\">")

#define SUBTAG_3A_START_SIGN QString::fromUtf8("<div class=\"punktlista\">")
#define SUBTAG_3A_END_SIGN   QString::fromUtf8("</div>")

#define SUBTAG_START_SIGN QString::fromUtf8("<td>")
#define SUBTAG_END_SIGN QString::fromUtf8("/<td>")

const QString parseCompDescription::m_removeTockenArr[2] =
{
    "<ul class=\"agenda\"><li>",
    "</li></ul><ul class=\"agenda\"><li>"
};

/****************************************************************
 *
 * Function:    readFile()
 *
 * Description:
 *
 *
 ****************************************************************/

parseCompDescription::parseCompDescription()
{
}


/****************************************************************
 *
 * Function:    readFile()
 *
 * Description:
 *
 *
 ****************************************************************/
bool parseCompDescription::
readFile(QString filename, QString assetName, QString assetSymbol)
{
    int i;
    QString str;
    QString resStr;
    QFile file(filename);
    QString errStr = QString("Fail to open file: %1").arg(filename);
    CDbHndl db;
    NordnetCompanyDescription_ST data;
    CUtil cu;




    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::critical(NULL, QString::fromUtf8("Fail to open file"), errStr);
        return false;
    }

    QTextStream inStream(&file);
    inStream.setCodec("ISO 8859-1");

    m_mainState = MSTATE_MAIN_COMPANY_INFO;

    while(!inStream.atEnd())
    {
        str = inStream.readLine();
        str = str.trimmed();

        switch(m_mainState)
        {
        default:
        case MSTATE_MAIN_COMPANY_INFO:
            if(str.contains(MAIN_TAG2_START_END_TAG))
            {
                qDebug() << str;
                for(i = 0; i < 3; i++)
                {
                    str = inStream.readLine();
                    str = str.trimmed();
                }
                if(str.contains(SUBTAG_2A_SUB_STRING))
                {
                    //qDebug() << str;

                    // Get stock sector
                    str = inStream.readLine();
                    str = str.trimmed();
                    data.sector = str.mid(4, (str.size() - 9));
                    data.sector = data.sector.trimmed();
                    qDebug() << data.sector;


                    // Get stock branch
                    str = inStream.readLine();
                    str = str.trimmed();

                    if(str.contains(SUBTAG_2B_SUB_STRING))
                    {
                        str = inStream.readLine();
                        str = str.trimmed();

                        data.bransch = str.mid(4, (str.size() - 9));
                        data.bransch = data.bransch.trimmed();
                        qDebug() << data.bransch;
                    }

                    m_mainState = MSTATE_COMPANY_INFO_EXECUTIVE_DIRECTOR;

                }
                else
                {
                    return false;
                }
            }
            break;
        case MSTATE_COMPANY_INFO_EXECUTIVE_DIRECTOR:
            if(str.contains(SUBTAG_2C_SUB_STRING))
            {
                // qDebug() << str;
                str = inStream.readLine();
                str = str.trimmed();
                resStr = str.mid(16, (str.size() - 21));

                cu.htmlCodesToChar(resStr, resStr);
                #if 0
                resStr = resStr.replace(QString::fromUtf8("&#246;"), QString::fromUtf8("ö"));
                resStr = resStr.replace(QString::fromUtf8("&#228;"), QString::fromUtf8("ä"));
                resStr = resStr.replace(QString::fromUtf8("&#229;"), QString::fromUtf8("å"));

                resStr = resStr.replace(QString::fromUtf8("&#214;"), QString::fromUtf8("Ö"));
                resStr = resStr.replace(QString::fromUtf8("&#196;"), QString::fromUtf8("Ä"));
                resStr = resStr.replace(QString::fromUtf8("&#197;"), QString::fromUtf8("Å"));
                resStr = resStr.replace(QString::fromUtf8("&#233;"), QString::fromUtf8("é"));7
                #endif

                data.executiveDirector = resStr;
                data.executiveDirector.trimmed();
                qDebug() << data.executiveDirector;
                m_mainState = MSTATE_COMPANY_DESCR;

            }
            break;
        case MSTATE_COMPANY_DESCR:

           // qDebug() << str;
            if(str.contains(MAIN_TAG3_START_END_TAG))
            {
                for(i = 0; i < 2; i++)
                {
                    str = inStream.readLine();
                    str = str.trimmed();
                }

                resStr = str;
                // qDebug() << resStr;
                m_mainState = MSTATE_COMPANY_DESCR_TXT;
            }
            break;

        case MSTATE_COMPANY_DESCR_TXT:

            if(str.contains(QString::fromUtf8("</div><!-- /twocol right -->")))
            {
                resStr = resStr.replace(QString::fromUtf8("</li></ul><ul class=\"agenda\"><li>"), "");
                resStr = resStr.replace(QString::fromUtf8("<ul class=\"agenda\"><li>"), "");
                resStr = resStr.replace(QString::fromUtf8("</li></ul>"), "");
                resStr = resStr.replace(QString::fromUtf8("</div>"), "");

                cu.htmlCodesToChar(resStr, resStr);
                #if 0
                resStr = resStr.replace(QString::fromUtf8("&#246;"), QString::fromUtf8("ö"));
                resStr = resStr.replace(QString::fromUtf8("&#228;"), QString::fromUtf8("ä"));
                resStr = resStr.replace(QString::fromUtf8("&#229;"), QString::fromUtf8("å"));

                resStr = resStr.replace(QString::fromUtf8("&#214;"), QString::fromUtf8("Ö"));
                resStr = resStr.replace(QString::fromUtf8("&#196;"), QString::fromUtf8("Ä"));
                resStr = resStr.replace(QString::fromUtf8("&#197;"), QString::fromUtf8("Å"));
                resStr = resStr.replace(QString::fromUtf8("&#233;"), QString::fromUtf8("é"));
                #endif

                resStr = resStr.replace(QString::fromUtf8("<br/>"), QString::fromUtf8("\n"));
                resStr = resStr.replace(QString::fromUtf8("</li></ul></div>"), QString::fromUtf8(""));


                data.businessDescription = resStr.trimmed();
                data.assetName = assetName;
                data.assetSymbol = assetSymbol;
                db.insertNordnetCompanyDescriptionData(data);


                qDebug() << resStr;
                file.close();
                return true;
            }
            else
            {
                resStr += str;
                // qDebug() << resStr;
            }

            break;
        }
    }

    file.close();
    return false;
}


