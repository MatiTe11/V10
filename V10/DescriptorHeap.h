#pragma once
#include "stdafx.h"
#include "Graphics.h"

namespace V10
{
	struct DescLocation
	{
		ID3D12DescriptorHeap* descHeap;
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
	};

	class DescriptorHeap
	{
	private:
		Graphics& m_graphics;
		ID3D12DescriptorHeap* m_descHeap;
		UINT m_incrementSize;
		SIZE_T m_nextFreeSlot;
		int m_usedSlots;


	public:
		DescriptorHeap(Graphics& graphics, UINT size);
		~DescriptorHeap();

		DescLocation GetNextDescriptor();

	};
}

