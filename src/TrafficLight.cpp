#include <iostream>
#include <random>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */

/*
template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait()
    // to wait for and receive new messages and pull them from the queue using move semantics.
    // The received object should then be returned by the receive function.
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex>
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
}
*/

/* Implementation of class "TrafficLight" */

TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop
    // runs and repeatedly calls the receive function on the message queue.
    // Once it receives TrafficLightPhase::green, the method returns.
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // create a thread that runs on cycleThroughPhases and add it in the list of traffic object threads
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// virtual function which is executed in a thread
[[noreturn]] void TrafficLight::cycleThroughPhases()
{
    // https://www.techiedelight.com/generate-random-numbers-in-the-specified-range-in-cpp/
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(4000.0, 6000.0);
    const double loopDelay = dist(gen);
    https://cplusplus.com/reference/chrono/duration/
    auto startTime = std::chrono::high_resolution_clock::now();
    auto waitTime = loopDelay;

    while (true)
    {
        // wait between subsequent checks
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        // skip exec if loopDelay is not met
        if (std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - startTime).count() < loopDelay)
            continue;

        // reset startTime to current time
        startTime = std::chrono::high_resolution_clock::now();

        // switch traffic light phase
        switch(_currentPhase) {
            case TrafficLightPhase::red:
                _currentPhase = TrafficLightPhase::green;
                break;
            case TrafficLightPhase::green:
                _currentPhase = TrafficLightPhase::red;
                break;
        }

        // TODO: send update to message queue using move semantics
    }
}
