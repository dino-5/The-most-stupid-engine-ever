#include"commin.h"

namespace DescriptorHeap 
{
	D3D12_DESCRIPTOR_HEAP_TYPE types[] =
	{
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV	,
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV	,
		D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
		D3D12_DESCRIPTOR_HEAP_TYPE_DSV	,
		D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES
	};

	void CreateDescriptorHeap(ComPtr<ID3D12Device>& device,ComPtr<ID3D12DescriptorHeap>& descHeap, UINT count, Types type, UINT flags )
	{
		D3D12_DESCRIPTOR_HEAP_DESC rtvDesc = {};
		rtvDesc.Type = types[type];
		rtvDesc.NumDescriptors = count;
		rtvDesc.Flags = (D3D12_DESCRIPTOR_HEAP_FLAGS)flags;

		ThrowIfFailed(
			device->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&descHeap))
		);

	}
};
