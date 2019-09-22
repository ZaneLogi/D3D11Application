#include "Application.h"
#include "Input.h"
#include "Graphics.h"
#include "Logger.h"

static Application* g_application = nullptr;

// Number of frames with a delay of 0 ms before the
// animation thread yields to other running threads.
const int NO_DELAYS_PER_YIELD = 16;

const double MAX_STATS_INTERVAL = 1.0; // 1 second


bool Application::initialize(HINSTANCE hInstance, int nCmdShow, int screen_width, int screen_height)
{
    LOG.addListener(*this);

    g_application = this;

    if (!init_window(hInstance, nCmdShow, screen_width, screen_height))
    {
        return false;
    }

    m_input = new Input;
    if (!m_input ||
        !m_input->initialize(m_hwnd))
    {
        return false;
    }

    // Create the graphics object.  This object will handle rendering all the graphics for this application.
    m_graphics = new Graphics;
    if (!m_graphics ||
        !m_graphics->initialize(m_hwnd, screen_width, screen_height))
    {
        return false;
    }

    m_timer = new Timer;
    if (!m_timer)
    {
        return false;
    }

    return true;
}

int Application::run()
{
    double beforeTime, afterTime, timeDiff, sleepTime;
    double overSleepTime = 0;
    long noDelays = 0;
    double excess = 0;

    m_startTime = m_timer->getTime();
    m_prevStatsTime = m_startTime;

    beforeTime = m_startTime;

    initStats();

    // Main message loop
    MSG msg = { 0 };
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            if (!update() || !render())
            {
                m_quit = true;
                break;
            }

            afterTime = m_timer->getTime();
            timeDiff = afterTime - beforeTime;
            sleepTime = (m_FRAME_INTERVAL - timeDiff) - overSleepTime;

            if (sleepTime > 0) {
                // some time left in this cycle
                Sleep((DWORD)(sleepTime * 1000));
                overSleepTime = (m_timer->getTime() - afterTime) - sleepTime;
            }
            else {
                // sleepTime <= 0; frame took longer than the period
                excess -= sleepTime;  // store excess time value
                overSleepTime = 0L;
                if (++noDelays >= NO_DELAYS_PER_YIELD) {
                    Sleep(0); // give another thread a chance to run
                    noDelays = 0;
                }
            }

            beforeTime = m_timer->getTime();

            // If frame animation is taking too long, update the game state
            // without rendering it, to get the updates/sec nearer to
            // the required FPS.
            long skips = 0;
            while ((excess > m_FRAME_INTERVAL) && (skips < m_MAX_FRAME_SKIPS)) {
                excess -= m_FRAME_INTERVAL;
                update();      // update state but don't render
                skips++;
            }

            m_framesSkipped += skips;

            storeStats();
        }
    }

    printStats();

    return (int)msg.wParam;
}

void Application::shutdown()
{
    if (m_timer)
    {
        delete m_timer;
        m_timer = nullptr;
    }

    if (m_graphics)
    {
        m_graphics->shutdown();
        delete m_graphics;
        m_graphics = nullptr;
    }

    if (m_input)
    {
        delete m_input;
        m_input = nullptr;
    }

    LOG.removeListener(*this);
}

