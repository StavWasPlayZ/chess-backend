cmake_minimum_required(VERSION 3.5.0)
project(chess_backend VERSION 0.1.0 LANGUAGES C CXX)

file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(chess_backend ${SOURCES})

# Only include Linux commincation on non-Windows devices.
# (Technically this works on all Unix devices, so 'NOT WIN32'.)
if(NOT WIN32)
    set(LINUX_COMM_PATH "${CMAKE_SOURCE_DIR}/comm/linux")

    target_include_directories(chess_backend PRIVATE ${LINUX_COMM_PATH}/)

    file(GLOB_RECURSE LINUX_COMM ${LINUX_COMM_PATH}/*.cpp)
    target_sources(chess_backend PRIVATE ${LINUX_COMM})
endif()
