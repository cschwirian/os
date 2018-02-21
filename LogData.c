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

void logToFile( LogData *data, char *fileName )
{
    FILE *filePointer = fopen( fileName, WRITE_ONLY_FLAG );

    LogData *current = data;

    while( current != NULL )
    {
        fprintf( filePointer, "%s\n", current->logLine );
    }

    fclose( filePointer );
}

#endif
