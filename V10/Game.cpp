#include "stdafx.h"
#include "Game.h"


Game::Game()
{
	//m_input = new  InputManager(nullptr);
	m_timestamp = std::chrono::high_resolution_clock::now();

}


Game::~Game()
{
}

void Game::MeasureElapsedTime()
{
	auto newTimestamp = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = newTimestamp - m_timestamp;
	m_elapsedTimeSeconds = elapsed.count();
	m_timestamp = newTimestamp;
}

void Game::Update()
{
	MeasureElapsedTime();
	//m_input.Update(1);
	for (auto object : m_gameObjects)
		object->Update(m_elapsedTimeSeconds);
}

void Game::Render()
{
}
