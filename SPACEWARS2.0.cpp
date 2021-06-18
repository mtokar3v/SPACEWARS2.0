#include <iostream>
#include <time.h>
#include <windows.h>
#include <random>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include <chrono>

#include "data.h"
#include "textures.h"

#include "timer.h"
#include "object.h"
#include "player.h"
#include "shot.h"
#include "player_m.inl"
#include "boom.h"
#include "heal.h"
#include "bonus.h"
#include "dungeonMaster.h"
#include "enemy.h"

#include "arcade.h"
#include "company.h"


int main(int argc, char* argv[])
{
	//ShowWindow(GetConsoleWindow(), SW_HIDE);
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