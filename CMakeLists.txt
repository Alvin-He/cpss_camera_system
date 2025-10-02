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

set(THREADS_PREFER_PTHREAD_FLAG ON)

# fmt 12.0.0
function(fmt)
    set(FMT_TEST OFF)
    set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
    add_subdirectory(libs/fmt EXCLUDE_FROM_ALL SYSTEM)    
endfunction(fmt)
fmt()
# opencv
function(opencv)
    set(ENABLE_FAST_MATH ON)
    set(WITH_TBB ON)
    set(WITH_QT OFF)
    set(WITH_OPENGL ON)
    set(WITH_GSTREAMER ON)
    set(WITH_EIGEN ON)
    set(OPENCV_ENABLE_NONFREE ON)
    set(OPENCV_EXTRA_MODULES_PATH libs/opencv_contrib/modules)
    set(INSTALL_PYTHON_EXAMPLES OFF)
    set(INSTALL_C_EXAMPLES OFF)
    set(BUILD_EXAMPLES OFF)
endfunction(opencv)
opencv()

# main executable
add_executable(csys src/main.cpp)
target_link_libraries(csys PUBLIC fmt::fmt)
