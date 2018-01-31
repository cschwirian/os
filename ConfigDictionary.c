/**
  * ConfigDictionary.c
  *
  * Created by -REDACTED- on 17 January 2018
  *
  * Contains .cnf file data
  *
  * Requires ConfigLinkedList.h
  *
  * Version 1.0 31/1/2018
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

int getConfig( ConfigDictionary *config, const char *fileName )
{
    FILE *filePointer;
    int strIndex, dataStrIndex, charAsInt, stepIndex;
    char strBuffer[ MAX_STR_LEN ], dataStrBuffer[ STD_STR_LEN ];

    filePointer = fopen( fileName, READ_ONLY_FLAG );

    if( filePointer != NULL )
    {
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

        if( compareString( strBuffer,
                           "Start Simulator Configuration File" ) != 0 )
        {
            fclose( filePointer );
            return INIT_ERROR;
        }

        stepIndex = 0;

        while( feof( filePointer ) == NOT_AT_FILE_END )
        {
            charAsInt = fgetc( filePointer );

            strIndex = 0;

            while( charAsInt != (int)( COLON ) &&
                   feof( filePointer ) == NOT_AT_FILE_END )
            {
                strBuffer[ strIndex ] = (char)( charAsInt );
                strIndex++;
                strBuffer[ strIndex ] = NULL_CHAR;
                charAsInt = fgetc( filePointer );

                if( charAsInt == (int)( PERIOD ) )
                {
                    if( compareString( strBuffer,
                                     "End Simulator Configuration File" ) == 0 )
                    {
                        fclose( filePointer );
                        return logConfig( config, config->logFilePath );

                    }
                    else
                    {
                        fclose( filePointer );
                        return FILE_END_ERROR;
                    }
                }
            }

            charAsInt = fgetc( filePointer );

            while( charAsInt == (int)( SPACE ) )
            {
                charAsInt = fgetc( filePointer );
            }

            dataStrIndex = 0;

            while( charAsInt != (int)( NEWLINE ) &&
                   feof( filePointer ) == NOT_AT_FILE_END )
            {
                dataStrBuffer[ dataStrIndex ] = (char)( charAsInt );
                dataStrIndex++;
                dataStrBuffer[ dataStrIndex ] = NULL_CHAR;
                charAsInt = fgetc( filePointer );
            }

            switch( stepIndex )
            {
                case VERSION:
                    if( compareString( strBuffer, "Version/Phase" ) == 0 )
                    {
                        int tempVersion = stringToInt( dataStrBuffer );
                        if( versionIsValid( tempVersion ) )
                        {
                            config->versionNumber = tempVersion;
                        }
                        else
                        {
                            fclose( filePointer );
                            return DATA_ERROR;
                        }
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
                        if ( cpuCodeIsValid( dataStrBuffer ) )
                        {
                            if( compareString( dataStrBuffer, "NONE" ) == 0 )
                            {
                                copyString( config->schedulingCode, "FCFS-N" );
                            }
                            else
                            {
                                copyString( config->schedulingCode,
                                            dataStrBuffer );
                            }
                        }
                        else
                        {
                            fclose( filePointer );
                            return DATA_ERROR;
                        }
                    }
                    else
                    {
                        fclose( filePointer );
                        return DATA_ORDER_ERROR;
                    }
                    break;

                case QUANTUM_TIME:
                    if( compareString( strBuffer,
                                       "Quantum Time (cycles)" ) == 0 )
                    {
                        int tempQuantumTime = stringToInt( dataStrBuffer );
                        if( quantumTimeIsValid( tempQuantumTime ) )
                        {
                            config->quantumTime = tempQuantumTime;
                        }
                        else
                        {
                            fclose( filePointer );
                            return DATA_ERROR;
                        }
                    }
                    else
                    {
                        fclose( filePointer );
                        return DATA_ORDER_ERROR;
                    }
                    break;

                case MEMORY:
                    if( compareString( strBuffer,
                        "Memory Available (KB)" ) == 0 )
                    {
                        int tempMemory = stringToInt( dataStrBuffer );
                        if( memoryIsValid( tempMemory ) )
                        {
                            config->memoryAvailible = tempMemory;
                        }
                        else
                        {
                            fclose( filePointer );
                            return DATA_ERROR;
                        }
                    }
                    else
                    {
                        fclose( filePointer );
                        return DATA_ORDER_ERROR;
                    }
                    break;

                case P_CYCLE_TIME:
                    if( compareString( strBuffer,
                        "Processor Cycle Time (msec)" ) == 0 )
                    {
                        int tempCycle = stringToInt( dataStrBuffer );
                        if( processorCycleIsValid( tempCycle ) )
                        {
                            config->processorCycleTime = tempCycle;
                        }
                        else
                        {
                            fclose( filePointer );
                            return DATA_ERROR;
                        }
                    }
                    else
                    {
                        fclose( filePointer );
                        return DATA_ORDER_ERROR;
                    }
                    break;

                case IO_CYCLE_TIME:
                    if( compareString( strBuffer,
                        "I/O Cycle Time (msec)" ) == 0 )
                    {
                        int tempCycle = stringToInt( dataStrBuffer );
                        if( ioCycleIsValid( tempCycle ) )
                        {
                            config->ioCyleTime = tempCycle;
                        }
                        else
                        {
                            fclose( filePointer );
                            return DATA_ERROR;
                        }
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
                        if( logInstructionIsValid( dataStrBuffer ) )
                        {
                            copyString( config->logInstruction, dataStrBuffer );
                        }
                        else
                        {
                            fclose( filePointer );
                            return DATA_ERROR;
                        }
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
                        if( compareString( config->logInstruction,
                                           "MONITOR" ) == 0 )
                        {
                            copyString( config->logFilePath, "none" );
                        }
                        else
                        {
                            copyString( config->logFilePath, dataStrBuffer );
                        }
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
    else
    {
        fclose( filePointer );
        return FILE_NOT_FOUND;
    }

    fclose( filePointer );

    return UNKNOWN_ERROR;
}

int logConfig( ConfigDictionary *config, const char *fileName )
{
    if( compareString( config->logInstruction, "Both" ) == 0 ||
        compareString( config->logInstruction, "File" ) == 0 )
    {
        FILE *filePointer;

        filePointer = fopen( fileName, WRITE_ONLY_FLAG );

        fprintf( filePointer, "Config File Log\n");
        fprintf( filePointer, "===============\n");
        fprintf( filePointer, "Version            : ");
        fprintf( filePointer, "%d\n", config->versionNumber );
        fprintf( filePointer, "Program File Name  : ");
        fprintf( filePointer, "%s\n", config->filePath );
        fprintf( filePointer, "CPU Schedule Code  : ");
        fprintf( filePointer, "%s\n", config->schedulingCode );
        fprintf( filePointer, "Quantum Time       : ");
        fprintf( filePointer, "%d\n", config->quantumTime );
        fprintf( filePointer, "Process Cycle Time : ");
        fprintf( filePointer, "%d\n", config->memoryAvailible );
        fprintf( filePointer, "Memory Available   : ");
        fprintf( filePointer, "%d\n", config->processorCycleTime );
        fprintf( filePointer, "I/O Cycle Time     : ");
        fprintf( filePointer, "%d\n", config->ioCyleTime );
        fprintf( filePointer, "Log To             : ");
        fprintf( filePointer, "%s\n", config->logInstruction );
        fprintf( filePointer, "Log File Name      : ");
        fprintf( filePointer, "%s\n\n", config->logFilePath );

        fclose( filePointer );
    }

    if( compareString( config->logInstruction, "Monitor" ) == 0 ||
        compareString( config->logInstruction, "Both" )    == 0 )
    {
        printf( "Config File Log\n");
        printf( "===============\n");
        printf( "Version            : ");
        printf( "%d\n", config->versionNumber );
        printf( "Program File Name  : ");
        printf( "%s\n", config->filePath );
        printf( "CPU Schedule Code  : ");
        printf( "%s\n", config->schedulingCode );
        printf( "Quantum Time       : ");
        printf( "%d\n", config->quantumTime );
        printf( "Process Cycle Time : ");
        printf( "%d\n", config->memoryAvailible );
        printf( "Memory Available   : ");
        printf( "%d\n", config->processorCycleTime );
        printf( "I/O Cycle Time     : ");
        printf( "%d\n", config->ioCyleTime );
        printf( "Log To             : ");
        printf( "%s\n", config->logInstruction );
        printf( "Log File Name      : ");
        printf( "%s\n\n", config->logFilePath );
    }

    return NO_ERROR_MSG;

}

int versionIsValid( int versionNumber )
{
    if( versionNumber >= 0 && versionNumber <= 10 )
    {
        return True;
    }

    return False;
}

int cpuCodeIsValid( char *cpuCode )
{
    if( compareString( cpuCode, "NONE" )   == 0 ||
        compareString( cpuCode, "FCFS-N" ) == 0 ||
        compareString( cpuCode, "SJF-N" )  == 0 ||
        compareString( cpuCode, "SRTF-P" ) == 0 ||
        compareString( cpuCode, "FCFS-P" ) == 0 ||
        compareString( cpuCode, "RR-P" )   == 0 )
    {
        return True;
    }

    return False;
}

int quantumTimeIsValid( int quantumTime )
{
    if( quantumTime >= 0 && quantumTime <= 100 )
    {
        return True;
    }

    return False;
}

int memoryIsValid( int memory )
{
    if( memory >= 0 && memory <= 1048576 )
    {
        return True;
    }

    return False;
}

int processorCycleIsValid( int processorCycle )
{
    if( processorCycle >= 0 && processorCycle <= 1000 )
    {
        return True;
    }

    return False;
}

int ioCycleIsValid( int ioCycle )
{
    if( ioCycle >= 0 && ioCycle <= 10000 )
    {
        return True;
    }

    return False;
}

int logInstructionIsValid( char *instruction )
{
    if( compareString( instruction, "Monitor" ) == 0 ||
        compareString( instruction, "File" )    == 0 ||
        compareString( instruction, "Both" )    == 0 )
    {
        return True;
    }

    return False;
}

#endif
