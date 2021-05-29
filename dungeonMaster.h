#pragma once
#include "classes.h"

class Rainbow : public Object
{
private:
	Player* player;

public:
	Rainbow(SDL_Renderer* ren, SDL_Texture* texture, Player* player) : Object(ren, texture)
	{
		this->player = player;
	}

	bool go()
	{
		resize(0, 10);

		if (check_confines(0, rect->h))
		{
			if (rect->x <= player->get_x() + player->get_w() && rect->x >= player->get_x() - player->get_w() && rect->h >= player->get_y())
				player->makeDamage();
			render();
			return true;
		}
		else
			return false;
	}
};

class Master : public Object
{
private:
	int hp;
	int speed;
	bool right;
	bool starSpawn;
	bool rainbowSpawn;
	bool end;
	Timer* t;
	int spawnTime;
	Player* player;

	void sparkle()
	{
		spawningEnemy((int)t->elapsed(), spawnTime, starSpawn, player);
	}

	void getDamage()
	{
		std::vector<Object*>::iterator at;
		for (at = ShotList.begin(); at != ShotList.end(); at++)
			if ((**at).get_x() <= rect->x + rect->w && (**at).get_x() >= rect->x - rect->w / 2 && (**at).get_y() <= rect->y + rect->h)
			{
				texture = master_damage_texture;
				render();
				SDL_RenderPresent(ren);
				(**at).moveTo(-10, 0);
				hp -= 20;
				SDL_Delay(10);
				texture = master_texture;
			}

		if (hp < 5000)
		{
			spawnTime = 7;
			speed = 4;
		}

		if (hp < 300)
		{
			spawnTime = 2;
			speed = 10;
		}

		if (hp <= 0)
			end = true;
	}

	void attack()
	{
		if (rect->x <= player->get_x() + player->get_w() && rect->x >= player->get_x() - player->get_w() && rainbowSpawn)
		{
			Rainbow* rainbow = new Rainbow(ren, rainbow_texture, player);
			rainbow->resizeOn(150, 10);
			rainbow->moveTo(rect->x, rect->y);
			EnemyList.push_back(rainbow);
			rainbowSpawn = false;
		}

		//ïðè íàåçäå íà áîññà æèçíè òðàòÿòñÿ 
		if (rect->x <= player->get_x() + player->get_w() && rect->x >= player->get_x() - player->get_w() && rect->h >= player->get_y())
			player->makeDamage();

		if (!((int)t->elapsed() % 5))
			rainbowSpawn = true;
	}

public:
	Master(SDL_Renderer* ren, SDL_Texture* texture, Player* player) : Object(ren, texture)
	{
		hp = 10000;
		speed = 2;
		spawnTime = 10;
		right = true;
		starSpawn = true;
		end = false;
		rainbowSpawn = true;
		t = new Timer();
		this->player = player;
		for (int i = 0; i < 4; i++)
			enemy_textures[i] = sparkle_texture;
	}

	void active()
	{

		if (right)
		{
			move(speed, 0);
			if (rect->x >= displayMode.w * 0.8)
				right = false;
		}
		else
		{
			move(-speed, 0);
			if (rect->x <= displayMode.w * 0.1)
				right = true;
		}
		add_text(ren, font, color, std::to_string(hp), displayMode.w / 2, 50);
		sparkle();
		getDamage();
		attack();
	}

	bool isEnd()
	{
		return end;
	}
};