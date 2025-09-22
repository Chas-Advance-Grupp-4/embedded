idf_component_register(
    SRCS "SensorUnitManager.cpp"
    INCLUDE_DIRS "."
    PRIV_REQUIRES sensor_data log nvs_flash
)