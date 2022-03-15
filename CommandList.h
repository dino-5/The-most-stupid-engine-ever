#pragma once
#include"commin.h"
#include"CommandAllocator.h"

template<typename CLType>
class CommandList;

using GraphicsCommandList = CommandList<ID3D12GraphicsCommandList>;

template<typename CLType>
class CommandList 
{
private:
	ComPtr<CLType> m_commandList;

public:
	CommandList() = default;
	void Init(ComPtr<ID3D12Device> device, CommandAllocator alloc )
	{
		ThrowIfFailed(device->CreateCommandList(0, alloc.GetType(), alloc.Get(), nullptr, IID_PPV_ARGS(&m_commandList)));
		m_commandList->Close();
	}

	CLType* Get()
	{
		return m_commandList.Get();
	}

	ComPtr<CLType> operator->()
	{
		return m_commandList;
	}

	CLType** GetAddressOf()
	{
		return m_commandList.GetAddressOf();
	}

};

