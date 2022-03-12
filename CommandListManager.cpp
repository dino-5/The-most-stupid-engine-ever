#include "CommandListManager.h"

template<typename T>
CommandListManager<T>::CommandListManager(UINT countListPerThread) : m_lists(countListPerThread)
{
	auto allocPool = CommandAllocatorPool::GetAllocPool();
	allocPool.Resize(countListPerThread);
	auto defAlloc = allocPool.GetAllocator(0);
	for (int i = 0; i < countListPerThread; i++)
	{

	}
}

template<typename T>
CommandList<T>& CommandListManager<T>::GetList(ListId id)
{

}
