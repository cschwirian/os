/**
  * MetaDataLinkedList.h
  *
  * Created by -REDACTED- on 17 January 2018
  *
  * Heavily modified from provided linkedlist.h
  *
  * Parses and contains simulator metadata
  *
  * Required by MetaDataLinkedList.c
  *
  * Version 0.1 (Development) 17/1/2018
**/

// Precompiler directive, to eliminate multiple compiles of h file
#ifndef MetaDataLinkedList_H
#define MetaDataLinkedList_H

#include "Utilities.h"

// global constants, forward declaration

// struct declaration
typedef struct MetaDataNode
{
    char commandLetter;
    char operation[ 10 ];
    int commandValue;
    struct MetaDataNode *next;
} MetaDataNode;

// function prototypes
MetaDataNode *addNode( MetaDataNode *headNode, MetaDataNode *newNode );
MetaDataNode *clearList( MetaDataNode *headNode );
MetaDataNode *makeNode( char inCmdLetter, char *inOperation, int inCmdValue );
int getMetaData( MetaDataNode **headNode, char *fileName, char *logFileName );
int logMetaData( MetaDataNode *headNode, char *logFileName );

#endif
