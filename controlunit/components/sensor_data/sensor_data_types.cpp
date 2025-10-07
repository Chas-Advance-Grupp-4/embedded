/**
 * @file sensor_data_types.cpp
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Implements utility functions for UUID comparison and validation.
 *
 * Provides the implementation of Uuid methods such as toString(), isValid(),
 * and comparison operators.
 * @date 2025-10-07
 *
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 *
 */
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
