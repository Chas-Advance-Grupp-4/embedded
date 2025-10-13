#pragma once
#include <etl/string.h>

struct ControlUnitInfo {
    etl::string<32> ssid;
    uint8_t controlUnitId;
};