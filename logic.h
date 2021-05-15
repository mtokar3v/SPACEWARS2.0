#pragma once
#include "textures.h"
#include "classes.h"

Timer* Player::timer = new Timer();

bool init()
{
	if (SDL_Init(SDL_INIT_VIDEO))
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return false;
	}

	if (SDL_GetDesktopDisplayMode(0, &displayMode))
	{
		std::cout << "SDL_display Error: " << SDL_GetError() << std::endl;
		return false;
	}

	win = SDL_CreateWindow("SDL_testing", 0, 0, displayMode.w, displayMode.h, SDL_WINDOW_SHOWN);

	if (win == nullptr)
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		return false;
	}

	if (TTF_Init() < 0)
	{
		std::cout << "Error initializing SDL_ttf: " << TTF_GetError() << std::endl;
		return false;
	}

	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (!ren)
	{
		std::cout << "Error creating renderer: " << SDL_GetError() << std::endl;
		return false;
	}
	SDL_SetRenderDrawColor(ren, 0x00,0x00, 0x00, 0xFF);
	std::cout << "init complete sucsesseful" << std::endl;
	return true;
}

void move_dimanic_object()
{
	std::vector<Object*> copy;
	auto newEnd = std::remove_if(EnemyList.begin(), EnemyList.end(),
		[&copy](Object* tmp) {
			if (!tmp->go())
			{
				copy.push_back(tmp);
				return true;
			}
			else
				return false; });
	EnemyList.erase(newEnd, EnemyList.end());

	newEnd = std::remove_if(ShotList.begin(), ShotList.end(),
		[&copy](Object* tmp) {
			if (!tmp->go())
			{
				copy.push_back(tmp);
				return true;
			}
			else
				return false; });
	ShotList.erase(newEnd, ShotList.end()); 

	std::vector<Object*>::iterator at;
	for (at = copy.begin(); at != copy.end(); at++)
		delete (*at);

	copy.clear();
}

//############# ВЫСТРЕЛЫ #################
void shoting(SDL_Event event, Player& player)
{
	if (event.key.keysym.sym == SDLK_SPACE)
	{
		ShotTr tr = Player::getModificator();
		switch (tr)
		{
		case RAY:
		{
			add_shot(ren, shot_texture, 0, -300, 10, displayMode.h / 2, 2, tr, player);
			break;
		}
		case TRIPLE:
		{
			add_shot(ren, shot_texture, 0, 0, 10, 18, 1, tr, player);
			add_shot(ren, shot_texture, 0, 0, 10, 18, 2, tr, player);
			add_shot(ren, shot_texture, 0, 0, 10, 18, 3, tr, player);
			break;
		}
		case DOUBLE:
		{
			add_shot(ren, shot_texture, 0, 0, 10, 18, 1, tr, player);
			add_shot(ren, shot_texture, 0, 0, 10, 18, 3, tr, player);
			break;
		}
		default:
		{
			add_shot(ren, shot_texture, 0, 0, 10, 18, 2, tr, player);
		}
		}
	}
}

//############# ПЕРЕМЕЩЕНИЕ #################
void moving(Player& player)
{
	SDL_PumpEvents();

	if (((keyboardState[SDL_SCANCODE_UP]) || (keyboardState[SDL_SCANCODE_W])) && player.check_confines(0, -player.playerSpeed()))
		player.move(0, -player.playerSpeed());

	if (((keyboardState[SDL_SCANCODE_DOWN]) || (keyboardState[SDL_SCANCODE_S])) && player.check_confines(0, player.playerSpeed()))
		player.move(0, player.playerSpeed());

	if (((keyboardState[SDL_SCANCODE_LEFT]) || (keyboardState[SDL_SCANCODE_A])) && player.check_confines(-player.playerSpeed(), 0))
		player.move(-player.playerSpeed(), 0);

	if (((keyboardState[SDL_SCANCODE_RIGHT]) || (keyboardState[SDL_SCANCODE_D])) && player.check_confines(player.playerSpeed(), 0))
		player.move(player.playerSpeed(), 0);

}

