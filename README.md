# Multipass Technical Test`

This project implements a small C++17 program that fetches Ubuntu release data from the official **Simplestreams** JSON feed. It then provides:

1. A list of supported Ubuntu releases.
2. The current Ubuntu LTS release.
3. The sha256 checksum of the `disk1.img` artifact for a given Ubuntu release.

## Building

Requirements:
- A C++17 compiler (gcc, clang, or MSVC)
- CMake (>=3.10)
- libcurl

Steps:
```bash
mkdir build
cd build
cmake ..
make
