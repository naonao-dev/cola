/**
 * @FilePath     : /cola/cola/UtilsCtrl/ThreadPool/Queue/UAtomicQueue.h
 * @Description  : 设计了一个安全队列
 * @Author       : naonao
 * @Version      : 0.0.1
 * @LastEditors  : naonao
 * @LastEditTime : 2024-11-15 14:36:38
 **/
#ifndef NAO_UATOMICQUEUE_H
#define NAO_UATOMICQUEUE_H

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>


#include "../UThreadPoolDefine.h"
#include "UQueueObject.h"

NAO_NAMESPACE_BEGIN

template<typename T>
class UAtomicQueue : public UQueueObject
{
public:
    UAtomicQueue() = default;

    /**
     * 等待弹出
     * @param value
     */
    NVoid waitPop(T& value)
    {
        NAO_UNIQUE_LOCK lk(mutex_);
        cv_.wait(lk, [this] { return !queue_.empty(); });
        value = std::move(*queue_.front());
        queue_.pop();
    }


    /**
     * 尝试弹出
     * @param value
     * @return
     */
    NBool tryPop(T& value)
    {
        NBool result = false;
        if (!queue_.empty() && mutex_.try_lock()) {
            if (!queue_.empty()) {
                value = std::move(*queue_.front());
                queue_.pop();
                result = true;
            }
            mutex_.unlock();
        }

        return result;
    }


    /**
     * 尝试弹出多个任务
     * @param values
     * @param maxPoolBatchSize
     * @return
     */
    NBool tryPop(std::vector<T>& values, int maxPoolBatchSize)
    {
        NBool result = false;
        if (!queue_.empty() && mutex_.try_lock()) {
            while (!queue_.empty() && maxPoolBatchSize-- > 0) {
                values.emplace_back(std::move(*queue_.front()));
                queue_.pop();
                result = true;
            }
            mutex_.unlock();
        }

        return result;
    }


    /**
     * 阻塞式等待弹出
     * @return
     */
    std::unique_ptr<T> popWithTimeout(NMSec ms)
    {
        NAO_UNIQUE_LOCK lk(mutex_);
        if (!cv_.wait_for(lk, std::chrono::milliseconds(ms),
                          [this] { return (!queue_.empty()) || (!ready_flag_); })) {
            return nullptr;
        }

        if (queue_.empty() || !ready_flag_) {
            return nullptr;
        }

        std::unique_ptr<T> result = std::move(queue_.front());
        queue_.pop();   // 如果等成功了，则弹出一个信息
        return result;
    }


    /**
     * 非阻塞式等待弹出
     * @return
     */
    std::unique_ptr<T> tryPop()
    {
        NAO_LOCK_GUARD lk(mutex_);
        if (queue_.empty()) {
            return std::unique_ptr<T>();
        }
        std::unique_ptr<T> ptr = std::move(queue_.front());
        queue_.pop();
        return ptr;
    }


    /**
     * 传入数据
     * @param value
     */
    NVoid push(T&& value)
    {
        std::unique_ptr<typename std::remove_reference<T>::type> task(
            c_make_unique<typename std::remove_reference<T>::type>(std::forward<T>(value)));
        while (true) {
            if (mutex_.try_lock()) {
                queue_.push(std::move(task));
                mutex_.unlock();
                break;
            }
            else {
                NAO_YIELD();
            }
        }
        cv_.notify_one();
    }


    /**
     * 判定队列是否为空
     * @return
     */
    NBool empty()
    {
        NAO_LOCK_GUARD lk(mutex_);
        return queue_.empty();
    }

    /**
     * 功能是通知所有的辅助线程停止工作
     * @return
     */
    NVoid reset() {
        ready_flag_ = false;
        cv_.notify_all();
    }


    /**
     * 初始化状态
     * @return
     */
    NVoid setup() {
        ready_flag_ = true;
        queue_ = {};
    }

    NAO_NO_ALLOWED_COPY(UAtomicQueue)

private:
    std::queue<std::unique_ptr<T>> queue_{};
    NBool ready_flag_ { true };                  // 执行标记，主要用于快速释放 destroy 逻辑中，多个辅助线程等待的状态
};

NAO_NAMESPACE_END

#endif   // NAO_UATOMICQUEUE_H
