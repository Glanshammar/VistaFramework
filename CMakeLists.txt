cmake_minimum_required(VERSION 3.28)
project(VistaFramework)
set(CMAKE_CXX_STANDARD 23)
set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
if(WIN32)
    set(USER_HOME $ENV{USERPROFILE})
elseif(UNIX)
    set(USER_HOME $ENV{HOME})
endif()

set(CMAKE_TOOLCHAIN_FILE "${USER_HOME}/.vcpkg-clion/VistaFW/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
link_directories("${USER_HOME}/.vcpkg-clion/VistaFW/installed/x64-windows/debug/lib")

include_directories("Vista/include/Vista/")

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
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

set(LIBRARIES
        Vulkan::Vulkan
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
        "Vista/Graphics/vulkangui.cpp"
        "Vista/GUI/VConsole.cpp" "Vista/GUI/VConsole.hpp"
        "Vista/Core/VObject.cpp" "Vista/Core/VObject.hpp"
        "Vista/Serials/VSerialBus.cpp" "Vista/Serials/VSerialBus.hpp"
        "Vista/Serials/VSerialPort.cpp" "Vista/Serials/VSerialPort.hpp"
        "Vista/Core/VThread.cpp" "Vista/Core/VThread.hpp"
        "Vista/Core/Filesystem/VFile.cpp" "Vista/Core/Filesystem/VFile.hpp"
        "Vista/Core/DataTypes/VString.cpp" "Vista/Core/DataTypes/VString.hpp"
        "Vista/Core/DataTypes/VArray.tpp" "Vista/Core/DataTypes/VArray.hpp"
        "Vista/Core/DataTypes/VAny.cpp" "Vista/Core/DataTypes/VAny.hpp"
)

add_library(VistaFramework STATIC ${VistaFiles})
target_link_libraries(VistaFramework
        PUBLIC
        ${LIBRARIES}
        $<IF:$<TARGET_EXISTS:Libssh2::libssh2_shared>,Libssh2::libssh2_shared,Libssh2::libssh2_static>
)

add_executable(VistaLoader
        "Vista/libloader.cpp"
)
target_link_libraries(VistaLoader
        PUBLIC
        VistaFramework
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