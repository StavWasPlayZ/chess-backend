cmake_minimum_required(VERSION 3.5.0)
project(chess_backend VERSION 0.1.0 LANGUAGES C CXX)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(chess_backend ${SOURCES})

