#include "stdafx.h"
#include "Graphics.h"
#include "Drawable.h"
#include "CommandAllocatorPool.h"
#include "CommandListPool.h"
#include "CommandQueue.h"
#include "DumbInputDevice.h"
#include "XboxInputDevice.h"
#include "Model.h"

namespace V10
{
	void Graphics::Init(HWND hwnd)
	{
		m_commandListPool = std::make_unique<CommandListPool>(this);
		m_allocatorPool = std::make_unique<CommandAllocatorPool>(this);

		ID3D12Debug* debugController;
		D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));

		debugController->EnableDebugLayer();
		CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&m_factory));
		m_factory->EnumAdapters(0, &m_adapter);
		D3D12CreateDevice(m_adapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_device));

		m_commandQueue = std::make_unique<CommandQueue>(this);

		DXGI_SWAP_CHAIN_DESC scDesc{};
		scDesc.BufferCount = 2;
		scDesc.BufferDesc = { 1920,1080,{60,1},DXGI_FORMAT_R16G16B16A16_FLOAT,DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE,DXGI_MODE_SCALING_CENTERED };
		scDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
		scDesc.SampleDesc = { 1,0 };
		scDesc.Windowed = true;
		scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		m_hWnd = hwnd;
		scDesc.OutputWindow = m_hWnd;
		m_factory->CreateSwapChain(m_commandQueue->Get(), &scDesc, &m_swapchain);
		m_currentBackBuffer = 0;

		D3D12_DESCRIPTOR_HEAP_DESC rtvDesc;
		rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvDesc.NumDescriptors = frameCount;
		rtvDesc.NodeMask = 0;
		rtvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		m_device->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&m_rtvDescHeap));
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvDescHeap->GetCPUDescriptorHandleForHeapStart());

		for (size_t i = 0; i < frameCount; i++)
		{
			m_swapchain->GetBuffer(i, IID_PPV_ARGS(&m_backBuffer[i]));
			m_device->CreateRenderTargetView(m_backBuffer[i], NULL, rtvHandle);
			rtvHandle.Offset(1, m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
		}

		m_drawable = std::make_unique<Drawable>(*this);
		m_firstObj = std::make_unique<Model>(*this);
		m_firstObj->Move(DirectX::XMVectorSet(2, 0, 0, 1));
		m_grass = std::make_unique<Model>(*this);
		m_grass->Move(DirectX::XMVectorSet(-2, 0, 0, 1));
		m_drawable->PushDrawableObject(m_firstObj.get());
		m_drawable->PushDrawableObject(m_grass.get());
		m_camera = std::make_unique<Camera>();
		m_inputManager = std::make_unique<InputManager>(new XboxInputDevice()); //TODO:: mem leak
	}


	void Graphics::Update()
	{
		static int cnt = 0;
		cnt++;
		m_firstObj->Update(cnt);
		m_inputManager->Update(1);
		m_camera->Update(m_inputManager.get());

		CommandList* cl = GetCommandList();
		RecordCL(cl->GetCommandList());
		Execute(cl);
		m_swapchain->Present(1, 0);
		m_commandQueue->Sync();
		m_currentBackBuffer++;
		m_currentBackBuffer = m_currentBackBuffer % 2;
	}

	void Graphics::RecordCL(ID3D12GraphicsCommandList* cl)
	{
		D3D12_VIEWPORT viewport;
		// Fill out the Viewport
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = 1920;
		viewport.Height = 1080;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		// Fill out a scissor rect
		D3D12_RECT scissorRect;
		scissorRect.left = 0;
		scissorRect.top = 0;
		scissorRect.right = 1920;
		scissorRect.bottom = 1020;

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvDescHeap->GetCPUDescriptorHandleForHeapStart());
		rtvHandle.Offset(m_currentBackBuffer, m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
		float color[4] = { 1,1,0,1 };
		auto barrier = GetTransition(m_backBuffer[m_currentBackBuffer], D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_RENDER_TARGET);
		cl->ResourceBarrier(1, &barrier);
		cl->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
		cl->ClearRenderTargetView(rtvHandle, color, 0, NULL);

		cl->RSSetScissorRects(1, &scissorRect);
		cl->RSSetViewports(1, &viewport);
		m_drawable->Draw(cl, m_camera.get());
		barrier = GetTransition(m_backBuffer[m_currentBackBuffer], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COMMON);
		cl->ResourceBarrier(1, &barrier);
		cl->Close();
	}


	void Graphics::Execute(CommandList* cl)
	{
		m_commandQueue->Execute(1, cl);
	}


	CommandList* Graphics::GetCommandList()
	{
		auto cl = m_commandListPool->GetCommandList();
		auto allocator = m_allocatorPool->GetAllocator();
		allocator->Reset();
		cl->Reset(allocator);
		return cl;
	}

	ID3D12Resource* Graphics::CreateUploadBuffer(UINT64 size)
	{
		ID3D12Resource* buffer;
		m_device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // upload heap
			D3D12_HEAP_FLAG_NONE, // no flags
			&CD3DX12_RESOURCE_DESC::Buffer(size), // resource description for a buffer
			D3D12_RESOURCE_STATE_GENERIC_READ, // GPU will read from this buffer and copy its contents to the default heap
			nullptr,
			IID_PPV_ARGS(&buffer));

		return buffer;
	}

	ID3D12Resource* Graphics::CreateResource(D3D12_RESOURCE_DESC& desc, D3D12_RESOURCE_STATES state)
	{
		ID3D12Resource* resource;

		m_device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), // a default heap
			D3D12_HEAP_FLAG_NONE, // no flags
			&desc, // resource description for a buffer
			D3D12_RESOURCE_STATE_COPY_DEST, // we will start this heap in the copy destination state since we will copy data
			nullptr, // optimized clear value must be null for this type of resource. used for render targets and depth/stencil buffers
			IID_PPV_ARGS(&resource));

		return resource;
	}

	void Graphics::BringBackAllocators(ID3D12Fence* fence, UINT64 value, int numCL, CommandList* commandLists)
	{
		auto finishEvent = CreateEventA(NULL, FALSE, FALSE, NULL);
		fence->SetEventOnCompletion(value, finishEvent);
		WaitForSingleObject(finishEvent, INFINITE);
		for (size_t i = 0; i < numCL; i++)
		{
			m_allocatorPool->SetAvailable(commandLists[i].GetAssociatedCommandAllocator());
		}
		CloseHandle(finishEvent);
	}

	void Graphics::ResetCommandList(int identifier)
	{
		m_commandListPool->MakeAvailable(identifier); //TODO: Ÿle powinno byæ po execute a nie reset
	}

	ID3D12CommandAllocator* Graphics::GetCommandAllocator()
	{
		return m_allocatorPool->GetAllocator();
	}

	D3D12_RESOURCE_BARRIER Graphics::GetTransition(ID3D12Resource* res, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter)
	{
		D3D12_RESOURCE_BARRIER bar{};
		bar.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		D3D12_RESOURCE_TRANSITION_BARRIER a{};
		a.pResource = res;
		a.StateBefore = stateBefore;
		a.StateAfter = stateAfter;
		a.Subresource = 0;
		bar.Transition = a;
		return bar;
	}

	Graphics::Graphics()
	{
	}


	Graphics::~Graphics()
	{
	}
}