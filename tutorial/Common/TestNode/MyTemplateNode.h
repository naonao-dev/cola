/**
 * @FilePath     : /cola/tutorial/Common/TestNode/MyTemplateNode.h
 * @Description  :
 * @Author       : naonao
 * @Date         : 2024-11-27 14:59:34
 * @Version      : 0.0.1
 * @LastEditors  : naonao
 * @LastEditTime : 2024-11-27 15:01:02
 * @Copyright (c) 2024 by G, All Rights Reserved.
 **/
#ifndef NAO_MYTEMPLATENODE_H
#define NAO_MYTEMPLATENODE_H

#include "../common.h"

template<typename... Args>
class MyTemplateNode : public nao::DTemplateNode<Args...>
{
public:
    // 通过两种不同的方式，实现构造函数，可以在pipeline中通过不同的方式 register 进来
    explicit MyTemplateNode(int num, float score)
    {
        num_   = num;
        score_ = score;
    }

    explicit MyTemplateNode(int num)
    {
        num_   = num;
        score_ = 7.0f;   // 默认值
    }

    NStatus run() override
    {
        nao::NAO_ECHO("[MyTemplateNode] num = [%d], score = [%f]", num_, score_);
        return NStatus();
    }

private:
    int   num_;
    float score_;
};

#endif   // NAO_MYTEMPLATENODE_H