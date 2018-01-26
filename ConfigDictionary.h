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

typdef struct ConfigDictionary
{
    int versionNumber;
    char filePath[ 30 ];
    char schedulingCode[ 6 ];
    int quantumTime;
    int memoryAvailible;
    int processorCycleTime;
    int ioCyleTime;
    char logInstruction[ 7 ];
    char logFilePath[ 30 ];
} ConfigDictionary;

// Message codes for file access
enum MESSAGE_CODES
{
    UNKNOWN_ERR = -1000, FILE_OPEN_ERR,
    INCOMPLETE_FILE_ERR, NO_ERROR_MSG = 0,
    END_FILE_FOUND_MSG
};

ConfigDictionary makeDictionary(
                                    int inVersionNumber,
                                    char *inFilePath,
                                    char *inSchedulingCode,
                                    int inQuantumTime,
                                    int inMemoryAvailible,
                                    int inProcessorCycleTime,
                                    int inIOCyleTime,
                                    char *inLogInstruction,
                                    char *inLogFilePath
                                );

ConfigDictionary clearDictionary( ConfigDictionary *config );
int getConfigFromFile( ConfigDictionary **configData, char *fileName );

#endif
