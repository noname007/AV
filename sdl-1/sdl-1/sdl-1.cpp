// sdl-1.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

extern "C"{
#include "SDL2/SDL.h"
};

int _tmain(int argc, _TCHAR* argv[])
{
	if(SDL_Init(SDL_INIT_VIDEO))
	{

		printf("ERROR -%s",SDL_GetError());
	}else{
		printf("SUCCESS");
	}
	return 0;
}

