#include "Engine.h"

void Engine::Init(UINT width, UINT height)
{
	m_renderer.Init(width, height);
}

void Engine::Draw()
{
	m_renderer.Draw();
}
