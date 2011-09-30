#include <stdlib.h>
#include <stdio.h>
#include <jni.h>
#include <string.h>

#include "libavcodec/avcodec.h"
#include "libavutil/mathematics.h"

AVCodec *codec;
AVCodecContext *c = NULL;
AVFrame *picture;
//static int offset = 0;

#define MIN(a,b) ((a>b) ? b : a)

JNIEXPORT jint JNICALL Java_com_dlink_mydlink_media_H264Decoder_InitDecoder(JNIEnv* env, jobject thiz, jint width, jint height)
//JNIEXPORT jint JNICALL Java_264_com_VView_InitDecoder(JNIEnv* env, jobject thiz, jint width, jint height)
{
	#if 1
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

	/* open codec */
	if (avcodec_open(c, codec) < 0) { 
		return -1; 
	} 
   #endif
    return -7;

} 
 
JNIEXPORT jint JNICALL Java_com_dlink_mydlink_media_H264Decoder_UninitDecoder(JNIEnv* env, jobject thiz)
//JNIEXPORT jint JNICALL Java_264_com_VView_UninitDecoder(JNIEnv* env, jobject thiz)
{
	#if 1
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
    #endif
	return 0;
}



JNIEXPORT jint JNICALL Java_com_dlink_mydlink_media_H264Decoder_DecoderNal(JNIEnv* env, jobject thiz, jbyteArray in, jint nalLen, jbyteArray out)
//JNIEXPORT jint JNICALL Java_264_com_VView_DecoderNal(JNIEnv* env, jobject thiz, jbyteArray in, jint nalLen, jbyteArray out)
{
#if 1
    int got_picture = 0, len = 0;
    AVPacket avpkt;
	
	jbyte *Buf = (jbyte*) (*env)->GetByteArrayElements(env, in, 0);	
	jbyte *Pixel = (jbyte*) (*env)->GetByteArrayElements(env, out, 0);	
	if (nalLen > 0) {
		avpkt.size = nalLen;
		avpkt.data = Buf;

		if (c == NULL) return -1;
		if (picture == NULL) return -2;
		
		do {
			len = avcodec_decode_video2(c, picture, &got_picture, &avpkt);
			if (len < 0)
			{
				return -3;
			}
			avpkt.size -= len;
			avpkt.data += len;
			
		} while (!got_picture && (avpkt.size > 4) );

		if (got_picture) {
			unsigned char *p_src, *p_dst = Pixel;
			int i, i_src_stride, i_dst_stride, iLoopCount, i_size;
			// Y
			p_src = picture->data[0];
			i_src_stride = picture->linesize[0];
			i_dst_stride = c->width;
			i_size = MIN( i_src_stride, i_dst_stride );
			iLoopCount = c->height;
			for( i = 0; i < iLoopCount; i++)
			{
				memcpy( p_dst, p_src, i_size );
				p_src += i_src_stride;
				p_dst += i_dst_stride;
			}
			
			// U
			p_src = picture->data[1];
			i_src_stride = picture->linesize[1];
			i_dst_stride = c->width/2;
			i_size = MIN(i_src_stride, i_dst_stride);
			iLoopCount = c->height/2;
			for( i = 0; i < iLoopCount; i++)
			{
				memcpy( p_dst, p_src, i_size );
				p_src += i_src_stride;
				p_dst += i_dst_stride;
			}
			
			// V
			p_src = picture->data[2];
			i_src_stride = picture->linesize[2];
			i_dst_stride = c->width/2;
			i_size = MIN( i_src_stride, i_dst_stride );
			iLoopCount = c->height/2;
			for( i = 0; i < iLoopCount; i++)
			{
				memcpy( p_dst, p_src, i_size );
				p_src += i_src_stride;
				p_dst += i_dst_stride;
			}
		
			return 0;
		}
		
	}
	(*env)->ReleaseByteArrayElements(env, in, Buf, 0);
	(*env)->ReleaseByteArrayElements(env, out, Pixel, 0);

	return len;
#endif
}

