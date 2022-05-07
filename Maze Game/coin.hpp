#pragma once

#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
using namespace std;

class Coin
{
private:
    
public:
     Coin(int size);
     ~Coin();
     void init(SDL_Renderer* renderer);
     void draw(SDL_Renderer* renderer,int xpos,int ypos);
     void clean();

     SDL_Rect destR;
     SDL_Texture* CoinTex;
};
 Coin:: Coin(int size)
 {
     destR.x=0;
     destR.y=0;
     destR.h=size;
     destR.w=size;
 }

 void Coin::init(SDL_Renderer* renderer)
 {
     string path = "./src/Coin.png";
	const char* path_array = path.c_str();

	SDL_Surface* tmpSurface = IMG_Load(path_array);
	CoinTex = SDL_CreateTextureFromSurface(renderer,tmpSurface);
	SDL_FreeSurface(tmpSurface);
 }
 void Coin::draw(SDL_Renderer* renderer,int xpos,int ypos)
 {
    destR.x = xpos;
    destR.y = ypos;
    SDL_RenderCopy(renderer,CoinTex,NULL,&(destR));
 }
 void Coin::clean()
 {
     CoinTex=nullptr;
 }
 Coin:: ~Coin(){}
