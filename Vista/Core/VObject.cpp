#include "VObject.hpp"

int32_t VObject::objectCount = 0;

VObject::VObject() {
    objectCount++;
    objectName = "Object" + std::to_string(objectCount);
}

VObject::~VObject() {
    objectCount--;
}

void VObject::setName(const std::string& name) {
    objectName = name;
}

std::string VObject::getName() const {
    return objectName;
}

void VObject::setParent(VObject *parent) {
    this->parent = parent;
    parent->addChild(this);
}

VObject *VObject::getParent() const {
    return parent;
}

void VObject::addChild(VObject *child) {
    children.push_back(child);
}

std::vector<VObject*> VObject::getChildren() const {
    return children;
}

void VObject::printChildren() const {
    for(auto& child : children) {
        std::cout << child->getName() << std::endl;
    }
}