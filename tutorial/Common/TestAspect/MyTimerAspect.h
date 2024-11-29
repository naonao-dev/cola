/**
 * @FilePath     : /cola/tutorial/Common/TestAspect/MyTimerAspect.h
 * @Description  :
 * @Author       : naonao
 * @Date         : 2024-11-27 15:08:09
 * @Version      : 0.0.1
 * @LastEditors  : naonao
 * @LastEditTime : 2024-11-27 15:08:52
 * @Copyright (c) 2024 by G, All Rights Reserved.
 **/

#ifndef NAO_MYTIMERASPECT_H
#define NAO_MYTIMERASPECT_H

#include <chrono>

#include "../common.h"

class MyTimerAspect : public nao::DAspect
{
public:
    /**
     * 实现计时切面逻辑，记录 run() 方法的执行耗时信息
     */
    NStatus beginRun() override
    {
        start_ts_ = std::chrono::steady_clock::now();
        return NStatus();
    }

    NVoid finishRun(const NStatus& curStatus) override
    {
        std::chrono::duration<double, std::milli> span = std::chrono::steady_clock::now() - start_ts_;
        nao::NAO_ECHO("----> [MyTimerAspect] [%s] time cost is : [%0.2lf] ms", this->getName().c_str(), span.count());
    }

private:
    std::chrono::steady_clock::time_point start_ts_;
};

#endif   // NAO_MYTIMERASPECT_H