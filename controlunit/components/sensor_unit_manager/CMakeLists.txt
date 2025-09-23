idf_component_register(
    SRCS "SensorUnitManager.cpp"
    INCLUDE_DIRS "."
    REQUIRES sensor_data log nvs_flash
)

if(CONFIG_UNIT_TEST_ENABLED)
    set(TEST_SRCS handlers/test/test_SensorUnitManager.cpp)
    idf_component_add_unit_test(sensor_unit_manager_tests "${TEST_SRCS}")
endif()