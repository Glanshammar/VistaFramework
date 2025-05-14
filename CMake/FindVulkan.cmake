# FindVulkan.cmake - Find Vulkan SDK
# Defines the following variables:
#  Vulkan_FOUND        - True if Vulkan was found
#  Vulkan_INCLUDE_DIR  - Include directories for Vulkan
#  Vulkan_LIBRARY      - Vulkan library to link against

# Check for VULKAN_SDK environment variable first
if(NOT "$ENV{VULKAN_SDK}" STREQUAL "")
    set(VULKAN_SDK_PATH "$ENV{VULKAN_SDK}")
    message(STATUS "Using Vulkan SDK path from environment: ${VULKAN_SDK_PATH}")
endif()

if (WIN32)
    # Windows-specific paths
    find_path(Vulkan_INCLUDE_DIR NAMES vulkan/vulkan.h
            HINTS 
                "${VULKAN_SDK_PATH}/Include"
                "$ENV{VK_SDK_PATH}/Include"
            PATH_SUFFIXES include)

    if (CMAKE_CL_64)
        find_library(Vulkan_LIBRARY NAMES vulkan-1
                HINTS 
                    "${VULKAN_SDK_PATH}/Lib" 
                    "$ENV{VK_SDK_PATH}/Lib")
    else()
        find_library(Vulkan_LIBRARY NAMES vulkan-1
                HINTS 
                    "${VULKAN_SDK_PATH}/Lib32" 
                    "$ENV{VK_SDK_PATH}/Lib32")
    endif()
else()
    # Linux-specific paths (more comprehensive search)
    find_path(Vulkan_INCLUDE_DIR NAMES vulkan/vulkan.h
            HINTS 
                "${VULKAN_SDK_PATH}/include"
                "/usr/include"
                "/usr/local/include"
                "/opt/vulkan/include"
            PATH_SUFFIXES vulkan)

    # On openSUSE, the library is usually in /usr/lib64
    find_library(Vulkan_LIBRARY 
            NAMES 
                vulkan
                libvulkan.so.1
                libvulkan.so
            HINTS 
                "${VULKAN_SDK_PATH}/lib"
                "/usr/lib"
                "/usr/lib64"
                "/usr/local/lib"
                "/usr/local/lib64"
                "/opt/vulkan/lib"
            PATH_SUFFIXES 
                x86_64-linux-gnu
                i386-linux-gnu
                arm-linux-gnueabihf
                aarch64-linux-gnu)

    # Print debug info
    message(STATUS "Vulkan search paths:")
    message(STATUS "  - VULKAN_SDK: $ENV{VULKAN_SDK}")
    message(STATUS "  - include: ${Vulkan_INCLUDE_DIR}")
    message(STATUS "  - library: ${Vulkan_LIBRARY}")
    
    # On openSUSE, try to manually install Vulkan if not found
    if(NOT Vulkan_INCLUDE_DIR OR NOT Vulkan_LIBRARY)
        message(STATUS "Vulkan not found in standard locations. On openSUSE, you can install it with:")
        message(STATUS "  sudo zypper install vulkan-devel vulkan-loader")
    endif()
endif()

# Handle the results
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Vulkan REQUIRED_VARS Vulkan_LIBRARY Vulkan_INCLUDE_DIR)
mark_as_advanced(Vulkan_INCLUDE_DIR Vulkan_LIBRARY)