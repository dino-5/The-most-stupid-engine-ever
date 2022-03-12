#include "CommandAllocatorPool.h"

AllocatorId CommandAllocatorPool::AddAllocators(CommandListType type, ComPtr<ID3D12Device> device, UINT count )
{
	AllocatorId returnValue = m_pool.size();
	for (int i = 0; i < count; i++)
	{
		m_pool.push_back(CommandAllocator(type, device));
	}
	return returnValue;
}
