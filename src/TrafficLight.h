#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"

enum class TrafficLightPhase {red, green};

// forward declarations to avoid include cycle
class Vehicle;

template <class T>
class MessageQueue
{
public:

    void send(T &&msg);

    T receive();

private:
    std::deque<T> _queue;
    std::condition_variable _cond;
    std::mutex _mtx;
};

class TrafficLight : public TrafficObject
{
public:
    // constructor / destructor
    TrafficLight();

    // getters / setters
    TrafficLightPhase getCurrentPhase();

    // typical behaviour methods
    void waitForGreen();

    void simulate();

private:
    // typical behaviour methods
    [[noreturn]] void cycleThroughPhases();

    TrafficLightPhase _currentPhase;

    // FP.4b : create a private member of type MessageQueue for messages of type TrafficLightPhase 
    // and use it within the infinite loop to push each new TrafficLightPhase into it by calling 
    // send in conjunction with move semantics.

    std::condition_variable _condition;
    std::mutex _mutex;
};

#endif