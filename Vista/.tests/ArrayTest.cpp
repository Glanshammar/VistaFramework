#include "ArrayTest.hpp"


void ArrayTest()
{
    VArray list(typeid(int));
    list.append<int>(10);
    list.append<int>(20);
    list.append<int>(30);

    VPrint("List size: ", list.size());

    for (auto it = list.begin(); it != list.end(); ++it) {
        VPrint(*it, " ");
    }
    VPrint();

    list.removeItem(1);

    for (auto it = list.begin(); it != list.end(); ++it) {
        VPrint(*it, " ");
    }
    VPrint();

    list.replace<int>(0, 100);
    VPrint("After replacing first element with 100: ");
    for (auto it = list.begin(); it != list.end(); ++it) {
        VPrint(*it, " ");
    }
    VPrint();

    VPrint("List contains 100: ", list.contains<int>(100) ? "Yes" : "No");
    VPrint("List contains 50: ", list.contains<int>(50) ? "Yes" : "No");

    list.clear();
    VPrint("List size after clear: ", list.size());
    VPrint("List is empty: ", list.empty() ? "Yes" : "No");

    list.append<int>(50);
    list.append<int>(60);
    list.append<int>(70);
    VPrint("List after appending 50, 60, 70: ");
    for (auto it = list.begin(); it != list.end(); ++it) {
        VPrint(*it, " ");
    }
    VPrint();

    list.removeFirst();
    VPrint("List after removing first element: ");
    for (auto it = list.begin(); it != list.end(); ++it) {
        VPrint(*it, " ");
    }
    VPrint();

    list.removeLast();
    VPrint("List after removing last element: ");
    for (auto it = list.begin(); it != list.end(); ++it) {
        VPrint(*it, " ");
    }
    VPrint();
}
