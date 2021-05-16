#pragma once
#include "data.h"

void setTextures()
{
	SDL_RenderClear(ren);

	//Космос
	background_texture = IMG_LoadTexture(ren, "image//space.png");

	//Корабль игрока
	player_texture = IMG_LoadTexture(ren, "image//player1.png");

	//Корабли проивников
	enemy_textures.resize(4);
	enemy_textures[0] = IMG_LoadTexture(ren, "image//enemy1.png");
	enemy_textures[1] = IMG_LoadTexture(ren, "image//enemy2.png");
	enemy_textures[2] = IMG_LoadTexture(ren, "image//enemy3.png");
	enemy_textures[3] = IMG_LoadTexture(ren, "image//enemy4.png");
	sparkle_texture = IMG_LoadTexture(ren, "image//sparkle.png");
	//Пуля
	shot_texture = IMG_LoadTexture(ren, "image//fire.png");//18x10

	//Бонус
	bonus_texture = IMG_LoadTexture(ren, "image//bonus.png");//400x400

	//шрифт
	font = TTF_OpenFont("fonts//Thintel.ttf", 25);
	bigFont = TTF_OpenFont("fonts//Thintel.ttf", 60);
	color = { 0xff, 0xff, 0xff };
	if (!font)
	{
		std::cout << "TTF_OpenFont";
		return;
	}

	//хилка
	heal_texture = IMG_LoadTexture(ren, "image//heal.png");

	//магазин
	spaceShop_texture = IMG_LoadTexture(ren, "image//spaceshop.png");
		
	//босс
	master_texture = IMG_LoadTexture(ren, "image//boss.png");
	master_damage_texture = IMG_LoadTexture(ren, "image//bossDamage.png");

	//радуга босса
	rainbow_texture = IMG_LoadTexture(ren, "image//rainbow.png");

	//главное меню
	mainMenu_texture = IMG_LoadTexture(ren, "image//mainmenu.png");
}