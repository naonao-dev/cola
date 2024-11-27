/**
 * @FilePath     : /cola/tutorial/Common/TestCondition/MyParamCondition.h
 * @Description  :
 * @Author       : naonao
 * @Date         : 2024-11-27 14:33:43
 * @Version      : 0.0.1
 * @LastEditors  : naonao
 * @LastEditTime : 2024-11-27 14:37:37
 * @Copyright (c) 2024 by G, All Rights Reserved.
 **/
#ifndef NAO_MYPARAMCONDITION_H
#define NAO_MYPARAMCONDITION_H

#include "../common.h"
#include "../TestParams/MyParam.h"


class MyParamCondition : public nao::DCondition
{
public:
    /**
     * 在这里主要演示condition中可以通过获取上方参数的形式，
     * 来决定执行执行当前的第几个逻辑
     * @return
     */
    NIndex choose() override
    {
        auto* myParam = NAO_GET_DPARAM_WITH_NO_EMPTY(MyParam, "param1") int cnt = 0;
        {
            NAO_PARAM_READ_CODE_BLOCK(myParam)   // 如果当前算子，跟其他相关依赖算子不存在并行关系，则参数可以直接使用，不需要加锁
            cnt = myParam->iCount;
        }
        return (cnt % (int)getRange());
    }
};

#endif   // NAO_MYPARAMCONDITION_H