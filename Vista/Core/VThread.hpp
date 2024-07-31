#pragma once

#include <VObject>
#include <thread>
#include <chrono>
#include <atomic>


class VThread {
private:
    std::thread thread;
    std::thread::id threadID;
    static std::atomic<int32_t> threadCount;
    std::function<void()> task;
    bool started;

public:
    explicit VThread(const std::function<void()>& functionName);
    ~VThread();

    VThread(const VThread&) = delete;
    VThread& operator=(const VThread&) = delete;

    VThread(VThread&& other) noexcept;
    VThread& operator=(VThread&& other) noexcept;

    void start();
    void join();
    [[nodiscard]] std::thread::id getID() const;
    static void Sleep(float seconds);
};

class VThreadGroup {
private:
    std::vector<VThread> threads;

public:
    VThreadGroup() = default;

    // Add a VThread to the group
    void addThread(VThread&& thread);

    // Start all threads in the group
    void startAll();

    // Join all threads in the group
    void joinAll();

    // Get the number of threads in the group
    [[nodiscard]] size_t size() const;
};