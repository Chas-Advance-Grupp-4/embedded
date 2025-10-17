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

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_INFO
#endif

/**
 * @brief Global variable that defines the current logging verbosity level.
 *
 * Used by the LOG macros and logMessage() to determine whether a message should be printed.
 * You can override the default level by defining LOG_LEVEL in config.h before including logging.h.
 */
inline LogLevel currentLogLevel = LOG_LEVEL;

/**
 * @brief Logs a formatted message to the serial console with optional ANSI color and severity filtering.
 *
 * This templated function provides the core implementation for all log output.
 * It prints a timestamped message prefixed with a severity character (E/W/I/D) and a user-defined tag.
 * Messages are filtered based on the global `currentLogLevel`, and optionally color-coded
 * if `LOG_COLORS` is defined.
 *
 * Designed to be used via the `LOG()` macro or its severity-specific wrappers (`LOG_ERROR`, `LOG_WARN`, etc.).
 *
 * @tparam Args Variadic template arguments matching the format string.
 * @param level The severity level of the message (e.g., LOG_ERROR, LOG_INFO).
 * @param tag   A short identifier for the log source (e.g., module or filename).
 * @param fmt   A printf-style format string.
 * @param args  Optional arguments to be formatted into the message.
 */
template<typename... Args>
inline void logMessage(LogLevel level, const char* tag, const char* fmt, Args... args) {
    if (level == LOG_NONE || level > currentLogLevel) return;

    const char* color = "";
    const char* reset = "";
    char levelChar = '?';

#ifdef LOG_COLORS
    reset = "\033[0m";
    switch (level) {
        case LOG_ERROR: levelChar = 'E'; color = "\033[31m"; break;
        case LOG_WARN:  levelChar = 'W'; color = "\033[33m"; break;
        case LOG_INFO:  levelChar = 'I'; color = "\033[32m"; break;
        case LOG_DEBUG: levelChar = 'D'; color = "\033[36m"; break;
        default: break;
    }
#else
    switch (level) {
        case LOG_ERROR: levelChar = 'E'; break;
        case LOG_WARN:  levelChar = 'W'; break;
        case LOG_INFO:  levelChar = 'I'; break;
        case LOG_DEBUG: levelChar = 'D'; break;
        default: break;
    }
#endif

    printf("%s%c (%lu) %s: ", color, levelChar, millis(), tag);
    printf(fmt, args...);
    printf("%s\n", reset);
}

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
#define LOG(level, tag, fmt, ...) logMessage(level, tag, fmt, ##__VA_ARGS__)

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