#include"Window.h"


LRESULT CALLBACK WndProc(HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (msg)
    {

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            if (MessageBox(0, L"Are you sure you want to exit?",
                L"Really?", MB_YESNO | MB_ICONQUESTION) == IDYES)
                DestroyWindow(hwnd);
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd,
        msg,
        wParam,
        lParam);
}

void Window::WindowRegisterClass::CreateWindowClass()
{

    m_wc.cbSize = sizeof(WNDCLASSEX);
    m_wc.style = CS_HREDRAW | CS_VREDRAW;
    m_wc.lpfnWndProc = WndProc;
    m_wc.cbClsExtra = NULL;
    m_wc.cbWndExtra = NULL;
    m_wc.hInstance = m_hinst;
    m_wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    m_wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    m_wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
    m_wc.lpszMenuName = NULL;
    m_wc.lpszClassName = m_className;
    m_wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassEx(&m_wc);
}

void Window::WindowRegisterClass::CreateWindowObject()
{
    m_hwnd = CreateWindowEx(NULL,
        m_className,
        m_title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        m_width, m_height,
        NULL,
        NULL,
        m_hinst,
        NULL);
}

Window::WindowRegisterClass::WindowRegisterClass(int width, int height, const wchar_t* className, const wchar_t* title, HINSTANCE hinst) :
    m_width(width), m_height(height), m_className(className), m_title(title), m_hinst(hinst), m_isInitialized(true)
{
    CreateWindowClass();
    CreateWindowObject();
}

Window::WindowRegisterClass Window::window;

void Window::Init(int width, int height, const wchar_t* className, const wchar_t* title, HINSTANCE hinst)
{
    window = Window::WindowRegisterClass(width, height, className, title, hinst); 
}

