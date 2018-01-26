/**
  * ConfigDictionary.c
  *
  * Created by Connor Schwirian on 17 January 2018
  *
  * Contains .cnf file data
  *
  * Requires ConfigLinkedList.h
  *
  * Version 0.1 (Development) 17/1/2018
**/

#ifndef ConfigDictionary_C
#define ConfigDictionary_C

#include "ConfigDictionary.h"

void clearDictionary( ConfigDictionary *config )
{
    free( config );
    config = NULL;
}

int getConfigFromFile( ConfigDictionary *config, const char *fileName )
{
    FILE *filePointer;
    int strIndex, dataStrIndex, charAsInt, stepIndex;
    char strBuffer[ MAX_STR_LEN ], dataStrBuffer[ STD_STR_LEN ];
    Boolean inProgress = True;

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

        if( compareString( strBuffer, "Start Simulator Configuration File" ) != 0 )
        {
            fclose( filePointer );
            return INIT_ERROR;
        }

        stepIndex = 0;

        while( inProgress == True )
        {
            // Push off the newline.
            charAsInt = fgetc( filePointer );

            if( feof( filePointer ) == NOT_AT_FILE_END )
            {
                strIndex = 0;

                while( charAsInt != (int)( COLON ) )
                {
                    strBuffer[ strIndex ] = (char)( charAsInt );

                    strIndex++;

                    strBuffer[ strIndex ] = NULL_CHAR;

                    charAsInt = fgetc( filePointer );

                    if( charAsInt == (int)( '.' ) )
                    {
                        if( compareString( strBuffer, "End Simulator Configuration File" ) == 0 )
                        {
                            fclose( filePointer );
                            return NO_ERROR_MSG;
                        }
                        else
                        {
                            fclose( filePointer );
                            return FILE_END_ERROR;
                        }
                    }
                }

                printf( "%s\n", strBuffer );

                // Push off colon.
                charAsInt = fgetc( filePointer );

                // Skip whitespace.
                while( charAsInt == (int)( SPACE ) )
                {
                    charAsInt = fgetc( filePointer );
                }

                dataStrIndex = 0;

                while( charAsInt != (int)( NEWLINE ) )
                {
                    dataStrBuffer[ dataStrIndex ] = (char)( charAsInt );

                    dataStrIndex++;

                    dataStrBuffer[ dataStrIndex ] = NULL_CHAR;

                    charAsInt = fgetc( filePointer );
                }

                printf( "%s\n", dataStrBuffer );

                switch( stepIndex )
                {
                    case VERSION:
                        if( compareString( strBuffer, "Version/Phase" ) == 0 )
                        {
                            config->versionNumber =  stringToInt( dataStrBuffer );
                        }
                        else
                        {
                            fclose( filePointer );
                            return DATA_ORDER_ERROR;
                        }
                        break;
                    case FILE_PATH:
                        if( compareString( strBuffer, "File Path" ) == 0 )
                        {
                            copyString( config->filePath, dataStrBuffer );
                        }
                        else
                        {
                            fclose( filePointer );
                            return DATA_ORDER_ERROR;
                        }
                        break;
                    case CPU_CODE:
                        if( compareString( strBuffer, "CPU Scheduling Code" ) == 0 )
                        {
                            copyString( config->schedulingCode, dataStrBuffer );
                        }
                        else
                        {
                            fclose( filePointer );
                            return DATA_ORDER_ERROR;
                        }
                        break;
                    case QUANTUM_TIME:
                        if( compareString( strBuffer, "Quantum Time (cycles)" ) == 0 )
                        {
                            config->quantumTime = stringToInt( dataStrBuffer );
                        }
                        else
                        {
                            fclose( filePointer );
                            return DATA_ORDER_ERROR;
                        }
                        break;
                    case MEMORY:
                        if( compareString( strBuffer, "Memory Available (KB)" ) == 0 )
                        {
                            config->memoryAvailible = stringToInt( dataStrBuffer );
                        }
                        else
                        {
                            fclose( filePointer );
                            return DATA_ORDER_ERROR;
                        }
                        break;
                    case P_CYCLE_TIME:
                        if( compareString( strBuffer, "Processor Cycle Time (msec)" ) == 0 )
                        {
                            config->processorCycleTime = stringToInt( dataStrBuffer );
                        }
                        else
                        {
                            fclose( filePointer );
                            return DATA_ORDER_ERROR;
                        }
                        break;
                    case IO_CYCLE_TIME:
                        if( compareString( strBuffer, "I/O Cycle Time (msec)" ) == 0 )
                        {
                            config->ioCyleTime = stringToInt( dataStrBuffer );
                        }
                        else
                        {
                            fclose( filePointer );
                            return DATA_ORDER_ERROR;
                        }
                        break;
                    case LOG_INSTRUCTION:
                        if( compareString( strBuffer, "Log To" ) == 0 )
                        {
                            copyString( config->logInstruction, dataStrBuffer );
                        }
                        else
                        {
                            fclose( filePointer );
                            return DATA_ORDER_ERROR;
                        }
                        break;
                    case LOG_PATH:
                        if( compareString( strBuffer, "Log File Path" ) == 0 )
                        {
                            copyString( config->logFilePath, dataStrBuffer );
                        }
                        else
                        {
                            fclose( filePointer );
                            return DATA_ORDER_ERROR;
                        }
                        break;
                }

                stepIndex++;
            }
        }
    }

    fclose( filePointer );

    return -1;
}

int logConfigData( ConfigDictionary *config, const char *fileName )
{
    return 1;
}

#endif
