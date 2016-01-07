/**
 * ��򵥵Ļ���FFmpeg�Ľ�����
 * Simplest FFmpeg Decoder
 *
 * ������ Lei Xiaohua
 * leixiaohua1020@126.com
 * �й���ý��ѧ/���ֵ��Ӽ���
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * ������ʵ������Ƶ�ļ��Ľ���(֧��HEVC��H.264��MPEG2��)��
 * ����򵥵�FFmpeg��Ƶ���뷽��Ľ̡̳�
 * ͨ��ѧϰ�����ӿ����˽�FFmpeg�Ľ������̡�
 * This software is a simplest video decoder based on FFmpeg.
 * Suitable for beginner of FFmpeg.
 *
 */



#include <stdio.h>

#define __STDC_CONSTANT_MACROS

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
};

int find_video_index(AVFormatContext *pFormatCtx){
	static AVFormatContext *temp = NULL;
	static int index = 0;
	
	if(temp == pFormatCtx)
	{
		return index;
	}

	


}

int main(int argc, char* argv[])
{
	AVFormatContext	*pFormatCtx;
	int				i, videoindex;
	AVCodecContext	*pCodecCtx;
	AVCodec			*pCodec;
	AVFrame	*pFrame,*pFrameYUV;
	uint8_t *out_buffer;
	AVPacket *packet;
	int y_size;
	int ret, got_picture;
	struct SwsContext *img_convert_ctx;
	//�����ļ�·��
	char filepath[]="Titanic.ts";

	int frame_cnt;

	av_register_all();
	avformat_network_init();
	pFormatCtx = avformat_alloc_context();

	if(avformat_open_input(&pFormatCtx,filepath,NULL,NULL)!=0){
		printf("Couldn't open input stream.\n");
		return -1;
	}
	if(avformat_find_stream_info(pFormatCtx,NULL)<0){
		printf("Couldn't find stream information.\n");
		return -1;
	}
	videoindex=-1;

	for(unsigned int i=0; i < pFormatCtx->nb_streams; i++) {
		if(pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){
			videoindex = i;
		}
	}

	if(videoindex==-1){
		printf("Didn't find a video stream.\n");
		return -1;
	}

	pCodecCtx=pFormatCtx->streams[videoindex]->codec;

	pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
	if(pCodec==NULL){
		printf("Codec not found.\n");
		return -1;
	}
	if(avcodec_open2(pCodecCtx, pCodec,NULL)<0){
		printf("Could not open codec.\n");
		return -1;
	}
	/*
	 * �ڴ˴���������Ƶ��Ϣ�Ĵ���
	 * ȡ����pFormatCtx��ʹ��fprintf()
	 */
	FILE *fp = fopen("info.txt","wb+");
	fprintf(fp,"ʱ����%d\r\n",pFormatCtx->duration/1000000); 
	fprintf(fp,"��װ��ʽ %s\r\n",pFormatCtx->iformat->long_name);
	fprintf(fp,"��� %d*%d\r\n",pFormatCtx->streams[videoindex]->codec->width,pFormatCtx->streams[videoindex]->codec->height);
	fclose(fp);

   	pFrame=av_frame_alloc();
	pFrameYUV=av_frame_alloc();
	out_buffer=(uint8_t *)av_malloc(avpicture_get_size(PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));
	avpicture_fill((AVPicture *)pFrameYUV, out_buffer, PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);
	packet=(AVPacket *)av_malloc(sizeof(AVPacket));
	//Output Info-----------------------------
	printf("--------------- File Information ----------------\n");
	av_dump_format(pFormatCtx,0,filepath,0);
	printf("-------------------------------------------------\n");
	img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, 
		pCodecCtx->width, pCodecCtx->height, PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL); 


	FILE * fp264 = fopen("test264.h264","wb+");
	FILE * fpyuv = fopen("testyuv.yuv","wb+");

	frame_cnt=0;
	while(av_read_frame(pFormatCtx, packet)>=0){
		if(packet->stream_index==videoindex){
				/*
				 * �ڴ˴�������H264�����Ĵ���
				 * ȡ����packet��ʹ��fwrite()
				 */
			//fwrite(packet->data,1,packet->size,fl);
			//fwrite(packet->data,1024,packet->size/1024,fp264); //may have some question ,result in error displaying not espected
			fwrite(packet->data,1,packet->size,fp264);
			ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
			if(ret < 0){
				printf("Decode Error.\n");
				return -1;
			}
			if(got_picture){
				sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, 
					pFrameYUV->data, pFrameYUV->linesize);
				printf("Decoded frame index: %d\n",frame_cnt);

				/*
				 * �ڴ˴�������YUV�Ĵ���
				 * ȡ����pFrameYUV��ʹ��fwrite()
				 */
				int size = pCodecCtx->width * pCodecCtx->height;
				fwrite(pFrameYUV->data[0],1,size,fpyuv);
				fwrite(pFrameYUV->data[1],1,size/4,fpyuv);
				fwrite(pFrameYUV->data[2],1,size/4,fpyuv);
				frame_cnt++;

			}
		}
		av_free_packet(packet);
	}

	fclose(fp264);

	sws_freeContext(img_convert_ctx);

	av_frame_free(&pFrameYUV);
	av_frame_free(&pFrame);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);

	return 0;
}

