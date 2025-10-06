# This file provides build configurations for building all of the source built libraries
# configure command: cmake -S . -B ./build
# build command 

cmake_minimum_required(VERSION 3.26)

project(
    cpss_camera_system_source_build_system 
    VERSION 0.0.0
    LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
# set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/artifacts")

set(THREADS_PREFER_PTHREAD_FLAG ON)

include(ExternalProject)

ExternalProject_Add(
    libjxl
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/libjxl/"
    BINARY_DIR "${CMAKE_BINARY_DIR}/libjxl/"
    INSTALL_DIR "${CMAKE_SOURCE_DIR}/../libs/libjxl"
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
        -DCMAKE_BUILD_TYPE=Release
        -DCMAKE_CXX_STANDARD=23
        -DCMAKE_C_STANDARD=23 
        -DBUILD_SHARED_LIBS=OFF
        -DBUILD_TESTING=OFF
)

# so opencv can find libjxl when configuring
# set(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:${CMAKE_SOURCE_DIR}/../libs/libjxl/lib/pkgconfig")
# ExternalProject_Add_Step(libjxl PATCH_PC 
#     DEPENDEES build
#     DEPENDERS install # for some reason opencv's findjpegxl.cmake looks for jxl.pc instead of libjxl.pc
#     COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/libjxl/lib/libjxl.pc" "${CMAKE_SOURCE_DIR}/../libs/libjxl/lib/pkgconfig/jxl.pc"
# )

# opencv source -> libs/opencv
ExternalProject_Add(
    OpenCV_External
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/opencv/opencv"
    BINARY_DIR "${CMAKE_BINARY_DIR}/opencv/"
    INSTALL_DIR "${CMAKE_SOURCE_DIR}/../libs/opencv"
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
        -DCMAKE_BUILD_TYPE=Release
        -DCMAKE_CXX_STANDARD=17
        -DCMAKE_C_STANDARD=17
        -DBUILD_SHARED_LIBS=OFF # off cuz we aren't installing to a normal location so we won't have the right env vars
        -DBUILD_EXAMPLES=OFF
        -DBUILD_TESTS=OFF
        -DENABLE_FAST_MATH=ON
        -DWITH_TBB=ON
        -DWITH_QT=OFF
        -DWITH_OPENGL=ON
        -DOPENCV_ENABLE_NONFREE=ON
        -DBUILD_opencv_python2=OFF
        -DBUILD_opencv_python3=OFF
        -DBUILD_JAVA=OFF
        -DOPENCV_EXTRA_MODULES_PATH:PATH=<SOURCE_DIR>/../opencv_contrib/modules
        -DWITH_EIGEN=ON # linux only
        -DWITH_LAPACK=ON # linux only
        # -DWITH_JPEGXL=ON
)
# ExternalProject_Add_StepDependencies(OpenCV_External configure libjxl)