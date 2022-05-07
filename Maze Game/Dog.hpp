#pragma once

#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
using namespace std;

int DOG_SIZE=18;

class Dog
{
private:
    
public:
     Dog(int size);
     ~Dog();
     void init(SDL_Renderer* renderer);
     void draw(SDL_Renderer* renderer,int xpos,int ypos);
     void clean();

     SDL_Rect destR;
     SDL_Texture* DogTex;

     int move=12;
     bool opposite=false;
   
};
 Dog:: Dog(int size)
 {
     destR.x=0;
     destR.y=0;
     destR.h=size;
     destR.w=size;
 
 }

 void Dog::init(SDL_Renderer* renderer)
 {
   string path = "./src/monster01.png";
	const char* path_array = path.c_str();

	SDL_Surface* tmpSurface = IMG_Load(path_array);
	DogTex = SDL_CreateTextureFromSurface(renderer,tmpSurface);
	SDL_FreeSurface(tmpSurface);
 }
 void Dog::draw(SDL_Renderer* renderer,int xpos,int ypos)
 {
    destR.x = xpos;
    destR.y = ypos;
    SDL_RenderCopy(renderer,DogTex,NULL,&(destR));
 }
 void Dog::clean()
 {
    DogTex=nullptr;
 }
 Dog:: ~Dog(){}

