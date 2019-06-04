#ifndef GB28181SERVER_H
#define GB28181SERVER_H

#include <stdio.h>
#include <string>
#include <list>
#include <map>
#include <thread>

#include <eXosip2/eXosip.h>

#include "types.h"
#include "Device/CameraDevice.h"
#include "RtpReciever/RtpReciever.h"

class GB28181ServerEventHandle;

class GB28181Server
{
public:
    GB28181Server();

    /**
     * @brief setEventHandle 设置回调函数
     * @param handle
     */
    void setEventHandle(const GB28181ServerEventHandle *handle){mGB28181ServerEventHandle = (GB28181ServerEventHandle *)handle;}

    /**
     * @brief setLocalIp 设置本地IP（此IP用于与相机交互时，写在source里面的IP）
     * @param ip
     * @param port
     */
    void setLocalIp(const char *ip, const int &port);

    /**
     * @brief setGBServerInfo 设置GB服务器信息
     * @param sipId    [in] ip地址
     * @param passwd   [in] 密码
     * @param realm    [in] 域
     */
    void setGBServerInfo(const char *sipId, const char *passwd, const char *realm);

    void start();
    void stop();

    void doSendCatalog(const CameraDevice &device);
    void doSendInvitePlay(const VideoChannel *channelNode);

    std::list<CameraDevice> getDeviceList(){return mDeviceList;}

    ///根据ssrc查找对应的videoChannel
    VideoChannel* getVideoChannel(int ssrc);

protected:
    void run(); //线程执行函数

private:
    char SERVER_SIP_ID[256];
    char GB_PASSWORD[36];
    char GB_REALM[36];

    char LOCAL_IP[36];
    int LOCAL_PORT;

    int SN = 1;
    struct eXosip_t *eCtx;

    RtpReciever *mRtpReciever; //接收rtp流的线程

    bool mIsStop;          //是否收到停止命令
    bool mIsThreadRunning; //现在是否在运行

    std::map<int, VideoChannel*>  mDeviceVideoChannelMap; //rtp invite的ssrc和ChannelNode的映射表
    std::list<CameraDevice> mDeviceList; //设备列表

    void Register401Unauthorized(struct eXosip_t * peCtx, eXosip_event_t *je);
    void RegisterSuccess(struct eXosip_t * peCtx,eXosip_event_t *je);
    void RegisterFailed(struct eXosip_t * peCtx,eXosip_event_t *je);

    void Response(struct eXosip_t * peCtx,eXosip_event_t *je, int value); //答复相机的命令
    void Response403(struct eXosip_t * peCtx,eXosip_event_t *je); //答复403
    void Response200(struct eXosip_t * peCtx,eXosip_event_t *je); //答复200OK

    void ResponseCallAck(struct eXosip_t * peCtx, eXosip_event_t *je);

    int SendQueryCatalog(struct eXosip_t *peCtx , CameraDevice deviceNode); //请求设备目录

    //请求视频信息，SDP信息
    int SendInvitePlay(struct eXosip_t *peCtx, const VideoChannel *channelNode);

    ///设置invite请求结束
    bool setCallFinished(int callId);

    ///用于输出到界面上的回调函数
private:
    GB28181ServerEventHandle *mGB28181ServerEventHandle; //回调函数 用于传递信息给界面

    void deviceRegisted(const CameraDevice &device); //设备注册成功
    void deviceUpdate(const CameraDevice &device);   //设备更新，catalog请求返回的设备信息更新
    void receiveMessage(const char *deviceID, const MessageType &type, const char *msgBody);  //接收到消息

};

#endif // GB28181SERVER_H
