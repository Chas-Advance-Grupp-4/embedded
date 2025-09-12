idf_component_register(
    SRCS "wifi_manager.cpp"
    INCLUDE_DIRS "."
    PRIV_REQUIRES esp_http_server esp_wifi log nvs_flash
)