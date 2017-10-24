#include <SDL.h>		// Always needs to be included for an SDL app
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <deque>
#include <string>

float countDown = 30.;
const int playerSize = 70;
const int p1 = 0;
const int p2 = 2;
const int _WIDTH = 640;
const int _HEIGHT = 480;
const int _PJSPEED = 5;
const int FPS = 60;

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

class gold {
public:
	coord pos;
	SDL_Rect goldRect;
};

bool doCollide(SDL_Rect a, SDL_Rect b) {
	int xDist = abs(getCenter(a).x - getCenter(b).x);
	int yDist = abs(getCenter(a).y - getCenter(b).y);
	return(xDist < (a.w / 2 + b.w / 2) && yDist < (a.h / 2 + b.h / 2));
}

void checkForCollisions(SDL_Rect &a, SDL_Rect &b, coord &dir) {
	int xDist = abs(getCenter(a).x - getCenter(b).x);
	int yDist = abs(getCenter(a).y - getCenter(b).y);

	if (doCollide(a, b)) {
		if (xDist - (a.w / 2 + b.w / 2) > yDist - (a.h / 2 + b.h / 2)) {

			if (getCenter(a).x > getCenter(b).x) {
				dir.x += abs(xDist - (a.w / 2 + b.w / 2));
			}
			else {
				dir.x -= abs(xDist - (a.w / 2 + b.w / 2));
			}
		}
		else {
			if (getCenter(a).y > getCenter(b).y) {
				dir.y += abs(yDist - (a.h / 2 + b.h / 2));
			}
			else {
				dir.y -= abs(yDist - (a.h / 2 + b.h / 2));
			}
		}
	}
}

void avoidFlee(SDL_Rect &a, coord &dir) {
	if (a.x < 0)
		dir.x+=abs(a.x);
	else if ((a.x + a.w) > _WIDTH)
		dir.x += (_WIDTH - (a.x+a.w));

	if (a.y < 0)
		dir.y -= abs(a.y);
	else if ((a.y + a.h) > _HEIGHT)
		dir.y += (_HEIGHT - (a.y + a.h));
}

