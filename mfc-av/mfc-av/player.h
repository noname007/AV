#pragma once
extern "C"{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
	//#include "SDL2/SDL.h"
};

class player
{
public:
	player(void);
	~player(void);
	play();
};

