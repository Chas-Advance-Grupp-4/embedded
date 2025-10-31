/**
 * @file TimeSyncManager.cpp
 * @brief Implementation of TimeSyncManager for NTP-based time synchronization.
 *
 * Handles SNTP initialization, time validation, and optional periodic resync
 * using a FreeRTOS task. Uses ESP-IDF logging for diagnostics.
 *
 * @author Erik Dahl (erik@iunderlandet.se)
 * @date 2025-10-07
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 */
#include "TimeSyncManager.h"
#include "esp_log.h"
#include "esp_sntp.h"
#include <time.h>

TimeSyncManager::TimeSyncManager(const std::string& ntpServer,
                                 uint32_t           syncIntervalMs)
    : m_ntpServer{ntpServer}, m_syncIntervalMs{syncIntervalMs},
      m_timeSynced{false} {
    ESP_LOGI(TAG, "Resync interval set to %d ms", m_syncIntervalMs);
}

void TimeSyncManager::start() {
    ESP_LOGI(TAG, "Initializing SNTP...");
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, m_ntpServer.c_str());
    esp_sntp_init();

    // Wait until time is synced
    time_t    now      = 0;
    struct tm timeinfo = {};
    int       retry    = 0;

    // Set time zone
    setenv("TZ", m_defaultTimezone.c_str(), 1);
    tzset();

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
        char time_str[64];
        strftime(time_str, sizeof(time_str), "%c", &timeinfo);
        ESP_LOGI(TAG, "Time synced: %s", time_str);
        enableAutoResync();
    } else {
        ESP_LOGW(TAG, "Could not sync time");
    }
    
}

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

void TimeSyncManager::resyncTask() {
   while (true) {
        vTaskDelay(pdMS_TO_TICKS(m_syncIntervalMs));
        resync();
    }
}

void TimeSyncManager::resync() {
    ESP_LOGI(TAG, "Resyncing time...");
    esp_sntp_stop();
    esp_sntp_init();

    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);

    if ((timeinfo.tm_year + 1900) >= 2020) {
        char time_str[64];
        strftime(time_str, sizeof(time_str), "%c", &timeinfo);
        ESP_LOGI(TAG, "Resync successful: %s", time_str);
        ESP_LOGI(TAG, "Resync interval is set to %d ms", m_syncIntervalMs);
        m_timeSynced = true;
    } else {
        ESP_LOGW(TAG, "Resync failed");
        m_timeSynced = false;
    }
}

bool TimeSyncManager::isTimeSynced() const {
    return m_timeSynced;
}