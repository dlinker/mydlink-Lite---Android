#include <stdlib.h>
#include <stdio.h>
#include <jni.h>
#include <string.h>

#include "libavcodec/avcodec.h"
#include "libavutil/mathematics.h"
#include "libswscale/swscale.h"

AVCodec *codec;
AVCodecContext *c = NULL;
AVFrame *picture, *out_pic;
struct SwsContext* img_convert_ctx;
AVCodecParserContext *h264_parser;

#define MIN(a,b) ((a>b) ? b : a)

//#define WRITEYUV 0

JNIEXPORT jint JNICALL Java_com_dlink_mydlink_media_H264Decoder_InitDecoder(JNIEnv* env, jobject thiz, jint width, jint height)
{
	 /* must be called before using avcodec lib */ 
	 avcodec_init(); 
	
	 /* register all the codecs */
	 avcodec_register_all(); 
	
	 /* find the h264 video decoder */
	 codec = avcodec_find_decoder(CODEC_ID_H264); 
	 if (!codec)
		 return -1;
	
	 /* allocate memory for video decoder */ 
	 c = avcodec_alloc_context(); 
	 if (!c)
		 return -1;
	 c->width = width; 
	 c->height = height;
	
	 /* allocate frame buffer */
	 picture = avcodec_alloc_frame(); 
	 if (!picture)
		 return -1;
	
	 if(codec->capabilities & CODEC_CAP_DR1)
		 c->flags |= CODEC_FLAG_EMU_EDGE;
	
	 // Inform the codec that we can handle truncated bitstreams -- i.e.,
	 // bitstreams where frame boundaries can fall in the middle of packets
	 if(codec->capabilities & CODEC_CAP_TRUNCATED)
		 c->flags|=CODEC_FLAG_TRUNCATED;

	 c->flags|=CODEC_CAP_DELAY;
	
	 /* open codec */
	 if (avcodec_open(c, codec) < 0) { 
		 return -1; 
	 } 
	 
	 h264_parser = av_parser_init(CODEC_ID_H264);
	 if (!h264_parser)
		 return -1;

	 return 99;

} 
 
JNIEXPORT jint JNICALL Java_com_dlink_mydlink_media_H264Decoder_UninitDecoder(JNIEnv* env, jobject thiz)
{
	/* close codec and release memory */
	if(c) { 
		avcodec_close(c); 
		av_free(c); 
		c = NULL; 
	} 
	/* release decode frame buffer*/
	if(picture) { 
		av_free(picture); 
		picture = NULL; 
	}
	if (h264_parser) {
		av_free(h264_parser);
		h264_parser = NULL;
	}	
	return 0;
}



JNIEXPORT jint JNICALL Java_com_dlink_mydlink_media_H264Decoder_DecoderNal(JNIEnv* env, jobject thiz, jbyteArray in, jint nalLen, jbyteArray out)
{
	jbyte *Buf = (jbyte*) (*env)->GetByteArrayElements(env, in, 0);	
	jbyte *Pixel = (jbyte*) (*env)->GetByteArrayElements(env, out, 0);	

	int got_picture = 0, len = 0;
    AVPacket avpkt;
	unsigned char *inBuf = Buf;	
	int bufLen = nalLen;

	if (nalLen > 0) {
		do {
			av_init_packet(&avpkt);
			len = av_parser_parse2(h264_parser, c, &avpkt.data, &avpkt.size, inBuf, bufLen, AV_NOPTS_VALUE, AV_NOPTS_VALUE, AV_NOPTS_VALUE);
			if (avpkt.size)
				avcodec_decode_video2(c, picture, &got_picture, &avpkt);

			inBuf += len;
			bufLen -= len;
			av_free_packet(&avpkt);
		} while (bufLen > 0);

		if (got_picture) 
		{
 			out_pic = avcodec_alloc_frame();
 			if (!out_pic)
		 		return -1;  
 			img_convert_ctx = sws_getContext(c->width, c->height, PIX_FMT_YUV420P, c->width, c->height, PIX_FMT_RGB565,SWS_BICUBIC, NULL, NULL, NULL);
 			if (!img_convert_ctx)
	 			return -1;	
			avpicture_fill((AVPicture *)out_pic, Pixel, PIX_FMT_RGB565, c->width, c->height);
			sws_scale(img_convert_ctx, picture->data, picture->linesize, 0, c->height, out_pic->data, out_pic->linesize); 
			
			sws_freeContext(img_convert_ctx); img_convert_ctx = NULL;
			av_free(out_pic); out_pic = NULL;

			(*env)->ReleaseByteArrayElements(env, in, Buf, 0);
			(*env)->ReleaseByteArrayElements(env, out, Pixel, 0);
			return 1;				
		}
		
	}


	(*env)->ReleaseByteArrayElements(env, in, Buf, 0);
	(*env)->ReleaseByteArrayElements(env, out, Pixel, 0);
	return 0;
}

