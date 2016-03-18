#include "../../inc/TaBuySellIdicator/taBuySellTask.h"
#include "QMessageBox"


CMyTaskQue m_taTaskInputQue(-1, true, 0);
CMyTaskQue m_taTaskOutputQue(-1, true, 0);



/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
taBuySellTask::taBuySellTask(QObject *parent) :
                                            QThread(parent)
{


}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
void taBuySellTask::run()
{
    CMyTaskQueData data;




    while(true)
    {
        m_file = fopen("debugSell.txt", "a+");
        m_taTaskInputQue.removeFirst(data);
        if(!m_file)
        {
            QMessageBox::information(NULL, QString::fromUtf8("File error"), QString::fromUtf8("Fail to open file"));
            return;
        }
        qDebug() << data.stockSymbol;
        fprintf(m_file, "%s\n", data.stockSymbol.toStdString().c_str());
        fclose(m_file);
    }
}
