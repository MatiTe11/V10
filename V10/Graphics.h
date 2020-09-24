#pragma once
#include "stdafx.h"
#include "CommandList.h"
#include "CommandQueue.h"

class Drawable;
class CommandAllocatorPool;
class CommandListPool;
class Graphics
{
private:
	static const int frameCount = 2;
	IDXGIFactory* m_factory;
	IDXGIAdapter* m_adapter;
	IDXGISwapChain* m_swapchain;
	ID3D12Device* m_device;
	ID3D12CommandQueue* m_commandqueue;
	ID3D12GraphicsCommandList * m_commandlist[frameCount];
	ID3D12GraphicsCommandList * m_initCommandList;
	ID3D12CommandAllocator* m_commandallocator[frameCount];
	ID3D12DescriptorHeap* m_rtvDescHeap;
	ID3D12Fence * m_fence;
	unsigned int m_fencevalue;
	HANDLE m_event;
	int m_currentBackBuffer;
	ID3D12Resource* m_backBuffer[frameCount];

	Drawable* m_firstObj;
	CommandAllocatorPool* m_allocatorPool;
	CommandListPool* m_commandListPool;

	HRESULT m_result;
	HWND m_hWnd;


public:
	void Init(HWND hwnd);

	void Update();
	void RecordCL(ID3D12GraphicsCommandList* cl, ID3D12CommandAllocator* ca);
	void ExecuteCL(ID3D12GraphicsCommandList* cl);

	ID3D12Device* GetDevice() { return m_device; }
	ID3D12GraphicsCommandList * GetDrawingCommandList();
	ID3D12GraphicsCommandList * GetInitCommandList();
	ID3D12Resource * CreateUploadBuffer(UINT64 size);
	ID3D12Resource * CreateResource(D3D12_RESOURCE_DESC& desc, D3D12_RESOURCE_STATES state);
	void BringBackAllocators(ID3D12Fence * fence, UINT64 value, int numCL, CommandList* commandLists);
	void ResetCommandList(int identifier);
	ID3D12CommandAllocator * GetCommandAllocator();

	static D3D12_RESOURCE_BARRIER GetTransition(ID3D12Resource * res, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter);

	Graphics();
	~Graphics();
};

