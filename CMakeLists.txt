cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)
project(IntelGPUControl)

# Level Zero disable stuff
set(ZE_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ZE_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ZE_ENABLE_VALIDATION_LAYER OFF CACHE BOOL "" FORCE)

# Disable wxWidgets stuff
set(wxUSE_LIBPNG     OFF CACHE BOOL "" FORCE)
set(wxUSE_LIBJPEG    OFF CACHE BOOL "" FORCE)
set(wxUSE_LIBTIFF    OFF CACHE BOOL "" FORCE)
set(wxUSE_EXPAT      OFF CACHE BOOL "" FORCE)
set(wxUSE_ZLIB       OFF CACHE BOOL "" FORCE)
set(wxUSE_REGEX      OFF CACHE BOOL "" FORCE)
set(wxUSE_RICHTEXT   OFF CACHE BOOL "" FORCE)
set(wxUSE_MEDIACTRL  OFF CACHE BOOL "" FORCE)
set(wxUSE_HTML       OFF CACHE BOOL "" FORCE)
set(wxUSE_WEBVIEW    OFF CACHE BOOL "" FORCE)
set(wxUSE_AUI        OFF CACHE BOOL "" FORCE)
set(wxUSE_RIBBON     OFF CACHE BOOL "" FORCE)
set(wxUSE_PROPGRID   OFF CACHE BOOL "" FORCE)
set(wxUSE_STC        OFF CACHE BOOL "" FORCE)
set(wxUSE_SOCKETS    OFF CACHE BOOL "" FORCE)
set(wxUSE_DEBUGREPORT OFF CACHE BOOL "" FORCE)

set(wxBUILD_SAMPLES  OFF CACHE BOOL "" FORCE)
set(wxBUILD_DEMOS    OFF CACHE BOOL "" FORCE)
set(wxBUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(wxBUILD_INSTALL  OFF CACHE BOOL "" FORCE)

if(MSVC)
    add_compile_options(/MP)
endif()


add_subdirectory(external/level-zero)
add_subdirectory(external/wxWidgets)


add_executable(IntelGPUControl main.cpp)

target_include_directories(IntelGPUControl PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/external/level-zero/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/wxWidgets/include
)
target_link_libraries(IntelGPUControl PRIVATE
    ze_loader
    wx::core
)