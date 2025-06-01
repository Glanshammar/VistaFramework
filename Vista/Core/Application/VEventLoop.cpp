#include "VEventLoop.hpp"
#include <algorithm>

VEventLoop::VEventLoop() 
    : running(false)
    , numThreads(1) {
    initializeThreads();
}

VEventLoop::~VEventLoop() {
    quit();
}

void VEventLoop::initializeThreads() {
    eventThreads.joinAll();
    for (size_t i = 0; i < numThreads; ++i) {
        VThread thread([this]() {
            eventLoop();
        }, "EventThread" + std::to_string(i));
        eventThreads.addThread(std::move(thread));
    }
}

void VEventLoop::setThreadCount(size_t count) {
    if (count == 0) count = 1; // Ensure at least one thread
    if (count != numThreads) {
        bool wasRunning = running;
        if (wasRunning) {
            pauseEventProcessing();
        }
        numThreads = count;
        initializeThreads();
        if (wasRunning) {
            resumeEventProcessing();
        }
    }
}

size_t VEventLoop::threadCount() const {
    return numThreads;
}

void VEventLoop::pauseEventProcessing() {
    running = false;
    queueCondition.notify_all();
}

void VEventLoop::resumeEventProcessing() {
    running = true;
    queueCondition.notify_all();
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
    processEvent(PendingEvent{receiver, std::unique_ptr<VEvent>(event), std::chrono::system_clock::now()});
}

void VEventLoop::processEvents() {
    std::unique_lock<std::mutex> lock(queueMutex);
    while (!eventQueue.empty()) {
        PendingEvent event = std::move(eventQueue.front());
        eventQueue.pop();
        lock.unlock();
        
        processEvent(event);
        
        lock.lock();
    }
}

void VEventLoop::processEvent(const PendingEvent& event) {
    if (event.receiver && event.event) {
        event.receiver->event(event.event.get());
    }
}

void VEventLoop::eventLoop() {
    while (true) {
        std::unique_lock<std::mutex> lock(queueMutex);
        queueCondition.wait(lock, [this] {
            return !running || !eventQueue.empty();
        });

        if (!running && eventQueue.empty()) {
            break;
        }

        if (!eventQueue.empty()) {
            PendingEvent event = std::move(eventQueue.front());
            eventQueue.pop();
            lock.unlock();
            
            processEvent(event);
        }
    }
}

bool VEventLoop::hasPendingEvents() const {
    std::lock_guard<std::mutex> lock(queueMutex);
    return !eventQueue.empty();
}

void VEventLoop::quit() {
    running = false;
    queueCondition.notify_all();
    eventThreads.joinAll();
}

bool VEventLoop::isRunning() const {
    return running;
} 