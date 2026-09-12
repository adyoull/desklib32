#!/usr/bin/env python3
"""
package.py - regenerate build/desklib_src.zip from ../src.

builddesklib.py builds from desklib_src.zip (it is the build input the RISC OS
online service unpacks). ../src is the same content, unpacked, as the readable
source of truth. Run this after editing anything under ../src so the zip the
build actually consumes matches the source in the repo.

    cd build && python3 package.py && python3 builddesklib.py
"""
import os, zipfile

HERE = os.path.dirname(os.path.abspath(__file__))
SRC  = os.path.normpath(os.path.join(HERE, "..", "src"))
OUT  = os.path.join(HERE, "desklib_src.zip")

def main():
    files = []
    for root, _dirs, names in os.walk(SRC):
        for n in names:
            if n == ".DS_Store":
                continue
            full = os.path.join(root, n)
            rel  = os.path.relpath(full, SRC).replace(os.sep, "/")
            files.append((full, rel))
    files.sort(key=lambda t: t[1])
    with zipfile.ZipFile(OUT, "w", zipfile.ZIP_DEFLATED) as z:
        for full, rel in files:
            z.write(full, rel)
    print("wrote %s (%d files)" % (OUT, len(files)))

if __name__ == "__main__":
    main()
