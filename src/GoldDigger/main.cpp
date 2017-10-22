#include <SDL.h>		// Always needs to be included for an SDL app
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <time.h>
#include <iostream>

const int _WIDTH = 640;
const int _HEIGHT = 480;
const int _PJSPEED = 5;

enum class scenes
{
	_menu, _game, _MAX
};

enum class mousButt {
	_right, _left, _mid, _none, _MAX
};

struct coord {
	int x;
	int y;
};

coord getCenter(SDL_Rect dunno) {
	int one, two;
	one = dunno.x + dunno.w / 2;
	two = dunno.y + dunno.h / 2;
	return (coord { one, two });
}

int main(int, char*[]) {
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	//STARTUP
		//window
	SDL_Window* myWindow;
	myWindow = SDL_CreateWindow("Gold Saucer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _WIDTH, _HEIGHT, SDL_WINDOW_SHOWN);
		//renderer
	SDL_Renderer* myRenderer;
	myRenderer = SDL_CreateRenderer(myWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		//images
	SDL_Texture* menuTexture;
	menuTexture = IMG_LoadTexture(myRenderer, "../../res/img/bg.jpg");
	SDL_Rect menuTextureRect{ 0, 0, _WIDTH, _HEIGHT };

	SDL_Texture* bgTexture;
	bgTexture = IMG_LoadTexture(myRenderer, "../../res/img/bgCastle.jpg");
	SDL_Rect bgTextureRect{ 0, 0, _WIDTH, _HEIGHT };

		//text
	TTF_Font* saiyanFont;
	saiyanFont = TTF_OpenFont("../../res/ttf/saiyan.ttf", 80);

	SDL_Surface *mySurface;
	SDL_Colour playColour{ 0, 255, 0, 255 };
	mySurface = TTF_RenderText_Blended(saiyanFont, "Play", playColour);
	SDL_Texture *playTexture = SDL_CreateTextureFromSurface(myRenderer, mySurface);
	SDL_Rect playRect{ _WIDTH / 2-mySurface->w/2, _HEIGHT / 4 , mySurface->w, mySurface->h };
	SDL_FreeSurface(mySurface);

	SDL_Colour exitColour{ 255, 0, 0, 255 };
	mySurface = TTF_RenderText_Blended(saiyanFont, "Exit", exitColour);
	SDL_Texture *exitTexture = SDL_CreateTextureFromSurface(myRenderer, mySurface);
	SDL_Rect exitRect{ _WIDTH / 2 - mySurface->w / 2, _HEIGHT /2, mySurface->w, mySurface->h };
	SDL_FreeSurface(mySurface);

	//colliders

	coord pjPos{ 0, (int)(_HEIGHT*0.7) };
	
	SDL_Rect pjRect{ pjPos.x, pjPos.y, 50, 50 };

	SDL_Rect skylineRect{ 0, 0, _WIDTH, 140 };


	SDL_Event evnt;
	bool isActive=true;
	mousButt button=mousButt::_none;
	scenes myScene = scenes::_menu;

	while (isActive) {
		switch (myScene) {
		case scenes::_menu:
			while (SDL_PollEvent(&evnt)) {
				switch (evnt.type) {
				case SDL_QUIT: isActive = false;
					break;
				case SDL_MOUSEBUTTONDOWN:
					switch (evnt.button.button) { //quin botó es el que s'ha premut?
					case SDL_BUTTON_RIGHT: button = mousButt::_right;
						break;
					case SDL_BUTTON_LEFT: button = mousButt::_left;
						break;
					case SDL_BUTTON_MIDDLE: button = mousButt::_mid;
						break;
					}
					break;
				default: button = mousButt::_none;
					break;
				}
			}


			//UPDATE
			int x;
			int y;
			SDL_GetMouseState(&x, &y);

			//El mouse és sobre el botó de play?
			if ((x < playRect.x + playRect.w && x> playRect.x) && (y < playRect.y + playRect.h && y>playRect.y)) {
				SDL_SetTextureColorMod(playTexture, 50, 50, 50);
				if (button == mousButt::_left) {
					myScene = scenes::_game;
				}
			}
			else {
				SDL_SetTextureColorMod(playTexture, playColour.r, playColour.g, playColour.b);
			}

			//El mouse es sobre el botó d'exit?
			if ((x < exitRect.x + exitRect.w && x> exitRect.x) && (y < exitRect.y + exitRect.h && y>exitRect.y)) {
				SDL_SetTextureColorMod(exitTexture, 50, 50, 50);
				if (button == mousButt::_left) {
					isActive = false;
				}
			}
			else {
				SDL_SetTextureColorMod(exitTexture, exitColour.r, exitColour.g, exitColour.b);
			}


			//DRAW
			SDL_RenderClear(myRenderer);

			SDL_RenderCopy(myRenderer, menuTexture, nullptr, &menuTextureRect);
			SDL_RenderCopy(myRenderer, playTexture, nullptr, &playRect);
			SDL_RenderCopy(myRenderer, exitTexture, nullptr, &exitRect);

			break;

		case scenes::_game:
		while (SDL_PollEvent(&evnt)) {
			switch (evnt.type) {
			case SDL_QUIT: isActive = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				switch (evnt.button.button) { //quin botó es el que s'ha premut?
				case SDL_BUTTON_RIGHT: button = mousButt::_right;
					break;
				case SDL_BUTTON_LEFT: button = mousButt::_left;
					break;
				case SDL_BUTTON_MIDDLE: button = mousButt::_mid;
					break;
				default: button = mousButt::_none;
					break;
				}
				break;
			case SDL_KEYDOWN:
				switch (evnt.key.keysym.sym) {
				}
				break;
			}
		}



		//UPDATE

		SDL_GetMouseState(&x, &y);

		const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

		if (keyboardState[SDL_SCANCODE_UP])
			pjPos.y -= 1 * _PJSPEED;
		else if (keyboardState[SDL_SCANCODE_DOWN]) 
			pjPos.y += 1 * _PJSPEED;
		if (keyboardState[SDL_SCANCODE_LEFT])
			pjPos.x -= 1 * _PJSPEED;
		else if (keyboardState[SDL_SCANCODE_RIGHT])
			pjPos.x += 1 * _PJSPEED;

		
		//collision check and spacial correction
		int xDist = abs(getCenter(pjRect).x - getCenter(skylineRect).x);
		int yDist = abs(getCenter(pjRect).y - getCenter(skylineRect).y);
		std::cout << xDist << " " << yDist << std::endl;
		if ( xDist < (pjRect.w/2 + skylineRect.w/2) && yDist < (pjRect.h / 2 + skylineRect.h / 2)) {
			std::cout << "hay colision\n";
			if (xDist - (pjRect.w / 2 + skylineRect.w / 2) > yDist - (pjRect.h / 2 + skylineRect.h / 2)) {

				if (getCenter(pjRect).x > getCenter(skylineRect).x) {
					pjPos.x += abs(xDist - (pjRect.w / 2 + skylineRect.w / 2));
					std::cout << "1\n";
				}
				else {
					pjPos.x -= abs(xDist - (pjRect.w / 2 + skylineRect.w / 2));
					std::cout << "2\n";
				}
			}
			else {
				if (getCenter(pjRect).y > getCenter(skylineRect).y) {
					pjPos.y += abs(yDist- (pjRect.h / 2 + skylineRect.h / 2));
					std::cout << "3\n";
				}
				else {
					pjPos.y -= abs(yDist - (pjRect.h / 2 + skylineRect.h / 2));
					std::cout << "4\n";
				}
			}
		}

		pjRect.x = pjPos.x;
		pjRect.y = pjPos.y;

		//DRAW
		SDL_RenderClear(myRenderer);

		SDL_RenderCopy(myRenderer, bgTexture, nullptr, &bgTextureRect);
		//SDL_SetRenderDrawColor(myRenderer, 100, 230, 50, 255);
		//SDL_RenderFillRect(myRenderer, &skylineRect);
		SDL_SetRenderDrawColor(myRenderer, 200, 200, 0, 255);
		SDL_RenderFillRect(myRenderer, &pjRect);
		
		break;
		}
		SDL_RenderPresent(myRenderer);
	}
	return 0;
}
	
