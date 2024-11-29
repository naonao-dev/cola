/**
 * @FilePath     : /cola/tutorial/Common/TestAspect/MyTemplateAspect.h
 * @Description  :
 * @Author       : naonao
 * @Date         : 2024-11-27 15:08:09
 * @Version      : 0.0.1
 * @LastEditors  : naonao
 * @LastEditTime : 2024-11-27 15:08:52
 * @Copyright (c) 2024 by G, All Rights Reserved.
 **/
#ifndef NAO_MYTEMPLATEASPECT_H
#define NAO_MYTEMPLATEASPECT_H

#include "../common.h"

template<class... Args>
class MyTemplateAspect : public nao::DTemplateAspect<Args...>
{
public:
    explicit MyTemplateAspect(int age, double score)
    {
        age_   = age;
        score_ = score;
    }

    NStatus beginInit() override
    {
        nao::NAO_ECHO("----> [MyTemplateAspect] [%s] input age is [%d], score is [%lf]", this->getName().c_str(), age_, score_);
        return NStatus();
    }

private:
    int    age_;
    double score_;
};

#endif   // NAO_MYTEMPLATEASPECT_H