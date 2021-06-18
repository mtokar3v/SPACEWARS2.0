#pragma once

class Enemy : public Object
{
private:
	int speed;
	int points;
	Player* player;
	Trajectory traj;
	SDL_Texture** boom_texture;

	bool check_confines(int x, int y);

public:
	Enemy(SDL_Renderer* ren, SDL_Texture* texture, int points, int speed, Trajectory tr, Player* pl, SDL_Texture** t);
	bool go();
};

bool Enemy::check_confines(int x, int y)
{
	int offset = 30;
	if (rect->x + x + rect->w + offset < displayMode.w && rect->x + x - offset> 0 && rect->y + y - offset < displayMode.h)
		return true;
	else if (rect->y + y > displayMode.h)
		player->makeDamage();
	return false;
}

Enemy::Enemy(SDL_Renderer* ren, SDL_Texture* texture, int points, int speed, Trajectory tr, Player* pl, SDL_Texture** t) : Object(ren, texture)
{
	player = pl;
	boom_texture = t;
	this->points = points;
	this->speed = speed;
	traj = tr;
}

bool Enemy::go()
{
	switch (traj)
	{
	case SINUSOID: move(sin(rect->y / 10) * 8, speed); break;
	case COSINUSOID: move(cos(rect->y / 10) * 8, speed); break;
	default: move(0, speed); break;
	}

	if (check_confines(0, speed))
	{
		if (isCrash(rect->x, rect->y, rect->w, rect->h))
		{
			BoomList.push_back(new Boom(ren, boom_texture, rect->x, rect->y, rect->w, rect->h));
			player->addTotalPoint(points);
			player->addPoint(points);
		}
		else
		{
			render();
			return true;
		}
	}
	return false;
}