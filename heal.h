#pragma once

class Heal : public Object
{
protected:
	int speed;
	Player* player;
public:
	Heal(SDL_Renderer* ren, SDL_Texture* texture, Player* pl);
	virtual bool go();
};

Heal::Heal(SDL_Renderer* ren, SDL_Texture* texture, Player* pl) : Object(ren, texture)
{
	speed = 2;
	player = pl;
}
bool Heal::go()
{
	move(0, speed);

	if (check_confines(0, speed))
	{
		if (isCrash(rect->x, rect->y, rect->w, rect->h))
		{
			player->fullHealth();
			return false;
		}
		else
		{
			render();
			return true;
		}
	}
}