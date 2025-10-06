#include "TimeSyncManager.h"
#include "esp_log.h"
#include "esp_sntp.h"
#include <time.h>

TimeSyncManager::TimeSyncManager(const std::string& ntpServer,
                                 uint32_t           syncIntervalMs)
    : m_ntpServer{ntpServer}, m_syncIntervalMs{syncIntervalMs},
      m_timeSynced{false} {}

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
    } else {
        ESP_LOGW(TAG, "Could not sync time");
    }
}