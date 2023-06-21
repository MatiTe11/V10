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

	auto house = m_grahpics->CreateModel("house");
	house->ResetTransform();
	house->Scale(2);
	house->Move(DirectX::XMVectorSet(-5, -5, 5, 1));

	auto lamp = m_grahpics->CreateModel("lampNoGlass");
	lamp->ResetTransform();
	//lamp->Scale(2);
	lamp->Move(DirectX::XMVectorSet(0, -5.5, 0, 1));

	auto glass = m_grahpics->CreateModel("glass", V10::Material{ 1,1,1 });
	glass->ResetTransform();
	//lamp->Scale(2);
	glass->Move(DirectX::XMVectorSet(0, -5.5, 0, 1));


	auto brick = m_grahpics->CreateCubeGeometry("cobb");
	brick->ResetTransform();
	brick->Scale(30);
	//brick->Move(DirectX::XMVectorSet(0, -7, 0, 1)); //scale 30 pos -20
	brick->Move(DirectX::XMVectorSet(0, -20, 0, 1)); //scale 30 pos -20

	
	auto sun = m_grahpics->CreateCubeGeometry("sun");
	sun->ResetTransform();
	sun->Scale(0.1);
	sun->Move(DirectX::XMVectorSet(0, 0, 0, 1));
}

void CarGame::Update()
{
	auto start = std::chrono::high_resolution_clock::now();
	float speed = 0.01f;
	static float pos = 0;
	pos += speed;
	m_dragon->ResetTransform();
	m_dragon->Rotate(DirectX::XMVectorSet(0, 1, 0, 0), pos);

	m_dragon->Move(DirectX::XMVectorSet(-30, -1, 5, 1)); //-30


	m_car.Update(m_deltaTime);
	m_car.SetCameraBehind(m_camera);
	m_grahpics->Update();
	m_deltaTime = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(std::chrono::high_resolution_clock::now() - start).count();
}
