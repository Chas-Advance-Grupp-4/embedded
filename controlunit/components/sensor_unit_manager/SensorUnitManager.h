/**
 * @file SensorUnitManager.h
 * @brief Manages active sensor units and stores their readings.
 *
 * This class provides functionality for registering sensor units,
 * storing incoming sensor readings, and grouping them by timestamp
 * for batch processing or transmission.
 * 
 * It uses a FreeRTOS mutex for protecting resource shared between tasks
 *
 * Class functionality:
 * - Add or remove sensor units using their UUIDs.
 * - Store readings as they arrive.
 * - Retrieve grouped readings for backend dispatch.
 * - Clear stored readings when succesfully dispatched.
 *
 * @author Erik Dahl (erik@iunderlandet.se)
 * @date 2025-10-07
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 */
#pragma once
#include "sensor_data_types.h"
#include <map>
#include <string>
#include <vector>
#include "freertos/FreeRTOS.h"

/**
 * @class SensorUnitManager
 * @brief Handles registration and data storage for sensor units.
 *
 * Maintains a list of active sensor units and stores their readings.
 * Readings can be grouped by timestamp for batch operations.
 */
class SensorUnitManager {
  public:
    /**
     * @brief Default constructor added just for clarity
     * 
     */
    SensorUnitManager() = default;
    /**
     * @brief Class needs to run init in app_main to create the 
     * FreeRTOR mutex needed to protect shared resources
     * 
     */
    void init();
    /**
     * @brief Registers a sensor unit by UUID.
     * @param uuid Unique identifier of the sensor unit.
     */
    void addUnit(const Uuid& uuid);
    /**
     * @brief Removes a sensor unit from the registry.
     * @param uuid Unique identifier of the sensor unit to remove.
     */
    void removeUnit(const Uuid& uuid);

    /**
     * @brief Checks if a sensor unit is currently registered.
     * @param uuid UUID to check.
     * @return true if the unit is registered, false otherwise.
     */
    bool hasUnit(const Uuid& uuid) const;

    /**
     * @brief Stores a snapshot reading from a sensor unit.
     * @param reading Sensor data including timestamp, temperature, and
     * humidity.
     */
    void storeReading(const ca_sensorunit_snapshot& reading);
    /**
     * @brief Groups stored readings by timestamp.
     * @return Map of timestamp to vector of sensor readings.
     */
    std::map<time_t, std::vector<ca_sensorunit_snapshot>>
    getGroupedReadings() const;
    /**
     * @brief Clears all stored sensor readings.
     */
    void clearReadings();

  private:
    mutable SemaphoreHandle_t m_readingsMutex = nullptr;
    std::map<Uuid, std::shared_ptr<Uuid>>
        m_active_units; /**< Registered sensor units by UUID. */
    std::vector<ca_sensorunit_snapshot>
        m_all_readings; /**< All stored sensor readings. */
    static constexpr const char* TAG = "SensorUnitManager";
};