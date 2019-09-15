#include <Windows.h>
#include "Logger.h"
#include "Timer.h"


Timer::Timer()
{
    // get the frequency of the PerformanceCounter
    long long int frequency = 0;
    if (QueryPerformanceFrequency((LARGE_INTEGER*)&frequency))
    {
        // compute the m_secondsPerCount as the reciprocal of the frequency
        m_secondsPerCount = 1.0 / (double)frequency;
#ifndef NDEBUG
        LOG << "The high-precision timer was created successfully.\n";
#endif
    }
    else
        // the hardware does not support a high-precision timer -> throw an error
        throw std::runtime_error("The hardware does not support a high-precision timer!");
}

Timer::~Timer()
{
#ifndef NDEBUG
    // log success
    LOG << "The timer was successfully destroyed.\n";
#endif
}

double Timer::getTime() const
{
    long long int now = 0;
    if (!QueryPerformanceCounter((LARGE_INTEGER*)&now))
        // unable to query the performance counter, throw an error
        throw std::runtime_error("Unable to query the performance counter!");

    return now * m_secondsPerCount;
}

double Timer::getDeltaTime() const
{
    // this function returns the time elapsed between two frames; delta time is updated during the game loop
    return m_deltaTime;
}

double Timer::getTotalTime() const
{
    // this function returns the total time since the game started: (t_now - t_start) - t_totalIdle
    if (m_isStopped)
        return (m_pausedTime - m_startTime - m_totalIdleTime)*m_secondsPerCount;
    else
        return (m_currentTime - m_startTime - m_totalIdleTime)*m_secondsPerCount;
}

void Timer::reset()
{
    // this function resets the timer
    long long int now = 0;
    if (QueryPerformanceCounter((LARGE_INTEGER*)&now))
    {
        m_startTime = now;
        m_previousTime = now;
        m_pausedTime = 0;
        m_isStopped = false;

#ifndef NDEBUG
        LOG << "The timer was reset.\n";
#endif

    }
    else
        // unable to query the performance counter, throw an error
        throw std::runtime_error("Unable to query the performance counter!");
}

void Timer::start()
{
    // this function starts the timer (if it is not already running)
    if (m_isStopped)
    {
        long long int now = 0;
        if (QueryPerformanceCounter((LARGE_INTEGER*)&now))
        {
            // add the duration of the pause to the total idle time
            m_totalIdleTime += (now - m_pausedTime);

            // set the previous time to the current time
            m_previousTime = now;

            // reset the pauseTime to 0 and m_isStopped to false
            m_pausedTime = 0;
            m_isStopped = false;

#ifndef NDEBUG
            LOG << "The timer was started.\n";
#endif
        }
        else
            // unable to query the performance counter, throw an error
            throw std::runtime_error("Unable to query the performance counter!");
    }

}

void Timer::stop()
{
    // this function stops the timer (if it is currently running)
    if (!m_isStopped)
    {
        long long int now = 0;
        if (QueryPerformanceCounter((LARGE_INTEGER*)&now))
        {
            // set the time the timer was stopped to "now"
            m_pausedTime = now;
            m_isStopped = true;

#ifndef NDEBUG
            LOG << "The timer was stopped.\n";
#endif
        }
        else
            // unable to query the performance counter, throw an error
            throw std::runtime_error("Unable to query the performance counter!");
    }
}

void Timer::tick()
{
    // this function lets the timer tick, i.e. it computes the time that has elapsed between two frames
    if (m_isStopped)
    {
        // if the game is stopped, the elapsed time is obviously 0
        m_deltaTime = 0.0;
    }
    else
    {
        // get the current time
        if (QueryPerformanceCounter((LARGE_INTEGER*)&m_currentTime))
        {
            // compute the time elapsed since the previous frame
            m_deltaTime = (m_currentTime - m_previousTime) * m_secondsPerCount;

            // set m_previousTime to m_currentTime, as in the next tick, this frame will be the previous frame
            m_previousTime = m_currentTime;

            // m_deltaTime can be negative if the processor goes idle for example
            if (m_deltaTime < 0.0)
                m_deltaTime = 0.0;
        }
        else
            // unable to query the performance counter, throw an error
            throw std::runtime_error("Unable to query the performance counter!");
    }
}