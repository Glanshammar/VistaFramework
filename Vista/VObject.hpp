#pragma once

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <memory>
#include <fstream>
#include <sstream>

#define vObject constexpr std::unique_ptr<VObject>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

using std::string;
using std::vector;
using std::cout;

class VObject {
private:
    string objectName;

public:
    void SetName(const string& name);
    static bool Connect(const VObject& object);
    static bool Disconnect(const VObject& object);

};
