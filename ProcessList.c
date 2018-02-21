

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
    return 1;
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

#endif
