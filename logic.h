#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "classes.h"
#include <time.h>
#include <random>
#include "stuff.h"

SDL_Renderer* ren;
SDL_DisplayMode displayMode;
SDL_Window* win;
SDL_Surface* surface;
const int w = displayMode.w;
const int h = displayMode.h;

extern enum Trajectory;
extern enum ShotTr;

std::vector<Object*> ShotList;
std::vector<Object*> EnemyList;
int Player::point = 0;
int Player::health = 10000;
ShotTr Player::tr = NONE;
int spawnTime = 3;

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

//void render_static_object()
//{
//	std::vector<Object>::iterator at;
//	for (at = objectList.begin(); at != objectList.end(); at++)
//		(*at).render();
//}

void move_dimanic_object()
{
	auto newEnd = std::remove_if(EnemyList.begin(), EnemyList.end(),
		[](Object* tmp) {
			return !tmp->go(); });
	EnemyList.erase(newEnd, EnemyList.end());

	newEnd = std::remove_if(ShotList.begin(), ShotList.end(),
		[](Object* tmp) {
			return !tmp->go(); });
	ShotList.erase(newEnd, ShotList.end());
}

void play()
{
	srand(time(0));
	if(!init())
		return;
	SDL_RenderClear(ren);

	//Космос
	SDL_Texture* background_texture = IMG_LoadTexture(ren, "image//space.png");
	Object backgroung(ren, background_texture);
	backgroung.resizeOn(displayMode.w, displayMode.h); //640x360
	backgroung.render();

	//Корабль игрока
	SDL_Texture* player_texture = IMG_LoadTexture(ren, "image//player1.png");
	Player player(ren, player_texture);	
	player.resizeOn(122, 165); //165x122
	player.moveTo(displayMode.w/2 - 61, displayMode.h - 165);
	player.render();

	//Корабль противника1
	SDL_Texture* enemy1_texture = IMG_LoadTexture(ren, "image//enemy1.png");//126x122

	//Пуля
	SDL_Texture* shot_texture = IMG_LoadTexture(ren, "image//fire.png");//18x10
	SDL_RenderPresent(ren);

	//Бонус
	SDL_Texture* bonus_texture = IMG_LoadTexture(ren, "image//bonus.png");//400x400

	//шрифт
	TTF_Font* font = TTF_OpenFont("fonts//Thintel.ttf", 25);
	SDL_Color color = { 0xff, 0xff, 0xff };
	if (!font)
	{
		std::cout << "TTF_OpenFont";
		return;
	}


	bool run = true;
	bool enemySpawn = true;
	bool bonusSpawn = true;
	SDL_Event event;
	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
	std::string time;

	Timer t;
	while (run)
	{
		if (Player::getHealth() < 0)
		{
			run = false;
			break;
		}
		//чтобы замедлить обработку и движение корабля
		SDL_Delay(30);

		SDL_PollEvent(&event);
		//считывались все эвенты, даже одновременные, поэтому можно бегать о диагонали 
		SDL_PumpEvents();

		switch (event.type)
		{
		case SDL_QUIT: run = false; break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT && event.button.x <= 10 && event.button.y <= 10)
				run = false; break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_0)
				Player::setModificator(NONE);
			//############# ВЫСТРЕЛЫ #################
			if (event.key.keysym.sym == SDLK_SPACE)
			{
				ShotTr tr =  Player::getModificator();
				
				std::cout << tr;
				switch (tr)
				{
				case RAY: 
				{
					Shot* someShot = new Shot(ren, shot_texture, 2);
					someShot->upgrade(RAY);
					someShot->move(player.get_x(), player.get_y() - 300);
					someShot->resizeOn(10, displayMode.h / 2);
					ShotList.push_back(someShot);
					break;
				}
				case TRIPLE:
				{
					Shot* someShot = new Shot(ren, shot_texture, 1);
					someShot->upgrade(TRIPLE);
					someShot->move(player.get_x(), player.get_y());
					someShot->resizeOn(10, 18);
					ShotList.push_back(someShot);
					
					Shot* someShot2 = new Shot(ren, shot_texture, 2);
					someShot2->upgrade(TRIPLE);
					someShot2->move(player.get_x(), player.get_y());
					someShot2->resizeOn(10, 18);
					ShotList.push_back(someShot2);

					Shot* someShot3 = new Shot(ren, shot_texture, 3);
					someShot3->upgrade(TRIPLE);
					someShot3->move(player.get_x(), player.get_y());
					someShot3->resizeOn(10, 18);
					ShotList.push_back(someShot3);
					break;
				}
				case DOUBLE:
				{
					Shot* someShot = new Shot(ren, shot_texture, 1);
					someShot->move(player.get_x(), player.get_y());
					someShot->resizeOn(10, 18);
					someShot->upgrade(DOUBLE);
					ShotList.push_back(someShot);

					Shot* someShot2 = new Shot(ren, shot_texture, 3);
					someShot2->move(player.get_x(), player.get_y());
					someShot2->resizeOn(10, 18);
					someShot2->upgrade(DOUBLE);
					ShotList.push_back(someShot2);
					break;
				}
				default:
				{
					Shot* someShot = new Shot(ren, shot_texture, 2 );
					someShot->upgrade(tr);
					someShot->move(player.get_x(), player.get_y());
					someShot->resizeOn(10, 18);
					ShotList.push_back(someShot);
				}
				}
			}

			
			break;

		}

		//############# ПЕРЕМЕЩЕНИЕ #################
		if (((keyboardState[SDL_SCANCODE_UP]) || (keyboardState[SDL_SCANCODE_W])) && player.check_confines(0, -player.playerSpeed()))
			player.move(0, -player.playerSpeed());

		if (((keyboardState[SDL_SCANCODE_DOWN]) || (keyboardState[SDL_SCANCODE_S])) && player.check_confines(0, player.playerSpeed()))
			player.move(0, player.playerSpeed());


		if (((keyboardState[SDL_SCANCODE_LEFT]) || (keyboardState[SDL_SCANCODE_A])) && player.check_confines(-player.playerSpeed(), 0))
			player.move(-player.playerSpeed(), 0);

		if (((keyboardState[SDL_SCANCODE_RIGHT]) || (keyboardState[SDL_SCANCODE_D])) && player.check_confines(player.playerSpeed(), 0))
			player.move(player.playerSpeed(), 0);



		//спавн врагов
		if (!((int)t.elapsed() % spawnTime) && enemySpawn)
		{
			enemySpawn = false;

			int rnd = rand() % 20;
			Trajectory tr = static_cast<Trajectory>(rand() % TRAJCOUNT);
			int x = rand() % displayMode.w;
			int y = 0;
			for (int i = 0; i < rnd; i++)
			{
				Enemy* someEnemy = new Enemy(ren, enemy1_texture, 100, 3, tr);
				switch (tr)
				{
				case WALL:someEnemy->moveTo(x += 30, y -= 30); break;
				case ARROW:
					if (i % 2)
						someEnemy->moveTo(x += 30 * i, y -= 30);
					else
						someEnemy->moveTo(x -= 30 * i, y); break;
				default: someEnemy->moveTo(x, y -= 30);
				}
				someEnemy->resizeOn(24, 24);
				EnemyList.push_back(someEnemy);
			}
		}
		else if ((int)t.elapsed() % spawnTime)
			enemySpawn = true;

		//спавн модификаторов 
		if (!((int)t.elapsed() % (spawnTime * 1)) && bonusSpawn)
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
		else if ((int)t.elapsed() % (spawnTime * 1))
			bonusSpawn = true;

		SDL_RenderClear(ren);
		backgroung.render();

		add_text(ren, font, color, "Time: ", 0, 0);
		add_text(ren, font, color, std::to_string((int)t.elapsed()), 45, 0);

		add_text(ren, font, color, "Point: ", 0, 25);
		add_text(ren, font, color, std::to_string(Player::getPoints()), 45, 25);

		add_text(ren, font, color, "Health: ", 0, 50);
		add_text(ren, font, color, std::to_string(Player::getHealth()), 45,50);

		
		player.render();
		
		move_dimanic_object();
		
		SDL_RenderPresent(ren);
		
	}
	SDL_Quit();
	
}

void create_object(int x, int y, int w, int h, SDL_Rect& rect, SDL_Texture* texture, std::string textureWay)
{
	rect = { x, y, w, h };
	texture = IMG_LoadTexture(ren, textureWay.c_str());
	SDL_RenderCopy(ren, texture, NULL, &rect);
}

