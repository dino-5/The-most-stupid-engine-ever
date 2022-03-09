#pragma once
#include"commin.h"
#include"CommandAllocator.h"

namespace Graphics
{

	template<typename CLType>
	class CommandList 
	{
	private:
		ComPtr<CLType> m_commandList;

	public:
		CommandList() = default;
		void Init(ComPtr<ID3D12Device> device, CommandAllocator alloc, ComPtr<ID3D12PipelineState> pso )
		{
			ThrowIfFailed(device->CreateCommandList(0, alloc.GetType(), alloc.Get(), pso.Get(), IID_PPV_ARGS(&m_commandList)));
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
}

