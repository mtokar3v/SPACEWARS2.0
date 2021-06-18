#pragma once

//####### SCREEN ##########
SDL_Renderer* ren;
SDL_DisplayMode displayMode;
SDL_Window* win;
SDL_Surface* surface;
const int w = displayMode.w;
const int h = displayMode.h;

//####### CLASSES ########
class Object;
class Boom;
class Shot;
class Player;
class Shot;
class Enemy;
class Shot;
class Heal;
class Bonus;
class Timer;

//####### GAME ###########
bool run = true;
bool bonusRespawn = true;
bool enemySpawn = true;
bool bonusSpawn = true;
bool levelUp = true;
int enemySpeed = 1;
int spawnTime = 5;
int wave = 1;
double powerMulty = 1;
SDL_Event event;
const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
std::string timeNow;

std::vector<Object*> ShotList;
std::vector<Object*> EnemyList;
std::vector<Object*> BoomList;
std::vector<SDL_Texture*> enemy_textures;
SDL_Texture* background_texture;
SDL_Texture* mainMenu_texture;
SDL_Texture* player_texture;
SDL_Texture* shot_texture;
SDL_Texture* ray_texture;
SDL_Texture* bonus_texture;
SDL_Texture* heal_texture;
SDL_Texture* spaceShop_texture;
SDL_Texture* sparkle_texture;
SDL_Texture* master_texture;
SDL_Texture* master_damage_texture;
SDL_Texture* rainbow_texture;
SDL_Texture* fire_texture[12];
SDL_Texture* boom_texture[7];
TTF_Font* font;
TTF_Font* bigFont;
SDL_Color color;

//######### FUNCTIONS ########
bool init();
void move_dimanic_object();
void add_text(SDL_Renderer* ren, TTF_Font* font, SDL_Color color, std::string msg, int x, int y);
bool isCrash(int x, int y, int w, int h);
void spawningEnemy(int time, int spawnTime, bool& enemySpawn, Player* player, SDL_Texture** boom_t);
void spawningBonus(int time, bool& bonusSpawn, Player* player);
void resetBonus(int time, bool bonusRespawn, Player* player);
void inputInfo(int time, Player* player);
void clearBattleArea();
void shopping(Player* player);

enum Trajectory
{
	LINE,
	WALL,
	SINUSOID,
	COSINUSOID,
	ARROW,
	TRAJCOUNT
};

enum ShotTr
{
	NONE,
	DUET,
	TRIPLE,
	RAY,
	SHOTCOUNT
};

