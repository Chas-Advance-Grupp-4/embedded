/**
 * 
 * * Copy this file to `config.h` and replace the placeholder values with your own.
 * * This file defines compile-time settings such as logging behavior and network credentials.
 * * Do not modify this file directly – changes may be overwritten during updates.
 * 
 */

#pragma once

// Copy this file to config.h and enter your own values

/**
 * @def LOG_LEVEL
 * @brief Defines the default verbosity level for logging output.
 *
 * This macro sets the initial value for `currentLogLevel`, determining which messages
 * are printed based on their severity. Messages with a level equal to or lower than
 * `LOG_LEVEL` will be shown. Typical values include LOG_ERROR, LOG_WARN, LOG_INFO, and LOG_DEBUG.
 *
 * You can override this value in your config.h file to control log filtering globally.
 */
#define LOG_LEVEL LOG_INFO

/**
 * @def LOG_COLORS
 * @brief Enables ANSI color codes in log output for supported terminals.
 *
 * When defined, log messages will include ANSI escape sequences to color-code
 * severity levels (e.g., red for errors, yellow for warnings, green for info).
 * This improves readability in terminals that support ANSI formatting.
 *
 * If undefined, log output will be plain text without color codes.
 * Recommended for use with `pio device monitor --raw` or compatible terminals.
 */
#define LOG_COLORS  

/**
 * @brief Unique identifier for this sensor unit.
 * Used to distinguish devices when sending data to a server or logging.
 */
constexpr const char* SENSOR_UNIT_ID = "REPLACE_WITH_UUID";

/**
 * @brief Control Unit IP
 * Update as needed. All control units should use the same fixed IP
 */
#define CONTROL_UNIT_IP_ADDR "192.168.4.1"

/**
 * @brief Control Unit Password
 *  Make sure it matches the password set in controlunit config
 */
#define CONTROL_UNIT_PASSWORD "<your-controlunit-password>"

/**
 * @brief Control Unit Port
 * Make sure it matches the port set in controlunit config
 */
#define CONTROL_UNIT_PORT 8080
