#pragma once
#include <windows.h>
#include<stdexcept>

class Window
{
private:
    struct WindowRegisterClass
    {
    private:
         UINT m_width, m_height;
         const wchar_t* m_className;
         const wchar_t* m_title;
         WNDCLASSEX m_wc;
         HWND m_hwnd;
         HINSTANCE m_hinst;
         bool m_isInitialized = false;
        
    private:
        void CreateWindowClass();
        void CreateWindowObject();

    public:
        WindowRegisterClass(int width, int height, const wchar_t* className, const wchar_t* title, HINSTANCE hinst);
        WindowRegisterClass() = default;
        bool IsInitialized() { return m_isInitialized; }
        HWND GetWindow() { return m_hwnd;  }


    };
    static WindowRegisterClass window;

public:
    Window() = delete;
    static void Init(int width, int height, const wchar_t* className, const wchar_t* title, HINSTANCE hinst);
    static bool IsInitialized() { return window.IsInitialized(); }
    static HWND GetWindow() { return window.GetWindow(); }
    static void Show(int nShowCmd)
    {
        if (window.IsInitialized())
        {
            ShowWindow(window.GetWindow(), nShowCmd);
            UpdateWindow(window.GetWindow());
        }
        else
        {
            throw std::runtime_error("Failed to show window. Window class isn't initialized");
        }
    }


};