#pragma once
#include "wifi_config.h"
#include <esp_http_server.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_netif.h>
#include <nvs_flash.h>

static const char* WIFI_TAG = "wifi_setup";

static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data);
void init_wifi();
