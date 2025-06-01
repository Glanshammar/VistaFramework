#include "VEvent.hpp"

VEvent::VEvent(Type type)
    : eventType(type)
    , accepted(false)
    , propagating(true)
    , eventTimestamp(std::chrono::system_clock::now())
{
} 