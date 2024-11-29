/**
 * @FilePath     : /cola/tutorial/Common/TestCondition/MyCondition.h
 * @Description  :
 * @Author       : naonao
 * @Date         : 2024-11-27 14:33:25
 * @Version      : 0.0.1
 * @LastEditors  : naonao
 * @LastEditTime : 2024-11-27 14:33:25
 * @Copyright (c) 2024 by G, All Rights Reserved.
 **/
#ifndef NAO_MYCONDITION_H
#define NAO_MYCONDITION_H

#include "../common.h"

class MyCondition : public nao::DCondition
{
public:
    /**
     * 表示在condition中，需要执行第几个。
     * 0表示第0个；1表示第1个；以此类推
     * -1 表示最后一个
     * 其中，CIndex is for [int]
     * @return
     */
    NIndex choose() override { return 1; }
};

#endif   // NAO_MYCONDITION_H