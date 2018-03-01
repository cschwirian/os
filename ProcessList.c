/**
  * ProcessList.c
  *
  * Created by -REDACTED- on 12 February 2018
  *
  * Takes in and runs a simulated program
  *
  * Requires ProcessList.h
  *
  * Version 1.0 26/2/2018
  *
  * Version 0.1 (Development) 12/2/2018
**/

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

int runProcesses( ProcessList *pList, ConfigDictionary *config )
{
    int processCount;
    int *timePointer;
    char timeString[ 10 ], logBuffer[ 81 ];
    Boolean logToFile, logToMonitor;
    MetaDataNode *process;
    LogData *logData;
    pthread_t threadID;

    logToFile = False;
    logToMonitor = False;
    logData = NULL;

    if( compareString( config->logInstruction, "File" ) == 0 ||
        compareString( config->logInstruction, "Both" ) == 0 )
    {
        logToFile = True;
        logData = (LogData *)malloc( sizeof( LogData ) );
    }

    if( compareString( config->logInstruction, "Monitor" ) == 0 ||
        compareString( config->logInstruction, "Both" ) == 0 )
    {
        logToMonitor = True;
    }

    accessTimer( ZERO_TIMER, timeString );
    sprintf( logBuffer, "Time: %s, System Start\n", timeString );
    followLogInstruction( logData, logToFile, logToMonitor, logBuffer );

    accessTimer( LAP_TIMER, timeString );
    sprintf( logBuffer, "Time: %s, OS: Begin PCB Creation\n", timeString );
    followLogInstruction( logData, logToFile, logToMonitor, logBuffer );

    accessTimer( LAP_TIMER, timeString );
    sprintf( logBuffer,
             "Time: %s, OS: All processes initialized in New state\n",
             timeString );
    followLogInstruction( logData, logToFile, logToMonitor, logBuffer );

    setProcessStates( pList, READY_STATE );

    accessTimer( LAP_TIMER, timeString );
    sprintf( logBuffer,
             "Time: %s, OS: All proceses now set in Ready state\n",
             timeString );
    followLogInstruction( logData, logToFile, logToMonitor, logBuffer );

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
        sprintf( logBuffer,
                "Time: %s, OS: %s strategy selects Process %d with time %d msec\n",
                timeString,
                config->schedulingCode,
                processCount,
                getTotalRuntime( pList->process, config ) );

        followLogInstruction( logData, logToFile, logToMonitor, logBuffer );

        pList->state = RUNNING_STATE;

        accessTimer( LAP_TIMER, timeString );
        sprintf( logBuffer,
                 "Time: %s, OS: Process %d set in Running state\n",
                 timeString, processCount );

        followLogInstruction( logData, logToFile, logToMonitor, logBuffer );

        process = pList->process;
        while( process != NULL )
        {
            accessTimer( LAP_TIMER, timeString );

            if( process->commandLetter == 'I' )
            {
                sprintf( logBuffer,
                         "Time: %s, Process %d, %s input start\n",
                         timeString, processCount, process->operation );
                followLogInstruction( logData, logToFile,
                                      logToMonitor, logBuffer );

                timePointer = (int *)malloc( sizeof( int ) );
                *timePointer = process->commandValue;
                *timePointer = (*timePointer) * config->ioCycleTime;

                pthread_create( &threadID, NULL,
                                runInput, (void *)timePointer );
                pthread_join( threadID, NULL );

                free( timePointer );

                accessTimer( LAP_TIMER, timeString );
                sprintf( logBuffer,
                         "Time: %s, Process %d, %s input end\n",
                         timeString, processCount, process->operation );
            }
            else if( process->commandLetter == 'O' )
            {
                sprintf( logBuffer,
                         "Time: %s, Process %d, %s output start\n",
                         timeString, processCount, process->operation );

                followLogInstruction( logData, logToFile,
                                      logToMonitor, logBuffer );

                timePointer = (int *)malloc( sizeof( int ) );
                *timePointer = process->commandValue;
                *timePointer = (*timePointer) * config->ioCycleTime;

                pthread_create( &threadID, NULL,
                                runOutput, (void *)timePointer );
                pthread_join( threadID, NULL );

                free( timePointer );

                accessTimer( LAP_TIMER, timeString );
                sprintf( logBuffer,
                         "Time: %s, Process %d, %s output end\n",
                         timeString, processCount, process->operation );
            }
            else if( process->commandLetter == 'P' )
            {
                sprintf( logBuffer,
                         "Time: %s, Process %d, run operation start\n",
                         timeString, processCount );

                followLogInstruction( logData, logToFile,
                                      logToMonitor, logBuffer );

                timePointer = (int *)malloc( sizeof( int ) );
                *timePointer = process->commandValue;
                *timePointer = (*timePointer) * config->processorCycleTime;

                pthread_create( &threadID, NULL,
                                runProcessor, (void *)timePointer );
                pthread_join( threadID, NULL );

                free( timePointer );

                accessTimer( LAP_TIMER, timeString );
                sprintf( logBuffer,
                         "Time: %s, Process %d, run operation end\n",
                         timeString, processCount );
            }
            else if( process->commandLetter == 'M' )
            {
                sprintf( logBuffer,
                         "Time: %s, Process %d, memory management %s action start\n",
                         timeString, processCount, process->operation );

                followLogInstruction( logData, logToFile,
                                      logToMonitor, logBuffer );

                accessTimer( LAP_TIMER, timeString );
                sprintf( logBuffer,
                         "Time: %s, Process %d, memory management %s action end\n",
                         timeString, processCount, process->operation );
            }
            else
            {
                accessTimer( STOP_TIMER, timeString );
                clearData( logData );

                return PROCESS_FORMAT_ERROR;
            }

            followLogInstruction( logData, logToFile, logToMonitor, logBuffer );

            process = process->next;
        }
        pList->state = EXIT_STATE;

        accessTimer( LAP_TIMER, timeString );
        sprintf( logBuffer,
                 "Time: %s, OS: Process %d set in Exit state\n",
                 timeString, processCount );

        followLogInstruction( logData, logToFile, logToMonitor, logBuffer );

        pList = pList->next;
        processCount++;
    }

    accessTimer( STOP_TIMER, timeString );
    sprintf( logBuffer, "Time: %s, System Stop\n", timeString );
    followLogInstruction( logData, logToFile, logToMonitor, logBuffer );

    if( logToFile == True )
    {
        logDataToFile( logData, config->logFilePath );
    }
    clearData( logData );

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
    pList->state = newProcess->state;
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
