#include "VThread.hpp"

// ************************************************************************************************
// VThread implementation
// ************************************************************************************************
std::atomic<int32_t> VThread::threadCount = 0;

VThread::VThread(const std::function<void()>& functionName, const std::string& name)
    : task(functionName), state(ThreadState::Created), shouldPause(false), threadName(name),
      completionFuture(completionPromise.get_future()) {
    if (!functionName) {
        throw ThreadException("Function cannot be null");
    }
    ++VThread::threadCount;
}

VThread::~VThread() {
    if (state == ThreadState::Running || state == ThreadState::Paused) {
        stop();
    }
    if (thread.joinable()) {
        thread.join();
    }
    --VThread::threadCount;
}

VThread::VThread(VThread&& other) noexcept
    : thread(std::move(other.thread)),
      threadID(other.threadID),
      task(std::move(other.task)),
      state(other.state.load()),
      threadName(std::move(other.threadName)),
      threadException(other.threadException),
      completionPromise(std::move(other.completionPromise)),
      completionFuture(std::move(other.completionFuture)) {
    other.state = ThreadState::Stopped;
}

VThread& VThread::operator=(VThread&& other) noexcept {
    if (this != &other) {
        if (state == ThreadState::Running || state == ThreadState::Paused) {
            stop();
        }
        if (thread.joinable()) {
            thread.join();
        }
        thread = std::move(other.thread);
        threadID = other.threadID;
        task = std::move(other.task);
        state = other.state.load();
        threadName = std::move(other.threadName);
        threadException = other.threadException;
        completionPromise = std::move(other.completionPromise);
        completionFuture = std::move(other.completionFuture);
        other.state = ThreadState::Stopped;
    }
    return *this;
}

void VThread::setState(ThreadState newState) {
    std::lock_guard<std::mutex> lock(stateMutex);
    state = newState;
}

void VThread::checkThreadState() const {
    if (state == ThreadState::Error) {
        rethrowIfException();
    }
}

void VThread::handleThreadException() {
    try {
        if (threadException) {
            std::rethrow_exception(threadException);
        }
    } catch (const std::exception& e) {
        setState(ThreadState::Error);
        completionPromise.set_exception(std::current_exception());
    }
}

void VThread::start() {
    std::lock_guard<std::mutex> lock(stateMutex);
    if (state != ThreadState::Created && state != ThreadState::Stopped) {
        throw ThreadException("Thread cannot be started in current state");
    }
    
    thread = std::thread([this]() {
        try {
            setState(ThreadState::Running);
            threadID = std::this_thread::get_id();
            
            while (state == ThreadState::Running) {
                {
                    std::unique_lock<std::mutex> lock(stateMutex);
                    if (shouldPause) {
                        setState(ThreadState::Paused);
                        pauseCondition.wait(lock, [this]() { 
                            return !shouldPause || state != ThreadState::Running; 
                        });
                        if (state != ThreadState::Running) break;
                        setState(ThreadState::Running);
                    }
                }
                
                if (state == ThreadState::Running) {
                    task();
                    {
                        std::lock_guard<std::mutex> lock(stateMutex);
                        if (state == ThreadState::Running) {
                            break;  // Only break if we're still running after task completion
                        }
                    }
                }
            }
            
            setState(ThreadState::Completed);
            completionPromise.set_value();
        } catch (...) {
            threadException = std::current_exception();
            setState(ThreadState::Error);
            completionPromise.set_exception(threadException);
        }
    });
}

void VThread::join() {
    if (thread.joinable()) {
        thread.join();
        checkThreadState();
    }
}

void VThread::detach() {
    if (thread.joinable()) {
        thread.detach();
    }
}

void VThread::pause() {
    std::lock_guard<std::mutex> lock(stateMutex);
    if (state == ThreadState::Running) {
        shouldPause = true;
        setState(ThreadState::Paused);
        pauseCondition.notify_one();
    }
}

void VThread::resume() {
    std::lock_guard<std::mutex> lock(stateMutex);
    if (state == ThreadState::Paused) {
        shouldPause = false;
        setState(ThreadState::Running);
        pauseCondition.notify_one();
    }
}

void VThread::stop() {
    std::lock_guard<std::mutex> lock(stateMutex);
    if (state == ThreadState::Running || state == ThreadState::Paused) {
        shouldPause = false;
        setState(ThreadState::Stopped);
        pauseCondition.notify_one();
    }
}

std::thread::id VThread::getID() const {
    return threadID;
}

ThreadState VThread::getState() const {
    std::lock_guard<std::mutex> lock(stateMutex);
    return state;
}

std::string VThread::getName() const {
    return threadName;
}

bool VThread::isRunning() const {
    return state == ThreadState::Running;
}

bool VThread::isPaused() const {
    return state == ThreadState::Paused;
}

bool VThread::hasError() const {
    return state == ThreadState::Error;
}

void VThread::Sleep(float seconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(seconds * 1000)));
}

int32_t VThread::getThreadCount() {
    return threadCount;
}

std::thread::id VThread::getCurrentThreadId() {
    return std::this_thread::get_id();
}

void VThread::rethrowIfException() const {
    if (threadException) {
        std::rethrow_exception(threadException);
    }
}

std::future<void> VThread::getCompletionFuture() {
    return std::move(completionFuture);
}

// ************************************************************************************************
// VThreadGroup implementation
// ************************************************************************************************
void VThreadGroup::addThread(VThread&& thread) {
    std::lock_guard<std::mutex> lock(groupMutex);
    threads.push_back(std::move(thread));
}

void VThreadGroup::startAll() {
    std::lock_guard<std::mutex> lock(groupMutex);
    for (auto& thread : threads) {
        thread.start();
    }
}

void VThreadGroup::joinAll() {
    std::lock_guard<std::mutex> lock(groupMutex);
    for (auto& thread : threads) {
        thread.join();
    }
}

void VThreadGroup::pauseAll() {
    std::lock_guard<std::mutex> lock(groupMutex);
    for (auto& thread : threads) {
        thread.pause();
    }
}

void VThreadGroup::resumeAll() {
    std::lock_guard<std::mutex> lock(groupMutex);
    for (auto& thread : threads) {
        thread.resume();
    }
}

void VThreadGroup::stopAll() {
    std::lock_guard<std::mutex> lock(groupMutex);
    for (auto& thread : threads) {
        thread.stop();
    }
}

size_t VThreadGroup::size() const {
    std::lock_guard<std::mutex> lock(groupMutex);
    return threads.size();
}

bool VThreadGroup::hasErrors() const {
    std::lock_guard<std::mutex> lock(groupMutex);
    return std::any_of(threads.begin(), threads.end(), [](const VThread& t) { return t.hasError(); });
}

void VThreadGroup::rethrowFirstException() {
    std::lock_guard<std::mutex> lock(groupMutex);
    for (auto& thread : threads) {
        thread.rethrowIfException();
    }
}

void VThreadGroup::waitForAll() {
    std::lock_guard<std::mutex> lock(groupMutex);
    for (auto& thread : threads) {
        thread.getCompletionFuture().wait();
    }
}

void VThreadGroup::waitForAny() {
    std::lock_guard<std::mutex> lock(groupMutex);
    std::vector<std::future<void>> futures;
    futures.reserve(threads.size());
    for (auto& thread : threads) {
        futures.push_back(thread.getCompletionFuture());
    }
    
    std::future_status status;
    do {
        for (auto& future : futures) {
            status = future.wait_for(std::chrono::milliseconds(100));
            if (status == std::future_status::ready) {
                return;
            }
        }
    } while (status == std::future_status::timeout);
}