#pragma once

#include <string>
#include <chrono>
#include <memory>
#include <typeindex>

class VEvent {
public:
    enum class Type {
        None,
        User,
        Timer,
        KeyPress,
        KeyRelease,
        MousePress,
        MouseRelease,
        MouseMove,
        MouseWheel,
        WindowClose,
        WindowResize,
        WindowMove,
        WindowFocus,
        WindowBlur,
        Custom
    };

    VEvent(Type type = Type::None);
    virtual ~VEvent() = default;

    Type type() const { return eventType; }
    bool isAccepted() const { return accepted; }
    void accept() { accepted = true; }
    void ignore() { accepted = false; }

    // Event propagation control
    bool isPropagating() const { return propagating; }
    void stopPropagation() { propagating = false; }

    // Event timestamp
    std::chrono::system_clock::time_point timestamp() const { return eventTimestamp; }

    // Type identification for custom events
    virtual std::type_index getEventType() const { return std::type_index(typeid(VEvent)); }

private:
    Type eventType;
    bool accepted = false;
    bool propagating = true;
    std::chrono::system_clock::time_point eventTimestamp = std::chrono::system_clock::now();
};

// Event handler interface
class VEventHandler {
public:
    virtual ~VEventHandler() = default;
    virtual bool handleEvent(VEvent* event) = 0;
};

// Event dispatcher for type-safe event handling
template<typename EventType>
class VEventDispatcher {
public:
    using HandlerType = std::function<bool(EventType*)>;

    void addHandler(HandlerType handler) {
        handlers.push_back(std::move(handler));
    }

    bool dispatch(EventType* event) {
        for (const auto& handler : handlers) {
            if (handler(event)) {
                return true;
            }
        }
        return false;
    }

private:
    std::vector<HandlerType> handlers;
}; 