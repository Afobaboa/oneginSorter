#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include "../headers/logPrinter.h"

/**
 * There is pointer to log file.
 */
static FILE* logFile = NULL;


/**
 * There is name of log file. 
 * You can change it yourself by
 * LogSetFileName() before opening file.
 */
static char* logFileName = "logs/log.txt";


/**
 * There is name of emergency log
 * file. You can't change its name
 * from other files.
 */ 
static const char* logEmergencyFileName = "logs/emergencyLog.txt";


/**
 * This function check if log is opened.
 * 
 * @return true if it's opened.
 * @return false if it's closed.
 */
static bool IsLogOpen();


/**
 * This function get name of mode
 * for printing it to log file.
 * 
 * @param logMode Mode of log note.
 * 
 * @return Name of logMode changed to char*.
 */
static const char* LogModeGetName(const logMode_t logMode);


/**
 * This function help you to handle
 * hidden errors with using logs in 
 * your program.
 * 
 * That prints message to special file
 * "logs/emergencyLog.txt". You can't 
 * change this path from other files.
 * 
 * @param message Message tha will be printed.
 * @param place   Place that will be printed.
 */
void LogEmergencyPrint(const Place place, const char* message);


/**
 * 
 */
void LogPrintTime(const FILE* file);


/**
 * 
 */
void LogPrintPlace(const Place* place);


void LogSetFileName(const Place place, const char* newLogFileName)
{
    if (IsLogOpen())
    {
        LogEmergencyPrint(place, "You can't change log file name "
                                 "because log is already open.");
        return;
    }   

    logFileName = (char*) newLogFileName;
}


void LogOpen(const Place place) 
{
    if (IsLogOpen())
    {
        LogEmergencyPrint(place, "You trying to open file that is already opened.");
        return;
    }
    
    logFile = fopen(logFileName, "a");
    setvbuf(logFile, NULL, _IONBF, 0);

    time_t myTime      = time(NULL);
    tm*    myLocalTime = localtime(&myTime);

    fprintf(logFile, "\n\n\\* This log was created %d.%d.%d at %d:%d:%d *\\\n\n",
                     myLocalTime->tm_mday,  myLocalTime->tm_mon+1, myLocalTime->tm_year+1900,
                     myLocalTime->tm_hour, myLocalTime->tm_min,   myLocalTime->tm_sec);
}


void LogClose(const Place place)
{
    if (IsLogOpen())
    {
        time_t myTime      = time(NULL);
        tm*    myLocalTime = localtime(&myTime);

        fprintf(logFile, "\\* This log was closed %d.%d.%d at %d:%d:%d *\\\n\n\n\n",
                        myLocalTime->tm_mday,  myLocalTime->tm_mon+1, myLocalTime->tm_year+1900,
                        myLocalTime->tm_hour, myLocalTime->tm_min,   myLocalTime->tm_sec);
                        
        fclose(logFile);
        logFile = NULL;
    }
    else 
        LogEmergencyPrint(place, "You are trying to close log that is already closed.");
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
        LogEmergencyPrint(place, "You are trying LOG_PRINT(), but you didn't "
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


void LogEmergencyPrint(const Place place, const char* message) 
{
    FILE* logEmergencyFile = fopen(logEmergencyFileName, "a");
    setvbuf(logFile, NULL, _IONBF, 0);

    time_t myTime      = time(NULL);
    tm*    myLocalTime = localtime(&myTime);

    fprintf(logEmergencyFile, "\n%d.%d.%d at %d:%d:%d:\n"
                              "in %s: %s(): line %d:\n "
                              "\t%s\n",
            myLocalTime->tm_mday,  myLocalTime->tm_mon+1, myLocalTime->tm_year+1900,
            myLocalTime->tm_hour,  myLocalTime->tm_min,   myLocalTime->tm_sec,

            place.file, place.function, place.line,

            message);

    fclose(logEmergencyFile);
}