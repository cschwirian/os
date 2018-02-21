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


    printf( "Parsing Operating System Data...\n" );

    ConfigDictionary *config = malloc( sizeof( ConfigDictionary ) );

    configMessage = getConfig(config, argv[ 1 ] );

    printf( "Configuration Data Parsed...\n" );

    MetaDataNode *metaData = NULL;
    metaDataMessage = UNKNOWN_ERROR;

    if( configMessage == NO_ERROR_MSG )
    {
        metaDataMessage = getMetaData( &metaData, config->filePath,
                    config->logInstruction, config->logFilePath );
    }

    printf( "Metadata Parsed...\n" );

    printf( "Total Program Runtime: %d ms\n", getTotalRuntime( metaData, config ) );

    ProcessList *pList = NULL;
    processMessage = UNKNOWN_ERROR;

    if( metaDataMessage == NO_ERROR_MSG )
    {
        processMessage = populateList( &pList, metaData );
    }

    printf( "Process List Populated...\n" );

    if( processMessage == NO_PROCESS_ERROR )
    {
        runProcesses( pList, config );
    }

    free( config );
    config = NULL;

    clearList( metaData );

    clearProcessList( pList );
}

#endif
