#include"commin.h"


const D3D12_DESCRIPTOR_HEAP_TYPE descriptorTypes[] =
{
	D3D12_DESCRIPTOR_HEAP_TYPE_RTV	,
	D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV	,
	D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
	D3D12_DESCRIPTOR_HEAP_TYPE_DSV	,
	D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES
};

const D3D12_COMMAND_LIST_TYPE comListTypes[] =
{
	
	D3D12_COMMAND_LIST_TYPE_DIRECT,
	D3D12_COMMAND_LIST_TYPE_BUNDLE,
	D3D12_COMMAND_LIST_TYPE_COMPUTE,
	D3D12_COMMAND_LIST_TYPE_COPY,
};

void CreateDescriptorHeap(ComPtr<ID3D12Device>& device,ComPtr<ID3D12DescriptorHeap>& descHeap, UINT count, DescriptorHeapTypes type, UINT flags )
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvDesc = {};
	rtvDesc.Type = descriptorTypes[static_cast<int>(type)];
	rtvDesc.NumDescriptors = count;
	rtvDesc.Flags = (D3D12_DESCRIPTOR_HEAP_FLAGS)flags;

	ThrowIfFailed(
		device->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&descHeap))
	);

}
