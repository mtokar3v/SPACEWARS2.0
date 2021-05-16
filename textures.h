#pragma once
#include "data.h"

void setTextures()
{
	SDL_RenderClear(ren);

	//������
	background_texture = IMG_LoadTexture(ren, "image//space.png");

	//������� ������
	player_texture = IMG_LoadTexture(ren, "image//player1.png");

	//������� ����������
	enemy_textures.resize(4);
	enemy_textures[0] = IMG_LoadTexture(ren, "image//enemy1.png");
	enemy_textures[1] = IMG_LoadTexture(ren, "image//enemy2.png");
	enemy_textures[2] = IMG_LoadTexture(ren, "image//enemy3.png");
	enemy_textures[3] = IMG_LoadTexture(ren, "image//enemy4.png");
	sparkle_texture = IMG_LoadTexture(ren, "image//sparkle.png");
	//����
	shot_texture = IMG_LoadTexture(ren, "image//fire.png");//18x10

	//�����
	bonus_texture = IMG_LoadTexture(ren, "image//bonus.png");//400x400

	//�����
	font = TTF_OpenFont("fonts//Thintel.ttf", 25);
	bigFont = TTF_OpenFont("fonts//Thintel.ttf", 60);
	color = { 0xff, 0xff, 0xff };
	if (!font)
	{
		std::cout << "TTF_OpenFont";
		return;
	}

	//�����
	heal_texture = IMG_LoadTexture(ren, "image//heal.png");

	//�������
	spaceShop_texture = IMG_LoadTexture(ren, "image//spaceshop.png");
		
	//����
	master_texture = IMG_LoadTexture(ren, "image//boss.png");
	master_damage_texture = IMG_LoadTexture(ren, "image//bossDamage.png");

	//������ �����
	rainbow_texture = IMG_LoadTexture(ren, "image//rainbow.png");

	//������� ����
	mainMenu_texture = IMG_LoadTexture(ren, "image//mainmenu.png");
}