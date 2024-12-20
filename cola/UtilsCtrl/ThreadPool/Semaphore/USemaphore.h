﻿/**
 * @FilePath     : /cola/src/UtilsCtrl/ThreadPool/Semaphore/USemaphore.h
 * @Description  :
 * @Author       : naonao
 * @Version      : 0.0.1
 * @LastEditors  : naonao
 * @LastEditTime : 2024-06-20 19:53:42
 **/
#ifndef NAO_USEMAPHORE_H
#define NAO_USEMAPHORE_H
#include <condition_variable>
#include <mutex>


#include "../../../NBasic/NBasicInclude.h"
#include "../UThreadObject.h"
NAO_NAMESPACE_BEGIN

class USemaphore : public UThreadObject
{
public:
    /**
     * 触发一次信号
     */
    NVoid signal()
    {
        NAO_UNIQUE_LOCK lk(mutex_);
        cnt_++;
        if (cnt_ <= 0) {
            cv_.notify_one();
        }
    }

    /**
     * 等待信号触发
     */
    NVoid wait()
    {
        NAO_UNIQUE_LOCK lk(mutex_);
        cnt_--;
        if (cnt_ < 0) {
            cv_.wait(lk);
        }
    }

private:
    NInt                    cnt_ = 0;   // 记录当前的次数
    std::mutex              mutex_;
    std::condition_variable cv_;
};

NAO_NAMESPACE_END

#endif   // NAO_USEMAPHORE_H
