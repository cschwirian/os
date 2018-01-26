/**
  * ConfigDictionary.c
  *
  * Created by Connor Schwirian on 17 January 2018
  *
  * Contains .cnf file data
  *
  * Requires ConfigLinkedList.h
  *
  * Version 0.1 (Development) 17/1/2018
**/

#ifndef ConfigDictionary_C
#define ConfigDictionary_C

#include "ConfigDictionary.h"

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
                                )
{
    ConfigDictionary config = malloc( sizeof( ConfigDictionary * ) );

    config->versionNumber = inVersionNumber;
    config->filePath = inFilePath;
    config->schedulingCode = inSchedulingCode;
    config->quantumTime = inQuantumTime;
    config->memoryAvailible = inMemoryAvailible;
    config->processorCycleTime = inProcessorCycleTime;
    config->ioCyleTime = inIOCyleTime;
    config->logInstruction = inLogInstruction;
    config->logFilePath = inLogFilePath;

    return config;
}

ConfigDictionary clearDictionary( ConfigDictionary *config )
{
    free( config );
    config = NULL;

    return config;
}

int getConfigFromFile( ConfigDictionary **configData, char *fileName )
{
    File filePointer;
    int strIndex, charAsInt;
    char tempChar, stringBuffer[  ];
    Boolean inProgress = TRUE;
}

#endif
