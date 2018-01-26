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

const int STD_STR_LEN = 40;
const int MAX_STR_LEN = 60;
const int NOT_AT_FILE_END = 0;
const char READ_ONLY_FLAG[] = "r";
const char NULL_CHAR = '\0';
const char COLON = ':';
const char SPACE = ' ';
const char NEWLINE = '\n';

typedef int Boolean;
Boolean True = 1;
Boolean False = 0;

void copyString( char *target, char *original );
int compareString( char *string1, char *string2 );
int stringToInt( char *string );

#endif
