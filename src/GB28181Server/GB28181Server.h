#ifndef GB28181SERVER_H
#define GB28181SERVER_H

#include <stdio.h>
#include <string>
#include <list>

#include <eXosip2/eXosip.h>

///通道数据，这个数据需要通过catalog后获取到的，基本上就是相机的一个通道
struct ChannelItem
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

    std::list<ChannelItem> channelList; //一个通道对应一路视频流，一个相机可以有多个通道

};


class GB28181Server
{
public:
    GB28181Server();

    void start();

    int SN = 1;
    struct eXosip_t *eCtx;

    std::list<DeviceNode> mDeviceList;

private:
    void run();

    void Register401Unauthorized(struct eXosip_t * peCtx,eXosip_event_t *je);
    void RegisterSuccess(struct eXosip_t * peCtx,eXosip_event_t *je);
    void RegisterFailed(struct eXosip_t * peCtx,eXosip_event_t *je);

    void Response(struct eXosip_t * peCtx,eXosip_event_t *je, int value);
    void Response403(struct eXosip_t * peCtx,eXosip_event_t *je);
    void Response200(struct eXosip_t * peCtx,eXosip_event_t *je); //答复200OK

    int SendQueryCatalog(struct eXosip_t *peCtx , DeviceNode deviceNode);

};

#endif // GB28181SERVER_H
