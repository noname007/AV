#include "stdafx.h"
#include <stdio.h>

#define __STDC_CONSTANT_MACROS

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "SDL2/SDL.h"
};


#define SFM_REFRESH_EVENT  (SDL_USEREVENT + 1)
#define SFM_BREAK_EVENT  (SDL_USEREVENT + 2)

int thread_exit =0;





AVFormatContext * open_av_file(char * file)
{
	AVFormatContext	*pFormatCtx = avformat_alloc_context();

	if(avformat_open_input(&pFormatCtx,file,NULL,NULL)!=0){
		printf("Couldn't open input stream.\n");
		return NULL;
	}
	if(avformat_find_stream_info(pFormatCtx,NULL)<0){
		printf("Couldn't find stream information.\n");
		return NULL;
	}
	return pFormatCtx;
}


int find_video_index(AVFormatContext * pFormatCtx)
{

	int videoindex=-1;

	for(unsigned int i=0; i < pFormatCtx->nb_streams; i++) {
		if(pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){
			videoindex = i;
			break;
		}
	}

	return videoindex;
}


AVCodecContext * init_video_codec(AVFormatContext *pFormatCtx,int videoindex)
{
	AVCodecContext	*pCodecCtx;
	AVCodec			*pCodec;

	pCodecCtx=pFormatCtx->streams[videoindex]->codec;

	pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
	if(pCodec==NULL){
		printf("Codec not found.\n");
		return NULL;
	}
	if(avcodec_open2(pCodecCtx, pCodec,NULL)<0){
		printf("Could not open codec.\n");
		return NULL;
	}
	return pCodecCtx;
}

typedef struct {
	AVCodecContext *pCodecCtx;
	AVPacket * packet;
}Passed_t;


int diaplay_video_thread(void * opaque)
{
	Passed_t *data = (Passed_t *) opaque;
	
	AVPacket *packet = data->packet;
	AVCodecContext * pCodecCtx = data->pCodecCtx;

	int window_w = pCodecCtx->width;
	int window_h = pCodecCtx->height;
	
	AVFrame	*pFrame,*pFrameYUV;
	uint8_t *out_buffer;

	pFrame=av_frame_alloc();
	pFrameYUV=av_frame_alloc();
	out_buffer=(uint8_t *)av_malloc(avpicture_get_size(PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));
	avpicture_fill((AVPicture *)pFrameYUV, out_buffer, PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);


	struct SwsContext * img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, 
		pCodecCtx->width, pCodecCtx->height, PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL); 

	//===sdl

	if(SDL_Init(SDL_INIT_VIDEO))
	{
		printf("ERROR -%s",SDL_GetError());
		return -1;
	}else{
		printf("SUCCESS");
	}

	SDL_Window *window = SDL_CreateWindow("yangzz",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,window_w,window_h,SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);


	if(!window)
	{
		return -1;
	}

	SDL_Renderer *render = SDL_CreateRenderer(window,-1,0);

	Uint32 pixformat = SDL_PIXELFORMAT_IYUV;
	SDL_Texture *txture = SDL_CreateTexture(render,pixformat,SDL_TEXTUREACCESS_STREAMING,window_w,window_h);



	int ret, got_picture;
	SDL_Event event;
	while(1)
	{
		SDL_WaitEvent(&event);
		if(event.type != SFM_REFRESH_EVENT)
		{

			continue;
		}

		ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
		if(ret < 0){
			printf("Decode Error.\n");
			return -1;
		}
		if(got_picture){
			sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);

			SDL_UpdateTexture( txture, NULL, pFrameYUV->data[0], pFrameYUV->linesize[0] );  
			SDL_RenderClear( render );  
			SDL_RenderCopy( render, txture, NULL, NULL);  
			SDL_RenderPresent( render );  
			SDL_Delay(40);
		}
		
	}


	sws_freeContext(img_convert_ctx);
	av_frame_free(&pFrameYUV);
	av_frame_free(&pFrame);

	return 0;
}


int _tmain(int argc, char* argv[])
{

	AVFormatContext	*pFormatCtx;
	int				 videoindex;
	AVCodecContext	*pCodecCtx;
	AVCodec			*pCodec;

	AVPacket *packet;

	

	char filepath[]="Titanic.ts";

	av_register_all();

	pFormatCtx = open_av_file(filepath);

	if(pFormatCtx == NULL)
	{
		printf("openFile fail");
		return -1;
	}


	videoindex = find_video_index(pFormatCtx);
	if(videoindex < 0){
		printf("Didn't find a video stream.\n");
		return -1;
	}


	pCodecCtx = init_video_codec(pFormatCtx,videoindex);


	
	packet=(AVPacket *)av_malloc(sizeof(AVPacket));


	Passed_t data = {pCodecCtx,packet};

	SDL_Thread  *video = SDL_CreateThread(diaplay_video_thread,"yangzz_display_video",&data);
	SDL_Event event;
	while(av_read_frame(pFormatCtx, packet)>=0)
	{
		if(packet->stream_index==videoindex){
			event.type = SFM_REFRESH_EVENT;
			SDL_PushEvent(&event);
			av_free_packet(packet);
		}
	}

	thread_exit = 1;
	
	//SDL_WaitThread();

	SDL_Quit();


	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);

	return 0;
}

