/**
 * @file TimeSyncManager.h
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Classes for syncing ESP clock with NTP
 * @date 2025-10-07
 * 
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 * 
 */
#pragma once
#include <string>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class TimeSyncManager {
public:
    TimeSyncManager(const std::string& ntpServer = "se.pool.ntp.org", uint32_t syncIntervalMs = 60*60*1000);
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