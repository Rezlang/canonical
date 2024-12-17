cmake_minimum_required(VERSION 3.10)
include(FetchContent)
project(canonical CXX)

# Require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# FetchContent configuration for nlohmann_json
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(json)

# Find and link CURL
find_package(CURL REQUIRED)

if (CURL_FOUND)
    message(STATUS "Found CURL: ${CURL_LIBRARY}, includes: ${CURL_INCLUDE_DIR}")
else()
    set(CURL_INCLUDE_DIR "/usr/include/aarch64-linux-gnu/curl")
    set(CURL_LIBRARY "/usr/lib/aarch64-linux-gnu/libcurl.so")
endif()

# Sources and headers
set(SOURCES
    src/Main.cpp
    src/UbuntuReleaseInfoImpl.cpp
    src/HttpClient.cpp
)

include_directories(include)

# Define the executable
add_executable(canonical ${SOURCES})

# Link libraries to the executable
target_link_libraries(canonical PRIVATE CURL::libcurl nlohmann_json::nlohmann_json)
