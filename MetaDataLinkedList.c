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

const int STD_STR_LEN = 40;
const int MAX_STR_LEN = 60;
const int NOT_AT_FILE_END = 0;
const char READ_ONLY_FLAG[] = "r";
const char WRITE_ONLY_FLAG[] = "w";
const char NULL_CHAR = '\0';
const char SEMICOLON = ';';
const char OPEN_PAREN = '(';
const char CLOSE_PAREN = ')';
const char SPACE = ' ';
const char NEWLINE = '\n';

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
    char tempChar, stringBuffer[ MAX_STR_LEN ];
    Boolean inProgress = True;
    MetaDataNode *tempNode = ( MetaDataNode *)malloc( sizeof( MetaDataNode ) );

    filePointer = fopen( fileName, READ_ONLY_FLAG );

    if( filePointer != NULL )
    {
        strIndex = 0;

        charAsInt = fgetc( filePointer );

        while( feof( filePointer ) == NOT_AT_FILE_END && charAsInt != (int)( '\n' ) )
        {
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

                strIndex = 0;

                while( charAsInt != (int)( OPEN_PAREN ) )
                {
                    strBuffer[ strIndex ] = (char)( charAsInt );

                    strIndex++;

                    strBuffer[ strIndex ] = NULL_CHAR;

                    charAsInt = fgetc( filePointer );
                }
                printf( "%s", strBuffer );
                return 1;
            }
        }
    }
}

#endif
