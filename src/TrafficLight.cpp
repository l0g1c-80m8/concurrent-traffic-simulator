#include <iostream>
#include <random>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */

template <typename T>
T MessageQueue<T>::receive()
{
    // define a unique lock on _mtx
    std::unique_lock<std::mutex> lock(_mtx);
    // block until notified
    _cond.wait(lock, [this]{ return !_queue.empty(); });

    // get last message, remove it from queue and return
    T msg = std::move(_queue.back());
    _queue.pop_back();
    return msg;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // acquire lock in RAII style
    std::lock_guard<std::mutex> lock(_mtx);
    // add a new message to the queue
    _queue.emplace_back(std::move(msg));
    // notify a blocked thread on _mtx to exec
    _cond.notify_one();
}

/* Implementation of class "TrafficLight" */

TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    while (true)
    {
        TrafficLightPhase currPhase = _msgQueue.receive();
        if (currPhase == TrafficLightPhase::green)
            return;
    }
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

    while (true)
    {
        // wait between subsequent checks (10ms is sufficient compared to ~ 4 - 6 seconds)
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

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

        // send update to message queue using move semantics
        _msgQueue.send(std::move(_currentPhase));
    }
}
