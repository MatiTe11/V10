#include "stdafx2.h"
#include "CarGame.h"

CarGame::CarGame(V10::GraphicsInterface* graphics) 
	: m_grahpics(graphics)
	, m_car(m_grahpics->CreateModel("car"), m_grahpics->GetInputInterface())
{
	m_dragon = m_grahpics->CreateModel("dragon");
	float speed = 0.01f;
	static float pos = 0;
	pos += speed;
	m_dragon->ResetTransform();
	m_dragon->Rotate(DirectX::XMVectorSet(0, 1, 0, 0), pos);

	m_dragon->Move(DirectX::XMVectorSet(-3, -1, 5, 1));
	auto backpack = m_grahpics->CreateModel("backpack");
	backpack->ResetTransform();
	backpack->Move(DirectX::XMVectorSet(0, 0, -10, 1));
}

void CarGame::Update(double deltaTime)
{
	float speed = 0.01f;
	static float pos = 0;
	pos += speed;
	m_dragon->ResetTransform();
	m_dragon->Rotate(DirectX::XMVectorSet(0, 1, 0, 0), pos);

	m_dragon->Move(DirectX::XMVectorSet(-3, -1, 5, 1));


	m_car.Update(deltaTime);
	m_grahpics->Update();
}
