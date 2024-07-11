#pragma once

#include <VObject>
#include <thread>
#include <chrono>

class VThread : public VObject {
private:
    std::thread thread;
public:
    explicit VThread(const std::function<void()>& functionName);
    explicit VThread(VObject* object);
    ~VThread();

    static void start();
    static void stop();

    static void sleep(int milliseconds);
};