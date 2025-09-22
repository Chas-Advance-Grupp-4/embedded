/**
 * @brief Test file for SensorUnitManager.cpp
 * 
 * 
 */
extern "C" {
    #include "unity.h"
}
#include "SensorUnitManager.h"

extern "C" void when_manager_empty_then_hasUnit_returns_false(void) {
    SensorUnitManager manager;
    Uuid uuid  {"abc"};
    bool hasUnit = manager.hasUnit(uuid);
    TEST_ASSERT_EQUAL(hasUnit, false);
}

extern "C" void when_unit_added_then_hasUnit_returns_true(void) {
    SensorUnitManager manager;
    Uuid uuid {"abc"};
    manager.addUnit(uuid);
    bool hasUnit = manager.hasUnit(uuid);
    TEST_ASSERT_EQUAL(hasUnit, true);
}

extern "C" void when_unit_added_and_removed_then_hasUnit_returns_false(void) {
    SensorUnitManager manager;
    Uuid uuid {"abc"};
    manager.addUnit(uuid);
    manager.removeUnit(uuid);
    bool hasUnit = manager.hasUnit(uuid);
    TEST_ASSERT_EQUAL(hasUnit, false);
}

extern "C" void when_unit_added_twice_then_logs_error(void) {
    SensorUnitManager manager;
    Uuid uuid {"abc"};
    manager.addUnit(uuid);
    manager.addUnit(uuid); 
    TEST_ASSERT_EQUAL(manager.hasUnit(uuid), true);
}

extern "C" void when_nonexistent_unit_removed_then_logs_error(void) {
    SensorUnitManager manager;
    Uuid uuid {"xyz"};
    manager.removeUnit(uuid); 
    TEST_ASSERT_EQUAL(manager.hasUnit(uuid), false);
}

extern "C" void stress_test_many_units(void) {
    SensorUnitManager manager;
    for (int i = 0; i < 1000; ++i) {
        manager.addUnit(Uuid{std::to_string(i)});
    }
    TEST_ASSERT_EQUAL(manager.hasUnit(Uuid{"500"}), true);
}