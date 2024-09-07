#include "VThread.hpp"

// ************************************************************************************************
// VThread implementation
// ************************************************************************************************
std::atomic<int32_t> VThread::threadCount = 0;

VThread::VThread(const std::function<void()>& functionName)
    : task(functionName), started(false), threadID(std::this_thread::get_id()) {
    if (!functionName) {
        throw std::invalid_argument("Function cannot be null");
    }
    ++VThread::threadCount;
}

VThread::~VThread() {
    if (thread.joinable()) {
        thread.join();
    }
    ++VThread::threadCount;
}

VThread::VThread(VThread&& other) noexcept
    : thread(std::move(other.thread)), threadID(other.threadID), task(std::move(other.task)), started(other.started) {
    other.started = false;
}

VThread& VThread::operator=(VThread&& other) noexcept {
    if (this != &other) {
        if (thread.joinable()) {
            thread.join();
        }
        thread = std::move(other.thread);
        threadID = other.threadID;
        task = std::move(other.task);
        started = other.started;
        other.started = false;
    }
    return *this;
}

void VThread::start() {
    if (started) {
        throw std::runtime_error("Thread already started");
    }
    if (task) {
        thread = std::thread(task);
        threadID = thread.get_id();
        started = true;
    }
}

void VThread::join() {
    if (thread.joinable()) {
        thread.join();
    }
}

std::thread::id VThread::getID() const {
    return threadID;
}

void VThread::Sleep(float seconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(seconds * 1000)));
}

// ************************************************************************************************
// VThreadGroup implementation
// ************************************************************************************************
void VThreadGroup::addThread(VThread&& thread) {
    threads.push_back(std::move(thread));
}

void VThreadGroup::startAll() {
    for (auto& thread : threads) {
        thread.start();
    }
}

void VThreadGroup::joinAll() {
    for (auto& thread : threads) {
        thread.join();
    }
}

size_t VThreadGroup::size() const {
    return threads.size();
}