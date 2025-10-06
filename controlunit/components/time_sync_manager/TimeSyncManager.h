#pragma once
#include <string>

class TimeSyncManager {
public:
    TimeSyncManager(const std::string& ntpServer = "se.pool.ntp.org", uint32_t syncIntervalMs = 3600000);
    void start();

private:
    std::string m_ntpServer;
    uint32_t m_syncIntervalMs;
    bool m_timeSynced;
    int m_maxRetries { 10 };
    static constexpr const char* TAG = "TimeSyncManager";
};