#pragma once

class Object
{
protected:
	SDL_Rect* rect;
	SDL_Texture* texture;
	SDL_Renderer* ren;

public:
	Object(SDL_Renderer* ren, SDL_Texture* texture);
	virtual bool check_confines(int x, int y);
	void change_textures(SDL_Texture* texture);
	void move(double x, double y);
	void moveTo(int x, int y);
	void resizeOn(int w, int h);
	void resize(int w, int h);
	int get_x();
	int get_y();
	int get_w();
	int get_h();
	virtual void render();
	virtual bool go();
	virtual ~Object(){}

};

Object::Object(SDL_Renderer* ren, SDL_Texture* texture)
{
	this->ren = ren;
	rect = new SDL_Rect;
	this->texture = texture;
	*rect = { 0, 0, 0, 0 };
}

bool Object::check_confines(int x, int y)
{
	if (rect->x + x + rect->w < displayMode.w && rect->x + x > 0 && rect->y + y + rect->h < displayMode.h && rect->y + y > 0)
		return true;
	return false;
}

void Object::change_textures(SDL_Texture* texture)
{
	this->texture = texture;
}

void Object::move(double x, double y)
{
	rect->x += x;
	rect->y += y;
}

void Object::moveTo(int x, int y)
{
	rect->x = x;
	rect->y = y;
}

void Object::resizeOn(int w, int h)
{
	rect->w = w;
	rect->h = h;
}

void Object::resize(int w, int h)
{
	rect->w += w;
	rect->h += h;
}

int Object::get_x()
{
	return rect->x;
}

int Object::get_y()
{
	return rect->y;
}

int Object::get_w()
{
	return rect->w;
}

int Object::get_h()
{
	return rect->h;
}

void Object::render()
{
	SDL_RenderCopy(ren, texture, NULL, rect);
}

bool Object::go() { return true; }


