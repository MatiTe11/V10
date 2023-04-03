#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <DirectXMath.h>
#include <memory>
#include <string>
#include "ModelInterface.h"


namespace V10
{


	class GraphicsInterface
	{

	public:
		virtual void Init(HWND hwnd) = 0;
		virtual void Update() = 0;
		virtual std::shared_ptr<ModelInterface> CreateModel(std::string model_name) = 0;

	};

}