/**
  * MetaDataLinkedList.c
  *
  * Created by -REDACTED- on 17 January 2018
  *
  * Heavily modified from provided linkedlist.c
  *
  * Parses and contains simulator metadata
  *
  * Requires MetaDataLinkedList.h
  *
  * Version 1.0 31/1/2018
  *
  * Version 0.1 (Development) 17/1/2018
**/

#ifndef MetaDataLinkedList_C
#define MetaDataLinkedList_C

#include "MetaDataLinkedList.h"

MetaDataNode *addNode( MetaDataNode *headNode, MetaDataNode *newNode )
{
    if( headNode != NULL )
    {
        headNode->next = addNode( headNode->next, newNode );
        return headNode;
    }

    headNode = makeNode( newNode->commandLetter,
                         newNode->operation,
                         newNode->commandValue );

    return headNode;
}

MetaDataNode *makeNode( char inCmdLetter, char *inOperation, int inCmdValue )
{
    MetaDataNode *newNode = (MetaDataNode *)malloc( sizeof( MetaDataNode ) );

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

int getMetaData( MetaDataNode **headNode, char *fileName,
                 char *instruction, char *logFilePath )
{
    FILE *filePointer;
    int strIndex, charAsInt;
    char tempCmdLetter;
    char strBuffer[ MAX_STR_LEN ], tempOperation[ STD_STR_LEN ];
    MetaDataNode *tempNode = malloc( sizeof( MetaDataNode ) );

    filePointer = fopen( fileName, READ_ONLY_FLAG );

    if( filePointer == NULL )
    {
        fclose( filePointer );
        return FILE_NOT_FOUND;
    }

    strIndex = 0;

    charAsInt = fgetc( filePointer );

    while( feof( filePointer ) == NOT_AT_FILE_END &&
           charAsInt != (int)( '\n' ) )
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

    while( feof( filePointer ) == NOT_AT_FILE_END )
    {
        charAsInt = fgetc( filePointer );

        if( feof( filePointer ) == NOT_AT_FILE_END )
        {
            while( charAsInt == (int)( SPACE ) ||
                   charAsInt == (int)( NEWLINE )
                 )
            {
                charAsInt = fgetc( filePointer );
            }

            if( charAsInt == (int)( 'E' ) )
            {
                strIndex = 0;

                while( charAsInt != (int)( PERIOD ) )
                {
                    strBuffer[ strIndex ] = (char)( charAsInt );
                    strIndex++;
                    strBuffer[ strIndex ] = NULL_CHAR;
                    charAsInt = fgetc( filePointer );
                }

                if( compareString( strBuffer,
                                   "End Program Meta-Data Code" ) == 0 )
                {
                    fclose( filePointer );
                    return logMetaData( *headNode, instruction, logFilePath );
                }
                else
                {
                    fclose( filePointer );
                    return FILE_END_ERROR;
                }
            }

            tempCmdLetter = (char)( charAsInt );

            charAsInt = fgetc( filePointer );
            if( charAsInt != (int)( OPEN_PAREN ) )
            {
                fclose( filePointer );
                return DATA_FORMAT_ERROR;
            }

            strIndex = 0;

            charAsInt = fgetc( filePointer );
            while( charAsInt != (char)( CLOSE_PAREN ) &&
                   feof( filePointer ) == NOT_AT_FILE_END )
            {
                tempOperation[ strIndex ] = (char)( charAsInt );
                strIndex++;
                tempOperation[ strIndex ] = NULL_CHAR;
                charAsInt = fgetc( filePointer );
            }

            strIndex = 0;

            charAsInt = fgetc( filePointer );
            while( charAsInt != (int)( SEMICOLON ) &&
                   charAsInt != (int)( PERIOD ) &&
                   feof( filePointer ) == NOT_AT_FILE_END )
            {
                strBuffer[ strIndex ] = (char)( charAsInt );
                strIndex++;
                strBuffer[ strIndex ] = NULL_CHAR;
                charAsInt = fgetc( filePointer );
            }

            if( operationIsValid( tempOperation ) == False )
            {
                fclose( filePointer );
                return DATA_ERROR;
            }

            tempNode = makeNode( tempCmdLetter,
                                 tempOperation,
                                 stringToInt( strBuffer ) );

            *headNode = addNode( *headNode, tempNode );

            free( tempNode );
        }
    }

    fclose( filePointer );
    return UNKNOWN_ERROR;
}

int logMetaData( MetaDataNode *headNode, char *instruction, char *logFilePath )
{
    if( compareString( instruction, "Monitor" ) == 0 ||
        compareString( instruction, "Both" )    == 0 )
    {
        MetaDataNode *currentNode = headNode;

        printf( "Meta-Data File Log\n");
        printf( "==================\n");

        while( currentNode != NULL )
        {
            printf( "The data item component letter is: ");
            printf( "%c\n", currentNode->commandLetter );
            printf( "The data item operation string is: ");
            printf( "%s\n", currentNode->operation );
            printf( "The data item cycle time is      : ");
            printf( "%d\n\n", currentNode->commandValue );

            currentNode = currentNode->next;
        }
    }

    if( compareString( instruction, "File" ) == 0 ||
        compareString( instruction, "Both" ) == 0 )
    {
        FILE *filePointer;

        filePointer = fopen( logFilePath, APPEND );

        MetaDataNode *currentNode = headNode;

        fprintf( filePointer, "Meta-Data File Log\n");
        fprintf( filePointer, "==================\n");

        while( currentNode != NULL )
        {
            fprintf( filePointer, "The data item component letter is: ");
            fprintf( filePointer, "%c\n", currentNode->commandLetter );
            fprintf( filePointer, "The data item operation string is: ");
            fprintf( filePointer, "%s\n", currentNode->operation );
            fprintf( filePointer, "The data item cycle time is      : ");
            fprintf( filePointer, "%d\n\n", currentNode->commandValue );

            currentNode = currentNode->next;
        }

        fclose( filePointer );
    }

    return NO_ERROR_MSG;
}

int operationIsValid( char *operation )
{
    if( compareString( operation, "access" )     == 0 ||
        compareString( operation, "allocate" )   == 0 ||
        compareString( operation, "end" )        == 0 ||
        compareString( operation, "hard drive" ) == 0 ||
        compareString( operation, "keyboard" )   == 0 ||
        compareString( operation, "printer" )    == 0 ||
        compareString( operation, "monitor" )    == 0 ||
        compareString( operation, "run" )        == 0 ||
        compareString( operation, "start" )      == 0 )
    {
        return True;
    }

    return False;
}

#endif
