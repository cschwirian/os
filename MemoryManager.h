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
Boolean isValidAccess( MMU *memory, int segment, int base, int offset );
MMU *clearMMU( MMU *memory );

#endif
