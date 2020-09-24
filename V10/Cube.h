#pragma once
#include "IGameObject.h"
class Cube : IGameObject
{
private:

public:
	Cube();
	~Cube();

	// Inherited via IGameObject
	virtual void Update(double elapsedTimeSeconds) override;
};

