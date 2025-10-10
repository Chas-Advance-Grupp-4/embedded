/**
 * @file logging.h
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Lightweight logging macros for Arduino-based projects using PlatformIO.
 *
 * Provides configurable log level filtering and formatted output via the serial console.
 * Designed to be similiar in use to the ESP-based logging used in the Control Unit repo.
 * Enables consistent tagging and severity-based logging for debugging and runtime diagnostics.
 * @date 2025-10-10
 * 
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 * 
 */
#pragma once
#include <Arduino.h>

/**
 * @enum LogLevel
 * @brief Defines the severity levels for logging output.
 *
 * This enumeration is used to categorize log messages based on their importance.
 * It allows filtering of logs depending on the current log level setting.
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
 * It is used by the LOG macros to filter output according to the selected LogLevel.
 * It is defined once in logging.cpp
 * It is declared here as extern and is therefor available when this file is included
 * Change your current log level in your config.h file
 */
extern LogLevel currentLogLevel;

/**
 * @def LOG(level, tag, msg)
 * @brief Generic logging macro that prints messages to the serial console.
 *
 * This macro checks the provided log level against the global `currentLogLevel`
 * and prints the message if the level is equal to or lower than the current setting.
 *
 * @param level The severity level of the log message (see LogLevel).
 * @param tag   A short identifier or module name for the log source.
 * @param msg   The message string to be printed.
 */
#define LOG(level, tag, msg) \
    do { \
        if (level != LOG_NONE && level <= currentLogLevel) { \
            char levelChar = '?'; \
            switch (level) { \
                case LOG_ERROR: levelChar = 'E'; break; \
                case LOG_WARN:  levelChar = 'W'; break; \
                case LOG_INFO:  levelChar = 'I'; break; \
                case LOG_DEBUG: levelChar = 'D'; break; \
                default:        levelChar = '?'; break; \
            } \
            Serial.print(levelChar); \
            Serial.print(" ("); \
            Serial.print(millis()); \
            Serial.print(") "); \
            Serial.print(tag); \
            Serial.print(": "); \
            Serial.println(msg); \
        } \
    } while (0)

    /**
 * @def LOG_ERROR(tag, msg)
 * @brief Logs an error-level message.
 *
 * Equivalent to calling LOG(LOG_ERROR, tag, msg).
 */
#define LOG_ERROR(tag, msg) LOG(LOG_ERROR, tag, msg)

/**
 * @def LOG_WARN(tag, msg)
 * @brief Logs a warning-level message.
 *
 * Equivalent to calling LOG(LOG_WARN, tag, msg).
 */
#define LOG_WARN(tag, msg)  LOG(LOG_WARN,  tag, msg)

/**
 * @def LOG_INFO(tag, msg)
 * @brief Logs an informational message.
 *
 * Equivalent to calling LOG(LOG_INFO, tag, msg).
 */
#define LOG_INFO(tag, msg)  LOG(LOG_INFO,  tag, msg)

/**
 * @def LOG_DEBUG(tag, msg)
 * @brief Logs a debug-level message.
 *
 * Equivalent to calling LOG(LOG_DEBUG, tag, msg).
 */
#define LOG_DEBUG(tag, msg) LOG(LOG_DEBUG, tag, msg)