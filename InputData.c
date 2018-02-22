/**
  * InputData.c
  *
  * Created by -REDACTED- on 17 January 2018
  *
  * Simulator driver file
  *
  * Version 1.0 31/1/2018
  *
  * Version 0.1 (Development) 17/1/2018
**/

#ifndef InputData_C
#define InputData_C

#include "InputData.h"

int main( int argc, char *argv[] )
{
    int configMessage, metaDataMessage, processMessage;

    ConfigDictionary *config = malloc( sizeof( ConfigDictionary ) );

    configMessage = getConfig(config, argv[ 1 ] );

    MetaDataNode *metaData = NULL;
    metaDataMessage = UNKNOWN_ERROR;

    if( configMessage == NO_ERROR_MSG )
    {
        metaDataMessage = getMetaData( &metaData, config->filePath,
                    config->logInstruction, config->logFilePath );
    }

    ProcessList *pList = NULL;
    processMessage = UNKNOWN_ERROR;

    if( metaDataMessage == NO_ERROR_MSG )
    {
        processMessage = runProcesses( pList, config, metaData );
    }

    free( config );
    config = NULL;

    clearList( metaData );

    clearProcessList( pList );
}

#endif
