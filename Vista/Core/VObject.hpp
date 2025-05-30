#pragma once

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <algorithm>
#include <ranges>
#include <functional>
#include <memory>
#include <fstream>
#include <sstream>
#include <VString>
#include <VConsole>
#include <VEvent>


#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif


template <typename... Args>
class Signal {
public:
    using SlotType = std::function<void(Args...)>;

    void connect(SlotType slot) {
        slots_.push_back(slot);
    }

    void emit(Args... args) {
        for (auto& slot : slots_) {
            slot(args...);
        }
    }

private:
    std::vector<SlotType> slots_;
};


class VObject {
private:
    static int32_t objectCount;
    std::string objectName;
    VObject *parent = nullptr;
    std::vector<VObject*> children;

    Signal<VObject*> destroyed;

public:
    VObject();
    ~VObject();

    void setName(const std::string& name);
    [[nodiscard]] std::string getName() const;
    void setParent(VObject *parent);
    void disconnect(VObject *object);
    [[nodiscard]] VObject *getParent() const;
    void addChild(VObject *child);
    [[nodiscard]] std::vector<VObject*> getChildren() const;
    void printChildren() const;

    void onDestroyed(const Signal<VObject*>::SlotType& slot) {
        destroyed.connect(slot);
    }

    // Event handling
    virtual bool event(VEvent* event) {
        return false; // Base implementation does nothing
    }

protected:
    void emitDestroyed() {
        destroyed.emit(this);
    }
};
