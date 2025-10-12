/**
 * @file logging.h
 * @author Erik Dahl (erik@iunderlandet.se)
 /**
 * @file logging.h
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Lightweight logging macros for Arduino-based projects using PlatformIO.
 *
 * Provides configurable log level filtering and formatted output via the serial console.
 * Designed to resemble the ESP-style logging used in the Control Unit repository.
 * Enables consistent tagging and severity-based logging for debugging and runtime diagnostics.
 *
 * Depends on the LibPrintf library.
 * To use these macros, add the following to your platformio.ini:
 *     lib_deps = embeddedartistry/LibPrintf@^1.2.13
 *
 * @date 2025-10-10
 * 
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 * 
 */
#pragma once
#include <Arduino.h>
#include <LibPrintf.h>
#include "config.h"

/**
 * @enum LogLevel
 * @brief Defines severity levels for log output.
 *
 * Used to categorize log messages by importance and filter them
 * based on the current verbosity setting.
 */
enum LogLevel {
    LOG_NONE = 0,
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG
};

/**
 * @brief Global variable that defines the current logging verbosity level.
 *
 * Used by the LOG macros to determine whether a message should be printed.
 * Declared here as extern and defined in logging.cpp.
 * Set your desired log level in config.h.
 */
extern LogLevel currentLogLevel;

/**
 * @def LOG(level, tag, fmt, ...)
 * @brief Generic logging macro for formatted output to the serial console.
 *
 * Prints a log message if the specified level is equal to or lower than
 * the currentLogLevel. Includes timestamp, severity, and tag.
 *
 * @param level The severity level of the message (see LogLevel).
 * @param tag   A short identifier or module name for the log source.
 * @param fmt   A printf-style format string.
 * @param ...   Optional arguments to be formatted into the message.
 */

#ifdef LOG_COLORS
#define LOG(level, tag, fmt, ...) \
    do { \
        if (level != LOG_NONE && level <= currentLogLevel) { \
            const char* color = ""; \
            const char* reset = "\033[0m"; \
            char levelChar = '?'; \
            switch (level) { \
                case LOG_ERROR: levelChar = 'E'; color = "\033[31m"; break; \
                case LOG_WARN:  levelChar = 'W'; color = "\033[33m"; break; \
                case LOG_INFO:  levelChar = 'I'; color = "\033[32m"; break; \
                case LOG_DEBUG: levelChar = 'D'; color = "\033[36m"; break; \
            } \
            printf("%s%c (%lu) %s: " fmt "%s\n", color, levelChar, millis(), tag, ##__VA_ARGS__, reset); \
        } \
    } while (0)
#else
#define LOG(level, tag, fmt, ...) \
    do { \
        if (level != LOG_NONE && level <= currentLogLevel) { \
            char levelChar = '?'; \
            switch (level) { \
                case LOG_ERROR: levelChar = 'E'; break; \
                case LOG_WARN:  levelChar = 'W'; break; \
                case LOG_INFO:  levelChar = 'I'; break; \
                case LOG_DEBUG: levelChar = 'D'; break; \
            } \
            printf("%c (%lu) %s: " fmt "\n", levelChar, millis(), tag, ##__VA_ARGS__); \
        } \
    } while (0)
#endif

/**
 * @def LOG_ERROR(tag, fmt, ...)
 * @brief Logs a message with error severity.
 *
 * Equivalent to calling LOG(LOG_ERROR, tag, fmt, ...).
 */
#define LOG_ERROR(tag, fmt, ...) LOG(LOG_ERROR, tag, fmt, ##__VA_ARGS__)

/**
 * @def LOG_WARN(tag, fmt, ...)
 * @brief Logs a message with warning severity.
 *
 * Equivalent to calling LOG(LOG_WARN, tag, fmt, ...).
 */
#define LOG_WARN(tag, fmt, ...)  LOG(LOG_WARN, tag, fmt, ##__VA_ARGS__)

/**
 * @def LOG_INFO(tag, fmt, ...)
 * @brief Logs a message with informational severity.
 *
 * Equivalent to calling LOG(LOG_INFO, tag, fmt, ...).
 */
#define LOG_INFO(tag, fmt, ...)  LOG(LOG_INFO, tag, fmt, ##__VA_ARGS__)

/**
 * @def LOG_DEBUG(tag, fmt, ...)
 * @brief Logs a message with debug severity.
 *
 * Equivalent to calling LOG(LOG_DEBUG, tag, fmt, ...).
 */
#define LOG_DEBUG(tag, fmt, ...) LOG(LOG_DEBUG, tag, fmt, ##__VA_ARGS__)