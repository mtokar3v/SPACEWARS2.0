#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

class Object;
class Enemy;
extern SDL_DisplayMode displayMode;
extern std::vector<Object*> ShotList;

enum Trajectory
{
	LINE,		//пр€ма€ лини€ в сторону игрока
	WALL,		//стенкой
	SINUSOID,	//синусоида
	COSINUSOID,	//косинусоида
	TRAJCOUNT	//количество траекторий
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

bool isCrash(int x, int y, int w, int h/* std::vector<Object*> tmp*/);

class Player : public Object
{
private:
	double speed;
	static int point;
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
		if (rect->y + y - offset < displayMode.h)
				return true;
		return false;
	}

	bool go()
	{
		switch (traj)
		{
		case SINUSOID: move(sin(rect->y / 10) * 8, speed); break;
		case COSINUSOID: move(cos(rect->y / 10) * 8, speed); break;
		case LINE: move(0, speed); break;
		case WALL: move(0, speed); break;
			
			

		}
		
		//SDL_Delay(3);
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

bool isCrash (int x, int y, int w, int h)
{
	std::vector<Object*>::iterator at;
	for (at = ShotList.begin(); at != ShotList.end(); at++)
		if ((**at).get_x() <= x + w / 2 && (**at).get_x() >= x - w / 2 && (**at).get_y() <= y + h / 2)
		{
			(**at).moveTo(0, 0);
			return true;
		}
	return false;
}


//у выстрелов тоже может быть разна€ траектори€, разна€ скорость и тд, поэтому отдельный класс
class Shot : public Object
{
	int speed;
public:
	Shot(SDL_Renderer* ren, SDL_Texture* texture) : Object(ren, texture)
	{
		speed = 7;
	}

	virtual bool check_confines(int x, int y)
	{
		int offset = 70;
		if (rect->y + y + offset > 0 && rect->x + x + offset < displayMode.w && rect->x + x + offset > 0)
			return true;
		return false;
	}

	//относительно траектории
	bool go()
	{
		//во втором аргументе - функци€
		move(0, - speed);
		//std::cout << rect->x << " " << rect->y << std::endl;
		if (check_confines(0, -speed) )
		{
			render();
			return true;
		}
		else
			return false;
	}
};