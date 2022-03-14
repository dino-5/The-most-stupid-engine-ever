#ifndef COMMAND_ALLOCATOR_POOL_H
#define COMMAND_ALLOCATOR_POOL_H
#include"commin.h"
#include"CommandAllocator.h"
#include"Directx12Renderer.h"
#include<vector>

class CommandAllocatorPool
{
private:
	const UINT poolsCount = 4;
	std::vector<std::vector<CommandAllocator>> m_pool;

public:
	static CommandAllocatorPool& GetAllocPool()
	{
		static CommandAllocatorPool obj;
		return obj;
	}
	
	void Resize(int count, CommandListType type);

	// returns the id of the first allocator, the next one's id incremented
	AllocatorId AddAllocators(CommandListType type,  UINT count=1); 	

	
	CommandAllocator& GetAllocator(AllocatorId id, CommandListType type)
	{
		GetAllocatorPool(type)[id];
	}

private:
	std::vector<CommandAllocator>& GetAllocatorPool(CommandListType type)
	{
		return m_pool[static_cast<int>(type)];
	}
	CommandAllocatorPool() :m_pool(poolsCount) {}
	
};

#endif

