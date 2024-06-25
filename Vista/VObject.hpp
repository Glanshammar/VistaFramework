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


#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

using std::string;
using std::vector;
using std::cout;

class VObject {
public:
    string objectName;

    void setName(const string& name);
};