if (WIN32)
    find_path(Vulkan_INCLUDE_DIR NAMES vulkan/vulkan.h
            HINTS "$ENV{VULKAN_SDK}/Include" "$ENV{VK_SDK_PATH}/Include"
            PATH_SUFFIXES include)

    if (CMAKE_CL_64)
        find_library(Vulkan_LIBRARY NAMES vulkan-1
                HINTS "$ENV{VULKAN_SDK}/Lib" "$ENV{VK_SDK_PATH}/Lib")
    else()
        find_library(Vulkan_LIBRARY NAMES vulkan-1
                HINTS "$ENV{VULKAN_SDK}/Lib32" "$ENV{VK_SDK_PATH}/Lib32")
    endif()
else()
    find_path(Vulkan_INCLUDE_DIR NAMES vulkan/vulkan.h
            HINTS "$ENV{VULKAN_SDK}/include" "/usr/include" "/usr/local/include"
            PATH_SUFFIXES vulkan)

    find_library(Vulkan_LIBRARY NAMES vulkan
            HINTS "$ENV{VULKAN_SDK}/lib" "/usr/lib" "/usr/local/lib"
            PATH_SUFFIXES x86_64-linux-gnu)

    # Print out found paths for debugging
    message(STATUS "Vulkan_INCLUDE_DIR: ${Vulkan_INCLUDE_DIR}")
    message(STATUS "Vulkan_LIBRARY: ${Vulkan_LIBRARY}")
endif()

# Handle the results
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Vulkan REQUIRED_VARS Vulkan_LIBRARY Vulkan_INCLUDE_DIR)
mark_as_advanced(Vulkan_INCLUDE_DIR Vulkan_LIBRARY)