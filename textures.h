#pragma once
#include "data.h"

void setTextures()
{
	SDL_RenderClear(ren);

	//������
	background_texture = IMG_LoadTexture(ren, "image//space.png");

	//������� ������
	player_texture = IMG_LoadTexture(ren, "image//player1.png");

	//������� �����
	fire_texture[0] = IMG_LoadTexture(ren, "image//fire//fire1.png");
	fire_texture[1] = IMG_LoadTexture(ren, "image//fire//fire2.png");
	fire_texture[2] = IMG_LoadTexture(ren, "image//fire//fire3.png");
	fire_texture[3] = IMG_LoadTexture(ren, "image//fire//fire4.png");
	fire_texture[4] = IMG_LoadTexture(ren, "image//fire//fire5.png");
	fire_texture[5] = IMG_LoadTexture(ren, "image//fire//fire6.png");

	//����� �����
	fire_texture[6] = IMG_LoadTexture(ren, "image//fire//blue_fire_1.png");
	fire_texture[7] = IMG_LoadTexture(ren, "image//fire//blue_fire_2.png");
	fire_texture[8] = IMG_LoadTexture(ren, "image//fire//blue_fire_3.png");
	fire_texture[9] = IMG_LoadTexture(ren, "image//fire//blue_fire_4.png");
	fire_texture[10] = IMG_LoadTexture(ren, "image//fire//blue_fire_5.png");
	fire_texture[11] = IMG_LoadTexture(ren, "image//fire//blue_fire_6.png");


	//������� ����������
	enemy_textures.resize(4);
	enemy_textures[0] = IMG_LoadTexture(ren, "image//enemy1.png");
	enemy_textures[1] = IMG_LoadTexture(ren, "image//enemy2.png");
	enemy_textures[2] = IMG_LoadTexture(ren, "image//enemy3.png");
	enemy_textures[3] = IMG_LoadTexture(ren, "image//enemy4.png");
	sparkle_texture = IMG_LoadTexture(ren, "image//sparkle.png");

	boom_texture[0] = IMG_LoadTexture(ren, "image//boom//boom_1.png");
	boom_texture[1] = IMG_LoadTexture(ren, "image//boom//boom_2.png");
	boom_texture[2] = IMG_LoadTexture(ren, "image//boom//boom_3.png");
	boom_texture[3] = IMG_LoadTexture(ren, "image//boom//boom_4.png");
	boom_texture[4] = IMG_LoadTexture(ren, "image//boom//boom_5.png");
	boom_texture[5] = IMG_LoadTexture(ren, "image//boom//boom_6.png");
	boom_texture[6] = IMG_LoadTexture(ren, "image//boom//boom_7.png");

	//����
	shot_texture = IMG_LoadTexture(ren, "image//fire.png");//18x10
	ray_texture = IMG_LoadTexture(ren, "image//ray.png");//18x10

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