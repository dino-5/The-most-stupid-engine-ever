#include "Engine.h"

void Engine::Init(UINT width, UINT height)
{
	m_renderer.Init(width, height);
	m_aspectRatio = width / height;

	Vertex triangleVertices[] =
	{
		{ { 0.25f, 0.25f * m_aspectRatio, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
		{ { 0.25f, -0.25f * m_aspectRatio, 0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ { -0.25f, -0.25f * m_aspectRatio, 0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
		{ { -0.25f, 0.25f * m_aspectRatio, 0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
	};

	UINT indexBuff[] =
	{
		0, 1, 2,
		3, 0, 2
	};

	const UINT vertexBufferSize = sizeof(triangleVertices);
	const UINT indexBufferSize= sizeof(indexBuff);
	

	m_indexBufferId= m_renderer.CreateVertexBuffer(triangleVertices, vertexBufferSize);
	m_vertexBufferId= m_renderer.CreateIndexBuffer(indexBuff, indexBufferSize);
}

void Engine::Draw()
{

    m_renderer.SetVertexBuffer(m_vertexBufferId);
    m_renderer.SetIndexBuffer(m_indexBufferId);
	m_renderer.Draw();
}
