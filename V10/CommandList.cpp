#include "stdafx.h"
#include "CommandList.h"
#include "Graphics.h"


CommandList::CommandList(int identifier, ID3D12GraphicsCommandList* cl)
	:m_identifier(identifier), m_commandList(cl)
{
}


CommandList::~CommandList()
{
}

void CommandList::Reset(ID3D12CommandAllocator * ca)
{
	m_associatedCommandAllocator = ca;
	m_graphics->ResetCommandList(m_identifier);
	//TODO: powinno równie¿ przywróciæ do commandallocator poola poprzedni associated ca
}
