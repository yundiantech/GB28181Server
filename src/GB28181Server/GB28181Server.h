#ifndef GB28181SERVER_H
#define GB28181SERVER_H

#include <eXosip2/eXosip.h>

class GB28181Server
{
public:
    GB28181Server();

    void start();

    int SN = 1;
    struct eXosip_t *eCtx;

private:
    void run();

    void Register401Unauthorized(struct eXosip_t * peCtx,eXosip_event_t *je);
    void RegisterSuccess(struct eXosip_t * peCtx,eXosip_event_t *je);
    void RegisterFailed(struct eXosip_t * peCtx,eXosip_event_t *je);

};

#endif // GB28181SERVER_H
