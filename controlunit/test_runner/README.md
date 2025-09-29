# Unity-based Test Application for ESP-IDF

This folder contains a standalone ESP-IDF application used exclusively for running unit and integration tests. It follows a common structure in ESP-IDF projects where tests are isolated from production code.

## Folder structure

- `main/main.cpp`: Defines a minimal `app_main()` that initializes Unity and runs registered tests.
- `main/CMakeLists.txt`: Lists test sources and dependencies explicitly.
- `sdkconfig.defaults`: Contains configuration tailored for test execution (e.g., log level, partitioning).

## Run the tests

If target is not set you may first need to run:  

```bash
idf.py set-target esp32s3
```

Then from the root folder:  

```bash
cd test_runner
```

Then each time you want to run the tests:  

```bash
idf.py build
idf.py flash monitor
```

If monitor is not found you can set it explicitly (replace usbmodem1101 with your port):  

```bash
idf.py -p /dev/tty.usbmodem1101 flash monitor
```