int main(int, char*[]) {
	srand(time(nullptr));

	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	const Uint8 mixFlags{ MIX_INIT_MP3 | MIX_INIT_OGG };
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024);
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

	SDL_Texture* goldTexture;
	goldTexture = IMG_LoadTexture(myRenderer, "../../res/img/gold.png");

		//audio
	Mix_Music *soundtrack{ Mix_LoadMUS("../../res/au/initialD.mp3") };
	Mix_VolumeMusic(MIX_MAX_VOLUME);
	Mix_PlayMusic(soundtrack, -1);

		//text
	TTF_Font* saiyanFont;
	saiyanFont = TTF_OpenFont("../../res/ttf/saiyan.ttf", 80);
	TTF_Font* marioFont;
	marioFont = TTF_OpenFont("../../res/ttf/MarioLuigi2.ttf", 35);

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
	TTF_CloseFont(saiyanFont);

	SDL_Colour timerColour{ 230, 0, 0, 255 };
	SDL_Texture *timerTexture=nullptr;
	
	SDL_Colour scoreColour{ 0, 0, 0, 255 };
	mySurface = TTF_RenderText_Blended(marioFont, "Pl 1:", scoreColour);
	SDL_Texture *pointsOne = SDL_CreateTextureFromSurface(myRenderer, mySurface);
	SDL_Rect pointsOneRect{ 0, 0, mySurface->w, mySurface->h };

	mySurface = TTF_RenderText_Blended(marioFont, "Pl 2:", scoreColour);
	SDL_Texture *pointsTwo = SDL_CreateTextureFromSurface(myRenderer, mySurface);
	SDL_Rect pointsTwoRect{ 0, mySurface->h, mySurface->w, mySurface->h };

	//sprites
	SDL_Texture* numericSprite;
	numericSprite = IMG_LoadTexture(myRenderer, "../../res/img/num.png");
	int frameWnum, frameHnum, textureW, textureH;
	SDL_QueryTexture(numericSprite, NULL, NULL, &textureW, &textureH);

	frameWnum = textureW / 10;
	frameHnum = textureH / 1;

	SDL_Rect numericRectOne{ 0, 0, frameWnum, frameHnum }; //el primer digit del player 1
	SDL_Rect numericRectDrawOne{ pointsOneRect.x+pointsOneRect.w, pointsOneRect.y, 35, 35 };
	SDL_Rect numericRectOneTwo{ 0, 0, frameWnum, frameHnum }; //el segon digit del player 1
	SDL_Rect numericRectDrawOneTwo{ pointsOneRect.x + pointsOneRect.w+numericRectDrawOne.w, pointsOneRect.y, 35, 35 };

	SDL_Rect numericRectTwo{ 0, 0, frameWnum, frameHnum }; //el primer digit del player 2
	SDL_Rect numericRectDrawTwo{ pointsTwoRect.x + pointsTwoRect.w, pointsTwoRect.y, 35, 35 };
	SDL_Rect numericRectTwoTwo{ 0, 0, frameWnum, frameHnum }; //el segon digit del player 2
	SDL_Rect numericRectDrawTwoTwo{ pointsTwoRect.x + pointsTwoRect.w + numericRectDrawTwo.w, pointsTwoRect.y, 35, 35 };

	SDL_Texture* playersSprite;
	playersSprite = IMG_LoadTexture(myRenderer, "../../res/img/spCastle.png");
	int frameWpj, frameHpj;
	SDL_QueryTexture(playersSprite, NULL, NULL, &textureW, &textureH);
	frameWpj = textureW / 12;
	frameHpj = textureH / 8;

	SDL_Rect pjRectimg{ 0, 0, frameWpj, frameHpj };
	SDL_Rect rivalRectimg{ frameWpj * 3, 0, frameWpj, frameHpj };

	SDL_Rect pjRect{ 0, 0, playerSize, playerSize };
	SDL_Rect rivalRect{ 0, 0, playerSize, playerSize };

	std::pair <int, int> pjSpeed, rivalSpeed;
	pjSpeed.first = pjSpeed.second = rivalSpeed.first = rivalSpeed.second = 0;
	float frameTimer;
	frameTimer = 0.;

	//colliders

	coord pjPos{ 0, (int)(_HEIGHT*0.7) };
	
	coord rivalPos{ _WIDTH - 50, (int)(_HEIGHT*0.7) };

	SDL_Rect skylineRect{ 0, 0, _WIDTH, 140 };

	//gold deque
	std::deque<gold> initialLoot;

	for (int a = 0; a < 5; ++a) {
		initialLoot.push_back(gold());
		int w = rand();
		initialLoot[a].goldRect.w = initialLoot[a].goldRect.h = 50;
		initialLoot[a].pos.x = rand() % (_WIDTH - initialLoot[a].goldRect.w);
		initialLoot[a].pos.y = skylineRect.h + rand() % (_HEIGHT - initialLoot[a].goldRect.h-skylineRect.h);
		initialLoot[a].goldRect.x = initialLoot[a].pos.x;
		initialLoot[a].goldRect.y = initialLoot[a].pos.y;
	}

	SDL_Event evnt;
	bool isActive=true;
	mousButt button=mousButt::_none;
	scenes myScene = scenes::_menu;
	unsigned int scoreOne, scoreTwo;
	scoreOne = scoreTwo = 0;

	float deltaTime, lastTime;
	deltaTime = 0;
	lastTime = clock();
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
			deltaTime = clock() - lastTime;
			lastTime = clock();

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

		if (keyboardState[SDL_SCANCODE_UP]) {
			pjPos.y -= 1 * _PJSPEED; 
			pjSpeed.second = 1;
		}
		else if (keyboardState[SDL_SCANCODE_DOWN]) {
			pjPos.y += 1 * _PJSPEED;
			pjSpeed.second = -1;
		}
		else
			pjSpeed.second = 0;

		if (keyboardState[SDL_SCANCODE_LEFT]) {
			pjPos.x -= 1 * _PJSPEED;
			pjSpeed.first = -1;
		}
		else if (keyboardState[SDL_SCANCODE_RIGHT]) {
			pjPos.x += 1 * _PJSPEED;
			pjSpeed.first = 1;
		}
		else
			pjSpeed.first = 0;

		if (keyboardState[SDL_SCANCODE_W]) {
			rivalPos.y -= 1 * _PJSPEED;
			rivalSpeed.second = 1;
		}
		else if (keyboardState[SDL_SCANCODE_S]) {
			rivalPos.y += 1 * _PJSPEED;
			rivalSpeed.second = -1;
		}
		else
			rivalSpeed.second = 0;

		if (keyboardState[SDL_SCANCODE_A]) {
			rivalPos.x -= 1 * _PJSPEED;
			rivalSpeed.first = -1;
		}
		else if (keyboardState[SDL_SCANCODE_D]) {
			rivalPos.x += 1 * _PJSPEED;
			rivalSpeed.first = 1;
		}
		else
			rivalSpeed.first = 0;
		
		deltaTime = clock() - lastTime;
		lastTime = clock();
		
		frameTimer += deltaTime*0.01f;
		if (frameTimer >= 3)
			frameTimer = 0;

			//ANIMATIONS
		if (pjSpeed.first > 0) {
			pjRectimg.y = frameHpj * 2;
		}
		else if(pjSpeed.first < 0){
			pjRectimg.y = frameHpj;
		}
		if (pjSpeed.second > 0) {
			pjRectimg.y = frameHpj * 3;
		}
		else if (pjSpeed.second < 0) {
			pjRectimg.y = frameHpj * 0;
		}

		if (pjSpeed.first == 0 && pjSpeed.second == 0) {
			pjRectimg.x = frameWpj;
		}
		else {
			pjRectimg.x = (int)frameTimer*frameWpj;
		}

		if (rivalSpeed.first > 0) {
			rivalRectimg.y = frameHpj * 2;
		}
		else if (rivalSpeed.first < 0) {
			rivalRectimg.y = frameHpj;
		}
		if (rivalSpeed.second > 0) {
			rivalRectimg.y = frameHpj * 3;
		}
		else if (rivalSpeed.second < 0) {
			rivalRectimg.y = frameHpj * 0;
		}

		if (rivalSpeed.first == 0 && rivalSpeed.second == 0) {
			rivalRectimg.x = frameWpj*3 + frameWpj;
		}
		else {
			rivalRectimg.x = ((int)frameTimer*frameWpj) + frameWpj * 3;
		}

			//TIMER
		countDown -= deltaTime / 1000;
		std::string min(std::to_string((int)countDown / 60));
		int aux = (int)countDown % 60;
		std::string seg;
		if (aux<10)
			seg = "0" + (std::to_string(aux));
		else
			seg = (std::to_string(aux));

		std::string a(min + ":" + seg);
		mySurface = TTF_RenderText_Blended(marioFont, a.c_str(), timerColour);
		timerTexture = SDL_CreateTextureFromSurface(myRenderer, mySurface);
		SDL_Rect timerRect{ _WIDTH - mySurface->w, 0, mySurface->w, mySurface->h };
		
			//collision check and spacial correction
		checkForCollisions(pjRect, skylineRect, pjPos);//aquesta comproba col·lisions entre els jugadors i l'horitzó
		checkForCollisions(rivalRect, skylineRect, rivalPos);
		checkForCollisions(pjRect, rivalRect, pjPos);
		checkForCollisions(rivalRect, pjRect, rivalPos);

		avoidFlee(pjRect, pjPos);//aquesta les col·lisions dels jugadors amb els límits de l'escenari per a que no s'escapin
		avoidFlee(rivalRect, rivalPos);		

		for (int a = 0; a < initialLoot.size(); ++a) {//comprobació de xoc entre jugadors i or
			if (doCollide(pjRect, initialLoot.at(a).goldRect)) {
				initialLoot.erase(initialLoot.begin() + a);
				scoreOne++;
			}
			else if (doCollide(rivalRect, initialLoot.at(a).goldRect)) {
				initialLoot.erase(initialLoot.begin() + a);
				scoreTwo++;
			}
		}

		if (initialLoot.size() < 3) {//comprobació de si cal generar més or
			int newGold = rand() % 4 + 2;
			for (int i = 0; i < newGold; ++i) {
				initialLoot.push_back(gold());
				initialLoot.back().goldRect.w = initialLoot.back().goldRect.h = 50;
				initialLoot.back().pos.x = rand() % (_WIDTH - initialLoot.back().goldRect.w);
				initialLoot.back().pos.y = skylineRect.h + rand() % (_HEIGHT - initialLoot.back().goldRect.h - skylineRect.h);
				initialLoot.back().goldRect.x = initialLoot.back().pos.x;
				initialLoot.back().goldRect.y = initialLoot.back().pos.y;
			}
		}

		//actualizació de les posicions dels rectangles dels jugadors
		pjRect.x = pjPos.x;
		pjRect.y = pjPos.y;

		rivalRect.x = rivalPos.x;
		rivalRect.y = rivalPos.y;

		numericRectOne.x = ((scoreOne - scoreOne % 10) / 10)%10* frameWnum;
		numericRectOneTwo.x = (scoreOne % 10)*frameWnum;

		numericRectTwo.x = ((scoreTwo - scoreTwo % 10) / 10) % 10 * frameWnum;
		numericRectTwoTwo.x = (scoreTwo % 10)*frameWnum;

		//WIN CONDITION
		if (countDown <= 0) {
			if(scoreOne > scoreTwo){
				std::cout << "Ha ganado el player 1" << std::endl;
			}
			else if(scoreTwo > scoreOne){
				std::cout << "Ha ganado el player 2" << std::endl;
			}
			else {
				std::cout << "Habéis empatado!" << std::endl;
			}
			isActive = false;
		}

		//DRAW
		SDL_RenderClear(myRenderer);

		SDL_RenderCopy(myRenderer, bgTexture, nullptr, &bgTextureRect);
		for (int a = 0; a < initialLoot.size(); ++a) {
			SDL_RenderCopy(myRenderer, goldTexture, nullptr, &initialLoot.at(a).goldRect);
		}

		SDL_RenderCopy(myRenderer, timerTexture, nullptr, &timerRect);
		SDL_RenderCopy(myRenderer, pointsOne, nullptr, &pointsOneRect);
		SDL_RenderCopy(myRenderer, pointsTwo, nullptr, &pointsTwoRect);

		SDL_RenderCopy(myRenderer, numericSprite, &numericRectOne, &numericRectDrawOne);
		SDL_RenderCopy(myRenderer, numericSprite, &numericRectOneTwo, &numericRectDrawOneTwo);
		SDL_RenderCopy(myRenderer, numericSprite, &numericRectTwo, &numericRectDrawTwo);
		SDL_RenderCopy(myRenderer, numericSprite, &numericRectTwoTwo, &numericRectDrawTwoTwo);

		SDL_RenderCopy(myRenderer, playersSprite, &pjRectimg, &pjRect);
		SDL_RenderCopy(myRenderer, playersSprite, &rivalRectimg, &rivalRect);
		
		break;
		}
		SDL_RenderPresent(myRenderer);
	}

	//DESTROYS
		//menu
	SDL_DestroyTexture(menuTexture);
	SDL_DestroyTexture(playTexture);
	SDL_DestroyTexture(exitTexture);
		//menu
	SDL_DestroyTexture(bgTexture);
	SDL_DestroyTexture(timerTexture);
	SDL_DestroyTexture(pointsOne);
	SDL_DestroyTexture(pointsTwo);
	SDL_DestroyTexture(numericSprite);
	SDL_DestroyTexture(playersSprite);
	for (int i = 0; i < initialLoot.size(); ++i) {
		initialLoot.erase(initialLoot.begin() + i);
	}

	SDL_DestroyRenderer(myRenderer);
	SDL_DestroyWindow(myWindow);

	Mix_Quit();
	IMG_Quit();
	SDL_Quit();

	char end;
	std::cout << "Press a letter + return to end the program" << std::endl;
	std::cin >> end;
	return 0;
}
	
