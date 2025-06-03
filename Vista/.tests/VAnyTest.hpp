#pragma once
#include <cassert>
#include <VConsole>

class VAnyTest {
public:
    VAnyTest();

private:
    void testBasicTypes();
    void testComplexTypes();
    void testReassignment();
    void testCastOr();
}; 