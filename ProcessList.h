/**
  * ProcessList.h
  *
  * Created by -REDACTED- on 12 February 2018
  *
  * Takes in and runs a simulated program
  *
  * Required by ProcessList.c
  *
  * Version 1.0 26/2/2018
  *
  * Version 0.1 (Development) 12/2/2018
**/

#ifndef ProcessList_H
#define ProcessList_H

#include "MemoryManager.h"
#include "LogData.h"
#include "MetaDataLinkedList.h"
#include "ConfigDictionary.h"
#include "simtimer.h"

typedef struct ProcessList
{
    MetaDataNode *process;
	MetaDataNode *currentProcess;
    int state;
    int processNum;
    int timeRemaining;
    struct ProcessList *next;
} ProcessList;

int populateList( ProcessList **pList, MetaDataNode *data,
                  ConfigDictionary *config );

int runProcesses( ProcessList *pList, MetaDataNode *data,
                  ConfigDictionary *config );

ProcessList *addProcess( ProcessList *pList, ProcessList *process );
ProcessList *sortProcesses( ProcessList *pList, char *schedulingCode );
int getTotalRuntime( MetaDataNode *process, ConfigDictionary *config );
ProcessList *clearProcess( ProcessList *pList );
ProcessList *clearProcessList( ProcessList *pList );
void *runInput( void *time );
void *runOutput( void *time );
void *runProcessor( void *time );
void setProcessStates( ProcessList *pList, int state );

#endif
