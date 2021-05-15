#pragma once
#include "logic.h"

int Player::point = 0;
int Player::health = 100;
ShotTr Player::tr = NONE;

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
		if (Player::getHealth() < 0) { run = false; break; }

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
	}
}