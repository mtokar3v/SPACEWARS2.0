#ifndef CLASSES
#define CLASSES
#include "data.h"

enum Trajectory
{
	LINE,		
	WALL,		
	SINUSOID,	
	COSINUSOID,	
	ARROW,
	TRAJCOUNT	
};

enum ShotTr
{
	NONE,
	DUET,
	TRIPLE,
	RAY,
	SHOTCOUNT
};

class Timer
{
private:
	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1> >;

	std::chrono::time_point<clock_t> m_beg;

public:
	Timer() : m_beg(clock_t::now())
	{
	}

	void reset()
	{
		m_beg = clock_t::now();
	}

	double elapsed() const
	{
		return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
	}
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

	virtual bool check_confines(int x, int y)
	{
		if (rect->x + x + rect->w < displayMode.w && rect->x + x > 0 && rect->y + y + rect-> h < displayMode.h && rect->y + y > 0)
			return true;
		return false;
	}

	void change_textures(SDL_Texture* texture)
	{
		this->texture = texture;
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

	virtual void render()
	{
		SDL_RenderCopy(ren, texture, NULL, rect);
	}

	virtual bool go() { return true; }

	virtual ~Object()
	{
	}

};

class Shot : public Object
{
private:
	ShotTr modificator;
	int num;
	bool invulnerability;
	int speed;
	Player* player;

	virtual bool check_confines(int x, int y);
public:
	Shot(SDL_Renderer* ren, SDL_Texture* texture, int num, Player* player);
	ShotTr get_modificator();
	void upgrade(ShotTr tr);
	bool getInv();
	bool go();
};


class Player : public Object
{
private:
	int speed;
	Timer* timer;
	int health;
	int maxHealth;
	int point;
	int totalPoint;
	ShotTr tr;

	SDL_Texture** fire_textures;//6 + 6
	Object* fire[3]; // 2 + 1

	void add_shot(SDL_Texture* texture, int x, int y, int w, int h, int position)
	{
		Shot* someShot = new Shot(ren, texture, position, this);
		someShot->upgrade(tr);
		someShot->move(x, y);
		someShot->resizeOn(w, h);
		ShotList.push_back(someShot);
	}

public:
	Player(SDL_Renderer* ren, SDL_Texture* texture, SDL_Texture** fire_texture) : Object(ren, texture)
	{
		timer = new Timer;
		point = 0;
		totalPoint = 0;
		health = 100;
		maxHealth = 100;
		speed = 10;
		tr = NONE;

		fire_textures = fire_texture;
		fire[0] = new Object(ren, fire_texture[0]);
		fire[0]->resize(12, 26);
		fire[1] = new Object(ren, fire_texture[0]);
		fire[1]->resize(12, 26);
		fire[2] = new Object(ren, fire_texture[0]);
		fire[2]->resize(16, 50);

	}

	void render(int i)
	{
		SDL_RenderCopy(ren, texture, NULL, rect);


		fire[0]->change_textures(fire_textures[(i / 2) % 6]);
		fire[1]->change_textures(fire_textures[(i / 2) % 6]);
		fire[2]->change_textures(fire_textures[6 + ((i / 2) % 6)]);

		fire[0]->render();
		fire[1]->render();
		fire[2]->render();
	}

	int playerSpeed()
	{
		return speed;
	}

	void upSpeed(double tmp)
	{
		speed = tmp;
	}

	int getPoints()
	{
		return point;
	}

	void addTotalPoint(int s)
	{
		totalPoint += s;
	}

	int getTotalPoint()
	{
		return totalPoint;
	}

	bool spendPoints(int cash)
	{
		if (cash <= point)
		{
			point -= cash;
			return true;
		}
		else
			return false;
	}

	void makeDamage()
	{
		health -= 10;
	}

	int getHealth()
	{
		return health;
	}

	void fullHealth()
	{
		health = maxHealth;
	}

	void upMaxHealth(int tmp)
	{
		maxHealth = tmp;
	}

	void setModificator(ShotTr t)
	{
		timer->reset();
		tr = t;
	}

	double getModificatorTime()
	{
		return timer->elapsed();
	}

	ShotTr get_modificator()
	{
		return tr;
	}

	void addPoint(int somePoint)
	{
		point += somePoint;
	}

