# Helper script for running python script with python3 or python
#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIPT_PATH="$SCRIPT_DIR/merge_compile_commands.py"

# Try with python3
if python3 "$SCRIPT_PATH"; then
    echo "Merged with python3"
else
    echo "python3 failed, trying med python"
    if python "$SCRIPT_PATH"; then
        echo "Merged with python"
    else
        echo "Merge failed. Script not found or python not installed."
        exit 1
    fi
fi