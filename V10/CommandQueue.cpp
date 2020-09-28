#include "stdafx.h"
#include "CommandQueue.h"
#include "Graphics.h"


CommandQueue::CommandQueue(Graphics * graphics)
	:m_graphics(graphics)
{
	m_fenceValue = 0;

	D3D12_COMMAND_QUEUE_DESC cqDesc{};
	cqDesc.NodeMask = 0;
	cqDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	cqDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
	cqDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	m_graphics->GetDevice()->CreateCommandQueue(&cqDesc, IID_PPV_ARGS(&m_commandQueue));
	m_graphics->GetDevice()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
}


CommandQueue::~CommandQueue()
{
}

void CommandQueue::Execute(int num, CommandList * commandLists)
{
	ID3D12CommandList** cls = new ID3D12CommandList*[num];

	for (size_t i = 0; i < num; i++)
	{
		commandLists[i].GetCommandList()->QueryInterface(__uuidof(ID3D12CommandList), (void**)&cls[i]);
	}

	m_commandQueue->ExecuteCommandLists(num, cls);
	m_fenceValue++;
	m_commandQueue->Signal(m_fence, m_fenceValue);
	m_graphics->BringBackAllocators(m_fence, m_fenceValue, num, commandLists);

}

void CommandQueue::Sync()
{
	ResetEvent(m_event);
	m_fenceValue++;
	m_commandQueue->Signal(m_fence, m_fenceValue);
	m_fence->SetEventOnCompletion(m_fenceValue, m_event);
	WaitForSingleObject(m_event, INFINITE);
}
