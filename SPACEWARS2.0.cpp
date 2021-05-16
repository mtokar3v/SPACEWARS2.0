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
#include "dungeonMaster.h"

int Player::point = 0;
int Player::health = 100;
int Player::maxHealth = 100;
int Player::speed = 10;
ShotTr Player::tr = NONE;


int main(int argc, char* argv[])
{
	srand(time(0));
	if (!init())
		return 1;

	setTextures();
	//playArcade();
	playCompany();
	return 0;

	//в конце нужно закрыть потоки и все остальное
}