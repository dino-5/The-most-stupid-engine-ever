#ifndef COMMAND_ALLOCATOR_POOL_H
#define COMMAND_ALLOCATOR_POOL_H
#include"commin.h"
#include"CommandAllocator.h"
#include<vector>

class CommandAllocatorPool
{
private:
	std::vector<CommandAllocator> m_pool;

public:
	static CommandAllocatorPool& GetAllocPool()
	{
		static CommandAllocatorPool obj;
		return obj;
	}
	
	void Resize(int size)
	{
		if (size > m_pool.capacity())
		{
			m_pool.reserve(size);
		}
	}

	// returns the id of the first allocator, the next one's id incremented
	AllocatorId AddAllocators(CommandListType type, ComPtr<ID3D12Device> device, UINT count=1); 	
	
	CommandAllocator& GetAllocator(AllocatorId id)
	{
		return m_pool[id];
	}

private:
	CommandAllocatorPool() = default;
	
};

#endif

