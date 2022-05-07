#include <iostream>
#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>
#include <pthread.h>
#include <time.h>
#include <stdint.h>
#include "Game.hpp"

#define PORT 5000
#define MAXLINE 1000

// #include "server.hpp"
// #include "client.hpp"

using namespace std;

// int main(int argc, char const *argv[])
// {
//     /* code */
//      Game* game=new Game(1,1*TILE_SIZE,5*TILE_SIZE);

//      string title = "PLAYER";
//     char char_title[8];

//     title = title + to_string(game->id);
//     string begin;

//     strcpy(char_title,title.c_str());
//     game->init(char_title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT);
//     while(game->isRunning)
//     {
//         game->handleEventforP1();
//         game->update();
//         game->render();
//     }
//     game->clean();
//     return 0;
// }


int main(int argc, char const *argv[])
{
    int arr[45];

    cout<<"Arguments:"<<argc<<endl;

    if (argc==1)
    {
        Game* game=new Game(1,1*TILE_SIZE,5*TILE_SIZE);
        string title = "PLAYER";   

        title = title + to_string(game->id);
        char char_title[title.length()+1];


        strcpy(char_title,title.c_str());
        game->init(char_title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT-SCORE_HEIGHT);

        const int FPS=60;
        const int frameDelay=1000/FPS;

        Uint32 frameStart;
        int frameTime;

        while(game->isRunning)
        {
            frameStart=SDL_GetTicks();

            game->handleEventforP1();
            game->update();
            game->render();

            frameTime=SDL_GetTicks()-frameStart;

            if(frameDelay>frameTime)
            {
                SDL_Delay(frameDelay-frameTime);
            }
        }
        game->clean();
        return 0;
    }
    if(argc==2)
    {

        if(string(argv[1])=="s")
        {
            cout<<"Server Here.."<<endl;
            
            char buffer[1000];
            char *message = "Hello Client";
            int array[45];
            int listenfd;
            socklen_t len;
            struct sockaddr_in servaddr, cliaddr;
            bzero(&servaddr, sizeof(servaddr));

            // Create a UDP Socket
            listenfd = socket(AF_INET, SOCK_DGRAM, 0);		
            servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
            servaddr.sin_port = htons(PORT);
            servaddr.sin_family = AF_INET;

            // bind server address to socket descriptor
            bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

            //receive the datagram
            len = sizeof(cliaddr);

            
            //========================
            Game* game=new Game(1,1*TILE_SIZE,5*TILE_SIZE);

            string title = "PLAYER";
            
            char char_title[title.length()+1];
            title = title + to_string(game->id);
            string begin;

            strcpy(char_title,title.c_str());
            game->init(char_title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT);
            while(game->isRunning)
            {
                game->handleEventforP1();
                game->update();
                game->render();

                int n = recvfrom(listenfd,array, sizeof(array),0, (struct sockaddr*)&cliaddr,&len); //receive message from serve
                sendto(listenfd, game->send_event, sizeof(game->send_event), 0,(struct sockaddr*)&cliaddr, sizeof(cliaddr));

                game->recievedEvent(array);
            }
            game->clean();
            return 0;

        }
    }

    if(argc==3)
    {
        if(string(argv[1])=="c")
        {
            string ip_addr=argv[2];

           
            int array[45];
            int sockfd, n;
            struct sockaddr_in servaddr;
            
            // clear servaddr
            bzero(&servaddr, sizeof(servaddr));
            servaddr.sin_addr.s_addr = inet_addr(ip_addr.c_str());
            servaddr.sin_port = htons(PORT);
            servaddr.sin_family = AF_INET;
            
            // create datagram socket
            sockfd = socket(AF_INET, SOCK_DGRAM, 0);
            
            // connect to server
            if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
            {
                printf("\n Error : Connect Failed \n");
                exit(0);
            }

            Game* game=new Game(2,1*TILE_SIZE,5*TILE_SIZE);

            string title = "PLAYER";

            

            title = title + to_string(game->id);
            char char_title[title.length()+1];
            string begin;

            strcpy(char_title,title.c_str());
            game->init(char_title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT);
            while(game->isRunning)
            {
                game->handleEventforP2();
                game->update();
                game->render();

                sendto(sockfd, game->send_event, sizeof(game->send_event), 0, (struct sockaddr*)NULL, sizeof(servaddr));
                recvfrom(sockfd, array, sizeof(array), 0, (struct sockaddr*)NULL, NULL);
                game->recievedEvent(array);

            }
            game->clean();
            close(sockfd);

            
        }
    }
}

   

