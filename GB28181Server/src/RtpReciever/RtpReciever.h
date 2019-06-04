#ifndef RTPRECEIVER_H
#define RTPRECEIVER_H

#include <stdint.h>
#include <string.h>
#include <iostream>

#define MEDIASERVER_RTP_PORT 8000

class RtpReciever
{
public:
    RtpReciever();
    ~RtpReciever();

public:
    void start();

    void stop(bool isWait = false);

protected:
    bool mIsStop;
    bool mIsThreadRunning;

    void run();

public:
    /**
     * @brief GetH264FromPs   rtp解析h264视频信息
     * @param buffer          输入rtp数据（国标的ps封装）
     * @param length          长度
     * @param h264Buffer      输出的h264数据
     * @param h264length_out
     * @return
     */
    static int GetH264FromPs(char* buffer, int length, char *h264Buffer, int *h264length_out);


};

#endif // RTPRECEIVER_H
