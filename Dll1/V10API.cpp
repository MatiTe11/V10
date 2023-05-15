#include "stdafx.h"
#include <utility>
#include <limits.h>
#include "ExportedHeaders/V10API.h"
#include "Graphics.h"

static V10::Graphics* s_graphics = nullptr;

V10::GraphicsInterface * GetGraphics()
{
	return s_graphics ? s_graphics : new V10::Graphics();
}
