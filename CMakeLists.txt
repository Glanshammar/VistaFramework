cmake_minimum_required(VERSION 3.28)
project(VistaFramework)
set(CMAKE_CXX_STANDARD 23)
set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
if(WIN32)
    set(USER_HOME $ENV{USERPROFILE})
elseif(UNIX)
    set(USER_HOME $ENV{HOME})
endif()
set(CMAKE_TOOLCHAIN_FILE "${USER_HOME}/.vcpkg-clion/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

set(LIBRARIES
        Vulkan::Vulkan
        glfw
)

add_library(VistaFramework STATIC
        "Vista/Vulkan/vulkangui.cpp"
        "Vista/GUI/VConsole.cpp" "Vista/GUI/VConsole.hpp"
        "Vista/VObject.cpp" "Vista/VObject.hpp"
        "Vista/Serials/VSerialBus.cpp" "Vista/Serials/VSerialBus.hpp"
        "Vista/Serials/VSerialPort.cpp" "Vista/Serials/VSerialPort.hpp"
        Vista/Core/VThread.cpp
        Vista/Core/VThread.hpp
)
target_link_libraries(VistaFramework PUBLIC ${LIBRARIES})

add_executable(VistaLoader
        "Vista/libloader.cpp"
)
target_link_libraries(VistaLoader PUBLIC VistaFramework ${LIBRARIES})

add_executable(VistaTest
        "Vista/.tests/consoletest.cpp"
)
target_link_libraries(VistaTest PUBLIC VistaFramework ${LIBRARIES})