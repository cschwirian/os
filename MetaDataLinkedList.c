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

int getMetaDataFromFile( MetaDataNode *headNode, char *fileName)
{
    FILE *filePointer;
    int strIndex, charAsInt, tempCmdValue;
    char tempCmdLetter;
    char strBuffer[ MAX_STR_LEN ];
    Boolean inProgress = True;
    MetaDataNode *tempNode = ( MetaDataNode *)malloc( sizeof( MetaDataNode ) );

    filePointer = fopen( fileName, READ_ONLY_FLAG );

    if( filePointer != NULL )
    {
        strIndex = 0;

        charAsInt = fgetc( filePointer );

        while( feof( filePointer ) == NOT_AT_FILE_END && charAsInt != (int)( '\n' ) )
        {
            printf( "%c", (char)( charAsInt ) );
            strBuffer[ strIndex ] = (char)( charAsInt );

            strIndex++;

            strBuffer[ strIndex ] = NULL_CHAR;

            charAsInt = fgetc( filePointer );
        }

        if( compareString( strBuffer, "Start Program Meta-Data Code:" ) != 0 )
        {
            fclose( filePointer );
            return INIT_ERROR;
        }

        while( inProgress == True )
        {
            charAsInt = fgetc( filePointer );

            if( feof( filePointer ) == NOT_AT_FILE_END )
            {
                while( charAsInt == (int)( SPACE ) )
                {
                    charAsInt = fgetc( filePointer );
                }

                tempCmdLetter = (char)( charAsInt );

                charAsInt = fgetc( filePointer );
                if( charAsInt != (int)( OPEN_PAREN ) )
                {
                    fclose( filePointer );
                    return DATA_FORMAT_ERROR;
                }

                // TODO: while to catch operation

                // TODO: catch close paren

                // TODO: take in command value

                // TODO: check for semicolon

                // TODO: put data in new node and add to list

                return 1;
            }
        }
    }
}

#endif
