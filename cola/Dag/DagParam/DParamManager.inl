/**
 * @FilePath     : /cola/src/Dag/DagParam/DParamManger.inl
 * @Description  :
 * @Author       : naonao
 * @Date         : 2024-06-24 11:51:49
 * @Version      : 0.0.1
 * @LastEditors  : naonao
 * @LastEditTime : 2024-06-24 11:51:58
 */

#ifndef NAO_DPARAMMANGER_INL
#define NAO_DPARAMMANGER_INL

#include <typeinfo>

#include "DParamManager.h"

NAO_NAMESPACE_BEGIN

template<typename T, c_enable_if_t<std::is_base_of<DParam, T>::value, int>>
NStatus DParamManager::create(const std::string& key, NBool backtrace)
{
    NAO_FUNCTION_BEGIN
    NAO_LOCK_GUARD lock(this->mutex_);
    auto iter = params_map_.find(key);
    if (iter != params_map_.end()) {
        /* 如果是重复创建，则返回ok；非重复创建（类型不同）则返回err */
        auto param = iter->second;
        return (typeid(*param).name() == typeid(T).name()) ?
               NStatus() : NStatus("create [" + key + "] param duplicate");
    }

    T* ptr = NAO_SAFE_MALLOC_NOBJECT(T)
    ((DParamPtr)ptr)->key_              = key;
    ((DParamPtr)ptr)->backtrace_enable_ = backtrace;
    params_map_.insert(std::pair<std::string, T*>(key, ptr));
    NAO_FUNCTION_END
}

template<typename T, c_enable_if_t<std::is_base_of<DParam, T>::value, int>>
T* DParamManager::get(const std::string& key)
{
    const auto& iter = params_map_.find(key);
    if (iter == params_map_.end()) {
        return nullptr;
    }

    /**
     * 实测比 return dynamic_cast<T *>(iter->second); 快很多
     * dynamic_cast<T *> : 当前方案，耗时比约为 10:3
     */
    auto param = iter->second;
    return likely(typeid(T) == typeid(*param)) ? static_cast<T *>(param) : nullptr;
}

NAO_NAMESPACE_END

#endif