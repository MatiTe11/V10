#include "stdafx.h"
#include "CommandQueue.h"
#include "Graphics.h"


CommandQueue::CommandQueue()
{
	m_fenceValue = 0;
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
