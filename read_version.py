import json

Import("env")

with open("library.json") as f:
    lib = json.load(f)

version = lib.get("version", "0.0.0")

# Inject as a preprocessor definition
env.Append(CPPDEFINES=[("LIB_VERSION", f'\\"{version}\\"')])
