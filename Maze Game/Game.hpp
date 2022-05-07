#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>
#include "Player.hpp"


using namespace std;

class Game
{
public:
    bool isRunning=false;

	Game(int id1,int row,int col);
    ~Game();
    void init(char* title,int x, int y, int width,int height);
	bool loadMedia();

    void update();
    void render();
	void handleEventforP1();
	void handleEventforP2();
	void recievedEvent(int arr[]);
    void clean();
    bool running(){ return isRunning;}

    SDL_Window *window=nullptr;
    SDL_Renderer *renderer=nullptr;

    SDL_Texture* Message;
    SDL_Rect Msg_Rect;
    TTF_Font* Font; 
	SDL_Color Black;

    Map* map=nullptr;
	// scoreBoard* scorer;
    Player *player1;
    Player *player2;



    Mix_Chunk* coinPick;
	Mix_Chunk* recharge;
	Mix_Chunk* collide;
	Mix_Chunk* killed;

	int id;
	int won=0;
	int startRow,startCol;
	int map_remove[40]={1,3,1,4,1,5,1,3,1,4,1,5,1,3,1,4,1,5,1,3,1,3,1,4,1,5,1,3,1,4,1,5,1,3,1,4,1,5,1,3};
	int start_remove=0;
	int send_event[7];
	string scoreboard;
	SDL_Rect rect = {20,20,20,20};

    
};

Game::Game(int id1,int row,int col)
{
	id=id1;
	startRow=row;
	startCol=col;
	// scorer=new scoreBoard(id1,0,SCREEN_HEIGHT,SCREEN_WIDTH,SCORE_HEIGHT);
}

Game::~Game(){}

void Game::init(char* title,int x, int y, int width,int height)
{
    if(SDL_Init(SDL_INIT_EVERYTHING)==0)
	{
		std::cout<<"The game has been initialised\n";
	}
	window = SDL_CreateWindow(title,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,width,height+SCORE_HEIGHT,0);

	isRunning = true;
	if(window==0)
	{
		isRunning = false;
		std::cout<<"Window has not been created yet";
	}
	else
	{
		std::cout << "Window created"<<endl;
	}

	renderer = SDL_CreateRenderer(window,-1,0);
	if(renderer==0)
	{
		std::cout<<"Renderer has not been initialised";
		isRunning = false;
	}
	else
	{
		std::cout << "Renderer initialised" <<endl;
	}
	SDL_SetRenderDrawColor(renderer,128,128,128,255);
	

	//for text rendering.....
	if(TTF_Init()==-1) 
	{
		std::cout<<"Error in loading ttf";
	}
	else
	{
		std::cout << "TTF loaded succesfully";
	}
	
	Font = TTF_OpenFont("./src/font.ttf", 30);
	Black = {100, 0, 0,255};

	
	//Initialize SDL_mixer
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
    }


    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
    }

	string boundary;
	for(int i=0;i<(SCREEN_WIDTH-3);i++)
	{
		boundary = boundary + "-";
	}
	
	// SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Font, boundary.c_str(), Black);
	// Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	// SDL_FreeSurface(surfaceMessage);

	
	Msg_Rect.x = 0;  
	Msg_Rect.y = SCREEN_HEIGHT;
	SDL_QueryTexture(Message,nullptr,nullptr,&Msg_Rect.w,&Msg_Rect.h);


	map=new Map(renderer);
	std::cout<<"Player: "<<id<<", "<<startRow<<", "<<startCol<<endl;

	player2= new Player(startCol,startRow,PLAYER_SIZE,0,SCREEN_HEIGHT,SCORE_HEIGHT,SCREEN_WIDTH);
	player1= new Player(startCol,startRow,PLAYER_SIZE,200,700,20,20);

	player1->init(renderer,"./src/ghost1.png");
	player2->init(renderer,"./src/ghost2.png");


	

	if(!loadMedia())
	{
		printf("Failed to load Media!");
	}

	printf("loaded Media!");
}

bool Game::loadMedia()
{
	bool success=true;
	coinPick=Mix_LoadWAV("./src/coin_pick.wav");
	printf("Loading Media..");
	if (coinPick==NULL)
	{
		std::cout<<"Failed to load coin pick sound."<<endl;
		success=false;
	}

	recharge=Mix_LoadWAV("./src/recharging.wav");
	if (recharge==NULL)
	{
		std::cout<<"Failed to load recharging sound."<<endl;
		success=false;
	}

	collide=Mix_LoadWAV("./src/killed_sound.wav");
	if (collide==NULL)
	{
		std::cout<<"Failed to load colliding sound."<<endl;
		success=false;
	}

	killed=Mix_LoadWAV("./src/killed_sound.wav");
	if (killed==NULL)
	{
		std::cout<<"Failed to load killed sound."<<endl;
		success=false;
	}

	return success;
}

