#pragma once
#include <iostream>
#include <time.h>
#include "SDL.h"

#define RECT_SIZE 20
#define WIDTH 800
#define HEIGHT 600

struct Vector2D {
	int x;
	int y;
	Vector2D::Vector2D() :x(0), y(0) {};
	Vector2D::Vector2D(int uX, int uY) :x(uX), y(uY) {};

	Vector2D& operator+=(const Vector2D& a) {
		//TO DO
	}
};

void sleep(int milliseconds) {
	clock_t time_end = clock() + milliseconds * CLOCKS_PER_SEC / 1000;
	while (clock() < time_end);
}

void randomColor(SDL_Color &col) {
	//TO DO
}

int main(int argc, char *argv[]) {
	SDL_Window* window = SDL_CreateWindow("Hello SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	Vector2D position = Vector2D(100, 100);
	SDL_Color color = { 0,255,0, 0xFF };

	bool quit = false;
	while (!quit)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT: quit = SDL_TRUE; break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE: quit = SDL_TRUE; break;
				}
				break;
			}
		}

		//Update
		randomColor(color);
		//TO DO
		sleep(2);

		//Background
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0xFF);
		SDL_RenderClear(renderer);
		//Rect
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderFillRect(renderer, new SDL_Rect{ position.x, position.y, RECT_SIZE, RECT_SIZE });
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = nullptr;
	renderer = nullptr;
	SDL_Quit();

	return 0;
}