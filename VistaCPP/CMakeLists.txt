cmake_minimum_required(VERSION 3.28)
project(VistaFramework)
set(CMAKE_CXX_STANDARD 23)
set(PROJECT_ROOT ..)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

if(WIN32)
    set(USER_HOME $ENV{USERPROFILE})
elseif(UNIX)
    set(USER_HOME $ENV{HOME})
endif()

set(VCPKG_DIR "${USER_HOME}/.vcpkg-clion/Vista")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
link_directories("${VCPKG_DIR}/installed/x64-windows/lib")

include_directories("include/Vista")
include_directories("include/stb/") #Download STB from github.com/nothings/stb, and extract to include folder.

# All of the packages installed through vcpkg
# Use MSVC for Windows and GCC for Linux and macOS
find_package(libxml2 REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(Libssh2 CONFIG REQUIRED)
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
    set(Vulkan_ROOT "/opt/Vulkan-1.3.290.0") # Set the root path of Vulkan installation
    set(Vulkan_INCLUDE_DIR "${Vulkan_ROOT}/x86_64/include/")
    set(Vulkan_LIBRARY "${Vulkan_ROOT}/x86_64/lib/libvulkan.so")

    if(EXISTS "${Vulkan_ROOT}" AND IS_DIRECTORY "${Vulkan_ROOT}")
        set(Vulkan_FOUND ON)
    endif()
endif()

set(LIBRARIES
        glfw
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
        "Graphics/vulkangui.cpp" "Graphics/vulkangui.hpp"
        "Core/Application/VConsole.cpp" "Core/Application/VConsole.hpp"
        "Core/VObject.cpp" "Core/VObject.hpp"
        "Serials/VSerialBus.cpp" "Serials/VSerialBus.hpp"
        "Serials/VSerialPort.cpp" "Serials/VSerialPort.hpp"
        "Core/VThread.cpp" "Core/VThread.hpp"
        "Core/Filesystem/VFile.cpp" "Core/Filesystem/VFile.hpp"
        "Core/DataTypes/VString.cpp" "Core/DataTypes/VString.hpp"
        "Core/DataTypes/VArray.tpp" "Core/DataTypes/VArray.hpp"
        "Core/Application/VApplication.cpp" "Core/Application/VApplication.hpp"
        "Core/DataTypes/VInt.tpp" "Core/DataTypes/VInt.hpp"
)

add_library(VistaFramework
        STATIC
        ${VistaFiles}
)
target_link_libraries(VistaFramework
        PUBLIC
        ${LIBRARIES}
        ${LIBXML2_LIBRARIES}
        $<IF:$<TARGET_EXISTS:Libssh2::libssh2_shared>,Libssh2::libssh2_shared,Libssh2::libssh2_static>
)


add_executable(VistaTest
        ${VistaFiles}
        ".tests/TestMain.cpp"
        ".tests/PlatformTest.cpp" ".tests/PlatformTest.hpp"
        ".tests/ConsoleTest.cpp" ".tests/ConsoleTest.hpp"
        ".tests/ObjectTest.cpp" ".tests/ObjectTest.hpp"
        ".tests/ArrayTest.cpp" ".tests/ArrayTest.hpp"
)
target_link_libraries(VistaTest
        PUBLIC
        VistaFramework
)

add_executable(VistaGUI
        ${VistaFiles}
        .tests/GUI/VistaGUI.cpp
        .tests/GUI/VistaGUI.hpp)
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