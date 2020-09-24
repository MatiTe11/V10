#pragma once
#include "stdafx.h"
#include "CommandList.h"
class CommandListPool
{
private:
	std::queue<CommandList> m_AvailableCommandLists;

public:
	CommandListPool();
	~CommandListPool();
};

