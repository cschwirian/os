#ifndef LogData_H
#define LogData_H

#include "Utilities.h"

typedef struct LogData
{
    char logLine[ 80 ];     // 80 serves as the logfile line length.
    struct LogData *next;
} LogData;

void addLine( LogData *data, char *line );
LogData *clearData( LogData *data );
void logToFile( LogData *data, char *fileName );

#endif
