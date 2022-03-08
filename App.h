#pragma once 
#include"Window.h"
#include"Directx12Renderer.h"
#include<memory>

class App
{
private:
    static inline Direct12Renderer m_renderer;
public:
    static void Init(UINT width, UINT height, const wchar_t* className, const wchar_t* title, HINSTANCE hinst) 
    {
        Window::Init(width, height, className, title, hinst);  
        m_renderer.Init(width, height);
    }
     
    static int Run(int nShowCmd)
    {
        MSG msg;
        try 
        {
            Window::Show(nShowCmd);
        }
        catch (std::runtime_error err)
        {
            return -1;
        }

        while (true)
        {
            m_renderer.Draw();
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                    break;

                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        return 0;
    }

};