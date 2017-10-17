#include <SDL.h>		// Always needs to be included for an SDL app
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

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
	SDL_Rect playerRect{ 0, 0, 350, 189 };
	SDL_Rect playerTarget{ 0, 0, 100, 100 };


		// --- Animated Sprite ---

	// --- TEXT ---
	TTF_Font *font{ TTF_OpenFont("../../res/ttf/saiyan.ttf", 80) };
	if (font == nullptr) throw "No s'ha pogut inicialitar la font";
	SDL_Surface *tmpSurf{ TTF_RenderText_Blended(font, "My first SDL game", SDL_Colour{ 244, 150, 0, 255}) };
	if (tmpSurf == nullptr) TTF_CloseFont(font), throw "vainas";
	SDL_Texture *textTexture{ SDL_CreateTextureFromSurface(renderer, tmpSurf) };
	SDL_Rect textRect{ 100, 50, tmpSurf->w, tmpSurf->h };
	SDL_FreeSurface(tmpSurf);
	TTF_CloseFont(font);

	SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

	// --- AUDIO ---
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		throw "kljdfaskljdfaskjokljfes";
	}
	Mix_Music *soundtrack{ Mix_LoadMUS("../../res/au/mainTheme.mp3") };
	if (!soundtrack) throw"dgfasokljdfas";
	Mix_VolumeMusic(MIX_MAX_VOLUME );
	Mix_PlayMusic(soundtrack, -1);

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
				playerRect.x = evnt.motion.x; 
				playerRect.y = evnt.motion.y;

				playerRect.x = evnt.motion.x - 50;
				playerRect.y = evnt.motion.y - 50;

				break; 
			}
			default:;
			}
		}

		// UPDATE
		//playerRect.x += playerTarget.x / 10;
		// DRAW
		
			//Background
		SDL_RenderClear(renderer);
		
		SDL_RenderCopy(renderer, bgTexture, nullptr, &bgRect);
		SDL_RenderCopy(renderer, playerTexture, nullptr, &playerRect);
		SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
		SDL_RenderPresent(renderer);
			//Animated Sprite
		SDL_RenderPresent(renderer);

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