#pragma once

#include <VObject>
#include <thread>
#include <chrono>

class VThread : public VObject {
public:
    VThread() = default;
    ~VThread();

    void start();
    void stop();

    void sleep(int milliseconds);
};