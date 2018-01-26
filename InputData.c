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

    getConfigFromFile( config, argv[ 1 ] );

    printf( "%d\n", config->versionNumber );

    free( config );
    config = NULL;
}

#endif
