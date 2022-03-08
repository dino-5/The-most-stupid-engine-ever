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



namespace Graphics
{
}

namespace DescriptorHeap
{
	enum Types
	{
		RTV = 0 ,
		SCU,
		SAMPLER,
		DSV

	};
	extern D3D12_DESCRIPTOR_HEAP_TYPE types[];


	void CreateDescriptorHeap(ComPtr<ID3D12Device>& device, ComPtr<ID3D12DescriptorHeap>&descHeap, UINT count, Types type, UINT flags = 0);
	
}


#endif
