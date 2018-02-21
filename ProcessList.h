

#ifndef ProcessList_H
#define ProcessList_H

#include "LogData.h"
#include "MetaDataLinkedList.h"
#include "ConfigDictionary.h"

typedef struct ProcessList
{
    MetaDataNode *process;
    struct ProcessList *next;
} ProcessList;

int populateList( ProcessList **pList, MetaDataNode *data );
int runProcesses( ProcessList *pList, ConfigDictionary *config );
ProcessList *addProcess( ProcessList *pList, ProcessList *process );
ProcessList *addEmptyProcess( ProcessList *pList );
int getTotalRuntime( MetaDataNode *process, ConfigDictionary *config );
ProcessList *clearProcess( ProcessList *pList );
ProcessList *clearProcessList( ProcessList *pList );

#endif
