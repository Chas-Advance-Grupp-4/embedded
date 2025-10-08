#pragma once
#include <ctime>

struct ca_sensorunit_reading {
    time_t timestamp;           /**< Timestamp of the snapshot (Unix time). */
    double temperature;         /**< Measured temperature in degrees Celsius. */
    double humidity;            /**< Measured humidity in percentage. */
};