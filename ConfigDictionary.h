/**
  * ConfigDictionary.h
  *
  * Created by Connor Schwirian on 17 January 2018
  *
  * Contains .cnf file data
  *
  * Required by ConfigLinkedList.c
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
    int memoryAvailible;
    int processorCycleTime;
    int ioCyleTime;
    char logInstruction[ 8 ];
    char logFilePath[ 30 ];
} ConfigDictionary;

// Message codes for file access
enum CONFIG_MESSAGE_CODES
{
    UNKNOWN_ERROR = -1,
    NO_ERROR_MSG = 0,
    INIT_ERROR = 1,
    DATA_ORDER_ERROR = 2,
    DATA_FORMAT_ERROR = 3,
    INCOMPLETE_DATA_ERROR = 4,
    FILE_END_ERROR = 5
};

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
int getConfigFromFile( ConfigDictionary *config, const char *fileName );
int logConfigData( ConfigDictionary *config, const char *fileName );

#endif
