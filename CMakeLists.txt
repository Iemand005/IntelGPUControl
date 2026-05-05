cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)
project(IntelGPUControl)

set(ZE_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ZE_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ZE_ENABLE_VALIDATION_LAYER OFF CACHE BOOL "" FORCE)


add_subdirectory(external/level-zero)


add_executable(IntelGPUControl main.cpp)

target_include_directories(IntelGPUControl PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/external/level-zero/include
)
target_link_libraries(IntelGPUControl PRIVATE ze_loader)