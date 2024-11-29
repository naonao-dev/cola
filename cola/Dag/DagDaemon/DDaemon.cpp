/**
 * @FilePath     : /cola/cola/Dag/DagDaemon/DDaemon.cpp
 * @Description  :
 * @Author       : naonao
 * @Date         : 2024-06-24 18:06:37
 * @Version      : 0.0.1
 * @LastEditors  : naonao
 * @LastEditTime : 2024-11-17 10:41:19
 **/
#include "DDaemon.h"

NAO_NAMESPACE_BEGIN

NStatus DDaemon::init()
{
    NAO_FUNCTION_BEGIN
    timer_.start(interval_, [this] {
        daemonTask(param_);
    }, [this] {
        return modify(param_);
    });
    NAO_FUNCTION_END
}


NStatus DDaemon::destroy()
{
    NAO_FUNCTION_BEGIN
    timer_.stop();
    NAO_FUNCTION_END
}


NMSec DDaemon::getInterval() const
{
    return interval_;
}

NMSec DDaemon::modify(DDaemonParamPtr param) {
    return 0;
}

NAO_NAMESPACE_END
