/**
 * @FilePath     : /cola/tutorial/Common/TestAspect/MyConnAspect.h
 * @Description  :
 * @Author       : naonao
 * @Date         : 2024-11-27 15:24:10
 * @Version      : 0.0.1
 * @LastEditors  : naonao
 * @LastEditTime : 2024-11-27 15:24:10
 * @Copyright (c) 2024 by G, All Rights Reserved.
 **/
#ifndef NAO_MYCONNASPECT_H
#define NAO_MYCONNASPECT_H

#include "../TestParams/MyConnParam.h"
#include "../common.h"

class MyConnAspect : public nao::DAspect
{
public:
    NStatus beginInit() override
    {
        auto* param = this->getAParam<MyConnParam>();   // 注意，这里是AParam，表示的是切面自己的参数，不是GParam
        if (param) {
            // 如果传入类型不匹配，则返回param值为空
            mockConnect(param->ip_, param->port_);
        }
        return NStatus();
    }

    NVoid finishDestroy(const NStatus& curStatus) override
    {
        auto* param = this->getAParam<MyConnParam>();
        if (param) {
            mockDisconnect(param->ip_, param->port_);
        }
    }

protected:
    /** 模拟连接功能 */
    NVoid mockConnect(const std::string& ip, short port)
    {
        nao::NAO_ECHO("----> [MyConnAspect] [%s : %d] has been connected ...", ip.c_str(), port);
        conn_status_ = true;
    }

    /** 模拟断开连接功能 */
    NVoid mockDisconnect(const std::string& ip, short port)
    {
        if (conn_status_) {
            nao::NAO_ECHO("----> [MyConnAspect] [%s : %d] has been disconnected ...", ip.c_str(), port);
            conn_status_ = false;
        }
        else {
            nao::NAO_ECHO("----> [MyConnAspect] [%s : %d] is already been disconnected ...", ip.c_str(), port);
        }
    }

private:
    bool conn_status_{false};   // 表示连接状态
};

#endif   // NAO_MYCONNASPECT_H