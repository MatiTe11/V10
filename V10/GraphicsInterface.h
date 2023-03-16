#pragma once
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

namespace V10
{
	class Drawable;
	class CommandAllocatorPool;
	class CommandListPool;
	class CommandQueue;
	class CubeGeometry;
	class Model;

	class GraphicsInterface
	{



	public:
		virtual void Init(HWND hwnd)= 0;
		virtual void Update() = 0;


		virtual ~GraphicsInterface();
	};

}