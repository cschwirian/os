/**
  * InterruptQueue.c
  *
  * Created by -REDACTED- on 19 April 2018
  *
  * Holds and processes interrupts
  *
  * Requires InterruptQueue.h
  *
  * Version 1.0 20/4/2018
**/

#ifndef InterruptQueue_C
#define InterruptQueue_C

#include "InterruptQueue.h"

Interrupt *addInterrupt( int processNum, float endTime,
                   char *ioType, char *operation, Interrupt *queue )
{
    Interrupt *currentInterrupt, *temp;
    if( queue == NULL )
    {
        queue = malloc( sizeof( Interrupt ) );
        queue->processNum = processNum;
        // copyString( queue->endTime, endTime );
        // sprintf( queue->endTime, "%f", endTime );
        queue->endTime = endTime;
        copyString( queue->ioType, ioType );
        copyString( queue->operation, operation );
        queue->next = NULL;

        return queue;
    }
    else if( endTime < queue->endTime )
    {
        currentInterrupt = queue;
        queue = malloc( sizeof( Interrupt ) );
        queue->processNum = processNum;
        // copyString( queue->endTime, endTime );
        // sprintf( queue->endTime, "%f", endTime );
        queue->endTime = endTime;
        copyString( queue->ioType, ioType );
        copyString( queue->operation, operation );
        queue->next = currentInterrupt;

        return queue;
    }

    currentInterrupt = queue;
    while( currentInterrupt->next != NULL )
    {
        if(  endTime < currentInterrupt->next->endTime )
        {
            temp = currentInterrupt->next;
            currentInterrupt->next = malloc( sizeof( Interrupt ) );
            currentInterrupt->next->processNum = processNum;
            // copyString( currentInterrupt->next->endTime,  endTime );
            // sprintf( currentInterrupt->next->endTime, "%f", endTime );
            currentInterrupt->next->endTime = endTime;
            copyString( currentInterrupt->next->ioType, ioType );
            copyString( currentInterrupt->next->operation, operation );
            currentInterrupt->next->next = temp;

            return queue;
        }

        currentInterrupt = currentInterrupt->next;
    }

    currentInterrupt->next = malloc( sizeof( Interrupt ) );
    currentInterrupt->next->processNum = processNum;
    // copyString( currentInterrupt->next->endTime,  endTime );
    // sprintf( currentInterrupt->next->endTime, "%f", endTime );
    currentInterrupt->next->endTime = endTime;
    copyString( currentInterrupt->next->ioType, ioType );
    copyString( currentInterrupt->next->operation, operation );
    currentInterrupt->next->next = NULL;

    return queue;
}

Interrupt *checkForInterrupt( float currentTime, Interrupt **queue )
{
    Interrupt *returnInter;
    returnInter = NULL;

    if( *queue != NULL )
    {
        if( currentTime >= (*queue)->endTime )
        {
            returnInter = *queue;

            *queue = (*queue)->next;
        }
    }

    return returnInter;
}

void printInterrupts( Interrupt *queue )
{
    printf("interrupts:\n");
    while( queue != NULL )
    {
        printf("%d, ", queue->processNum);

        queue = queue->next;
    }

    printf("\n");
}

#endif
