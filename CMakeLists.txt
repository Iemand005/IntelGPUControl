cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)
project(IntelGPUControl)

# Level Zero disable stuff
set(ZE_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ZE_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ZE_ENABLE_VALIDATION_LAYER OFF CACHE BOOL "" FORCE)

# Disable wxWidgets stuff
# set(wxUSE_LIBPNG     OFF CACHE BOOL "" FORCE)
# set(wxUSE_LIBJPEG    OFF CACHE BOOL "" FORCE)
# set(wxUSE_LIBWEBP    OFF CACHE BOOL "" FORCE)
# set(wxUSE_NANOSVG    OFF CACHE BOOL "" FORCE)
# set(wxUSE_LIBTIFF    OFF CACHE BOOL "" FORCE)
# set(wxUSE_EXPAT      OFF CACHE BOOL "" FORCE)
# set(wxUSE_ZLIB       OFF CACHE BOOL "" FORCE)
# set(wxUSE_REGEX      OFF CACHE BOOL "" FORCE)
# set(wxUSE_RICHTEXT   OFF CACHE BOOL "" FORCE)
# set(wxUSE_MEDIACTRL  OFF CACHE BOOL "" FORCE)
# set(wxUSE_HTML       OFF CACHE BOOL "" FORCE)
# set(wxUSE_WEBVIEW    OFF CACHE BOOL "" FORCE)
# set(wxUSE_AUI        OFF CACHE BOOL "" FORCE)
# set(wxUSE_RIBBON     OFF CACHE BOOL "" FORCE)
# set(wxUSE_PROPGRID   OFF CACHE BOOL "" FORCE)
# set(wxUSE_STC        OFF CACHE BOOL "" FORCE)
# set(wxUSE_SOCKETS    OFF CACHE BOOL "" FORCE)
# set(wxUSE_PROTOCOL_FTP    OFF CACHE BOOL "" FORCE)
# set(wxUSE_PROTOCOL_HTTP    OFF CACHE BOOL "" FORCE)
# set(wxUSE_DEBUGREPORT OFF CACHE BOOL "" FORCE)
# set(wxUSE_ARTPROVIDER_TANGO OFF CACHE BOOL "" FORCE)
# set(wxUSE_ARTPROVIDER_STD   OFF CACHE BOOL "" FORCE)
# set(wxUSE_IMAGE            OFF CACHE BOOL "" FORCE)
# set(wxUSE_STREAMS          ON CACHE BOOL "" FORCE)

# set(wxUSE_LIBPNG     OFF CACHE BOOL "" FORCE)
# set(wxUSE_LIBJPEG    OFF CACHE BOOL "" FORCE)
# set(wxUSE_LIBTIFF    OFF CACHE BOOL "" FORCE)
# set(wxUSE_TARSTREAM  OFF CACHE BOOL "" FORCE)
# set(wxUSE_FILESYSTEM OFF CACHE BOOL "" FORCE)
# set(wxUSE_FS_ARCHIVE OFF CACHE BOOL "" FORCE)
# set(wxUSE_FS_ZIP     OFF CACHE BOOL "" FORCE)
# set(wxUSE_IMAGE      OFF CACHE BOOL "" FORCE)
# # set(wxUSE_STREAMS    OFF CACHE BOOL "" FORCE)
# set(wxUSE_PROTOCOL    OFF CACHE BOOL "" FORCE)
# set(wxUSE_URL    OFF CACHE BOOL "" FORCE)
# set(wxUSE_FS_INET    OFF CACHE BOOL "" FORCE)
# set(wxUSE_ANIMATIONCTRL    OFF CACHE BOOL "" FORCE)
# set(wxUSE_WXHTML_HELP    OFF CACHE BOOL "" FORCE)
# set(wxUSE_DRAGIMAGE    OFF CACHE BOOL "" FORCE)
# set(wxUSE_GIF    OFF CACHE BOOL "" FORCE)
# set(wxUSE_PNM    OFF CACHE BOOL "" FORCE)

