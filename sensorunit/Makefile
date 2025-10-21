# Convenience commands for long build commands in this Arduino project
.PHONY: help build flash test clean monitor

.DEFAULT_GOAL := help

help:
	@echo "Available commands:"
	@echo "  make build       - Build the project"
	@echo "  make flash       - Flash and open monitor"
	@echo "  make test        - Build and flash the test file"
	@echo "  make clean       - Clean PlatformIO cache"
	@echo "  make monitor     - Open monitor"

# build and run merge command
build:
	pio run

flash:
	pio run -t upload && pio device monitor --raw

test:
	pio test

clean:
	pio run -t clean

monitor:
	pio device monitor --raw