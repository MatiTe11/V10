#pragma once
#include "stdafx.h"
#include "CommandList.h"

namespace V10
{
	class Graphics;

	class CommandListPool
	{
	private:
		Graphics* m_graphics;
		std::vector<CommandList*> m_commandLists;
		std::queue<int> m_availableIndexes;

	public:
		CommandListPool(Graphics* graphics);
		~CommandListPool();
		CommandList* GetCommandList();
		void MakeAvailable(int index);

	private:
		void AddCommandList();
	};

}