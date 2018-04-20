/**
  * InterruptQueue.c
  *
  * Created by -REDACTED- on 19 April 2018
  *
  * Holds and processes interrupts
  *
  * Requires InterruptQueue.h
  *
**/

#ifndef InterruptQueue_C
#define InterruptQueue_C

#include "InterruptQueue.h"

Interrupt *addInterrupt( int processNum, char *endTime,
                   char *ioType, char *operation, Interrupt *queue )
{
    Interrupt *currentInterrupt, *temp;

    if( queue == NULL )
    {
        queue = malloc( sizeof( Interrupt ) );
        queue->processNum = processNum;
        copyString( queue->endTime, endTime );
        copyString( queue->ioType, ioType );
        copyString( queue->operation, operation );
        queue->next = NULL;

        return queue;
    }
    else if( compareString( time, (char *)queue->endTime ) < 0 )
    {
        currentInterrupt = queue;
        queue = malloc( sizeof( Interrupt ) );
        queue->processNum = processNum;
        copyString( queue->endTime, endTime );
        copyString( queue->ioType, ioType );
        copyString( queue->operation, operation );
        queue->next = currentInterrupt;

        return queue;
    }

    currentInterrupt = queue;
    while( currentInterrupt->next != NULL )
    {
        if( compareString( time, (char *)currentInterrupt->next->endTime) < 0 )
        {
            temp = currentInterrupt->next;
            currentInterrupt->next = malloc( sizeof( Interrupt ) );
            currentInterrupt->next->processNum = processNum;
            copyString( currentInterrupt->next->endTime, endTime );
            copyString( currentInterrupt->next->ioType, ioType );
            copyString( currentInterrupt->next->operation, operation );
            currentInterrupt->next->next = temp;

            return queue;
        }

        currentInterrupt = currentInterrupt->next;
    }

    currentInterrupt->next = malloc( sizeof( Interrupt ) );
    currentInterrupt->next->processNum = processNum;
    copyString( currentInterrupt->next->endTime, endTime );
    copyString( currentInterrupt->next->ioType, ioType );
    copyString( currentInterrupt->next->operation, operation );
    currentInterrupt->next->next = NULL;

    return queue;
}

Interrupt *checkForInterrupt( char *currentTime, Interrupt *queue )
{
    Interrupt *returnInter = NULL;

    if( compareString( currentTime, queue->endTime ) >= 0 )
    {
        returnInter = queue;

        queue = queue->next;
    }

    return returnInter;
}

#endif
