#include "VThread.hpp"

VThread::VThread(const std::function<void()>& functionName) {
    if (functionName) {
        thread = std::thread(functionName);
    }
}

VThread::VThread(VObject* object) {
    VThread::Connect(object);
}

VThread::~VThread()
{
    if (thread.joinable()) {
        thread.join();  // Wait for thread to finish before destruction
    }
}


void VThread::start()
{

}
