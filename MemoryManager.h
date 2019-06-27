/**
  * MemoryManager.h
  *
  * Created by -REDACTED- on 4 April 2018
  *
  * Performs the actions of a MMU
  *
  * Required by MemoryManager.c
  *
  * Version 1.0 4/4/2018
**/

#ifndef MemoryManager_H
#define MemoryManager_H

#include "Utilities.h"

typedef struct MMU
{
    int segment;
    int base;
    int offset;
    int processNum;
    struct MMU *next;
} MMU;

MMU* addSegment( MMU *memory, int segment, int base, int offset, int processNum );
Boolean isValidAlloc( MMU *memory, int segment, int base, int offset );
Boolean isValidAccess( MMU *memory, int segment, int base,
                       int offset, int processNum );
                       
MMU *clearMMU( MMU *memory );

#endif
