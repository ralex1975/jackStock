#ifndef DEBUG_H
#define DEBUG_H

#include <QFile>

    void debugInit(void);
    void debugPrintToFile(QString debugStr, int line, QString fileName);


#endif // DEBUG_H
