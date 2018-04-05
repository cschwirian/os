/**
  * ConfigDictionary.h
  *
  * Created by -REDACTED- on 17 January 2018
  *
  * Contains .cnf file data
  *
  * Required by ConfigLinkedList.c
  *
  * Version 1.0 31/1/2018
  *
  * Version 0.1 (Development) 17/1/2018
**/

#ifndef ConfigDictionary_H
#define ConfigDictionary_H

#include "Utilities.h"

typedef struct ConfigDictionary
{
    int versionNumber;
    char filePath[ 30 ];
    char schedulingCode[ 7 ];
    int quantumTime;
    int memoryAvailable;
    int processorCycleTime;
    int ioCycleTime;
    char logInstruction[ 8 ];
    char logFilePath[ 30 ];
} ConfigDictionary;

enum DATA_CODES
{
    VERSION = 0,
    FILE_PATH = 1,
    CPU_CODE = 2,
    QUANTUM_TIME = 3,
    MEMORY = 4,
    P_CYCLE_TIME = 5,
    IO_CYCLE_TIME = 6,
    LOG_INSTRUCTION = 7,
    LOG_PATH = 8
};

void clearDictionary( ConfigDictionary *config );
int getConfig( ConfigDictionary *config, const char *fileName );
int logConfig( ConfigDictionary *config, const char *fileName );
int versionIsValid( int versionNumber );
int cpuCodeIsValid( char *cpuCode );
int quantumTimeIsValid( int quantumTime );
int memoryIsValid( int memory );
int processorCycleIsValid( int processorCycle );
int ioCycleIsValid( int ioCycle );
int logInstructionIsValid( char *instruction );

#endif
