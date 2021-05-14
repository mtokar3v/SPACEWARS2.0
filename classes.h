#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "stuff.h"


class Object;
class Shot;
class Enemy;
extern SDL_DisplayMode displayMode;
extern std::vector<Object*> ShotList;

enum Trajectory
{
	LINE,		//прямая линия в сторону игрока
	WALL,		//стенкой
	SINUSOID,	//синусоида
	COSINUSOID,	//косинусоида
	ARROW,
	TRAJCOUNT	//количество траекторий
};

enum ShotTr
{
	NONE,
	DOUBLE,			//выстрелы с двух сторон
	TRIPLE,			//выстрелы  в три стороны
	RAY,			//луч
	SHOTCOUNT		//количество режимов стрельбы
};

class Object 
{
protected:
	SDL_Rect* rect;
	SDL_Texture* texture;
	SDL_Renderer* ren;
public:
	Object(SDL_Renderer* ren, SDL_Texture* texture)
	{
		this->ren = ren;
		rect = new SDL_Rect;
		this->texture = texture;
		*rect = { 0, 0, 0, 0 };
	}

	void move(double x, double y)
	{
		rect->x += x;
		rect->y += y;
	}

	void moveTo(int x, int y)
	{
		rect->x = x;
		rect->y = y;
	}

	void resizeOn(int w, int h)
	{
		rect->w = w;
		rect->h = h;
	}

	void resize(int w, int h)
	{
		rect->w += w;
		rect->h += h;
	}

	int get_x()
	{
		return rect->x;
	}

	int get_y()
	{
		return rect->y;
	}

	int get_w()
	{
		return rect->w;
	}

	int get_h()
	{
		return rect->h;
	}

	virtual bool check_confines(int x, int y)
	{
		int offset = 70;
		if (rect->x + x + offset < displayMode.w && rect->x + x + offset > 0 && rect->y + y - offset < displayMode.h && rect->y + y + offset > 0)
			return true;
		return false;
	}

	void render()
	{
		SDL_RenderCopy(ren, texture, NULL, rect);
	}

	virtual bool go() { return true; }

	~Object()
	{
		SDL_DestroyTexture(texture);
		delete rect;
	}

};

bool isCrash(int x, int y, int w, int h);

class Player : public Object
{
private:
	double speed;
	static int health;
	static int point;
	static ShotTr tr;
	//static Timer* timer;
public:
	Player(SDL_Renderer* ren, SDL_Texture* texture) : Object(ren, texture)
	{
		speed = 10;
	}
	int playerSpeed()
	{
		return speed;
	}

	static int getPoints()
	{
		return point;
	}

	static void makeDamage()
	{
		health -= 10;
	}

	static int getHealth()
	{
		return health;
	}

	static void setModificator(ShotTr t)
	{
		//if (t != NONE)
		//{
			//if(timer)
				//delete timer;
		//	timer = new Timer();
		//}
		tr = t;
	}

	/*static double getModificatorTime()
	{
		if (timer)
			return timer->elapsed();
		else
			return 0;
	}*/

	static ShotTr getModificator()
	{
		return tr;
	}

	static void addPoint(int somePoint)
	{
		point += somePoint;
	}
};

class Enemy : public Object
{
private:
	int speed;
	int points;
	double o = 0;
	Trajectory traj;
public:
	Enemy(SDL_Renderer* ren, SDL_Texture* texture, int points, int speed, Trajectory tr) : Object(ren, texture)
	{
		this->points = points;
		this->speed = speed;
		traj = tr;
	}

	bool check_confines(int x, int y)
	{
		int offset = 70;
		if (rect->x + x + offset < displayMode.w && rect->x + x - offset > 0 && rect->y + y - offset < displayMode.h)
				return true;
		else if (rect->y + y > displayMode.h)
			Player::makeDamage();
		return false;
	}

	bool go()
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
				Player::addPoint(points);
			else
			{
				render();
				return true;
			}
		}

		return false;
	}
};


//у выстрелов тоже может быть разная траектория, разная скорость и тд, поэтому отдельный класс
class Shot : public Object
{
	ShotTr modificator;
	int num;
	bool invulnerability;
	int speed;
public:
	Shot(SDL_Renderer* ren, SDL_Texture* texture, int num) : Object(ren, texture)
	{
		modificator = NONE;
		invulnerability = false;
		this->num = num;
		switch (num)
		{
		case 2: rect->x += 55; break;
		case 3: rect->x += 110; break;
		}
		speed = 7;
	}

	void upgrade(ShotTr tr)
	{
		modificator = tr;
	}

	bool getInv()
	{
		return invulnerability;
	}

	virtual bool check_confines(int x, int y)
	{
		int offset = 250;
		if (rect->y + y + offset > 0)
			return true;
		return false;
	}

	bool go()
	{
		if (modificator == RAY)
			invulnerability = true;
		
		if (modificator == TRIPLE)
		{
			switch (num)
			{
			case 1: move(-5, -speed); break;
			case 2: move( 0, -speed); break;
			case 3: move(5, -speed); break;
			}
		}
		else
			move(0, -speed);
		
		if (check_confines(0, -speed) )
		{
			render();
			return true;
		}
		else
			return false;
	}
};

class Bonus : public Object
{
private:
	ShotTr tr;
	int speed;
public:
	Bonus(SDL_Renderer* ren, SDL_Texture* texture, ShotTr tr) : Object(ren, texture)
	{
		this->tr = tr;
		speed = 2;
	}
	bool go()
	{
		move(0, speed);

		if (check_confines(0, speed))
		{
			if (isCrash(rect->x, rect->y, rect->w, rect->h))
			{
				moveTo(-40, 0);
				Player::setModificator(tr);
			}
			else
			{
				render();
				return true;
			}
		}
	}
};

bool isCrash(int x, int y, int w, int h)
{
	std::vector<Object*>::iterator at;
	for (at = ShotList.begin(); at != ShotList.end(); at++)
		if ((**at).get_x() <= x + w && (**at).get_x() >= x - w / 2 && (**at).get_y() <= y + h / 2)
		{
			if (!(dynamic_cast<Shot*>(*at))->getInv())
				(**at).moveTo(-10, 0);
			return true;
		}
	return false;
}