cmake_minimum_required(VERSION 3.28)
project(VistaFramework)
set(CMAKE_CXX_STANDARD 23)
set(PROJECT_ROOT .)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

if(WIN32)
    set(USER_HOME $ENV{USERPROFILE})
elseif(UNIX)
    set(USER_HOME $ENV{HOME})
endif()

set(VCPKG_DIR "${USER_HOME}/.vcpkg-clion/Vista")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
link_directories("${VCPKG_DIR}/installed/x64-windows/lib")

include_directories("Vista/include/Vista")
include_directories("Vista/include/stb/") #Download STB from github.com/nothings/stb, and extract to include folder.

# All of the packages installed through vcpkg
# Use MSVC for Windows and GCC for Linux and macOS
find_package(libxml2 REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(libssh2 CONFIG REQUIRED)
find_package(CURL CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Poco REQUIRED COMPONENTS
        Foundation
        Util
        Net
        XML
        JSON
        Data
        Zip
        Redis
)

# Include external libraries installed on the system by automatically searching for them.
# Automatic search is on by default, but you can manually include them if you prefer.
set(AUTOMATIC_SEARCH_VULKAN ON)

if(AUTOMATIC_SEARCH_VULKAN)
    include(FindVulkan)
else()
    set(Vulkan_ROOT "C:/VulkanSDK/1.3.296.0") # Set the root path of Vulkan installation
    set(Vulkan_INCLUDE_DIR "${Vulkan_ROOT}/x86_64/include/")
    set(Vulkan_LIBRARY "${Vulkan_ROOT}/x86_64/lib/libvulkan.so")

    if(EXISTS "${Vulkan_ROOT}" AND IS_DIRECTORY "${Vulkan_ROOT}")
        set(Vulkan_FOUND ON)
    endif()
endif()

set(LIBRARIES
        glfw
        LibXml2::LibXml2
        Libssh2::libssh2
        CURL::libcurl
        OpenSSL::SSL
        OpenSSL::Crypto
        Poco::Foundation
        Poco::Util
        Poco::Net
        Poco::XML
        Poco::JSON
        Poco::Data
        Poco::Zip
        Poco::Redis
)

set(VistaFiles
        "Vista/Graphics/vulkangui.cpp" "Vista/Graphics/vulkangui.hpp"
        "Vista/Core/Application/VConsole.cpp" "Vista/Core/Application/VConsole.hpp"
        "Vista/Core/VObject.cpp" "Vista/Core/VObject.hpp"
        "Vista/Serials/VSerialBus.cpp" "Vista/Serials/VSerialBus.hpp"
        "Vista/Serials/VSerialPort.cpp" "Vista/Serials/VSerialPort.hpp"
        "Vista/Core/VThread.cpp" "Vista/Core/VThread.hpp"
        "Vista/Core/Filesystem/VFile.cpp" "Vista/Core/Filesystem/VFile.hpp"
        "Vista/Core/DataTypes/VString.cpp" "Vista/Core/DataTypes/VString.hpp"
        "Vista/Core/DataTypes/VArray.tpp" "Vista/Core/DataTypes/VArray.hpp"
        "Vista/Core/Application/VApplication.cpp" "Vista/Core/Application/VApplication.hpp"
        "Vista/Core/DataTypes/VInt.tpp" "Vista/Core/DataTypes/VInt.hpp"
)

add_library(VistaFramework
        STATIC
        ${VistaFiles}
)
target_link_libraries(VistaFramework
        PUBLIC
        ${LIBRARIES}
)


add_executable(VistaTest
        ${VistaFiles}
        "Vista/.tests/TestMain.cpp"
        "Vista/.tests/PlatformTest.cpp" "Vista/.tests/PlatformTest.hpp"
        "Vista/.tests/ConsoleTest.cpp" "Vista/.tests/ConsoleTest.hpp"
        "Vista/.tests/ObjectTest.cpp" "Vista/.tests/ObjectTest.hpp"
        "Vista/.tests/ArrayTest.cpp" "Vista/.tests/ArrayTest.hpp"
)
target_link_libraries(VistaTest
        PUBLIC
        VistaFramework
)

add_executable(VistaGUI
        ${VistaFiles}
        Vista/.tests/GUI/VistaGUI.cpp
        Vista/.tests/GUI/VistaGUI.hpp)
target_link_libraries(VistaGUI
        PUBLIC
        VistaFramework
)

if (Vulkan_FOUND)
    message(STATUS "Found Vulkan library: ${Vulkan_LIBRARY}")
    message(STATUS "Found Vulkan include: ${Vulkan_INCLUDE_DIR}")
    include_directories(${Vulkan_INCLUDE_DIR})
    target_link_libraries(VistaFramework PUBLIC ${Vulkan_LIBRARY})
    target_link_libraries(VistaTest PUBLIC ${Vulkan_LIBRARY})
else()
    message(FATAL_ERROR "Vulkan not found.")
endif()