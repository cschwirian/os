/**
  * MemoryManager.c
  *
  * Created by -REDACTED- on 4 April 2018
  *
  * Performs the actions of a MMU
  *
  * Requires MemoryManager.h
  *
  * Version 1.0 4/4/2018
**/

#ifndef MemoryManager_C
#define MemoryManager_C

#include "MemoryManager.h"

MMU *addSegment( MMU *memory, int segment, int base, int offset, int processNum )
{
    if( memory != NULL )
    {
        memory->next = addSegment( memory->next, segment, base, offset, processNum );
        return memory;
    }

    memory = (MMU *)malloc( sizeof( MMU ) );
    memory->segment = segment;
    memory->base = base;
    memory->offset = offset;
    memory->processNum = processNum;

    return memory;
}

Boolean isValidAlloc( MMU *memory, int segment, int base, int offset )
{
    while( memory != NULL )
    {
        if( memory->segment == segment )
        {
            if( memory->base > base )
            {
                if( memory->base <= base + offset - 1 )
                {
                    return False;
                }
            }
            else if( memory->base < base )
            {
                if( base <= memory->base + memory->offset - 1 )
                {
                    return False;
                }
            }
            else if( memory->base == base )
            {
                return False;
            }
        }

        memory = memory->next;
    }

    return True;
}

Boolean isValidAccess( MMU *memory, int segment, int base, int offset )
{
    while( memory != NULL )
    {
        if( memory->segment == segment )
        {
            if( memory->base > base )
            {
                memory = memory->next;
                continue;
            }
            else if( memory->base <= base )
            {
                if( base + offset - 1 <= memory->base + memory->offset - 1 )
                {
                    return True;
                }
            }
        }

        memory = memory->next;
    }

    return False;
}

MMU *clearMMU( MMU *memory )
{
    if( memory == NULL )
    {
        return NULL;
    }
    else if( memory->next != NULL )
    {
        clearMMU( memory->next );
    }

    free( memory );
    memory = NULL;

    return memory;
}

#endif
