cmake_minimum_required(VERSION 3.24)
project(H264_Compression C )

add_subdirectory(lib/FFmpeg)

add_executable(H264_Compression src/main.c)
target_link_libraries(H264_Compression FFmpeg)