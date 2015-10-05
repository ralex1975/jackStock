#include "threadcalcefficentfront.h"
#include "extendedTable.h"

threadCalcEfficentFront::threadCalcEfficentFront(QObject *parent) :
    QThread(parent),
    m_isIdle(true)
{
}


void threadCalcEfficentFront::run()
{

    calcEfficPortData();

}

void threadCalcEfficentFront::initCalcEfficPortData(QString startDate,
                                                QString endDate,
                                                QTableView **tableView,
                                                CExtendedTable &extendedtable,
                                                double **varCovMatrix,
                                                double **correlationMatrix,
                                                QVector <CDbHndl::EfficPortStockData_ST> &stockArr1)
{
    if(m_isIdle == true)
    {
        m_startDate = startDate;
        m_endDate = endDate;
        m_tableView = *(tableView);
        m_extendedtable = &extendedtable;

        m_varCovMatrix = varCovMatrix;
        m_correlationMatrix = correlationMatrix;
        m_stockArr1 = &stockArr1;
    }
}


void threadCalcEfficentFront::calcEfficPortData(void)
{
    QString tmp;
    CDbHndl::EfficPortStockData_ST data;
    CDbHndl::EfficPortStockData_ST data1;
    double result;
    CDbHndl db;


#if 1
    if(m_isIdle == true)
    {
        m_isIdle = false;
        db.openDb(PATH_JACK_STOCK_DB, true);


        for(int row = 0; row < m_stockArr1->size(); row++)
        {
            for(int col = 0; col < m_stockArr1->size(); col++)
            {
                data = (*m_stockArr1)[row];
                data1 = (*m_stockArr1)[col];
                if(data.isValid == true && data1.isValid==true && data.isSelected == true && data1.isSelected == true)
                {
                    if(true == db.efficPortfCreateVarianceArr(m_startDate, m_endDate, data, db.m_rowVarianseArr, true))
                    {
                        if(true == db.efficPortfCreateVarianceArr(m_startDate, m_endDate, data1, db.m_colVarianseArr, true))
                        {
                            if(true == db.efficPortfCalcCovar(db.m_rowVarianseArr, db.m_colVarianseArr, result))
                            {
                                m_varCovMatrix[row][col] = result;

                                data = (*m_stockArr1)[row];
                                data1 = (*m_stockArr1)[col];
                                m_correlationMatrix[row][col] = (result / (data.riskStdDev * data1.riskStdDev));
                                tmp.sprintf("%.1f", 100 *(result / (data.riskStdDev * data1.riskStdDev)));
                                // tmp = "Hej";
                                m_extendedtable->addData(m_tableView, tmp, row, col);
                                // qDebug() << tmp;
                                // qDebug() << "correlation A" << m_correlationMatrix[row][col];
                                // qDebug() << "row" << row << "col" << col;
                            }
                            else
                            {
                                tmp = "kass";
                                m_extendedtable->addData(m_tableView, tmp, row, col);
                            }
                        }
                        else
                        {
                            tmp = "kass";
                            m_extendedtable->addData(m_tableView, tmp, row, col);
                        }
                    }
                    else
                    {
                        tmp = "kass";
                        m_extendedtable->addData(m_tableView, tmp, row, col);
                    }
                }
                else
                {
                    tmp = "kass";
                    m_extendedtable->addData(m_tableView, tmp, row, col);
                }
            }
        }
         m_tableView->resizeColumnsToContents();
         db.closeDb(true);
         m_isIdle = true;
   }
#endif

}
