#include "App.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    UINT width = 800;
    UINT height = 600;
    const wchar_t* className = L"Directx12";
    const wchar_t* title = L"Directx12";

    App::Init(width, height, className, title, hInstance);
    return App::Run(nCmdShow);
}