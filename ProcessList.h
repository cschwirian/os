

#ifndef ProcessList_H
#define ProcessList_H

#include "LogData.h"
#include "MetaDataLinkedList.h"

typedef struct ProcessList
{
    MetaDataNode *process;
    struct ProcessList *next;
} ProcessList;

int populateList( ProcessList *pList, MetaDataNode *data );
void addEmptyProcess( ProcessList *list );
ProcessList *clearProcessList( ProcessList *list );

#endif
