/**
  * MetaDataLinkedList.h
  *
  * Created by Connor Schwirian on 17 January 2018
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
    MetaDataNode *next;
} MetaDataNode;

enum MESSAGE_CODES
{
    UNKNOWN_ERR = -1000, FILE_OPEN_ERR,
    INCOMPLETE_FILE_ERR, NO_ERROR_MSG = 0,
    END_FILE_FOUND_MSG
};

// function prototypes
MetaDataNode *addNode( MetaDataNode *headNode, MetaDataNode *newNode );
MetaDataNode *clearList( MetaDataNode *headNode );
MetaDataNode *makeNode( char inCmdLetter, char *inOperation, int inCmdValue );
int getMetaDataFromFile( MetaDataNode **headNode, char *fileName);

#endif
