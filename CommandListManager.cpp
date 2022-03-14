#include "CommandListManager.h"

template<typename T>
CommandListManager<T>::CommandListManager(UINT countListPerThread) 
{
	if (countListPerThread > m_lists.size())
	{
		auto allocPool = CommandAllocatorPool::GetAllocPool();
		allocPool.Resize(countListPerThread);
		auto defAlloc = allocPool.GetAllocator(0);
		auto device = Direct12Renderer::GetDevice();
		for (int i = 0; i < countListPerThread; i++)
		{
			m_lists[i] = CommandList(device, defAlloc);
		}
	}
}

template<typename T>
CommandList<T>& CommandListManager<T>::GetList(FrameId id)
{
	return m_lists[id];
}