void Game::render()
{
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer,128,128,128,255);	
	map->draw(renderer,TILE_SIZE);

	SDL_RenderCopy(renderer,player1->playerTex,NULL,&(player1->destR));
	SDL_RenderCopy(renderer,player2->playerTex,NULL,&(player2->destR));

    
	//SDL_RenderCopy(renderer,Message,&rect,&(player1->scoreR));
	// scorer->render(renderer);

	SDL_RenderPresent(renderer);
}

void Game::handleEventforP1()
{
	for (int i=0;i<40;i++)
	{
		send_event[i]=map_remove[i];
	}


	
		SDL_Event event;
	    SDL_PollEvent(&event);
		
		if(event.type==SDL_QUIT) {isRunning = false;}
	
	    if(event.type == SDL_KEYDOWN )	
        {
			switch (event.key.keysym.sym)
    	    {
			case SDLK_ESCAPE:	player1->life = 0;	//the one who leaves the game first loses...
        	case SDLK_LEFT: 	player1->xVel = -1;player1->yVel= 0;break;
        	case SDLK_RIGHT: 	player1->xVel = 1;player1->yVel= 0;break;
        	case SDLK_UP:    	player1->yVel = -1;player1->xVel=0;break;
        	case SDLK_DOWN:  	player1->yVel = 1;player1->xVel= 0;break;
			}
		}

		if(event.type == SDL_KEYUP )	
        {
			switch (event.key.keysym.sym)
    	    {
			//the one who leaves the game first loses...
        	case SDLK_LEFT: 	;player1->xVel = 0;player1->yVel= 0;break;
        	case SDLK_RIGHT: 	player1->xVel = 0;player1->yVel= 0;break;
        	case SDLK_UP:    	player1->yVel = 0;player1->xVel=0;break;
        	case SDLK_DOWN:  	player1->yVel = 0;player1->xVel= 0;break;
			}
		}


		send_event[0]=player1->destR.x;
		send_event[1]=player1->destR.y;
		send_event[2]=player1->life;
		send_event[3]=player1->coins;
		send_event[4]=won;
}

void Game::handleEventforP2()
	{
		SDL_Event event;
	    SDL_PollEvent(&event);

		if(event.type==SDL_QUIT) isRunning = false;
	
	    if(event.type == SDL_KEYDOWN && event.key.repeat==0)	
        {
			switch (event.key.keysym.sym)
    	    {
			case SDLK_ESCAPE:	player2->life = 0;	//the one who leaves the game first loses...
        	case SDLK_LEFT: 	player2->xVel = -1;player2->yVel= 0;break;
        	case SDLK_RIGHT: 	player2->xVel = 1;player2->yVel= 0;break;
        	case SDLK_UP:    	player2->yVel = -1;player2->xVel=0;break;
        	case SDLK_DOWN:  	player2->yVel = 1;player2->xVel= 0;break;
			}
		}

		 if(event.type == SDL_KEYUP && event.key.repeat==0)	
        {
			switch (event.key.keysym.sym)
    	    {
			//the one who leaves the game first loses...
        	case SDLK_LEFT: 	player2->xVel = 0;player2->yVel= 0;break;
        	case SDLK_RIGHT: 	player2->xVel = 0;player2->yVel= 0;break;
        	case SDLK_UP:    	player2->yVel = 0;player2->xVel=0;break;
        	case SDLK_DOWN:  	player2->yVel = 0;player2->xVel= 0;break;
			}
		}

		send_event[0]=player2->destR.x;
		send_event[1]=player2->destR.y;
		send_event[2]=player2->life;
		send_event[3]=player2->coins;
		send_event[4]=won;

	}


void Game::recievedEvent(int arr[])
{
	if(arr[4]==0)
	{
		if (id==1)
		{
			player2->destR.x=arr[0];
			player2->destR.y=arr[1];
			player2->life=arr[2];
			player2->coins=arr[3];

			int map_array[2];
			for (int i=0;i<2;i++)
			{
				map_array[i]=arr[i];
			}

			//map->update(map_array,2);
		}
		else if(id==2)
		{
			player1->destR.x=arr[0];
			player1->destR.y=arr[1];
			player1->life=arr[2];
			player1->coins=arr[3];

			int map_array[2];
			for (int i=5;i<7;i++)
			{
				map_array[i-5]=arr[i];
			}

			// map->update(map_array,2);
		}
	}
	else
	{
		isRunning=false;
		std::cout<<"Player"<<arr[4]<<" Won the Game!"<<endl;
	}
}

