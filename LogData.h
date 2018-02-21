

#ifndef LogData_H
#define LogData_H

#include "Utilities.h"

const int LINE_LENGTH = 80;

typedef struct LogData
{
    char logLine[ LINE_LENGTH ];
    LogData next;
} LogData;

void addLine( LogData *data, char *line );
void clearData( LogData *data );
void logToFile( LogData *data, char *fileName );

#endif
