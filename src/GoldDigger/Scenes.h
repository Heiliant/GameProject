#pragma once
#include "Renderer.h"

class Scenes
{
public:
	Scenes();
	~Scenes();

	enum class sceneTypes { _menu, _play, _exit };

protected:
	//cosas en com�n en todas las escenas
	sceneTypes type;
	SDL_Rect bgRect;
	SDL_Texture* bgTexture;
	int width, height;
	
};


