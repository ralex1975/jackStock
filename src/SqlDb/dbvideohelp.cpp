#include "dbHndl.h"
#include <QMessageBox>



/****************************************************************
 *
 * Function:    saveExerciseWordAndSoundToFile()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::saveExerciseWordAndSoundToFile(QString filename,
                                        bool dbIsHandledExternly)
{
    QFile file(filename);
    QSqlRecord rec;
    QString str;
    bool found1 = false;
    bool found2 = false;




    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }


    QSqlQuery qry(m_db);

    str.sprintf("SELECT exerciseSoundPath, exerciseWord "
                " FROM TblExerciseData "
                " ORDER BY exerciseWord ASC;");


    qDebug() <<  str;

    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QObject::tr("db error"), qry.lastError().text().toUtf8().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);

        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("exerciseWord").isNull() == true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                found1 = true;
            }

            if(rec.value("exerciseSoundPath").isNull() == true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                found2 = true;
            }

            if(found1 == true && found2 == true)
            {
                found1 = false;
                found2 = false;

                out << (QString)rec.value("exerciseWord").toString().toUtf8() << "|";
                out << (QString)rec.value("exerciseSoundPath").toString().toUtf8() << endl;
            }
        }
    }

    file.close();
    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}






/****************************************************************
 *
 * Function:    addExerciseDataToTreeWidget()
 *
 * Description: This combobox has a checkbox
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::addExerciseDataToTreeWidget(QTreeWidget *treeWidget,
                                          QVector<CDbHndl::exerciseData_ST> &exerciseDataArr,
                                          QString exerciseTopDescp,
                                          QString exerciseSubDescp,
                                          bool random,
                                          bool clearExerciseDataArr,
                                          bool dbIsHandledExternly)
{
    QSqlRecord rec;
    QString str;
    CDbHndl::exerciseData_ST data;
    bool found1 = false;
    bool found2 = false;
    int i = 0;
    int topExerciseDescpId;
    int subExerciseDescpId;


    if(m_mutex.tryLock(100) == true)
    {
        m_mutex.unlock();

    }
    else
    {
        // Mutex is locked
        return false;
    }


    if(false == findTopExerciseDataId(exerciseTopDescp, topExerciseDescpId))
    {
        return false;

    }

    if(false == findSubExerciseDataId(exerciseSubDescp, topExerciseDescpId, subExerciseDescpId))
    {
        return false;
    }


    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }


    QSqlQuery qry(m_db);

// DISTINCT
    if(random == true)
    {
        str.sprintf("SELECT exerciseTopDescpId, exerciseSubDescpId, exerciseWord, exercisePronunciation, exerciseSoundPath "
                    " FROM TblExerciseData"
                    " WHERE exerciseTopDescpId = %d AND exerciseSubDescpId = %d "
                    " ORDER BY Random();",
                    topExerciseDescpId,
                    subExerciseDescpId);
    }
    else
    {
        str.sprintf("SELECT DISTINCT exerciseTopDescpId, exerciseSubDescpId, exerciseWord, exercisePronunciation, exerciseSoundPath "
                    " FROM TblExerciseData"
                    " WHERE exerciseTopDescpId = %d AND exerciseSubDescpId = %d "
                    " ;",
                    topExerciseDescpId,
                    subExerciseDescpId);

//        str.sprintf("SELECT DISTINCT exerciseTopDescpId, exerciseSubDescpId, exerciseWord, exercisePronunciation "
//                    " FROM TblExerciseData"
//                    " WHERE exerciseTopDescpId = %d AND exerciseSubDescpId = %d "
//                    " ORDER BY exerciseWord ASC;",
//                    topExerciseDescpId,
//                    subExerciseDescpId);

    }


    qry.prepare(str);

     qDebug() <<  str;


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QObject::tr("db error"), qry.lastError().text().toUtf8().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        if(clearExerciseDataArr == true)
        {
            exerciseDataArr.clear();
        }

        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("exerciseWord").isNull()==true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                found1 = true;
            }

            if(rec.value("exercisePronunciation").isNull()==true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                found2 = true;
            }

            if(found1 == true && found2 == true)
            {
                found1 = false;
                found2 = false;

                QTreeWidgetItem *item = new QTreeWidgetItem;
                item->setText(0, (QString)rec.value("exerciseWord").toString());
                item->setText(1, (QString)rec.value("exercisePronunciation").toString());
                item->setExpanded(true);
                               item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
                               item->setCheckState(2, Qt::Unchecked);
                               item->setCheckState(3, Qt::Unchecked);
                               item->setText(2, " ");
                               item->setText(3, " ");
                treeWidget->addTopLevelItem(item);

                data.exerciseWord = (QString)rec.value("exerciseWord").toString();
                data.exercisePronunciation = (QString)rec.value("exercisePronunciation").toString();

                if(rec.value("exercisePronunciation").isNull()==true)
                {
                    data.exerciseSound.clear();
                }
                else
                {
                    data.exerciseSound = (QString)rec.value("exerciseSoundPath").toString();
                }
                    exerciseDataArr.insert(i, data);
                i++;
            }
        }
    }

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}





/****************************************************************
 *
 * Function:    findExerciseListId()
 *
 * Description:
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::addSubExerciseDataToComboBox(QComboBox *comboBox,
                                      QString topExerciseName,
                                      bool dbIsHandledExternly)
{
    QSqlRecord rec;
    QString str;
    int topExerciseDescpId;


    if(m_mutex.tryLock(100) == true)
    {
        m_mutex.unlock();

    }
    else
    {
        // Mutex is locked
        return false;
    }


   if(false == findTopExerciseDataId(topExerciseName, topExerciseDescpId, dbIsHandledExternly))
   {
       return false;
   }


    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

    comboBox->clear();


     str.sprintf(" SELECT exerciseSubDescp, exerciseTopDescpId "
                 " FROM TblExerciseSubDescp "
                 " WHERE exerciseTopDescpId = %d;",
                   topExerciseDescpId);

    qDebug() << str;

    qry.prepare(str);

    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toUtf8().constData());
        }
        qDebug() << qry.lastError();
        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }
    else
    {
        while(qry.next())
        {
            rec = qry.record();


            if(rec.value("exerciseSubDescp").isNull() == true)
            {
                qry.finish();
                if(dbIsHandledExternly == false)
                {
                    closeDb();
                    m_mutex.unlock();
                }
                return false;
            }
            else
            {
                comboBox->addItem((QString)rec.value("exerciseSubDescp").toString().toUtf8());
                qDebug( ) << rec.value("exerciseSubDescp").toString().toUtf8();
            }

        }
    }

    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }
    return true;
}






/****************************************************************
 *
 * Function:    addExerciseListsToComboBox()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::addTopExerciseDataToComboBox(QComboBox *comboBox)
{
    QSqlRecord rec;
    QString str;
    int index;


    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    str.sprintf("SELECT exerciseTopDescp "
                " FROM TblExerciseTopDescp"
                " ORDER BY LOWER(exerciseTopDescp) %s;",
                SQL_STR_ASC);


    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toUtf8().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        index = comboBox->currentIndex();
        comboBox->clear();
        qDebug( ) << "Clear combobox";
        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("exerciseTopDescp").isNull()==true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                comboBox->addItem((QString)rec.value("exerciseTopDescp").toString().toUtf8());
                qDebug( ) << rec.value("exerciseTopDescp").toString().toUtf8();
            }
        }
    }

    comboBox->setCurrentIndex(index);
    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}



/*****************************************************************
 *
 * Function:		delAllExerciseList()
 *
 * Description:		This function delete all data in temp list
 *
 *
 *
 *****************************************************************/
