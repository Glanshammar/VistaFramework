#include "VObject.hpp"

int32_t VObject::objectCount = 0;

VObject::VObject() {
    objectCount++;
    objectName = "Object" + std::to_string(objectCount);
}

VObject::~VObject() {
    objectCount--;
}

void VObject::setName(const VString& name) {
    objectName = name;
}