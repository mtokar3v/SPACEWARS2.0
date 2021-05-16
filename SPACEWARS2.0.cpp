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
int Player::totalPoint = 0;
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


	Object menu(ren, mainMenu_texture);
	
	bool run = true;
	while (run)
	{
		menu.resizeOn(displayMode.w, displayMode.h);
		menu.render();
		SDL_RenderPresent(ren);

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT: run = false; break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT && event.button.x <= 750 && event.button.x >= 530 && event.button.y <= 361 && event.button.y >= 277)
					playCompany();
				if (event.button.button == SDL_BUTTON_LEFT && event.button.x <= 750 && event.button.x >= 530 && event.button.y <= 492 && event.button.y >= 408)
					playArcade();
				if (event.button.button == SDL_BUTTON_LEFT && event.button.x <= 750 && event.button.x >= 530 && event.button.y <= 636 && event.button.y >= 545)
					run = false;
				break;
			}
		}
	}

	close();
	return 0;
}