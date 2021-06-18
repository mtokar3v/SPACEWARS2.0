#pragma once

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

	void add_shot(SDL_Texture* texture, int x, int y, int w, int h, int position);
	bool check_confines(int x, int y);
	

public:
	Player(SDL_Renderer* ren, SDL_Texture* texture, SDL_Texture** fire_texture);
	void render(int i);
	int playerSpeed();
	void upSpeed(double tmp);
	int getPoints();
	void addTotalPoint(int s);
	int getTotalPoint();
	bool spendPoints(int cash);
	void makeDamage();
	int getHealth();
	void fullHealth();
	void upMaxHealth(int tmp);
	void setModificator(ShotTr t);
	double getModificatorTime();
	ShotTr get_modificator();
	void addPoint(int somePoint);
	void shoting(SDL_Event event);
	void moving();
};