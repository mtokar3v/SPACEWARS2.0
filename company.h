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
		if (Player::getHealth() < 0) { run = false; break; }

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
				std::cout << "you win!";
				run = false;
			}
		}

		player.render();

		move_dimanic_object();
		SDL_RenderPresent(ren);
	}
	SDL_Delay(15000);
	if(master)
		delete master;
	powerMulty = 1;
	Player::upMaxHealth(100);
	Player::upSpeed(10);
}