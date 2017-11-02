#pragma once
#include "Renderer.h"
#include "Types.h"
#include "Constants.h"

class Button
{
public:
	Button();
	~Button();
private:
	void Update();
private:
	int x, y;
	int width, height;

	std::string fontID;
	SDL_Rect* textPlaceholder;
	Text myText;
};

