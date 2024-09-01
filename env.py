import os
Import("env")
import time

# include toolchain paths
env.Replace(COMPILATIONDB_INCLUDE_TOOLCHAIN=True)

# override compilation DB path
path = os.path.join(".compilationdb", env['PIOENV'], "compile_commands.json")
os.makedirs(os.path.dirname(path), exist_ok=True)
env.Replace(COMPILATIONDB_PATH=path)


def after_upload(source, target, env):
    time.sleep(1)  # delay for the port to be ready

env.AddPostAction("upload", after_upload)

