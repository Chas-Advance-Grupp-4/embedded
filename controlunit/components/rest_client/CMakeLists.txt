idf_component_register(
    SRCS "RestClient.cpp"
    INCLUDE_DIRS "."
    REQUIRES esp_http_client log
)