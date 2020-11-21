// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <vector>
#include <tchar.h>
#include "d3d12.h"
#include "dxgi1_3.h"
#include "d3dx12.h"
#include <DirectXMath.h>
#include <Xinput.h>
#include <fstream>
#include <wrl.h>
#include <atlbase.h>
#include <chrono>
#include <queue>
#include <set>
#include <filesystem>


struct SimpleVertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 texCoord;

	static D3D12_INPUT_LAYOUT_DESC GetLayout() 
	{
		
		static D3D12_INPUT_ELEMENT_DESC inputElement[] = { { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
														   { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 } };
		static D3D12_INPUT_LAYOUT_DESC  inputLayoutDesc;
		inputLayoutDesc.NumElements = 2;
		inputLayoutDesc.pInputElementDescs = inputElement;

		return inputLayoutDesc;
	}
};

// reference additional headers your program requires here
