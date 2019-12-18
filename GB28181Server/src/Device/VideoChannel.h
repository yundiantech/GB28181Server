#ifndef VIDEOCHANNEL_H
#define VIDEOCHANNEL_H

#include <stdint.h>
#include <stdio.h>
#include <list>

#include "Mutex/Mutex.h"
#include "Thread/DataListThread.h"

extern "C"
{
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include <libavutil/time.h>
    #include "libavutil/pixfmt.h"
    #include "libswscale/swscale.h"
    #include "libswresample/swresample.h"
    #include "libavutil/imgutils.h"
}

class VideoChannelEventHandle;

struct RtpDataNode
{
    uint8_t * buffer;
    int size;
    long time;
    bool isLostPacket; //是否丢帧了

    RtpDataNode()
    {
        buffer = NULL;
        size = 0;
        time = 0;
        isLostPacket = false;
    }
};

///通道数据，这个数据需要通过catalog后获取到的，基本上就是相机的一个通道
class VideoChannel : public DataListThread<RtpDataNode>
{
public:
    VideoChannel();
    ~VideoChannel();

    /**
     * @brief setEventHandle 设置回调函数
     * @param handle
     */
    void setEventHandle(const VideoChannelEventHandle *handle){mVideoChannelEventHandle = (VideoChannelEventHandle *)handle;}

    ///塞入rtp数据等待处理
    void inputRtpBuffer(uint8_t *buffer, int size, uint32_t sequenceNumber, bool isLastPacket);

    void setCallId(int callId){mCallId = callId;}
    int getCallId(){return mCallId;}

protected:
    void threadStart();
    void threadStop();
    void dealwithDataNode(const RtpDataNode &node);

public:
    int RtpSSRC; //rtp身份标志，用于invite请求，用来区分多个rtp流。
    std::string DeviceName;
    std::string DeviceID;
    std::string IPAddress;
    int Port;
    std::string Status;

private:
    int mCallId; //发送invite的call id, 此id不为-1 说明正在发送invite请求，发送invite过程中，不能再次发送invite请求，否则会有问题。

    uint32_t mReceiveRtpTime;     //接收到rtp数据的时间(用来判断rtp数据接收是否超时了)

    bool mIsCurrentFrameLostPacket;
    uint32_t mLastSequenceNumber; //上一次的rtp序号

    uint8_t *mRtpBuffer; //存放RTP收到的PS流
    int mRtpBufferSize;

    ///ffmpeg解码相关参数
    AVCodec *pCodec;
    AVCodecContext *pCodecCtx;
    AVFrame * pFrameRGB;
    AVFrame * pFrame;
    AVPacket packet;

    SwsContext * img_convert_ctx;
    uint8_t *out_buffer_rgb;
    int numBytes_rgb;

    char *mH264buf;

    bool mIsLastKeyFrameLostPacket; //上一个I帧是否丢包了，是的话，接下来的帧都有可能花屏 接下来所有的帧， 都不传入检测
    bool mIsKeyFrameGetted; //用来记录I帧是否获取到了 否则丢弃得到的h264帧

    bool openH264Decoder();
    void closeH264Decoder();

    void decodeH264Buffer(uint8_t *buffer, int size, bool isLostPacket);

    ///用于输出到界面上的回调函数
private:
    VideoChannelEventHandle *mVideoChannelEventHandle; //回调函数 用于传递信息给界面

    void dealWithRgb32Frame(const uint8_t *rgb32Buffer, const int bufferSize,  const int width, const int height, const bool isLostPacket, const bool isKeyFrame); //显示视频数据

};

#endif // VIDEOCHANNEL_H
