#include <iostream>
#include <VString>
#include "VAnyTest.hpp"
#include <VAny>

VAnyTest::VAnyTest() {
    std::cout << "Running VAny tests...\n";
    
    testBasicTypes();
    testComplexTypes();
    testReassignment();
    testCastOr();
    
    std::cout << "All VAny tests passed!\n";
}

void VAnyTest::testBasicTypes() {
    // Test integers
    VAny intValue = 42;
    std::cout << "Integer value: " << intValue.cast<int>() << "\n";
    assert(intValue.isType<int>());
    assert(intValue.cast<int>() == 42);
    assert(!intValue.isEmpty());
    
    // Test doubles
    VAny doubleValue = 3.14;
    std::cout << "Double value: " << doubleValue.cast<double>() << "\n";
    assert(doubleValue.isType<double>());
    assert(doubleValue.cast<double>() == 3.14);
    
    // Test strings
    VAny stringValue = VString("Hello, VAny!");
    std::cout << "String value: " << stringValue.cast<VString>() << "\n";
    assert(stringValue.isType<VString>());
    assert(stringValue.cast<VString>() == "Hello, VAny!");
    
    // Test boolean
    VAny boolValue = true;
    std::cout << "Boolean value: " << (boolValue.cast<bool>() ? "true" : "false") << "\n";
    assert(boolValue.isType<bool>());
    assert(boolValue.cast<bool>() == true);
}

void VAnyTest::testComplexTypes() {
    struct TestStruct {
        int x;
        double y;
        VString z;
        
        bool operator==(const TestStruct& other) const {
            return x == other.x && y == other.y && z == other.z;
        }
    };
    
    TestStruct ts{1, 2.5, "test"};
    VAny complexValue = ts;
    
    std::cout << "Complex type test\n";
    assert(complexValue.isType<TestStruct>());
    
    TestStruct retrieved = complexValue.cast<TestStruct>();
    assert(retrieved == ts);
    std::cout << "Retrieved struct: {" << retrieved.x << ", " 
              << retrieved.y << ", " << retrieved.z << "}\n";
}

void VAnyTest::testReassignment() {
    VAny value = 10;
    assert(value.isType<int>());
    assert(value.cast<int>() == 10);
    VPrint("Reassignment Value 1 (int): ", value);
    
    value = VString("New value");
    assert(!value.isType<int>());
    assert(value.isType<VString>());
    assert(value.cast<VString>() == "New value");
    VPrint("Reassignment Value 2 (VString): ", value);
    
    // Test reset
    value.reset();
    assert(value.isEmpty());
    VPrint("Reassignment Value 3 (reset): ", value.isEmpty() ? "empty" : "not empty");
    
    // Test copy assignment
    VAny original = 100;
    VAny copy = original;
    assert(copy.isType<int>());
    assert(copy.cast<int>() == 100);
    VPrint("Reassignment Value 4 (copy): ", copy);

    // Verify that copy is independent
    original = 200;
    assert(copy.cast<int>() == 100);
    assert(original.cast<int>() == 200);
    VPrint("Reassignment Value 5 (original): ", original);
    VPrint("Reassignment Value 6 (copy): ", copy);
}

void VAnyTest::testCastOr() {
    VAny value = 42;
    
    // Successful cast
    int result = value.castOr<int>(0);
    assert(result == 42);
    
    // Failed cast with default
    VString strResult = value.castOr<VString>("default");
    assert(strResult == "default");
    
    // Empty value with default
    VAny emptyValue;
    double defaultDouble = emptyValue.castOr<double>(3.14);
    assert(defaultDouble == 3.14);
} 