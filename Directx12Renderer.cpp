#include"Directx12Renderer.h"
#include<stdexcept>
#include<string>
#include<sstream>

void ThrowIfFailed(HRESULT hr)
{
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to do something");
    }
}

Direct12Renderer::Direct12Renderer(UINT width, UINT height) :
    m_width(width),
    m_height(height),
    m_viewport(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height)),
    m_scissorRect(0, 0, static_cast<LONG>(width), static_cast<LONG>(height))
{

}
Direct12Renderer& Direct12Renderer::GetInstance()
{
	static Direct12Renderer instance(0,0);
	return instance;
}

void Direct12Renderer::Init(UINT width, UINT height)
{
    static bool isInitialized = false;
    if (!isInitialized)
    {
		*this = Direct12Renderer(width, height);
		CreateDevice();
		CreateQueue();
		CreateSwapChain();
		GetDescSize();
		CreateDescriptors();
		CreateCommandAllocator();
        m_commandAllocator = CommandAllocator(CommandListType::DIRECT, m_device);
		CreateRootSignature();
		LoadShaders();
        isInitialized = true;
    }
}

void Direct12Renderer::LoadShaders()
{
    ComPtr<ID3DBlob> vertexShader;
    ComPtr<ID3DBlob> pixelShader;

#if defined(_DEBUG)
    // Enable better shader debugging with the graphics debugging tools.
    UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    UINT compileFlags = 0;
#endif

    ThrowIfFailed(D3DCompileFromFile(L"shader.hlsl", nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, nullptr));
    ThrowIfFailed(D3DCompileFromFile(L"shader.hlsl", nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, nullptr));

    // Define the vertex input layout.
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    // Describe and create the graphics pipeline state object (PSO).
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
    psoDesc.pRootSignature = m_rootSignature.Get();
    psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader.Get());
    psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader.Get());
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState.DepthEnable = FALSE;
    psoDesc.DepthStencilState.StencilEnable = FALSE;
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDesc.SampleDesc.Count = 1;
    ThrowIfFailed(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pso)));

    m_commandList.Init(m_device, m_commandAllocator, m_pso);
    //ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), m_pso.Get(), IID_PPV_ARGS(&m_commandList)));
    //ThrowIfFailed(m_commandList->Close());

    // Command lists are created in the recording state, but there is nothing
    // to record yet. The main loop expects it to be closed, so close it now.

    {
        ThrowIfFailed(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
        m_fenceValue = 1;

        // Create an event handle to use for frame synchronization.
        m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (m_fenceEvent == nullptr)
        {
            ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
        }

        // Wait for the command list to execute; we are reusing the same command 
        // list in our main loop but for now, we just want to wait for setup to 
        // complete before continuing.
        WaitForPreviousFrame();
    }

    // Create the vertex buffer.
    {
        // Define the geometry for a triangle.
        Vertex triangleVertices[] =
        {
            { { 0.25f, 0.25f * m_aspectRatio, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
            { { 0.25f, -0.25f * m_aspectRatio, 0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
            { { -0.25f, -0.25f * m_aspectRatio, 0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
            { { -0.25f, 0.25f * m_aspectRatio, 0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
        };

        UINT indexBuff[] =
        {
            0, 1, 2,
            3, 0, 2
        };

        const UINT vertexBufferSize = sizeof(triangleVertices);
        const UINT indexBufferSize= sizeof(indexBuff);

        // Note: using upload heaps to transfer static data like vert buffers is not 
        // recommended. Every time the GPU needs it, the upload heap will be marshalled 
        // over. Please read up on Default Heap usage. An upload heap is used here for 
        // code simplicity and because there are very few verts to actually transfer.
        CreateResource(m_commandList, m_vertexBuffer, triangleVertices, vertexBufferSize);

        // Initialize the vertex buffer view.
        m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
        m_vertexBufferView.StrideInBytes = sizeof(Vertex);
        m_vertexBufferView.SizeInBytes = vertexBufferSize;

        CreateResource(m_commandList, m_indexBuffer, indexBuff, indexBufferSize);
        m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
        m_indexBufferView.SizeInBytes = indexBufferSize;
        m_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
}
    // void CreateResource(CommandList, ComPtr<ID3D12Resource> res, const void* data, size_t dataSize);

    // Create synchronization objects and wait until assets have been uploaded to the GPU.

}

void Direct12Renderer::CreateResource(GraphicsCommandList commandList, ComPtr<ID3D12Resource>& res, const void* data, UINT dataSize)
{
	auto heapType = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(dataSize);

	ThrowIfFailed(m_device->CreateCommittedResource(
		&heapType,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&res)));

	heapType = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	ComPtr<ID3D12Resource> intermideateRes;
	ThrowIfFailed(m_device->CreateCommittedResource(
		&heapType,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&intermideateRes)));

	D3D12_SUBRESOURCE_DATA subresData = {};
	subresData.pData = data;
	subresData.RowPitch = dataSize;
	subresData.SlicePitch = dataSize;

	ThrowIfFailed(m_commandAllocator->Reset());
	commandList->Reset(m_commandAllocator.Get(), m_pso.Get());

	UpdateSubresources(commandList.Get(), res.Get(), intermideateRes.Get(), 0, 0, 1, &subresData);
	auto resourceTransition = CD3DX12_RESOURCE_BARRIER::Transition(res.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER);
	commandList->ResourceBarrier(1, &resourceTransition);
	commandList->Close();
	ID3D12CommandList* lists[] = {commandList.Get()};
	m_commandQueue->ExecuteCommandLists(1, lists);
	WaitForPreviousFrame();
}


void Direct12Renderer::PopulateCommandList()

{
    // fences to determine GPU execution progress.
    ThrowIfFailed(m_commandAllocator->Reset());

    // However, when ExecuteCommandList() is called on a particular command 
    // list, that command list can then be reset at any time and must be before 
    // re-recording.
    ThrowIfFailed(m_commandList->Reset(m_commandAllocator.Get(), m_pso.Get()));

    // Set necessary state.
    m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
    m_commandList->RSSetViewports(1, &m_viewport);
    m_commandList->RSSetScissorRects(1, &m_scissorRect);

    // Indicate that the back buffer will be used as a render target.
    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    m_commandList->ResourceBarrier(1, &barrier);

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_RTVDescHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescSize);
    m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

    // Record commands.
    const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
    m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
    m_commandList->IASetIndexBuffer(&m_indexBufferView);
    m_commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);

    // Indicate that the back buffer will now be used to present.
    barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    m_commandList->ResourceBarrier(1, &barrier);
    ThrowIfFailed(m_commandList->Close());


}

void Direct12Renderer::Draw()
{
	// Record all the commands we need to render the scene into the command list.
	PopulateCommandList();

	// Execute the command list.
	ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
	m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	// Present the frame.
	ThrowIfFailed(m_swapChain->Present(1, 0));

	WaitForPreviousFrame();

}

void Direct12Renderer::WaitForPreviousFrame()
{
    // WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
    // This is code implemented as such for simplicity. The D3D12HelloFrameBuffering
    // sample illustrates how to use fences for efficient resource usage and to
    // maximize GPU utilization.

    // Signal and increment the fence value.
    const UINT64 fence = m_fenceValue;
    ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), fence));
    m_fenceValue++;

    auto temp = m_fence->GetCompletedValue();
    // Wait until the previous frame is finished.
    if (temp< fence)
    {
        ThrowIfFailed(m_fence->SetEventOnCompletion(fence, m_fenceEvent));
        WaitForSingleObject(m_fenceEvent, INFINITE);
    }

    m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
}


void Direct12Renderer::CreateRootSignature()
{
    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
    rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ComPtr<ID3DBlob> signature;
    ComPtr<ID3DBlob> error;
    ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));
    ThrowIfFailed(m_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)));
}

