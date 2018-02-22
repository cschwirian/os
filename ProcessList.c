

#ifndef ProcessList_C
#define ProcessList_C

#include "ProcessList.h"

int populateList( ProcessList **pList, MetaDataNode *data )
{
    Boolean inProgress = False;
    MetaDataNode *currentData, *tempNode;
    ProcessList *currentProcess;

    if( data->commandLetter != 'S' ||
        compareString( data->operation, "start") != 0 ||
        data->commandValue != 0 )
    {
        return NO_SYSTEM_START;
    }

    currentData = data->next;

    while( currentData != NULL )
    {
        //printf( "Current command: %c | commandValue = %d\n", currentData->commandLetter, currentData->commandValue );
        if( currentData->commandLetter == 'A' )
        {
            if( compareString( currentData->operation, "start") == 0 &&
                currentData->commandValue == 0 )
            {
                inProgress = True;

                currentData = currentData->next;

                currentProcess = (ProcessList *)malloc( sizeof( ProcessList ) );
                currentProcess->process = NULL;
                currentProcess->state = NEW_STATE;
                currentProcess->next = NULL;
            }
            else if( compareString( currentData->operation, "end") == 0 &&
                     currentData->commandValue == 0 )
            {
                if( inProgress == True )
                {
                    inProgress = False;
                }
                else
                {
                    clearList( currentProcess->process );
                    free( currentProcess );
                    return PROCESS_FORMAT_ERROR;
                }

                currentData = currentData->next;

                *pList = addProcess( *pList, currentProcess );

                free( currentProcess );
            }
        }
        else if( currentData->commandLetter == 'S' )
        {
            if( compareString( currentData->operation, "end") == 0 &&
                currentData->commandValue == 0 )
            {
                if( inProgress == True )
                {
                    clearProcess( currentProcess );
                    return PROCESS_FORMAT_ERROR;
                }
                else
                {
                    return NO_PROCESS_ERROR;
                }
            }
            else
            {
                return NO_SYSTEM_END;
            }
        }
        else if( inProgress == True )
        {
            tempNode = copyNode( currentData );

            currentProcess->process = addNode( currentProcess->process,
                                               tempNode );

            currentData = currentData->next;

            free( tempNode );
        }
        else
        {
            clearProcess( currentProcess );
            return UNKNOWN_ERROR;
        }
    }

    return PROCESS_FORMAT_ERROR;
}

