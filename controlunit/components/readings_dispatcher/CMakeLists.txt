idf_component_register(
    SRCS "ReadingsDispatcher.cpp"
    INCLUDE_DIRS "."
    REQUIRES esp_timer rest_client control_unit_manager
)