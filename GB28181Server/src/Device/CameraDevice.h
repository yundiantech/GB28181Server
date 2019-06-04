#ifndef CAMERADEVICE_H
#define CAMERADEVICE_H

#include <stdint.h>
#include <stdio.h>
#include <list>

#include "VideoChannel.h"

///设备数据，通过注册请求获取到的信息
struct CameraDevice
{
    std::string DeviceID;
    std::string IPAddress;
    int Port;

    std::list<VideoChannel*> channelList; //一个通道对应一路视频流，一个相机可以有多个通道

    bool operator == (CameraDevice node)//重载运算符函数的具体实现
    {
        bool isSame = false;

        if ((node.DeviceID.compare(this->DeviceID) == 0) && (node.IPAddress.compare(this->IPAddress) == 0))
        {
            isSame = true;
        }

        return isSame;
    }
};

#endif // CAMERADEVICE_H
