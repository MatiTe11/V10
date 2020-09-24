#pragma once

class Graphics;

class CommandList
{
	Graphics * m_graphics;
	ID3D12GraphicsCommandList* m_commandList;
	ID3D12CommandAllocator * m_associatedCommandAllocator;
	int m_identifier;

public:
	CommandList(int identifier, ID3D12GraphicsCommandList* cl);
	~CommandList();
	ID3D12GraphicsCommandList* GetCommandList() { return m_commandList; }
	ID3D12CommandAllocator* GetAssociatedCommandAllocator() { return m_associatedCommandAllocator; }
	void Reset(ID3D12CommandAllocator* ca);
};

