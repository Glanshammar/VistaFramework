#include "ArrayTest.hpp"
#include <VString>

void ArrayTest() {
    VPrint("\n=== Starting Array Test ===");
    
    VPrint("\nCreating array...");
    VArray<int> arr;
    VPrint("Array created with size: ", arr.size());

    VPrint("\nAdding elements...");
    for (int i = 0; i < 5; i++) {
        arr.append(i);
        VPrint("Added element: ", i);
        VPrint("Current size: ", arr.size());
    }

    VPrint("\nAccessing elements...");
    for (int i = 0; i < arr.size(); i++) {
        VPrint("Element at index ", i, ": ", arr[i]);
    }

    VPrint("\nTesting iterator...");
    for (const auto& element : arr) {
        VPrint("Iterating element: ", element);
    }

    VPrint("\nTesting clear...");
    arr.clear();
    VPrint("After clear - Size: ", arr.size());

    VPrint("\nTesting append after clear...");
    for (int i = 0; i < 3; i++) {
        arr.append(42);
        VPrint("Added element: ", 42);
    }
    VPrint("New size: ", arr.size());

    VPrint("\nTesting contains...");
    VPrint("Contains 42: ", arr.contains(42) ? "Yes" : "No");
    VPrint("Contains 100: ", arr.contains(100) ? "Yes" : "No");

    VPrint("\nTesting remove operations...");
    VPrint("Size before removeFirst: ", arr.size());
    arr.removeFirst();
    VPrint("Size after removeFirst: ", arr.size());
    
    VPrint("Size before removeLast: ", arr.size());
    arr.removeLast();
    VPrint("Size after removeLast: ", arr.size());

    VPrint("\nTesting string array...");
    VArray<VString> strArr;
    strArr.append("Hello 👋");
    strArr.append("World 🌍");
    strArr.append("from");
    strArr.append("Vista");
    strArr.append("🚀");
    
    VPrint("\nString array contents:");
    for (int i = 0; i < strArr.size(); i++) {
        VPrint("String at index ", i, ": ", strArr[i]);
    }

    VPrint("\nClearing arrays...");
    strArr.clear();
    arr.clear();
    if (strArr.size() == 0 && arr.size() == 0) {
        VPrint("Arrays cleared");
    } else {
        VPrint("Arrays not cleared");
    }

    VPrint("\n=== Array Test Complete ===\n");
}
