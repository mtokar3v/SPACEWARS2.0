#pragma once

//####### SCREEN ##########
SDL_Renderer* ren;
SDL_DisplayMode displayMode;
SDL_Window* win;
SDL_Surface* surface;
const int w = displayMode.w;
const int h = displayMode.h;

//####### CLASSES ########
enum Trajectory;
enum ShotTr;
class Object;
class Shot;
class Enemy;
class Player;
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
std::vector<SDL_Texture*> enemy_textures;
SDL_Texture* background_texture;
SDL_Texture* mainMenu_texture;
SDL_Texture* player_texture;
SDL_Texture* shot_texture;
SDL_Texture* bonus_texture;
SDL_Texture* heal_texture;
SDL_Texture* spaceShop_texture;
SDL_Texture* sparkle_texture;
SDL_Texture* master_texture;
SDL_Texture* master_damage_texture;
SDL_Texture* rainbow_texture;
TTF_Font* font;
TTF_Font* bigFont;
SDL_Color color;

//######### FUNCTIONS ########
bool init();
void move_dimanic_object();
void add_shot(SDL_Renderer* ren, SDL_Texture* texture, int x, int y, int w, int h, int position, ShotTr tr, Player& player);
void add_text(SDL_Renderer* ren, TTF_Font* font, SDL_Color color, std::string msg, int x, int y);
bool isCrash(int x, int y, int w, int h);
void spawningBonus(int time, bool &bonusSpawn);