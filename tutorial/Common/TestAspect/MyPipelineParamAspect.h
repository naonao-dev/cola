/**
 * @FilePath     : /cola/tutorial/Common/TestAspect/MyPipelineParamAspect.h
 * @Description  :
 * @Author       : naonao
 * @Date         : 2024-11-27 15:24:49
 * @Version      : 0.0.1
 * @LastEditors  : naonao
 * @LastEditTime : 2024-11-27 15:30:22
 * @Copyright (c) 2024 by G, All Rights Reserved.
 **/
#ifndef NAO_MYPIPELINEPARAMASPECT_H
#define NAO_MYPIPELINEPARAMASPECT_H

#include "../TestParams/MyParam.h"
#include "../common.h"

class MyPipelineParamAspect : public nao::DAspect
{
public:
    NStatus beginRun() override
    {
        NStatus status;
        auto*   pipelineParam = NAO_GET_DPARAM_WITH_NO_EMPTY(MyParam, "param1")
            /** 遇到并发情况，请考虑加锁保护（参考 T05-Param 中逻辑） */
            int cnt = pipelineParam->iCount;
        nao::NAO_ECHO("----> [MyPipelineParamAspect] pipeline param iCount is [%d] before run.", cnt);

        if (cnt < 0) {
            /** 模拟：在切面中，对pipeline中的参数，做一些异常值的处理逻辑
             * 在 beginRun() 切面中 返回非STATUS_OK值，则pipeline停止执行
             * */
            return NStatus("aspect demo error");
        }
        return status;
    }
};

#endif   // NAO_MYPIPELINEPARAMASPECT_H