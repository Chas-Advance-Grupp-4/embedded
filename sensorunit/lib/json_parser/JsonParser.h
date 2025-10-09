/**
 * @file JsonParser.h
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Declaration of the JsonParser class for serializing sensor readings into JSON format.
 *
 * This class provides static methods to convert a collection of sensor readings
 * into a structured JSON string suitable for transmission or storage.
 * @date 2025-10-08
 *
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 *
 */
#pragma once
#include "constants.h"
#include "sensor_data_types.h"
#include <ArduinoJson.h>
#include <etl/string.h>
#include <etl/vector.h>

/**
 * @class JsonParser
 * @brief Static class for handling JSON serialization and deserialization.
 *
 *
 * All methods are static and stateless.
 */
class JsonParser {
  public:
    /**
     * @brief Composes a JSON string representing a group of sensor readings.
     *
     * This method takes a vector of sensor readings and a UUID string identifying
     * the sensor unit. It returns a JSON-formatted string containing all readings
     * in a structured format.
     *
     * @param readings A vector containing timestamped sensor readings (temperature, humidity).
     * @param uuid A string representing the unique identifier of the sensor unit.
     * @return etl::string<MAX_JSON_SIZE> A JSON string representing the sensor snapshot group.
     */
    static etl::string<json_config::max_json_size>
    composeSensorSnapshotGroup(etl::vector<ca_sensorunit_reading, json_config::max_batch_size>& readings,
                               const char*                                         uuid);
};
