cmake_minimum_required(VERSION 3.5.0)
project(chess_backend VERSION 0.1.0 LANGUAGES C CXX)

file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE COMM_COMMON "${CMAKE_SOURCE_DIR}/comm/common/*.cpp")

add_executable(chess_backend ${SOURCES} ${COMM_COMMON})


# Named Pipes

set(COMM_PATH "${CMAKE_SOURCE_DIR}/comm")

# Only include Linux commincation on non-Windows devices.
# (Technically this works on all Unix devices, so 'NOT WIN32'.)
if(NOT WIN32)
    set(COMM_OS_PATH "${COMM_PATH}/linux")
else()
    set(COMM_OS_PATH "${COMM_PATH}/windows")
endif()

target_include_directories(chess_backend PRIVATE "${COMM_PATH}/common")

file(GLOB_RECURSE COMM ${COMM_OS_PATH}/*.cpp)
target_sources(chess_backend PRIVATE ${COMM})