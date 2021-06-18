#pragma once

extern SDL_Renderer* ren;
extern SDL_DisplayMode displayMode;

class Bonus : public Heal
{
private:
	ShotTr tr;
public:
	Bonus(SDL_Renderer* ren, SDL_Texture* texture, Player* pl, ShotTr tr);
	~Bonus();
	bool go();
};

Bonus::Bonus(SDL_Renderer* ren, SDL_Texture* texture, Player* pl, ShotTr tr) : Heal(ren, texture, pl)
{
	this->tr = tr;
}
Bonus::~Bonus()
{
}
bool Bonus::go()
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