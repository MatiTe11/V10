#pragma once
#include "CommandList.h"

namespace V10
{
	class Graphics;

	class CommandQueue
	{
	private:
		Graphics* m_graphics;
		ID3D12CommandQueue* m_commandQueue;
		ID3D12Fence* m_fence;
		UINT64 m_fenceValue;
		HANDLE m_event;

	public:
		CommandQueue(Graphics* graphics);
		~CommandQueue();
		void Execute(int num, CommandList* commandLists);
		void Sync();
		ID3D12CommandQueue* Get() { return m_commandQueue; }
	};

}