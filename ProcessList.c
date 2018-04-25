/**
  * ProcessList.c
  *
  * Created by -REDACTED- on 12 February 2018
  *
  * Takes in and runs a simulated program
  *
  * Requires ProcessList.h
  *
  * Version 1.1 20/4/2018
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
				currentProcess->currentProcess = NULL;
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
				currentProcess->currentProcess = currentProcess->process;

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

                    if( compareString( config->schedulingCode, "RR-P" ) == 0 ||
                        compareString( config->schedulingCode, "SRTF-P" ) == 0 ||
                        compareString( config->schedulingCode, "FCFS-P" ) == 0 )
                    {
                        return NO_PROCESS_ERROR_P;
                    }
                    else
                    {
                        return NO_PROCESS_ERROR;
                    }
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
    else if( config == NULL )
    {
        clearData( logData );
        clearProcessList( pList );

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

        process = pList->currentProcess;
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
                base = (int)( memoryCommand / BASE_OFFSET ) % BASE_OFFSET;
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
                    if( isValidAccess( memory, segment, base, offset, processNum ) )
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

						 followLogInstruction( logData, logToFile,
                                               logToMonitor, logBuffer );

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

int runProcessesPreemptive( ProcessList *pList, ConfigDictionary *config )
{
    int memoryAvailable, processNum, segment, base, offset, memoryCommand;
    int quantum;
    float currentTime, addedTime;
    char timeString[ 10 ], logBuffer[ 81 ];
    Boolean logToFile, logToMonitor;
    ProcessList *originalProcessList, *tempPList;
    LogData *logData;
    MMU *memory;
    Interrupt *interruptQueue, *currentInterrupt;

    logToFile = False;
    logToMonitor = False;
    logData = NULL;

    memoryAvailable = config->memoryAvailable;
    memory = NULL;

    interruptQueue = NULL;
    currentInterrupt = NULL;

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
    else if( config == NULL )
    {
        clearData( logData );
        clearProcessList( pList );

        return UNKNOWN_CONFIGURATION_ERROR;
    }

    accessTimer( LAP_TIMER, timeString );
    sprintf( logBuffer,
             "Time: %s, OS: All processes initialized in New state\n",
             timeString );
    followLogInstruction( logData, logToFile, logToMonitor, logBuffer );

    setProcessStates( pList, READY_STATE );
    originalProcessList = pList;

    accessTimer( LAP_TIMER, timeString );
    sprintf( logBuffer,
             "Time: %s, OS: All proceses now set in Ready state\n",
             timeString );
    followLogInstruction( logData, logToFile, logToMonitor, logBuffer );

    while( !isExited( originalProcessList ) )
    {
        if( isIdle( originalProcessList ) )
        {
            accessTimer( LAP_TIMER, timeString );
            sprintf( logBuffer, "Time: %s, OS: CPU Idle\n", timeString );
            followLogInstruction( logData, logToFile, logToMonitor, logBuffer );
        }
        while( isIdle( originalProcessList ) )
        {
            accessTimer( LAP_TIMER, timeString );
            currentTime = stringToFloat( timeString );

            currentInterrupt = checkForInterrupt( currentTime, &interruptQueue );
            while( currentInterrupt != NULL )
            {
                accessTimer( LAP_TIMER, timeString );
                sprintf( logBuffer,
                         "Time: %s, OS: Interrupt, Process %d\n",
                         timeString, currentInterrupt->processNum );

                followLogInstruction( logData, logToFile,
                                      logToMonitor, logBuffer );

                accessTimer( LAP_TIMER, timeString );
                sprintf( logBuffer,
                         "Time: %s, Process %d, %s %s end\n",
                         timeString, currentInterrupt->processNum,
                         currentInterrupt->operation,
                         currentInterrupt->ioType );

                followLogInstruction( logData, logToFile,
                                      logToMonitor, logBuffer );

                tempPList = originalProcessList;
                while( tempPList != NULL )
                {
                    if( tempPList->processNum == currentInterrupt->processNum )
                    {
                        tempPList->state = READY_STATE;
                        break;
                    }

                    tempPList = tempPList->next;
                }

                accessTimer( LAP_TIMER, timeString );
                sprintf( logBuffer,
                         "Time: %s, OS: Process %d set in Ready state\n \n",
                         timeString, currentInterrupt->processNum );

                followLogInstruction( logData, logToFile,
                                      logToMonitor, logBuffer );

                free( currentInterrupt );

                originalProcessList = sortProcesses( originalProcessList, config->schedulingCode );

                currentInterrupt = checkForInterrupt( currentTime, &interruptQueue );
            }
        }

        originalProcessList = sortProcesses( originalProcessList, config->schedulingCode );
        pList = getReadyProcess( originalProcessList );

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

        processNum = pList->processNum;

        quantum = config->quantumTime;
        while( pList->currentProcess != NULL && pList->timeRemaining > 0 &&
               quantum > 0 )
        {
            accessTimer( LAP_TIMER, timeString );

            if( pList->currentProcess->commandLetter == 'I' )
            {
                sprintf( logBuffer,
                         "Time: %s, Process %d, %s input start\n",
                         timeString, processNum, pList->currentProcess->operation );

                followLogInstruction( logData, logToFile,
                                      logToMonitor, logBuffer );

                pList->timeRemaining -= pList->currentProcess->commandValue * config->ioCycleTime;

                accessTimer( LAP_TIMER, timeString );

                currentTime = stringToFloat( timeString );
                addedTime = pList->currentProcess->commandValue * config->ioCycleTime;
                addedTime *= .001;
                currentTime += addedTime;

                interruptQueue = addInterrupt( pList->processNum, currentTime,
                                               "input",
                                               pList->currentProcess->operation,
                                               interruptQueue );

                pList->currentProcess = pList->currentProcess->next;
                pList->state = BLOCKED_STATE;
                break;
            }
            else if( pList->currentProcess->commandLetter == 'O' )
            {
                sprintf( logBuffer,
                         "Time: %s, Process %d, %s output start\n",
                         timeString, processNum, pList->currentProcess->operation );

                followLogInstruction( logData, logToFile,
                                      logToMonitor, logBuffer );

                pList->timeRemaining -= pList->currentProcess->commandValue * config->ioCycleTime;

                accessTimer( LAP_TIMER, timeString );

                currentTime = stringToFloat( timeString );
                addedTime = pList->currentProcess->commandValue * config->ioCycleTime;
                addedTime *= .001;
                currentTime += addedTime;

                interruptQueue = addInterrupt( pList->processNum, currentTime,
                                               "output",
                                               pList->currentProcess->operation,
                                               interruptQueue );

                pList->currentProcess = pList->currentProcess->next;
                pList->state = BLOCKED_STATE;
                break;
            }
            else if( pList->currentProcess->commandLetter == 'P' )
            {
                if( quantum == config->quantumTime )
                {
                    sprintf( logBuffer,
                             "Time: %s, Process %d, run operation start\n",
                             timeString, processNum );

                    followLogInstruction( logData, logToFile,
                                          logToMonitor, logBuffer );
                }

                runTimer( config->processorCycleTime );

                pList->currentProcess->commandValue--;
                pList->timeRemaining -= config->processorCycleTime;
                quantum--;

                if( pList->currentProcess->commandValue == 0 )
                {
                    accessTimer( LAP_TIMER, timeString );
                    sprintf( logBuffer,
                             "Time: %s, Process %d, run operation end\n",
                             timeString, processNum );

                    followLogInstruction( logData, logToFile,
                                          logToMonitor, logBuffer );

                    pList->currentProcess = pList->currentProcess->next;
                }

                currentTime = stringToFloat( timeString );

                currentInterrupt = checkForInterrupt( currentTime, &interruptQueue );
                if( currentInterrupt != NULL )
                {
                    break;
                }
            }
            else if( pList->currentProcess->commandLetter == 'M' )
            {
                memoryCommand = pList->currentProcess->commandValue;

                segment = (int)( memoryCommand / SEG_OFFSET );
                base = (int)( memoryCommand / BASE_OFFSET ) % BASE_OFFSET;
                offset = memoryCommand % BASE_OFFSET;

                sprintf( logBuffer,
                         "Time: %s, Process %d, MMU %s: %d/%d/%d start\n",
                         timeString, processNum, pList->currentProcess->operation,
                         segment, base, offset );

                followLogInstruction( logData, logToFile,
                                      logToMonitor, logBuffer );

                if( compareString( pList->currentProcess->operation, "allocate") == 0 )
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
                                 timeString, processNum, pList->currentProcess->operation );

                        followLogInstruction( logData, logToFile,
                                              logToMonitor, logBuffer );


                        pList->currentProcess = pList->currentProcess->next;
                    }
                    else
                    {
                        accessTimer( LAP_TIMER, timeString );
                        sprintf( logBuffer,
                                 "Time: %s, Process %d, MMU %s: Failed\n",
                                 timeString, processNum, pList->currentProcess->operation );

                        followLogInstruction( logData, logToFile,
                                              logToMonitor, logBuffer );

                        accessTimer( LAP_TIMER, timeString );
                        sprintf( logBuffer,
                                 "Time: %s, OS: Process %d, Segmentation Fault - Process Ended\n",
                                 timeString, processNum );

                        followLogInstruction( logData, logToFile,
                                              logToMonitor, logBuffer );

                        pList->state = EXIT_STATE;

                        break;
                    }
                }
                else if( compareString( pList->currentProcess->operation, "access") == 0 )
                {
                    if( isValidAccess( memory, segment, base, offset, processNum ) )
                    {
                        accessTimer( LAP_TIMER, timeString );
                        sprintf( logBuffer,
                                 "Time: %s, Process %d, MMU %s: Successful\n",
                                 timeString, processNum, pList->currentProcess->operation );

                        followLogInstruction( logData, logToFile,
                                              logToMonitor, logBuffer );

                        pList->currentProcess = pList->currentProcess->next;
                    }
                    else
                    {
                        accessTimer( LAP_TIMER, timeString );
                        sprintf( logBuffer,
                                 "Time: %s, Process %d, MMU %s: Failed\n",
                                 timeString, processNum, pList->currentProcess->operation );

                        followLogInstruction( logData, logToFile,
                                              logToMonitor, logBuffer );

                        accessTimer( LAP_TIMER, timeString );
                        sprintf( logBuffer,
                                 "Time: %s, OS: Process %d, Segmentation Fault - Process Ended\n",
                                 timeString, processNum );

						followLogInstruction( logData, logToFile,
                                              logToMonitor, logBuffer );

                        pList->state = EXIT_STATE;

                        break;
                    }
                }
            }
            else
            {
                accessTimer( STOP_TIMER, timeString );
                clearData( logData );
                clearMMU( memory );
                clearProcessList( originalProcessList );

                return PROCESS_FORMAT_ERROR;
            }
        }

        if( pList->state == BLOCKED_STATE )
        {
            accessTimer( LAP_TIMER, timeString );
            sprintf( logBuffer,
                     "Time: %s, OS: Process %d set in Blocked state\n\n",
                     timeString, processNum );

            followLogInstruction( logData, logToFile, logToMonitor, logBuffer );
        }
        else if( pList->timeRemaining == 0 || pList->state == EXIT_STATE )
        {
            pList->state = EXIT_STATE;

            accessTimer( LAP_TIMER, timeString );
            sprintf( logBuffer,
                     "Time: %s, OS: Process %d set in Exit state\n\n",
                     timeString, processNum );

            followLogInstruction( logData, logToFile, logToMonitor, logBuffer );
        }
        else if( currentInterrupt != NULL )
        {
            while( currentInterrupt != NULL )
            {
                accessTimer( LAP_TIMER, timeString );
                sprintf( logBuffer,
                         "Time: %s, OS: Interrupt, Process %d\n",
                         timeString, currentInterrupt->processNum );

                followLogInstruction( logData, logToFile,
                                      logToMonitor, logBuffer );

                if( pList->state != READY_STATE )
                {
                    pList->state = READY_STATE;

                    accessTimer( LAP_TIMER, timeString );
                    sprintf( logBuffer,
                           "Time: %s, OS: Process %d set in Ready state\n",
                           timeString, processNum );

                    followLogInstruction( logData, logToFile,
                                        logToMonitor, logBuffer );
                }

                accessTimer( LAP_TIMER, timeString );
                sprintf( logBuffer,
                         "Time: %s, Process %d, %s %s end\n",
                         timeString, currentInterrupt->processNum,
                         currentInterrupt->operation,
                         currentInterrupt->ioType );

                followLogInstruction( logData, logToFile,
                                      logToMonitor, logBuffer );

                tempPList = originalProcessList;
                while( tempPList != NULL )
                {
                    if( tempPList->processNum == currentInterrupt->processNum )
                    {
                        tempPList->state = READY_STATE;
                        break;
                    }

                    tempPList = tempPList->next;
                }

                accessTimer( LAP_TIMER, timeString );
                sprintf( logBuffer,
                         "Time: %s, OS: Process %d set in Ready state\n\n",
                         timeString, currentInterrupt->processNum );

                followLogInstruction( logData, logToFile,
                                      logToMonitor, logBuffer );

                free( currentInterrupt );

                originalProcessList = sortProcesses( originalProcessList, config->schedulingCode );

                currentInterrupt = checkForInterrupt( currentTime, &interruptQueue );
            }
        }
        else if( pList->state == RUNNING_STATE )
        {
            accessTimer( LAP_TIMER, timeString );
            sprintf( logBuffer,
                     "Time: %s, OS: Process %d quantum time out\n",
                     timeString, processNum );

            followLogInstruction( logData, logToFile, logToMonitor, logBuffer );

            pList->state = READY_STATE;

            accessTimer( LAP_TIMER, timeString );
            sprintf( logBuffer,
                     "Time: %s, OS: Process %d set in Ready state\n\n",
                     timeString, processNum );

            followLogInstruction( logData, logToFile, logToMonitor, logBuffer );
        }
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
    clearProcessList( originalProcessList );

    return NO_PROCESS_ERROR;
}

void printList( ProcessList *pList )
{
    printf("processes: ");
    while( pList != NULL )
    {
        printf( "%d: %d, ", pList->processNum, pList->state );
        pList = pList->next;
    }
    printf("\n");
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
	pList->currentProcess = newProcess->currentProcess;
    pList->processNum = newProcess->processNum;
    pList->timeRemaining = newProcess->timeRemaining;
    pList->state = newProcess->state;
    pList->next = NULL;

    return pList;
}

ProcessList *sortProcesses( ProcessList *pList, char *schedulingCode )
{
    int tempTime, tempNum, tempState;
    MetaDataNode *tempData, *tempCurrent;
    Boolean swapped;
    ProcessList *current, *iterator;

    if( pList == NULL )
    {
        return pList;
    }

    if( compareString( schedulingCode, "FCFS-N" ) == 0 ||
        compareString( schedulingCode, "FCFS-P" ) == 0 )
    {
        return pList;
    }
    else if( compareString( schedulingCode, "RR-P" ) == 0 )
    {
        iterator = NULL;

        do
        {
            swapped = False;
            current = pList;

            while( current->next != iterator )
            {
                if( current->state == BLOCKED_STATE )
                {
                    tempTime = current->timeRemaining;
                    tempNum = current->processNum;
                    tempState = current->state;
                    tempData = current->process;
                    tempCurrent = current->currentProcess;

                    current->timeRemaining = current->next->timeRemaining;
                    current->processNum = current->next->processNum;
                    current->state = current->next->state;
                    current->process = current->next->process;
                    current->currentProcess = current->next->currentProcess;

                    current->next->timeRemaining = tempTime;
                    current->next->processNum = tempNum;
                    current->next->state = tempState;
                    current->next->process = tempData;
                    current->next->currentProcess = tempCurrent;

                    swapped = True;
                }
                current = current->next;
            }
            iterator = current;
        }
        while( swapped == True );
    }
    else if( compareString( schedulingCode, "SJF-N" ) == 0 ||
             compareString( schedulingCode, "SRTF-P" ) == 0 )
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
                    tempCurrent = current->currentProcess;

                    current->timeRemaining = current->next->timeRemaining;
                    current->processNum = current->next->processNum;
                    current->state = current->next->state;
                    current->process = current->next->process;
                    current->currentProcess = current->next->currentProcess;

                    current->next->timeRemaining = tempTime;
                    current->next->processNum = tempNum;
                    current->next->state = tempState;
                    current->next->process = tempData;
                    current->next->currentProcess = tempCurrent;

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

ProcessList *getReadyProcess( ProcessList *pList )
{
    while( pList != NULL )
    {
        if( pList->state == READY_STATE )
        {
            return pList;
        }

        pList = pList->next;
    }

    return NULL;
}

Boolean isIdle( ProcessList *pList )
{
    while( pList != NULL )
    {
        if( pList->state == READY_STATE )
        {
            return False;
        }

        pList = pList->next;
    }

    return True;
}

Boolean isExited( ProcessList *pList )
{
    while( pList != NULL )
    {
        if( pList->state != EXIT_STATE )
        {
            return False;
        }

        pList = pList->next;
    }

    return True;
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