	void shoting(SDL_Event event)
	{
		if (event.key.keysym.sym == SDLK_SPACE)
		{
			switch (tr)
			{
			case RAY:
			{
				if ((ShotList.empty() || dynamic_cast<Shot*>(ShotList.back())->get_modificator() != RAY))
					add_shot(ray_texture, get_x(), get_y(), 10 * powerMulty, 18, 2);
				break;
			}
			case TRIPLE:
			{
				add_shot(shot_texture, get_x() + 10, get_y() + 75, 10 * powerMulty, 18 * powerMulty, 1);
				add_shot(shot_texture, get_x(), get_y(), 10 * powerMulty, 18 * powerMulty, 2);
				add_shot(shot_texture, get_x() - 10, get_y() + 75, 10 * powerMulty, 18 * powerMulty, 3);
				break;
			}
			case DUET:
			{
				add_shot(shot_texture, get_x() + 10, get_y() + 75, 10 * powerMulty, 18 * powerMulty, 1);
				add_shot(shot_texture, get_x() - 10, get_y() + 75, 10 * powerMulty, 18 * powerMulty, 3);
				break;
			}
			default:
			{
				add_shot(shot_texture, get_x(), get_y(), 10 * powerMulty, 18 * powerMulty, 2);
			}
			}
		}
	}

	void moving()
	{
		SDL_PumpEvents();

		if (((keyboardState[SDL_SCANCODE_UP]) || (keyboardState[SDL_SCANCODE_W])) && check_confines(0, -speed))
			move(0, -speed);

		if (((keyboardState[SDL_SCANCODE_DOWN]) || (keyboardState[SDL_SCANCODE_S])) && check_confines(0, speed))
			move(0, speed);

		if (((keyboardState[SDL_SCANCODE_LEFT]) || (keyboardState[SDL_SCANCODE_A])) && check_confines(-speed, 0))
			move(-speed, 0);

		if (((keyboardState[SDL_SCANCODE_RIGHT]) || (keyboardState[SDL_SCANCODE_D])) && check_confines(speed, 0))
			move(speed, 0);

		fire[0]->moveTo(get_x() + 20, get_y() + get_h() - 46);
		fire[1]->moveTo(get_x() + get_w() - 34, get_y() + get_h() - 46);
		fire[2]->moveTo(get_x() + get_w() / 2 - 9, get_y() + get_h() - 28);

	}
};

class Enemy : public Object
{
private:
	int speed;
	int points;
	Player* player;
	Trajectory traj;
	int i;

	bool check_confines(int x, int y)
	{
		int offset = 30;
		if (rect->x + x < displayMode.w && rect->x + x > 0 && rect->y + y - offset < displayMode.h)
			return true;
		else if (rect->y + y > displayMode.h)
			player->makeDamage();
		return false;
	}

public:
	Enemy(SDL_Renderer* ren, SDL_Texture* texture, int points, int speed, Trajectory tr, Player* pl) : Object(ren, texture)
	{
		player = pl;
		this->points = points;
		this->speed = speed;
		traj = tr;
		i = 0;
	}

	~Enemy()
	{
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
			if (isCrash(rect->x, rect->y, rect->w, rect->h) || i)
			{
				if (i != 7)
				{
					change_textures(boom_texture[i++]);
					render();
					return true;
				}
				else
				{
					player->addTotalPoint(points);
					player->addPoint(points);
				}
			}
			else
			{
				render();
				return true;
			}
		}
		return false;
	}
};



class Heal : public Object
{
protected:
	int speed;
	Player* player;
public:
	Heal(SDL_Renderer* ren, SDL_Texture* texture, Player* pl) : Object(ren, texture)
	{
		speed = 2;
		player = pl;
	}
	virtual ~Heal()
	{
	}
	virtual bool go()
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
};

class Bonus : public Heal
{
private:
	ShotTr tr;
public:
	Bonus(SDL_Renderer* ren, SDL_Texture* texture, Player* pl, ShotTr tr) : Heal(ren, texture, pl)
	{
		this->tr = tr;
	}
	~Bonus()
	{
	}
	bool go()
	{
		move(0, speed);

		if (check_confines(0, speed))
		{
			if (isCrash(rect->x, rect->y, rect->w, rect->h))
			{
				player->setModificator(tr);
				return false;
			}
			else
			{
				render();
				return true;
			}
		}
	}
};

#include "methods.inl"
#endif CLASSES
