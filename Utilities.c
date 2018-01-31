/**
  * StandardFunctions.c
  *
  * Created by -REDACTED- on 17 January 2018
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

const int STD_STR_LEN = 40;
const int MAX_STR_LEN = 60;
const int NOT_AT_FILE_END = 0;
const char READ_ONLY_FLAG[] = "r";
const char WRITE_ONLY_FLAG[] = "w";
const char NULL_CHAR = '\0';
const char SEMICOLON = ';';
const char OPEN_PAREN = '(';
const char CLOSE_PAREN = ')';
const char COLON = ':';
const char SPACE = ' ';
const char NEWLINE = '\n';

void copyString( char *target, char *original )
{
    int stringIndex = 0;

    while( original[ stringIndex ] )
    {
        target[ stringIndex ] = original[ stringIndex ];

        stringIndex++;
    }

    target[ stringIndex ] = '\0';
}

int compareString( char *string1, char *string2 )
{
    while( *string1 == *string2 )
    {
        if( *string1 == '\0' && *string2 == '\0' )
        {
            return 0;
        }
        string1++;
        string2++;
    }

    return (int)( string1 ) - (int)( string2 );
}

int stringToInt( char *string )
{
    int returnVal = 0;
    int strIndex = 0;

    while( string[ strIndex ] != '\0' && string[ strIndex ] != '.' )
    {
        returnVal = returnVal * 10 + string[ strIndex ] - '0';

        strIndex++;
    }

    return returnVal;
}

#endif
