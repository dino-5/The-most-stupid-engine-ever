#ifndef ENGINE_H
#define ENGINE_H
#include"commin.h"
#include"Directx12Renderer.h"

class Engine
{
private:
	static inline Direct12Renderer m_renderer;
	Engine() = default;

	float m_aspectRatio;
	
	BufferId m_indexBufferId;
	BufferId m_vertexBufferId;
public:
	void Init(UINT width, UINT height);

	void Draw();
	static Engine& GetEngine()
	{
		static Engine obj;
		return obj;
	}
};

#endif

