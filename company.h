#pragma once
#include "logic.h"
#include "dungeonMaster.h"

void playCompany()
{
	Object backgroung(ren, background_texture);
	backgroung.resizeOn(displayMode.w, displayMode.h);
	backgroung.render();

	Player player(ren, player_texture);
	player.resizeOn(122, 165);
	player.moveTo(displayMode.w / 2 - 61, displayMode.h - 165);
	player.render();

	Master* master = nullptr;

	enemySpeed = 1;
	spawnTime = 5;

	Timer t;
	while (run)
	{
		//чтобы замедлить обработку и движение корабля
		SDL_Delay(30);
		//считывались все эвенты, даже одновременные, поэтому можно бегать о диагонали 
		SDL_PollEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT: run = false; break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT && event.button.x <= 10 && event.button.y <= 10)
				run = false; break;
		case SDL_KEYDOWN:
			shoting(event, player);
			break;
		}
		moving(player);

		spawningEnemy((int)t.elapsed(), spawnTime, enemySpawn);
		spawningBonus((int)t.elapsed(), bonusSpawn);
		resetBonus((int)Player::getModificatorTime(), bonusRespawn);

		if (!((int)t.elapsed() % 30) && levelUp && (int)t.elapsed() && wave != 5)
		{
			wave++;
			int x = rand() % displayMode.w;
			int y = 0;
			shopping();
			t.reset();
			clearBattleArea();
			Player::fullHealth();
			enemySpeed++;
			spawnTime--;
			levelUp = false;

			if (wave == 5)
			{
				master = new Master(ren, master_texture, &player);
				master->resizeOn(150, 150);
				master->moveTo(0, 30);
				master->render();
			}
		}
		else if ((int)t.elapsed() % 30)
			levelUp = true;

		SDL_RenderClear(ren);
		
		backgroung.render();
		inputInfo((int)t.elapsed());
		if (wave == 5)
		{
			spawnTime = 1;
			master->active();
			master->render();

			if (master->isEnd())
			{
				SDL_RenderClear(ren);
				backgroung.render();
				add_text(ren, bigFont, color, "YOU WIN", displayMode.w * 0.45, displayMode.h * 0.45);
				add_text(ren, font, color, "total point: ", displayMode.w * 0.45, displayMode.h * 0.55);
				add_text(ren, font, color, std::to_string(Player::getPoints()), displayMode.w * 0.53, displayMode.h * 0.55);
				SDL_RenderPresent(ren);
				while (run)
				{
					SDL_PollEvent(&event);
					if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
						run = false;
				}
				SDL_RenderClear(ren);
			}
		}

		player.render();

		move_dimanic_object();
		SDL_RenderPresent(ren);

		if (Player::getHealth() <= 0)
		{
			SDL_RenderClear(ren);
			backgroung.render();
			add_text(ren, bigFont, color, "YOU LOSE", displayMode.w * 0.45, displayMode.h * 0.45);
			add_text(ren, font, color, "total point: ", displayMode.w * 0.45, displayMode.h * 0.55);
			add_text(ren, font, color, std::to_string(Player::getPoints()), displayMode.w * 0.53, displayMode.h * 0.55);
			SDL_RenderPresent(ren);
			while (run)
			{
				SDL_PollEvent(&event);
				if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
					run = false;
			}
			SDL_RenderClear(ren);
		}
	}

	if(master)
		delete master;
	powerMulty = 1;
	Player::spendPoints(Player::getPoints());
	Player::upMaxHealth(100);
	Player::fullHealth();
	Player::upSpeed(10);
	clearBattleArea();
	wave = 1;
	run = true;
}