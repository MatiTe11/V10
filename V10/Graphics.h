#pragma once
#include "stdafx.h"
#include "CommandList.h"
#include "Camera.h"

class Drawable;
class CommandAllocatorPool;
class CommandListPool;
class CommandQueue;

class Graphics
{
private:
	static const int frameCount = 2;
	IDXGIFactory* m_factory;
	IDXGIAdapter* m_adapter;
	IDXGISwapChain* m_swapchain;
	ID3D12Device* m_device;
	ID3D12DescriptorHeap* m_rtvDescHeap;
	ID3D12Resource* m_backBuffer[frameCount];
	int m_currentBackBuffer;

	std::unique_ptr<CommandQueue> m_commandQueue;
	std::unique_ptr<CommandAllocatorPool> m_allocatorPool;
	std::unique_ptr<CommandListPool> m_commandListPool;
	std::unique_ptr<Drawable> m_firstObj;
	std::unique_ptr<Drawable> m_grass;
	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<InputManager> m_inputManager;

	HRESULT m_result;
	HWND m_hWnd;


public:
	void Init(HWND hwnd);

	void Update();
	void RecordCL(ID3D12GraphicsCommandList* cl);
	void Execute(CommandList * cl);

	ID3D12Device* GetDevice() { return m_device; }
	CommandList * GetCommandList();
	ID3D12Resource * CreateUploadBuffer(UINT64 size);
	ID3D12Resource * CreateResource(D3D12_RESOURCE_DESC& desc, D3D12_RESOURCE_STATES state);
	void BringBackAllocators(ID3D12Fence * fence, UINT64 value, int numCL, CommandList* commandLists);
	void ResetCommandList(int identifier);
	ID3D12CommandAllocator * GetCommandAllocator();

	static D3D12_RESOURCE_BARRIER GetTransition(ID3D12Resource * res, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter);

	Graphics();
	~Graphics();
};

