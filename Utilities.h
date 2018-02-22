/**
  * StandardFunctions.h
  *
  * Created by -REDACTED- on 17 January 2018
  *
  * Contains disallowed standard functions that I have recreated with useful
  * constants. Additionally takes care of boilerplate inclusions.
  *
  * Required by Utilities.c
  *
  * Version 1.0 31/1/2018
  *
  * Version 0.1 17/1/2018
**/

#ifndef Utilities_H
#define Utilities_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

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
const char APPEND[ 1 ];
const char NULL_CHAR;
const char SEMICOLON;
const char OPEN_PAREN;
const char CLOSE_PAREN;
const char COLON;
const char SPACE ;
const char NEWLINE;
const char PERIOD;

enum ERROR_MESSAGE_CODES
{
    UNKNOWN_ERROR = -1,
    NO_ERROR_MSG = 0,
    INIT_ERROR = 1,
    DATA_ORDER_ERROR = 2,
    DATA_FORMAT_ERROR = 3,
    DATA_ERROR = 4,
    INCOMPLETE_DATA_ERROR = 5,
    FILE_END_ERROR = 6,
    FILE_NOT_FOUND = 7
};

enum PROCESS_ERROR_CODES
{
    NO_PROCESS_ERROR = 10,
    NO_SYSTEM_START = 11,
    NO_SYSTEM_END = 12,
    MISSING_PROCESS_END = 13,
    PROCESS_FORMAT_ERROR = 14,
    PROCESS_MEMORY_ERROR = 15,
    EMPTY_PROGRAM_ERROR = 16,
    UNKNOWN_CONFIGURATION_ERROR = 17
};

enum PCB_STATES
{
    NEW_STATE = 0,
    READY_STATE = 1,
    RUNNING_STATE = 2,
    EXIT_STATE = 3
};

void copyString( char *target, char *original );
int compareString( char *string1, char *string2 );
int stringToInt( char *string );

#endif
