#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include "../headers/logPrinter.h"


static FILE* logFile     = NULL;
static char* logFileName = "log.txt";


static bool IsLogOpen();


void LogSetFileName(const char* newLogFileName)
{
    if (IsLogOpen())
        fprintf(stderr, "You can't change log file name "
                        "because log is alreade open.\n");

    logFileName = (char*) newLogFileName;
}


void LogOpen() 
{
    if (IsLogOpen())
        return;
    
    logFile = fopen(logFileName, "w");
}


void LogClose()
{
    if (IsLogOpen())
        fclose(logFile);
}


static bool IsLogOpen()
{
    if (logFile == NULL)
        return false;
    
    return true;
}


void LogPrint(Place place, logMode_t logMode, const char* message, ...) 
{

}