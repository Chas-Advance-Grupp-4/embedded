#include "ReadingBuffer.h"

bool ReadingBuffer::hasReadings() const {
    return m_buffer.size() >= 1;
}

etl::vector<CaSensorunitReading, buffer_config::max_batch_size> ReadingBuffer::getBatch() const {
    etl::vector<CaSensorunitReading, buffer_config::max_batch_size> batch;
    for (size_t i = 0; i < std::min(m_buffer.size(), buffer_config::max_batch_size); ++i) {
        batch.push_back(m_buffer[i]); // NOTE: Reads without erasing
    }
    return batch;
}

void ReadingBuffer::removeBatch(size_t count) {
    for (size_t i = 0; i < count && !m_buffer.empty(); ++i) {
        m_buffer.pop();
    }
}

void ReadingBuffer::push(const CaSensorunitReading& reading) {
    if (!m_buffer.full()) {
        m_buffer.push(reading);
    } else {
        // TODO: Add log message warning. Buffer full, overwriting oldest value
        m_buffer.pop();
        m_buffer.push(reading);
    }
}

size_t ReadingBuffer::size() const {
    return m_buffer.size();
}