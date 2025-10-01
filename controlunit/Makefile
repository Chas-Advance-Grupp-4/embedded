# Convenience commands for long build commands in this ESP-IDF project
.PHONY: help merge build flash test testflash

.DEFAULT_GOAL := help

help:
	@echo "Available commands:"
	@echo "  make build       - Build the project"
	@echo "  make flash       - Flash and open monitor"
	@echo "  make test        - Build the test file"
	@echo "  make testflash   - Flash and monitor test file"
	@echo "  make merge       - Merge compile_commands.json for Intellisense"

# Merge compile_commands.json for correct Intellisense path
merge:
	bash ./helpers/mergerunner.sh

# build and run merge command
build:
	idf.py build
	@$(MAKE) merge

flash:
	idf.py flash monitor

test:
	(cd test_runner && idf.py build)

testflash:
	(cd test_runner && idf.py flash monitor)