#include "stdafx2.h"
#include "CarGame.h"
#include <chrono>
#include <iostream>

CarGame::CarGame(V10::GraphicsInterface* graphics) 
	: m_grahpics(graphics)
	, m_car(m_grahpics->CreateModel("car"), m_grahpics->GetInputInterface())
	, m_camera(m_grahpics->GetCameraInterface())
{
	float speed = 0.01f;
	static float pos = 0;
	pos += speed;

	m_dragon = m_grahpics->CreateModel("dragon");
	m_dragon->ResetTransform();
	m_dragon->Rotate(DirectX::XMVectorSet(0, 1, 0, 0), pos);
	m_dragon->Move(DirectX::XMVectorSet(-3, -1, 5, 1));
	auto backpack = m_grahpics->CreateModel("backpack");
	backpack->ResetTransform();
	backpack->Move(DirectX::XMVectorSet(0, 0, -10, 1));

	auto brick = m_grahpics->CreateCubeGeometry("tex");
	brick->ResetTransform();
	brick->Move(DirectX::XMVectorSet(3, -5, 5, 1));
}

void CarGame::Update(double deltaTime)
{
	auto start = std::chrono::high_resolution_clock::now();
	float speed = 0.01f;
	static float pos = 0;
	pos += speed;
	m_dragon->ResetTransform();
	m_dragon->Rotate(DirectX::XMVectorSet(0, 1, 0, 0), pos);

	m_dragon->Move(DirectX::XMVectorSet(-3, -1, 5, 1));


	m_car.Update(deltaTime);
	m_car.SetCameraBehind(m_camera);
	m_grahpics->Update();
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << duration.count() << std::endl;
}
