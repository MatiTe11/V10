#include "stdafx2.h"
#include "Car.h"

Car::Car(std::shared_ptr<V10::ModelInterface> carModel, std::shared_ptr<V10::InputInterface> input)
	:m_carModel(carModel)
	,m_input(input)
{
	m_carModel->ResetTransform();
	m_carModel->Move(DirectX::XMVectorSet(3, -1, 5, 1));

}

void Car::Update(double deltaTime)
{
	float speed = 0.01f;
	static float pos = 0;
	pos += speed;
	m_carModel->ResetTransform();
	m_carModel->Rotate(DirectX::XMVectorSet(0, 1, 0, 0), pos);
	m_carModel->Move(DirectX::XMVectorSet(3, -1, 5, 1));



	if(m_input->isKeyPressed(V10::Button::X))
		m_carModel->Move(DirectX::XMVectorSet(3, pos, 5, 1));
	//m_carModel->Rotate(DirectX::XMVectorSet(0, 1, 0, 0), 0.01);


}
