/**
  * InputData.c
  *
  * Created by -REDACTED- on 17 January 2018
  *
  *
  *
  * Version 0.1 (Development) 17/1/2018
**/

#ifndef InputData_C
#define InputData_C

#include "InputData.h"

int main( int argc, char *argv[] )
{
    ConfigDictionary *config = malloc( sizeof( ConfigDictionary ) );

    getConfig(config, argv[ 1 ] );

    MetaDataNode *metaData = malloc( sizeof( MetaDataNode ) );

    printf( "%d", getMetaData( metaData, config->filePath, config->logFilePath ) );

    clearList( metaData );

    free( config );
    config = NULL;
}

#endif
