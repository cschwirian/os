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

// TODO: Ask about these.
#define TRUE 1
#define FALSE 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void copyString( char *target, char *original );

#endif
