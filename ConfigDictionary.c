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
                                    ConfigDictionary *config,
                                    int versionNumber,
                                    char *filePath,
                                    char *schedulingCode,
                                    int quantumTime,
                                    int memoryAvailible,
                                    int processorCycleTime,
                                    int ioCyleTime,
                                    char *logInstruction,
                                    char *logFilePath
                                )
{

}

ConfigDictionary clearDictionary( ConfigDictionary *config )
{

}

int getConfigFromFile( ConfigDictionary **configData, char *fileName )
{

}

#endif
