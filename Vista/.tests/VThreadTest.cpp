#include "VThreadTest.hpp"
#include <VThread>
#include <VConsole>
#include <cassert>
#include <atomic>

void TestBasicThread() {
    VPrint("Starting basic thread test...");
    int value = 0;
    VThread thread([&value]() {
        value = 42;
    }, "BasicThread");
    
    thread.start();
    thread.join();
    
    assert(value == 42);
    VPrint("Basic thread test passed");
}

void TestThreadGroup() {
    VPrint("Starting thread group test...");
    std::atomic<int> counter(0);
    VThreadGroup group;
    
    for (int i = 0; i < 5; ++i) {
        VPrint("Adding thread " + std::to_string(i));
        group.addThread(VThread([&counter]() {
            counter++;
        }, "GroupThread" + std::to_string(i)));
    }
    
    VPrint("Starting all threads");
    group.startAll();
    VPrint("Waiting for all threads to complete");
    group.joinAll();
    
    assert(counter == 5);
    VPrint("Thread group test passed");
}

void TestThreadErrorHandling() {
    VPrint("Starting error handling test...");
    bool exceptionCaught = false;
    
    try {
        VThread thread([]() {
            VPrint("Thread throwing exception");
            throw std::runtime_error("Test exception");
        }, "ErrorThread");
        
        thread.start();
        VPrint("Waiting for thread to complete");
        thread.join();
        
        VPrint("Checking for exception");
        thread.rethrowIfException();
    } catch (const std::runtime_error& e) {
        exceptionCaught = true;
        assert(std::string(e.what()) == "Test exception");
        VPrint("Exception caught: " + std::string(e.what()));
    }
    
    assert(exceptionCaught);
    VPrint("Thread error handling test passed");
}

void TestThreadFuture() {
    VPrint("Starting future test...");
    std::promise<int> promise;
    std::future<int> future = promise.get_future();
    
    VThread thread([&promise]() {
        VPrint("Thread setting future value");
        promise.set_value(42);
    }, "FutureThread");
    
    thread.start();
    VPrint("Waiting for future value");
    assert(future.get() == 42);
    VPrint("Future value received, waiting for thread");
    thread.join();
    
    VPrint("Thread future test passed");
}

void VThreadTest() {
    VPrint("\n=== Starting VThread Tests ===");
    
    TestBasicThread();
    TestThreadGroup();
    TestThreadErrorHandling();
    TestThreadFuture();
    
    VPrint("=== All VThread Tests Passed ===\n");
} 