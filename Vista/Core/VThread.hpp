#pragma once

#include <VObject>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <future>
#include <string>
#include <stdexcept>

enum class ThreadState {
    Created,
    Running,
    Paused,
    Stopped,
    Completed,
    Error
};

class ThreadException : public std::runtime_error {
public:
    explicit ThreadException(const std::string& message) : std::runtime_error(message) {}
};

class VThread {
private:
    std::thread thread;
    std::thread::id threadID;
    static std::atomic<int32_t> threadCount;
    std::function<void()> task;
    std::atomic<ThreadState> state;
    mutable std::mutex stateMutex;
    std::condition_variable pauseCondition;
    bool shouldPause;
    std::string threadName;
    std::exception_ptr threadException;
    std::promise<void> completionPromise;
    std::future<void> completionFuture;

public:
    explicit VThread(const std::function<void()>& functionName, const std::string& name = "");
    ~VThread();

    VThread(const VThread&) = delete;
    VThread& operator=(const VThread&) = delete;

    VThread(VThread&& other) noexcept;
    VThread& operator=(VThread&& other) noexcept;

    // Thread control
    void start();
    void join();
    void detach();
    void pause();
    void resume();
    void stop();
    
    // Thread information
    [[nodiscard]] std::thread::id getID() const;
    [[nodiscard]] ThreadState getState() const;
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] bool isRunning() const;
    [[nodiscard]] bool isPaused() const;
    [[nodiscard]] bool hasError() const;
    
    // Thread utilities
    static void Sleep(float seconds);
    static int32_t getThreadCount();
    static std::thread::id getCurrentThreadId();
    
    // Error handling
    void rethrowIfException() const;
    [[nodiscard]] std::future<void> getCompletionFuture();

private:
    void setState(ThreadState newState);
    void checkThreadState() const;
    void handleThreadException();
};

class VThreadGroup {
private:
    std::vector<VThread> threads;
    mutable std::mutex groupMutex;

public:
    VThreadGroup() = default;
    ~VThreadGroup() = default;

    // Thread management
    void addThread(VThread&& thread);
    void startAll();
    void joinAll();
    void pauseAll();
    void resumeAll();
    void stopAll();
    
    // Thread information
    [[nodiscard]] size_t size() const;
    [[nodiscard]] bool hasErrors() const;
    void rethrowFirstException();
    
    // Thread utilities
    void waitForAll();
    void waitForAny();
};