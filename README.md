# Multipass Technical Test

This project implements a small C++ program that fetches Ubuntu release data from the official **Simplestreams** JSON feed. It then provides:

1. A list of supported Ubuntu releases.
2. The current Ubuntu LTS release.
3. The sha256 of the `disk1.img` artifact for a given Ubuntu release.

## Building

Requirements:
- A C++17 compiler (gcc, clang, or MSVC)
- CMake (>=3.10)
- libcurl

Steps:
### Mac / Linux
```bash
mkdir build
cd build
cmake ..
make
./canonical
```
### Windows
Note: The windows installation is untested as I do not currently have access to a windows machine set up for development.
```bash
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release
.\Release\canonical.exe
```

Github: https://github.com/Rezlang/canonical

Thank you for the opportunity to demonstrate my C++ skills through this project. I am excited about the prospect of advancing to the next stage of the interview process and contributing to Canonical as soon as possible.

