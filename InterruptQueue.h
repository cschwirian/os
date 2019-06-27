/**
  * InterruptQueue.h
  *
  * Created by -REDACTED- on 19 April 2018
  *
  * Holds and processes interrupts
  *
  * Required by InterruptQueue.c
  *
**/

#ifndef InterruptQueue_H
#define InterruptQueue_H

#include "Utilities.h"

typedef struct Interrupt
{
    int processNum;
    float endTime;
    char ioType[ 30 ];
    char operation[ 30 ];
    struct Interrupt *next;
} Interrupt;

Interrupt *addInterrupt( int processNum, float endTime,
                   char *ioType, char *operation, Interrupt *queue );

Interrupt *checkForInterrupt( float currentTime, Interrupt **queue );

#endif
