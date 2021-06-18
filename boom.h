#pragma once

class Boom :public Object
{
private:
	int i;
	SDL_Texture** boom_texture;
public:
	Boom(SDL_Renderer* ren, SDL_Texture** t, int x, int y, int w, int h);
	bool go();
};

Boom::Boom(SDL_Renderer* ren, SDL_Texture** t, int x, int y, int w, int h) : Object(ren, t[0])
{
	move(x, y);
	resize(w, h);
	boom_texture = t;
	i = 0;
}

bool Boom::go()
{
	if ((i / 2) < 7)
	{
		change_textures(boom_texture[i++ / 2]);
		render();
		return true;
	}
	else
		return false;
}