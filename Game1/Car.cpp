#include "stdafx2.h"
#include "Car.h"

Car::Car(std::shared_ptr<V10::ModelInterface> carModel, std::shared_ptr<V10::InputInterface> input)
	:m_carModel(carModel)
	,m_input(input)
{
	m_carModel->ResetTransform();
	m_carModel->Move(DirectX::XMVectorSet(3, -1, 5, 1));
	m_speed = 0;
	m_rotation = 0;


}

void Car::Update(double deltaTime)
{
	using namespace DirectX;
	//m_rotation += m_speed;
	m_carModel->ResetTransform();
	m_carModel->Rotate(DirectX::XMVectorSet(0, 1, 0, 0), m_rotation);
	m_carModel->Move(m_vec);



	if (m_input->isKeyPressed(V10::Button::LEFT))
		m_rotation -= 0.01;
	if (m_input->isKeyPressed(V10::Button::RIGHT))
		m_rotation += 0.01;
	if (m_input->isKeyPressed(V10::Button::A))
		m_speed = 0.01;
	else if (m_input->isKeyPressed(V10::Button::B))
		m_speed = -0.01;
	else
		m_speed = 0;

	m_vec = XMVectorAdd( XMVectorScale( XMVectorSet(XMScalarSin(m_rotation), 0, XMScalarCos(m_rotation), 0) , m_speed), m_vec);

	//m_carModel->Rotate(DirectX::XMVectorSet(0, 1, 0, 0), 0.01);


}
