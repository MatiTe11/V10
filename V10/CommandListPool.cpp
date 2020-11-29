#include "stdafx.h"
#include "CommandListPool.h"
#include "Graphics.h"

namespace V10
{
	CommandListPool::CommandListPool(Graphics* graphics)
		:m_graphics(graphics)
	{
	}


	CommandListPool::~CommandListPool()
	{
	}

	CommandList* CommandListPool::GetCommandList()
	{
		if (!m_availableIndexes.empty())
		{
			auto front = m_availableIndexes.front();
			m_availableIndexes.pop();
			return m_commandLists[front];
		}
		else
		{
			AddCommandList();
			return GetCommandList();
		}
	}

	void CommandListPool::MakeAvailable(int index)
	{
		m_availableIndexes.push(index); //TODO: kolejka to chyba nie najlepsza struktura
	}

	void CommandListPool::AddCommandList()
	{
		ID3D12GraphicsCommandList* cl;
		m_graphics->GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_graphics->GetCommandAllocator(), NULL, IID_PPV_ARGS(&cl));
		cl->Close();
		MakeAvailable(m_commandLists.size());
		CommandList* commandList = new CommandList(m_commandLists.size(), cl, m_graphics);
		m_commandLists.push_back(commandList);
	}
}