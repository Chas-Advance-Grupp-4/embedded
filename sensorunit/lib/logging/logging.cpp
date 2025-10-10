#include "config.h"
#include "logging.h"

#ifdef LOG_LEVEL
LogLevel currentLogLevel = LOG_LEVEL;
#else
LogLevel currentLogLevel = LOG_INFO;
#endif