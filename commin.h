#ifndef COMMON_H
#define COMMON_H
#include<d3d12.h>
#include<DirectXMath.h>
#include<dxgi.h>
#include"d3dx12.h"
#include<wrl/client.h>_
#include<dxgi1_3.h>
#include<dxgi1_4.h>
#include"Window.h"
#include<d3dcompiler.h>
#include<string>
#include<sstream>

template<typename T>
using ComPtr = Microsoft::WRL::ComPtr <T> ;

void ThrowIfFailed(HRESULT hr);

extern const D3D12_COMMAND_LIST_TYPE comListTypes[];
enum class CommandListType 
{
	DIRECT = 0,
	BUNDLE,
	COMPUTE,
	COPY
};

extern const D3D12_DESCRIPTOR_HEAP_TYPE descriptorTypes[];
enum class DescriptorHeapTypes
{
	RTV = 0 ,
	SCU,
	SAMPLER,
	DSV

};
void CreateDescriptorHeap(ComPtr<ID3D12Device>& device, ComPtr<ID3D12DescriptorHeap>&descHeap, UINT count, DescriptorHeapTypes type, UINT flags = 0);

using AllocatorId = int;
using ListId= int;
using FrameId = int;



#endif
