# Convenience commands for long build commands in this ESP-IDF project
.PHONY: help merge build flash test testflash size

.DEFAULT_GOAL := help

help:
	@echo "Available commands:"
	@echo "  make build       - Build the project, merge and print size"
	@echo "  make flash       - Flash and open monitor"
	@echo "  make test        - Build the test file"
	@echo "  make testflash   - Flash and monitor test file"
	@echo "  make merge       - Merge compile_commands.json for Intellisense"
	@echo "  make size        - Print Size Table for latest build"

# Merge compile_commands.json for correct Intellisense path
merge:
	bash ./helpers/mergerunner.sh

# build and run merge command
build:
	idf.py build
	@$(MAKE) merge
	@$(MAKE) size

flash:
	idf.py flash monitor

test:
	(cd test_runner && idf.py build)

testflash:
	(cd test_runner && idf.py flash monitor)

# Warning, Make sure Project Name is the same as in CMakeLists.txt
PROJECT_NAME := Chas_Advance_4_ESP32_control_unit
ESP_PYTHON := $(shell which python)
IDF_SIZE := $(IDF_PATH)/tools/idf_size.py
MAP_FILE := build/$(PROJECT_NAME).map

size:
	$(ESP_PYTHON) $(IDF_SIZE) $(MAP_FILE)