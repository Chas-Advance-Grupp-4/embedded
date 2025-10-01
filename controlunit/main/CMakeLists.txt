idf_component_register(
    SRCS "main.cpp"
    INCLUDE_DIRS "."   
    PRIV_REQUIRES net_utils rest_server rest_client esp_http_server esp_wifi log nvs_flash
)