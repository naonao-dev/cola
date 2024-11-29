/**
 * @FilePath     : /cola/tutorial/Common/TestNode/MyReadParamNode.h
 * @Description  :
 * @Author       : naonao
 * @Date         : 2024-11-27 14:11:06
 * @Version      : 0.0.1
 * @LastEditors  : naonao
 * @LastEditTime : 2024-11-27 14:17:30
 * @Copyright (c) 2024 by G, All Rights Reserved.
 **/
#ifndef NAO_MYREADPARAMNODE_H
#define NAO_MYREADPARAMNODE_H

#include "../TestParams/MyParam.h"
#include "../common.h"


class MyReadParamNode : public nao::DNode
{
public:
    NStatus run() override
    {
        auto    myParam = NAO_GET_DPARAM_WITH_NO_EMPTY(MyParam, "param1")   // 获取类型为MyParam且名为"param1"的参数
            int val     = 0;
        {
            /* 对需要使用（读或写）参数的位置，加括号{}范围限定，以减少互斥等待时间 */
            NAO_PARAM_READ_CODE_BLOCK(myParam)
            val = myParam->iValue;
        }
        nao::NAO_ECHO("[%s], iValue is : [%d] ... ", this->getName().c_str(), val);
        return NStatus();
    }
};

#endif   // NAO_MYREADPARAMNODE_H