int runProcesses( ProcessList *pList,
                  ConfigDictionary *config,
                  MetaDataNode *data )
{
    int processCount, processCode;
    int *timePointer;
    char timeString[ 10 ];
    MetaDataNode *process;
    pthread_t threadID;
    pthread_attr_t threadAttr;

    accessTimer( ZERO_TIMER, timeString );
    printf( "Time: %s, System Start\n", timeString );

    accessTimer( LAP_TIMER, timeString );
    printf( "Time: %s, OS: Begin PCB Creation\n", timeString );

    processCode = populateList( &pList, data );

    accessTimer( LAP_TIMER, timeString );
    printf( "Time: %s, OS: All processes initialized in New state\n", timeString );

    setProcessStates( pList, READY_STATE );

    accessTimer( LAP_TIMER, timeString );
    printf( "Time: %s, OS: All proceses now set in Ready state\n", timeString );

    if( processCode != NO_PROCESS_ERROR )
    {
        return processCode;
    }

    if( pList == NULL )
    {
        return EMPTY_PROGRAM_ERROR;
    }
    else if( config == NULL )
    {
        return UNKNOWN_CONFIGURATION_ERROR;
    }

    processCount = 0;

    while( pList != NULL )
    {

        accessTimer( LAP_TIMER, timeString );
        printf( "Time: %s, OS: %s strategy selects Process %d with time %d msec\n",
                timeString,
                config->schedulingCode,
                processCount,
                getTotalRuntime( pList->process, config ) );

        pList->state = RUNNING_STATE;

        accessTimer( LAP_TIMER, timeString );
        printf( "Time: %s, OS: Process %d set in Running state\n", timeString, processCount );

        process = pList->process;
        while( process != NULL )
        {
            accessTimer( LAP_TIMER, timeString );
            printf( "Time: %s, Process %d, ", timeString, processCount );

            if( process->commandLetter == 'I' )
            {
                printf( "%s input start\n", process->operation );

                timePointer = (int *)malloc( sizeof( int ) );
                *timePointer = process->commandValue;
                *timePointer = (*timePointer) * config->ioCycleTime;

                pthread_attr_init( &threadAttr );
                pthread_create( &threadID, &threadAttr, runInput, (void *)timePointer );
                pthread_join( threadID, NULL );

                free( timePointer );

                accessTimer( LAP_TIMER, timeString );
                printf( "Time: %s, Process %d, ", timeString, processCount );
                printf( "%s input end\n", process->operation );
            }
            else if( process->commandLetter == 'O' )
            {
                printf( "%s output start\n", process->operation );

                timePointer = (int *)malloc( sizeof( int ) );
                *timePointer = process->commandValue;
                *timePointer = (*timePointer) * config->ioCycleTime;

                pthread_attr_init( &threadAttr );
                pthread_create( &threadID, &threadAttr, runOutput, (void *)timePointer );
                pthread_join( threadID, NULL );

                free( timePointer );

                accessTimer( LAP_TIMER, timeString );
                printf( "Time: %s, Process %d, ", timeString, processCount );
                printf( "%s output end\n", process->operation );
            }
            else if( process->commandLetter == 'P' )
            {
                printf( "run operation start\n");

                timePointer = (int *)malloc( sizeof( int ) );
                *timePointer = process->commandValue;
                *timePointer = (*timePointer) * config->processorCycleTime;

                pthread_attr_init( &threadAttr );
                pthread_create( &threadID, &threadAttr, runProcessor, (void *)timePointer );
                pthread_join( threadID, NULL );

                free( timePointer );

                accessTimer( LAP_TIMER, timeString );
                printf( "Time: %s, Process %d, ", timeString, processCount );
                printf( "run operation end\n");
            }
            else if( process->commandLetter == 'M' )
            {
                printf( "memory management " );
                printf( "%s action start\n", process->operation );

                accessTimer( LAP_TIMER, timeString );
                printf( "Time: %s, Process %d, ", timeString, processCount );
                printf( "memory management " );
                printf( "%s action end\n", process->operation );
            }
            else
            {
                accessTimer( STOP_TIMER, timeString );

                return PROCESS_FORMAT_ERROR;
            }

            process = process->next;
        }
        pList->state = EXIT_STATE;

        accessTimer( LAP_TIMER, timeString );
        printf( "Time: %s, OS: Process %d set in Exit state\n",
                timeString,
                processCount );

        pList = pList->next;
        processCount++;
    }

    accessTimer( STOP_TIMER, timeString );
    printf( "Time: %s, System Stop\n", timeString );

    return NO_PROCESS_ERROR;
}

ProcessList *addProcess( ProcessList *pList, ProcessList *newProcess )
{
    if( pList != NULL )
    {
        pList->next = addProcess( pList->next, newProcess );
        return pList;
    }

    pList = (ProcessList *)malloc( sizeof( ProcessList ) );
    pList->process = newProcess->process;
    pList->next = NULL;

    return pList;
}

int getTotalRuntime( MetaDataNode *process, ConfigDictionary *config )
{
    MetaDataNode *current;
    int totalRuntime;

    if( process == NULL )
    {
        return 0;
    }

    current = process;
    totalRuntime = 0;

    while( current != NULL )
    {
        if( current->commandLetter == 'I' || current->commandLetter == 'O' )
        {
            totalRuntime += config->ioCycleTime * current->commandValue;
        }
        else if( current->commandLetter == 'M' )
        {
            current = current->next;
            continue;
        }
        else
        {
            totalRuntime += config->processorCycleTime * current->commandValue;
        }

        current = current->next;
    }

    return totalRuntime;
}

ProcessList *clearProcess( ProcessList *pList )
{
    clearList( pList->process );

    free( pList );

    pList = NULL;

    return pList;
}

ProcessList *clearProcessList( ProcessList *pList )
{
    if( pList == NULL )
    {
        return NULL;
    }

    if( pList->next != NULL )
    {
        clearProcessList( pList->next );
    }

    clearList( pList->process );

    free( pList );

    pList = NULL;

    return pList;
}

void *runInput( void *time )
{
    runTimer( *(int *)time );
    pthread_exit( NULL );
}

void *runOutput( void *time )
{
    runTimer( *(int *)time );
    pthread_exit( NULL );
}

void *runProcessor( void *time )
{
    runTimer( *(int *)time );
    pthread_exit( NULL );
}

void setProcessStates( ProcessList *pList, int state )
{
    while( pList != NULL )
    {
        pList->state = state;
        pList = pList->next;
    }
}

#endif
