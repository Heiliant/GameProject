#include <SDL.h>		// Always needs to be included for an SDL app
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <time.h>
#include <iostream>

const int _WIDTH = 640;
const int _HEIGHT = 480;

int main(int, char*[]) {
	SDL_Init(SDL_INIT_EVERYTHING);
	//STARTUP
	SDL_Window* myWindow;
	myWindow = SDL_CreateWindow("Gold Digger", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _WIDTH, _HEIGHT, SDL_WINDOW_SHOWN);

	SDL_Renderer* myRenderer;
	myRenderer = SDL_CreateRenderer(myWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	SDL_Texture* menuTexture;
	menuTexture = IMG_LoadTexture(myRenderer, "../../res/img/bg.jpg");
	SDL_Rect menuTextureRect{ 0, 0, _WIDTH, _HEIGHT };

	SDL_Texture* bgTexture;
	bgTexture = IMG_LoadTexture(myRenderer, "../../res/img/bgCastle.jpg");
	SDL_Rect bgTextureRect{ 0, 0, _WIDTH, _HEIGHT };

	//EVENTOS
	SDL_Event evnt;
	bool isActive=true;
	while (isActive) {
		while (SDL_PollEvent(&evnt)) {
			switch (evnt.type) {
			case SDL_QUIT: isActive = false;
				break;
			}
		}
		//UPDATE

		//DRAW
		SDL_RenderClear(myRenderer);

		SDL_RenderCopy(myRenderer, menuTexture, nullptr, &menuTextureRect);
		SDL_RenderPresent(myRenderer);
	}

	return 0;
}