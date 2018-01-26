/**
  * MetaDataLinkedList.c
  *
  * Created by Connor Schwirian on 17 January 2018
  *
  * Heavily modified from provided linkedlist.c
  *
  * Parses and contains simulator metadata
  *
  * Requires MetaDataLinkedList.h
  *
  * Version 0.1 (Development) 17/1/2018
**/

#ifndef MetaDataLinkedList_C
#define MetaDataLinkedList_C

// header files
#include "MetaDataLinkedList.h"

// supporting function implementations


MetaDataNode *addNode( MetaDataNode *headNode, MetaDataNode *newNode )
{
    if( headNode != NULL )
    {
        headNode->next = addNode( headNode->next, newNode );
        return headNode;
    }

    headNode = makeNode( newNode->commandLetter, newNode->operation,
                                                    newNode->commandValue );

    return headNode;
}

// function eliminates redundant code in addNode
MetaDataNode *makeNode( char inCmdLetter, char *inOperation, int inCmdValue )
{
    MetaDataNode *newNode = ( MetaDataNode * ) malloc( sizeof( MetaDataNode ) );

    newNode->commandLetter = inCmdLetter;
    copyString( newNode->operation, inOperation );
    newNode->commandValue = inCmdValue;
    newNode->next = NULL;

    return newNode;
}

MetaDataNode *clearList( MetaDataNode *headNode )
{
    if( headNode == NULL )
    {
        return NULL;
    }

    if( headNode->next != NULL )     {
        clearList( headNode->next );
    }

    free( headNode );

    headNode = NULL;

    return headNode;
}

int getMetaDataFromFile( MetaDataNode **headNode, char *fileName)
{
    File filePointer;
    int strIndex, charAsInt;
    char tempChar, stringBuffer[  ];
    Boolean inProgress = TRUE;
    MetaDataNode *tempNode = ( MetaDataNode *)malloc( sizeof( MetaDataNode ) );
}

#endif
