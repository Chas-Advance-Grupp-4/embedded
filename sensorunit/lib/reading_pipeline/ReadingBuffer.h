/**
 * @file ReadingBuffer.h
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Reading Buffer that internally uses an ETL circular buffer
 *
 * - push individual readings to the buffer,
 * - get a batch of readings of up to a predefined maximum size,
 * - remove a batch of given size
 * - check size and check if buffer has readings.
 *
 * @date 2025-10-24
 *
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 *
 */
#pragma once
#include "sensor_data_types.h"
#include <etl/circular_buffer.h>
#include <etl/string.h>
#include <etl/vector.h>

/**
 * @brief Defines maximum sizes for ETL containers
 * used in the ReadingBuffer class
 *
 */
namespace buffer_config {
constexpr size_t max_buffer_size = 20; // ~400 bytes in RAM
constexpr size_t max_batch_size  = 10;
} // namespace buffer_config

class ReadingBuffer {
  public:
    /**
     * @brief Default contructor. Nothing to see here.
     */
    ReadingBuffer() = default;
    /**
     * @brief Get a batch of readings of up to the max_batch_size
     *
     * Note that the function reads without deleting
     * so caller needs to manually call removeBatch after verifying successful processing
     *
     * @return etl::vector<CaSensorunitReading, buffer_config::max_batch_size>
     */
    etl::vector<CaSensorunitReading, buffer_config::max_batch_size> getBatch() const;
    /**
     * @brief Remove a batch of a specified size
     * Caller needs to manually remove buffer items
     *
     * @param count number of readings to remove from the buffer
     */
    void removeBatch(size_t count);
    /**
     * @brief Add an individual reading to the buffer
     *
     * @param reading a reading containing timestamp and sensor values
     */
    void push(const CaSensorunitReading& reading);
    /**
     * @brief Functions that checks if the buffer has readings
     *
     * @return true if buffer has one or more readings
     * @return false if buffer is empty
     */
    bool hasReadings() const;
    /**
     * @brief Function returns the number of readings in the buffer
     *
     * @return size_t Number of readings in the buffer
     */
    size_t size() const;

  private:
    etl::circular_buffer<CaSensorunitReading, buffer_config::max_buffer_size> m_buffer;
    static constexpr const char*                                              TAG = "ReadingBuffer";
};
