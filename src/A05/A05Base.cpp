#include <SDL.h>		// Always needs to be included for an SDL app
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <time.h>
#include <iostream>

//Game general information
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(int, char*[]) {

	// --- INIT ---
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) throw "No es pot inicialitzar SDL subsystems";

	const Uint8 imgFlags{ IMG_INIT_PNG | IMG_INIT_JPG };
	if (!(IMG_Init(imgFlags) & imgFlags)) throw "Error: SDL_image init";

	if (TTF_Init() != 0) throw "No s'han pogut carregar les fonts";

	const Uint8 mixFlags{ MIX_INIT_MP3 | MIX_INIT_OGG };
	if (!(Mix_Init(mixFlags) & mixFlags)) throw " kill me";

	// --- WINDOW ---
	SDL_Window *window{ SDL_CreateWindow("SDL...", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN) };
	if (window == nullptr) throw "No es pot inicialitzar SDL_Window";


	// --- RENDERER ---
	SDL_Renderer *renderer{ SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) };
	if (renderer == nullptr) throw "No es pot inicialitzar SDL_Renderer";	

	// --- SPRITES ---
	SDL_Texture *bgTexture{ IMG_LoadTexture(renderer, "../../res/img/bg.jpg") };
	if (bgTexture == nullptr) throw "No s'ha pogut crear les texures";
	SDL_Rect bgRect{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	SDL_Texture *playerTexture{ IMG_LoadTexture(renderer, "../../res/img/kintoun.png") };
	if (playerTexture == nullptr) throw "No s'han pogut crear les textures";
	SDL_Rect playerRect{ 0, 0, 350/2, 189/2 };
	SDL_Rect playerTarget{ 0, 0, 100, 100};

	#define FPS 60

		// --- Animated Sprite ---

	playerTexture = { IMG_LoadTexture(renderer, "../../res/img/sp01.png") };
	SDL_Rect playerRectS{ 0, 0, 256, 256 };
	SDL_Rect playerPositionS;
	int textWidth, textHeight, frameWidth, frameHeight;
	SDL_QueryTexture(playerTexture, NULL, NULL, &textWidth, &textHeight);
	frameWidth = textWidth / 6;
	frameHeight = textHeight / 1;
	playerPositionS.x = playerPositionS.y = 0;
	playerRectS.x = playerRectS.y = 0;
	playerPositionS.h = playerRectS.h = frameHeight;
	playerPositionS.w = playerRectS.w = frameWidth;
	int frameTime = 0;

	// --- TEXT ---
	TTF_Font *font{ TTF_OpenFont("../../res/ttf/saiyan.ttf", 80) };
	if (font == nullptr) throw "No s'ha pogut inicialitar la font";

	

	SDL_Surface *tmpSurf{ TTF_RenderText_Blended(font, "My first SDL game", SDL_Colour{ 244, 150, 0, 255}) };
	if (tmpSurf == nullptr) TTF_CloseFont(font), throw "vainas";
	SDL_Texture *textTexture{ SDL_CreateTextureFromSurface(renderer, tmpSurf) };
	SDL_Rect textRect{ 100, 50, tmpSurf->w, tmpSurf->h };
	SDL_FreeSurface(tmpSurf);


	SDL_Surface *gobut = TTF_RenderText_Blended(font, "FUCKING CUNT", SDL_Colour{ 255, 0, 0, 255 });
	if (gobut == nullptr) TTF_CloseFont(font), throw "vainas";
	SDL_Texture *textTexture1{ SDL_CreateTextureFromSurface(renderer, gobut) };
	SDL_Rect textRect1{ 100, 150, gobut->w, gobut->h };
	SDL_FreeSurface(gobut);


	SDL_Surface *stpbut{ TTF_RenderText_Blended(font, "YOU MORON", SDL_Colour{ 0, 0, 255, 255 }) };
	if (stpbut == nullptr) TTF_CloseFont(font), throw "vainas";
	SDL_Texture *textTexture2{ SDL_CreateTextureFromSurface(renderer, stpbut) };
	SDL_Rect textRect2{ 100, 250, stpbut->w, stpbut->h };
	SDL_FreeSurface(stpbut);

	TTF_CloseFont(font);

	// --- AUDIO ---
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		throw "kljdfaskljdfaskjokljfes";
	}
	Mix_Music *soundtrack{ Mix_LoadMUS("../../res/au/mainTheme.mp3") };
	if (!soundtrack) throw"dgfasokljdfas";
	Mix_VolumeMusic(MIX_MAX_VOLUME );
	Mix_PlayMusic(soundtrack, -1);

	// --- TIME ---
	clock_t lastTime = clock();
	float timeDown = 10.;
	float deltaTime = 0;

	// --- GAME LOOP ---
	SDL_Event evnt;
	bool isRunning = true;
	while (isRunning){
		// HANDLE EVENTS
		while (SDL_PollEvent(&evnt)) {
			switch (evnt.type) {
			case SDL_QUIT:		isRunning = false; break;
			case SDL_KEYDOWN:	if (evnt.key.keysym.sym == SDLK_ESCAPE) isRunning = false; break;
			case SDL_MOUSEMOTION: { 
				/*
				playerTarget.x = evnt.motion.x-playerTarget.w/2;
				playerTarget.y = evnt.motion.y-playerTarget.h/2;
				*/
				break; 
			}
			default:;
			}
		}

		// UPDATE
		playerRect.x += (playerTarget.x - playerRect.x)/ 10;
		playerRect.y += (playerTarget.y - playerRect.y) / 10;

		frameTime++;
		if (FPS / frameTime <= 9) {
			frameTime = 0;
			playerRectS.x += frameWidth;
			if (playerRectS.x >= frameWidth)
				playerRectS.x = 0;
		}
		
		deltaTime = (clock() - lastTime);
		lastTime = clock();
		if (timeDown > 0) {
			timeDown -= deltaTime / 1000;
			std::cout << timeDown << std::endl;
		}
		else {
			std::cout << "ALOMOMOLA ME MOLA ME MOLA ALOMOMOLA ";
		}

		// DRAW
		
			//Background
		SDL_RenderClear(renderer);
		
		SDL_RenderCopy(renderer, bgTexture, nullptr, &bgRect);
		SDL_RenderCopy(renderer, playerTexture, nullptr, &playerRect);
		SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
		SDL_RenderCopy(renderer, textTexture1, nullptr, &textRect1);
		SDL_RenderCopy(renderer, textTexture2, nullptr, &textRect2);
		SDL_RenderPresent(renderer);
			//Animated Sprite
		SDL_RenderPresent(renderer);
		SDL_RenderCopy(renderer, playerTexture, &playerRectS, &playerPositionS);
	}

	// --- DESTROY ---
	SDL_DestroyTexture(textTexture);
	SDL_DestroyTexture(bgTexture);
	SDL_DestroyTexture(playerTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	// --- QUIT ---
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
	return 0;
}