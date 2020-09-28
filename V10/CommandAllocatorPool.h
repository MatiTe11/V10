#pragma once
#include "Graphics.h"

class CommandAllocatorPool
{
private:
	Graphics * m_graphics;

	std::queue<ID3D12CommandAllocator*> m_availableCA;
	//std::queue<ID3D12CommandAllocator*> m_inUseCA;
public:
	CommandAllocatorPool(Graphics * graphics);
	~CommandAllocatorPool();

	void SetAvailable(ID3D12CommandAllocator* ca) { m_availableCA.push(ca); }
	ID3D12CommandAllocator * GetAllocator();

private:
	void CreateAllocator();
};

