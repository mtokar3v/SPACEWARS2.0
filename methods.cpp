#ifndef METHODS
#define METHODS
#include "classes.h"
Shot::Shot(SDL_Renderer* ren, SDL_Texture* texture, int num, Player* player) : Object(ren, texture)
{
	modificator = NONE;
	this->player = player;
	invulnerability = false;
	this->num = num;
	switch (num)
	{
	case 2: rect->x += 55; break;
	case 3: rect->x += 110; break;
	}
	speed = 7;
}

bool Shot::check_confines(int x, int y)
{
	int offset = 50;
	if (rect->y + y + offset > 0)
		return true;
	return false;
}

void Shot::upgrade(ShotTr tr)
{
	modificator = tr;
	if (modificator == RAY)
		invulnerability = true;
	else
		invulnerability = false;
}

bool Shot::getInv()
{
	return invulnerability;
}

ShotTr Shot::get_modificator()
{
	return modificator;
}

bool Shot::go()
{

	switch (modificator)
	{
	case RAY:
		if (player->get_modificator() == RAY)
		{
			moveTo(player->get_x() + 55, player->get_y() - rect->h + 30);
			if (rect->y > 0)
				resize(0, 2 * speed);
			render();
			return true;
		}

		if (rect->h > 0)
		{
			resize(0, -2 * speed);
			render();
			return true;
		}
		else
		{
			moveTo(0, -displayMode.h);
		}
		break;
	case TRIPLE:
		switch (num)
		{
		case 1: move(-5, -speed); break;
		case 2: move(0, -speed); break;
		case 3: move(5, -speed); break;
		}
		break;
	default:move(0, -speed);
	}
	
	if (check_confines(0, -speed))
	{
		render();
		return true;
	}
	else
		return false;
}
#endif METHODS
