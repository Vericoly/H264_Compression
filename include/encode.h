extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}
#define STREAM_DURATION   20.0 /*Stream duration in seconds*/
#define STREAM_FRAME_RATE 20 /* 20 images/s */
#define INPUT_PIX_FMT    AV_PIX_FMT_BGR24 
#define OUTPUT_PIX_FMT    AV_PIX_FMT_YUV420P 

static void encode(AVCodecContext *enc_ctx, AVFrame *frame, AVPacket *pkt)
{
    int ret;

    ret = avcodec_send_frame(enc_ctx, frame);
    if (ret < 0) {
        exit(1);
    }

    while (ret >= 0) {
        ret = avcodec_receive_packet(enc_ctx, pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return;
        else if (ret < 0) {
            exit(1);
        }
        av_packet_unref(pkt);
    }
}

int encoder()
{
    av_register_all();
    const char *codec_name;
    const AVCodec *codec;
    AVCodecContext *c= NULL;
    int i, ret, x, y;
    FILE *f;
    AVFrame *frame, *tmp_frame;
    AVPacket *pkt;
    uint8_t endcode[] = { 0, 0, 1, 0xb7 };
    struct SwsContext *sws_ctx;
    
    codec_name = "libx264"; // H264 codec

    /* find the mpeg1video encoder */
    codec = avcodec_find_encoder_by_name(codec_name);
    if (!codec) {
        exit(1);
    }

    c = avcodec_alloc_context3(codec);
    if (!c) {
        exit(1);
    }

    pkt = av_packet_alloc();
    if (!pkt)
        exit(1);

    /* put sample parameters */
    c->bit_rate = 400000;
    /* resolution must be a multiple of two */
    c->width = 800;
    c->height = 600;
    /* frames per second */
    c->time_base = (AVRational){1, 20};
    c->framerate = (AVRational){20, 1};

    /* emit one intra frame every ten frames
     * check frame pict_type before passing frame
     * to encoder, if frame->pict_type is AV_PICTURE_TYPE_I
     * then gop_size is ignored and the output of encoder
     * will always be I frame irrespective to gop_size
     */
    c->gop_size = 10;
    c->max_b_frames = 1;
    c->pix_fmt = OUTPUT_PIX_FMT;

    if (codec->id == AV_CODEC_ID_H264)
        av_opt_set(c->priv_data, "preset", "slow", 0);

    /* open codec */
    ret = avcodec_open2(c, codec, NULL);
    if (ret < 0) {
        exit(1);
    }

    frame = av_frame_alloc();
    if (!frame) {
        exit(1);
    }
    frame->format = c->pix_fmt;
    frame->width  = c->width;
    frame->height = c->height;

    ret = av_frame_get_buffer(frame, 0);
    if (ret < 0) {
        exit(1);
    }

    tmp_frame = av_frame_alloc();
    if (!tmp_frame) {
        exit(1);
    }
    tmp_frame->format = INPUT_PIX_FMT;
    tmp_frame->width  = c->width;
    tmp_frame->height = c->height;

    ret = av_frame_get_buffer(tmp_frame, 0);
    if (ret < 0) {
        exit(1);
    }

    sws_ctx = sws_getContext(c->width, c->height,
                                          INPUT_PIX_FMT,
                                          c->width, c->height,
                                          OUTPUT_PIX_FMT,
                                          SWS_BICUBIC, NULL, NULL, NULL);

    /* Encode video loop*/
    for (i = 0; i < (STREAM_DURATION * STREAM_FRAME_RATE); i++) {
        fflush(stdout);

        /* Make sure the frame data is writable.*/
        ret = av_frame_make_writable(frame);
        if (ret < 0)
            exit(1);

        ret = av_frame_make_writable(tmp_frame);
        if (ret < 0)
            exit(1);

        /* Prepare a dummy image.*/
        for (y = 0; y < c->height; y++) {
            for (x = 0; x < c->width; x++) {
            tmp_frame->data[0][y * c->width * 3 + x * 3    ] = 255-(i*1);
            tmp_frame->data[0][y * c->width * 3 + x * 3 + 1] = 255-(i*2);
            tmp_frame->data[0][y * c->width * 3 + x * 3 + 2] = 255-(i*3);            
            }
        }

        /*Convert frame from BGR24 to YUV420P format*/
        sws_scale(sws_ctx, (const uint8_t * const *) tmp_frame->data,
                  tmp_frame->linesize, 0, c->height, frame->data,
                  frame->linesize);
        frame->pts = i;

        /* Encode the image */
        encode(c, frame, pkt);
    }

    /* flush the encoder */
    encode(c, NULL, pkt);

    avcodec_free_context(&c);
    av_frame_free(&frame);
    av_packet_free(&pkt);

    return 0;
}