LRESULT Application::on_message(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_KEYDOWN:
        m_input->key_down((int)wParam);
        return 0;
    case WM_KEYUP:
        m_input->key_up((int)wParam);
        return 0;
    case WM_MOUSEMOVE:
        m_input->handle_mousemove();
        return 0;
    case WM_LBUTTONDOWN:
        m_input->handle_lbutton_down(hWnd);
        return 0;
    case WM_LBUTTONUP:
        m_input->handle_lbutton_up();
        return 0;
    case WM_RBUTTONDOWN:
        m_input->handle_rbutton_down(hWnd);
        return 0;
    case WM_RBUTTONUP:
        m_input->handle_rbutton_up();
        return 0;
    case WM_INPUT:
        m_input->handle_input((HRAWINPUT)lParam);
        return 0;
    case WM_SIZE:
    {
        WORD width = LOWORD(lParam);
        WORD height = HIWORD(lParam);
        LOG << "width = " << width << ", height = " << height << "\n";
        // if the application is going to quit, do nothing
        if (!m_quit && width != 0 && height != 0 && m_graphics)
        {
            m_graphics->resize_window(width, height);
        }
        return 0;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

bool Application::init_window(HINSTANCE hInstance, int nCmdShow, int screen_width, int screen_height)
{
    const PCWSTR window_class_name = L"D3D11ApplicationWindowClass";
    const PCWSTR window_title = L"Direct3D 11 Application";

    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = window_class_name;
    wcex.hIconSm = wcex.hIcon;
    if (!RegisterClassEx(&wcex))
        return false;

    // Create window
    m_instance = hInstance;
    RECT rc = { 0, 0, screen_width, screen_height };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    m_hwnd = CreateWindow(window_class_name, window_title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
        nullptr);
    if (!m_hwnd)
        return false;

    ShowWindow(m_hwnd, nCmdShow);

    return true;
}

bool Application::update()
{
    // Check if the user pressed escape and wants to exit the application.
    if (m_input->is_key_down(VK_ESCAPE))
    {
        return false;
    }

    // Do the frame processing for the graphics object.
    int mouse_x, mouse_y;
    m_input->get_mouse_move(&mouse_x, &mouse_y);
    m_graphics->update_camera(mouse_x, mouse_y);
    if (!m_graphics->update())
    {
        return false;
    }

    return true;
}

bool Application::render()
{
    return m_graphics->render();
}

void Application::initStats()
{
    m_startTime = 0;
    m_timeSpent = 0;

    m_statsInterval = 0;
    m_totalElapsedTime = 0;
    m_frameCount = 0;
    m_framesSkipped = 0;
    m_totalFramesSkipped = 0;
    m_statsCount = 0;

    for (int i = 0; i < MAX_STATS_COUNT; i++)
    {
        m_fpsStore[i] = 0;
        m_upsStore[i] = 0;
    }

    m_averageFPS = 0;
    m_averageUPS = 0;
}

void Application::storeStats()
{
    m_frameCount++;
    m_statsInterval += m_FRAME_INTERVAL;

    if (m_statsInterval >= MAX_STATS_INTERVAL)
    {
        double timeNow = m_timer->getTime();
        m_timeSpent = timeNow - m_startTime;

        double realElapsedTime = timeNow - m_prevStatsTime; // time since last stats collection

        m_totalElapsedTime += realElapsedTime;

        double timingError = ((double)realElapsedTime - m_statsInterval) / m_statsInterval * 100.0;

        m_totalFramesSkipped += m_framesSkipped;

        double actualFPS = 0; // calculate the latest FPS and UPS
        double actualUPS = 0;
        if (m_totalElapsedTime > 0)
        {
            actualFPS = (double)m_frameCount / m_totalElapsedTime;
            actualUPS = (double)(m_frameCount + m_totalFramesSkipped) / m_totalElapsedTime;
        }

        // store the latest FPS and UPS
        m_fpsStore[m_statsCount % MAX_STATS_COUNT] = actualFPS;
        m_upsStore[m_statsCount % MAX_STATS_COUNT] = actualUPS;
        m_statsCount = m_statsCount + 1;

        double totalFPS = 0.0;     // total the stored FPSs and UPSs
        double totalUPS = 0.0;
        for (int i = 0; i < MAX_STATS_COUNT; i++)
        {
            totalFPS += m_fpsStore[i];
            totalUPS += m_upsStore[i];
        }

        if (m_statsCount < MAX_STATS_COUNT)
        { // obtain the average FPS and UPS
            m_averageFPS = totalFPS / m_statsCount;
            m_averageUPS = totalUPS / m_statsCount;
        }
        else
        {
            m_averageFPS = totalFPS / MAX_STATS_COUNT;
            m_averageUPS = totalUPS / MAX_STATS_COUNT;
        }

#if 0
        printf("%7d %7d %5.2f%% %5d %3d/%3d skip; %5.2f %5.2f afps; %5.2f %5.2f aups;\n",
            m_statsInterval,
            realElapsedTime,
            timingError,
            m_frameCount,
            m_framesSkipped,
            m_totalFramesSkipped,
            actualFPS,
            m_averageFPS,
            actualUPS,
            m_averageUPS);
#endif

        m_framesSkipped = 0;
        m_prevStatsTime = timeNow;
        m_statsInterval = 0L;   // reset
    }
}

void Application::printStats()
{
    char text[128];
    sprintf_s(text, "Frame Count/Loss: %d / %d\r\n", m_frameCount, m_totalFramesSkipped);
    LOG << text;
    sprintf_s(text, "Average FPS: %f\n", m_averageFPS);
    LOG << text;
    sprintf_s(text, "Average UPS: %f\n", m_averageUPS);
    LOG << text;
}

void Application::onLog(const std::string& text)
{
    OutputDebugStringA(text.c_str());
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return g_application->on_message(hWnd, message, wParam, lParam);
    }

    return 0;
}