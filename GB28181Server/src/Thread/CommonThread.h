#ifndef COMMONTHREAD_H
#define COMMONTHREAD_H

#include <thread>

/**
 * @brief The CommonThread class
 * 一直循环执行一个函数的线程
 */

class CommonThread
{
public:
    CommonThread();
    ~CommonThread();

public:
    void start();

    void stop(bool isWait = false);

protected:
    bool mIsStop;
    bool mIsThreadRunning;

    void run();

protected:
    virtual void threadFunc() = 0;

};

#endif // COMMONTHREAD_H
