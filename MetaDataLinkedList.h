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
  * Version 1.0 31/1/2018
  *
  * Version 0.1 (Development) 17/1/2018
**/

#ifndef MetaDataLinkedList_H
#define MetaDataLinkedList_H

#include "Utilities.h"

typedef struct MetaDataNode
{
    char commandLetter;
    char operation[ 10 ];
    int commandValue;
    struct MetaDataNode *next;
} MetaDataNode;

MetaDataNode *addNode( MetaDataNode *headNode, MetaDataNode *newNode );
MetaDataNode *makeNode( char inCmdLetter, char *inOperation, int inCmdValue );
MetaDataNode *copyNode( MetaDataNode *original );
MetaDataNode *clearList( MetaDataNode *headNode );

int getMetaData( MetaDataNode **headNode, char *fileName,
                 char *instruction, char *logFilePath );

int logMetaData( MetaDataNode *headNode, char *instruction, char *logFilePath );
int operationIsValid( char *operation );
int commandIsValid( char command );

#endif
