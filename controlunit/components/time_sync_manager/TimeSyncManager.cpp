/**
 * @file TimeSyncManager.cpp
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Classes for syncing ESP clock with NTP
 * @date 2025-10-07
 * 
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 * 
 */
#include "TimeSyncManager.h"
#include "esp_log.h"
#include "esp_sntp.h"
#include <time.h>

/**
 * @brief Constructor for Time Sync Manager
 * 
 * @param ntpServer Defaults to "se.pool.ntp.org"
 * @param syncIntervalMs Defaults to 60*60*1000
 */
TimeSyncManager::TimeSyncManager(const std::string& ntpServer,
                                 uint32_t           syncIntervalMs)
    : m_ntpServer{ntpServer}, m_syncIntervalMs{syncIntervalMs},
      m_timeSynced{false} {
    ESP_LOGI(TAG, "Resync interval set to %d ms", m_syncIntervalMs);
}

/**
 * @brief start the sync. Combines init and run into one function call
 * @param m_maxRetries defaults to 10
 */
void TimeSyncManager::start() {
    ESP_LOGI(TAG, "Initializing SNTP...");
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, m_ntpServer.c_str());
    esp_sntp_init();

    // Wait until time is synced
    time_t    now      = 0;
    struct tm timeinfo = {};
    int       retry    = 0;

    while (!m_timeSynced && retry++ < m_maxRetries) {
        time(&now);
        localtime_r(&now, &timeinfo);

        if ((timeinfo.tm_year + 1900) >= 2020) {
            m_timeSynced = true;
        } else {
            ESP_LOGI(TAG, "Waiting for time... (%d/%d)", retry, m_maxRetries);
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
    }

    if (retry < m_maxRetries) {
        ESP_LOGI(TAG, "Time synced: %s", asctime(&timeinfo));
        enableAutoResync();
    } else {
        ESP_LOGW(TAG, "Could not sync time");
    }
    
}

/**
 * @brief Start a task that periodically resyncs the clock
 * 
 */
void TimeSyncManager::enableAutoResync() {
    if (m_resyncTaskHandle != nullptr) {
        ESP_LOGW(TAG, "Resync task already running");
        return;
    }

    if (m_syncIntervalMs < 10000) {
        ESP_LOGW(TAG, "Sync interval too short, forcing minimum 10s");
        m_syncIntervalMs = 10000;
    }

    ESP_LOGI(TAG, "Starting resync task with interval %d ms", m_syncIntervalMs);
    xTaskCreate(
        [](void* arg) {
            static_cast<TimeSyncManager*>(arg)->resyncTask();
        },
        "TimeResyncTask",
        4096,
        this,
        1,
        &m_resyncTaskHandle);
    }

/**
 * @brief The task used by TimeSyncManager::enableAutoResync
 * 
 */
void TimeSyncManager::resyncTask() {
   while (true) {
        vTaskDelay(pdMS_TO_TICKS(m_syncIntervalMs));
        resync();
    }
}

/**
 * @brief The actual resync funtction run by TimeSyncManager::resyncTask
 * 
 */
void TimeSyncManager::resync() {
    ESP_LOGI(TAG, "Resyncing time...");
    esp_sntp_stop();
    esp_sntp_init();

    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);

    if ((timeinfo.tm_year + 1900) >= 2020) {
        ESP_LOGI(TAG, "Resync successful: %s", asctime(&timeinfo));
        ESP_LOGI(TAG, "Resync interval is set to %d ms", m_syncIntervalMs);
        m_timeSynced = true;
    } else {
        ESP_LOGW(TAG, "Resync failed");
        m_timeSynced = false;
    }
}