void Direct12Renderer::CreateDescriptors()
{
    CreateDescriptorHeap(m_device, m_RTVDescHeap, m_frameCount, DescriptorHeapTypes::RTV);
    CreateRTV();
}

void Direct12Renderer::CreateRTV()
{
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_RTVDescHeap->GetCPUDescriptorHandleForHeapStart());
    for (int i = 0; i < m_frameCount; i++)
    {
        m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_renderTargets[i]));
        m_device->CreateRenderTargetView(m_renderTargets[i].Get(), nullptr, rtvHandle);
        rtvHandle.Offset( m_rtvDescSize);
    }
}

void Direct12Renderer::CreateCommandAllocator()
{
}


void Direct12Renderer::CreateQueue()
{
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    ThrowIfFailed(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));
}

void Direct12Renderer::CreateSwapChain()
{
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = m_frameCount;
    swapChainDesc.Width = m_width;
    swapChainDesc.Height = m_height;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;

    ComPtr<IDXGISwapChain1> swapChain;
    ThrowIfFailed(m_dxgiFactory->CreateSwapChainForHwnd(
        m_commandQueue.Get(),        // Swap chain needs the queue so that it can force a flush on it.
        Window::GetWindow(),
        &swapChainDesc,
        nullptr,
        nullptr,
        &swapChain
    ));
    ThrowIfFailed(m_dxgiFactory->MakeWindowAssociation(Window::GetWindow(), DXGI_MWA_NO_ALT_ENTER));
    ThrowIfFailed(swapChain.As(&m_swapChain));

    m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
}

void Direct12Renderer::GetDescSize()
{
    m_rtvDescSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    m_dsvDescSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    m_cbvSrvDescSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void Direct12Renderer::CreateDevice()
{
    UINT dxgiFactoryFlags = 0;

#if defined(_DEBUG)
    // Enable the debug layer (requires the Graphics Tools "optional feature").
    // NOTE: Enabling the debug layer after device creation will invalidate the active device.
    {
        ComPtr<ID3D12Debug> debugController;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
        {
            debugController->EnableDebugLayer();

            // Enable additional debug layers.
            dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
        }
    }
#endif
    ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&m_dxgiFactory)));
    EnumAdapters();

    ThrowIfFailed(
        D3D12CreateDevice(
            m_defaultAdapter.Get(),
            D3D_FEATURE_LEVEL_11_0,
            IID_PPV_ARGS(&m_device))
    );

}

void Direct12Renderer::EnumAdapters()
{
    ComPtr<IDXGIFactory> factory { nullptr };
    m_dxgiFactory->QueryInterface(IID_PPV_ARGS(&factory) );
    UINT index = 0;
    ComPtr<IDXGIAdapter> adapter { nullptr };
    std::wstringstream stream;
    const wchar_t* comment = L"\n*******************************************************************************************************************************************************************************\n\n";
    OutputDebugString(comment);

    while (SUCCEEDED( factory->EnumAdapters(index++, &adapter)) )
    {
        if (adapter)
        {

            DXGI_ADAPTER_DESC desc;
            adapter->GetDesc(&desc);
            stream << "desc: " << desc.Description << ' ' << "\nDevice id: " << desc.DeviceId << ' ' << "\nDedicatedVideoMemory: " << desc.DedicatedVideoMemory << std::endl;
            OutputDebugString(stream.str().c_str());
            adapter->QueryInterface(IID_PPV_ARGS(&m_defaultAdapter));
            break;
        }
    }
    OutputDebugString(comment);

}

