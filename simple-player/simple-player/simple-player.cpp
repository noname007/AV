// simple-player.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"




#include <stdio.h>

#define __STDC_CONSTANT_MACROS

extern "C"
{
	#include "libavcodec/avcodec.h"
	#include "SDL2/SDL.h"
};

int main(int argc, char* argv[])
{
	printf("%s", avcodec_configuration());
	if(SDL_Init(SDL_INIT_VIDEO)) {  
		printf( "Could not initialize SDL - %s\n", SDL_GetError()); 
	} else{
		printf("Success init SDL");
	}
	return 0;
}

