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
static char* logFileName = "logs/log.txt";


/**
 * 
 */
static bool IsLogOpen();


/**
 * 
 */
static const char* LogModeGetName(const logMode_t logMode);


/**
 * 
 */
void LogEmergencyPrint(const char* message);


void LogSetFileName(const char* newLogFileName)
{
    if (IsLogOpen())
        LOG_PRINT(WARNING, "You can't change log file name "
                           "because log is already open.");

    logFileName = (char*) newLogFileName;
}


void LogOpen() 
{
    if (IsLogOpen())
        return;
    
    logFile = fopen(logFileName, "a");

    time_t myTime      = time(NULL);
    tm*    myLocalTime = localtime(&myTime);

    fprintf(logFile, "\n\\* This log was created %d.%d.%d at %d:%d:%d *\\\n",
                     myLocalTime->tm_mday,  myLocalTime->tm_mon+1, myLocalTime->tm_year+1900,
                     myLocalTime->tm_hour, myLocalTime->tm_min,   myLocalTime->tm_sec);
}


void LogClose()
{
    if (IsLogOpen())
    {
        fclose(logFile);
        logFile = NULL;
    }
    else 
        LogEmergencyPrint("You are trying to close closed log.");
}


static bool IsLogOpen()
{
    if (logFile == NULL)
        return false;
    
    return true;
}


void LogPrint(logMode_t logMode, Place place, const char* message, ...) 
{   
    if (!IsLogOpen())
        LogEmergencyPrint("You are trying LOG_PRINT(), but you didn't "
                          "use LogOpen() before. Try to use it in main.cpp.");

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


void LogEmergencyPrint(const char* message) 
{
    FILE* emergencyLogFile = fopen("logs/emergencyLog.txt", "a");

    time_t myTime      = time(NULL);
    tm*    myLocalTime = localtime(&myTime);

    fprintf(emergencyLogFile, "\n %d.%d.%d at %d:%d:%d:\n \t%s\n",
            myLocalTime->tm_mday,  myLocalTime->tm_mon+1, myLocalTime->tm_year+1900,
            myLocalTime->tm_hour,  myLocalTime->tm_min,   myLocalTime->tm_sec,
            message);

    fclose(emergencyLogFile);
}