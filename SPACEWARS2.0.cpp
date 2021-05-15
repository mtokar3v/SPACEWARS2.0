#include <iostream>
#include <time.h>
#include <random>
#include <chrono>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>

#include "data.h"
#include "textures.h"
#include "logic.h"
#include "arcade.h"
#include "company.h"


int main(int argc, char* argv[])
{
	srand(time(0));
	if (!init())
		return 1;
	shopping(ren,spaceShop_texture);
	setTextures();
	playArcade();
	return 0;

	//в конце нужно закрыть потоки и все остальное
}