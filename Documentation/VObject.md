# VObject Class Documentation

## Overview
VObject is a base class that provides fundamental object management functionalities for objects within the Vista framework.
It includes features such as naming, parent-child relationships, signal-slot connections, and automatic destruction notifications.

## Class Members
### Constructors & Destructor
VObject()

Description: Default constructor that initializes the object. It increments a static object count and assigns a default name based on this count.
Usage: VObject obj;
~VObject()

Description: Destructor that emits the destroyed signal before decrementing the object count. It also handles disconnection from parent and clears child pointers.
Usage: Automatically called when the object goes out of scope or is deleted.

___
### Methods

#### Setting object names
```C++
void VObject::setName(const std::string& name)
```

**Description:** Sets the name of the object. Exceptions during the operation are caught and logged. Default name of an object is "Object[X]" where X is the number of objects.
**Parameters:**
name: The name to assign to the object.
**Usage:**
```C++
VObject obj;
obj.setName("MyObject");
```

#

#### Retrieving object names
```C++
void VObject::getName();
```
**Description:** Retrieves the name of the object. Any exception encountered is caught, and "Error" is returned in such cases.
**Usage:**
```C++
std::string name = obj.getName();
```

#

#### Object Parents
```C++
void VObject::setParent(VObject parent)
```

Sets the parent of the current object. If the parent is valid and different from the current one, it is assigned, and the current object is added as a child of the new parent.

**Usage:**
```C++
VObject parentObj;
VObject childObj;
childObj.setParent(&parentObj);
```

#

```C++
void VObject::getParent() const
```

```C++
void VObject::hasParent() const
```

The getParent() function returns a pointer to the parent VObject, or nullptr if there is no parent.


**Usage:**
```C++
VObject *parent = obj.getParent();
```

```C++
if(obj.hasParent()){
    std::cout << obj.getParent()->getName() << std::endl;
}
```

#

#### Disconnecting objects
```C++
void VObject::disconnect(VObject object)
```
Disconnects the current object from a specified parent or child object. It removes the current object from the parent's children list and sets its parent pointer to nullptr.

**Usage:**
```C++
obj1.disconnect(&obj2);
```

#
```C++
void addChild(VObject child)
```

Adds a child object to the current object's children list.
**Usage:**

```C++
obj.addChild(childObj);
```

#

#### Retrieving child objects from a parent

```C++
std::vector<VObject> getChildren() const*
```

Retrieves the list of child objects.

**Usage:**

```C++
std::vector<VObject*> children = obj.getChildren();
```

```C++
std::cout << app.getChildren()[0] << std::endl;
```

```C++
obj.printChildren();
```

#

#### Signals & Slots

The VObject class provides a mechanism for objects to communicate with each other through signals and slots. This allows objects to notify others of certain events without needing to know the details of the receiving object. It is a central part of the Vista framework's event-driven architecture.

```C++
void VObject::onDestroyed(DestroyedCallback callback)
void VObject::onDestroyed(void (*func)(VObject*))
void VObject::onDestroyed(std::function<void()> callback)
void VObject::onDestroyed(void (*func)())
```

1. Lambda or std::function<void(VObject*)>
    * Connects a lambda or std::function that takes a VObject* as an argument.
```C++
obj.onDestroyed([](VObject* object) {
    std::cout << "Object destroyed: " << object->getName() << std::endl;
});
```

2. Free Function with VObject* Parameter
   * Connects a free function that takes a VObject* as an argument.

```C++
void onObjectDestroy(VObject* obj) {
    std::cout << "Object destroyed: " << obj->getName() << std::endl;
}

obj.onDestroyed(onObjectDestroy);
```
