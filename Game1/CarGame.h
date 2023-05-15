#pragma once
#include "Car.h"
#include "../Dll1/GraphicsInterface.h"
class CarGame
{
private:

	V10::GraphicsInterface* m_grahpics;
	Car m_car;
	std::shared_ptr<V10::ModelInterface> m_dragon;
	std::shared_ptr<V10::CameraInterface> m_camera;
	double m_deltaTime;

public:

	CarGame(V10::GraphicsInterface* graphics);
	void Update(double deltaTime);

};

