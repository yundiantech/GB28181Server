#ifndef DATALISTTHREAD_H
#define DATALISTTHREAD_H

#include <thread>
#include <list>

#include "Mutex/Cond.h"

#include "AppConfig.h"

/**
 * @brief The DataListThread class
 */

template<class T>
class DataListThread
{
public:
    DataListThread();
    virtual ~DataListThread();

public:
    void start();

    void stop(bool isWait = false);

    int inputData(const T &node);

    void clearData();

protected:
    bool mIsStop;
    bool mIsThreadRunning;

    void run();

private:
    Cond *mCondition;

    std::list<T> mList;

protected:
    virtual void threadStart();
    virtual void threadStop();
    virtual void dealwithDataNode(const T &node) = 0;

};


template<typename T>
DataListThread<T>::DataListThread()
{
    mIsStop = true;
    mIsThreadRunning = false;
    mCondition = new Cond;
}

template<typename T>
DataListThread<T>::~DataListThread()
{

}

template<typename T>
void DataListThread<T>::start()
{
    mIsStop = false;

    //启动新的线程
    std::thread([&](DataListThread *pointer)
    {
        pointer->run();

    }, this).detach();
}

template<typename T>
void DataListThread<T>::stop(bool isWait)
{
    mIsStop = true;

    if (isWait)
    {
        while(mIsThreadRunning)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

template<typename T>
int DataListThread<T>::inputData(const T &node)
{
    int size = 0;

    mCondition->Lock();
    mList.push_back(node);
    size = mList.size();
    mCondition->Unlock();

    return size;
}

template<typename T>
void DataListThread<T>::clearData()
{
    mCondition->Lock();
    mList.clear();
    mCondition->Unlock();
}

template<typename T>
void DataListThread<T>::run()
{
    mIsThreadRunning = true;

    threadStart();

    while(!mIsStop)
    {
        mCondition->Lock();

        if (mList.size() <= 0)
        {
            mCondition->Unlock();
            AppConfig::mSleep(100);
            continue;
        }

        T node = mList.front();
        mList.pop_front();
        mCondition->Unlock();

        dealwithDataNode(node);
    }

    mList.clear(); //清空队列

    threadStop();

    mIsThreadRunning = false;
}

template<typename T>
void DataListThread<T>::threadStart()
{

}

template<typename T>
void DataListThread<T>::threadStop()
{

}

#endif // DATALISTTHREAD_H
