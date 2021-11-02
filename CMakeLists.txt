cmake_minimum_required(VERSION 3.10)

enable_language(CXX)

project(mobo VERSION 1.0.002 LANGUAGES CXX)

configure_file(MoboConfig.h.in MoboConfig.h)

include(${CMAKE_SOURCE_DIR}/base_includes.txt)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/toolchain.txt")

set(LIBREALSENSE_BASE, "/home/$ENV{USER}/Documents/librealsense")
set(LIBS "/home/$ENV{USER}/Documents/librealsense/build/librealsense2.so" "/home/$ENV{USER}/Documents/librealsense/build/third-party/glfw/src/libglfw3.a" /usr/local/lib/libglut.a png GL GLU rt dl m X11 Xrandr Xxf86vm Xi pthread uuid)

add_definitions(-DBUILD_EASYLOGGINGPP -DBUILD_SHARED_LIBS -DCHECK_FOR_UPDATES -DCOM_MULTITHREADED -DCURL_STATICLIB -DEASYLOGGINGPP_ASYNC -DELPP_NO_DEFAULT_LOG_FILE -DELPP_THREAD_SAFE -DHWM_OVER_XU -DRS2_USE_V4L2_BACKEND -DSQLITE_HAVE_ISNAN -DUNICODE)

add_subdirectory(GUI)
add_subdirectory(Math)
add_subdirectory(lodepng)

add_executable(mobo main.cpp)

target_include_directories(mobo PUBLIC "${LIBREALSENSE_BASE}/build" "${LIBREALSENSE_BASE}/include" "${LIBREALSENSE_BASE}/third-party/glfw/include" /usr/local/cuda/include)
target_link_libraries(mobo PUBLIC GUI Math lodepng "${LIBS}")
