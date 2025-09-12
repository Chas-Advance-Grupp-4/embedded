idf_component_register(
    SRCS "rest_server.cpp"
         "handlers/hello_handler.cpp"
         "handlers/status_handler.cpp"
    INCLUDE_DIRS "." "handlers"
    PRIV_REQUIRES esp_http_server esp_wifi log nvs_flash
)