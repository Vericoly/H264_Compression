cmake_minimum_required(VERSION 3.24)
project(H264_Compression C CXX )

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(lib/FFmpeg)

add_executable(H264_Compression src/main.cpp)
target_include_directories(H264_Compression PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(H264_Compression FFmpeg)
