#include "VObject.hpp"
#include <VGlobals>

int32_t VObject::objectCount = 0;

VObject::VObject() {
    objectCount++;
    objectName = "Object" + std::to_string(objectCount);
}

VObject::~VObject() {
    emitDestroyed();
    objectCount--;
    if (parent) {
        disconnect(parent);
    }
    for (auto child : children) {
        child->parent = nullptr;
    }
}

void VObject::setName(const std::string& name) {
    try
    {
        objectName = name;
    } catch (const std::exception& e)
    {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
}

std::string VObject::getName() const {
    try
    {
        return objectName;
    } catch (const std::exception& e)
    {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return "Error";
    }
}

void VObject::setParent(VObject *parent) {
    if (!parent) {
        return;
    }

    if (this == parent) {
        std::cout << "An object cannot be its own parent." << std::endl;
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
        std::cout << "Children disconnected from: " << object->getName() << std::endl;
        object->printChildren();
    } else {
        std::cout << "Child object is not connected " << std::endl;
    }

    if (this->parent == object) {
        std::cout << "Parent object disconnected: " << this->parent->getName() << " from " << this->getName() << std::endl;
        this->parent = nullptr;
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
    if(this->getChildren().empty())
    {
        std::cout << this->getName() << " has no child objects." << std::endl;
    } else
    {
        for(auto& child : children) {
            std::cout << "Child of " << this->getName() << ": " << child->getName() << std::endl;
        }
    }
}