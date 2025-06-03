#pragma once

#include <cassert>
#include <VArray>
#include <VString>
#include <VConsole>
#include <VGlobals>
#include <iostream>

// Forward declaration for the global test function
void RunArrayTest();

class ArrayTest {
public:
    ArrayTest();
    
private:
    // Test methods for different aspects of VArray functionality
    void testConstructors();
    void testElementAccess();
    void testModifiers();
    void testCapacity();
    void testEnhancedFunctionality();
    void testFunctionalOperations();
    void testStringOperations();
}; 