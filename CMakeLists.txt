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

# Detect Platform
if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(PLATFORM "macOS")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(PLATFORM "Linux")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(PLATFORM "Windows")
else()
    message(FATAL_ERROR "Unsupported platform!")
endif()

# Find and Link CURL
if (PLATFORM STREQUAL "Linux" OR PLATFORM STREQUAL "macOS")
    find_package(CURL REQUIRED)
    if (CURL_FOUND)
        message(STATUS "Found CURL: ${CURL_LIBRARY}, includes: ${CURL_INCLUDE_DIR}")
    else()
        message(FATAL_ERROR "CURL not found on ${PLATFORM}")
    endif()
elseif (PLATFORM STREQUAL "Windows")
    # On Windows, manually locate CURL library
    set(CURL_INCLUDE_DIR "C:/curl/include")
    set(CURL_LIBRARY "C:/curl/lib/libcurl.lib")
    include_directories(${CURL_INCLUDE_DIR})
    link_libraries(${CURL_LIBRARY})
endif()

# Platform-specific compiler definitions
if (PLATFORM STREQUAL "macOS")
    message(STATUS "Configuring for macOS")
    # macOS curl library path (Homebrew installation)
    execute_process(COMMAND brew --prefix OUTPUT_VARIABLE BREW_PREFIX OUTPUT_STRIP_TRAILING_WHITESPACE)
    set(CURL_INCLUDE_DIR "${BREW_PREFIX}/include")
    set(CURL_LIBRARY "${BREW_PREFIX}/lib/libcurl.dylib")
    include_directories(${CURL_INCLUDE_DIR})
elseif (PLATFORM STREQUAL "Windows")
    message(STATUS "Configuring for Windows")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif (PLATFORM STREQUAL "Linux")
    message(STATUS "Configuring for Linux")
    # Nothing extra needed for Linux
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
if (PLATFORM STREQUAL "Windows")
    target_link_libraries(canonical PRIVATE ${CURL_LIBRARY} nlohmann_json::nlohmann_json)
else()
    target_link_libraries(canonical PRIVATE CURL::libcurl nlohmann_json::nlohmann_json)
endif()
