#include <iostream>
#include <stdio.h>
#include <utility>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "Map.hpp"
#include "constants.hpp"


using namespace std;

class Player
{
	public:
	
    int life=3;
    int coins=0;
    int PlayerSize;
	int lastDir=-1;
	int xVel=0;
	int yVel=0;
	int speed=3;
    
    SDL_Texture* playerTex;
    SDL_Texture* scoreTex;
    SDL_Rect destR,scoreR;

    Player(int x,int y,int playerSize,int score_x,int score_y,int scoreH,int scoreW);
    ~Player();
    void init(SDL_Renderer* renderer, string path);
    void ForScore(TTF_Font* Font,SDL_Renderer *renderer,string intro,bool add);
    bool touch(Map* maze,int type,int TILE_SIZE);
	void update(Map* maze);
	pair<int,int> get(Map* maze,int type);
	void clean();
    SDL_Rect valid_move(SDL_Rect box,int xmove,int ymove,int map[30][100]);
};

Player::Player(int x,int y,int playerSize,int score_x,int score_y,int scoreH,int scoreW)
{
    destR.x=x;
    destR.y=y;
    destR.w=playerSize;
    destR.h=playerSize;

    scoreR.x=score_x;
    scoreR.y=score_y;
    scoreR.h=scoreH;
    scoreR.w=scoreW;

    life=5;
    coins=0;

    PlayerSize=playerSize;
}

Player::~Player(){}
void Player::init(SDL_Renderer* renderer,string path)
{
    const char* path_array = path.c_str();
	SDL_Surface* tmpSurface = IMG_Load(path_array);
	playerTex = SDL_CreateTextureFromSurface(renderer,tmpSurface);
	SDL_FreeSurface(tmpSurface);
}

void Player::ForScore(TTF_Font* Font,SDL_Renderer *renderer,string intro,bool add)
{
	SDL_Color Black= {0,0,0,255};

	if(add) intro =  intro + to_string(coins);
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Font, intro.c_str(), Black);
	scoreTex = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	SDL_FreeSurface(surfaceMessage);

	SDL_QueryTexture(scoreTex,nullptr,nullptr,&scoreR.w,&scoreR.h);
}



bool Player::touch(Map* maze,int type,int TILE_SIZE)		//(3*PLAYER_SIZE)/4 to for much more efficient touc
{
	int xpos = destR.x;
	int ypos = destR.y;

	if(maze->map[ypos/TILE_SIZE][xpos/TILE_SIZE]==type)
	{
		maze->map[ypos/TILE_SIZE][xpos/TILE_SIZE] = 1; 
		return true;
	}
	else if(maze->map[ypos/TILE_SIZE][(xpos+((3*PlayerSize)/4))/TILE_SIZE]==type)
	{
		maze->map[ypos/TILE_SIZE][(xpos+((3*PlayerSize)/4))/TILE_SIZE] = 1; 
		return true;
	}
	else if(maze->map[(ypos+((3*PlayerSize)/4))/TILE_SIZE][xpos/TILE_SIZE]==type)
	{
		maze->map[(ypos+((3*PlayerSize)/4))/TILE_SIZE][xpos/TILE_SIZE] = 1;
		return true;	
	}
	else if(maze->map[(ypos+((3*PlayerSize)/4))/TILE_SIZE][(xpos+((3*PlayerSize)/4))/TILE_SIZE]==type)
	{
		maze->map[(ypos+((3*PlayerSize)/4))/TILE_SIZE][(xpos+((3*PlayerSize)/4))/TILE_SIZE] = 1;
		return true;
	}
	return false;
}

pair<int,int> Player::get(Map* maze,int type)
{
	int xpos = destR.x;
	int ypos = destR.y;

	pair<int,int> result;

	if(maze->map[xpos/TILE_SIZE][ypos/TILE_SIZE]==type)
	{
		result.first=(xpos)/TILE_SIZE ;
		result.second=(ypos)/TILE_SIZE;
	}
	else if(maze->map[(xpos+((3*PlayerSize)/4))/TILE_SIZE][ypos/TILE_SIZE]==type)
	{
		result.first=(xpos+((3*PlayerSize)/4))/TILE_SIZE ;
		result.second=(ypos)/TILE_SIZE;
	}
	else if(maze->map[xpos/TILE_SIZE][(ypos+((3*PlayerSize)/4))/TILE_SIZE]==type)
	{
		result.first=xpos/TILE_SIZE ;
		result.second=(ypos+((3*PlayerSize)/4))/TILE_SIZE;	
	}
	else if(maze->map[(xpos+((3*PlayerSize)/4))/TILE_SIZE][(ypos+((3*PlayerSize)/4))/TILE_SIZE]==type)
	{
		result.first=(xpos+((3*PlayerSize)/4))/TILE_SIZE ;
		result.second=(ypos+((3*PlayerSize)/4))/TILE_SIZE;

	}
	return result;
}

SDL_Rect Player::valid_move(SDL_Rect box,int xmove,int ymove,int map[30][100])
{
	bool check = (box.x+xmove>=0) && (box.x+PLAYER_SIZE+xmove<=SCREEN_WIDTH);
	check = check && (box.y+ymove>=0) && (box.y+PLAYER_SIZE+ymove<=SCREEN_HEIGHT);
	
	SDL_Rect rslt= box;
	if(check)
	{
		check = check && (map[(box.y+ymove)/TILE_SIZE][(box.x+xmove)/TILE_SIZE]%2==1); 
		check = check && (map[(box.y+ymove+PLAYER_SIZE)/TILE_SIZE][(box.x+xmove+PLAYER_SIZE)/TILE_SIZE]%2==1);
		check = check && (map[(box.y+ymove)/TILE_SIZE][(box.x+xmove+PLAYER_SIZE)/TILE_SIZE]%2==1); 
		check = check && (map[(box.y+ymove+PLAYER_SIZE)/TILE_SIZE][(box.x+xmove)/TILE_SIZE]%2==1);

		if(check)
		{
			rslt.x = box.x + xmove;
			rslt.y = box.y + ymove;
		}
	}

	return rslt;
}

void Player::update(Map* maze)
{
	SDL_Rect tmpRect=valid_move(destR,speed*xVel,speed*yVel,maze->map);
	
	destR=tmpRect;
	SDL_Delay(2);
	
}

void Player::clean()
{
	scoreTex=nullptr;
	playerTex=nullptr;
}