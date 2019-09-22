#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>

#include "Input.h"
#include "Graphics.h"
#include "Timer.h"
#include"Logger.h"

const int DEFAULT_FRAME_INTERVAL = 20000; // micro-seconds, 50fps

// no. of frames that can be skipped in any one animation loop
// i.e the games state is updated but not rendered
const int DEFAULT_MAX_FRAME_SKIPS = 5;

const int MAX_STATS_COUNT = 64;

class Application : public Logger::Listener
{
public:
    Application() = default;
    Application(const Application&) = delete;

    bool initialize(HINSTANCE hInstance, int nCmdShow, int screen_width, int screen_height);
    int run();
    void shutdown();

    LRESULT on_message(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    bool init_window(HINSTANCE hInstance, int nCmdShow, int screen_width, int screen_height);
    bool update();
    bool render();

    void initStats();
    void storeStats();
    void printStats();

    virtual void onLog(const std::string& text) override;

public:
    HINSTANCE   m_instance;
    HWND        m_hwnd;

private:
    Input*      m_input = nullptr;
    Graphics*   m_graphics = nullptr;
    Timer*      m_timer = nullptr;

    bool        m_quit = false;

    double m_FRAME_INTERVAL = 1.0 / 100; // seconds
    int    m_MAX_FRAME_SKIPS ; // max allowable frame skips

    double m_startTime;         // this is set when the thread is started in Run()
    double m_timeSpent;         // this is set when MAX_STATS_INTERVAL is reached in StoreStats()
    double m_prevStatsTime;     // the last time STATS is calcualted
    double m_statsInterval = 0; // how long it is from the last STATS calculation
    double m_totalElapsedTime = 0;
    unsigned long m_frameCount = 0;
    unsigned long m_framesSkipped = 0;
    unsigned long m_totalFramesSkipped = 0;

    unsigned long m_statsCount = 0;
    double m_upsStore[MAX_STATS_COUNT];
    double m_fpsStore[MAX_STATS_COUNT];
    double m_averageFPS = 0;
    double m_averageUPS = 0;
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
