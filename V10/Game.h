#pragma once
#include "Graphics.h"
#include "stdafx.h"
#include "Drawable.h"
#include "InputManager.h"
#include "IGameObject.h"


class Game
{
private:
	Graphics m_graphics;
	//InputManager m_input;
	double m_elapsedTimeSeconds;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_timestamp;
	std::vector<IGameObject*> m_gameObjects;

public:
	Game();
	~Game();

	void Update();
	void Render();

private:
	void MeasureElapsedTime();

};

