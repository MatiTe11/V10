#pragma once
#include "../GraphicsInterface.h"

#ifdef MATHLIBRARY_EXPORTS
#define V10_API __declspec(dllexport)
#else
#define V10_API __declspec(dllimport)
#endif


extern "C" V10_API V10::GraphicsInterface* GetGraphics();
