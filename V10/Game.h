#pragma once
#include "Graphics.h"
#include "Drawable.h"


class Game
{
private:
	Graphics m_graphics;
public:
	Game();
	~Game();

	void Update(float elapsedSeconds);
	void Render();
};

