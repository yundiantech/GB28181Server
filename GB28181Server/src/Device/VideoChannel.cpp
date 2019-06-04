#include "VideoChannel.h"

#include "AppConfig.h"
#include "RtpReciever/RtpReciever.h"
#include "EventHandle/EventHandle.h"

#define MAX_PS_BUFFER_SIZE 2048000
#define H264_FRAME_SIZE_MAX (1024*1024*2)

VideoChannel::VideoChannel()
{
    Port = 0;

    RtpSSRC = 1;

    mCallId = -1;

    mLastSequenceNumber = 0;
    mIsCurrentFrameLostPacket = false;

    mReceiveRtpTime = 0;

    mRtpBuffer = (uint8_t*)malloc(MAX_PS_BUFFER_SIZE);
    mRtpBufferSize = 0;

    pCodecCtx = nullptr;
    pCodec = nullptr;

    pCodecCtx = nullptr;
    pFrameRGB = nullptr;
    pFrame = nullptr;

    out_buffer_rgb = nullptr;
    numBytes_rgb = 0;

    mH264buf = nullptr;

    mVideoChannelEventHandle = nullptr;
}

VideoChannel::~VideoChannel()
{
    if (mRtpBuffer != nullptr)
        free(mRtpBuffer);
    mRtpBuffer = nullptr;
}

void VideoChannel::threadStart()
{
    mH264buf = (char *)malloc(H264_FRAME_SIZE_MAX);

    mIsLastKeyFrameLostPacket = false; //上一个I帧是否丢包了，是的话，接下来的帧都有可能花屏 接下来所有的帧， 都不传入检测
    mIsKeyFrameGetted = false; //用来记录I帧是否获取到了 否则丢弃得到的h264帧

    openH264Decoder();
}

void VideoChannel::threadStop()
{
    closeH264Decoder();

    if (mH264buf != nullptr)
        free(mH264buf);
}

void VideoChannel::dealwithDataNode(const RtpDataNode &node)
{
    unsigned char *ptr = node.buffer;
    int psLen = node.size;

    if (ptr[0] == 0x00 && ptr[1] == 0x00 && ptr[2] == 0x01 && ptr[3] == 0xBA && psLen > 0)
    {
        int h264length = 0;

        RtpReciever::GetH264FromPs((char *)ptr, psLen, mH264buf, &h264length);

        if (h264length > 0)
        {
            bool isKeyFrame = false;

//                ///还没获取到关键帧 则先判断是否是关键帧 否则丢弃
//                if (!isKeyFrameGetted)
            {
                int pos = 0;
                while(1)
                {
                    if ((h264length - pos) < 4)
                    {
                        break;
                    }

                    unsigned char *buf = (unsigned char *)(mH264buf+pos);

                    int index = 0;

                    if (buf[0] == 0x00 && buf[1] == 0x00 && buf[2] == 0x01)
                    {
                        index = 3;
                    }
                    else if (buf[0] == 0x00 && buf[1] == 0x00 && buf[2] == 0x00 && buf[3] == 0x01)
                    {
                        index = 4;
                    }
                    else
                    {
                        pos++;
                        continue;
                    }

                    int nal_unit_type = (buf[index]) & 0x1f;                   // 5 bit

                    if (nal_unit_type == 7)
                    {
                        mIsLastKeyFrameLostPacket = node.isLostPacket;
                        isKeyFrame = true;
                        mIsKeyFrameGetted = true;
                        break;
                    }

                    pos++;
                }
            }

            if (mIsKeyFrameGetted)
            {
                bool isLostPacket = node.isLostPacket;

                if (node.isLostPacket)
                {
                    fprintf(stderr, "lost packet! isKeyFrame=%d\n",isKeyFrame);
                }

                if (mIsLastKeyFrameLostPacket) //I帧丢了，接下来的帧都不检测，直到遇到下一个I帧
                {
                    isLostPacket = true;
                }

                decodeH264Buffer((uint8_t*)mH264buf, h264length, isLostPacket);
            }
        }
    }

    free(node.buffer);

}

///塞入rtp数据等待处理
void VideoChannel::inputRtpBuffer(uint8_t *buffer, int size, uint32_t sequenceNumber, bool isLastPacket)
{
    if ((sequenceNumber - mLastSequenceNumber ) != 1)
    {
        if (mLastSequenceNumber != 0 && sequenceNumber != 0)
        {
            mIsCurrentFrameLostPacket = true;

             fprintf(stderr,"sequenceNumber=%d %d\n", sequenceNumber, mLastSequenceNumber);
        }
    }

    mLastSequenceNumber = sequenceNumber;

    mReceiveRtpTime = AppConfig::getTimeStamp_MilliSecond();

    unsigned char *ptr = buffer;

    ///这是一个ps头 则处理上一包
    if (ptr[0] == 0x00 && ptr[1] == 0x00 && ptr[2] == 0x01 && ptr[3] == 0xBA)
    {
        if (mRtpBufferSize > 0)
        {
            RtpDataNode node;
            node.isLostPacket = mIsCurrentFrameLostPacket;
            node.size = mRtpBufferSize;
            node.buffer = (uint8_t *)malloc(mRtpBufferSize+10);
            memcpy(node.buffer, mRtpBuffer, mRtpBufferSize);

            int listSize = inputData(node);

            mRtpBufferSize = 0;
            mIsCurrentFrameLostPacket = false;

            if (listSize > 20)
            {
                fprintf(stderr, "decode too slow! mRtpBufferList=%d RtpSSRC=%d\n", listSize, RtpSSRC);
            }
        }
    }

    if ((mRtpBufferSize + size) > MAX_PS_BUFFER_SIZE)
    {
        fprintf(stderr, "rtp packet out of range! mRtpBufferSize=%d size=%d isLastPacket=%d\n", mRtpBufferSize, size, isLastPacket);
        mRtpBufferSize = 0;
        return;
    }

    memcpy(mRtpBuffer + mRtpBufferSize, buffer, size);
    mRtpBufferSize += size;

}

