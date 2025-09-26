# type: ignore
Import("env")
env.AddPostAction("buildprog", "platformio run --target compiledb")