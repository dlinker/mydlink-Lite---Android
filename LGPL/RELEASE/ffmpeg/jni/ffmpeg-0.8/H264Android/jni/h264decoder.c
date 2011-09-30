#include <stdlib.h>
#include <stdio.h>
#include <jni.h>
#include <string.h>

#include "libavcodec/avcodec.h"
#include "libavutil/mathematics.h"

AVCodec *codec;
AVCodecContext *c = NULL;
AVFrame *picture;

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
	
	/* allocate frame buffer */
	picture = avcodec_alloc_frame(); 
	if (!picture)
        return -1;

	/* open codec */
	if (avcodec_open(c, codec) < 0) { 
		return -1; 
	} 

} 
 

JNIEXPORT jint JNICALL jint Java_com_dlink_mydlink_media_H264Decoder_UninitDecoder(JNIEnv* env, jobject thiz)
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
}

JNIEXPORT jint JNICALL jint Java_com_dlink_mydlink_media_H264Decoder_DecoderNal(JNIEnv* env, jobject thiz, jbyteArray in, jint nalLen, jbyteArray out)
{
    int got_picture, len = 0;
    AVPacket avpkt;

	if (insize > 0) {
		avpkt.size = insize;
		avpkt.data = in;
		
		len = avcodec_decode_video2(c, picture, &got_picture, &avpkt);

		if (len < 0) {
//			fprintf(stderr, "Error while decoding frame!");
			av_log(NULL, AV_LOG_ERROR, "Error when decoding frame!\n");
			return -1;
		}
		if (got_picture) {
			out = picture->data;
		}
		
	}
	
	return len;
}


