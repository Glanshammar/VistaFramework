#include <iostream>
#include "PlatformTest.hpp"

void PlatformTest()
{
    #if defined(__ANDROID__)
    #if defined(__aarch64__) || defined(__x86_64__)
        std::cout << "Compiled for Android 64-bit" << std::endl;
    #else
        std::cout << "Compiled for Android 32-bit" << std::endl;
    #endif
    #endif

    #if defined(__linux__)
    #if defined(__x86_64__) || defined(__ppc64__) || defined(__PPC64__) || defined(__powerpc64__)
        std::cout << "Compiled for Linux 64-bit" << std::endl;
    #else
        std::cout << "Compiled for Linux 32-bit" << std::endl;
    #endif
    #endif

    #if defined(__APPLE__) && defined(__MACH__)
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
        std::cout << "Compiled for iOS" << std::endl;
    #elif TARGET_OS_MAC
    #ifdef __x86_64__
        std::cout << "Compiled for MacOS 64-bit" << std::endl;
    #else
        std::cout << "Compiled for MacOS 32-bit" << std::endl;
    #endif
    #endif
    #endif

    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #ifdef _WIN64
        std::cout << "Compiled for Windows 64-bit" << std::endl;
    #else
        std::cout << "Compiled for Windows 32-bit" << std::endl;
    #endif
    #endif
}