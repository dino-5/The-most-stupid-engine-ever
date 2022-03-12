#ifndef COMMAND_ALLOCATOR_H
#define COMMAND_ALLOCATOR_H
#include"commin.h"


class CommandAllocator
{
private:
	ComPtr<ID3D12CommandAllocator> m_allocator;
	D3D12_COMMAND_LIST_TYPE m_type;
public:
	CommandAllocator() = default;
	CommandAllocator(CommandListType type, ComPtr<ID3D12Device>device):m_type(comListTypes[static_cast<int>(type)])
	{
		ThrowIfFailed(device->CreateCommandAllocator(m_type, IID_PPV_ARGS(&m_allocator)));
	}

	D3D12_COMMAND_LIST_TYPE GetType()
	{
		return m_type;
	}

	ComPtr<ID3D12CommandAllocator> operator->()
	{
		return m_allocator;
	}

	ID3D12CommandAllocator* Get()
	{
		return m_allocator.Get();
	}

};



#endif
