#include "Application.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    const int screen_width = 800;
    const int screen_height = 600;

    Application* application = new Application;
    if (!application ||
        !application->initialize(hInstance, nCmdShow, screen_width, screen_height))
    {
        return 0;
    }

    int ret = application->run();
    application->shutdown();
    delete application;
    return ret;
}
