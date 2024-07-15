#include "VObject.hpp"

std::unique_ptr<VObject> CreateVObject() {
    auto ptr = std::make_unique<VObject>();

    return ptr;
}

int VObject::objectCount = 0;

VObject::VObject() : objectConnection(nullptr)
{
    objectCount++;
    objectName = "VObject" + std::to_string(objectCount);
}

VObject::~VObject() {
    objectCount--;
}

void VObject::SetName(const VString& name) {
    objectName = name;
}

/*
bool VObject::Connect(VObject* object) {
    if (object != nullptr) {
        objectConnection = object;
        return true;
    }
    return false;
}

bool VObject::Disconnect() {
    if (objectConnection != nullptr) {
        if (objectConnection->objectConnection == this) {
            objectConnection->objectConnection = nullptr;
        }
        objectConnection = nullptr;
        return true;
    }
    return false;
}

void VObject::PrintConnection() const {
    if (objectConnection != nullptr) {
        std::cout << objectName << " is connected to " << objectConnection->objectName << std::endl;
    } else {
        std::cout << objectName << " is not connected to any object" << std::endl;
    }
}
*/
