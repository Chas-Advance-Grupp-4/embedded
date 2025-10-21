/**
 * @file sensor_data_types.h
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Defines data structures for sensor readings and UUID handling.
 *
 * Contains the Uuid class for identifier management and ca_sensorunit_snapshot
 * for representing timestamped sensor data.
 * @date 2025-10-07
 *
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 *
 */
#pragma once
#include <ctime>
#include <memory>
#include <string>

/**
 * @struct Uuid
 * @brief Represents a universally unique identifier (UUID) as a string.
 *
 * Provides basic functionality for UUID handling, including string conversion,
 * validity check, and comparison operators.
 */
struct Uuid {
    std::string value; /**< Raw UUID string value. */

    /**
     * @brief Default constructor.
     */
    Uuid() = default;

    /**
     * @brief Constructs a Uuid from a string value.
     * @param v UUID string.
     */
    explicit Uuid(const std::string& v) : value(v) {}

    /**
     * @brief Returns the UUID as a string.
     * @return Reference to the internal string value.
     */
    const std::string& toString() const;

    /**
     * @brief Checks whether the UUID is valid (non-empty).
     * @return True if valid, false otherwise.
     */
    bool isValid() const;

    /**
     * @brief Equality comparison operator.
     * @param other UUID to compare with.
     * @return True if equal, false otherwise.
     */
    bool operator==(const Uuid& other) const;

    /**
     * @brief Less-than comparison operator.
     * @param other UUID to compare with.
     * @return True if this UUID is less than the other.
     */
    bool operator<(const Uuid& other) const;
};

/**
 * @struct ca_sensorunit_snapshot
 * @brief Represents a snapshot of sensor data from a sensor unit.
 *
 * Contains timestamped readings for temperature and humidity, along with
 * a UUID identifying the sensor unit.
 */
struct ca_sensorunit_snapshot {
    std::shared_ptr<Uuid> uuid; /**< UUID of the sensor unit. */
    time_t timestamp;           /**< Timestamp of the snapshot (Unix time). */
    double temperature;         /**< Measured temperature in degrees Celsius. */
    double humidity;            /**< Measured humidity in percentage. */
};