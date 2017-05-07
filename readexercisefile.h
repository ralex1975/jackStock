#ifndef READEXERCISEFILE_H
#define READEXERCISEFILE_H
#include <QtCore>
#include <QString>
#include "common.h"
#include "dbHndl.h"

struct ExerciseTopSubData_ST
{
    QString topLevelName;
    QString subLevelName;
    QString filename;
};


class readExerciseFile
{
private:
    CDbHndl m_db;

public:
     QVector<ExerciseTopSubData_ST> m_exerciseTopSubArr;
    readExerciseFile();
    bool readTopExerciseFile(void);
    bool readExerciseDataFile(QString filename, int exerciseTopDescpId, int exerciseSubDescpId);


};

#endif // READEXERCISEFILE_H
