#pragma once
#include "logic.h"

void playArcade()
{
	Object backgroung(ren, background_texture);
	backgroung.resizeOn(displayMode.w, displayMode.h);
	backgroung.render();

	Player player(ren, player_texture);
	player.resizeOn(122, 165);
	player.moveTo(displayMode.w / 2 - 61, displayMode.h - 165);
	player.render();

	Timer t;
	while (run)
	{

		//����� ��������� ��������� � �������� �������
		SDL_Delay(30);
		//����������� ��� ������, ���� �������������, ������� ����� ������ � ��������� 
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

		//��������� �������� ������ � ����� �������
		if (!((int)t.elapsed() % 30) && levelUp && (int)t.elapsed())
		{           
			wave++;
			int x = rand() % displayMode.w;
			int y = 0;

			Heal* heal = new Heal(ren, heal_texture);
			heal->moveTo(x, y);
			heal->resizeOn(40, 40);
	 		EnemyList.push_back(heal);
			enemySpeed = rand() % 4 + 2;
			spawnTime = rand() % 3 + 1;
			levelUp = false;
		}
		else if ((int)t.elapsed() % 30)
			levelUp = true;

		SDL_RenderClear(ren);
		backgroung.render();
		inputInfo((int)t.elapsed());
		player.render();

		move_dimanic_object();
		SDL_RenderPresent(ren);

		if (Player::getHealth() < 0)
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
	Player::spendPoints(Player::getPoints());
	Player::fullHealth();
	clearBattleArea();
	wave = 1;
	run = true;
}