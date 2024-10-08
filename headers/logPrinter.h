/**
 * @file 
 * For correct working of rhis file
 * use OPEN_LOG() at the beginning 
 * of main() and close it before 
 * return in main().
 * 
 * Use macroses for convinient
 * using of logginng.
 * 
 * If you want to switch off loging
 * without removing code just add
 * 
 * #define LOG_SWITCH_OFF
 * 
 * in main file, where you are
 * opening log. 
 * 
 * DEFINE IT ONLY BEFORE INCLUDING
 * ANY FILES, ESLE LOG WON'T 
 * BE SWITCHED OFF!!!
 */


#ifndef LOG_PRINTER_H
#define LOG_PRINTER_H


//----------------------------------------------------------------------------------------


/**
 * This function automaticaly gets
 * file, function and line information
 * about plac where it was used and
 * wraps that information into struct Place.
 */
#define GET_PLACE()                \
    (struct Place)                 \
    {                              \
        .file     = __FILE__,      \
        .function = __FUNCTION__,  \
        .line     = __LINE__       \
    }                              \


#ifndef LOG_SWITCH_OFF


/**
 * This function works like LogPrint()
 * but it's automaticaly place GET_PLACE()
 * to arguments. 
 * 
 * That provides you more 
 * convinient syntax.
 */
#define LOG_PRINT(logMode, ...)                  \
    LogPrint(logMode, GET_PLACE(), __VA_ARGS__); \


/**
 * This definition provides you more 
 * convinient way to use logSetFileName().
 */
#define LOG_SET_FILE_NAME(newLogFileName)         \
    LogSetFileName(GET_PLACE(), newLogFileName); \


/**
 * This definition provides you more
 * convinient way to use logOpen().
 * 
 * Also this function will print
 * date, time and place, where and
 * when log was opened.
 */
#define LOG_OPEN()        \
    LogOpen(GET_PLACE()); \


/**
 * This definition provides you more
 * convinient way to use logClosw().
 * 
 * Also this function will print
 * date, time and place, where and
 * when log was closed
 */
#define LOG_CLOSE()        \
    LogClose(GET_PLACE()); \


#else


/** 
 * Where are switching off
 * all log's defines.
 */
#define LOG_PRINT(logMode, ...) ;
#define LOG_SET_FILE_NAME(newLogFileName) ;
#define LOG_OPEN() ;
#define LOG_CLOSE() ;


#endif // LOG_SWITCHING_OFF


//----------------------------------------------------------------------------------------



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


//----------------------------------------------------------------------------------------


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
 * This colores used for
 * ColoredPrintf() to set
 * needed color.
 */
enum COLORS {
    GREEN,   /**< Green color. */
    RED,     /**< Red color.   */
    YELLOW,  /**< Yellow color */
    WHITE    /**< White color  */
};
typedef enum COLORS color_t;


//----------------------------------------------------------------------------------------


/**
 * This function will set path to 
 * log file from "logs/log.txt" to
 * your own. If default log file is
 * already open, this function will
 * print special message to 
 * "logs/emergencyLog.txt".
 * 
 * DONT'T MISS FILE EXTENSION.
 * 
 * @param place          Place that will be printed
 *                       in emergency log file.
 * @param newLogFileName New value of logFileName.
 */
void LogSetFileName(const Place place, const char* newLogFileName);


/**
 * This function open log file.
 * If this file is already opened,
 * this function print special message
 * to "logs/emergencyLog.txt".
 * 
 * @param place Place that will be printed
 *              in emergency log file.
 */
void LogOpen(const Place place);


/**
 * This function close log file.
 * If this file is already closeed,
 * this function print special message
 * to "logs/emergencyLog.txt".
 * 
 * @param place Place that will be printed
 *              in emergency log file.
 */
void LogClose(const Place place);


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


/**
 * This function is similary
 * to printf(), but this function
 * can print colored text.
 * 
 * @param color Color of text. You have
 * to use something from enum COLORS.
 * @param format Format of your text.
 * You have to use printf() format
 * 
 * Other arguments used for subtitution
 * to format. You have to use as many
 * arguments as you us in format, like 
 * in printf().
 * 
 * @return Count of used other arguments.
 */
int ColoredPrintf(color_t color, const char* format, ...);


//----------------------------------------------------------------------------------------


#endif // LOG_PRINTER_H