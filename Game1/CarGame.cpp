#include "stdafx2.h"
#include "CarGame.h"

CarGame::CarGame(V10::GraphicsInterface* graphics) 
	: m_grahpics(graphics)
{
	auto dragon = m_grahpics->CreateModel("dragon");
	dragon->Move(DirectX::XMVectorSet(0, -1, 5, 1));
	auto backpack = m_grahpics->CreateModel("backpack");
	backpack->Move(DirectX::XMVectorSet(0, 0, -10, 1));
}

void CarGame::Update(double deltaTime)
{
	m_grahpics->Update();
}
