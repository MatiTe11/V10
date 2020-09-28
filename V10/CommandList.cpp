#include "stdafx.h"
#include "CommandList.h"
#include "Graphics.h"


CommandList::CommandList(int identifier, ID3D12GraphicsCommandList* cl, Graphics * graphics)
	:m_identifier(identifier), m_commandList(cl), m_graphics(graphics)
{
}


CommandList::~CommandList()
{
}

void CommandList::Reset(ID3D12CommandAllocator * ca)
{
	m_commandList->Reset(ca, nullptr);
	m_associatedCommandAllocator = ca;
	m_graphics->ResetCommandList(m_identifier);
	//TODO: powinno równie¿ przywróciæ do commandallocator poola poprzedni associated ca
}
