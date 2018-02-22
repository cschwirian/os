

#ifndef ProcessList_H
#define ProcessList_H

#include "LogData.h"
#include "MetaDataLinkedList.h"
#include "ConfigDictionary.h"
#include "simtimer.h"

typedef struct ProcessList
{
    MetaDataNode *process;
    int state;
    struct ProcessList *next;
} ProcessList;

int populateList( ProcessList **pList, MetaDataNode *data );

int runProcesses( ProcessList *pList,
                  ConfigDictionary *config,
                  MetaDataNode *data );

ProcessList *addProcess( ProcessList *pList, ProcessList *process );
ProcessList *addEmptyProcess( ProcessList *pList );
int getTotalRuntime( MetaDataNode *process, ConfigDictionary *config );
ProcessList *clearProcess( ProcessList *pList );
ProcessList *clearProcessList( ProcessList *pList );
void *runInput( void *time );
void *runOutput( void *time );
void *runProcessor( void *time );
void setProcessStates( ProcessList *pList, int state );

#endif
