#pragma once

class IGameObject
{
public:

	virtual void Update(double elapsedTimeSeconds) = 0;
};