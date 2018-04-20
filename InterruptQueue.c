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

#ifndef INTERRUPT_C
#define INTERRUPT_C

void addInterrupt( int processNum, char *endTime, Interrupt *queue )
{
    Interrupt *currentInterrupt, *temp;

    if( queue == NULL )
    {
        queue = malloc( sizeof( Interrupt ) );
        queue->processNum = processNum;
        queue->endTime = endTime;
        queue->next = NULL;

        return queue;
    }
    else if( compareString( time, queue->time ) < 0 )
    {
        currentInterrupt = queue;
        queue = malloc( sizeof( Interrupt ) );
        queue->processNum = processNum;
        queue->endTime = endTime;
        queue->next = currentInterrupt;

        return queue;
    }

    currentInterrupt = queue;
    while( currentInterrupt->next != NULL )
    {
        if( compareString( time, currentInterrupt->next->time) < 0 )
        {
            temp = currentInterrupt->next;
            currentInterrupt->next = malloc( sizeof( Interrupt ) );
            currentInterrupt->next->processNum = processNum;
            currentInterrupt->next->endTime = endTime;
            currentInterrupt->next->next = temp;

            return queue;
        }

        currentInterrupt = currentInterrupt->next;
    }

    currentInterrupt->next = malloc( sizeof( Interrupt ) );
    currentInterrupt->next->processNum = processNum;
    currentInterrupt->next->endTime = endTime;
    currentInterrupt->next->next = NULL;

    return queue;
}

Interrupt checkForInterrupt( char *currentTime, Interrupt *queue )
{
    Interrupt returnInter = NULL;

    if( compareString( currentTime, queue->endTime ) >= 0 )
    {
        returnInter = queue;

        queue = queue->next;
    }

    return returnInter;
}

#endif INTERRUPT_C
