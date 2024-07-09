#include "VObject.hpp"

std::unique_ptr<VObject> CreateVObject() {
    auto ptr = std::make_unique<VObject>();

    return ptr;
}

void VObject::SetName(const string& name) {
    objectName = name;
}

bool VObject::Connect(const VObject& object)
{
    return false;
}

bool VObject::Disconnect(const VObject& object)
{
    return false;
}
