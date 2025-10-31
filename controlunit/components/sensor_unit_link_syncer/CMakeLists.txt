idf_component_register(
    SRCS "SensorUnitLinkSyncer.cpp"
    INCLUDE_DIRS "."
    REQUIRES esp_timer rest_client sensor_unit_manager json_parser
)