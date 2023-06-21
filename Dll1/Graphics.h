#pragma once
#include "stdafx.h"
#include "CommandList.h"
#include "InputInterface.h"
#include "GraphicsInterface.h"

namespace V10
{
	class DrawExecutor;
	class CommandAllocatorPool;
	class CommandListPool;
	class CommandQueue;
	class CubeGeometry;
	class Model;
	class InputManager;
	class Camera;
	class XboxInputDevice;

	class Graphics : public GraphicsInterface
	{
	private:
		static const int frameCount = 2;
		IDXGIFactory* m_factory;
		IDXGIAdapter* m_adapter;
		IDXGISwapChain* m_swapchain;
		ID3D12Device* m_device;
		ID3D12DescriptorHeap* m_rtvDescHeap;
		ID3D12Resource* m_backBuffer[frameCount];
		ID3D12Resource* m_depthStencilBuffer;
		ID3D12DescriptorHeap* m_dsDescriptorHeap;
		int m_currentBackBuffer;
		D3D12_VIEWPORT m_viewport;
		D3D12_RECT m_scissorRect;


		std::unique_ptr<CommandQueue> m_commandQueue;
		std::unique_ptr<CommandAllocatorPool> m_allocatorPool;
		std::unique_ptr<CommandListPool> m_commandListPool;
		std::unique_ptr<DrawExecutor> m_drawExecNormalMap;
		std::unique_ptr<DrawExecutor> m_drawExecNoNormal;
		std::shared_ptr<Camera> m_camera;
		std::shared_ptr<XboxInputDevice> m_inputInterface;

		HRESULT m_result;
		HWND m_hWnd;


	public:
		void Init(HWND hwnd) override;
		void Update() override;
		std::shared_ptr<ModelInterface> CreateModel(std::string model_name, Material material = Material{ 0.1,1,1 }) override;
		std::shared_ptr<ModelInterface> CreateCubeGeometry(std::string tex_name) override;
		std::shared_ptr<InputInterface> GetInputInterface() override;
		virtual std::shared_ptr<CameraInterface> GetCameraInterface() override;

		enum class CLcalls {None, Begin, End};
		void RecordCL(ID3D12GraphicsCommandList* cl, DrawExecutor* drawExecutor, CLcalls calls = CLcalls::None);
		void Execute(CommandList* cl);

		ID3D12Device* GetDevice() { return m_device; }
		CommandList* GetCommandList();
		ID3D12Resource* CreateUploadBuffer(UINT64 size);
		ID3D12Resource* CreateResource(D3D12_RESOURCE_DESC& desc, D3D12_RESOURCE_STATES state);
		void BringBackAllocators(ID3D12Fence* fence, UINT64 value, int numCL, CommandList** commandLists);
		void ResetCommandList(int identifier);
		ID3D12CommandAllocator* GetCommandAllocator();

		static D3D12_RESOURCE_BARRIER GetTransition(ID3D12Resource* res, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter);

		Graphics();
		~Graphics();
	};

}