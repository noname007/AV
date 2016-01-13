// sdl-1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
//#include "stdlib.h"

extern "C"{
#include "SDL2/SDL.h"
};


#define REFRESH_EVENT (SDL_USEREVENT + 1)
#define BREAK_EVENT (SDL_USEREVENT + 2)


int thread_exit = 0;

int v_pause = 0x00;

//int i = 0;
int refresh_video(void * opaque)
{
	thread_exit = 0;
	
	while(thread_exit == 0)
	{
		if(v_pause)
		{

			SDL_Delay(1000);
			continue;
		}

		SDL_Event event;
		event.type = REFRESH_EVENT;
		//printf("%d=================================>\n",i++);
		SDL_PushEvent(&event);
		//x2 2倍速
		//SDL_Delay(40);
		SDL_Delay(20);

	}
	SDL_Event event;
	thread_exit = 0;
	event.type = BREAK_EVENT;
	SDL_PushEvent(&event);

	return 0;
}
/************************************************************************/
/* 

整个流程
	------->SDL_Init 初始化 init
	  |
	  |---->SDL_CreateWindow 创建window 
	  |
	  |---->SDL_CreateRenderer 创建render
	  |
	  |---->SDL_CreateTexture 创建Texture
		|
		|---->SDL_UpdateTexture
		|
		|---->SDL_RenderClear
		|
		|---->SDL_RenderCopy
		|
		|---->SDL_RenderPresent
		|
		|---->SDL_Delay
		
	
*/
/************************************************************************/
int _tmain(int argc, _TCHAR* argv[])
{

	//yuv 数据一帧的宽高
	const int pix_w = 640 ,pix_h = 360;
	//const int pix_w = 320 ,pix_h = 180;
	
	// 显示窗口的宽高
	int window_w = 700,window_h = 700;

	const int bpp =12;
	
	//一个像素YUV数据  8bit Y 2bit U 2bit V == 12 bit
	//故一帧视频数据大小 w*h * 12/8 byte大小
	const int one_frame_yuv_data = pix_h*pix_w*bpp/8;
	//unsigned char buffer [pix_w *pix_h /8];
	//buffer 大小分配不足，导致程序一直崩溃，差一点都是问题
	unsigned char buffer [one_frame_yuv_data];
	
	//const int  black_white = one_frame_yuv_data*2/3 + 100; //内存稍微的分配的大一点，就会避免一些自己没有想到的内存溢出问题
	//unsigned char black_withebuffer [black_white];
	//
	
	/**
	*SDL_Init初始化
	*/
	if(SDL_Init(SDL_INIT_VIDEO))
	{
		printf("ERROR -%s",SDL_GetError());
		return -1;
	}else{
		printf("SUCCESS");
	}

	//SDL_Window *window = SDL_CreateWindow("yangzz",,100,100,window_w,window_h,SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
	SDL_Window *window = SDL_CreateWindow("yangzz",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,window_w,window_h,SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);


	if(!window)
	{
		//fail process
	}


	FILE *fpyuv = NULL;
	char filename[] = "sintel_640_360.yuv";
	//char filename[] = "test_yuv420p_320x180.yuv";
 	fpyuv = fopen(filename,"rb+");

	if(fpyuv == NULL)
	{
		printf("cant't open the file%s",filename);
		return -1;
	}



	SDL_Renderer *render = SDL_CreateRenderer(window,-1,0);

	Uint32 pixformat = SDL_PIXELFORMAT_IYUV;
	SDL_Texture *txture = SDL_CreateTexture(render,pixformat,SDL_TEXTUREACCESS_STREAMING,pix_w,pix_h);

	SDL_Rect rect;

	SDL_Event event;
	SDL_Thread * thread = SDL_CreateThread(refresh_video,NULL,NULL);



	while (1)
	{
		SDL_WaitEvent(&event);
		//printf("%d************************\n",i++);
		if(event.type != REFRESH_EVENT)
			printf("Event %x\n",event.type);
		//fflush(stdout);
		if(event.type == REFRESH_EVENT)
		{
			if(fread(buffer,1,one_frame_yuv_data,fpyuv) != one_frame_yuv_data)
			{
				//read the end and read again
				fseek(fpyuv,0,SEEK_SET);
				fread(buffer,1,one_frame_yuv_data,fpyuv);
			}


			//memccpy(black_withebuffer,buffer,)
			//memcpy(black_withebuffer,buffer,black_white);
			//black_withebuffer[black_white] = '\0';
			
			
			/**
			*更新纹理
			* pix_w ？
			*/
			SDL_UpdateTexture(txture,NULL,buffer,pix_w);
			//SDL_UpdateTexture(txture,nullptr,black_withebuffer,pix_w);

			/************************************************************************/
			/*修改显示位置,视频显示大小                                   			*/ 
			/************************************************************************/
			rect.x =10;
			rect.y =200;
			rect.w = window_w/2;
			rect.h = window_h/2;

			SDL_RenderClear(render);
			/**
			*rect 告诉渲染器把纹理渲染到window的位置，以及在Window中的宽高
			*/
			SDL_RenderCopy(render,txture,NULL,&rect);
			SDL_RenderPresent(render);
			

		}
		else if(event.type ==SDL_WINDOWEVENT)
		{
			SDL_GetWindowSize(window,&window_w,&window_h);
			printf("window width*height:%d*%d\n",window_w,window_h);
			printf("timestamp %d",event.common.timestamp);
			
			//fflush(stdout);
		}
		else if(event.type == SDL_QUIT)
		{
			thread_exit = 1;

		}
		else if(event.type == BREAK_EVENT)
		{
			break;
		}
		else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
		{
			v_pause ^= 0x01;
		}
	
		
	}

	SDL_Quit();
	return 0;
}

