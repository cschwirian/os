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


struct Node *addNode( struct Node *headNode, struct Node *newNode )
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
struct Node *makeNode( char inCommandLetter, char *inOperation, int inCommandValue )
{
    struct Node *newNode = ( struct Node * ) malloc( sizeof( struct Node ) );

    newNode->commandLetter = inCommandLetter;
    copyString( newNode->operation, inOperation );
    newNode->commandValue = inCommandValue;
    newNode->next = NULL;

    return newNode;
}

struct Node *clearList( struct NodeType *headNode )
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

// returned node memory is not freed inside linked list
// it must be freed in calling function
struct Node *findNodeByData( struct Node *workingNode, char *data )
{
    if( workingNode != NULL )
    {
        if( strcmp( workingNode->data, data ) == 0 )
        {
            return makeNode( workingNode->data );
        }

        return findNodeByName( workingNode->next, data );
    }

    return NULL;
}

void copyString( char *target, char *original )
{
    int stringIndex = 0;

    while( stringIndex <= sizeof( original ) )
    {
        target[ stringIndex ] = original[ stringIndex ];

        stringIndex++;
    }

}

#endif
