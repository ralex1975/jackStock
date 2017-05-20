#include "readexercisefile.h"
#include <QMessageBox>

/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
readExerciseFile::readExerciseFile()
{
}



/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
bool readExerciseFile::readTopExerciseFile(void)
{
     QString filename = PATH_EXERCICE_FILE;
     QFile file(filename);
     QString str;
     QString exerciseFile;
     ExerciseTopSubData_ST data;
    int topExerciseDescpId;
    int subExerciseDescpId;



    m_db.delAllExerciseTopData();
    m_db.delAllExerciseSubData();
    m_db.delAllExerciseData();

     if(!file.open(QFile::ReadOnly | QFile::Text))
     {
         QString errStr = QString("Fail to open file: %1").arg(filename);
         QMessageBox::critical(NULL, QString::fromUtf8("Fail to open file"), errStr);
         return false;
     }

     QTextStream inStream(&file);

     while(!inStream.atEnd())
     {
         str = inStream.readLine();
         qDebug() << "str" << str;

         if(str.indexOf("|") != -1)
         {
             data.topLevelName = str.left(str.indexOf("|"));
             qDebug() << "top" << data.topLevelName;
             m_db.insertTopExerciseListData(data.topLevelName);
             str = str.mid((str.indexOf("|")+1), str.size());


            if(true == m_db.findTopExerciseDataId(data.topLevelName, topExerciseDescpId))
                qDebug() << "topExerciseDescpId" << topExerciseDescpId << data.topLevelName;
            else
                qDebug() << "failed topExerciseDescpId";
         }


         if(str.indexOf("|") != -1)
         {
             data.subLevelName = str.left(str.indexOf("|"));
             qDebug() << "sub" << data.subLevelName;

             m_db.insertSubExerciseListData(data.subLevelName, topExerciseDescpId);

             m_db.findSubExerciseDataId(data.subLevelName,
                                        topExerciseDescpId,
                                        subExerciseDescpId);

             qDebug() << "sub" << data.subLevelName << "topId" << topExerciseDescpId << "subId" << subExerciseDescpId;


         }


         if(str.indexOf("|") != -1)
         {
             exerciseFile = qApp->applicationDirPath();
             exerciseFile += "/";
             exerciseFile += PATH_EXERCICE_FILES;
             exerciseFile += str.right(str.length() - str.indexOf("|")-1);
             exerciseFile = exerciseFile.trimmed();

             qDebug() << "file" << exerciseFile << "\n";


             if(false == readExerciseDataFile(exerciseFile, topExerciseDescpId, subExerciseDescpId))
             {
                 str.sprintf("Kan inte lägga %s", exerciseFile.toLocal8Bit().constData());
                 QMessageBox::critical(NULL, QString::fromUtf8("Lägga till exercise list"), str.toLocal8Bit().constData());
                 return false;
             }
          }
     }

     qDebug() << "close file\n";
     file.close();
     return true;

}


/*******************************************************************
 *
 * Function:    openExerciseFile()
 *
 * Description:
 *
 *
 *******************************************************************/
bool readExerciseFile::readExerciseDataFile(QString filename,
                                            int exerciseTopDescpId,
                                            int exerciseSubDescpId)
{
    QFile file(filename);
    QString str;
    QString exerciseWord;
    QString exercisePronunciation;



    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QString errStr = QString("Fail to open file: %1").arg(filename);
        QMessageBox::critical(NULL, QString::fromUtf8("Fail to open file"), errStr);
        return false;
    }

    m_db.openDb(PATH_JACK_STOCK_DB, true);


    QTextStream inStream(&file);
    // inStream.setCodec("ISO-8859-15"); // ajn 141123

    while(!inStream.atEnd())
    {
        str = inStream.readLine();


        if(str.indexOf("|") != -1)
        {
            exerciseWord = str.left(str.indexOf("|"));
            qDebug() << exerciseWord;
        }

        if(str.indexOf("|") != -1)
        {
            exercisePronunciation = str.right(str.length() - str.indexOf("|")-1);
            qDebug() << exercisePronunciation;
        }

        exerciseWord = exerciseWord.trimmed();
        exercisePronunciation = exercisePronunciation.trimmed();
        m_db.addExerciseData(exerciseWord, exercisePronunciation,
                                exerciseTopDescpId, exerciseSubDescpId, true);

    }

    qDebug() << "close file\n";
    file.close();
    m_db.closeDb(true);
    return true;
}

