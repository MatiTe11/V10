#include "stdafx.h"
//#include "Game.h"
//#include "XboxInputDevice.h"
//
//Game::Game(HWND hwnd)
//	:m_input(new XboxInputDevice()), m_graphics(),m_elapsedTimeSeconds(0)
//{
//	m_timestamp = std::chrono::high_resolution_clock::now();
//	m_graphics.Init(hwnd);
//}
//
//
//Game::~Game()
//{
//}
//
//void Game::MeasureElapsedTime()
//{
//	auto newTimestamp = std::chrono::high_resolution_clock::now();
//	std::chrono::duration<double> elapsed = newTimestamp - m_timestamp;
//	m_elapsedTimeSeconds = elapsed.count();
//	m_timestamp = newTimestamp;
//}
//
//void Game::Update()
//{
//	MeasureElapsedTime();
//	m_input.Update(m_elapsedTimeSeconds);
//	for (auto object : m_gameObjects)
//		object->Update(m_elapsedTimeSeconds);
//}
//
//void Game::Render()
//{
//}
