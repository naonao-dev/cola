/**
 * @FilePath     : /cola/tutorial/Common/TestNode/MyWriteParamNode.h
 * @Description  :
 * @Author       : naonao
 * @Date         : 2024-11-27 14:10:43
 * @Version      : 0.0.1
 * @LastEditors  : naonao
 * @LastEditTime : 2024-11-27 14:10:43
 * @Copyright (c) 2024 by G, All Rights Reserved.
 **/
#ifndef NAO_MYWRITEPARAMNODE_H
#define NAO_MYWRITEPARAMNODE_H

#include "../TestParams/MyParam.h"
#include "../common.h"


class MyWriteParamNode : public nao::DNode
{
public:
    NStatus init() override
    {
        NStatus status;
        /**
         * 推荐在init()中，将可能用到的参数创建好。也支持在run的时候创建
         * 支持在任意节点创建，任意节点读取同类型（MyParam）同名（"param1"）参数
         */
        status = NAO_CREATE_DPARAM(MyParam, "param1")
        return status;
    }

    NStatus run() override
    {
        /**
         * 为了提高执行效率，
         * 在【创建参数】的时候，【提供】锁保护机制
         * 在【获取参数】的时候，【不提供】锁保护的机制
         * 故无法通过在run()过程中，并发的通过 createGParam 和 getGParam 创建和获取参数
         * 如果需要做此操作，请自行外部加锁
         */
        auto* myParam = NAO_GET_DPARAM_WITH_NO_EMPTY(MyParam, "param1")
        int val = 0;
        int cnt = 0;
        {
            NAO_PARAM_WRITE_CODE_BLOCK(myParam)
            /**
             * 建议将需要的内容获取出来，然后在 CODE_BLOCK 外进行与param无关的参数加工（本例中为：打印信息）。
             * 这样做的好处，是可以尽可能减少 CODE_BLOCK 的范围
             */
            val = ++myParam->iValue;   // 对param中的信息，进行写入
            cnt = ++myParam->iCount;
        }
        /* 执行时，仅依赖val值，跟当前param1->iValue值无关，可以放到 CODE_BLOCK 外部 */
        nao::NAO_ECHO("[%s], iValue value is : [%d], iCount value is [%d] ... ", this->getName().c_str(), val, cnt);
        return NStatus();
    }
};

#endif   // NAO_MYWRITEPARAMNODE_H