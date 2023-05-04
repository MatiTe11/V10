#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <DirectXMath.h>
#include <memory>
#include <string>
#include "ModelInterface.h"
#include "InputInterface.h"
#include "CameraInterface.h"


namespace V10
{


	class GraphicsInterface
	{

	public:
		virtual void Init(HWND hwnd) = 0;
		virtual void Update() = 0;
		virtual std::shared_ptr<ModelInterface> CreateModel(std::string model_name) = 0;
		virtual std::shared_ptr<ModelInterface> CreateCubeGeometry(std::string tex_name) = 0;
		virtual std::shared_ptr<InputInterface> GetInputInterface() = 0;
		virtual std::shared_ptr<CameraInterface> GetCameraInterface() = 0;

	};

}