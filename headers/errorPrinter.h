#ifndef ERROR_PRINTER_H
#define ERROR_PRINTER_H


#include <stdarg.h>
#include <stdio.h>


/**
 * This function print your own 
 * error message. Also this function
 * print FILE, FUNCTION and LINE 
 * where it was called.
 * 
 * @param __VA_ARGS__ There should
 * be const char* format and arguments
 * like in printf().
 * 
 * @return Count of output chars, 
 * @return Negative number if error.
 */
#define ErrorPrint(__VA_ARGS__ ...)                  \
{                                                    \
    fprintf(stderr, "%s: %s(): ERROR in line %d. ",  \
            __FILE__, __FUNCTION__, __LINE__);       \
    fprintf(stderr, __VA_ARGS__);                    \
}


#endif // ERROR_PRINTER_H