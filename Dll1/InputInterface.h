#pragma once
namespace V10 {

	enum class Button {UP, DOWN, LEFT, RIGHT, X, Y, A, B};

	class InputInterface
	{
	public:
		virtual bool isKeyPressed(Button b) = 0;
	};
}