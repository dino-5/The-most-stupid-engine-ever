#include "CommandAllocatorPool.h"

AllocatorId CommandAllocatorPool::AddAllocators(CommandListType type,  UINT count )
{
	auto pool = GetAllocatorPool(type);
	auto device = Direct12Renderer::GetDevice();
	AllocatorId returnValue = pool.size();
	for (int i = 0; i < count; i++)
	{
		pool.push_back(CommandAllocator(type, device));
	}
	return returnValue;
}

void CommandAllocatorPool::Resize(int count, CommandListType type)
{
	auto pool = GetAllocatorPool(type);
	
	if (count > pool.capacity())
	{
		pool.reserve(count);
		auto device = Direct12Renderer::GetDevice();
		for (int i = 0; i < count; i++)
		{
			pool.push_back(CommandAllocator(type, device));
		}
	}
}
