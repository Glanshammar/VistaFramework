#include "ArrayTest.hpp"
#include "../Core/Application/VConsole.hpp"

void ArrayTest()
{
    VArray<int> list;
    list.append(10);
    list.append(20);
    list.append(30);

    VPrint("List size: ", list.size());

    for (int & it : list) {
        VPrint(it, " ");
    }
    std::cout << std::endl;

    list.removeItem(1);

    for (int & it : list) {
        std::cout << it << " ";
    }
    std::cout << std::endl;

    list.replace(0, 100);
    std::cout << "After replacing first element with 100: " << std::endl;
    for (int & it : list) {
        std::cout << it << " ";
    }
    std::cout << std::endl;

    VPrint("List contains 100: ", list.contains(100) ? "Yes" : "No");
    VPrint("List contains 30: ", list.contains(50) ? "Yes" : "No");

    list.clear();
    VPrint("List size after clear: ", list.size());
    VPrint("List is empty: ", list.empty() ? "Yes" : "No");

    list.append(50);
    list.append(60);
    list.append(70);
    std::cout << "List after appending 50, 60, 70: " << std::endl;
    for (int & it : list) {
        std::cout << it << " ";
    }
    std::cout << std::endl;

    list.removeFirst();
    std::cout << "List after removing first element: " << std::endl;
    for (int & it : list) {
        std::cout << it << " ";
    }
    std::cout << std::endl;

    list.removeLast();
    std::cout << "List after removing last element: " << std::endl;
    for (int & it : list) {
        std::cout << it << " ";
    }
    std::cout << std::endl;
}