void spawningEnemy(int time, int spawnTime, bool &enemySpawn)
{
	if (!(time % spawnTime) && enemySpawn)
	{
		enemySpawn = false;

		int rnd = rand() % 10 + 10;
		Trajectory tr = static_cast<Trajectory>(rand() % TRAJCOUNT);
		int x = rand() % displayMode.w;
		int y = 0;

		std::cout << rnd << ":" << tr << std::endl;
		for (int i = 0; i < rnd; i++)
		{
			switch (tr)
			{
			case WALL:
			{
				Enemy* someEnemy = new Enemy(ren, enemy_textures[1], 100, enemySpeed, tr);
				someEnemy->moveTo(x += 30, y -= 30);
				someEnemy->resizeOn(24, 24);
				EnemyList.push_back(someEnemy);
				break;
			}
			case ARROW:
			{
				Enemy* someEnemy = new Enemy(ren, enemy_textures[0], 200, enemySpeed + 1, tr);
				if (i % 2)
					someEnemy->moveTo(x += 15 * i, y -= 30);
				else
					someEnemy->moveTo(x -= 15 * i, y);

				someEnemy->resizeOn(24, 24);
				EnemyList.push_back(someEnemy);
				break;
			}
			case LINE:
			{
				Enemy* someEnemy = new Enemy(ren, enemy_textures[2], 50, enemySpeed, tr);
				someEnemy->moveTo(x, y -= 30);
				someEnemy->resizeOn(24, 24);
				EnemyList.push_back(someEnemy);
				break;
			}
			//sin & cos
			default:
			{
				Enemy* someEnemy = new Enemy(ren, enemy_textures[3], 200, enemySpeed + 2, tr);
				someEnemy->moveTo(x, y -= 30);
				someEnemy->resizeOn(24, 24);
				EnemyList.push_back(someEnemy);
				break;
			}
			}
		}
	}
	else if (time % spawnTime)
		enemySpawn = true;
}

//добавление бонуса на поле
void spawningBonus(int time, bool& bonusSpawn)
{
	if (!(time % 16) && bonusSpawn && time)
	{
		bonusSpawn = false;

		ShotTr tr = static_cast<ShotTr>(rand() % SHOTCOUNT);
		int x = rand() % displayMode.w;
		int y = 0;

		Bonus* bonus = new Bonus(ren, bonus_texture, tr);
		bonus->moveTo(x, y);
		bonus->resizeOn(40, 40);
		EnemyList.push_back(bonus);
	}
	else if (time % 16)
		bonusSpawn = true;
}

//сбрасывание модификаций через определенное время
void resetBonus(int time, bool bonusRespawn)
{
	if (!(time % 8) && time && bonusRespawn)
	{
		Player::setModificator(NONE);
		bonusRespawn = false;
	}
	else if (time % 8)
		bonusRespawn = true;
}

//информация о текущей сессии 
void inputInfo(int time)
{
	add_text(ren, font, color, "Wave: ", 0, 0);
	add_text(ren, font, color, std::to_string(wave), 45, 0);

	add_text(ren, font, color, "Time: ", 0, 25);
	add_text(ren, font, color, std::to_string(time), 45, 25);

	add_text(ren, font, color, "Point: ", 0, 50);
	add_text(ren, font, color, std::to_string(Player::getPoints()), 45, 50);

	add_text(ren, font, color, "Health: ", 0, 75);
	add_text(ren, font, color, std::to_string(Player::getHealth()), 45, 75);

	add_text(ren, font, color, "Modificator: ", 0, 100);
	std::string mode;
	switch (Player::getModificator())
	{
	case NONE: mode = "NONE"; break;
	case DOUBLE: mode = "DOUBLE"; break;
	case TRIPLE: mode = "TRIPLE"; break;
	case RAY: mode = "RAY"; break;
	}
	add_text(ren, font, color, mode, 80, 100);
}

//void create_object(int x, int y, int w, int h, SDL_Rect& rect, SDL_Texture* texture, std::string textureWay)
//{
//	rect = { x, y, w, h };
//	texture = IMG_LoadTexture(ren, textureWay.c_str());
//	SDL_RenderCopy(ren, texture, NULL, &rect);
//}

void add_shot(SDL_Renderer* ren, SDL_Texture* texture, int x, int y, int w, int h, int position, ShotTr tr, Player& player)
{
	Shot* someShot = new Shot(ren, texture, position);
	someShot->upgrade(tr);
	someShot->move(player.get_x() + x, player.get_y() + y);
	someShot->resizeOn(w, h);
	ShotList.push_back(someShot);
}

void add_text(SDL_Renderer* ren, TTF_Font* font, SDL_Color color, std::string msg, int x, int y)
{
	SDL_Surface* text = TTF_RenderText_Solid(font, msg.c_str(), color);

	if (!text)
	{
		std::cout << "Failed to render text: " << TTF_GetError();
	}
	else
	{
		SDL_Texture* text_texture = SDL_CreateTextureFromSurface(ren, text);
		SDL_Rect dest = { x, y, text->w, text->h };
		SDL_RenderCopy(ren, text_texture, NULL, &dest);
		SDL_DestroyTexture(text_texture);
	}
	SDL_FreeSurface(text);
}

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

void shopping(SDL_Renderer* ren, SDL_Texture*)
{
	bool run = true;
	SDL_RenderClear(ren);
	Object shop(ren, background_texture);
	shop.resizeOn(w, h);
	shop.render();
	SDL_RenderPresent(ren);



	while (run)
	{
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT: run = false; break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT && event.button.x <= 10 && event.button.y <= 10)
				run = false; break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				run = false;
			break;
		}
	}
}