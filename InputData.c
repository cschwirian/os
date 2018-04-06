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
    int configMessage, metaDataMessage, processCode;

    ConfigDictionary *config = malloc( sizeof( ConfigDictionary ) );
    configMessage = UNKNOWN_ERROR;

    if( argc == 2 )
    {
        configMessage = getConfig(config, argv[ 1 ] );
    }

    MetaDataNode *metaData = NULL;
    metaDataMessage = UNKNOWN_ERROR;

    if( configMessage == NO_ERROR_MSG )
    {
        metaDataMessage = getMetaData( &metaData, config->filePath,
                    config->logInstruction, config->logFilePath );
    }

    ProcessList *pList = NULL;
    processCode = UNKNOWN_ERROR;

    if( metaDataMessage == NO_ERROR_MSG )
    {
        processCode = populateList( &pList, metaData, config );
    }

    if( processCode == NO_PROCESS_ERROR )
    {
        runProcesses( pList, metaData, config );
    }

    free( config );
    config = NULL;

<<<<<<< HEAD
    metaData = clearList( metaData );

    pList = clearProcessList( pList );
=======
    clearList( metaData );
    clearProcessList( pList );
>>>>>>> 1a02ff6baea7cd5a73b836ec5fb384d722705297
}

#endif
