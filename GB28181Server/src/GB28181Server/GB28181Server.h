#ifndef GB28181SERVER_H
#define GB28181SERVER_H

#include <stdio.h>
#include <string>
#include <list>

#include <eXosip2/eXosip.h>

///通道数据，这个数据需要通过catalog后获取到的，基本上就是相机的一个通道
struct ChannelNode
{
    std::string DeviceID;
    std::string IPAddress;
    int Port;
    std::string Status;
};

///设备数据，通过注册请求获取到的信息
struct DeviceNode
{
    std::string DeviceID;
    std::string IPAddress;
    int Port;

    std::list<ChannelNode> channelList; //一个通道对应一路视频流，一个相机可以有多个通道

    bool operator == (DeviceNode node)//重载运算符函数的具体实现
    {
        bool isSame = false;

        if ((node.DeviceID.compare(this->DeviceID) == 0) && (node.IPAddress.compare(this->IPAddress) == 0))
        {
            isSame = true;
        }

        return isSame;
    }

};

enum MessageType
{
    MessageType_Register = 0,
    MessageType_KeepAlive,
    MessageType_Catalog,
    MessageType_CallAnswer,
    MessageType_CallFailed
};

class GB28181Server
{
public:
    GB28181Server();

    void setLocalIp(char *ip, int port);
    void setGBServerInfo(char *sipId, char *passwd, char *realm);

    void start();
    void stop();

    void doSendCatalog(DeviceNode node);
    void doSendInvitePlay(ChannelNode node);

    std::list<DeviceNode> getDeviceList(){return mDeviceList;}

    void run(); //线程执行函数

protected:
    virtual void deviceRegisted(DeviceNode node); //设备注册成功
    virtual void deviceUpdate(DeviceNode node);   //设备更新，catalog请求返回的设备信息更新
    virtual void receiveMessage(const char *deviceID, MessageType type, char *msgBody);  //接收到消息

private:
    char SERVER_SIP_ID[256];
    char GB_PASSWORD[36];
    char GB_REALM[36];

    char LOCAL_IP[36];
    int LOCAL_PORT;

    int SN = 1;
    struct eXosip_t *eCtx;

    bool mIsStop;          //是否收到停止命令
    bool mIsThreadRunning; //现在是否在运行

    std::list<DeviceNode> mDeviceList;

    void Register401Unauthorized(struct eXosip_t * peCtx,eXosip_event_t *je);
    void RegisterSuccess(struct eXosip_t * peCtx,eXosip_event_t *je);
    void RegisterFailed(struct eXosip_t * peCtx,eXosip_event_t *je);

    void Response(struct eXosip_t * peCtx,eXosip_event_t *je, int value); //答复相机的命令
    void Response403(struct eXosip_t * peCtx,eXosip_event_t *je); //答复403
    void Response200(struct eXosip_t * peCtx,eXosip_event_t *je); //答复200OK

    void ResponseCallAck(struct eXosip_t * peCtx, eXosip_event_t *je);

    int SendQueryCatalog(struct eXosip_t *peCtx , DeviceNode deviceNode); //请求设备目录

    //请求视频信息，SDP信息
    int SendInvitePlay(struct eXosip_t *peCtx, ChannelNode cameraNode);

};

#endif // GB28181SERVER_H
