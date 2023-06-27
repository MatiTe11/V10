#include "stdafx2.h"
#include "CarGame.h"
#include <chrono>
#include <iostream>

CarGame::CarGame(V10::GraphicsInterface* graphics) 
	: m_grahpics(graphics)
	, m_car(m_grahpics->CreateModel("car", V10::Material{ 0.2,1,256 }), m_grahpics->GetInputInterface())
	, m_camera(m_grahpics->GetCameraInterface())
{

	auto house = m_grahpics->CreateModel("house");
	house->ResetTransform();
	house->Scale(2);
	house->Move(DirectX::XMVectorSet(-5, -5, 5, 1));

	auto lamp = m_grahpics->CreateModel("lampNoGlass");
	lamp->ResetTransform();
	lamp->Move(DirectX::XMVectorSet(0, -5.5, 0, 1));

	auto glass = m_grahpics->CreateModel("glass", V10::Material{ 1,1,1 });
	glass->ResetTransform();
	glass->Move(DirectX::XMVectorSet(0, -5.5, 0, 1));


	auto brick = m_grahpics->CreateCubeGeometry("cobb");
	brick->ResetTransform();
	brick->Scale(30);
	brick->Move(DirectX::XMVectorSet(0, -20, 0, 1)); //scale 30 pos -20

	
	auto sun = m_grahpics->CreateCubeGeometry("sun");
	sun->ResetTransform();
	sun->Scale(0.1);
	sun->Move(DirectX::XMVectorSet(0, 0, 0, 1));
}

void CarGame::Update()
{
	auto start = std::chrono::high_resolution_clock::now();
	
	m_car.Update(m_deltaTime);
	m_car.SetCameraBehind(m_camera);
	m_grahpics->Update();
	m_deltaTime = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(std::chrono::high_resolution_clock::now() - start).count();
}
