/**
 * @FilePath     : /cola/tutorial/Common/TestParams/MyConnParam.h
 * @Description  :
 * @Author       : naonao
 * @Date         : 2024-11-27 15:26:05
 * @Version      : 0.0.1
 * @LastEditors  : naonao
 * @LastEditTime : 2024-11-27 15:26:05
 * @Copyright (c) 2024 by G, All Rights Reserved.
 **/
#ifndef NAO_MYCONNPARAM_H
#define NAO_MYCONNPARAM_H

#include "../common.h"

const static char* CONN_PARAM_KEY = "conn";

struct MyConnParam : public nao::DPassedParam
{
    /**
     * 必须实现 clone 函数
     * DAspectParam 和 DDaemonParam 均为 DPassedParam的同名内容
     * 继承自GPassedParam的参数，可以用于在 aspect 或 daemon 之间传递
     * 本例中，MyConnParam既用于DAspectParam(T10)传递，有用于DDaemonParam(T13)传递
     * 故写法为 `: public nao::DPassedParam`
     * @param param
     */
    NVoid clone(nao::DPassedParamPtr param) override
    {
        if (nullptr == param) {
            return;   // 异常判断，理论不可能出现此情况
        }

        auto* ptr = dynamic_cast<MyConnParam*>(param);   // 将传入的参数进行强转，并且赋值到本地
        ip_       = ptr->ip_;
        port_     = ptr->port_;
    }

    std::string ip_{"0.0.0.0"};
    short       port_{0};
};

#endif   // NAO_MYCONNPARAM_H