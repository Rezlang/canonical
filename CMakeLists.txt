cmake_minimum_required(VERSION 3.10)
project(canonical CXX)

# Require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(nlohmann_json 3.11.2 QUIET)
if (NOT nlohmann_json_FOUND)
    include(FetchContent)
    FetchContent_Declare(
      json
      GIT_REPOSITORY https://github.com/nlohmann/json.git
      GIT_TAG v3.11.2
    )
    FetchContent_MakeAvailable(json)
endif()

# Find cURL system-wide
find_package(CURL REQUIRED)
message(STATUS "Found CURL: ${CURL_LIBRARIES}, includes: ${CURL_INCLUDE_DIRS}")

# Include directories for your headers
include_directories(include)

# Sources
set(SOURCES
    src/Main.cpp
    src/UbuntuReleaseInfoImpl.cpp
    src/HttpClient.cpp
)

# Define the executable
add_executable(canonical ${SOURCES})

# Link libraries to the executable
target_link_libraries(canonical PRIVATE CURL::libcurl nlohmann_json::nlohmann_json)
