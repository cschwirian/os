/**
  * LogData.h
  *
  * Created by -REDACTED- on 12 February 2018
  *
  * Takes in and logs relevant information
  *
  * Required by LogData.c
  *
  * Version 1.0 26/2/2018
  *
  * Version 0.1 (Development) 12/2/2018
**/

#ifndef LogData_H
#define LogData_H

#include "Utilities.h"

typedef struct LogData
{
    char logLine[ 81 ];     // 81 serves as the logfile line length plus \0.
    struct LogData *next;
} LogData;

void addLine( LogData *data, char *line );
LogData *clearData( LogData *data );

void followLogInstruction( LogData *data,
                           Boolean logToFile,
                           Boolean logToMonitor,
                           char *logBuffer );

void logDataToFile( LogData *data, char *fileName );

#endif
