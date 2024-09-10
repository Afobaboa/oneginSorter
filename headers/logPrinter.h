#ifndef LOG_PRINTER_H
#define LOG_PRINTER_H


#include <stdarg.h>
#include <stdio.h>


/**
 * This function automaticaly gets
 * file, function and line information
 * about plac where it was used and
 * wraps that information into struct Place.
 */
#define GET_PLACE()                \
{                                  \
    (struct Place)                 \
    {                              \
        .file     = __FILE__,      \
        .function = __FUNCTION__,  \
        .line     = __LINE__       \
    }                              \
}                                  \


/**
 * This function works like LogPrint()
 * but it's automaticaly place GET_PLACE()
 * to arguments. 
 * 
 * That provides you more 
 * convinient syntax.
 */
#define LOG_PRINT(logMode, ...)                     \
{                                                   \
    LogPrint(logMode, GET_PLACE(), __VA_ARGS__);    \
}                                                   \


/**
 * This struct proides you convinient
 * way to keep information about places in code.
 */
struct Place
{
    const char* file;        /**< Name of file.     */
    const char* function;    /**< Name of function. */
    const int   line;        /**< Number of line.   */
};


/**
 * There are log modes which 
 * used for notes in log records.
 */
enum LOG_MODES 
{
    ERROR,      /**< ERROR   */
    WARNING,    /**< WARNING */
    INFO        /**< INFO    */
};
typedef enum LOG_MODES logMode_t;


/**
 * This function will set path to 
 * log file from "logs/log.txt" to
 * your own. If default log file is
 * already open, this function will
 * print special message to 
 * "logs/emergencyLog.txt".
 * 
 * DONT'T MISS FILE EXTENSION.
 */
void LogSetFileName(const char* newLogFileName);


/**
 * This function open log file.
 * If this file is already opened,
 * this function print special message
 * to "logs/emergencyLog.txt".
 */
void LogOpen();


/**
 * This function close log file.
 * If this file is already closeed,
 * this function print special message
 * to "logs/emergencyLog.txt".
 */
void LogClose();


/**
 * This function print your message
 * to "logs/log.txt" by default (you can
 * chane log's output file by 
 * LogSetFileName()). Also it prints
 * Place where it was used.
 * 
 * @param logMode Note to log message.
 * @param place   Place that will be printed.
 * @param message Message with fomat like in printf(). 
 *                Next arguments will be used in message
 */
void LogPrint(logMode_t logMode, Place place, const char* message, ...);


#endif // LOG_PRINTER_H