cmake_minimum_required(VERSION 3.20)

project(TZOOTZ_ASIO
    VERSION 0.1.0
    DESCRIPTION "Prototipo inicial del driver TZOOTZ_ASIO"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Opciones de RtAudio
set(RTAUDIO_BUILD_TESTING OFF CACHE BOOL "Disable RtAudio tests" FORCE)
set(RTAUDIO_BUILD_EXAMPLES OFF CACHE BOOL "Disable RtAudio examples" FORCE)
set(RTAUDIO_API_ASIO ON CACHE BOOL "Enable ASIO backend" FORCE)

include(FetchContent)
FetchContent_Declare(
    rtaudio
    GIT_REPOSITORY https://github.com/thestk/rtaudio.git
    GIT_TAG 6.0.1
)

FetchContent_MakeAvailable(rtaudio)

add_executable(tzootz_asio
    src/main.cpp
    src/RealtekDetector.cpp
    src/TzootzAsioEngine.cpp)

target_compile_features(tzootz_asio PRIVATE cxx_std_17)

if (WIN32)
    target_compile_definitions(tzootz_asio PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN)
endif()

target_include_directories(tzootz_asio
    PRIVATE
        ${rtaudio_SOURCE_DIR}/include
        ${rtaudio_BINARY_DIR}/include)

target_link_libraries(tzootz_asio
    PRIVATE
        rtaudio)
