cmake_minimum_required(VERSION 3.28)

# Set vcpkg toolchain file immediately - MUST BE BEFORE project()
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
    message(STATUS "Using vcpkg from: $ENV{VCPKG_ROOT}")
else()
    message(FATAL_ERROR "VCPKG_ROOT environment variable is not set. Please set it to your vcpkg installation directory.")
endif()

project(VistaFramework)

set(CMAKE_CXX_STANDARD 23)
set(PROJECT_ROOT .)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

# Install dependencies using vcpkg
set(VCPKG_INSTALLED_DIR "${CMAKE_BINARY_DIR}/vcpkg_installed")
message(STATUS "Looking for packages in: ${VCPKG_INSTALLED_DIR}")

# Include directories
include_directories("Vista/include/Vista")
include_directories("Vista/include/stb/") #Download STB from https://github.com/nothings/stb, and extract to include folder ./Vista/include/stb/


include(FindVulkan)
if(NOT Vulkan_FOUND)
    message(WARNING "Vulkan not found. Some features will be disabled. To install Vulkan on openSUSE: sudo zypper install vulkan-devel vulkan-loader")
    # Create a dummy Vulkan interface for compilation to succeed
    add_library(Vulkan::Vulkan INTERFACE IMPORTED GLOBAL)
    set_target_properties(Vulkan::Vulkan PROPERTIES
        INTERFACE_COMPILE_DEFINITIONS "VULKAN_DISABLED"
    )
else()
    include_directories(${Vulkan_INCLUDE_DIR})
    # Create a proper Vulkan interface
    add_library(Vulkan::Vulkan INTERFACE IMPORTED GLOBAL)
    set_target_properties(Vulkan::Vulkan PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${Vulkan_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "${Vulkan_LIBRARY}"
    )
endif()


# All of the packages installed through vcpkg
find_package(LibXml2 REQUIRED)
find_package(libssh2 REQUIRED)
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Poco REQUIRED COMPONENTS
        Foundation
        Util
        Net
        XML
        JSON
        Zip
        Redis
)

# Find X11 for GUI
find_package(X11 REQUIRED)
if(X11_FOUND)
    message(STATUS "Found X11:")
    message(STATUS "  - Include Dir: ${X11_INCLUDE_DIR}")
    message(STATUS "  - Libraries: ${X11_LIBRARIES}")
    include_directories(${X11_INCLUDE_DIR})
endif()

# Libraries to link
set(LIBRARIES
    LibXml2::LibXml2
    CURL::libcurl
    OpenSSL::SSL
    OpenSSL::Crypto
    Poco::Foundation
    Poco::Util
    Poco::Net
    Poco::XML
    Poco::JSON
    Poco::Zip
    Poco::Redis
    Vulkan::Vulkan
    ${X11_LIBRARIES}
)

# libssh2 has inconsistent target naming across different versions
if(TARGET libssh2::libssh2)
    list(APPEND LIBRARIES libssh2::libssh2)
elseif(TARGET Libssh2::libssh2)
    list(APPEND LIBRARIES Libssh2::libssh2)
else()
    message(FATAL_ERROR "Could not find a valid libssh2 target")
endif()


set(VistaCoreFiles
    "Vista/Core/Application/VApplication.cpp" "Vista/Core/Application/VApplication.hpp"
    "Vista/Core/VObject.cpp" "Vista/Core/VObject.hpp"
    "Vista/Core/VThread.cpp" "Vista/Core/VThread.hpp"
    "Vista/Core/Filesystem/VFile.cpp" "Vista/Core/Filesystem/VFile.hpp"
    "Vista/Core/DataTypes/VString.cpp" "Vista/Core/DataTypes/VString.hpp"
    "Vista/Core/DataTypes/VArray.tpp" "Vista/Core/DataTypes/VArray.hpp"
    "Vista/Core/DataTypes/VAny.cpp" "Vista/Core/DataTypes/VAny.hpp"
    "Vista/Core/DataTypes/VInt.tpp" "Vista/Core/DataTypes/VInt.hpp"
)

set(VistaGraphicsFiles
    "Vista/Graphics/VistaGUI.cpp" "Vista/Graphics/VistaGUI.hpp"
)

set(VistaSerialsFiles
    "Vista/Serials/VSerialBus.cpp" "Vista/Serials/VSerialBus.hpp"
    "Vista/Serials/VSerialPort.cpp" "Vista/Serials/VSerialPort.hpp"
)

add_library(VistaFramework
    STATIC
    ${VistaCoreFiles}
    ${VistaGraphicsFiles}
    ${VistaSerialsFiles}
)
target_link_libraries(VistaFramework
    PUBLIC
    ${LIBRARIES}
)

# Generate compile_commands.json for IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(VistaTest
    ${VistaCoreFiles}
    ${VistaGraphicsFiles}
    ${VistaSerialsFiles}
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
    ${VistaCoreFiles}
    ${VistaGraphicsFiles}
    ${VistaSerialsFiles}
    "Vista/.tests/GUITest.cpp"
)
target_link_libraries(VistaGUI
    PUBLIC
    VistaFramework
)

# Add install rules for cross-platform deployment
install(TARGETS VistaFramework
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(TARGETS VistaTest VistaGUI
    RUNTIME DESTINATION bin
)