#include "GB28181Server.h"

#include <stdio.h>
#include <stdlib.h>

#if defined(WIN32)
    #include <winsock2.h>
    #include <windows.h>
#else
    #include <pthread.h>
#endif

#define LOCAL_PORT 5060
#define APP_LOG printf

GB28181Server::GB28181Server()
{

}

void GB28181Server::start()
{
    run();
}

void GB28181Server::run()
{
    int iReturnCode = 0;

    //初始化跟踪信息
    TRACE_INITIALIZE(6, NULL);

    //初始化eXosip和osip栈
    eCtx = eXosip_malloc();
    iReturnCode = eXosip_init(eCtx);
    if (iReturnCode != OSIP_SUCCESS)
    {
        APP_LOG("Can't initialize eXosip!");
        exit(1);
    }
    else
    {
        APP_LOG("eXosip_init successfully!\n");
    }

    //打开一个UDP socket 接收信号
    iReturnCode = eXosip_listen_addr(eCtx, IPPROTO_UDP, NULL, LOCAL_PORT, AF_INET, 0);
    if (iReturnCode != OSIP_SUCCESS)
    {
        APP_LOG("eXosip_listen_addr error!");
        exit(1);
    }


    //与相机进行消息交换的主线程
    //监听并回复摄像机消息
    while (1)
    {
        eXosip_event_t *je = NULL;
        //处理事件
        je = eXosip_event_wait(eCtx, 0, 4);
        if (je == NULL)
        {
            osip_usleep(100000);
            continue;
        }

        switch (je->type)
        {
            case EXOSIP_MESSAGE_NEW:				//新消息到来
            {
                APP_LOG("new msg method:%s\n", je->request->sip_method);

                //处理注册消息
                if ( MSG_IS_REGISTER(je->request) )
                {
                    ///这里暂时不做用户名密码的验证，直接发送注册成功的消息
                    RegisterSuccess(eCtx, je);
                }
                else if (MSG_IS_MESSAGE(je->request))
                {
                    RegisterSuccess(eCtx, je);
                }
                else if (strncmp(je->request->sip_method, "BYE", 4) != 0)
                {
                    APP_LOG("msg BYE\n");
                }

                break;
            }
            default:
                APP_LOG("##test,%s:%d, unsupport type:%d\n", __FILE__, __LINE__, je->type);
                RegisterSuccess(eCtx, je);
                break;
        }
        eXosip_event_free(je);
    }

    eXosip_quit(eCtx);
    osip_free(eCtx);
    eCtx = NULL;
}

void GB28181Server::RegisterSuccess(struct eXosip_t * peCtx,eXosip_event_t *je)
{
    int iReturnCode = 0 ;
    osip_message_t * pSRegister = NULL;
    iReturnCode = eXosip_message_build_answer (peCtx,je->tid,200,&pSRegister);

    char timeCh[32] = {0};

#if defined(WIN32)
    SYSTEMTIME sys;
    GetLocalTime( &sys );

    sprintf(timeCh,"%d-%02d-%02dT%02d:%02d:%02d.%03d",
            sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);

#else
    struct timeval    tv;
    struct timezone tz;

    struct tm         *p;

    gettimeofday(&tv, &tz);
    p = localtime(&tv.tv_sec);


    sprintf(timeCh,"%d-%02d-%02dT%02d:%02d:%02d.%03d",
            1900+p->tm_year, 1+p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, tv.tv_usec/1000);

#endif

//    osip_message_set_topheader (pSRegister, "date", "2018-04-25T21:42:40.075");
    osip_message_set_topheader (pSRegister, "date", timeCh);

    if ( iReturnCode == 0 && pSRegister != NULL )
    {
        eXosip_lock(peCtx);
        eXosip_message_send_answer (peCtx,je->tid,200,pSRegister);
        eXosip_unlock(peCtx);
    }
}

void GB28181Server::RegisterFailed(struct eXosip_t * peCtx,eXosip_event_t *je)
{
    int iReturnCode = 0 ;
    osip_message_t * pSRegister = NULL;
    iReturnCode = eXosip_message_build_answer (peCtx,je->tid, 401, &pSRegister);
    if ( iReturnCode == 0 && pSRegister != NULL )
    {
        eXosip_lock(peCtx);
        eXosip_message_send_answer (peCtx,je->tid,401, pSRegister);
        eXosip_unlock(peCtx);
    }
}
