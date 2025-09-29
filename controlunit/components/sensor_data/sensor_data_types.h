#pragma once
#include <ctime>
#include <memory>
#include <string>

struct Uuid {
    std::string value;

    Uuid() = default;
    explicit Uuid(const std::string& v) : value(v) {}
    const std::string& toString() const;
    bool               isValid() const;
    bool               operator==(const Uuid& other) const;
    bool               operator<(const Uuid& other) const;
};

struct ca_sensorunit_snapshot {
    std::shared_ptr<Uuid> uuid;
    time_t                timestamp;
    double                temperature;
    double                humidity;
};