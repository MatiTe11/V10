#pragma once
#include <memory>
#include "../Dll1/ModelInterface.h"
#include "../Dll1/InputInterface.h"

class Car
{
private:
	struct Position {
		double x;
		double y;
	} m_position;

	std::shared_ptr<V10::ModelInterface> m_carModel;
	std::shared_ptr<V10::InputInterface> m_input;
	DirectX::XMVECTOR m_vec = DirectX::XMVectorSet(0, 0, 0, 0);;

public:
	Car(std::shared_ptr<V10::ModelInterface> carModel, std::shared_ptr<V10::InputInterface> input);
	void Update(double deltaTime);
};

