cmake_minimum_required(VERSION 3.24)
project(FFmpeg)

find_package(PkgConfig)
pkg_check_modules(AVCODEC       REQUIRED IMPORTED_TARGET libavcodec)
# pkg_check_modules(AVFILTER      REQUIRED IMPORTED_TARGET libavfilter)
pkg_check_modules(AVFORMAT      REQUIRED IMPORTED_TARGET libavformat)
# pkg_check_modules(AVDEVICE      REQUIRED IMPORTED_TARGET libavdevice)
pkg_check_modules(AVUTIL        REQUIRED IMPORTED_TARGET libavutil)
pkg_check_modules(SWRESAMPLE    REQUIRED IMPORTED_TARGET libswresample)
pkg_check_modules(SWSCALE       REQUIRED IMPORTED_TARGET libswscale)

add_library(FFmpeg INTERFACE IMPORTED GLOBAL)

target_link_libraries(FFmpeg INTERFACE 
    PkgConfig::AVCODEC
    # PkgConfig::AVFILTER
    PkgConfig::AVFORMAT
    # PkgConfig::AVDEVICE
    PkgConfig::AVUTIL
    PkgConfig::SWRESAMPLE
    PkgConfig::SWSCALE
    )

target_include_directories(FFmpeg INTERFACE 
    ${AVCODEC_INCLUDE_DIRS}
    ${AVFORMAT_INCLUDE_DIRS}
    # ${AVFILTER_INCLUDE_DIRS}
    # ${AVDEVICE_INCLUDE_DIRS}
    ${AVUTIL_INCLUDE_DIRS}
    ${SWRESAMPLE_INCLUDE_DIRS}
    ${SWSCALE_INCLUDE_DIRS}
)
target_link_options(FFmpeg INTERFACE
    ${AVCODEC_LDFLAGS}
    ${AVFORMAT_LDFLAGS}
    # ${AVFILTER_LDFLAGS}
    # ${AVDEVICE_LDFLAGS}
    ${AVUTIL_LDFLAGS}
    ${SWRESAMPLE_LDFLAGS}
    ${SWSCALE_LDFLAGS}
)




