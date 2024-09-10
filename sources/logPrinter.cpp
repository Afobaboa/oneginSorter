#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include "../headers/logPrinter.h"

/**
 * 
 */
static FILE* logFile = NULL;


/**
 * 
 */
static char* logFileName = "log.txt";


/**
 * 
 */
static bool IsLogOpen();


/**
 * 
 */
static const char* LogModeGetName(const logMode_t logMode);


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

    time_t myTime      = time(NULL);
    tm*    myLocalTime = localtime(&myTime);

    fprintf(logFile, "This log was created %d.%d.%d at %d:%d:%d\n\n",
                     myLocalTime->tm_mday,  myLocalTime->tm_mon+1, myLocalTime->tm_year+1900,
                     myLocalTime->tm_hour, myLocalTime->tm_min,   myLocalTime->tm_sec);
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


void LogPrint(logMode_t logMode, Place place, const char* message, ...) 
{
    va_list messageArgs;
    va_start(messageArgs, message);

    fprintf(logFile, "%s: in %s: %s(): line %d: \n\t", LogModeGetName(logMode),
                     place.file, place.function, place.line);
    vfprintf(logFile, message, messageArgs);
    fprintf(logFile, "\n");

    va_end(messageArgs);
}


static const char* LogModeGetName(const logMode_t logMode) 
{
    switch (logMode) 
    {
    case ERROR:
        return "ERROR";
    case WARNING:
        return "WARNING";
    case INFO:
        return "INFO";
    default:
        return "WRONG MODE";
    }
}