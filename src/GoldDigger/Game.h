#pragma once
#include "Renderer.h"
#include "Scenes.h"

class Game
{
public:
	Game();
	~Game();

public:
	Scenes* activeScene;

};

