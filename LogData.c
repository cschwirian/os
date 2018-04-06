/**
  * LogData.c
  *
  * Created by -REDACTED- on 12 February 2018
  *
  * Takes in and logs relevant information
  *
  * Requires LogData.h
  *
  * Version 1.0 26/2/2018
  *
  * Version 0.1 (Development) 12/2/2018
**/

#ifndef LogData_C
#define LogData_C

#include "LogData.h"

void addLine( LogData *data, char *line )
{
    if( data == NULL )
    {
        data = (LogData *)malloc( sizeof( LogData ) );
        copyString( data->logLine, line );
        data->next = NULL;
    }
    else
    {
        LogData *current = data;
        while( current->next != NULL )
        {
            current = current->next;
        }

        current->next = (LogData *)malloc( sizeof( LogData ) );
        current = current->next;

        copyString( current->logLine, line );
        current->next = NULL;
    }
}

LogData *clearData( LogData *data )
{
    if( data == NULL )
    {
        return NULL;
    }

    if( data->next != NULL )
    {
        clearData( data->next );
    }

    free( data );

    data = NULL;

    return data;
}

void followLogInstruction( LogData *data,
                           Boolean logToFile,
                           Boolean logToMonitor,
                           char *logBuffer )
{
    if( logToMonitor == True )
    {
        printf( "%s", logBuffer );
    }
    if( logToFile == True )
    {
        addLine( data, logBuffer );
    }
}

void logDataToFile( LogData *data, char *fileName )
{
    FILE *filePointer = fopen( fileName, WRITE_ONLY_FLAG );

    LogData *current = data;

    while( current != NULL )
    {
        fprintf( filePointer, "%s", current->logLine );

        current = current->next;
    }

    fclose( filePointer );
}

#endif
