/**
  * StandardFunctions.h
  *
  * Created by Connor Schwirian on 17 January 2018
  *
  * Contains disallowed standard functions that I have recreated with useful
  * constants. Additionally takes care of boilerplate inclusions.
  *
  * Required by Utilities.c
  *
  * Version 1.0 17/1/2018
**/

#ifndef Utilities_H
#define Utilities_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int Boolean;
enum
{
    False,
    True
};

const int STD_STR_LEN;
const int MAX_STR_LEN;
const int NOT_AT_FILE_END;
const char READ_ONLY_FLAG[ 1 ];
const char WRITE_ONLY_FLAG[ 1 ];
const char NULL_CHAR;
const char SEMICOLON;
const char OPEN_PAREN;
const char CLOSE_PAREN;
const char COLON;
const char SPACE ;
const char NEWLINE;

enum ERROR_MESSAGE_CODES
{
    UNKNOWN_ERROR = -1,
    NO_ERROR_MSG = 0,
    INIT_ERROR = 1,
    DATA_ORDER_ERROR = 2,
    DATA_FORMAT_ERROR = 3,
    INCOMPLETE_DATA_ERROR = 4,
    FILE_END_ERROR = 5,
    FILE_NOT_FOUND = 6
};

void copyString( char *target, char *original );
int compareString( char *string1, char *string2 );
int stringToInt( char *string );

#endif
