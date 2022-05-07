#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

using namespace std;

class Tile
{
private:
    SDL_Rect destR;
    SDL_Texture* tileTex;
    /* data */
public:
    Tile();
    ~Tile();
    void init(SDL_Renderer* renderer,string path);
    void draw(SDL_Renderer* renderer,int size,int x,int y);
    void clean();
};

Tile::Tile(){}

Tile::~Tile(){}

void Tile:: init(SDL_Renderer* renderer, string path)
{
    const char* patharray=path.c_str();

    SDL_Surface* tmpSurface=IMG_Load(patharray);
    tileTex=SDL_CreateTextureFromSurface(renderer,tmpSurface);
    SDL_FreeSurface(tmpSurface);
}

void Tile:: draw(SDL_Renderer* renderer,int size,int x,int y)
{
    destR.x=x;
    destR.y=y;
    destR.h=size;
    destR.w=size;
    SDL_RenderCopy(renderer, tileTex,NULL,&destR);

}

void Tile::clean()
{
    tileTex=nullptr;
}