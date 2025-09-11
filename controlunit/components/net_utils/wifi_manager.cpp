#include "wifi_manager.h"

static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data) {
    ESP_LOGI(WIFI_TAG, "wifi_event_handler triggered: base=%s, id=%d", event_base, event_id);
    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(WIFI_TAG, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
        ESP_LOGI(WIFI_TAG, "Gateway: " IPSTR, IP2STR(&event->ip_info.gw));
        ESP_LOGI(WIFI_TAG, "Netmask: " IPSTR, IP2STR(&event->ip_info.netmask));
    }
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
    ESP_LOGW(WIFI_TAG, "Disconnected from Wi-Fi, retrying...");
    esp_wifi_connect();  // Optional: auto-reconnect
    }
}

void init_wifi() {
    esp_log_level_set("wifi", ESP_LOG_VERBOSE);

    ESP_LOGI(WIFI_TAG, "Initializing Wi-Fi");

    esp_netif_init();
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    esp_err_t err = esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL);
    if (err != ESP_OK) {
      ESP_LOGE(WIFI_TAG, "Failed to register IP event handler: %s", esp_err_to_name(err));
    }

    esp_err_t err2 = esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_CONNECTED, &wifi_event_handler, NULL);
    if (err2 != ESP_OK) {
        ESP_LOGE(WIFI_TAG, "Failed to register Wi-Fi event handler: %s", esp_err_to_name(err2));
    }

    esp_err_t err3 = esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &wifi_event_handler, NULL);
    if (err3 != ESP_OK) {
        ESP_LOGE(WIFI_TAG, "Failed to register Wi-Fi disconnect handler: %s", esp_err_to_name(err3));
    }

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    wifi_config_t wifi_config = {};
    strncpy((char*)wifi_config.sta.ssid, WIFI_SECRET_SSID, sizeof(wifi_config.sta.ssid));
    strncpy((char*)wifi_config.sta.password, WIFI_SECRET_PASS, sizeof(wifi_config.sta.password));

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();
    esp_wifi_connect();

    ESP_LOGI(WIFI_TAG, "Wi-Fi started, connecting to SSID: %s", WIFI_SECRET_SSID);
}