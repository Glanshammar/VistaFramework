#include "VEventLoop.hpp"
#include <algorithm>

VEventLoop::VEventLoop() : running(true) {
    eventThread = std::thread(&VEventLoop::eventLoop, this);
}

VEventLoop::~VEventLoop() {
    quit();
    if (eventThread.joinable()) {
        eventThread.join();
    }
}

void VEventLoop::postEvent(VObject* receiver, std::unique_ptr<VEvent> event) {
    if (!receiver || !event) return;

    PendingEvent pendingEvent{
        receiver,
        std::move(event),
        std::chrono::system_clock::now()
    };

    {
        std::lock_guard<std::mutex> lock(queueMutex);
        eventQueue.push(std::move(pendingEvent));
    }
    queueCondition.notify_one();
}

void VEventLoop::sendEvent(VObject* receiver, VEvent* event) {
    if (!receiver || !event) return;
    processEvent({receiver, std::unique_ptr<VEvent>(event), std::chrono::system_clock::now()});
}

void VEventLoop::processEvents() {
    std::queue<PendingEvent> eventsToProcess;
    
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        if (eventQueue.empty()) return;
        eventsToProcess = std::move(eventQueue);
    }
    
    while (!eventsToProcess.empty()) {
        processEvent(eventsToProcess.front());
        eventsToProcess.pop();
    }
}

bool VEventLoop::hasPendingEvents() const {
    std::lock_guard<std::mutex> lock(queueMutex);
    return !eventQueue.empty();
}

void VEventLoop::quit() {
    running = false;
    queueCondition.notify_all();
}

bool VEventLoop::isRunning() const {
    return running;
}

void VEventLoop::processEvent(const PendingEvent& event) {
    if (!event.receiver || !event.event) return;
    
    // Here you would typically call the event handler on the receiver
    // For now, we'll just check if the event is accepted
    if (!event.event->isAccepted()) {
        // Handle unaccepted events if needed
    }
}

void VEventLoop::eventLoop() {
    while (running) {
        std::unique_lock<std::mutex> lock(queueMutex);
        queueCondition.wait(lock, [this] { 
            return !running || !eventQueue.empty(); 
        });
        
        if (!running) break;
        
        // Process all pending events
        while (!eventQueue.empty()) {
            PendingEvent event = std::move(eventQueue.front());
            eventQueue.pop();
            lock.unlock();
            
            processEvent(event);
            
            lock.lock();
        }
    }
} 