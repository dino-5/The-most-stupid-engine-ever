#pragma once
#include"commin.h"

template<typename CLType>
class CommandList 
{
private:
	ComPtr<CLType> m_commandList;

public:
	CommandList() = default;
	void Init(ComPtr<ID3D12Device> device, ComPtr<ID3D12CommandAllocator> alloc, ComPtr<ID3D12PipelineState> pso)
	{
		ThrowIfFailed(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, alloc.Get(), pso.Get(), IID_PPV_ARGS(&m_commandList)));
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

};

