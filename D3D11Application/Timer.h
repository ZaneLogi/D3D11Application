#pragma once


class Timer
{
public:
    // constructor
    Timer();
    ~Timer();

    double getTime() const; // return seconds

    // getters: return time measured in seconds
    double getTotalTime() const;        // returns the total time the game has been running (minus paused time)
    double getDeltaTime() const;        // returns the time between two frames

    // methods
    void start();                       // starts the timer, called each time the game is unpaused
    void reset();                       // sets the counter to zero, called once before message loop
    void tick();                        // called every frame, lets the time tick
    void stop();                        // called when the game is paused

private:
    // times measured in counts
    long long int m_startTime = 0;      // time at the start of the application
    long long int m_totalIdleTime = 0;  // total time the game was idle
    long long int m_pausedTime = 0;     // time at the moment the game was paused last
    long long int m_currentTime = 0;    // stores the current time; i.e. time at the current frame
    long long int m_previousTime = 0;   // stores the time at the last inquiry before current; i.e. time at the previous frame

    // times measured in seconds
    double m_secondsPerCount = 0;       // reciprocal of the frequency, computed once at the initialization of the class
    double m_deltaTime = 0;             // time between two frames, updated during the game loop

    // state of the timer
    bool m_isStopped = false;           // true iff the timer is stopped
};