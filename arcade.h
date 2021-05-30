#pragma once
#include "logic.h"

void playArcade()
{
	Object backgroung(ren, background_texture);
	backgroung.resizeOn(displayMode.w, displayMode.h);

	Object backgroung2(ren, background_texture);
	backgroung2.resizeOn(displayMode.w, displayMode.h);
	backgroung2.move(0, -displayMode.h);

	Player player(ren, player_texture, fire_texture);
	player.resizeOn(122, 165);
	player.moveTo(displayMode.w / 2 - 61, displayMode.h - 165);
	player.render(0);

	Timer t;
	while (run)
	{
		for (int i = 0; i < 60 && run; i++)
		{
			if (!(i % 2))
			{
				backgroung.move(0, 1);
				backgroung2.move(0, 1);
			}
			if (backgroung.get_y() > displayMode.h)
				backgroung.move(0, -(2 * displayMode.h));

			if (backgroung2.get_y() > displayMode.h)
				backgroung2.move(0, -(2 * displayMode.h));

			SDL_Delay(30);
			SDL_PollEvent(&event);

			player.shoting(event);
			player.moving();

			if (event.key.keysym.sym == SDLK_q)
				run = false;

			spawningEnemy((int)t.elapsed(), spawnTime, enemySpawn, &player, boom_texture);
			spawningBonus((int)t.elapsed(), bonusSpawn, &player);
			resetBonus((int)player.getModificatorTime(), bonusRespawn, &player);

			if (!((int)t.elapsed() % 30) && levelUp && (int)t.elapsed())
			{
				wave++;
				int x = rand() % displayMode.w;
				int y = 0;

				Heal* heal = new Heal(ren, heal_texture, &player);
				heal->moveTo(x, y);
				heal->resizeOn(40, 40);
				EnemyList.push_back(heal);
				enemySpeed = rand() % 5 + 1;
				spawnTime = rand() % 4 + 2;
				levelUp = false;
			}
			else if ((int)t.elapsed() % 30)
				levelUp = true;

			SDL_RenderClear(ren);

			backgroung.render();
			backgroung2.render();
			inputInfo((int)t.elapsed(), &player);
			player.render(i);

			move_dimanic_object();
			SDL_RenderPresent(ren);

			if (player.getHealth() < 0)
			{
				SDL_RenderClear(ren);
				backgroung.render();
				backgroung2.render();
				add_text(ren, bigFont, color, "YOU LOSE", displayMode.w * 0.45, displayMode.h * 0.45);
				add_text(ren, font, color, "total point: ", displayMode.w * 0.45, displayMode.h * 0.55);
				add_text(ren, font, color, std::to_string(player.getPoints()), displayMode.w * 0.53, displayMode.h * 0.55);
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
	player.spendPoints(player.getPoints());
	player.addTotalPoint(-player.getTotalPoint());
	player.fullHealth();
	clearBattleArea();
	wave = 1;
	run = true;
}