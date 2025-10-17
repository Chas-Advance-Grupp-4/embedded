#pragma once
#include "sensor_data_types.h"
#include <etl/circular_buffer.h>
#include <etl/string.h>
#include <etl/vector.h>

namespace buffer_config {
constexpr size_t max_buffer_size = 20;  // ~400 bytes in RAM
constexpr size_t max_batch_size  = 10;
} // namespace buffer_config

class ReadingBuffer {
  public:
    bool hasReadings() const;
    etl::vector<CaSensorunitReading, buffer_config::max_batch_size> getBatch() const;
    void                                                            removeBatch(size_t count);
    void push(const CaSensorunitReading& reading);
    size_t size() const;

  private:
    etl::circular_buffer<CaSensorunitReading, buffer_config::max_buffer_size> m_buffer;
    static constexpr const char* TAG = "ReadingBuffer";
};
