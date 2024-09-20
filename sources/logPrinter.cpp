#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#include "../headers/logPrinter.h"


//----------------------------------------------------------------------------------------


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
 * Codes used to set colors 
 * for ColoredPrint() and 
 * PrintColor().
 */
const char* COLOR_RED    = "\x1b[31m";
const char* COLOR_GREEN  = "\x1b[32m";
const char* COLOR_YELLOW = "\033[1;33m";
const char* COLOR_WHITE  = "\033[1;37m";
const char* COLOR_RESET  = "\x1b[0m";


//----------------------------------------------------------------------------------------


/**
 * This function check if log is opened.
 * 
 * @return true if it's opened.
 * @return false if it's closed.
 */
static bool IsLogOpen();


/**
 * This function print name of mode to log file.
 * 
 * @param logMode Mode of log note.
 */
static const char* LogPrintMode(const FILE* outputFile, const logMode_t logMode);


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
static void LogEmergencyPrint(const Place place, const char* message);


/**
 * This function print time to outputFile
 * without using '\n'.
 * 
 * @param outputFile File where time will be printed.
 */
static void LogPrintTime(FILE* outputFile);


/**
 * This function print place to outputFile
 * without using '\n'.
 * 
 * @param outputFile File where place will be printed.
 * @param place      Pointer to place which will b printed.
 */
static void LogPrintPlace(FILE* outputFile, const Place* place);


/**
 * This time print info about log message
 * Template of printing:
 * 
 * logMode: place: date at time: '\n'.
 * 
 * @param outputFile File where oifo will be printed.
 * @param logMod     Mode of loging.
 * @param place      Pointer to place which will be printed.
 */
static void LogPrintInfo(FILE* outputFile, const logMode_t logMode, const Place* place);


/** 
 * Print needed color (used defines).
 * 
 * This function used to make 
 * ColoredPrintf() more universal.
 * 
 * @param color Color you want to set.
 */
static void PrintColor(const color_t color);


//----------------------------------------------------------------------------------------


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

    fprintf(logFile, "\n\n\\* This log was created ");
    LogPrintTime(logFile);
    fprintf(logFile, "*\\\n\n");
}


void LogClose(const Place place)
{
    if (IsLogOpen())
    {
        fprintf(logFile, "\n\\* This log was closed ");
        LogPrintTime(logFile);
        fprintf(logFile, "*\\\n\n\n\n");
                        
        fclose(logFile);
        logFile = NULL;
    }
    else 
        LogEmergencyPrint(place, "You are trying to close log that is already closed.");
}


void LogPrint(logMode_t logMode, Place place, const char* message, ...) 
{   
    if (!IsLogOpen())
        LogEmergencyPrint(place, "You are trying LOG_PRINT(), but you didn't "
                                 "use LogOpen() before. Try to use it in main.cpp.");

    va_list messageArgs;
    va_start(messageArgs, message);

    LogPrintInfo(logFile, logMode, &place);
    vfprintf(logFile, message, messageArgs);
    fprintf(logFile, "\n");

    va_end(messageArgs);
}


int ColoredPrintf(color_t color, const char* format, ...) {
    va_list args;
    va_start(args, format);

    PrintColor(color);
    int result = vprintf(format, args);
    printf("%s", COLOR_RESET);
    
    va_end(args);
    return result;
}


//----------------------------------------------------------------------------------------


static bool IsLogOpen()
{
    if (logFile == NULL)
        return false;
    
    return true;
}


static void LogPrint(FILE* outputFile, const logMode_t logMode) 
{
    switch (logMode) 
    {
    case ERROR:
        fprintf(outputFile, "ERROR: ");
        break;

    case WARNING:
        fprintf(outputFile, "WARNING: ");
        break;
        
    case INFO:
        fprintf(outputFile, "INFO");
        break;

    default:
        LogEmergencyPrint(GET_PLACE(), "WRONG LOG PRINT MODE");
        break;
    }
}


static void LogEmergencyPrint(const Place place, const char* message) 
{
    FILE* logEmergencyFile = fopen(logEmergencyFileName, "a");
    setvbuf(logFile, NULL, _IONBF, 0);

    LogPrintInfo(logEmergencyFile, ERROR, &place);
    fprintf(logEmergencyFile, "\t%s\n", message);

    fclose(logEmergencyFile);
}


static void PrintColor(const color_t color) {
    switch(color) 
    {
    case GREEN:
        printf("%s", COLOR_GREEN);
        break;

    case RED:
        printf("%s", COLOR_RED);
        break;

    case YELLOW:
        printf("%s", COLOR_YELLOW);
        break;

    case WHITE:
        printf("%s", COLOR_WHITE);
        break;

    default: 
        LogEmergencyPrint(GET_PLACE(), "Wrong color code.");
        break;
    }
}


static void LogPrintTime(FILE* outputFile)
{
    time_t myTime      = time(NULL);
    tm*    myLocalTime = localtime(&myTime);

    fprintf(outputFile, "%d.%d.%d at %d:%d:%d: ",
            myLocalTime->tm_mday,  myLocalTime->tm_mon+1, myLocalTime->tm_year+1900,
            myLocalTime->tm_hour,  myLocalTime->tm_min,   myLocalTime->tm_sec);
}


static void LogPrintPlace(FILE* outputFile, const Place* place)
{
    fprintf(outputFile, "%s: in %s: %s(): line %d: ",
            place->file, place->function, place->line);
}


static void LogPrintInfo(FILE* outputFile, const logMode_t logMode, const Place* place)
{
    LogPrintMode(outputFile, logMode);
    LogPrintTime(outputFile);
    LogPrintPlace(outputFile, place);
    fprintf(outputFile, "\n");
}