/**
  * StandardFunctions.c
  *
  * Created by Connor Schwirian on 17 January 2018
  *
  * Contains disallowed standard functions that I have recreated
  *
  * Requires StandardFunctions.h
  *
  * Version 1.0 17/1/2018
**/

#ifndef StandardFunctions_C
#define StandardFunctions_C

#include "StandardFunctions.h"

const int TRUE = 1;
const int FALSE = 0;

void copyString( char *target, char *original )
{
    int stringIndex = 0;

    while( stringIndex <= sizeof( original ) )
    {
        target[ stringIndex ] = original[ stringIndex ];

        stringIndex++;
    }
}

#endif
