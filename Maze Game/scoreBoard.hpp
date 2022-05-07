
#include <iostream>
#include<stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>

using namespace std;

class scoreBoard
{
private:
    int id;
    SDL_Color color1;
    SDL_Color color2;

    SDL_Rect yourR[5];
    SDL_Rect enemyR[5];

    SDL_Texture* yourTex[5];
    SDL_Texture* enemyTex[5];

    SDL_Surface* texSur;

    TTF_Font* gfont;
    /* data */
public:
    scoreBoard(int idin,int x,int y,int scoreW,int scoreH);
    void init(SDL_Renderer* renderer,int score1,int life1,int score2,int life2);
    void render(SDL_Renderer* renderer);
    void clean();
    ~scoreBoard();
};
scoreBoard::scoreBoard(int idin,int x,int y,int scoreW,int scoreH)
{
    id=idin;
    color1={3,219,150,225};
    color2={22,219,220,50};
    gfont=TTF_OpenFont("./src/F1.ttf",20);

    int w=scoreW;
    int h=scoreH;

    yourR[0].h=h/2;
    yourR[0].w=w/2;

    yourR[1].h=h/4;
    yourR[1].w=w/4;

    yourR[2].h=h/4;
    yourR[2].w=w/4;

    yourR[3].h=h/4;
    yourR[3].w=w/4;

    yourR[4].h=h/4;
    yourR[4].w=w/4;

    enemyR[0].h=h/2;
    enemyR[0].w=w/2;

    enemyR[1].h=h/4;
    enemyR[1].w=w/4;

    enemyR[2].h=h/4;
    enemyR[2].w=w/4;

    enemyR[3].h=h/4;
    enemyR[3].w=w/4;

    enemyR[4].h=h/4;
    enemyR[4].w=w/4;

    yourR[0].x=x;
    yourR[0].y=y;
    yourR[1].x=x;
    yourR[1].y=y+h/2;
    yourR[2].x=x+w/4;
    yourR[2].y=y+h/2;
    yourR[3].x=x;
    yourR[3].y=y+3*h/4;
    yourR[4].x=x+w/4;
    yourR[4].y=y+3*h/4;

    enemyR[0].x=x+w/2;
    enemyR[0].y=y;
    enemyR[1].x=x+w/2;
    enemyR[1].y=y+h/2;
    enemyR[2].x=x+3*w/4;
    enemyR[2].y=y+h/2;
    enemyR[3].x=x+w/2;
    enemyR[3].y=y+3*h/4;
    enemyR[4].x=x+3*w/4;
    enemyR[4].y=y+3*h/4;

}

void scoreBoard::init(SDL_Renderer* renderer,int score1,int life1,int score2,int life2)
{
    texSur=TTF_RenderText_Solid(gfont,"   You  ",color1);
    yourTex[0]=SDL_CreateTextureFromSurface(renderer,texSur);
    SDL_FreeSurface(texSur);
    SDL_QueryTexture(yourTex[0],nullptr,nullptr,&yourR[0].w,&yourR[0].h);

    texSur=TTF_RenderText_Solid(gfont," Coins :   ",color1);
    yourTex[1]=SDL_CreateTextureFromSurface(renderer,texSur);
    SDL_FreeSurface(texSur);
    SDL_QueryTexture(yourTex[1],nullptr,nullptr,&yourR[1].w,&yourR[1].h);

    texSur=TTF_RenderText_Solid(gfont,(to_string(score1)).c_str(),color1);
    yourTex[2]=SDL_CreateTextureFromSurface(renderer,texSur);
    SDL_FreeSurface(texSur);
    SDL_QueryTexture(yourTex[2],nullptr,nullptr,&yourR[2].w,&yourR[2].h);

    texSur=TTF_RenderText_Solid(gfont," Life : ",color1);
    yourTex[3]=SDL_CreateTextureFromSurface(renderer,texSur);
    SDL_FreeSurface(texSur);
    SDL_QueryTexture(yourTex[3],nullptr,nullptr,&yourR[3].w,&yourR[3].h);

    texSur=TTF_RenderText_Solid(gfont,(to_string(life1)).c_str(),color1);
    yourTex[4]=SDL_CreateTextureFromSurface(renderer,texSur);
    SDL_FreeSurface(texSur);
    SDL_QueryTexture(yourTex[4],nullptr,nullptr,&yourR[4].w,&yourR[4].h);

    texSur=TTF_RenderText_Solid(gfont,"   Enemy  ",color1);
    enemyTex[0]=SDL_CreateTextureFromSurface(renderer,texSur);
    SDL_FreeSurface(texSur);
    SDL_QueryTexture(enemyTex[0],nullptr,nullptr,&enemyR[0].w,&enemyR[0].h);

    texSur=TTF_RenderText_Solid(gfont," Coins : ",color1);
    enemyTex[1]=SDL_CreateTextureFromSurface(renderer,texSur);
    SDL_FreeSurface(texSur);
    SDL_QueryTexture(enemyTex[1],nullptr,nullptr,&enemyR[1].w,&enemyR[1].h);

    texSur=TTF_RenderText_Solid(gfont,(to_string(score1)).c_str(),color1);
    enemyTex[2]=SDL_CreateTextureFromSurface(renderer,texSur);
    SDL_FreeSurface(texSur);
    SDL_QueryTexture(enemyTex[2],nullptr,nullptr,&enemyR[2].w,&enemyR[2].h);

    texSur=TTF_RenderText_Solid(gfont," Life : ",color1);
    enemyTex[3]=SDL_CreateTextureFromSurface(renderer,texSur);
    SDL_FreeSurface(texSur);
    SDL_QueryTexture(enemyTex[3],nullptr,nullptr,&enemyR[3].w,&enemyR[3].h);

    texSur=TTF_RenderText_Solid(gfont,(to_string(score1)).c_str(),color1);
    enemyTex[4]=SDL_CreateTextureFromSurface(renderer,texSur);
    SDL_FreeSurface(texSur);
    SDL_QueryTexture(enemyTex[4],nullptr,nullptr,&enemyR[4].w,&enemyR[4].h);

}

void scoreBoard::render(SDL_Renderer* renderer)
{
    for (int i=0;i<5;i++)
    {
        SDL_RenderCopy(renderer,yourTex[i],NULL,&yourR[i]);
        SDL_RenderCopy(renderer,enemyTex[i],NULL,&enemyR[i]);
    }
}

scoreBoard::~scoreBoard()
{
}

void scoreBoard::clean()
{
    for (int i=0;i<5;i++)
    {
        yourTex[i]=nullptr;
        enemyTex[i]=nullptr;
    }
}
