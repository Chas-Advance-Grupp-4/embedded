#pragma once
#include <string>
#include <ctime>
#include <memory>

struct ca_sensorunit_snapshot {
    std::shared_ptr<std::string> uuid;
    time_t timestamp;
    double temperature;
    double humidity;
};