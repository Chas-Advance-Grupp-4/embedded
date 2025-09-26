#include "sensor_data_types.h"

const std::string& Uuid::toString() const {
    return value;
}

bool Uuid::isValid() const {
    return !value.empty();
}

bool Uuid::operator==(const Uuid& other) const {
    return value == other.value;
}

bool Uuid::operator<(const Uuid& other) const {
    return value < other.value;
}
