#pragma once
#include "logic.h"

void playCompany()
{
	Object backgroung(ren, background_texture);
	backgroung.resizeOn(displayMode.w, displayMode.h);
	backgroung.render();

	Object backgroung2(ren, background_texture);
	backgroung2.resizeOn(displayMode.w, displayMode.h);
	backgroung2.move(0, -displayMode.h);

	Player player(ren, player_texture, fire_texture);
	player.resizeOn(122, 165);
	player.moveTo(displayMode.w / 2 - 61, displayMode.h - 165);
	player.render(0);

	Master* master = nullptr;

	enemySpeed = 1;
	spawnTime = 5;

	Timer t;
	while (run)
	{
		for (int i = 0; i < 60 && run; i++)
		{
			SDL_Delay(30);
			if (!(i % 2))
			{
				backgroung.move(0, 1);
				backgroung2.move(0, 1);
			}
			if (backgroung.get_y() > displayMode.h)
				backgroung.move(0, -(2 * displayMode.h));

			if (backgroung2.get_y() > displayMode.h)
				backgroung2.move(0, -(2 * displayMode.h));

			SDL_PollEvent(&event);

			player.shoting(event);
			player.moving();

			if (event.key.keysym.sym == SDLK_q)
				run = false;


			spawningEnemy((int)t.elapsed(), spawnTime, enemySpawn, &player, boom_texture);
			spawningBonus((int)t.elapsed(), bonusSpawn, &player);
			resetBonus((int)player.getModificatorTime(), bonusRespawn, &player);

			if (!((int)t.elapsed() % 30) && levelUp && (int)t.elapsed() && wave != 5)
			{
				wave++;
				int x = rand() % displayMode.w;
				int y = 0;
				shopping(&player);
				t.reset();
				clearBattleArea();
				player.fullHealth();
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
			backgroung2.render();
			inputInfo((int)t.elapsed(), &player);

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
					add_text(ren, font, color, std::to_string(player.getTotalPoint()), displayMode.w * 0.53, displayMode.h * 0.55);
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

			player.render(i);

			move_dimanic_object();
			SDL_RenderPresent(ren);

			if (player.getHealth() <= 0)
			{
				SDL_RenderClear(ren);
				backgroung.render();
				add_text(ren, bigFont, color, "YOU LOSE", displayMode.w * 0.45, displayMode.h * 0.45);
				add_text(ren, font, color, "total point: ", displayMode.w * 0.45, displayMode.h * 0.55);
				add_text(ren, font, color, std::to_string(player.getTotalPoint()), displayMode.w * 0.53, displayMode.h * 0.55);
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
	}

	if (master)
		delete master;
	powerMulty = 1;
	player.spendPoints(player.getPoints());
	player.addTotalPoint(-player.getTotalPoint());
	player.upMaxHealth(100);
	player.fullHealth();
	player.upSpeed(10);
	clearBattleArea();
	wave = 1;
	run = true;
}