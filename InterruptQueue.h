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

#ifndef INTERRUPT_H
#define INTERRUPT_H

typedef struct Interrupt
{
    int processNum;
    char endTime[ 10 ];
    struct Interrupt *next;
} Interrupt;

void addInterrupt( int processNum, char *endTime, Interrupt *queue );
Interrupt checkForInterrupt( char *currentTime, Interrupt *queue );

#endif INTERRUPT_H
