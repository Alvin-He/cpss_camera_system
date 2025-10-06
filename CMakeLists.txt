cmake_minimum_required(VERSION 3.26)

project(
    cpss_camera_system 
    VERSION 0.0.0
    LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
# set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/artifacts")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "force to static due to opencv constraint" FORCE)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
set(THREADS_PREFER_PTHREAD_FLAG ON)

# fmt 12.0.0
function(fmt)
    set(FMT_TEST OFF)
    set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
    add_subdirectory(libs/fmt EXCLUDE_FROM_ALL SYSTEM)    
endfunction(fmt)
fmt()

# opencv
set(OPENCV_MAP_IMPORTED_CONFIG "")
find_package(Eigen3 QUIET) # litertlly opencv being weird if built on linux 
find_package(Iconv QUIET)
find_package(OpenCV REQUIRED PATHS libs/opencv)

# libjxl
function(libjxl)
    find_package(PkgConfig)
    set(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:${CMAKE_SOURCE_DIR}/libs/libjxl/lib/pkgconfig")
    pkg_check_modules(Jxl REQUIRED IMPORTED_TARGET libjxl libjxl_cms libjxl_threads libbrotlicommon)
endfunction(libjxl)
libjxl()

# main executable
add_executable(csys src/main.cpp)
target_link_libraries(csys 
    PUBLIC fmt::fmt
    PUBLIC PkgConfig::Jxl
    PUBLIC ${OpenCV_LIBS}
)
