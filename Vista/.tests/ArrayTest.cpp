#include "ArrayTest.hpp"

ArrayTest::ArrayTest() {
    VPrint("Running VArray tests...");
    
    testConstructors();
    testElementAccess();
    testModifiers();
    testCapacity();
    testEnhancedFunctionality();
    testFunctionalOperations();
    testStringOperations();
    
    VPrint("All VArray tests passed!");
}

// Global function to run the tests
void RunArrayTest() 
{
    ArrayTest test;
}

void ArrayTest::testConstructors() {
    VPrint("--------------------------------");
    VPrint("Testing constructors...");
    
    // Default constructor
    VArray<int> empty;
    assert(empty.isEmpty());
    VPrint("Empty array created: ", empty.isEmpty() ? "true" : "false");
    
    // Count constructor
    VArray<int> repeated(5, 10);
    assert(repeated.size() == 5 && repeated[0] == 10 && repeated[4] == 10);
    for(Range(repeated.size()))
    {
        VPrint("Repeated array: ", repeated[i]);
    }

    // Initializer list constructor
    VArray<int> init = {1, 2, 3, 4, 5};
    assert(init.size() == 5 && init[0] == 1 && init[4] == 5);
    for(Range(init.size()))
    {
        VPrint("Initializer list array: ", init[i]);
    }

    // Copy constructor
    VArray<int> copy(init);
    assert(copy.size() == init.size() && copy[0] == init[0]);
    for(Range(copy.size()))
    {
        VPrint("Copy array: ", copy[i]);
    }
    
    // Move constructor
    VArray<int> toMove = {10, 20, 30};
    VArray<int> moved(std::move(toMove));
    assert(moved.size() == 3 && moved[0] == 10 && toMove.isEmpty());
    for(Range(moved.size()))
    {
        VPrint("Moved array: ", moved[i]);
    }

    // Range constructor
    std::vector<double> stdVec = {1.1, 2.2, 3.3};
    VArray<double> rangeConstructed(stdVec.begin(), stdVec.end());
    assert(rangeConstructed.size() == 3 && rangeConstructed[0] == 1.1);
    for(Range(rangeConstructed.size()))
    {
        VPrint("Range constructed array: ", rangeConstructed[i]);
    }
}

void ArrayTest::testElementAccess() {
    VPrint("--------------------------------");
    VPrint("Testing element access...");
    
    VArray<int> arr = {10, 20, 30, 40, 50};
    
    // Test operator[] and at()
    assert(arr[0] == 10 && arr[4] == 50);
    assert(arr.at(1) == 20 && arr.at(3) == 40);
    
    // Test front() and back()
    assert(arr.front() == 10 && arr.back() == 50);
    
    // Test data()
    const int* data = arr.data();
    assert(data[0] == 10 && data[4] == 50);
    
    // Test bounds checking with at()
    bool exceptionThrown = false;
    try {
        arr.at(10); // Out of bounds
    } catch (const std::out_of_range&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);
}

void ArrayTest::testModifiers() {
    VPrint("--------------------------------");
    VPrint("Testing modifiers...");
    
    // Test push_back and pop_back
    VArray<int> arr;
    arr.push_back(10);
    arr.push_back(20);
    assert(arr.size() == 2 && arr[0] == 10 && arr[1] == 20);
    
    arr.pop_back();
    assert(arr.size() == 1 && arr[0] == 10);
    
    // Test clear
    arr.clear();
    assert(arr.isEmpty());
    
    // Test insert
    arr = {10, 30, 40};
    arr.insert(arr.begin() + 1, 20);
    assert(arr.size() == 4 && arr[1] == 20);
    
    // Test emplace_back
    struct Point { 
        int x, y; 
        bool operator==(const Point& other) const { return x == other.x && y == other.y; }
    };
    VArray<Point> points;
    points.emplace_back(Point{1, 2});
    assert(points.size() == 1 && points[0].x == 1 && points[0].y == 2);
    
    // Test erase
    arr = {10, 20, 30, 40, 50};
    arr.erase(arr.begin() + 2);
    assert(arr.size() == 4 && arr[2] == 40);
    
    // Test range erase
    arr = {10, 20, 30, 40, 50};
    arr.erase(arr.begin() + 1, arr.begin() + 4);
    assert(arr.size() == 2 && arr[0] == 10 && arr[1] == 50);
}

void ArrayTest::testCapacity() {
    VPrint("--------------------------------");
    VPrint("Testing capacity functions...");
    
    VArray<int> arr;
    assert(arr.isEmpty() && arr.size() == 0);
    
    // Test reserve and capacity
    arr.reserve(100);
    assert(arr.capacity() >= 100 && arr.isEmpty());
    
    // Test resize
    arr.resize(5);
    assert(arr.size() == 5);
    
    // Test resize with value
    arr.resize(10, 42);
    assert(arr.size() == 10 && arr[5] == 42 && arr[9] == 42);
    
    // Test shrink_to_fit
    arr.clear();
    arr.reserve(1000);
    auto oldCapacity = arr.capacity();
    arr.push_back(1);
    arr.shrink_to_fit();
    assert(arr.capacity() <= oldCapacity);
}

