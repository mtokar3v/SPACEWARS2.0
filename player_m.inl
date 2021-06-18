#ifndef PLAYER
#define PLAYER

void Player::add_shot(SDL_Texture* texture, int x, int y, int w, int h, int position)
{
	Shot* someShot = new Shot(ren, texture, position, this);
	someShot->upgrade(tr);
	someShot->move(x, y);
	someShot->resizeOn(w, h);
	ShotList.push_back(someShot);
}

bool Player::check_confines(int x, int y)
{
	int offset = 100;
	if (rect->x + x + rect->w - offset < displayMode.w && rect->x + x + offset> 0 && rect->y + y + rect->h - offset < displayMode.h && rect->y + y + offset> 0)
		return true;
	return false;
}

Player::Player(SDL_Renderer* ren, SDL_Texture* texture, SDL_Texture** fire_texture) : Object(ren, texture)
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

void Player::render(int i)
{
	SDL_RenderCopy(ren, texture, NULL, rect);

	fire[0]->change_textures(fire_textures[(i / 2) % 6]);
	fire[1]->change_textures(fire_textures[(i / 2) % 6]);
	fire[2]->change_textures(fire_textures[6 + ((i / 2) % 6)]);

	fire[0]->render();
	fire[1]->render();
	fire[2]->render();
}

int Player::playerSpeed()
{
	return speed;
}

void Player::upSpeed(double tmp)
{
	speed = tmp;
}

int Player::getPoints()
{
	return point;
}

void Player::addTotalPoint(int s)
{
	totalPoint += s;
}

int Player::getTotalPoint()
{
	return totalPoint;
}

bool Player::spendPoints(int cash)
{
	if (cash <= point)
	{
		point -= cash;
		return true;
	}
	else
		return false;
}

void Player::makeDamage()
{
	health -= 10;
}

int Player::getHealth()
{
	return health;
}

void Player::fullHealth()
{
	health = maxHealth;
}

void Player::upMaxHealth(int tmp)
{
	maxHealth = tmp;
}

void Player::setModificator(ShotTr t)
{
	timer->reset();
	tr = t;
}

double Player::getModificatorTime()
{
	return timer->elapsed();
}

ShotTr Player::get_modificator()
{
	return tr;
}

void Player::addPoint(int somePoint)
{
	point += somePoint;
}

void Player::shoting(SDL_Event event)
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

void Player::moving()
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

#endif PLAYER