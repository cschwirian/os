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

int populateList( ProcessList **pList, MetaDataNode *data,
                  ConfigDictionary *config )
{
    int processCount;
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
    processCount = 0;

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

                currentProcess->processNum = processCount;
                currentProcess->timeRemaining = getTotalRuntime( currentProcess->process, config );

                *pList = addProcess( *pList, currentProcess );
                free( currentProcess );
                processCount++;
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

int runProcesses( ProcessList *pList, MetaDataNode *data,
                  ConfigDictionary *config )
{
    int memoryAvailable, processNum, segment, base, offset, memoryCommand;
    int *timePointer;
    char timeString[ 10 ], logBuffer[ 81 ];
    Boolean logToFile, logToMonitor;
    MetaDataNode *process;
    LogData *logData;
    MMU *memory;
    pthread_t threadID;

    logToFile = False;
    logToMonitor = False;
    logData = NULL;

    memoryAvailable = config->memoryAvailable;
    memory = NULL;

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

    if( pList == NULL )
    {
        clearData( logData );

        return EMPTY_PROGRAM_ERROR;
    }
    else if( data == NULL )
    {
        clearData( logData );

        return UNKNOWN_ERROR;
    }
    else if( config == NULL )
    {
        clearData( logData );

        return UNKNOWN_CONFIGURATION_ERROR;
    }

    accessTimer( LAP_TIMER, timeString );
    sprintf( logBuffer,
             "Time: %s, OS: All processes initialized in New state\n",
             timeString );
    followLogInstruction( logData, logToFile, logToMonitor, logBuffer );

    setProcessStates( pList, READY_STATE );
    pList = sortProcesses( pList, config->schedulingCode );

    accessTimer( LAP_TIMER, timeString );
    sprintf( logBuffer,
             "Time: %s, OS: All proceses now set in Ready state\n",
             timeString );
    followLogInstruction( logData, logToFile, logToMonitor, logBuffer );


    while( pList != NULL )
    {
        accessTimer( LAP_TIMER, timeString );
        sprintf( logBuffer,
                "Time: %s, OS: %s strategy selects Process %d with time %d msec\n",
                timeString,
                config->schedulingCode,
                pList->processNum,
                pList->timeRemaining );

        followLogInstruction( logData, logToFile, logToMonitor, logBuffer );

        pList->state = RUNNING_STATE;

        accessTimer( LAP_TIMER, timeString );
        sprintf( logBuffer,
                 "Time: %s, OS: Process %d set in Running state\n",
                 timeString, pList->processNum );

        followLogInstruction( logData, logToFile, logToMonitor, logBuffer );

        process = pList->process;
        processNum = pList->processNum;
        while( process != NULL )
        {
            accessTimer( LAP_TIMER, timeString );

            if( process->commandLetter == 'I' )
            {
                sprintf( logBuffer,
                         "Time: %s, Process %d, %s input start\n",
                         timeString, processNum, process->operation );
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
                         timeString, processNum, process->operation );
            }
            else if( process->commandLetter == 'O' )
            {
                sprintf( logBuffer,
                         "Time: %s, Process %d, %s output start\n",
                         timeString, processNum, process->operation );

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
                         timeString, processNum, process->operation );
            }
            else if( process->commandLetter == 'P' )
            {
                sprintf( logBuffer,
                         "Time: %s, Process %d, run operation start\n",
                         timeString, processNum );

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
                         timeString, processNum );
            }
            else if( process->commandLetter == 'M' )
            {
                memoryCommand = process->commandValue;

                segment = (int)( memoryCommand / SEG_OFFSET );
                base = memoryCommand % SEG_OFFSET;
                offset = memoryCommand % BASE_OFFSET;

                sprintf( logBuffer,
                         "Time: %s, Process %d, MMU %s: %d/%d/%d start\n",
                         timeString, processNum, process->operation,
                         segment, base, offset );

                followLogInstruction( logData, logToFile,
                                      logToMonitor, logBuffer );


                if( compareString( process->operation, "allocate") == 0 )
                {
                    if( isValidAlloc( memory, segment, base, offset ) &&
                        offset <= memoryAvailable )
                    {
                        memory = addSegment( memory, segment, base, offset,
                                             pList->processNum );

                        memoryAvailable -= offset;

                        accessTimer( LAP_TIMER, timeString );
                        sprintf( logBuffer,
                                 "Time: %s, Process %d, MMU %s: Successful\n",
                                 timeString, processNum, process->operation );
                    }
                    else
                    {
                        accessTimer( LAP_TIMER, timeString );
                        sprintf( logBuffer,
                                 "Time: %s, Process %d, MMU %s: Failed\n",
                                 timeString, processNum, process->operation );

                        followLogInstruction( logData, logToFile,
                                              logToMonitor, logBuffer );

                        accessTimer( LAP_TIMER, timeString );
                        sprintf( logBuffer,
                                 "Time: %s, OS: Process %d, Segmentation Fault - Process Ended\n",
                                 timeString, processNum );

                        followLogInstruction( logData, logToFile,
                                              logToMonitor, logBuffer );

                        break;
                    }
                }
                else if( compareString( process->operation, "access") == 0 )
                {
                    if( isValidAccess( memory, segment, base, offset ) )
                    {
                        accessTimer( LAP_TIMER, timeString );
                        sprintf( logBuffer,
                                 "Time: %s, Process %d, MMU %s: Successful\n",
                                 timeString, processNum, process->operation );
                    }
                    else
                    {
                        accessTimer( LAP_TIMER, timeString );
                        sprintf( logBuffer,
                                 "Time: %s, Process %d, MMU %s: Failed\n",
                                 timeString, processNum, process->operation );

                        followLogInstruction( logData, logToFile,
                                              logToMonitor, logBuffer );

                        accessTimer( LAP_TIMER, timeString );
                        sprintf( logBuffer,
                                 "Time: %s, OS: Process %d, Segmentation Fault - Process Ended\n",
                                 timeString, processNum );

                        break;
                    }
                }
            }
            else
            {
                accessTimer( STOP_TIMER, timeString );
                clearData( logData );
                clearMMU( memory );
                clearProcessList( pList );

                return PROCESS_FORMAT_ERROR;
            }

            followLogInstruction( logData, logToFile, logToMonitor, logBuffer );

            process = process->next;
        }
        pList->state = EXIT_STATE;

        accessTimer( LAP_TIMER, timeString );
        sprintf( logBuffer,
                 "Time: %s, OS: Process %d set in Exit state\n",
                 timeString, processNum );

        followLogInstruction( logData, logToFile, logToMonitor, logBuffer );

        pList = pList->next;
    }

    accessTimer( STOP_TIMER, timeString );
    sprintf( logBuffer, "Time: %s, System Stop\n", timeString );
    followLogInstruction( logData, logToFile, logToMonitor, logBuffer );

    if( logToFile == True )
    {
        logDataToFile( logData, config->logFilePath );
    }
    clearData( logData );
    clearMMU( memory );
    clearProcessList( pList );

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
    pList->processNum = newProcess->processNum;
    pList->timeRemaining = newProcess->timeRemaining;
    pList->state = newProcess->state;
    pList->next = NULL;

    return pList;
}

ProcessList *sortProcesses( ProcessList *pList, char *schedulingCode )
{
    int tempTime, tempNum, tempState;
    MetaDataNode *tempData;
    Boolean swapped;
    ProcessList *current, *iterator;

    if( pList == NULL )
    {
        return pList;
    }

    if( compareString( schedulingCode, "FCFS-N" ) == 0 )
    {
        return pList;
    }
    else if( compareString( schedulingCode, "SJF-N" ) == 0 )
    {
        iterator = NULL;

        do
        {
            swapped = False;
            current = pList;

            while( current->next != iterator )
            {
                if( current->timeRemaining > current->next->timeRemaining )
                {
                    tempTime = current->timeRemaining;
                    tempNum = current->processNum;
                    tempState = current->state;
                    tempData = current->process;

                    current->timeRemaining = current->next->timeRemaining;
                    current->processNum = current->next->processNum;
                    current->state = current->next->state;
                    current->process = current->next->process;

                    current->next->timeRemaining = tempTime;
                    current->next->processNum = tempNum;
                    current->next->state = tempState;
                    current->next->process = tempData;

                    swapped = True;
                }
                current = current->next;
            }
            iterator = current;
        }
        while( swapped == True );
    }

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
        pList->next = clearProcessList( pList->next );
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
