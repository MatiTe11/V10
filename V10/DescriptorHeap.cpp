#include "stdafx.h"
#include "DescriptorHeap.h"

namespace V10
{
	DescriptorHeap::DescriptorHeap(Graphics& graphics, UINT size)
		:m_graphics(graphics)
	{
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.NumDescriptors = size;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

		m_graphics.GetDevice()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_descHeap));
		m_graphics.GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		m_nextFreeSlot = m_descHeap->GetCPUDescriptorHandleForHeapStart().ptr;
	}

	DescriptorHeap::~DescriptorHeap()
	{
	}

	DescLocation DescriptorHeap::GetNextDescriptor()
	{
		auto freeSlot = m_nextFreeSlot;
		m_nextFreeSlot += m_incrementSize;
		///TODO: kompletnie zle, dziala tylko dla jednego descriptora!!!
		return DescLocation{ m_descHeap, m_descHeap->GetCPUDescriptorHandleForHeapStart(), m_descHeap->GetGPUDescriptorHandleForHeapStart() };
	}
}