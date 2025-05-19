#pragma once

#include "VEvent.hpp"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <chrono>

struct PendingEvent {
    VObject* receiver;
    std::unique_ptr<VEvent> event;
    std::chrono::system_clock::time_point timestamp;
};

class VEventLoop {
public:
    VEventLoop();
    ~VEventLoop();

    void postEvent(VObject* receiver, std::unique_ptr<VEvent> event);
    void sendEvent(VObject* receiver, VEvent* event);
    void processEvents();
    bool hasPendingEvents() const;
    void quit();
    bool isRunning() const;

private:
    std::queue<PendingEvent> eventQueue;
    mutable std::mutex queueMutex;
    std::condition_variable queueCondition;
    std::atomic<bool> running;
    std::thread eventThread;

    void processEvent(const PendingEvent& event);
    void eventLoop();
}; 