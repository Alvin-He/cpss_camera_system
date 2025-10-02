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
set(OPENCV_MAP_IMPORTED_CONFIG "")
find_package(OpenCV REQUIRED PATHS libs/opencv)

# main executable
add_executable(csys src/main.cpp)

target_link_libraries(csys 
    PUBLIC fmt::fmt
    PUBLIC ${OpenCV_LIBS}
)
