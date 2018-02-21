

#ifndef ProcessList_C
#define ProcessList_C

#include "ProcessList.h"

int populateList( ProcessList *pList, MetaDataNode *data )
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
    currentProcess = pList;
    addEmptyProcess( currentProcess );

    while( currentData != NULL )
    {
        if( currentData->commandLetter == 'A' )
        {
            if( compareString( currentData->operation, "start") == 0 &&
                currentData->commandValue == 0 )
            {
                inProgress = True;

                currentData = currentData->next;

                addEmptyProcess( currentProcess );
                currentProcess = currentProcess->next;
            }
            else if( compareString( currentData->operation, "end") == 0 &&
                     currentData->commandValue == 0 )
            {
                if( inProgress )
                {
                    inProgress = False;
                }
                else
                {
                    return PROCESS_FORMAT_ERROR;
                }

                currentData = currentData->next;
            }
        }
        else if( currentData->commandLetter == 'S' )
        {
            if( compareString( currentData->operation, "end") == 0 &&
                currentData->commandValue == 0 )
            {
                if( inProgress )
                {
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
        else if( inProgress )
        {
            tempNode = currentData;
            tempNode->next = NULL;

            addNode( currentProcess->process, tempNode );

            currentData = currentData->next;
        }
        else
        {
            return UNKNOWN_ERROR;
        }
    }

    return PROCESS_FORMAT_ERROR;
}

void addEmptyProcess( ProcessList *pList )
{
    ProcessList *current = pList;

    if( current == NULL )
    {
        current = (ProcessList *)malloc( sizeof( ProcessList ) );
        current->process = NULL;
        current->next = NULL;
    }
    else
    {
        while( current->next == NULL )
        {
            current = current->next;
        }

        current->next = (ProcessList *)malloc( sizeof( ProcessList ) );
        current = current->next;

        current->process = NULL;
        current->next = NULL;
    }
}

ProcessList *clearProcessList( ProcessList *list )
{
    if( list == NULL )
       {
        return NULL;
       }

    if( list->next != NULL )
       {
        clearProcessList( list->next );
       }

    free( list );

    list = NULL;

    return list;
}

#endif
