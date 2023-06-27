#include "stdafx2.h"
#include "Car.h"

Car::Car(std::shared_ptr<V10::ModelInterface> carModel, std::shared_ptr<V10::InputInterface> input)
	:m_carModel(carModel)
	,m_input(input)
{
	m_carModel->ResetTransform();
	m_carModel->Move(m_vec);
	m_speed = 0;
	m_rotation = 0;
	m_acceleration = 4;
}

void Car::Update(double deltaTime)
{
	using namespace DirectX;
	m_carModel->ResetTransform();
	m_carModel->Rotate(DirectX::XMVectorSet(0, 1, 0, 0), m_rotation);
	m_carModel->Move(m_vec);



	if (m_input->isKeyPressed(V10::Button::LEFT))
		m_rotation -= m_acceleration / 100;
	if (m_input->isKeyPressed(V10::Button::RIGHT))
		m_rotation += m_acceleration / 100;
	if (m_input->isKeyPressed(V10::Button::A))
		m_speed = m_acceleration;
	else if (m_input->isKeyPressed(V10::Button::B))
		m_speed = -m_acceleration;
	else
		m_speed = 0;

	m_vec = XMVectorAdd( XMVectorScale( XMVectorSet(XMScalarSin(m_rotation), 0, XMScalarCos(m_rotation), 0) , m_speed * deltaTime/1000), m_vec);

}

void Car::SetCameraBehind(std::shared_ptr<V10::CameraInterface> cam)
{
	using namespace DirectX;
	static double distance = -10.0f;
	cam->SetEyePosition(XMVectorAdd(XMVectorAdd(m_vec, XMVectorScale(XMVectorSet(sin(m_rotation), 0, cos(m_rotation),0),distance)), XMVectorSet(0, 3, 0, 0)));
	cam->SetLookAt(XMVectorSet(sin(m_rotation), 0, cos(m_rotation), 0));
}
