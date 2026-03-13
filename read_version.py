# Read the version number from library.json and inject it as a C++ preprocessor
# definition.

import json

Import("env")

with open("library.json") as f:
    lib = json.load(f)

version = lib.get("version", "0.0.0")

env.Append(CPPDEFINES=[("LIB_VERSION", f'\\"{version}\\"')])
