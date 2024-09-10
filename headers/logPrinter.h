#ifndef LOG_PRINTER_H
#define LOG_PRINTER_H


#include <stdarg.h>
#include <stdio.h>


/**
 * 
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
 * 
 */
#define LOG_PRINT(logMode, ...)                     \
{                                                   \
    LogPrint(logMode, GET_PLACE(), __VA_ARGS__);    \
}                                                   \


/**
 * 
 */
struct Place
{
    const char* file;        /**<  */
    const char* function;    /**<  */
    const int   line;        /**<  */
};


/**
 * 
 */
enum LOG_MODES 
{
    ERROR,      /**< */
    WARNING,    /**< */
    INFO        /**<  */
};
typedef enum LOG_MODES logMode_t;


/**
 * 
 */
void LogSetFileName(const char* newLogFileName);


/**
 * 
 */
void LogOpen();


/**
 * 
 */
void LogClose();


/**
 * 
 */
void LogPrint(logMode_t logMode, Place place, const char* message, ...);


#endif // LOG_PRINTER_H