from pathlib import Path
import json

repo_root = Path(__file__).parent.parent
build_dir = repo_root / "build"

root_cc = build_dir / "compile_commands.json"
test_cc = repo_root / "test_runner" / "build" / "compile_commands.json"

if not test_cc.exists():
    print("No test_runner/build/compile_commands.json found")
    exit(0)

with root_cc.open() as f1, test_cc.open() as f2:
    merged = json.load(f1) + json.load(f2)

with root_cc.open("w") as out:
    json.dump(merged, out, indent=2)

print(f"Merged main and test_runner compile_commands.json to {root_cc}")