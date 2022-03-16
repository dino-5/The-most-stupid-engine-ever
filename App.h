#pragma once 
#include"Window.h"
#include"Engine.h"
#include<memory>

class App
{
private:
    Engine& m_engine;
    App() :m_engine(Engine::GetEngine() ) {}
public:
    static App& GetApp()
    {
        static App obj;
        return obj;
    }

    void Init(UINT width, UINT height, const wchar_t* className, const wchar_t* title, HINSTANCE hinst) 
    {
        Window::Init(width, height, className, title, hinst);  
        m_engine.Init(width, height);
    }
     
    int Run(int nShowCmd)
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
            m_engine.Draw();
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