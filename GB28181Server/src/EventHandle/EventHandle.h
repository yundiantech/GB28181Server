#ifndef EVENTHANDLE_H
#define EVENTHANDLE_H

#include "types.h"

#include "Device/CameraDevice.h"

class GB28181ServerEventHandle
{
    ///用于输出到界面上的回调函数
public:
    virtual void onDeviceRegisted(const CameraDevice &device) = 0; //设备注册成功
    virtual void onDeviceUpdate(const CameraDevice &device) = 0;   //设备更新，catalog请求返回的设备信息更新
    virtual void onReceiveMessage(const char *deviceID, const MessageType &type, const char *msgBody) = 0;  //接收到消息

};

class VideoChannelEventHandle
{
    ///用于输出到界面上的回调函数
public:
    virtual void onDisPlayVideoData(const uint8_t *rgb32Buffer, const int bufferSize,  const int width, const int height) = 0; //显示视频数据

};

#endif // EVENTHANDLE_H
