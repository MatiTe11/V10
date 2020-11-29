#include "stdafx.h"
#include "CommandAllocatorPool.h"

namespace V10
{
	CommandAllocatorPool::CommandAllocatorPool(Graphics* graphics)
		:m_graphics(graphics)
	{
	}


	CommandAllocatorPool::~CommandAllocatorPool()
	{
	}

	ID3D12CommandAllocator* CommandAllocatorPool::GetAllocator()
	{
		if (!m_availableCA.empty())
		{
			auto front = m_availableCA.front();
			m_availableCA.pop();
			return front;
		}
		else
		{
			CreateAllocator();
			return GetAllocator();
		}
	}

	void CommandAllocatorPool::CreateAllocator()
	{
		ID3D12CommandAllocator* ca;
		m_graphics->GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&ca));
		m_availableCA.push(ca);
	}
}