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

void addInterrupt( int processNum, char *time, Interrupt queue )
{
    Interrupt currentInterrupt, temp;

    if( queue == NULL )
    {
        queue = malloc( sizeof( Interrupt ) );
        queue->processNum = processNum;
        queue->time = time;
        queue->next = NULL;

        return queue;
    }
    else if( compareString( time, queue->time ) < 0 )
    {
        currentInterrupt = queue;
        queue = malloc( sizeof( Interrupt ) );
        queue->processNum = processNum;
        queue->time = time;
        queue->next = currentInterrupt;

        return queue;
    }

    currentInterrupt = queue;
    while( currentInterrupt->next != NULL )
    {
        if( compareString( time, currentInterrupt->next->time) < 0 )
        {

        }
    }

    currentInterrupt->next = malloc( sizeof( Interrupt ) );
    currentInterrupt->next->processNum = processNum;
    currentInterrupt->next->time = time;
    currentInterrupt->next->next = NULL;

    return queue;
}

Interrupt checkForInterrupt( char *currentTime, Interrupt queue )
{

}
