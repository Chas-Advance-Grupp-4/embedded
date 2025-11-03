# helpers/

Utility scripts for development.

## merge_compile_commands.py

This script merges `compile_commands.json` from the main project (`build/`) and the test project (`test_runner/build/`) into a single file used by VSCode for IntelliSense.

### How to run

From the project root:

```bash
python3 helpers/merge_compile_commands.py
```

---

## mergerunner.sh

Script used by Makefile to determine which python version to use when merging compile commands

---

## cu_cppcheck.sh

Script for running cppcheck linting for `controlunit/`
