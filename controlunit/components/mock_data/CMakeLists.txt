idf_component_register(
    SRCS "MockDataGenerator.cpp"
    INCLUDE_DIRS "."
    REQUIRES esp_timer readings_dispatcher control_unit_manager json_parser
)