set(wxUSE_AUI OFF CACHE BOOL "Disable AUI")
set(wxUSE_HTML OFF CACHE BOOL "Disable HTML")
set(wxUSE_WEBVIEW OFF CACHE BOOL "Disable WebView")
set(wxUSE_MEDIACTRL OFF CACHE BOOL "Disable MediaCtrl")
set(wxUSE_RICHTEXT OFF CACHE BOOL "Disable RichText")
set(wxUSE_LIBJPEG OFF CACHE BOOL "Disable JPEG support")
set(wxUSE_LIBTIFF OFF CACHE BOOL "Disable TIFF support")
set(wxUSE_EXPAT   OFF CACHE BOOL "Disable XML parsing (Expat)")
set(wxUSE_ZLIB    OFF CACHE BOOL "Disable Zlib compression")
set(wxUSE_LIBJPEG OFF CACHE BOOL "" FORCE)
set(wxUSE_LIBTIFF OFF CACHE BOOL "" FORCE)
set(wxUSE_EXPAT   OFF CACHE BOOL "" FORCE)
set(wxUSE_WEBVIEW OFF CACHE BOOL "" FORCE)
set(wxUSE_NANOSVG OFF CACHE BOOL "" FORCE)
set(wxUSE_LIBWEBP OFF CACHE BOOL "" FORCE)
set(wxUSE_REGEX   OFF CACHE BOOL "" FORCE)
set(wxUSE_ARTPROVIDER_TANGO OFF CACHE BOOL "" FORCE)
set(wxUSE_ARTPROVIDER_STD   OFF CACHE BOOL "" FORCE)
set(wxUSE_GIF OFF CACHE BOOL "" FORCE)
set(wxUSE_GIF OFF CACHE BOOL "" FORCE)
set(wxUSE_PNM       OFF CACHE BOOL "" FORCE)
set(wxUSE_PCX       OFF CACHE BOOL "" FORCE)
set(wxUSE_IFF       OFF CACHE BOOL "" FORCE)
set(wxUSE_TGA       OFF CACHE BOOL "" FORCE)
set(wxUSE_XPM       OFF CACHE BOOL "" FORCE)
set(wxUSE_SVG       OFF CACHE BOOL "" FORCE)

set(wxUSE_DRAGIMAGE OFF CACHE BOOL "" FORCE)
set(wxUSE_TREELISTCTRL OFF CACHE BOOL "" FORCE)
set(wxUSE_BITMAPCOMBOBOX OFF CACHE BOOL "" FORCE)
set(wxUSE_TOOLBOOK    OFF CACHE BOOL "" FORCE)
set(wxUSE_AUIPANEINFO OFF CACHE BOOL "" FORCE)
set(wxUSE_BUTTONBAR   OFF CACHE BOOL "" FORCE)
set(wxUSE_TREEBOOK    OFF CACHE BOOL "" FORCE)
set(wxUSE_DOC_VIEW_FRAMEWORK OFF CACHE BOOL "" FORCE)
set(wxUSE_PRINTING_ARCHITECTURE OFF CACHE BOOL "" FORCE)

set(wxUSE_ZLIB OFF CACHE BOOL "" FORCE)
set(wxUSE_IMAGE ON CACHE BOOL "" FORCE)
set(wxUSE_LIBPNG OFF CACHE BOOL "" FORCE)
set(wxUSE_TOOLBAR OFF CACHE BOOL "" FORCE)
set(wxUSE_TOOLBAR_NATIVE OFF CACHE BOOL "" FORCE)

set(wxBUILD_SAMPLES  OFF CACHE BOOL "" FORCE)
set(wxBUILD_DEMOS    OFF CACHE BOOL "" FORCE)
set(wxBUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(wxBUILD_INSTALL  OFF CACHE BOOL "" FORCE)

set(wxBUILD_SHARED OFF CACHE BOOL "" FORCE)

set(CMAKE_VS_SPECTRE_MITIGATION "OFF")

if(MSVC)
    add_compile_options(/MP6)
endif()


add_subdirectory(external/level-zero)
add_subdirectory(external/wxWidgets)

# Disable spectre
if(MSVC)
    set(FLAG_VARS 
        CMAKE_CXX_FLAGS 
        CMAKE_C_FLAGS 
        CMAKE_CXX_FLAGS_DEBUG 
        CMAKE_CXX_FLAGS_RELEASE 
        CMAKE_C_FLAGS_DEBUG 
        CMAKE_C_FLAGS_RELEASE
    )

    foreach(flag_var ${FLAG_VARS})
        if(${flag_var})
            string(REPLACE "/Qspectre" "" updated_flags "${${flag_var}}")
            set(${flag_var} "${updated_flags} /Qspectre-" CACHE STRING "Disabled Spectre" FORCE)
        endif()
    endforeach()
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

if(MSVC)
    add_compile_options(/Zi)
    add_link_options(/DEBUG)
else()
    add_compile_options(-g)
endif()




add_executable(IntelGPUControl WIN32 main.cpp)

target_include_directories(IntelGPUControl PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/external/level-zero/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/wxWidgets/include
)
target_link_libraries(IntelGPUControl PRIVATE
    ze_loader
    wx::core
)