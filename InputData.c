/**
  * InputData.c
  *
  * Created by Connor Schwirian on 17 January 2018
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

    int configMessageCode = getConfigFromFile(config, argv[ 1 ] );
    printf( "%d\n", configMessageCode );

    if( configMessageCode == 0 )
    {
        logConfigData( config, config->logFilePath );
    }

    MetaDataNode *metaData = malloc( sizeof( MetaDataNode ) );

    getMetaDataFromFile( metaData, argv[ 2 ] );

    free( metaData );
    metaData = NULL;

    free( config );
    config = NULL;
}

#endif
