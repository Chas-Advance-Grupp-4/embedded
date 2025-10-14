/**
 * @file wifi_manager.cpp
 * @brief Implementation of Wi-Fi initialization and event handling.
 *
 * Sets up the ESP32 Wi-Fi stack in station mode, registers event handlers
 * for IP acquisition and disconnection, and connects to the network using
 * credentials from `wifi_config.h`.
 *
 * Uses FreeRTOS event groups to wait for successful connection.
 *
 * @author Erik Dahl (erik@iunderlandet.se)
 * @date 2025-10-07
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 */
#include "wifi_manager.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "wifi_config.h"
#include <esp_event.h>
#include <esp_http_server.h>
#include <esp_log.h>
#include <esp_netif.h>
#include <esp_netif_ip_addr.h>
#include <esp_wifi.h>
#include <nvs_flash.h>
#include <string>

#define WIFI_CONNECTED_BIT BIT0
static EventGroupHandle_t wifi_event_group;
static const char*        WIFI_TAG = "wifi_setup";

/**
 * @brief Handles Wi-Fi and IP events.
 *
 * - On IP acquisition, logs the IP info and sets the connected bit.
 * - On disconnection, attempts to reconnect automatically.
 *
 * @param arg Unused.
 * @param event_base Event category (e.g., WIFI_EVENT or IP_EVENT).
 * @param event_id Specific event ID.
 * @param event_data Pointer to event-specific data.
 */
static void wifi_event_handler(void*            arg,
                               esp_event_base_t event_base,
                               int32_t          event_id,
                               void*            event_data) {
    ESP_LOGI(WIFI_TAG,
             "wifi_event_handler triggered: base=%s, id=%d",
             event_base,
             event_id);
    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(WIFI_TAG, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
        ESP_LOGI(WIFI_TAG, "Gateway: " IPSTR, IP2STR(&event->ip_info.gw));
        ESP_LOGI(WIFI_TAG, "Netmask: " IPSTR, IP2STR(&event->ip_info.netmask));
    }
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGW(WIFI_TAG, "Disconnected from Wi-Fi, retrying...");
        esp_wifi_connect(); // Optional: auto-reconnect
    }
}

void init_wifi() {
    esp_log_level_set("wifi", ESP_LOG_VERBOSE);

    ESP_LOGI(WIFI_TAG, "Initializing Wi-Fi");

    wifi_event_group = xEventGroupCreate();

    esp_netif_init();
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Set up STA interface
    esp_netif_create_default_wifi_sta();

    // Set up AP interface with static IP
    esp_netif_t* ap_netif = esp_netif_create_default_wifi_ap();

    ESP_LOGI(WIFI_TAG, "Setting IP address for AP according to wifi_config.h");
    esp_netif_ip_info_t ip_info;
    ip_info.ip.addr      = esp_ip4addr_aton(CONTROL_UNIT_IP_ADDR);
    ip_info.gw.addr      = esp_ip4addr_aton(CONTROL_UNIT_GATEWAY);
    ip_info.netmask.addr = esp_ip4addr_aton(CONTROL_UNIT_NETMASK);

    ESP_ERROR_CHECK(esp_netif_dhcps_stop(ap_netif));
    ESP_ERROR_CHECK(esp_netif_set_ip_info(ap_netif, &ip_info));
    ESP_ERROR_CHECK(esp_netif_dhcps_start(ap_netif));

    // Register event handlers
    esp_err_t err = esp_event_handler_register(
        IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL);
    if (err != ESP_OK) {
        ESP_LOGE(WIFI_TAG,
                 "Failed to register IP event handler: %s",
                 esp_err_to_name(err));
    }

    esp_err_t err2 = esp_event_handler_register(
        WIFI_EVENT, WIFI_EVENT_STA_CONNECTED, &wifi_event_handler, NULL);
    if (err2 != ESP_OK) {
        ESP_LOGE(WIFI_TAG,
                 "Failed to register Wi-Fi event handler: %s",
                 esp_err_to_name(err2));
    }

    esp_err_t err3 = esp_event_handler_register(
        WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &wifi_event_handler, NULL);
    if (err3 != ESP_OK) {
        ESP_LOGE(WIFI_TAG,
                 "Failed to register Wi-Fi disconnect handler: %s",
                 esp_err_to_name(err3));
    }

    // Init WiFi
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));

    // Configure STA
    wifi_config_t sta_config = {};
    std::string   ssid       = WIFI_SECRET_SSID;
    std::string   password   = WIFI_SECRET_PASS;

    if (ssid.size() < sizeof(sta_config.sta.ssid)) {
        std::copy(ssid.begin(), ssid.end(), sta_config.sta.ssid);
        sta_config.sta.ssid[ssid.size()] = '\0';
    } else {
        ESP_LOGE(WIFI_TAG, "STA SSID too long");
    }
    if (password.size() < sizeof(sta_config.sta.password)) {
        std::copy(password.begin(), password.end(), sta_config.sta.password);
        sta_config.sta.password[password.size()] = '\0';
    } else {
        ESP_LOGE(WIFI_TAG, "STA Password too long");
    }

    esp_wifi_set_config(WIFI_IF_STA, &sta_config);

    // Configure AP
    wifi_config_t ap_config   = {};
    std::string   ap_ssid     = CONTROL_UNIT_SSID;
    std::string   ap_password = CONTROL_UNIT_PASSWORD;

    ap_config.ap.max_connection = 4;
    ap_config.ap.authmode       = WIFI_AUTH_WPA2_PSK;

    std::copy(ap_ssid.begin(), ap_ssid.end(), ap_config.ap.ssid);
    ap_config.ap.ssid[ap_ssid.size()] = '\0';
    ap_config.ap.ssid_len = ap_ssid.size();

    std::copy(ap_password.begin(), ap_password.end(), ap_config.ap.password);
    ap_config.ap.password[ap_password.size()] = '\0';

    ESP_LOGI(WIFI_TAG, "AP SSID: %s", CONTROL_UNIT_SSID);
    ESP_LOGI(WIFI_TAG, "AP IP: %s", CONTROL_UNIT_IP_ADDR);

    esp_wifi_set_config(WIFI_IF_AP, &ap_config);

    // Start WiFi
    esp_wifi_start();
    // Connect needed for STA
    esp_err_t connect_err = esp_wifi_connect();
    if (connect_err != ESP_OK) {
        ESP_LOGE(WIFI_TAG,
                 "Failed to connect to STA: %s",
                 esp_err_to_name(connect_err));
    }

    ESP_LOGI(WIFI_TAG, "Wi-Fi started, waiting for connection...");
    xEventGroupWaitBits(
        wifi_event_group, WIFI_CONNECTED_BIT, false, true, portMAX_DELAY);
    ESP_LOGI(WIFI_TAG, "Wi-Fi connected!");

    ESP_LOGI(
        WIFI_TAG, "Wi-Fi started, connecting to SSID: %s", WIFI_SECRET_SSID);
}