void ArrayTest::testEnhancedFunctionality() {
    VPrint("--------------------------------");
    VPrint("Testing enhanced functionality...");
    
    VArray<int> arr = {1, 2, 3, 4, 5, 3, 2, 1};
    
    // Test find and findIf
    auto it = arr.find(3);
    assert(it != arr.end() && *it == 3);
    
    auto evenIt = arr.findIf([](int n) { return n % 2 == 0; });
    assert(evenIt != arr.end() && *evenIt == 2);
    
    // Test contains
    assert(arr.contains(4) && !arr.contains(10));
    
    // Test count and countIf
    assert(arr.count(1) == 2 && arr.count(6) == 0);
    assert(arr.countIf([](int n) { return n % 2 == 0; }) == 3);
    
    // Test remove
    VArray<int> toRemove = {1, 2, 3, 1, 2, 3};
    auto removed = toRemove.remove(2);
    assert(removed == 2 && toRemove.size() == 4 && !toRemove.contains(2));
    
    // Test removeIf
    VArray<int> toRemoveIf = {1, 2, 3, 4, 5, 6};
    auto removedIf = toRemoveIf.removeIf([](int n) { return n % 2 == 0; });
    assert(removedIf == 3 && toRemoveIf.size() == 3);
    assert(toRemoveIf[0] == 1 && toRemoveIf[1] == 3 && toRemoveIf[2] == 5);
    
    // Test slice
    VArray<int> original = {10, 20, 30, 40, 50};
    auto sliced = original.slice(1, 4);
    assert(sliced.size() == 3 && sliced[0] == 20 && sliced[2] == 40);
    
    // Test concat
    VArray<int> first = {1, 2, 3};
    VArray<int> second = {4, 5, 6};
    auto combined = first.concat(second);
    assert(combined.size() == 6 && combined[0] == 1 && combined[5] == 6);
    
    // Test sort
    VArray<int> toSort = {5, 3, 8, 1, 9, 4};
    toSort.sort();
    assert(toSort[0] == 1 && toSort[5] == 9);
    
    // Test custom sort
    toSort = {5, 3, 8, 1, 9, 4};
    toSort.sort([](int a, int b) { return a > b; });
    assert(toSort[0] == 9 && toSort[5] == 1);
    
    // Test reverse
    VArray<int> toReverse = {1, 2, 3, 4, 5};
    toReverse.reverse();
    assert(toReverse[0] == 5 && toReverse[4] == 1);
    
    // Test firstOrNone
    auto firstEven = arr.firstOrNone([](int n) { return n % 2 == 0; });
    assert(firstEven.has_value() && *firstEven == 2);
    
    auto nonExistent = arr.firstOrNone([](int n) { return n > 100; });
    assert(!nonExistent.has_value());
}

void ArrayTest::testFunctionalOperations() {
    VPrint("--------------------------------");
    VPrint("Testing functional operations...");
    
    VArray<int> arr = {1, 2, 3, 4, 5};
    
    // Test transform
    arr.transform([](int n) { return n * 2; });
    assert(arr[0] == 2 && arr[4] == 10);
    
    // Test map
    auto mapped = arr.map([](int n) { return n + 1; });
    assert(mapped[0] == 3 && mapped[4] == 11);
    
    // Test filter
    auto filtered = arr.filter([](int n) { return n > 5; });
    assert(filtered.size() == 3);
    assert(filtered[0] == 6 && filtered[1] == 8 && filtered[2] == 10);
    
    // Test reduce
    int sum = arr.reduce(0, [](int acc, int val) { return acc + val; });
    assert(sum == 30); // 2 + 4 + 6 + 8 + 10
    
    // Test map with different return type
    auto strings = arr.map([](int n) { return std::to_string(n); });
    assert(strings[0] == "2" && strings[4] == "10");
}

void ArrayTest::testStringOperations() {
    VPrint("--------------------------------");
    VPrint("Testing string operations...");
    
    // Test join with strings
    VArray<VString> words;
    words.push_back("Hello");
    words.push_back("Vista");
    words.push_back("Framework");
    
    // Join with space separator
    VString spaceSeparator(" ");
    VString joined = words.join(spaceSeparator);
    VPrint("Space Separated: ", joined);
    assert(joined == "Hello Vista Framework");
    
    // Join with empty separator
    VString emptySeparator;
    joined = words.join(emptySeparator);
    VPrint("Empty Separated: ", joined);
    assert(joined == "HelloVistaFramework");
    
    // Test join with numbers
    VArray<int> numbers = {1, 2, 3, 4};
    VString commaSeparator(", ");
    VString joinedNumbers = numbers.join(commaSeparator);
    VPrint("Comma Separated: ", joinedNumbers);
    assert(joinedNumbers == "1, 2, 3, 4");
} 