bool VideoChannel::openH264Decoder()
{
    bool isOpenHardDecoderucceed = false;

    pCodecCtx = avcodec_alloc_context3(NULL);

    ///查找硬件解码器
    pCodec = avcodec_find_decoder_by_name("h264_cuvid");

    if (pCodec == NULL)
    {
        fprintf(stderr,"h264_cuvid Codec not found.\n");
    }
    else
    {
        ///打开解码器
        if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
        {
            avcodec_close(pCodecCtx);
            fprintf(stderr,"Could not open codec h264_cuvid\n");
        }
        else
        {
            isOpenHardDecoderucceed = true;
        }

    }


    if (!isOpenHardDecoderucceed)
    {
        fprintf(stderr,"\n\nOpen h264_cuvid failed! use AV_CODEC_ID_H264\n\n");

        pCodec = avcodec_find_decoder(AV_CODEC_ID_H264);

        if (pCodec == NULL)
        {
    //        qDebug("Codec not found.\n");
            return false;
        }

        pCodecCtx->thread_count = 8;

        ///打开解码器
        if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
            printf("Could not open codec.\n");
            avcodec_close(pCodecCtx);
            return false;
        }
    }
    else
    {
        fprintf(stderr,"\n\nOpen h264_cuvid succeed !\n\n");
    }

    pFrame = av_frame_alloc();

    av_init_packet(&packet);


printf("open codec succeed!\n");
    return true;
}

void VideoChannel::closeH264Decoder()
{

    if (pCodecCtx == NULL) {
        avcodec_close(pCodecCtx);
    }

    av_frame_free(&pFrame);
    av_frame_free(&pFrameRGB);

    av_free(out_buffer_rgb);

    pCodecCtx = NULL;
    pCodec = NULL;

    out_buffer_rgb = NULL;

    pFrameRGB = NULL;
    pFrame = NULL;
}

void VideoChannel::decodeH264Buffer(uint8_t *buffer, int size, bool isLostPacket)
{
    packet.data = buffer;
    packet.size = size;

    if (avcodec_send_packet(pCodecCtx, &packet) != 0)
    {
       fprintf(stderr, "input AVPacket to decoder failed!\n");
       return;
    }

    while (0 == avcodec_receive_frame(pCodecCtx, pFrame))
    {
        ///判断解码完毕的帧是否是关键帧
        bool isKeyFrame = false;

        if(pFrame->key_frame)
        {
            isKeyFrame = true;
        }

        if (pFrameRGB == NULL)
        {
            pFrameRGB = av_frame_alloc();

            ///将解码后的YUV数据转换成RGB32
            img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height,
                    pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height,
                    AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);

            numBytes_rgb = avpicture_get_size(AV_PIX_FMT_RGB32, pCodecCtx->width, pCodecCtx->height);
            out_buffer_rgb = (uint8_t *) av_malloc(numBytes_rgb * sizeof(uint8_t));

            avpicture_fill((AVPicture *) pFrameRGB, out_buffer_rgb, AV_PIX_FMT_RGB32,
                    pCodecCtx->width, pCodecCtx->height);
        }

        if (isLostPacket)
        {
            fprintf(stderr,"current frame lost packet! do not dealwith it! \n");
        }
        else
        {
            sws_scale(img_convert_ctx,
                    (uint8_t const * const *) pFrame->data,
                    pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data,
                    pFrameRGB->linesize);

            dealWithRgb32Frame(out_buffer_rgb, numBytes_rgb, pCodecCtx->width, pCodecCtx->height, isLostPacket, isKeyFrame);
        }
    }

}

void VideoChannel::dealWithRgb32Frame(const uint8_t *rgb32Buffer, const int bufferSize,  const int width, const int height, const bool isLostPacket, const bool isKeyFrame)
{
//    fprintf(stderr, "%s %d %d\n", __FUNCTION__, width, height);

    if (mVideoChannelEventHandle != nullptr)
    {
        mVideoChannelEventHandle->onDisPlayVideoData(rgb32Buffer, bufferSize, width, height);
    }
}