void Game::update()
{
	
	player1->update(map);
	player2->update(map);

	if(player1->life==0)
	{
		won=2;
		if(isRunning) Mix_PlayChannel(-1,killed, 0 );

		player1->ForScore(Font,renderer,"PLAYER1 : LOSSER ",false);
		player1->coins = 0;
		player2->ForScore(Font,renderer,"PLAYER2 : WINNER ",false);
		isRunning = false;
		return ;
	}
	else if(player2->life==0)
	{
		won=1;	
		if(isRunning) Mix_PlayChannel(-1,killed, 0 );

		player1->ForScore(Font,renderer,"PLAYER1 : WINNER ",false);
		player2->ForScore(Font,renderer,"PLAYER2 : LOSSER ",false);
		player2->coins = 0;
		isRunning = false;
		return ;
	}

	if(id==1)
	{
		if(player1->touch(map,7,TILE_SIZE))    // Touching a coin
		{
			if(isRunning) Mix_PlayChannel(-1,coinPick, 0 );
			player1->coins++;

			pair<int,int> p=player1->get(map,7);
			send_event[5]=p.first;
			send_event[6]=p.second;

		}

		if(player1->touch(map,11,TILE_SIZE))    // Touching a coin
		{
			won=1;
			isRunning=false;
		}

		if(player1->touch(map,9,TILE_SIZE))                 //Touching a Dog
		{
			if(isRunning) Mix_PlayChannel(-1,collide, 0 );
			player1->life--;

			pair<int,int> p=player1->get(map,9);
			send_event[5]=p.first;
			send_event[6]=p.second;

		}

		if (player1->coins>=3)
		{
			if(player1->touch(map,3,TILE_SIZE))                                 // Touching a charging point
			{
				if(isRunning) Mix_PlayChannel(-1,recharge, 0 );
				player1->coins=player1->coins-3;
				player1->life++;

				pair<int,int> p=player1->get(map,3);
				send_event[5]=p.first;
			    send_event[6]=p.second;
			}
		}

		if (player1->touch(map,11,TILE_SIZE))                           // Reaching the end point
		{
			isRunning=false;
			player2->life=0;
			won=1;
		}
	}

	else if(id==2)
	{
		if(player2->touch(map,7,TILE_SIZE))    // Touching a coin
		{
			if(isRunning) Mix_PlayChannel(-1,coinPick, 0 );
			player2->coins++;

			pair<int,int> p=player2->get(map,7);
			map_remove[start_remove*2]=p.first;
			map_remove[start_remove*2+1]=p.second;

			start_remove++;

		}

		if(player2->touch(map,11,TILE_SIZE))    // Touching a coin
		{
			won=2;
			isRunning=false;
		}


		if(player2->touch(map,9,TILE_SIZE))                 //Touching a Dog
		{
			if(isRunning) Mix_PlayChannel(-1,collide, 0 );
			player2->life--;

			pair<int,int> p=player2->get(map,9);
			map_remove[start_remove*2]=p.first;
			map_remove[start_remove*2+1]=p.second;

			start_remove++;

		}

		if (player2->coins>=3)
		{
			if(player2->touch(map,3,TILE_SIZE))                                 // Touching a charging point
			{
				if(isRunning) Mix_PlayChannel(-1,recharge, 0 );
				player2->coins=player2->coins-3;
				player2->life++;

				pair<int,int> p=player2->get(map,3);
				map_remove[start_remove*2]=p.first;
				map_remove[start_remove*2+1]=p.second;
				
				start_remove++;
			}
		}

		if (player2->touch(map,11,TILE_SIZE))                           // Reaching the end point
		{
			isRunning=false;
			player1->life=0;
			won=2;
		}
	}
}

void Game::clean()
{

	std::cout<<"=============================================="<<endl;
	std::cout<<"================ GAME OVER ==================="<<endl;
	std::cout<<"=============================================="<<endl;
	if(id==won)
	{
	std::cout<<"================= YOU WIN ====================="<<endl;
	}
	if(id!=won)
	{
	std::cout<<"================= YOU LOSS ====================="<<endl;
	}
	std::cout<<"------------------------------------------------"<<endl;
	std::cout<<"================== SCORES ====================="<<endl;
	std::cout<<"PLAYER1: "<<"coins :- "<<player1->coins<<" life :- "<< player1->life<<endl;
	std::cout<<"------------------------------------------------"<<endl;
	std::cout<<"PLAYER2: "<<"coins :- "<<player2->coins<<" life :- "<< player2->life<<endl;
	std::cout<<"------------------------------------------------"<<endl;
	
	if(player1->life==0 || won==2)
	{
		std::cout<<"\n\nPLAYER2 WON ";
	}
	else if(player2->life==0 || won==1) 
	{
		std::cout<<"\n\nPLAYER1 WON ";
	}


	Mix_FreeChunk(collide);
	collide = nullptr;

	Mix_FreeChunk(coinPick);
	coinPick = nullptr;

	Mix_FreeChunk(killed);
	killed = nullptr;

	Mix_FreeChunk(recharge);
	recharge = nullptr;

	
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();

	window = nullptr;
	renderer = nullptr;
	Message = nullptr;
	Font = nullptr; 

	player1->clean();
	player1 = nullptr;
	player2->clean();
	player2 = nullptr;
	map->clean();
	map = nullptr;
//	scorer->clean();
//	scorer=nullptr;
	Mix_CloseAudio();
	
	std::cout<<"\nROUND OVER"<<endl; 
}