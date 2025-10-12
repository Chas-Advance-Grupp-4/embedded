# Convenience commands for long build commands in this Arduino project
.PHONY: help build flash test

.DEFAULT_GOAL := help

help:
	@echo "Available commands:"
	@echo "  make build       - Build the project"
	@echo "  make flash       - Flash and open monitor"
	@echo "  make test        - Build and flash the test file"

# build and run merge command
build:
	pio run

flash:
	pio run -t upload && pio device monitor --raw

test:
	pio test