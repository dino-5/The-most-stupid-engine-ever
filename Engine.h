#ifndef ENGINE_H
#define ENGINE_H
#include"commin.h"
#include"Directx12Renderer.h"

class Engine
{
private:
    static inline Direct12Renderer m_renderer;
public:
	Engine()=default;
	void Init(UINT width, UINT height);
	void Draw();
};

#endif