bool CDbHndl::delAllExerciseTopData(void)
{
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    // Create sql question
    str.sprintf("DELETE "
                "FROM TblExerciseTopDescp;");

    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 1"), qry.lastError().text().toUtf8().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}


/*****************************************************************
 *
 * Function:		delAllExerciseList()
 *
 * Description:		This function delete all data in temp list
 *
 *
 *
 *****************************************************************/
bool CDbHndl::delAllExerciseSubData(void)
{
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    // Create sql question
    str.sprintf("DELETE "
                "FROM TblExerciseSubDescp;");

    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 1"), qry.lastError().text().toUtf8().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}


/*****************************************************************
 *
 * Function:		delAllExerciseList()
 *
 * Description:		This function delete all data in temp list
 *
 *
 *
 *****************************************************************/
bool CDbHndl::delAllExerciseData(void)
{
    QString str;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    // Create sql question
    str.sprintf("DELETE "
                "FROM TblExerciseData;");

    qry.prepare(str);

    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("Database Error 1"), qry.lastError().text().toUtf8().constData());
        }
        closeDb();
        m_mutex.unlock();
        return false;
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}



/****************************************************************
 *
 * Function:    addExerciseName()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::addExerciseData(QString exerciseWord,
                         QString exercisePronunciation,
                         int exerciseTopDescpId,
                         int exerciseSubDescpId,
                         bool dbIsHandledExternly)
{
    QString str;


    if(dbIsHandledExternly ==false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);





    str.sprintf("INSERT INTO TblExerciseData "
                "("
                "exerciseWord, "
                "exercisePronunciation, "
                "exerciseTopDescpId, "
                "exerciseSubDescpId"
                ") "
                " VALUES("
                "'%s',"
                "'%s',"
                "%d,"
                "%d);",
                exerciseWord.toUtf8().constData(),
                exercisePronunciation.toUtf8().constData(),
                exerciseTopDescpId,
                exerciseSubDescpId);

    qDebug() << str;

    qry.prepare(str);

    if( !qry.exec() )
    {
        qDebug() << qry.lastError();

        if(dbIsHandledExternly ==false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    qry.finish();
    if(dbIsHandledExternly ==false)
    {
        closeDb();
        m_mutex.unlock();
    }
    return true;
}





/****************************************************************
 *
 * Function:    findExerciseListId()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::findSubExerciseDataId(QString subExerciseName,
                               int topExerciseDescpId,
                               int &subExerciseDescpId,
                               bool dbIsHandledExternly)
{
    QSqlRecord rec;
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);



     str.sprintf(" SELECT TblExerciseTopDescp.exerciseTopDescp, TblExerciseTopDescp.exerciseTopDescpId, "
                 " TblExerciseSubDescp.exerciseSubDescp, TblExerciseSubDescp.exerciseSubDescpId AS id, "
                 " TblExerciseSubDescp.exerciseTopDescpId "
                 " FROM TblExerciseTopDescp, TblExerciseSubDescp "
                 " WHERE TblExerciseTopDescp.exerciseTopDescpId = TblExerciseSubDescp.exerciseTopDescpId "
                 " AND LOWER(TblExerciseSubDescp.exerciseSubDescp) = LOWER('%s') AND "
                 " TblExerciseSubDescp.exerciseTopDescpId = %d;",
                 subExerciseName.toUtf8().constData(),
                 topExerciseDescpId);

    qDebug() << str;

    qry.prepare(str);

    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toUtf8().constData());
        }
        qDebug() << qry.lastError();
        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }
    else
    {
        if(qry.next())
        {
            rec = qry.record();


            if(rec.value("id").isNull() == true)
            {
                qry.finish();
                if(dbIsHandledExternly == false)
                {
                    closeDb();
                    m_mutex.unlock();
                }
                return false;
            }
            else
            {
                subExerciseDescpId = rec.value("id").toInt();
            }

        }
    }

    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }
    return true;
}





/****************************************************************
 *
 * Function:    ()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::insertSubExerciseListData(QString subExerciseName,
                                   int exerciseTopDescpId,
                                   bool dbIsHandledExternly)
{
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

    str.sprintf("INSERT INTO TblExerciseSubDescp (exerciseSubDescp, exerciseTopDescpId) "
                " SELECT  '%s', %d "
                " WHERE NOT EXISTS (SELECT 1 FROM TblExerciseTopDescp "
                " WHERE exerciseTopDescpId = %d AND LOWER(exerciseTopDescp) = LOWER('%s'));"
                , subExerciseName.toUtf8().constData()
                , exerciseTopDescpId
                , exerciseTopDescpId
                , subExerciseName.toUtf8().constData());

    qry.prepare(str);


    qDebug() << str << "\n";
    qry.prepare(str);

    if( !qry.exec() )
    {
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;


}



/****************************************************************
 *
 * Function:    ()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::insertTopExerciseListData(QString topExerciseName, bool dbIsHandledExternly)
{
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

    str.sprintf("INSERT INTO TblExerciseTopDescp (exerciseTopDescp) "
                "SELECT  '%s' "
                "WHERE NOT EXISTS (SELECT 1 FROM TblExerciseTopDescp WHERE LOWER(exerciseTopDescp) = LOWER('%s'));"
                , topExerciseName.toUtf8().constData()
                , topExerciseName.toUtf8().constData());

    qry.prepare(str);


    qDebug() << str << "\n";
    qry.prepare(str);

    if( !qry.exec() )
    {
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }


    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}


/****************************************************************
 *
 * Function:    findExerciseListId()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::findTopExerciseDataId(QString topExerciseName, int &topExerciseDescpId, bool dbIsHandledExternly)
{
    QSqlRecord rec;
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

     str.sprintf(" SELECT exerciseTopDescp, exerciseTopDescpId "
                 " FROM TblExerciseTopDescp "
                 " WHERE LOWER(exerciseTopDescp) = LOWER('%s');",
                 topExerciseName.toUtf8().constData());

    qDebug() << str;

    qry.prepare(str);

    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toUtf8().constData());
        }
        qDebug() << qry.lastError();
        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }
    else
    {
        if(qry.next())
        {
            rec = qry.record();


            if(rec.value("exerciseTopDescp").isNull() == true)
            {
                qry.finish();
                if(dbIsHandledExternly == false)
                {
                    closeDb();
                    m_mutex.unlock();
                }
                return false;
            }
            else
            {
                topExerciseDescpId = rec.value("exerciseTopDescpId").toInt();
            }

        }
    }

    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }
    return true;
}



/****************************************************************
 *
 * Function:    findExerciseListId()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::findExerciseListId(QString exerciseListName, int &exerciseListId, bool dbIsHandledExternly)
{
    QSqlRecord rec;
    QString str;

    if(dbIsHandledExternly == false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);

    str.sprintf("SELECT exerciseListId, exerciseListName "
                " FROM TblExerciseList"
                " WHERE exerciseListName = '%s';",
                exerciseListName.toUtf8().constData());

    qry.prepare(str);

    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toUtf8().constData());
        }
        qDebug() << qry.lastError();
        if(dbIsHandledExternly == false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }
    else
    {
        if(qry.next())
        {
            rec = qry.record();

            if(rec.value("exerciseListId").isNull()==true)
            {
                qry.finish();
                if(dbIsHandledExternly == false)
                {
                    closeDb();
                    m_mutex.unlock();
                }
                return false;
            }
            else
            {
                exerciseListId = rec.value("exerciseListId").toInt();
            }

        }
    }

    qry.finish();
    if(dbIsHandledExternly == false)
    {
        closeDb();
        m_mutex.unlock();
    }
    return true;
}




/****************************************************************
 *
 * Function:    findExerciseWithNoSound()
 *
 * Description: This combobox has a checkbox
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::findExerciseWithNoSound(void)
{
    QSqlRecord rec;
    QString str;
    bool found1 = false;
    bool found2 = false;
    bool found3 = false;
    bool found4 = false;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);
    QFile file("wordWithNoSound.txt");

    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QString errStr = QString("Fail to open file: wordWithNoSound.txt");
        QMessageBox::critical(NULL, QString::fromUtf8("Fail to open file"), errStr);
        return false;
    }

    QTextStream out(&file);


    str.sprintf("SELECT exerciseWord, exercisePronunciation, exerciseSoundPath, exerciseTopDescpId, exerciseSubDescpId "
                " FROM TblExerciseData "
                " WHERE (exerciseSoundPath IS NULL OR exerciseSoundPath = '' ) "
                " ORDER BY  LOWER(exerciseWord) ASC;");

    qry.prepare(str);


    qDebug() << str;


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QObject::tr("db error"), qry.lastError().text().toUtf8().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("exerciseWord").isNull()==true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                found1 = true;
            }

            if(rec.value("exercisePronunciation").isNull()==true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                found2 = true;
            }



            if(rec.value("exerciseTopDescpId").isNull()==true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                found3 = true;
            }


            if(rec.value("exerciseSubDescpId").isNull()==true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                found4 = true;
            }


            if(found1 == true && found2 == true && found3 == true && found4 == true)
            {
                found1 = false;
                found2 = false;
                found3 = false;
                found4 = false;

                out << (QString) rec.value("exerciseWord").toString().toUtf8() << endl;
                // out << (QString) rec.value("exercisePronunciation").toString().toUtf8() << endl;
                // out << (QString) rec.value("exerciseTopDescpId").toString().toUtf8() << endl;
                // out << (QString) rec.value("exerciseSubDescpId").toString().toUtf8() << endl << endl;

            }
        }
    }

    qry.finish();
    closeDb();
    m_mutex.unlock();
    file.close();
    return true;
}






/****************************************************************
 *
 * Function:    addExerciseDataToTreeWidget()
 *
 * Description: This combobox has a checkbox
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::addExerciseDataToTreeWidget(QTreeWidget *treeWidget,
                                                int exerciseListId,
                                                CDbHndl::SortSymbolNameTreeWidget_ET sortOn,
                                                char *sortOrder)
{
    QSqlRecord rec;
    QString str;
    bool found1 = false;
    bool found2 = false;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);



    if(sortOn == SORT_TWIDGET_SYMBOL)
    {
        str.sprintf("SELECT exerciseNameId, exerciseWord, exercisePronunciation "
                    " FROM TblExerciseName"
                    " WHERE exerciseListId = '%d' "
                    " ORDER BY LOWER(exerciseWord) %s;",
                    exerciseListId,
                    sortOrder);
    }
    else
    {
        str.sprintf("SELECT exerciseNameId, exerciseWord, exercisePronunciation "
                    " FROM TblExerciseName"
                    " WHERE exerciseListId = '%d' "
                    " ORDER BY LOWER(exercisePronunciation) %s;",
                    exerciseListId,
                    sortOrder);
    }

    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QObject::tr("db error"), qry.lastError().text().toUtf8().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("exerciseWord").isNull()==true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                found1 = true;
            }

            if(rec.value("exercisePronunciation").isNull()==true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                found2 = true;
            }

            if(found1 == true && found2 == true)
            {
                found1 = false;
                found2 = false;

                QTreeWidgetItem *item = new QTreeWidgetItem;
                item->setText(0, (QString)rec.value("exerciseWord").toString().toUtf8());
                item->setText(1, (QString)rec.value("exercisePronunciation").toString().toUtf8());
                item->setExpanded(true);
                               item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
                               item->setCheckState(2, Qt::Unchecked);
                               item->setText(2, " ");
                treeWidget->addTopLevelItem(item);
            }
        }
    }

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}



/****************************************************************
 *
 * Function:    addExerciseDataToTreeWidget()
 *
 * Description: This combobox has a checkbox
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::addExerciseDataToTreeWidget(QTreeWidget *treeWidget,
                                          QVector<CDbHndl::exerciseData_ST> &exerciseDataArr,
                                          int exerciseListId)
{
    QSqlRecord rec;
    QString str;
    CDbHndl::exerciseData_ST data;
    bool found1 = false;
    bool found2 = false;
    int i = 0;

    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    str.sprintf("SELECT DISTINCT exerciseNameId, exerciseWord, exercisePronunciation "
                " FROM TblExerciseName"
                " WHERE exerciseListId = '%d' "
                " ORDER BY Random();",
                exerciseListId);


    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QObject::tr("db error"), qry.lastError().text().toUtf8().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        exerciseDataArr.clear();

        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("exerciseWord").isNull()==true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                found1 = true;
            }

            if(rec.value("exercisePronunciation").isNull()==true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                found2 = true;
            }

            if(found1 == true && found2 == true)
            {
                found1 = false;
                found2 = false;

                QTreeWidgetItem *item = new QTreeWidgetItem;
                item->setText(0, (QString)rec.value("exerciseWord").toString().toUtf8());
                item->setText(1, (QString)rec.value("exercisePronunciation").toString().toUtf8());
                item->setExpanded(true);
                               item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
                               item->setCheckState(2, Qt::Unchecked);
                               item->setCheckState(3, Qt::Unchecked);
                               item->setText(2, " ");
                               item->setText(3, " ");
                treeWidget->addTopLevelItem(item);

                data.exerciseWord = (QString)rec.value("exerciseWord").toString().toUtf8();
                data.exercisePronunciation = (QString)rec.value("exercisePronunciation").toString().toUtf8();
                exerciseDataArr.insert(i, data);
                i++;
            }
        }
    }

    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}





/****************************************************************
 *
 * Function:    addExerciseListsToComboBox()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::addExerciseListsToComboBox(QComboBox *comboBox)
{
    QSqlRecord rec;
    QString str;
    int index;


    m_mutex.lock();
    openDb(PATH_JACK_STOCK_DB);
    QSqlQuery qry(m_db);


    str.sprintf("SELECT exerciseListName "
                " FROM TblExerciseList"
                " ORDER BY LOWER(exerciseListName) %s;",
                SQL_STR_ASC);


    qry.prepare(str);


    if( !qry.exec() )
    {
        if(m_disableMsgBoxes == false)
        {
            QMessageBox::critical(NULL, QString::fromUtf8("db error"), qry.lastError().text().toUtf8().constData());
        }
        qDebug() << qry.lastError();
        closeDb();
        m_mutex.unlock();
        return false;
    }
    else
    {
        index = comboBox->currentIndex();
        comboBox->clear();
        qDebug( ) << "Clear combobox";
        while(qry.next())
        {
            rec = qry.record();

            if(rec.value("exerciseListName").isNull()==true)
            {
                qry.finish();
                closeDb();
                m_mutex.unlock();
                return false;
            }
            else
            {
                comboBox->addItem((QString)rec.value("exerciseListName").toString().toUtf8());
                qDebug( ) << rec.value("exerciseListName").toString().toUtf8();
            }
        }
    }

    comboBox->setCurrentIndex(index);
    qry.finish();
    closeDb();
    m_mutex.unlock();
    return true;
}






/****************************************************************
 *
 * Function:    addExerciseName()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CDbHndl::addExerciseName(QString exerciseWord,
                         QString exercisePronunciation,
                         int exerciseListId,
                         bool dbIsHandledExternly)
{
    QString str;


    if(dbIsHandledExternly ==false)
    {
        m_mutex.lock();
        openDb(PATH_JACK_STOCK_DB);
    }

    QSqlQuery qry(m_db);


    str.sprintf("INSERT INTO TblExerciseName "
                "( "
                "exerciseWord, "
                "exercisePronunciation, "
                "exerciseListId "
                ") "
                "VALUES( "
                "'%s',"
                "'%s',"
                "%d);",
                exerciseWord.toUtf8().constData(),
                exercisePronunciation.toUtf8().constData(),
                exerciseListId);

    qry.prepare(str);

    if( !qry.exec() )
    {
        qDebug() << qry.lastError();

        if(dbIsHandledExternly ==false)
        {
            closeDb();
            m_mutex.unlock();
        }
        return false;
    }


    qry.finish();
    if(dbIsHandledExternly ==false)
    {
        closeDb();
        m_mutex.unlock();
    }
    return true;
}


