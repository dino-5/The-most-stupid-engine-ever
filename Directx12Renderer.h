#include"commin.h"
#include"CommandList.h"
#include"CommandAllocator.h"

constexpr UINT m_frameCount=2;

class Direct12Renderer
{
private:
    struct Vertex
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT4 color;
    };

    UINT m_width, m_height;
    UINT m_frameIndex=0;
    float m_aspectRatio = 16/9;

    UINT64 m_fenceValue;

    UINT m_rtvDescSize, m_dsvDescSize, m_cbvSrvDescSize;

    ComPtr<IDXGIFactory2> m_dxgiFactory;
    ComPtr<IDXGIAdapter> m_defaultAdapter;
    ComPtr<ID3D12Device> m_device;
    ComPtr<IDXGISwapChain3> m_swapChain;
    ComPtr<ID3D12Fence> m_fence;
    ComPtr<ID3D12CommandQueue> m_commandQueue;
    CommandAllocator m_commandAllocator;
    ComPtr<ID3D12DescriptorHeap> m_RTVDescHeap;
    ComPtr<ID3D12Resource> m_renderTargets[m_frameCount];
    ComPtr<ID3D12RootSignature> m_rootSignature;
    ComPtr<ID3D12PipelineState> m_pso;
    CommandList<ID3D12GraphicsCommandList> m_commandList;
    CD3DX12_VIEWPORT m_viewport;
    CD3DX12_RECT m_scissorRect;


    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
    ComPtr<ID3D12Resource> m_vertexBuffer;
    D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
    ComPtr<ID3D12Resource> m_indexBuffer;
    HANDLE m_fenceEvent;
    
private:
    void PopulateCommandList();
    void CreateDevice();
    void GetDescSize();
    void CreateQueue();
    void CreateSwapChain();
    void CreateDescriptors();
    void CreateRTV();
    void CreateCommandAllocator();
    void CreateRootSignature();
    void LoadShaders();
    void WaitForPreviousFrame();
    void EnumAdapters();
	void CreateResource(GraphicsCommandList cl, ComPtr<ID3D12Resource>& res, const void* data, UINT dataSize);

    
    Direct12Renderer(UINT width, UINT height);
public:
    Direct12Renderer() {
        *this = GetInstance();
    }
    static Direct12Renderer& GetInstance();
    
    void Init(UINT width, UINT height);
    void Draw();
    ~Direct12Renderer() = default;
};