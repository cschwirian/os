/**
  * StandardFunctions.c
  *
  * Created by Connor Schwirian on 17 January 2018
  *
  * Contains disallowed standard functions that I have recreated
  *
  * Requires Utilities.h
  *
  * Version 1.0 17/1/2018
**/

#ifndef Utilities_C
#define Utilities_C

#include "Utilities.h"

const char READ_ONLY_FLAG[] = "r";
const char NULL_CHAR = '\0';
const char SEMICOLON = ';';
const char SPACE = ' ';

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
