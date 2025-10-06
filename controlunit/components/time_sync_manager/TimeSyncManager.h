#pragma once
#include <string>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class TimeSyncManager {
public:
    TimeSyncManager(const std::string& ntpServer = "se.pool.ntp.org", uint32_t syncIntervalMs = 1*60*1000);
    void start();
    void enableAutoResync();
    void resync();

    private:
    std::string m_ntpServer;
    uint32_t m_syncIntervalMs;
    bool m_timeSynced;
    int m_maxRetries { 10 };
    TaskHandle_t m_resyncTaskHandle = nullptr;
    void resyncTask();

    static constexpr const char* TAG = "TimeSyncManager";
};