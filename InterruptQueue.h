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

typedef struct Interrupt
{
    int processNum;
    char time[ 10 ];
    struct Interrupt next;
} Interrupt;

void addInterrupt( int processNum, char *time );
Interrupt checkForInterrupt( char *currentTime );
