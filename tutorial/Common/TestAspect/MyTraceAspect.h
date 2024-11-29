/**
 * @FilePath     : /cola/tutorial/Common/TestAspect/MyTraceAspect.h
 * @Description  :
 * @Author       : naonao
 * @Date         : 2024-11-27 15:08:59
 * @Version      : 0.0.1
 * @LastEditors  : naonao
 * @LastEditTime : 2024-11-27 15:08:59
 * @Copyright (c) 2024 by G, All Rights Reserved.
 **/
#ifndef NAO_MYTRACEASPECT_H
#define NAO_MYTRACEASPECT_H

#include "../common.h"

class MyTraceAspect : public nao::DAspect
{
public:
    /**
     * 实现几个切面函数逻辑，模拟在对应执行的位置，打印trace信息
     */
    NStatus beginInit() override
    {
        nao::NAO_ECHO("----> [MyTraceAspect] [%s] init begin ...", this->getName().c_str());
        return NStatus();
    }

    NVoid finishInit(const NStatus& curStatus) override { nao::NAO_ECHO("----> [MyTraceAspect] [%s] init finished, error code is [%d] ...", this->getName().c_str(), curStatus.getCode()); }

    NStatus beginRun() override
    {
        nao::NAO_ECHO("----> [MyTraceAspect] [%s] run begin ...", this->getName().c_str());
        return NStatus();
    }

    NVoid finishRun(const NStatus& curStatus) override
    {
        if (!curStatus.isOK()) {
            nao::NAO_ECHO("----> [MyTraceAspect] [%s] run finished, status is ok ...", this->getName().c_str());
        }
        else {
            nao::NAO_ECHO("----> [MyTraceAspect] [%s] run finished, error code is [%d] ...", this->getName().c_str(), curStatus.getCode());
        }
    }

    NStatus beginDestroy() override
    {
        nao::NAO_ECHO("----> [MyTraceAspect] [%s] destroy begin ...", this->getName().c_str());
        return NStatus();
    }

    NVoid finishDestroy(const NStatus& curStatus) override
    {
        nao::NAO_ECHO("----> [MyTraceAspect] [%s] destroy finished, error code is [%d] ...", this->getName().c_str(), curStatus.getCode());
    }
};

#endif   // NAO_MYTRACEASPECT_H