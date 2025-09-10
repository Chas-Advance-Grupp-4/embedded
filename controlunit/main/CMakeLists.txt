idf_component_register(
    SRCS "main.cpp"
    INCLUDE_DIRS "." "include"               
    PRIV_REQUIRES esp_http_server esp_wifi log nvs_flash
)