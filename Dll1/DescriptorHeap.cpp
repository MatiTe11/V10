#include "stdafx.h"
#include "DescriptorHeap.h"

namespace V10
{
	DescriptorHeap::DescriptorHeap(Graphics& graphics, UINT size)
		:m_graphics(graphics), m_usedSlots(0)
	{
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.NumDescriptors = size;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

		m_graphics.GetDevice()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_descHeap));
		m_graphics.GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		m_nextFreeSlot = m_descHeap->GetCPUDescriptorHandleForHeapStart().ptr;
		m_incrementSize = m_graphics.GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	DescriptorHeap::~DescriptorHeap()
	{
	}

	DescLocation DescriptorHeap::GetNextDescriptor()
	{
		auto freeSlot = m_nextFreeSlot;
		m_nextFreeSlot += m_incrementSize;
		DescLocation loc{ m_descHeap, m_descHeap->GetCPUDescriptorHandleForHeapStart(), m_descHeap->GetGPUDescriptorHandleForHeapStart() };
		loc.cpuHandle.ptr += m_incrementSize * m_usedSlots;
		loc.gpuHandle.ptr += m_incrementSize * m_usedSlots;
		m_usedSlots++;
		return loc;
	}
}