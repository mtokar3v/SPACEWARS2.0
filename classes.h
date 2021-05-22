#pragma once
#include "data.h"

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
	DUET,			//выстрелы с двух сторон
	TRIPLE,			//выстрелы  в три стороны
	RAY,			//луч
	SHOTCOUNT		//количество режимов стрельбы
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
		int offset = 30;
		if (rect->x + x + offset < displayMode.w && rect->x + x + offset > 0 && rect->y + y + offset < displayMode.h && rect->y + y + offset > 0)
			return true;
		return false;
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

	void render()
	{
		SDL_RenderCopy(ren, texture, NULL, rect);
	}

	virtual bool go() { return true; }

	virtual ~Object()
	{
	}

};

class Player : public Object
{
private:
	static int speed;
	static Timer* timer;
	static int health;
	static int maxHealth;
	static int point;
	static int totalPoint;
	static ShotTr tr;
public:
	Player(SDL_Renderer* ren, SDL_Texture* texture) : Object(ren, texture)
	{
	}
	static int playerSpeed()
	{
		return speed;
	}

	static void upSpeed(double tmp)
	{
		speed = tmp;
	}

	static int getPoints()
	{
		return point;
	}

	static void addTotalPoint(int s)
	{
		totalPoint += s;
	}
	
	static int getTotalPoint()
	{
		return totalPoint;
	}

	static bool spendPoints(int cash)
	{
		if (cash <= point)
		{
			point -= cash;
			return true;
		}
		else
			return false;
	}

	static void makeDamage()
	{
		health -= 10;
	}

	static int getHealth()
	{
		return health;
	}

	static void fullHealth()
	{
		health = maxHealth;
	}

	static void upMaxHealth(int tmp)
	{
		maxHealth = tmp;
	}

	static void setModificator(ShotTr t)
	{
		timer->reset();
		tr = t;
	}

	static double getModificatorTime()
	{
		return timer->elapsed();
		
	}

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
	Trajectory traj;

	bool check_confines(int x, int y)
	{
		int offset = 30;
		if (rect->x + x < displayMode.w && rect->x + x > 0 && rect->y + y - offset < displayMode.h	)
			return true;
		else if (rect->y + y > displayMode.h)
			Player::makeDamage();
		return false;
	}
public:
	Enemy(SDL_Renderer* ren, SDL_Texture* texture, int points, int speed, Trajectory tr) : Object(ren, texture)
	{
		this->points = points;
		this->speed = speed;
		traj = tr;
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
			if (isCrash(rect->x, rect->y, rect->w, rect->h))
			{
				Player::addTotalPoint(points);
				Player::addPoint(points);
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

class Shot : public Object
{
private:
	ShotTr modificator;
	int num;
	bool invulnerability;
	int speed;

	virtual bool check_confines(int x, int y)
	{
		int offset = 250;
		if (rect->y + y + offset > 0)
			return true;
		return false;
	}
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
	~Shot()
	{
	}
	void upgrade(ShotTr tr)
	{
		modificator = tr;
	}

	bool getInv()
	{
		return invulnerability;
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

class Heal : public Object
{
protected:
	int speed;
public:
	Heal(SDL_Renderer* ren, SDL_Texture* texture) : Object(ren, texture)
	{
		speed = 2;
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
				Player::fullHealth();
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
	Bonus(SDL_Renderer* ren, SDL_Texture* texture, ShotTr tr) : Heal(ren, texture)
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
				Player::setModificator(tr);
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

