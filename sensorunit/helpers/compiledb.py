# /helpers/compiledb.py
# This runs an extra script after buildprog (compilation)
# that generates a compile_commands.json file for Intellisense
# type: ignore
Import("env")
env.AddPostAction("buildprog", "platformio run --target compiledb")