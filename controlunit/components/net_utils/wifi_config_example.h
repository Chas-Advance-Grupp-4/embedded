/**
 * @file wifi_config_example.h
 * @brief Example configuration file for Wi-Fi credentials and client URL.
 *
 * This file provides placeholder definitions for Wi-Fi SSID, password, and
 * backend URL. It is intended as a template only.
 *
 * To use this configuration:
 * - Copy this file and rename it to `wifi_config.h`.
 * - Replace the placeholder values with your actual credentials.
 * - Ensure that `wifi_config.h` is excluded from version control (e.g., via `.gitignore`)
 *   to avoid exposing sensitive information.
 *
 * @note This file should not be included directly in production builds.
 */
#pragma once

#define MANUALLY_ADD_SENSORUNIT_FOR_TESTING
#define GENERATE_MOCKED_SENSOR_DATA
// #define REMOVE_AND_ADD_SENSORUNIT_WITH_DELAY_FOR_TESTING

#define WIFI_SECRET_SSID "Network"
#define WIFI_SECRET_PASS "Password"
#define CLIENT_URL "http://<your-url>"
#define CONTROL_UNIT_ID "00000000-0000-0000-0000-000000000000"  // Add your unique Contol Unit Id here

#define CONTROL_UNIT_SSID      "CU-0"           // Assign each Control Unit a unique number
#define CONTROL_UNIT_PASSWORD  "Password"
#define CONTROL_UNIT_IP_ADDR   "0.0.0.0"        // Change to your IP
#define CONTROL_UNIT_PORT      8000             // Set desired port
#define CONTROL_UNIT_NETMASK   "255.255.255.0"
#define CONTROL_UNIT_GATEWAY   "0.0.0.0"        // Change to your IP
constexpr const char* TEST_SENSOR_UNIT_ID = "00000000-0000-0000-0000-000000000000";  // UUID for testing