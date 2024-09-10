#ifndef LOG_PRINTER_H
#define LOG_PRINTER_H


#include <stdarg.h>
#include <stdio.h>


/**
 * 
 */
#define GetPlace()                 \
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
void LogPrint(Place place, logMode_t logMode, const char* message, ...);


#endif // LOG_PRINTER_H