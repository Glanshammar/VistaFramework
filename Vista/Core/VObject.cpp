#include "VObject.hpp"

int32_t VObject::objectCount = 0;

VObject::VObject() {
    objectCount++;
    objectName = "Object" + std::to_string(objectCount);
}

VObject::~VObject() {
    objectCount--;
    if (parent) {
        disconnect(parent);
    }
    for (auto child : children) {
        child->parent = nullptr;
    }
}

void VObject::setName(const std::string& name) {
    objectName = name;
}

std::string VObject::getName() const {
    return objectName;
}

void VObject::setParent(VObject *parent) {
    if (!parent) {
        return;
    }

    if (this->parent != parent) {
        this->parent = parent;
        parent->addChild(this);
    }
}

VObject *VObject::getParent() const {
    return parent;
}

void VObject::disconnect(VObject *object) {
    if (!object) {
        return;
    }

    const auto& siblings = object->getChildren();
    if (std::ranges::find(siblings, this) != siblings.end()) {
        std::erase(object->children, this);
        std::cout << "Child object disconnected." << object << std::endl;
    } else {
        std::cout << "Child object is not connected " << std::endl;
    }

    if (this->parent == object) {
        this->parent = nullptr;
        std::cout << "Parent object disconnected." << object << std::endl;
    } else {
        std::cout << "Parent object is not connected " << std::endl;
    }
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