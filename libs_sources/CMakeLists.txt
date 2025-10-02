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

# opencv source -> libs/opencv
ExternalProject_Add(
    OpenCV_External
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/opencv/opencv"
    BINARY_DIR "${CMAKE_BINARY_DIR}/opencv/"
    INSTALL_DIR "${CMAKE_SOURCE_DIR}/../libs/opencv"
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
        -DCMAKE_BUILD_TYPE=Release
        # -DBUILD_SHARED_LIBS=OFF # Or ON, depending on your needs
        -DBUILD_EXAMPLES=OFF
        -DBUILD_TESTS=ON
        -DENABLE_FAST_MATH=ON
        -DWITH_TBB=ON
        -DWITH_QT=OFF
        -DWITH_OPENGL=ON
        -DOPENCV_ENABLE_NONFREE=ON
        -DBUILD_opencv_python2=OFF
        -DBUILD_opencv_python3=OFF
        -DBUILD_JAVA=OFF
        -DOPENCV_EXTRA_MODULES_PATH:PATH=<SOURCE_DIR>/../opencv_contrib/modules
)
