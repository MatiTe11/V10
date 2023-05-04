#pragma once
#include <memory>
#include "../Dll1/ModelInterface.h"
#include "../Dll1/InputInterface.h"
#include "../Dll1/CameraInterface.h"

class Car
{
private:
	struct Position {
		double x;
		double y;
	} m_position;

	double m_speed;
	double m_rotation;

	std::shared_ptr<V10::ModelInterface> m_carModel;
	std::shared_ptr<V10::InputInterface> m_input;
	DirectX::XMVECTOR m_vec = DirectX::XMVectorSet(3, -1, 5, 1);;

public:
	Car(std::shared_ptr<V10::ModelInterface> carModel, std::shared_ptr<V10::InputInterface> input);
	void Update(double deltaTime);
	void SetCameraBehind(std::shared_ptr<V10::CameraInterface> cam);
};

