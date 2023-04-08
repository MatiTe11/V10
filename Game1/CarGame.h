#pragma once
#include "MathLibrary.h"
#include "../Dll1/GraphicsInterface.h"
class CarGame
{
private:
	V10::GraphicsInterface* m_grahpics;
public:
	CarGame(V10::GraphicsInterface* graphics);
	


	void Update(double deltaTime);

};

