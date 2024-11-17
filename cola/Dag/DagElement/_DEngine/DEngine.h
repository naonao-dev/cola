/**
 * @FilePath     : /cola/cola/Dag/DagElement/_DEngine/DEngine.h
 * @Description  :
 * @Author       : naonao
 * @Date         : 2024-06-26 11:29:25
 * @Version      : 0.0.1
 * @LastEditors  : naonao
 * @LastEditTime : 2024-11-15 13:19:43
 **/
#ifndef NAO_DENGINE_H
#define NAO_DENGINE_H

#include <queue>
#include <vector>


#include "../DElementObject.h"
#include "../DElementSorter.h"
#include "DEngineDefine.h"


NAO_NAMESPACE_BEGIN

class DEngine : public DElementObject
{
protected:
    explicit DEngine() = default;

    /**
     * 将所有注册进入 pipeline的内容，进行分析和解构
     * @param elements
     * @return
     */
    virtual NStatus setup(const DSortedDElementPtrSet& elements) = 0;


    /**
     * 分析所有的可以设置 linkable 的数据
     * @param elements
     * @return
     */
    NVoid link(const DSortedDElementPtrSet& elements)
    {
        /**
         * 认定图可以连通的判定条件：
         * 1，当前元素仅有一个后继
         * 2，当前元素的唯一后继，仅有一个前驱
         * 3，前后元素绑定机制是一样的
         */
        linked_size_ = 0;
        for (DElementPtr element : elements) {
            element->shape_ = internal::DElementShape::NORMAL;
            if (1 == element->run_before_.size() && 1 == (*element->run_before_.begin())->dependence_.size() && element->binding_index_ == (*(element->run_before_.begin()))->binding_index_) {
                element->shape_ = internal::DElementShape::LINKABLE;
                linked_size_++;
            }
        }
    }

    /**
     * 计算当前elements的 拓扑排序信息
     * @param elements
     * @return
     */
    static DElementPtrArr getTopo(const DSortedDElementPtrSet& elements)
    {
        DElementPtrArr          result;
        std::queue<DElementPtr> readyQueue;
        for (auto* element : elements) {
            element->left_depend_ = element->dependence_.size();
            if (0 == element->left_depend_) {
                readyQueue.push(element);
            }
        }

        while (!readyQueue.empty()) {
            auto* cur = readyQueue.front();
            readyQueue.pop();
            result.push_back(cur);

            for (auto* element : cur->run_before_) {
                if (0 == --element->left_depend_) {
                    readyQueue.push(element);
                }
            }
        }

        for (auto element : elements) {
            // 计算技术之后，需要恢复一下 depend的信息，以免引入误差
            element->left_depend_ = element->dependence_.size();
        }
        return result;
    }

    /**
     * 判断是否是dag的逻辑
     * @param elements
     * @return
     */
    static NBool isDag(const DSortedDElementPtrSet& elements)
    {
        const auto& result = getTopo(elements);
        return result.size() == elements.size();
    }

protected:
    UThreadPoolPtr thread_pool_{nullptr};                            // 内部执行的线程池
    int            schedule_strategy_ = NAO_DEFAULT_TASK_STRATEGY;   // 调度策略
    NSize          linked_size_       = 0;                           // 标记有多少个element，是 linkable 的数据

    friend class DElementManager;
    friend class DPipeline;
    friend class DMutable;
};

using DEnginePtr = DEngine*;

NAO_NAMESPACE_END

#endif   // NAO_DENGINE_H
