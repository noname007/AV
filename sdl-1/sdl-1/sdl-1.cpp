// sdl-1.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
//#include "stdlib.h"

extern "C"{
#include "SDL2/SDL.h"
};


#define REFRESH_EVENT (SDL_USEREVENT + 1)
#define BREAK_EVENT (SDL_USEREVENT + 2)


int thread_exit = 0;

//int i = 0;
int refresh_video(void * opaque)
{
	thread_exit = 0;
	
	while(thread_exit == 0)
	{
		SDL_Event event;
		event.type = REFRESH_EVENT;
		//printf("%d=================================>\n",i++);
		SDL_PushEvent(&event);
		SDL_Delay(40);
	}
	SDL_Event event;
	thread_exit = 0;
	event.type = BREAK_EVENT;
	SDL_PushEvent(&event);

	return 0;
}
/************************************************************************/
/* 

��������
	------->SDL_Init ��ʼ�� init
	  |
	  |---->SDL_CreateWindow ����window 
	  |
	  |---->SDL_CreateRenderer ����render
	  |
	  |---->SDL_CreateTexture ����Texture
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

	//yuv ����һ֡�Ŀ���
	const int pix_w = 640 ,pix_h = 360;
	
	// ��ʾ���ڵĿ���
	int window_w = 640,window_h = 360;

	const int bpp =12;
	
	//һ������YUV����  8bit Y 2bit U 2bit V == 12 bit
	//��һ֡��Ƶ���ݴ�С w*h * 12/8 byte��С
	const int one_frame_yuv_data = pix_h*pix_w*bpp/8;
	//unsigned char buffer [pix_w *pix_h /8];
	//buffer ��С���䲻�㣬���³���һֱ��������һ�㶼������
	unsigned char buffer [one_frame_yuv_data];

	/**
	*SDL_Init��ʼ��
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

	SDL_Renderer *render = SDL_CreateRenderer(window,-1,0);

	Uint32 pixformat = SDL_PIXELFORMAT_IYUV;
	SDL_Texture *txture = SDL_CreateTexture(render,pixformat,SDL_TEXTUREACCESS_STREAMING,pix_w,pix_h);

	FILE *fpyuv = NULL;
	char filename[] = "sintel_640_360.yuv";
 	fpyuv = fopen(filename,"rb+");

	if(fpyuv == NULL)
	{
		printf("cant't open the file%s",filename);
		return -1;
	}

	SDL_Rect rect;

	SDL_Event event;
	SDL_Thread * thread = SDL_CreateThread(refresh_video,NULL,NULL);

	while (1)
	{
		SDL_WaitEvent(&event);
		//printf("%d************************\n",i++);
		//printf("Event %x\n",event.type);
		//fflush(stdout);
		if(event.type == REFRESH_EVENT)
		{
			if(fread(buffer,1,one_frame_yuv_data,fpyuv) != one_frame_yuv_data)
			{
				//read the end and read again
				fseek(fpyuv,0,SEEK_SET);
				fread(buffer,1,one_frame_yuv_data,fpyuv);
			}

			/**
			*��������
			* pix_w ��
			*/
			SDL_UpdateTexture(txture,NULL,buffer,pix_w);
			rect.x =0;
			rect.y =0;
			rect.w = window_w;
			rect.h = window_h;

			SDL_RenderClear(render);
			/**
			*rect ������Ⱦ����������Ⱦ��window��λ�ã��Լ���Window�еĿ���
			*/
			SDL_RenderCopy(render,txture,NULL,&rect);
			SDL_RenderPresent(render);
			

		}
		else if(event.type ==SDL_WINDOWEVENT)
		{
			SDL_GetWindowSize(window,&window_w,&window_h);
			printf("window width*height:%d*%d\n",window_w,window_h);
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
		
	}

	SDL_Quit();
	return 0;
}
