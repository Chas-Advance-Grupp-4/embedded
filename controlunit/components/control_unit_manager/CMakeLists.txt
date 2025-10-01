idf_component_register(
    SRCS "ControlUnitManager.cpp"
    INCLUDE_DIRS "."
    REQUIRES sensor_data sensor_unit_manager log
)