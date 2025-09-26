# filter out gcc flags for clang-tidy
import os
import json

script_dir = os.path.dirname(os.path.abspath(__file__))
project_root = os.path.abspath(os.path.join(script_dir, ".."))
input_path = os.path.join(project_root, "build", "compile_commands.json")
output_path = os.path.join(project_root, "build", "clang-tidy", "compile_commands.json")

os.makedirs(os.path.dirname(output_path), exist_ok=True)

with open(input_path) as f:
    commands = json.load(f)

filtered = []
for entry in commands:
    if "components" not in entry["file"] and "main" not in entry["file"]:
        continue

    args = entry.get("arguments") or entry["command"].split()
    args = [arg for arg in args if arg not in [
        "-mlongcalls", "-fno-shrink-wrap", "-fno-tree-switch-conversion", "-fstrict-volatile-bitfields"
    ]]

    entry["arguments"] = args
    filtered.append(entry)

with open(output_path, "w") as f:
    json.dump(filtered, f, indent=2)