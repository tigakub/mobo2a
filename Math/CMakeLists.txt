get_filename_component(LIBNAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

include(${CMAKE_SOURCE_DIR}/base_includes.txt)

FILE(GLOB SOURCES *.cpp)
add_library(${LIBNAME} ${SOURCES})

target_include_directories(${LIBNAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
