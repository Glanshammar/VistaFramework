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
#include <VString>
#include <VConsole>


#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif


class VObject {
private:
    static int32_t objectCount;
    VString objectName;
    VObject* objectConnection;

public:
    VObject();
    ~VObject();

    void SetName(const VString& name);
    bool Connect(VObject* object);
    bool Disconnect();
    void PrintConnection() const;
};
